#include "crashhandler.h"

int main(int argc, char* argv[])
{
    CrashHandler::Parameters params(argc, argv);
    params._minidumpDir = L"c:\\temp";
    CrashHandler crashHandler(params);

    *((int*)(0)) = 3;

    return 0;
}
