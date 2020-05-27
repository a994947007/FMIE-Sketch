#pragma once
#include "Log.h"
#include "common.h"
class LargeFlowCounter
{
private:
	typedef struct Entry {
		FlowID fid;		//fid
		ULONG pVote;	//��Ʊ��
		ULONG fVote;	//��Ʊ��
		Entry():pVote(0),fVote(0) {}

		Entry(const Entry& entry) {
			fid = entry.fid;
			pVote = entry.pVote;
			fVote = entry.fVote;
		}

		Entry(const FlowID & fid):fid(fid), pVote(0), fVote(0){}

		//����=���������������
		inline Entry & operator=(const Entry& entry) {
			fid = entry.fid;
			pVote = entry.pVote;
			fVote = entry.fVote;
			return *this;
		}

		//�жϵ�ǰ���Ƿ�Ϊ��
		inline bool isEmpty() {
			return fid.isEmpty() && pVote == 0 && fVote == 0;
		}

		//��յ�ǰ��
		inline bool reset() {
			fid.reset();
			pVote = 0;
			fVote = 0;
		}
	}Entry;

	//ͳ�Ʊ�
	Entry ** entryTable;
	//�У�Ͱ�������У��ۣ���
	ULONG ROW_NUM, COL_NUM;	
	//��ͳ������
	ULONG curNum;
	//��/����ֵ�������߳�
	double voteThreshold;
	//����߳�����
	ULONG MAX_KICKOUT_NUM;

public:
	LargeFlowCounter(const ULONG row_num, const ULONG col_num,const ULONG MAX_KICKOUT_NUM, const double voteThreshold);
	/** ���һ��������ǰͳ������
	* @param1 ����ʶ������Ԫ��
	* @return ���û���߳�����ļ�¼Ϊtrue������Ϊfalse
	*/
	bool insert(const FlowID &);

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

private:
	/** ����fid�ڱ��в��Ҽ�¼
	* @param1 fid����Ԫ��
	* @return �鵽�ļ�¼
	*/
	Entry* query(const FlowID& fid);
	/** �ж��Ƿ񵽴���ֵ������������
	* @return �������÷���true�����򷵻�false
	*/
	bool checkAndReset(Entry & entry);
};

