#pragma once

#include "common.h"

/**
*	���ݰ���ȡ��
*/
class PacketReader
{
public:
	virtual bool readPacket(Packet& pkt) = 0;
};

