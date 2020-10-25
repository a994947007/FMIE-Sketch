#pragma once
#include <stdlib.h>
#include <cstring>
#include "iostream"
using namespace std;

class ResultWriter
{
private:
	FILE * pFile;
public:
	/** 文件写出默认加载，从config.h中加载写出路径
	*/
	ResultWriter();
	/** 文件写出，指定文件写出路径
	*/
	ResultWriter(const char * path);

	~ResultWriter();

	ResultWriter & operator << (const char * str);
	ResultWriter & operator << (const ULONG str);
	ResultWriter & operator << (const double str);
	ResultWriter & operator << (const float str);

	void close();
};

