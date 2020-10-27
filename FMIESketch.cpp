#include "pch.h"
#include "FMIESketch.h"
#include <list>

FMIESketch::FMIESketch(const UserConfig& info) :readNumLimit(info.PACKET_NUM_LIMIT),
	filterFlowPercent((double)info.FILTER_PKT_NUM/(info.FILTER_PKT_NUM + info.IDENTIFY_PKT_NUM))
{
	srand(unsigned(time(NULL)));
	init(info);
}

FMIESketch::~FMIESketch()
{
	if (filter != NULL) {
		delete filter;
		filter = NULL;
	}
	if (identifier != NULL) {
		delete identifier;
		identifier = NULL;
	}
	if (reader != NULL) {
		delete reader;
		reader = NULL;
	}
	if (realCounter != NULL) {
		delete realCounter;
		realCounter = NULL;
	}
}

FlowID FMIESketch::calcFlowID(const UCHAR proto, const EndID& src, const EndID& dst)
{
	FlowID fid;
	fid.proto = proto;
	fid.src = src;
	fid.dst = dst;
	return fid;
}

void FMIESketch::init(const UserConfig & info)
{
	ASSERT(!info.fileList.empty());

	FILTER_MAX_KICKOUT_NUM = info.FILTER_SHRESHOLD;
	Filter* cuckooFilter = new CuckooFilter(info.CUCKOO_ROW1,info.CUCKOO_COL1,info.CUCKOO_ROW2,info.CUCKOO_COL2, KICK_OUT_NUM,info.FILTER_SHRESHOLD);
	filter = new MiniFlowFilter(cuckooFilter);

	LFCounter* counter = new LargeFlowCounter(info.IDENTIFY_ROW,info.IDENTIFY_COL,info.IDENTIFY_THRESHOLD, LARGE_FLOW_LFCOUNTER_THRESHOLD);
	identifier = new LargeFlowIdentifier(counter);
	reader = new HSNPacketReader();
	list<string> strList = info.fileList;
	list<string>::iterator iter;
	for (iter = strList.begin(); iter != strList.end(); iter++)
	{
		reader->addFile(*iter);
	}
	realCounter = new RealCounter(LARGE_FLOW_REAL_THRESHOLD);
}

bool FMIESketch::add(const Packet & pkt)
{
	ASSERT(pkt.proto == PROTO_TCP || pkt.proto == PROTO_UDP);
	// ͳ����
	const FlowID fid = calcFlowID(pkt.proto, pkt.src, pkt.dst);
	realCounter->add(fid);	//�ӵ���ʵ��ͳ������
	// ������
	// 1������identifier�㿴һ���Ƿ��Ѿ����ڸ��������Ѵ������߹��˲�
	bool flag = identifier->incr(fid);
	// 2�����������ڵ�����£����з���
	// 3�����ݷ����������һ��������Filter����������ֵ��������뵽identifier��
	if (!flag) {
		if (rand() % 100 < filterFlowPercent * 100) {
			bool flag = filter->Filtering(fid);
			if (flag) {
				identifier->insertAndSetCounter(fid, FILTER_MAX_KICKOUT_NUM);
			}
		}	// 4���ڶ�������ֱ����identifier��
		else {
			identifier->insert(fid);
		}
	}

	return true;
}

void FMIESketch::run()
{
	Packet pkt;
	ULONG countNum = 0;
	//1��ͳ����
	while (countNum < readNumLimit && reader->readPacket(pkt)) {
		bool flag = add(pkt);
		if (flag)countNum++;
	}
	//2������ָ��
	list<FlowID*> flowListMeasure;
	list<ULONG> flowNumList;
	identifier->getLargeFlowNumList(flowListMeasure, flowNumList);
	ULONG judgeNum = flowListMeasure.size();	//�ж�Ϊ����������
	ULONG realLargeFlowNum = 0;	//�ж�Ϊ�������������Ǵ���������
	list<FlowID*>::iterator iter;
	for (iter = flowListMeasure.begin(); iter != flowListMeasure.end(); iter++)
	{
		ULONG fNum = realCounter->getFNum(**iter);		//ʵ������Ҳ����ֵ��
		if (fNum >= LARGE_FLOW_REAL_THRESHOLD) {
			realLargeFlowNum++;
		}
	}
	//list<FlowID*> flowListReal;
	//realCounter->getLargeFlowList(flowListReal);
	ULONG realFlowNum = realCounter->getFlowNum();
	ULONG _realLargeFlowNum = realCounter->getLargeFlowNum();	//��ʵ�����еĴ�������
	ULONG realLargeFlowPacketNum = realCounter->getLargeFlowPacketNum();

	//3��������
 	//writer << "������������:%d,ʵ�ʴ�������:%d,��ô�����ʵΪ����������:%d",judgeNum, _realLargeFlowNum, realLargeFlowNum);
	Log::create(logPath);
	Log::add("��ô�������:" + to_string(judgeNum) + "\t");
	Log::add("ʵ�ʴ�������:" + to_string(_realLargeFlowNum) + "\t");
	Log::add("��õĴ�������Ϊ����������:" + to_string(realLargeFlowNum) + "\n");
	 
	list<ULONG>::iterator numIter;
	Log::add("�����\t�������\t��ʵ����");
	ULONG i = 0;
	for (iter = flowListMeasure.begin(), numIter = flowNumList.begin(); iter != flowListMeasure.end()&&  numIter != flowNumList.end(); iter++,numIter++)
	{
		ULONG fNum = realCounter->getFNum(**iter);		//ʵ������Ҳ����ֵ��
		Log::add("f" + to_string(i++) + "\t" + to_string(*numIter) + "\t" + to_string(fNum));
	}
}
