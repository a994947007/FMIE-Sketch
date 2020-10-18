#include "pch.h"
#include "FMIESketch.h"
#include <list>

FMIESketch::FMIESketch(const UserConfig& info) :readNumLimit(info.readNumlimit), largeFlowNumThreshold(info.LargeFlow_threshold)
{
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
	if (writer != NULL) {
		delete writer;
		writer = NULL;
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

	filter = new MiniFlowFilter(info.CUSketch_countersNum, info.CUSketch_countersSize, info.MiniFlowFilter_threshold);
	identifier = new LargeFlowIdentifier(info.LargeFlowCounter_ROW_NUM, info.LargeFlowCounter_COL_NUM, info.LargeFlowCounter_MAX_KICKOUT_NUM, info.LargeFlowCounter_voteThreshold,info.LargeFlow_threshold);
	reader = new HSNPacketReader();
	list<string> strList = info.fileList;
	list<string>::iterator iter;
	for (iter = strList.begin(); iter != strList.end(); iter++)
	{
		reader->addFile(*iter);
	}
	writer = new ResultWriter(info.resultPath);
	realCounter = new RealCounter(info.LargeFlow_threshold);
}

int n = 0;
bool FMIESketch::add(const Packet & pkt)
{
	ASSERT(pkt.proto == PROTO_TCP || pkt.proto == PROTO_UDP);
	//ͳ����
	n++;
	const FlowID fid = calcFlowID(pkt.proto, pkt.src, pkt.dst);
	realCounter->add(fid);	//�ӵ���ʵ��ͳ������
	//������
	bool flag = filter->filter(fid);
	if (!flag) {	
		identifier->counting(fid);
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
	identifier->getLargeFlowList(flowListMeasure);
	ULONG judgeNum = flowListMeasure.size();	//�ж�Ϊ����������
	ULONG realLargeFlowNum = 0;	//�ж�Ϊ�������������Ǵ���������
	list<FlowID*>::iterator iter;
	for (iter = flowListMeasure.begin(); iter != flowListMeasure.end(); iter++)
	{
		ULONG fNum = realCounter->getFNum(**iter);		//ʵ������Ҳ����ֵ��
		if (fNum >= largeFlowNumThreshold) {
			realLargeFlowNum++;
		}
	}
	//list<FlowID*> flowListReal;
	//realCounter->getLargeFlowList(flowListReal);
	ULONG realFlowNum = realCounter->getFlowNum();
	ULONG _realLargeFlowNum = realCounter->getLargeFlowNum();	//��ʵ�����еĴ�������
	ULONG realLargeFlowPacketNum = realCounter->getLargeFlowPacketNum();

	//3��������
 	writer->write("����ֵ:" + judgeNum);
	writer->write("����ֵ������Ϊ����������:" + realLargeFlowNum);
}
