#pragma once
#include "FlowTable.h"

class RealCounter
{
private:
	FlowTable * flowTable;

public:
	RealCounter(const ULONG);
	virtual ~RealCounter();

	void add(const FlowID&);
	void getLargeFlowList(list<FlowID*> &);
	void getLargeFlowList(list<FlowID*> &,list<ULONG> &);
	ULONG getFNum(const FlowID&);

	ULONG getFlowNum();
	ULONG getLargeFlowNum();
	ULONG getLargeFlowPacketNum();
};

