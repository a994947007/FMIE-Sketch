#pragma once

const static char * logPath = "D:/FMIE-Sketch.log";
const static char * logErrorPath = "D:/FMIE-Sketch-error.log";
const static char * resultPath = "D:/FMIE-Sketch-result.txt";
const static unsigned long HASH_LEN = 4096;

const static ULONG KICK_OUT_NUM = 2;	// 过滤器最大踢除次数
const static ULONG LARGE_FLOW_REAL_THRESHOLD = 500;	//500个数据包则被判断为大流，用于真实统计
const static ULONG LARGE_FLOW_LFCOUNTER_THRESHOLD = 500;	// 大流统计器中判定为大流的阈值
static ULONG FILTER_MAX_KICKOUT_NUM;