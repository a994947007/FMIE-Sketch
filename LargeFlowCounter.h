#pragma once
#include "Log.h"
#include "common.h"
class LargeFlowCounter
{
private:
	typedef struct Entry {
		FlowID fid;		//fid
		ULONG pVote;	//正票数
		ULONG fVote;	//反票数
		inline Entry(FlowID & fid):fid(fid), pVote(0), fVote(0){}

		//拷贝构造函数
		Entry(const Entry & entry) {
			fid = entry.fid;
			pVote = entry.pVote;
			fVote = entry.fVote;
		}

		//重载=运算符，拷贝函数
		inline Entry & operator=(const Entry& entry) {
			fid = entry.fid;
			pVote = entry.pVote;
			fVote = entry.fVote;
			return *this;
		}

		//判断当前项是否为空
		inline bool isEmpty() {
			return fid.isEmpty() && pVote == 0 && fVote == 0;
		}

		//清空当前项
		inline bool reset() {
			fid.reset();
			pVote = 0;
			fVote = 0;
		}
	}Entry;

	//统计表
	Entry ** entryTable;
	//行（桶）数，列（槽）数
	ULONG ROW_NUM, COL_NUM;	
	//已统计项数
	ULONG curNum;
	//反/正阈值，用于踢除
	double voteThreshold;

public:
	/** 添加一条流到当前统计器中
	* @param1 流标识符，五元组
	*/
	bool add(const FlowID &);

	/** 统计所有大量的数量
	* @return 大流数量
	*/
	ULONG results();
};

