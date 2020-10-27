#pragma once
#include "LargeFlowCounter.h"

class LargeFlowIdentifier
{
private:	
	LFCounter * counter;
public:
	/**	����ͳ����Ĭ�Ϲ�����
	*/
	LargeFlowIdentifier(LFCounter * lfCounter);
	~LargeFlowIdentifier();
	
	/** ͳ�ƴ���
	*/
	void insert(const FlowID & fid);

	bool insertAndSetCounter(const FlowID& fid, const ULONG count);


	/**
	*	����������Ʊ
	*/
	bool incr(const FlowID& fid);

	/** ��ȡ��������
	* @return ��������
	*/
	void getLargeFlowList(list<FlowID*> &);

	void getLargeFlowNumList(list<FlowID*>&, list<ULONG>&);
};

