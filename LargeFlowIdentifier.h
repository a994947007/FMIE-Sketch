#pragma once
#include "LargeFlowCounter.h"

class LargeFlowIdentifier
{
private:	
	LargeFlowCounter * counter;
public:
	LargeFlowIdentifier(const ULONG row_num, const ULONG col_num, const ULONG MAX_KICKOUT_NUM, const double voteThreshold);
	~LargeFlowIdentifier();
	
	/** ͳ�ƴ���
	*/
	void counting(const FlowID & fid);
};

