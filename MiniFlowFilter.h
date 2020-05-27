#pragma once
#include "common.h"
#include "CUSketch.h"

class MiniFlowFilter
{
private:
	ULONG threshold;
	CUSketch * sketch;
public:
	/** С��������������
	* @param1  CUSketch����������1
	* @param2  CUSketch����������2
	* @param3  ��ֵ
	* @param4  ����߳�����
	*/
	MiniFlowFilter(const ULONG, const ULONG, const ULONG);
	~MiniFlowFilter();
	/** С��������
	* @param fid �����ӱ�ʶ��(��Ԫ��)
	* @return ����״̬���ɹ�Ϊtrue��ʧ��Ϊfalse
	*/
	bool filter(FlowID &);
};

