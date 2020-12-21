#include "pch.h"
#include "config.h"
#include "MiniFlowFilter.h"

MiniFlowFilter::MiniFlowFilter(Filter * filter)
{
	this->filter = filter;
}

MiniFlowFilter::~MiniFlowFilter() {
	if (this->filter != NULL) {
		delete this->filter;
		this->filter = NULL;
	}
}

bool MiniFlowFilter::Filtering(const FlowID & fid)
{
	 ULONG flowNum = filter->Find(fid);	
	if (flowNum > FLOW_COUNTER_THRESHOLD) {
		return false;
	}
	else {
		filter->Insert(fid);
		return true;
	}
}