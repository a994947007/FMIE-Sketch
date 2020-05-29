#include "pch.h"
#include "MiniFlowFilter.h"

MiniFlowFilter::MiniFlowFilter(const ULONG countersNum, const ULONG countersSize, const ULONG threshold):threshold(threshold)
{
	sketch = new CUSketch(countersNum, countersSize);
}

MiniFlowFilter::~MiniFlowFilter() {
	if (sketch != NULL) {
		delete sketch;
	}
}

bool MiniFlowFilter::filter(const FlowID & fid)
{
	ULONG n = sketch->query(fid);
	if (n >= threshold) {
		return false;
	}
	else {
		sketch->add(fid);
		return true;
	}
}
