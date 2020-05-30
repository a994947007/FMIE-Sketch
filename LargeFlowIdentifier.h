#pragma once
#include "LargeFlowCounter.h"

class LargeFlowIdentifier
{
private:	
	LargeFlowCounter * counter;
public:
	/**	����ͳ����Ĭ�Ϲ�����
	* @param1 cuckoo�������
	* @param2 cuckoo�������
	* @param3 ����߳�����
	* @param4 �ж�Ϊ��������ֵ
	*/
	LargeFlowIdentifier(const ULONG row_num, const ULONG col_num, const ULONG MAX_KICKOUT_NUM, const double voteThreshold,const ULONG largeFlowNumThreshold);
	~LargeFlowIdentifier();
	
	/** ͳ�ƴ���
	*/
	void counting(const FlowID & fid);

	/** ��ȡ��������
	* @return ��������
	*/
	void getLargeFlowList(list<FlowID*> &);
};

