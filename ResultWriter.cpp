#include "pch.h"
#include "ResultWriter.h"
#include "config.h"
#include <string>

#pragma warning(disable:4996)

ResultWriter::ResultWriter():pFile(NULL)
{
    pFile = fopen(resultPath, "w"); //新建文件
    if (pFile != NULL) {
        fclose(pFile);
    }
    pFile = fopen(resultPath, "at+");
}

ResultWriter::ResultWriter(const char * path)
{
    char resultPath[200] = {0};
    strcat(resultPath, path);
    strcat(resultPath, "/result.txt");
    pFile = fopen(resultPath, "w"); //新建文件
    if (pFile != NULL) {
        fclose(pFile);
    }
    pFile = fopen(resultPath, "at+");
}

ResultWriter::~ResultWriter()
{
    close();
}

ResultWriter & ResultWriter::operator<<( const char * result) {
    fprintf(pFile,"%s", result);
    return *this;
}

ResultWriter& ResultWriter::operator << (const ULONG result) {
    fprintf(pFile, "%d", result);
    return *this;
}
ResultWriter& ResultWriter::operator << (const double result) {
    fprintf(pFile, "%lf", result);
    return *this;
}
ResultWriter& ResultWriter::operator << (const float result) {
    fprintf(pFile, "%f", result);
    return *this;
}

void ResultWriter::close()
{
    if (pFile != NULL) {
        fclose(pFile);
        pFile = NULL;
    }
}
