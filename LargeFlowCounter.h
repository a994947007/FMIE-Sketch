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
		inline Entry(FlowID & fid):fid(fid), pVote(0), fVote(0){}

		//�������캯��
		Entry(const Entry & entry) {
			fid = entry.fid;
			pVote = entry.pVote;
			fVote = entry.fVote;
		}

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

public:
	/** ���һ��������ǰͳ������
	* @param1 ����ʶ������Ԫ��
	*/
	bool add(const FlowID &);

	/** ͳ�����д���������
	* @return ��������
	*/
	ULONG results();
};

