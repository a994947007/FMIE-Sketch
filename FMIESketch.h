#pragma once

#include "Log.h"
#include "common.h"
#include "PacketReader.h"
#include "HSNPacketReader.h"
#include "ResultWriter.h"
#include "MiniFlowFilter.h"
#include "LargeFlowIdentifier.h"
#include "UserConfig.h"

class FMIESketch
{
private:
	MiniFlowFilter * filter;
	LargeFlowIdentifier * identifier;
	PacketReader * reader;
	ResultWriter * writer;
public:
	/** FMIESketch默认构造器
	* @param1 用户配置项
	*/
	FMIESketch(const UserConfig &);
	~FMIESketch();

	/** 添加一个数据包到Sketch
	* @param 数据包信息
	*/
	void add(const Packet &);

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

