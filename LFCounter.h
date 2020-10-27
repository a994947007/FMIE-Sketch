#pragma once


#include "common.h"
#include <list>

class LFCounter {
public:
	virtual ~LFCounter() {}
	/** 添加一条流到当前统计器中
	* @param1 流标识符，五元组
	* @return 如果没有踢出多余的记录为true，否则为false
	*/
	virtual bool insert(const FlowID & fid) = 0;

	virtual bool insertAndSetCounter(const FlowID & fid,const ULONG count) = 0;
	
	/** 统计所有大量的数量
	* @param1 满足判定为大流的阈值
	* @return 大流数量
	*/
	virtual ULONG results(const ULONG threshold) = 0;

	/**	将fid在对应表中的计数+1
	* @param1 fid，五元组
	* @return 有可能在表中不存在与fid对应的项，增加失败返回false，否则增加成功为true
	*/
	virtual bool incr(const FlowID & fid) = 0;

	/** 获取当前大流统计器中的所有大流
	* @param1 大流容器
	*/
	virtual void getLargeFlowList(list<FlowID*>& container) = 0;

	virtual	void getLargeFlowNumList(list<FlowID*>&, list<ULONG>&) = 0;

	virtual bool isExists(const FlowID & fid) = 0;
};