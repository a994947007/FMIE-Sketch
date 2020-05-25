#include "pch.h"
#include "Log.h"
#include "config.h"
#include <fstream>

void Log::add(string content)
{
    ofstream ofileAgain;
    ofileAgain.open(logPath, ios::app);
    ofileAgain << content << endl;
    ofileAgain.close();
    ofileAgain.clear();
}

void Log::error(string content)
{
    ofstream ofileAgain;
    ofileAgain.open(logErrorPath, ios::app);
    ofileAgain << content << endl;
    ofileAgain.close();
    ofileAgain.clear();
}
