#pragma once
#include "LargeFlowCounter.h"

class LargeFlowIdentifier
{
private:	
	LargeFlowCounter * counter;
public:
	/**	大流统计器默认构造器
	* @param1 cuckoo表的行数
	* @param2 cuckoo表的列数
	* @param3 最多踢除次数
	* @param4 判定为大流的阈值
	*/
	LargeFlowIdentifier(const ULONG row_num, const ULONG col_num, const ULONG MAX_KICKOUT_NUM, const double voteThreshold);
	~LargeFlowIdentifier();
	
	/** 统计大流
	*/
	void counting(const FlowID & fid);

	/** 获取大流统计器中的小流数量
	* @return 小流数量
	*/
	ULONG getMiniFlowNum();

	/** 获取大流统计器中的大流数量
	* @return 大流数量
	*/
	ULONG getLargeFlowNum();
};

