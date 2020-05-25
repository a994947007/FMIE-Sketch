#pragma once
#include "common.h"

class H3Function
{
protected:
	typedef struct H3_Matrix {
		ULONG col[32];
	}H3_Matrix;

	//H3哈希矩阵
	H3_Matrix h3_matrix;

	//H3哈希函数的个数
	ULONG H3_HASH_NUM;

public: 
	H3Function();
	~H3Function();
	/** 初始化H3矩阵
	*/
	void init();

	/** 计算fid的哈希值，32位
	* @param fid，流标识符(五元组)
	* @return 计算结果，32位的哈希值
	*/
	ULONG compute(const FlowID &);
};

