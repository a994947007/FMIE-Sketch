#include "pch.h"
#include "FMIESketch.h"

FMIESketch::FMIESketch(const UserConfig & info):readNumLimit(info.readNumlimit)
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
	identifier = new LargeFlowIdentifier(info.LargeFlowCounter_ROW_NUM, info.LargeFlowCounter_COL_NUM, info.LargeFlowCounter_MAX_KICKOUT_NUM, info.LargeFlowCounter_voteThreshold);
	reader = new HSNPacketReader();
	list<string> strList = info.fileList;
	list<string>::iterator iter;
	for (iter = strList.begin(); iter != strList.end(); iter++)
	{
		reader->addFile(*iter);
	}
	writer = new ResultWriter(info.resultPath);
}

bool FMIESketch::add(const Packet & pkt)
{
	ASSERT(pkt.proto == PROTO_TCP || pkt.proto == PROTO_UDP);
	const FlowID fid = calcFlowID(pkt.proto, pkt.src, pkt.dst);
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
	while (countNum < readNumLimit && reader->readPacket(pkt)) {
		bool flag = add(pkt);
		if (flag)countNum++;
	}
	//1、默认参数设置
	//2、写出结果
}
