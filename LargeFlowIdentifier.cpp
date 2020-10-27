#include "pch.h"
#include "LargeFlowIdentifier.h"
#include "UserConfig.h"

LargeFlowIdentifier::LargeFlowIdentifier(LFCounter * lfCounter)
{
	counter = lfCounter;
}

LargeFlowIdentifier::~LargeFlowIdentifier()
{
	if (counter != NULL) {
		delete counter;
		counter = 0;
	}
}

void LargeFlowIdentifier::insert(const FlowID & fid)
{
	counter->insert(fid);
}

bool LargeFlowIdentifier::insertAndSetCounter(const FlowID& fid, const ULONG count) {
	return counter->insertAndSetCounter(fid, count);
}

bool LargeFlowIdentifier::incr(const FlowID& fid)
{
	return counter->incr(fid);
}

void LargeFlowIdentifier::getLargeFlowList(list<FlowID*> & flowList)
{
	counter->getLargeFlowList(flowList);
}

void LargeFlowIdentifier::getLargeFlowNumList(list<FlowID*>& flowList, list<ULONG>& numList)
{
	counter->getLargeFlowNumList(flowList, numList);
}
