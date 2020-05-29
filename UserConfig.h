#pragma once

#include "common.h"
#include <list>
#include <string>

using namespace std;

typedef struct UserConfig{
	list<string> fileList;					//数据包文件列表
	string resultPath;						//结果集保存路径
	ULONG CUSketch_countersNum;				//CUSketch计数器数组个数
	ULONG CUSketch_countersSize;			//CUSketch计数器数组长度
	ULONG MiniFlowFilter_threshold;			//小流过滤器阈值
	ULONG LargeFlowCounter_ROW_NUM;			//大流统计器的Cuckoo表行数
	ULONG LargeFlowCounter_COL_NUM;			//大流统计器的Cuckoo表列数
	ULONG LargeFlowCounter_MAX_KICKOUT_NUM;	//大流统计器的Cuckoo表最多踢除次数
	double LargeFlowCounter_voteThreshold;	//大流统计器判定为大流的反/正阈值
	ULONG readNumlimit;	//限制读取数据包个数
}UserConfig, * pUserConfig;