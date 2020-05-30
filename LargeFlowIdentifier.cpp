#include "pch.h"
#include "LargeFlowIdentifier.h"
#include "UserConfig.h"

LargeFlowIdentifier::LargeFlowIdentifier(const ULONG row_num, const ULONG col_num, const ULONG MAX_KICKOUT_NUM, const double voteThreshold,const ULONG largeFlowNumThreshold)
{
	counter = new LargeFlowCounter(row_num, col_num, MAX_KICKOUT_NUM, voteThreshold,largeFlowNumThreshold);
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

void LargeFlowIdentifier::getLargeFlowList(list<FlowID*> & flowList)
{
	counter->getLargeFlowList(flowList);
}
