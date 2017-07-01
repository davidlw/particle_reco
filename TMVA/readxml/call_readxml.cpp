#include "TSystem.h"
#include <string>

using namespace std;

void readxml(const string&, const string&);
void readxml(const string&, const string&);

void call_readxml(const string& lib_name, const string& backgroundTreeFileList, const string& outIndex)
{
    gSystem->Load(lib_name.c_str());

    readxml(backgroundTreeFileList, outIndex);
}

void call_readxml(const string& lib_name, const string* backgroundTreeFileList, int outIndex)
{
    call_readxml(lib_name, backgroundTreeFileList, to_string(outIndex));
}