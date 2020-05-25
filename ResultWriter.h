#pragma once
#include <string>
using namespace std;

class ResultWriter
{
private:
	ofstream * ofFile;
	string filePath;

public:
	/** 文件写出默认加载，从config.h中加载写出路径
	*/
	ResultWriter();
	/** 文件写出，指定文件写出路径
	*/
	ResultWriter(const string filePath);

	~ResultWriter();

	/** 写出数据结果
	* @param 结果
	*/
	void write(const string results);
	/**写出数据结果
	* @param dst 文件路径
	* @param result 结果
	*/
	void write(const string dst,const string results);

	void close();

};

