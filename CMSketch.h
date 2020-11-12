#pragma once

#include "common.h"

class CMSketch
{
protected:
	typedef struct H3_Matrix {
		ULONG col[32];
	}H3_Matrix;

protected:
	H3_Matrix * h3_matrix;

private:
	ULONG sketchCount;	// Sketch�ĸ���
	ULONG sketchSize;	// ÿ��sketch�Ĵ�С
	ULONG ** sketchs;	// sketch����

protected:
	bool init_h3_matrix();
	ULONG h3_function(const ULONG h3_row, const ULONG h3_hash, const int matrix_index);

public:
	CMSketch(ULONG sketchCount, ULONG sketchSize);
	~CMSketch();
	ULONG getFlowNum(FlowID * fid);
	bool insertFlow(FlowID * fid);
};

