#pragma once
#include "common.h"
#include "H3Function.h"

class CUSketch
{
protected:
	//�������������
	ULONG countersNum;
	//����������Ĳ۸���
	ULONG countersSize;
	//CUSketch���ݽṹ
	ULONG ** counters;
	//��ϣ��������
	H3Function * func;

public:
	CUSketch(const ULONG countersNum, const ULONG countersSize);
	~CUSketch();

	/** ����һ������CUSketch��
	* @param fid ����ʶ��(��Ԫ��)
	*/
	void insert(const FlowID &);

	/** ��ѯһ���������м����������е���С����
	* @param fid ����ʶ��(��Ԫ��)
	*/
	ULONG query(const FlowID&);
};

