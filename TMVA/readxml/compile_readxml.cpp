#include "TSystem.h"
#include <string>

using namespace std;

void compile_readxml(const string& lib_name)
{
    gSystem->CompileMacro("readxml.cc", "kfc", lib_name.c_str());
}