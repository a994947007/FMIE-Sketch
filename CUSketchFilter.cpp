#include "pch.h"
#include "CUSketchFilter.h"
#include "config.h"

CUSketchFilter::CUSketchFilter(const ULONG countersNum, const ULONG countersSize, const ULONG flow_counter_threshold):FLOW_COUNTER_THRESHOLD(flow_counter_threshold)
{
	counters = new ULONG * [countersNum];
	for (ULONG i = 0; i < countersNum; i++) {
		counters[i] = new ULONG[countersSize];
		memset(counters[i], 0, sizeof(ULONG) * countersSize);
	}
	func = new H3Function[countersNum];
}

CUSketchFilter::~CUSketchFilter()
{
	if (counters != NULL) {
		for (ULONG i = 0; i < countersNum; i++) {
			delete[] counters[i];
			counters[i] = NULL;
		}
		delete[] counters;
	}
	if (func != NULL) delete[] func;
}

bool CUSketchFilter::Insert(const FlowID& fid)
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
	return true;
}

bool CUSketchFilter::Find(const FlowID & fid, FlowID & result)
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
	if (min == 0) {
		return false;
	}
	if (min >= FLOW_COUNTER_THRESHOLD) {
		result = fid;
	//	counters[min_i][min_index] -= FLOW_COUNTER_THRESHOLD;
	}
	return true;
}
