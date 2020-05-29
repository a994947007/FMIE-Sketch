#pragma once
#include "common.h"
#include "CUSketch.h"

class MiniFlowFilter
{
private:
	ULONG threshold;
	CUSketch * sketch;
public:
	/** 小流过滤器构造器
	* @param1  CUSketch构造器参数1
	* @param2  CUSketch构造器参数2
	* @param3  阈值
	*/
	MiniFlowFilter(const ULONG, const ULONG, const ULONG);
	~MiniFlowFilter();
	/** 小流过滤器
	* @param fid 流连接标识符(五元组)
	* @return 过滤状态，成功为true，失败为false
	*/
	bool filter(const FlowID &);

	/** 获取小流过滤层中的小流数量
	* @return 小流数量
	*/
	ULONG getMiniFlowNum();
};

