#pragma once
#include "common.h"
#include "Filter.h"
#include "CuckooFilter.h"

class MiniFlowFilter
{
private:
	Filter * filter;
public:
	/** С��������������
	* @arg1  filter����������
	*/
	MiniFlowFilter(Filter * filter);
	~MiniFlowFilter();
	/** С��������
	* @arg1 fid �����ӱ�ʶ��(��Ԫ��)
	* @return ����״̬���ɹ�Ϊtrue��ʧ��Ϊfalse
	*/
	bool Filtering(const FlowID &);
};

