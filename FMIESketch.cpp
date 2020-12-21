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
	if(cmSketch != NULL){
		delete cmSketch;
		cmSketch = NULL;
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
	Filter* cuSketchFilter = new CUSketchFilter(info.CUCKOO_ROW1, info.CUCKOO_COL1,info.FILTER_SHRESHOLD);
	filter = new MiniFlowFilter(cuSketchFilter);

	LFCounter* counter = new LargeFlowCounter(info.IDENTIFY_ROW,info.IDENTIFY_COL,info.IDENTIFY_THRESHOLD, LARGE_FLOW_LFCOUNTER_THRESHOLD);
	identifier = new LargeFlowIdentifier(counter);

	cmSketch = new CMSketch(info.SKETCH_COUNT, info.SKETCH_SIZE);

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
	// 统计流
	const FlowID fid = calcFlowID(pkt.proto, pkt.src, pkt.dst);
	realCounter->add(fid);	//加到真实的统计器中
	// 本方案
	// 1、先在identifier层看一下是否已经存在该流，若已存在则不走过滤层
	bool flag = identifier->incr(fid);
	// 2、上述不存在的情况下，进行分流
	// 3、根据分流情况，第一部分流走Filter，若到达阈值，则将其加入到identifier层
	if (!flag) {
		if (rand() % 100 < filterFlowPercent * 100) {
			bool flag = filter->Filtering(fid);
			if (flag) {
				identifier->insertAndSetCounter(fid, FILTER_MAX_KICKOUT_NUM);
			}
		}	// 4、第二部分流直接走identifier层
		else {
			identifier->insert(fid);
		}
	}

	// 对比方案CMSketch
	cmSketch->insertFlow(fid);

	return true;
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
	list<ULONG> flowNumList;
	identifier->getLargeFlowNumList(flowListMeasure, flowNumList);
	ULONG judgeNum = flowListMeasure.size();	//判定为大流的数量

	list<FlowID*> flowListRealCounter;
	list<ULONG> flowNumListRealCounter;
	realCounter->getLargeFlowList(flowListRealCounter, flowNumListRealCounter);
	ULONG realLargeFlowNum = realCounter->getLargeFlowNum();	//真实样本中的大流数量
	list<FlowID*>::iterator iter;
	for (iter = flowListMeasure.begin(); iter != flowListMeasure.end(); iter++)
	{
		ULONG fNum = identifier->getFlowNum(**iter);		//实际数量也比阈值大
	}

	//3、输出结果
 	//writer << "测量大流数量:%d,实际大流数量:%d,测得大流真实为大流的数量:%d",judgeNum, _realLargeFlowNum, realLargeFlowNum);
	Log::create(logPath);
	Log::add("测得大流数量:" + to_string(judgeNum) + "\t");
	Log::add("实际大流数量:" + to_string(realLargeFlowNum) + "\t");
	 
	list<ULONG>::iterator numIter;
	Log::add("流编号\tFMIE测得数量\tCM测得数量\t真实数量");
	ULONG i = 0;
	for (iter = flowListRealCounter.begin(), numIter = flowNumListRealCounter.begin(); iter != flowListRealCounter.end()&&  numIter != flowNumListRealCounter.end(); iter++,numIter++)
	{
		ULONG fNum = identifier->getFlowNum(**iter);		//实际数量也比阈值大
		ULONG cmFNum = cmSketch->getFlowNum(**iter);
		Log::add("f" + to_string(i++) + "\t" + to_string(fNum) + "\t" + to_string(cmFNum) + "\t" + to_string(*numIter));
	}
}
