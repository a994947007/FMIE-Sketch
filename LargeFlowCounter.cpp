#include "pch.h"
#include "HashFunctions.h"
#include "LargeFlowCounter.h"

LargeFlowCounter::LargeFlowCounter(const ULONG row_num, const ULONG col_num, const double voteThreshold, const ULONG largeFlowThreshold) :
	ROW_NUM(row_num), COL_NUM(col_num), voteThreshold(voteThreshold), curNum(0), largeFlowThreshold(largeFlowThreshold)
{
	entryTable = new Entry*[row_num];
	fVotes = new ULONG[row_num];
	for (ULONG i = 0; i < row_num; i++) {
		entryTable[i] = new Entry[col_num];
		fVotes[i] = 0;
	}
}

LargeFlowCounter::~LargeFlowCounter()
{
	for (ULONG i = 0; i < ROW_NUM; i++) {
		delete[] entryTable[i];
	}
	delete[] entryTable;
	if (fVotes != NULL) {
		delete fVotes;
		fVotes = NULL;
	}
}

bool LargeFlowCounter::insert(const FlowID& fid)
{
	Pair<ULONG, ULONG> position(ULONG_MAX,ULONG_MAX);
	getFlowPosition(fid, position);
	ULONG index = position.k;
	ULONG col = position.v;

	// ��鵱ǰ���Ƿ��Ѿ����ڣ�����Ѿ����ڣ���ֱ����Ʊ��+1
	if (col != ULONG_MAX) {
		entryTable[index][col].pVote++;
		return true;
	}

	// �����ڣ����жϵ�ǰ���Ƿ���ڿ�λ��
	for (ULONG i = 0; i < COL_NUM; i++) {
		if (entryTable[index][i].isEmpty()) {
			entryTable[index][i].fid = fid;
			entryTable[index][i].pVote = 1;
			return true;
		}
	}

	// ���ܴ��ڿ�λ�ã���Ʊ+1
	fVotes[index] ++;
	
	// ���С����ֵ�Ĳ�
	checkAndReset(index);

	// ����������
	for (ULONG i = 0; i < COL_NUM; i++) {
		if (entryTable[index][i].isEmpty()) {
			entryTable[index][i].fid = fid;
			entryTable[index][i].pVote = 1;
			return true;
		}
	}
	return false;
}

bool LargeFlowCounter::insertAndSetCounter(const FlowID& fid, const ULONG count)
{
	Pair<ULONG, ULONG> position(ULONG_MAX, ULONG_MAX);
	getFlowPosition(fid, position);
	ULONG index = position.k;
	ULONG col = position.v;

	// ��鵱ǰ���Ƿ��Ѿ����ڣ�����Ѿ����ڣ���ֱ����Ʊ��+1
	if (col != ULONG_MAX) {
		entryTable[index][col].pVote += count;
		return true;
	}

	// �����ڣ����жϵ�ǰ���Ƿ���ڿ�λ��
	for (ULONG i = 0; i < COL_NUM; i++) {
		if (entryTable[index][i].isEmpty()) {
			entryTable[index][i].fid = fid;
			entryTable[index][i].pVote = count;
			return true;
		}
	}

	// �����ڿ�λ�ã���Ʊ+1
	fVotes[index] ++;

	// ���С����ֵ�Ĳ�
	checkAndReset(index);

	// ����������
	for (ULONG i = 0; i < COL_NUM; i++) {
		if (entryTable[index][i].isEmpty()) {
			entryTable[index][i].fid = fid;
			entryTable[index][i].pVote = count;
			return true;
		}
	}

	// ����û�м���ɹ�����ѡһ��pVote < count�ļ���
	ULONG j = ULONG_MAX;
	ULONG min = ULONG_MAX;
	for (ULONG i = 0; i < COL_NUM; i++) {
		if (entryTable[index][i].pVote < count && entryTable[index][i].pVote < min) {
			min = entryTable[index][i].pVote;
			j = i;
		}
	}
	if (j != ULONG_MAX) {
		entryTable[index][j].fid = fid;
		entryTable[index][j].pVote = count;
	}
	return false;
}

ULONG LargeFlowCounter::results(const ULONG threshold)
{
	int count = 0;
	for (ULONG i = 0; i < ROW_NUM; i++) {
		for (ULONG j = 0; j < COL_NUM; j++) {
			if (entryTable[i][j].pVote >= largeFlowThreshold) {
				count++;
			}
		}
	}
	return count;
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
	Pair<ULONG, ULONG> position(ULONG_MAX, ULONG_MAX);
	getFlowPosition(fid, position);

	if (position.v != ULONG_MAX) {
		return &entryTable[position.k][position.v];
	}
	return NULL;
}

bool LargeFlowCounter::checkAndReset(ULONG index)
{
	for (ULONG i = 0; i < COL_NUM; i++) {
		if (entryTable[index][i].pVote> 0 && (double)fVotes[index] / entryTable[index][i].pVote > voteThreshold) {
			entryTable[index][i].reset();
		}
	}
	return true;
}

void LargeFlowCounter::getFlowPosition(const FlowID& fid, Pair<ULONG, ULONG> & pair)
{
	UCHAR buf[FID_LEN];
	((FlowID*)&fid)->ToData(buf);
	ULONG index = BOB(buf, FID_LEN) % ROW_NUM;
	pair.k = index;
	for (ULONG i = 0; i < COL_NUM; i++) {
		if (entryTable[index][i].fid == fid) {
			pair.v = i;
			break;
		}
	}
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

void LargeFlowCounter::getLargeFlowNumList(list<FlowID*>& flowList,list<ULONG> & numList)
{
	for (ULONG i = 0; i < ROW_NUM; i++) {
		for (ULONG j = 0; j < COL_NUM; j++) {
			if (entryTable[i][j].pVote >= largeFlowThreshold) {
				flowList.push_back(&entryTable[i][j].fid );
				numList.push_back(entryTable[i][j].pVote);
			}
		}
	}
}

bool LargeFlowCounter::isExists(const FlowID& fid)
{
	Pair<ULONG, ULONG> position(ULONG_MAX, ULONG_MAX);
	getFlowPosition(fid, position);

	if (position.v != ULONG_MAX) {
		return true;
	}
	return false;
}
