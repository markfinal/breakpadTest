#ifndef CRASHHANDLER_H
#define CRASHHANDLER_H

#include <string>
#include <Windows.h>

namespace google_breakpad
{

class ExceptionHandler;

} // namespace google_breakpad

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
        std::string _oopExecutable;
    };

    explicit CrashHandler(const Parameters &);
    ~CrashHandler();

    void Initialize(const Parameters &);

private:
    CrashHandler(const CrashHandler &);

private:
    ::HANDLE _serverHandle;
    google_breakpad::ExceptionHandler *_exceptionHandler;
};

#endif // CRASHHANDLER_H
