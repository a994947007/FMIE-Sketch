#pragma once


#include "common.h"
#include <list>

class LFCounter {
public:
	virtual ~LFCounter() {}
	/** ���һ��������ǰͳ������
	* @param1 ����ʶ������Ԫ��
	* @return ���û���߳�����ļ�¼Ϊtrue������Ϊfalse
	*/
	virtual bool insert(const FlowID & fid) = 0;

	virtual bool insertAndSetCounter(const FlowID & fid,const ULONG count) = 0;
	
	/** ͳ�����д���������
	* @param1 �����ж�Ϊ��������ֵ
	* @return ��������
	*/
	virtual ULONG results(const ULONG threshold) = 0;

	/**	��fid�ڶ�Ӧ���еļ���+1
	* @param1 fid����Ԫ��
	* @return �п����ڱ��в�������fid��Ӧ�������ʧ�ܷ���false���������ӳɹ�Ϊtrue
	*/
	virtual bool incr(const FlowID & fid) = 0;

	/** ��ȡ��ǰ����ͳ�����е����д���
	* @param1 ��������
	*/
	virtual void getLargeFlowList(list<FlowID*>& container) = 0;

	virtual	void getLargeFlowNumList(list<FlowID*>&, list<ULONG>&) = 0;

	virtual bool isExists(const FlowID & fid) = 0;
};