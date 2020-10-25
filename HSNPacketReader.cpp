#include "pch.h"
#include "HSNPacketReader.h"
#include "Log.h"

HSNPacketReader::HSNPacketReader(ifstream * ifFile):PacketReader(ifFile)
{
}

HSNPacketReader::HSNPacketReader(const string src):PacketReader(src)
{
}

HSNPacketReader::HSNPacketReader()
{
}

HSNPacketReader::~HSNPacketReader()
{
}

/**读取一个数据包
* @param 读取到的数据包
* @return 读取到数据包的状态
*/
bool HSNPacketReader::readPacket(Packet& pkt)
{
	while (!m_fileStream.empty()) {
		ifstream* f = m_fileStream.front();
		while (!f->eof())
		{
			bool bRet = readPacket(f, pkt);
			if (!bRet)
			{
				//Log::error("数据包读取错误!");
				continue;
			}
			return true;
		}
		f->close();
		f->clear();
		m_fileStream.pop_front();
		delete f;
		f = NULL;
	}
	return false;
}

bool HSNPacketReader::readPacket(ifstream* ifFile, Packet& pkt)
{
	//读取数据包的时间戳
	ifFile->read((char*)&(pkt.time), sizeof(pkt.time));
	if (!ifFile)
	{
		TRACE("read timestamp failure!\n");
		return false;
	}

	//读取IP首部数据
	IpHeader* pIpHeader = (IpHeader*) new char[sizeof(IpHeader)];
	if (NULL == pIpHeader)
	{
		TRACE("new array error!");
		return false;
	}

	ifFile->read((char*)pIpHeader, sizeof(IpHeader));
	if (!ifFile)
	{
		TRACE("read IP header failure!\n");
		return false;
	}

	//提取IP首部关键字段信息
	USHORT usIpHdrLen = (pIpHeader->ver_ihl & 0x0f) * 4;//提取IP头部长度
	USHORT usIpTotalLen = ntohs(pIpHeader->total_len); 	//提取IP包总长度
	pkt.src.ip = ntohl(pIpHeader->srcIP);
	pkt.dst.ip = ntohl(pIpHeader->dstIP);
	pkt.proto = pIpHeader->protocol;

	delete[] pIpHeader;
	pIpHeader = NULL;

	//若有IP选项，跳过
	if (usIpHdrLen > 20)
	{
		ifFile->seekg(4, ios_base::cur);
		if (!ifFile)
		{
			TRACE("seek file failure!\n");
			return false;
		}
	}

	//根据协议字段区分提取传输层内容
	if (pkt.proto == PROTO_TCP)
	{
		//读取TCP首部数据
		TcpHeader* pTcpHeader = (TcpHeader*) new char[sizeof(TcpHeader)];
		if (NULL == pTcpHeader)
		{
			TRACE("new array error!");
			return false;
		}

		ifFile->read((char*)pTcpHeader, sizeof(TcpHeader));
		if (!ifFile)
		{
			TRACE("read TCP header failure!\n");
			return false;
		}

		//提取端口号
		pkt.src.port = ntohs(pTcpHeader->src_port);
		pkt.dst.port = ntohs(pTcpHeader->dst_port);

		delete[] pTcpHeader;
		pTcpHeader = NULL;
	}
	else if (pkt.proto == PROTO_UDP)
	{
		//读取UDP首部数据
		UdpHeader* pUdpHeader = (UdpHeader*)new char[sizeof(UdpHeader)];
		if (NULL == pUdpHeader)
		{
			TRACE("new array error!");
			return false;
		}

		ifFile->read((char*)pUdpHeader, sizeof(UdpHeader));
		if (!ifFile)
		{
			TRACE("read UDP Header Failure!\n");
			return false;
		}

		//提取端口号
		pkt.src.port = ntohs(pUdpHeader->src_port);
		pkt.dst.port = ntohs(pUdpHeader->dst_port);

		delete[] pUdpHeader;
		pUdpHeader = NULL;
	}
	else	//非TCP和UDP，如GRE(47)
	{
		ifFile->seekg(8, ios_base::cur);
		if (!ifFile)
			TRACE("seek file failure!\n");

		return false;
	}

	return true;
}
