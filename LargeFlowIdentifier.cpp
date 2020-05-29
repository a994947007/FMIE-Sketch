#include "pch.h"
#include "LargeFlowIdentifier.h"

LargeFlowIdentifier::LargeFlowIdentifier(const ULONG row_num, const ULONG col_num, const ULONG MAX_KICKOUT_NUM, const double voteThreshold)
{
	counter = new LargeFlowCounter(row_num, col_num, MAX_KICKOUT_NUM, voteThreshold);
}

LargeFlowIdentifier::~LargeFlowIdentifier()
{
	if (counter != NULL) {
		delete counter;
	}
}

void LargeFlowIdentifier::counting(const FlowID & fid)
{
	if (!counter->incr(fid)) {
		counter->insert(fid);
	}
}