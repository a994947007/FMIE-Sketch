#pragma once

#include "Log.h"
#include "common.h"
#include "PacketReader.h"
#include "HSNPacketReader.h"
#include "ResultWriter.h"
#include "MiniFlowFilter.h"
#include "LargeFlowIdentifier.h"
#include "UserConfig.h"
#include "RealCounter.h"
#include "config.h"



class FMIESketch
{
private:
	MiniFlowFilter * filter;
	LargeFlowIdentifier * identifier;
	PacketReader * reader;
	ResultWriter * writer;
	RealCounter * realCounter;
	ULONG readNumLimit;	//限制读取数据包个数
	double filterFlowPercent;
	//ULONG largeFlowNumThreshold; //判定为大流的阈值
public:
	/** FMIESketch默认构造器
	* @param1 用户配置项
	*/
	FMIESketch(const UserConfig &);
	~FMIESketch();

	/** 添加一个数据包到Sketch
	* @param 数据包信息
	* @return 统计完毕返回true
	*/
	bool add(const Packet &);

	/**
	* 开始统计
	*/
	void run();

private:
	/** 计算流关键字fid，五元组
	* @param1 协议类型
	* @param2 EndID，源ip、源端口
	* @param3 EndID，目的ip、目的端口
	* @return 返回fid
	*/
	FlowID calcFlowID(const UCHAR proto, const EndID& src, const EndID& dst);
	void init(const UserConfig&);
};

