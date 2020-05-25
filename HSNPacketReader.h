#pragma once

#include <string>
#include <fstream>
#include <list>
#include "PacketReader.h"

using namespace std;

class HSNPacketReader : public PacketReader
{
private:
	list<ifstream*>  m_fileStream;		//TRACE文件流
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

	/**添加一个数据包文件
	* @param 文件路径
	* @数据包读取状态，读取成功为true
	*/
	void addFile(const string src);

	/**关闭文件流*/
	void close();

private:
	bool readPacket(ifstream * ifFile, Packet& pkt);
};

