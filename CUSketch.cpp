#include "pch.h"
#include "CUSketch.h"

CUSketch::CUSketch(const ULONG countersNum, const ULONG countersSize):countersNum(countersNum),countersSize(countersSize)
{
	counters = new ULONG *[countersNum];
	for (ULONG i = 0; i < countersNum; i++) {
		counters[i] = new ULONG[countersSize];
		memset(counters[i], 0, sizeof(ULONG) * countersSize);
	}
	func = new H3Function[countersNum];
}

CUSketch::~CUSketch()
{
	if (counters != NULL) {
		for (ULONG i = 0; i < countersNum; i++) {
			delete [] counters[i];
			counters[i] = NULL;
		}
		delete[] counters;
	}
	if (func != NULL) delete [] func;
}

void CUSketch::add(const FlowID & fid)
{
	ULONG min = ULONG_MAX;
	ULONG min_i = 0;
	ULONG min_index = 0;
	for (ULONG i = 0; i < countersNum; i++) {
		ULONG index = func[i].compute(fid) % countersSize;
		if (counters[i][index] < min) {
			min_i = i;
			min_index = index;
			min = counters[i][index];
		}
	}
	counters[min_i][min_index]++;
}

ULONG CUSketch::query(const FlowID & fid)
{
	ULONG min = ULONG_MAX;
	ULONG min_i = 0;
	ULONG min_index = 0;
	for (ULONG i = 0; i < countersNum; i++) {
		ULONG index = func[i].compute(fid) % countersSize;
		if (counters[i][index] < min) {
			min_i = i;
			min_index = index;
			min = counters[i][index];
		}
	}
	return counters[min_i][min_index];
}
