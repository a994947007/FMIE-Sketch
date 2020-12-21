#include "pch.h"
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
	FlowID result;
	bool flag = filter->Find(fid,result);		// 写到这里了
	if (!flag) {
		filter->Insert(fid);
	}
	else {
		if (!result.isEmpty()) {
			return true;
		}
	}
	return false;
}