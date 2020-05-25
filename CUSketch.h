#pragma once
#include "common.h"
#include "H3Function.h"

class CUSketch
{
protected:
	//计数器数组个数
	ULONG countersNum;
	//计数器数组的槽个数
	ULONG countersSize;
	//CUSketch数据结构
	ULONG ** counters;
	//哈希函数集合
	H3Function * func;

public:
	CUSketch(const ULONG countersNum, const ULONG countersSize);
	~CUSketch();

	/** 插入一条流到CUSketch中
	* @param fid 流标识符(五元组)
	*/
	void insert(const FlowID &);

	/** 查询一条流在所有计数器数组中的最小计数
	* @param fid 流标识符(五元组)
	*/
	ULONG query(const FlowID&);
};

