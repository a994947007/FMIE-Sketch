#pragma once

#include "common.h"
#include <list>
#include <string>

using namespace std;

typedef struct UserConfig{
	list<string> fileList;					//���ݰ��ļ��б�
	string resultPath;						//���������·��
	ULONG CUSketch_countersNum;				//CUSketch�������������
	ULONG CUSketch_countersSize;			//CUSketch���������鳤��
	ULONG MiniFlowFilter_threshold;			//С����������ֵ
	ULONG LargeFlowCounter_ROW_NUM;			//����ͳ������Cuckoo������
	ULONG LargeFlowCounter_COL_NUM;			//����ͳ������Cuckoo������
	ULONG LargeFlowCounter_MAX_KICKOUT_NUM;	//����ͳ������Cuckoo������߳�����
	double LargeFlowCounter_voteThreshold;	//����ͳ�����ж�Ϊ�����ķ�/����ֵ
	ULONG readNumlimit;	//���ƶ�ȡ���ݰ�����
}UserConfig, * pUserConfig;