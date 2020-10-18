#pragma once

#include "common.h"

class Filter {
public:
	virtual ~Filter() {}

	/**
	*	查找Cuckoo缓存
	*	@arg1	流ID
	*	@arg2	如果达到阈值，则将其取出
	*	@return	是否能找到该流
	*/
	virtual bool Find(const FlowID&, FlowID&) = 0;

	/**
	*	插入一条缓
	*	@arg1	流ID
	*	@return	是否插入成功
	*/
	virtual bool Insert(const FlowID&) = 0;
};