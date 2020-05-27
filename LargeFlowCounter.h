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
		Entry():pVote(0),fVote(0) {}

		Entry(const Entry& entry) {
			fid = entry.fid;
			pVote = entry.pVote;
			fVote = entry.fVote;
		}

		Entry(const FlowID & fid):fid(fid), pVote(0), fVote(0){}

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
	//最多踢除次数
	ULONG MAX_KICKOUT_NUM;

public:
	LargeFlowCounter(const ULONG row_num, const ULONG col_num,const ULONG MAX_KICKOUT_NUM, const double voteThreshold);
	/** 添加一条流到当前统计器中
	* @param1 流标识符，五元组
	* @return 如果没有踢出多余的记录为true，否则为false
	*/
	bool insert(const FlowID &);

	/** 统计所有大量的数量
	* @param1 满足判定为大流的阈值
	* @return 大流数量
	*/
	ULONG results(const ULONG threshold);

	/**	将fid在对应表中的计数+1
	* @param1 fid，五元组
	* @return 有可能在表中不存在与fid对应的项，增加失败返回false，否则增加成功为true
	*/
	bool incr(const FlowID& fid);

private:
	/** 根据fid在表中查找记录
	* @param1 fid，五元组
	* @return 查到的记录
	*/
	Entry* query(const FlowID& fid);
	/** 判断是否到达阈值，到达则重置
	* @return 到达重置返回true，否则返回false
	*/
	bool checkAndReset(Entry & entry);
};

