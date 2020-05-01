#include "pch.h"
#include "ResultWriter.h"
#include "config.h"
#include "fstream"

void ResultWriter::write(string results)
{
    ofstream ofileAgain;
    ofileAgain.open(resultPath, ios::app);
    ofileAgain << results << endl;
    ofileAgain.close();
}
