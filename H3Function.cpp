#include "pch.h"
#include "HashFunctions.h"
#include "H3Function.h"

H3Function::H3Function()
{
	init();
}

H3Function::~H3Function()
{
}

void H3Function::init()
{
	for (int j = 0; j < 32; j++) {
		ULONG r = 0;
		for (int q = 0; q < 4; q++) {
			ULONG randf = rand() % (0xff + 1);
			r += randf;
			if (q == 3) {
				break;
			}
			r <<= 8;
		}
		this->h3_matrix.col[j] = r;
	}
}


ULONG H3Function::compute(const FlowID & fid)
{
	UCHAR buf[FID_LEN];
	((FlowID*)&fid)->ToData(buf);
	ULONG h3_hash = BOB(buf, FID_LEN);
	ULONG matrix_col = 0xffffffff;
	ULONG col_index = 0x00000001;
	for (int i = 0; i < 32; i++, col_index <<= 1) {
		if ((h3_hash & col_index) == col_index) {
			matrix_col ^= h3_matrix.col[i];
		}
	}
	return matrix_col;
}
