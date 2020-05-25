#pragma once
#include "common.h"

class H3Function
{
protected:
	typedef struct H3_Matrix {
		ULONG col[32];
	}H3_Matrix;

	//H3��ϣ����
	H3_Matrix h3_matrix;

	//H3��ϣ�����ĸ���
	ULONG H3_HASH_NUM;

public: 
	H3Function();
	~H3Function();
	/** ��ʼ��H3����
	*/
	void init();

	/** ����fid�Ĺ�ϣֵ��32λ
	* @param fid������ʶ��(��Ԫ��)
	* @return ��������32λ�Ĺ�ϣֵ
	*/
	ULONG compute(const FlowID &);
};

