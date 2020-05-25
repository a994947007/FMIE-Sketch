#pragma once

#include <string>
#include <fstream>
#include <list>
#include "PacketReader.h"

using namespace std;

class HSNPacketReader : public PacketReader
{
private:
	list<ifstream*>  m_fileStream;		//TRACE�ļ���
public:
	HSNPacketReader(ifstream * ifFile);
	HSNPacketReader(const string src);
	HSNPacketReader();

	~HSNPacketReader();

	/**��ȡһ�����ݰ�
	* @param ��ȡ�������ݰ�
	* @return ���ݰ���ȡ״̬����ȡ�ɹ�Ϊtrue
	*/
	bool readPacket(Packet& pkt);

	/**���һ�����ݰ��ļ�
	* @param �ļ�·��
	* @���ݰ���ȡ״̬����ȡ�ɹ�Ϊtrue
	*/
	void addFile(const string src);

	/**�ر��ļ���*/
	void close();

private:
	bool readPacket(ifstream * ifFile, Packet& pkt);
};

