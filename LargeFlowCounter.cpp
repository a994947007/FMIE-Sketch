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

	// 检查当前流是否已经存在，如果已经存在，则直接正票数+1
	if (col != ULONG_MAX) {
		entryTable[index][col].pVote++;
		return true;
	}

	// 不存在，则判断当前表是否存在空位置
	for (ULONG i = 0; i < COL_NUM; i++) {
		if (entryTable[index][i].isEmpty()) {
			entryTable[index][i].fid = fid;
			entryTable[index][i].pVote = 1;
			return true;
		}
	}

	// 不能存在空位置，反票+1
	fVotes[index] ++;
	
	// 清空小于阈值的槽
	checkAndReset(index);

	// 将新流加入
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

	// 检查当前流是否已经存在，如果已经存在，则直接正票数+1
	if (col != ULONG_MAX) {
		entryTable[index][col].pVote += count;
		return true;
	}

	// 不存在，则判断当前表是否存在空位置
	for (ULONG i = 0; i < COL_NUM; i++) {
		if (entryTable[index][i].isEmpty()) {
			entryTable[index][i].fid = fid;
			entryTable[index][i].pVote = count;
			return true;
		}
	}

	// 不存在空位置，反票+1
	fVotes[index] ++;

	// 清空小于阈值的槽
	checkAndReset(index);

	// 将新流加入
	for (ULONG i = 0; i < COL_NUM; i++) {
		if (entryTable[index][i].isEmpty()) {
			entryTable[index][i].fid = fid;
			entryTable[index][i].pVote = count;
			return true;
		}
	}

	// 新流没有加入成功，则选一个pVote < count的加入
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
	if (entry == NULL) {	//找不到对应的项，返回增加失败标记
		return false;
	}
	entry->pVote++;	//正票数+1
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
