#include "pch.h"
#include "FMIESketch.h"
#include <list>

FMIESketch::FMIESketch(const UserConfig& info) :readNumLimit(info.PACKET_NUM_LIMIT),
	filterFlowPercent((double)info.FILTER_PKT_NUM/info.FILTER_PKT_NUM + info.IDENTIFY_PKT_NUM)
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

	Filter* cuckooFilter = new CuckooFilter(info.CUCKOO_ROW1,info.CUCKOO_COL1,info.CUCKOO_ROW2,info.CUCKOO_COL2, KICK_OUT_NUM,info.FILTER_SHRESHOLD);
	filter = new MiniFlowFilter(cuckooFilter);

	identifier = new LargeFlowIdentifier(info.LargeFlowCounter_ROW_NUM, info.LargeFlowCounter_COL_NUM, info.LargeFlowCounter_MAX_KICKOUT_NUM, info.LargeFlowCounter_voteThreshold,info.LargeFlow_threshold);
	reader = new HSNPacketReader();
	list<string> strList = info.fileList;
	list<string>::iterator iter;
	for (iter = strList.begin(); iter != strList.end(); iter++)
	{
		reader->addFile(*iter);
	}
	writer = new ResultWriter(info.resultPath);
	realCounter = new RealCounter(LARGE_FLOW_REAL_THRESHOLD);
}

bool FMIESketch::add(const Packet & pkt)
{
	ASSERT(pkt.proto == PROTO_TCP || pkt.proto == PROTO_UDP);
	// 统计流
	const FlowID fid = calcFlowID(pkt.proto, pkt.src, pkt.dst);
	realCounter->add(fid);	//加到真实的统计器中
	// 本方案
	// 1、先在identifier层看一下是否已经存在该流，若已存在则不走过滤层

	// 2、上述不存在的情况下，进行分流

	// 3、根据分流情况，第一部分流走Filter，若到达阈值，则将其加入到identifier层
	FlowID result;
	bool flag = filter->Filtering(fid);
	if (flag) {	// 过滤被踢到了当前
		identifier->counting(result);
	}
	return true;
	// 4、第二部分流直接走identifier层

}

void FMIESketch::run()
{
	Packet pkt;
	ULONG countNum = 0;
	//1、统计流
	while (countNum < readNumLimit && reader->readPacket(pkt)) {
		bool flag = add(pkt);
		if (flag)countNum++;
	}
	//2、计算指标
	list<FlowID*> flowListMeasure;
	identifier->getLargeFlowList(flowListMeasure);
	ULONG judgeNum = flowListMeasure.size();	//判定为大流的数量
	ULONG realLargeFlowNum = 0;	//判定为大流并且真正是大流的数量
	list<FlowID*>::iterator iter;
	for (iter = flowListMeasure.begin(); iter != flowListMeasure.end(); iter++)
	{
		ULONG fNum = realCounter->getFNum(**iter);		//实际数量也比阈值大
		if (fNum >= LARGE_FLOW_REAL_THRESHOLD) {
			realLargeFlowNum++;
		}
	}
	//list<FlowID*> flowListReal;
	//realCounter->getLargeFlowList(flowListReal);
	ULONG realFlowNum = realCounter->getFlowNum();
	ULONG _realLargeFlowNum = realCounter->getLargeFlowNum();	//真实样本中的大流数量
	ULONG realLargeFlowPacketNum = realCounter->getLargeFlowPacketNum();

	//3、输出结果
 	writer->write("测量值:" + judgeNum);
	writer->write("测量值中真正为大流的数量:" + realLargeFlowNum);
}
