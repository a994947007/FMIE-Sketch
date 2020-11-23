#pragma once

#include "common.h"
#include <list>
#include <string>

using namespace std;

typedef struct UserConfig{
	list<string> fileList;					//数据包文件列表
	string resultPath;						//结果集保存路径
	ULONG PACKET_NUM_LIMIT;				
	ULONG FILTER_PKT_NUM;				
	ULONG IDENTIFY_PKT_NUM;		
	ULONG CUCKOO_ROW1;			
	ULONG CUCKOO_COL1;			
	ULONG CUCKOO_ROW2;		
	ULONG CUCKOO_COL2;	
	ULONG FILTER_SHRESHOLD;	
	ULONG IDENTIFY_ROW;		
	ULONG IDENTIFY_COL;		
	ULONG SKETCH_COUNT;
	ULONG SKETCH_SIZE;
	double IDENTIFY_THRESHOLD;
}UserConfig, * pUserConfig;