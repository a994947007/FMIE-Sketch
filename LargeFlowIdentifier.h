#pragma once
#include "LargeFlowCounter.h"

class LargeFlowIdentifier
{
private:	
	LFCounter * counter;
public:
	/**	大流统计器默认构造器
	*/
	LargeFlowIdentifier(LFCounter * lfCounter);
	~LargeFlowIdentifier();
	
	/** 统计大流
	*/
	void insert(const FlowID & fid);


	/**
	*	尝试增加正票
	*/
	bool incr(const FlowID& fid);

	/** 获取大流集合
	* @return 大流集合
	*/
	void getLargeFlowList(list<FlowID*> &);

	void getLargeFlowNumList(list<FlowID*>&, list<ULONG>&);
};

