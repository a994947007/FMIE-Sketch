#pragma once

#include "common.h"
#include <list>
#include <vector>

using namespace std;

class FlowTable
{

protected:
	typedef struct Node {
		FlowID fid;
		ULONG fNum;
		Node* next;
		Node():next(NULL),fNum(1){
		}
		Node(const FlowID & fid):next(NULL),fNum(1),fid(fid) {
		}
	};
	ULONG flowNum;
	ULONG hashLen;
	ULONG largeFlowThreshold;

	USHORT calcHash(const FlowID&);
	vector<Node*> hashTable;

	Node* query(const FlowID&);

public:
	FlowTable(const ULONG hashLen,const ULONG largeFlowThreshold);
	virtual ~FlowTable();

	void add(const FlowID&);
	void getLargeFlowList(list<FlowID *> &);
	void getLargeFlowList(list<FlowID*> &,list<ULONG> &);
	ULONG getFNum(const FlowID&);

	ULONG getFlowNum();
	ULONG getLargeFlowNum();
	ULONG getLargeFlowPacketNum();

};