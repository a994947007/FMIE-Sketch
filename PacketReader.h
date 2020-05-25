#pragma once

#include "common.h"

/**
*	数据包读取类
*/
class PacketReader
{
public:
	virtual bool readPacket(Packet& pkt) = 0;
};

