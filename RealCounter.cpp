#include "pch.h"
#include "RealCounter.h"
#include "config.h"
#include "UserConfig.h"

RealCounter::RealCounter(const ULONG largeFlowNumThreshold)
{
	flowTable = new FlowTable(HASH_LEN, largeFlowNumThreshold);
}

RealCounter::~RealCounter()
{
	if (flowTable != NULL) {
		delete flowTable;
	}
}

void RealCounter::add(const FlowID& key)
{
	flowTable->add(key);
}

void RealCounter::getLargeFlowList(list<FlowID*> & flowList)
{
	flowTable->getLargeFlowList(flowList);
}

void RealCounter::getLargeFlowList(list<FlowID*>& largeFlowList, list<ULONG> & flowNumList)
{
	flowTable->getLargeFlowList(largeFlowList, flowNumList);
}

ULONG RealCounter::getFNum(const FlowID& key)
{
	return flowTable->getFNum(key);
}

ULONG RealCounter::getFlowNum()
{
	return flowTable->getFlowNum();
}

ULONG RealCounter::getLargeFlowNum()
{
	return flowTable->getLargeFlowNum();
}

ULONG RealCounter::getLargeFlowPacketNum()
{
	return flowTable->getLargeFlowPacketNum();
}


