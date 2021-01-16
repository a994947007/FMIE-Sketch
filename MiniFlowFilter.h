#pragma once
#include "common.h"
#include "Filter.h"
#include "CuckooFilter.h"

class MiniFlowFilter
{
private:
	ULONG MIN_FLOW_THRESHOLD;
	Filter * filter;
public:
	/** С��������������
	* @arg1  filter����������
	*/
	MiniFlowFilter(Filter * filter);
	MiniFlowFilter(Filter* filter, ULONG minFlowThreshold);
	~MiniFlowFilter();
	/** С��������
	* @arg1 fid �����ӱ�ʶ��(��Ԫ��)
	* @return ����״̬���ɹ�Ϊtrue��ʧ��Ϊfalse
	*/
	bool Filtering(const FlowID &);
};

