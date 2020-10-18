#pragma once

#include "common.h"

class Filter {
public:
	virtual ~Filter() {}

	/**
	*	����Cuckoo����
	*	@arg1	��ID
	*	@arg2	����ﵽ��ֵ������ȡ��
	*	@return	�Ƿ����ҵ�����
	*/
	virtual bool Find(const FlowID&, FlowID&) = 0;

	/**
	*	����һ����
	*	@arg1	��ID
	*	@return	�Ƿ����ɹ�
	*/
	virtual bool Insert(const FlowID&) = 0;
};