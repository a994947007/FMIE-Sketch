#pragma once

#include "common.h"
#include "Filter.h"

class CuckooFilter:public Filter
{
protected:
	typedef struct Entry {
		ULONG sign;
		ULONG counter;
		Entry() :sign(0), counter(0) {
		}
		Entry(const ULONG sign) {
			this->sign = sign;
			this->counter = 0;
		}
		inline Entry operator=(const Entry& e) {
			sign = e.sign;
			counter = e.counter;
			return *this;
		}
		inline bool IsEmpty() {
			return sign == 0 && counter == 0;
		}
		inline bool Reset() {
			sign = 0;
			counter = 0;
			return true;
		}
	}Entry, * PEntry;

public:
	CuckooFilter(const ULONG row1, const ULONG col1, const ULONG row2, const ULONG col2, const ULONG kickout_Num, const ULONG flow_counter_num);
	virtual ~CuckooFilter();

	/**
	*	查找Cuckoo缓存
	*	@arg1	流ID
	*	@arg2	如果达到阈值，则将其取出
	*	@return	是否能找到该流
	*/
	bool Find(const FlowID&,FlowID &);

	/**
	*	插入一条缓
	*	@arg1	流ID
	*	@return	是否插入成功
	*/
	bool Insert(const FlowID&);

protected:
	Entry** filter1, ** filter2;
	ULONG ROW_NUM1, COL_NUM1;
	ULONG ROW_NUM2, COL_NUM2;
	ULONG m_ulCurNum1,m_ulCurNum2;
	ULONG MAX_KICKOUT_NUM;
	ULONG FLOW_COUNTER_THRESHOLD;
};

