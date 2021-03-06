#include "pch.h"
#include "CMSketch.h"
#include "Log.h"
#include "HashFunctions.h"
#include "config.h"

CMSketch::CMSketch(ULONG sketchCount, ULONG sketchSize):sketchCount(sketchCount),sketchSize(sketchSize)
{
	sketchs = new ULONG*[sketchCount];
	for (int i = 0; i < sketchCount; i++)
	{
		sketchs[i] = new ULONG[sketchSize];
		memset(sketchs[i], 0, sketchSize * sizeof(ULONG));
	}
	srand((unsigned)time(NULL));
	this->init_h3_matrix();
}

CMSketch::~CMSketch() {
	if (sketchs != NULL) {
		for (int i = 0; i < sketchCount; i++)
		{
			delete [] sketchs[i];
		}
		delete sketchs;
	}
}

bool CMSketch::init_h3_matrix() {
	this->h3_matrix = new H3_Matrix[sketchCount];
	for (int i = 0; i < sketchCount; i++) {
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
			this->h3_matrix[i].col[j] = r;
		}
	}
	return true;
}

ULONG CMSketch::getFlowNum(const FlowID & fid)
{
	UCHAR buf[FID_LEN];
	((FlowID*)&fid)->ToData(buf);
	ULONG h3_hash = BOB(buf, FID_LEN);
	ULONG min = ULONG_MAX;
	Pair<ULONG, ULONG> position(0, 0);
	for (int i = 0; i < sketchCount; i++) {		// 统计时获取最小计数
		ULONG index = h3_function(32, h3_hash, i) % sketchSize;
		if (sketchs[i][index] < ULONG_MAX) {
			min = sketchs[i][index];
			position.k = i;
			position.v = index;
		}
	}
	return sketchs[position.k][position.v];
}

bool CMSketch::insertFlow(const FlowID & fid)
{
	UCHAR buf[FID_LEN];
	((FlowID*)&fid)->ToData(buf);
	ULONG h3_hash = BOB(buf, FID_LEN);
	ULONG min = ULONG_MAX;
	for (int i = 0; i < sketchCount; i++) {		// 插入时，所有计数都加1
		ULONG index = h3_function(32, h3_hash, i) % sketchSize;
		sketchs[i][index]++;
	}
	return false;
}


ULONG CMSketch::h3_function(const ULONG h3_row, const ULONG h3_hash, const int matrix_index) {
	ULONG matrix_col = 0xffffffff;
	ULONG col_index = 0x00000001;
	for (int i = 0; i < 32; i++, col_index <<= 1) {
		if ((h3_hash & col_index) == col_index) {
			matrix_col ^= h3_matrix[matrix_index].col[i];
		}
	}
	matrix_col >>= (32 - h3_row);
	return matrix_col;
}
