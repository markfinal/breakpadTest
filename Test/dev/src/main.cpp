#include "crashhandler.h"

int main(int argc, char* argv[])
{
    CrashHandler::Parameters params(argc, argv);
    CrashHandler crashHandler(params);

    return 0;
}
