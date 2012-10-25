#ifndef CRASHHANDLER_H
#define CRASHHANDLER_H

#include <string>

class CrashHandler
{
public:
    struct Parameters
    {
        explicit Parameters(const int, char**);

        int _argc;
        char **_argv;
        std::wstring _minidumpDir;
        bool _forceInProcess;
    };

    explicit CrashHandler(const Parameters &);
    ~CrashHandler();

    void Initialize(const Parameters &);

private:
    CrashHandler(const CrashHandler &);
};

#endif // CRASHHANDLER_H
