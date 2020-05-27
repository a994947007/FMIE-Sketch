#pragma once

#include <string>
#include <fstream>
#include <list>
#include "PacketReader.h"

using namespace std;

class HSNPacketReader : public PacketReader
{
public:
	HSNPacketReader(ifstream * ifFile);
	HSNPacketReader(const string src);
	HSNPacketReader();

	~HSNPacketReader();

	/**读取一个数据包
	* @param 读取到的数据包
	* @return 数据包读取状态，读取成功为true
	*/
	bool readPacket(Packet& pkt);

private:
	bool readPacket(ifstream * ifFile, Packet& pkt);
};

