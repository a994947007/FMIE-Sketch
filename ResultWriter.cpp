#include "pch.h"
#include "ResultWriter.h"
#include "config.h"
#include "fstream"

ResultWriter::ResultWriter()
{
    ofFile = new ofstream;
    ofFile->open(resultPath, ios::app);
}

ResultWriter::ResultWriter(const string filePath):filePath(filePath)
{
    ofFile = new ofstream;
    ofFile->open(filePath, ios::app);
}

ResultWriter::~ResultWriter()
{
    close();
}

void ResultWriter::write(const string results)
{
    *ofFile << results << endl;
    ofFile->close();
}

void ResultWriter::write(const string dst, const string results)
{
    ofstream f;
    f.open(dst, ios::app);
    f << results << endl;
    f.close();
    f.clear();
}

void ResultWriter::close()
{
    if (ofFile != NULL) {
        ofFile->close();
        ofFile->clear();
        delete ofFile;
        ofFile = NULL;
    }
}
