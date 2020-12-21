#pragma once
#include "common.h"
#include "H3Function.h"

#include "Filter.h"
class CUSketchFilter :
    public Filter
{
protected:
	//�������������
	ULONG countersNum;
	//����������Ĳ۸���
	ULONG countersSize;
	//CUSketch���ݽṹ
	ULONG** counters;
	//��ϣ��������
	H3Function* func;

	ULONG FLOW_COUNTER_THRESHOLD;

public:
	CUSketchFilter(const ULONG countersNum, const ULONG countersSize, const ULONG flow_counter_threshold);
	virtual ~CUSketchFilter();

	virtual bool Insert(const FlowID&);

	virtual bool Find(const FlowID&, FlowID&);
};

