#pragma once

#include "common.h"
#include <list>
#include <string>

using namespace std;

typedef struct UserConfig{
	list<string> fileList;
	string resultPath;
}UserConfig, * pUserConfig;