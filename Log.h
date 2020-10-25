#pragma once
/**
*	日志类，用于保存程序日志
*/

#include <iostream>
using namespace std;

class Log
{
public:
	static void add(string content);	// 正常日志
	static void create(const char * path);	// 创建文件
	static void error(string content);	// 错误日志
};

