#include "pch.h"
#include "HashFunctions.h"
#include "LargeFlowCounter.h"

LargeFlowCounter::LargeFlowCounter(const ULONG row_num, const ULONG col_num, const ULONG MAX_KICKOUT_NUM, const double voteThreshold, const ULONG largeFlowThreshold) :ROW_NUM(row_num), COL_NUM(col_num), voteThreshold(voteThreshold), curNum(0), MAX_KICKOUT_NUM(MAX_KICKOUT_NUM), largeFlowThreshold(largeFlowThreshold)
{
	entryTable = new Entry*[row_num];
	for (ULONG i = 0; i < row_num; i++) {
		entryTable[i] = new Entry[col_num];
	}
}

LargeFlowCounter::~LargeFlowCounter()
{
	for (ULONG i = 0; i < ROW_NUM; i++) {
		delete[] entryTable[i];
	}
	delete[] entryTable;
}

bool LargeFlowCounter::insert(const FlowID& fid)
{
	UCHAR buf[FID_LEN];
	((FlowID*)&fid)->ToData(buf);
	ULONG sign = OAAT(buf, FID_LEN);
	ULONG row1 = BOB(buf, FID_LEN) % ROW_NUM;
	curNum++;
	
	LargeFlowCounter::Entry ce;
	ce.fid = fid;
	ce.pVote = 1;
	ce.fVote = 0;

	for (ULONG i = 0; i < COL_NUM; i++)
	{
		if (entryTable[row1][i].isEmpty())
		{
			entryTable[row1][i] = ce;
			return true;
		}
	}

	ULONG row2 = row1 ^ sign % ROW_NUM;
	for (ULONG i = 0; i < COL_NUM; i++)
	{
		if (entryTable[row2][i].isEmpty())
		{
			entryTable[row2][i] = ce;
			return true;
		}
	}

	//δ�ҵ��ղۣ�8���۵ķ�Ʊ��+1
	bool isInserted = false;
	for (ULONG i = 0; i < COL_NUM; i++)
	{
		entryTable[row1][i].fVote++;
		entryTable[row2][i].fVote++;
		if (checkAndReset(entryTable[row1][i])) {
			if (!isInserted) {
				entryTable[row1][i] = ce;
				isInserted = true;
			}
			curNum--;
		}
		if (checkAndReset(entryTable[row2][i])) {
			if (!isInserted) {
				entryTable[row2][i] = ce;
				isInserted = true;
			}
			curNum--;
		}
	}
	if (isInserted) {
		return true;
	}
	/*
	ULONG row_max = row1, col_max = 0;
	double voteRate_max = (double)(entryTable[row_max][col_max].fVote + 1) / entryTable[row_max][col_max].pVote;
	//ѡ��/��������
	for (ULONG i = 0; i < COL_NUM; i++) {
		if (voteRate_max < (double)(entryTable[row1][i].fVote + 1) / entryTable[row1][i].pVote) {
			voteRate_max = (double)(entryTable[row1][i].fVote + 1) / entryTable[row1][i].pVote;
			row_max = row1;
			col_max = i;
		}
		if (voteRate_max < (double)(entryTable[row2][i].fVote + 1) / entryTable[row2][i].pVote) {
			voteRate_max = (double)(entryTable[row2][i].fVote + 1) / entryTable[row2][i].pVote;
			row_max = row2;
			col_max = i;
		}
	}
	*/
	ULONG row = rand() % 2 == 0 ? row1 : row2;	//���ѡһ��Ͱ
	ULONG ulKickNum = 0;
	while (ulKickNum++ < MAX_KICKOUT_NUM)
	{
		ULONG col = rand() % COL_NUM;		//���ѡ��һ����
		Entry tmp = entryTable[row][col];
		entryTable[row][col] = ce;
		ce = tmp;

		UCHAR buf[FID_LEN];
		((FlowID*)&ce.fid)->ToData(buf);
		ULONG sign = OAAT(buf, FID_LEN);

		row ^= sign % ROW_NUM;
		for (ULONG i = 0; i < COL_NUM; i++)
		{
			if (entryTable[row][i].isEmpty())
			{
				entryTable[row][i] = ce;
				return true;
			}
		}
	}
	curNum--;	//��һ�����
	return false;
}

ULONG LargeFlowCounter::results(const ULONG threshold)
{
	return 0;
}

bool LargeFlowCounter::incr(const FlowID& fid)
{
	Entry* entry = query(fid);
	if (entry == NULL) {	//�Ҳ�����Ӧ�����������ʧ�ܱ��
		return false;
	}
	entry->pVote++;	//��Ʊ��+1
	return true;
}

LargeFlowCounter::Entry* LargeFlowCounter::query(const FlowID & fid)
{
	UCHAR buf[FID_LEN];
	((FlowID*)&fid)->ToData(buf);
	ULONG sign = OAAT(buf, FID_LEN);
	ULONG row1 = BOB(buf, FID_LEN) % ROW_NUM;
	for (ULONG i = 0; i < COL_NUM; i++) {
		if (entryTable[row1][i].fid == fid) {
			return &entryTable[row1][i];
		}
	}
	ULONG row2 = row1 ^ sign % ROW_NUM;
	for (ULONG i = 0; i < COL_NUM; i++) {
		if (entryTable[row2][i].fid == fid) {
			return &entryTable[row2][i];
		}
	}
	return NULL;
}

bool LargeFlowCounter::checkAndReset(Entry & entry)
{
	if ((double)entry.fVote / entry.pVote > voteThreshold) {
		entry.reset();
		return true;
	}
	return false;
}

void LargeFlowCounter::getLargeFlowList(list<FlowID *> & flowList)
{
	for (ULONG i = 0; i < ROW_NUM; i++) {
		for (ULONG j = 0; j < COL_NUM; j++) {
			if (entryTable[i][j].pVote >= largeFlowThreshold) {
				flowList.push_back(&entryTable[i][j].fid);
			}
		}
	}
}
