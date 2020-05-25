#pragma once
#include <string>
using namespace std;

class ResultWriter
{
private:
	ofstream * ofFile;
	string filePath;

public:
	/** �ļ�д��Ĭ�ϼ��أ���config.h�м���д��·��
	*/
	ResultWriter();
	/** �ļ�д����ָ���ļ�д��·��
	*/
	ResultWriter(const string filePath);

	~ResultWriter();

	/** д�����ݽ��
	* @param ���
	*/
	void write(const string results);
	/**д�����ݽ��
	* @param dst �ļ�·��
	* @param result ���
	*/
	void write(const string dst,const string results);

	void close();

};

