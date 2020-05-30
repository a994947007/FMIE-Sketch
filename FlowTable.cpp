#include "pch.h"
#include "FlowTable.h"
#include "HashFunctions.h"
#include "UserConfig.h"

USHORT FlowTable::calcHash(const FlowID& key)
{
	UCHAR buf[FID_LEN];
	((FlowID*)&key)->ToData(buf);
	return XOR16(buf, FID_LEN) % hashLen;
}

FlowTable::Node * FlowTable::query(const FlowID& key)
{
	USHORT usPos = calcHash(key);
	Node* p = hashTable[usPos]->next;
	while (p != NULL) {
		if (p->fid == key) {
			return p;
		}
		p = p->next;
	}
	return NULL;
}

FlowTable::FlowTable(const ULONG hashLen, const ULONG largeFlowThreshold) :hashLen(hashLen), flowNum(0), largeFlowThreshold(largeFlowThreshold)
{
	for (ULONG i = 0; i < hashLen; i++) {
		Node* node = new Node;
		hashTable.push_back(node);
	}
}

FlowTable::~FlowTable()
{
	for(ULONG i =0;i<hashLen;i++){
		Node* p = hashTable[i];
		while (p != NULL) {
			Node* s = p->next;
			delete p;
			p = s;
		}
	}
}

void FlowTable::add(const FlowID& key) {
	Node * node = query(key);
	if (node != NULL) {
		node->fNum++;
	}
	else {
		flowNum++;
		USHORT usPos = calcHash(key);
		Node* node = new Node(key);
		node->next = hashTable[usPos]->next;
		hashTable[usPos]->next = node;
	}
}

void FlowTable::getLargeFlowList(list<FlowID*> & flowList)
{
	for (ULONG i = 0; i < hashLen; i++) {
		for (Node* p = hashTable[i]->next; p != NULL; p = p->next) {
			if (p->fNum >= largeFlowThreshold) {
				flowList.push_back(&p->fid);
			}
		}
	}
}

ULONG FlowTable::getFNum(const FlowID& key)
{
	Node* node = query(key);
	if (node != NULL) {
		return node->fNum;
	}
	return 0;
}

ULONG FlowTable::getFlowNum()
{
	return flowNum;
}

ULONG FlowTable::getLargeFlowNum()
{
	ULONG largeFlowNum = 0;
	for (ULONG i = 0; i < hashLen; i++) {
		for (Node* p = hashTable[i]->next; p != NULL; p = p->next) {
			if (p->fNum >= largeFlowThreshold) {
				largeFlowNum++;
			}
		}
	}
	return largeFlowNum;
}

ULONG FlowTable::getLargeFlowPacketNum()
{
	ULONG largeFlowPacketNum = 0;
	for (ULONG i = 0; i < hashLen; i++) {
		for (Node* p = hashTable[i]->next; p != NULL; p = p->next) {
			if (p->fNum >= largeFlowThreshold) {
				largeFlowPacketNum += p->fNum;
			}
		}
	}
	return largeFlowPacketNum;
}
