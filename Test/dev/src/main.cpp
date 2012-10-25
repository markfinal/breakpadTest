#include "crashhandler.h"

#include <iostream>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning( disable : 4717 ) // warning C4717: '`anonymous namespace'::CrashAndBurn' : recursive on all control paths, function will cause runtime stack overflow
#endif
 
int CrashAndBurn(int x)
{
    return CrashAndBurn(x + 1) + CrashAndBurn(x + 1);
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

int main(int argc, char* argv[])
{
    CrashHandler::Parameters params(argc, argv);
    params._minidumpDir = L"c:\\temp";
    params._oopExecutable = "CrashServer.exe";
    CrashHandler crashHandler(params);

#if 1
    int result = CrashAndBurn(0);
#else
    *((int*)(0)) = 3;
#endif

    std::cout << "Finished..." << std::endl;

    return result;
}
