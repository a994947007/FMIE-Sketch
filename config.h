#pragma once

const static char * logPath = "D:/FMIE-Sketch.log";
const static char * logErrorPath = "D:/FMIE-Sketch-error.log";
const static char * resultPath = "D:/FMIE-Sketch-result.txt";
const static unsigned long HASH_LEN = 4096;

const static ULONG KICK_OUT_NUM = 2;	// ����������߳�����
const static ULONG LARGE_FLOW_REAL_THRESHOLD = 500;	//500�����ݰ����ж�Ϊ������������ʵͳ��
const static ULONG LARGE_FLOW_LFCOUNTER_THRESHOLD = 500;	// ����ͳ�������ж�Ϊ��������ֵ
static ULONG FILTER_MAX_KICKOUT_NUM;