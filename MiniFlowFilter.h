#pragma once
#include "common.h"
#include "Filter.h"
#include "CuckooFilter.h"

class MiniFlowFilter
{
private:
	Filter * filter;
public:
	/** 小流过滤器构造器
	* @arg1  filter构造器参数
	*/
	MiniFlowFilter(Filter * filter);
	~MiniFlowFilter();
	/** 小流过滤器
	* @arg1 fid 流连接标识符(五元组)
	* @arg2 result 达到阈值的流
	* @return 过滤状态，成功为true，失败为false
	*/
	bool Filtering(const FlowID &,FlowID &);
};

