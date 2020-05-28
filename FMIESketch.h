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
	/** FMIESketchĬ�Ϲ�����
	* @param1 �û�������
	*/
	FMIESketch(const UserConfig &);
	~FMIESketch();

	/** ���һ�����ݰ���Sketch
	* @param ���ݰ���Ϣ
	*/
	void add(const Packet &);

	/**
	* ��ʼͳ��
	*/
	void run();

private:
	/** �������ؼ���fid����Ԫ��
	* @param1 Э������
	* @param2 EndID��Դip��Դ�˿�
	* @param3 EndID��Ŀ��ip��Ŀ�Ķ˿�
	* @return ����fid
	*/
	FlowID calcFlowID(const UCHAR proto, const EndID& src, const EndID& dst);
	void init(const UserConfig&);
};

