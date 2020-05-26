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

bool MiniFlowFilter::filter(FlowID & fid)
{
	ULONG n = sketch->query(fid);
	if (n >= threshold) {
		return true;
	}
	else {
		sketch->insert(fid);
		return false;
	}
}
