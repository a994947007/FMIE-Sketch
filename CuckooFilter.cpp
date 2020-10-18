#include "pch.h"
#include "CuckooFilter.h"
#include "HashFunctions.h"

CuckooFilter::CuckooFilter(const ULONG row1, const ULONG col1, const ULONG row2, const ULONG col2, const ULONG kickoutNum,const ULONG flow_counter_threshold)
	:ROW_NUM1(row1), COL_NUM1(col1), ROW_NUM2(row2), COL_NUM2(col2),
	m_ulCurNum1(0),m_ulCurNum2(0),
	MAX_KICKOUT_NUM(kickoutNum),
	FLOW_COUNTER_THRESHOLD(flow_counter_threshold)
{
	filter1 = new PEntry[ROW_NUM1];
	for (ULONG i = 0; i < ROW_NUM1; i++) {
		filter1[i] = new Entry[COL_NUM1];
	}
	filter2 = new PEntry[ROW_NUM2];
	for (ULONG i = 0; i < ROW_NUM2; i++) {
		filter2[i] = new Entry[COL_NUM2];
	}
}

CuckooFilter::~CuckooFilter()
{
	for (ULONG i = 0; i < ROW_NUM1; i++) {
		delete [] filter1[i];
	}
	delete[] filter1;
	for (ULONG i = 0; i < ROW_NUM2; i++) {
		delete [] filter2[i];
	}
	delete[] filter2;
}

bool CuckooFilter::Find(const FlowID&fid,FlowID & result)
{
	UCHAR buf[FID_LEN];
	((FlowID*)&fid)->ToData(buf);

	ULONG sign = OAAT(buf, FID_LEN);
	ULONG row1 = BOB(buf, FID_LEN) % ROW_NUM1;

	for (ULONG i = 0; i < COL_NUM2; i++) {
		if (sign == filter1[row1][i].sign) {
			filter1[row1][i].counter++;
			if (filter1[row1][i].counter >= FLOW_COUNTER_THRESHOLD) {
				result = fid;
				filter1[row1][i].Reset();
			}
			return true;
		}
	}

	ULONG row2 = row1 ^ sign % ROW_NUM2;
	for (ULONG i = 0; i < COL_NUM2; i++) {
		if (sign == filter2[row2][i].sign) {
			filter2[row2][i].counter++;
			if (filter2[row2][i].counter >= FLOW_COUNTER_THRESHOLD) {
				result = fid;
				filter2[row2][i].Reset();
			}
			return true;
		}
	}

	return false;
}

bool CuckooFilter::Insert(const FlowID & fid)
{
	UCHAR buf[FID_LEN];
	((FlowID*)&fid)->ToData(buf);
	ULONG sign = OAAT(buf, FID_LEN);
	ULONG row1 = BOB(buf, FID_LEN) % ROW_NUM1;

	Entry e(sign);

	for (ULONG i = 0; i < COL_NUM1; i++) {
		if (filter1[row1][i].IsEmpty()) {
			filter1[row1][i] = e;
			return true;
		}
	}

	ULONG row2 = row1 ^ sign % ROW_NUM2;
	for (ULONG i = 0; i < COL_NUM2; i++) {
		if (filter2[row2][i].IsEmpty()) {
			filter2[row2][i] = e;
			return true;
		}
	}

	ULONG row = row1;
	ULONG col = COL_NUM1;
	ULONG ulKickNum = 0;
	Entry ** filter = filter1;
	ULONG counter = filter[row][0].counter;

	while (ulKickNum++ < MAX_KICKOUT_NUM) {
		bool flag = false;
		ULONG minIndex = 0;
		for (ULONG i = 0; i < col; i++) {
			if (filter[row][i].IsEmpty()) {
				filter[row][i] = e;
				return true;
			}
			if (filter[row][i].counter < counter) {
				minIndex = i;
				counter = filter[row][i].counter;
				flag = true;
			}
		}
		if (!flag) {
			break;
		}
		Entry tmp = filter[row][minIndex];
		filter[row][minIndex] = e;
		e = tmp;
		if (ulKickNum % 2 == 1) {
			row = e.sign % ROW_NUM2;
			col = COL_NUM2;
			filter = filter2;
		}
		else {
			row = e.sign % ROW_NUM1;
			col = COL_NUM1;
			filter = filter1;
		}
	}

	// ¶ªÁËÒ»¸ö
	return false;
}


