#pragma once
#include "Log.h"
#include "common.h"
#include "LFCounter.h"
#include "HashFunctions.h"
#include <list>

class LargeFlowCounter : public LFCounter
{
private:
	typedef struct Entry {
		//FlowID fid;		//fid
		ULONG sign;		// ǩ��
		ULONG pVote;	//��Ʊ��
		//ULONG fVote;	//��Ʊ��
		Entry():sign(0),pVote(0) {}

		Entry(const FlowID & fid): pVote(0){
			sign = OAAT(fid);
		}

		//����=���������������
		inline Entry & operator=(const Entry& entry) {
			sign = entry.sign;
			pVote = entry.pVote;
			return *this;
		}

		//�жϵ�ǰ���Ƿ�Ϊ��
		inline bool isEmpty() {
			return sign == 0 && pVote == 0;
		}

		//��յ�ǰ��
		inline bool reset() {
			sign = 0;
			pVote = 0;
			return true;
		}
	}Entry;

	//ͳ�Ʊ�
	Entry ** entryTable;
	// ��Ʊͳ�Ʊ�
	ULONG * fVotes;
	//�У�Ͱ�������У��ۣ���
	ULONG ROW_NUM, COL_NUM;	
	//��ǰ���д��ڵ�����
	ULONG curNum;
	//��/����ֵ�������߳�
	double voteThreshold;
	//�ж�Ϊ��������ֵ
	ULONG largeFlowThreshold;

public:
	LargeFlowCounter(const ULONG row_num, const ULONG col_num, const double voteThreshold,const ULONG);
	virtual ~LargeFlowCounter();
	/** ���һ��������ǰͳ������
	* @param1 ����ʶ������Ԫ��
	* @return ���û���߳�����ļ�¼Ϊtrue������Ϊfalse
	*/
	bool insert(const FlowID &);

	/**
	*	���һ��������ǰͳ�����У������ü���
	*/
	bool insertAndSetCounter(const FlowID&,const ULONG);

	/** ͳ�����д���������
	* @param1 �����ж�Ϊ��������ֵ
	* @return ��������
	*/
	ULONG results(const ULONG threshold);

	/**	��fid�ڶ�Ӧ���еļ���+1
	* @param1 fid����Ԫ��
	* @return �п����ڱ��в�������fid��Ӧ�������ʧ�ܷ���false���������ӳɹ�Ϊtrue
	*/
	bool incr(const FlowID& fid);

	void getLargeFlowList(list<ULONG> &);

	void getLargeFlowNumList(list<ULONG>&,list<ULONG> &);
;
	bool isExists(const FlowID& fid);
private:
	/** ����fid�ڱ��в��Ҽ�¼
	* @param1 fid����Ԫ��
	* @return �鵽�ļ�¼
	*/
	Entry* query(const FlowID& fid);
	/** �ж��Ƿ񵽴���ֵ������������
	* @return �������÷���true�����򷵻�false
	*/
	bool checkAndReset(ULONG);

	/*
		��ȡ��ǰ���ڱ��е�λ��
	*/
	void getFlowPosition(const FlowID & fid, Pair<ULONG, ULONG> &);


	virtual ULONG getFlowNum(const FlowID& fid);
};

