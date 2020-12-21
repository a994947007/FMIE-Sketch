#pragma once
#include "common.h"
#include "H3Function.h"

#include "Filter.h"
class CUSketchFilter :
    public Filter
{
protected:
	//计数器数组个数
	ULONG countersNum;
	//计数器数组的槽个数
	ULONG countersSize;
	//CUSketch数据结构
	ULONG** counters;
	//哈希函数集合
	H3Function* func;

	ULONG FLOW_COUNTER_THRESHOLD;

public:
	CUSketchFilter(const ULONG countersNum, const ULONG countersSize, const ULONG flow_counter_threshold);
	virtual ~CUSketchFilter();

	virtual bool Insert(const FlowID&);

	virtual bool Find(const FlowID&, FlowID&);
};

