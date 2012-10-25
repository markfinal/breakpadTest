#include "crashhandler.h"
#include "pipename_win.h"
#include "client/windows/handler/exception_handler.h"

#include <iostream>

namespace
{
bool BreakpadFilter(void* context, EXCEPTION_POINTERS* exinfo, MDRawAssertionInfo* assertion)
{
  // return true - allow Breakpad to write a minidump
  // return false - don't do anything, but allow another exception handler a chance to jump in

  std::cout << "Breakpad client filter called" << std::endl;
  std::cout << "\tcontext   = " << context << std::endl;
  std::cout << "\texinfo    = " << exinfo << std::endl;
  std::cout << "\tassertion = " << assertion << std::endl;
  return true;
}

bool BreakpadMinidumpCallback(const wchar_t* dump_path,
                              const wchar_t* minidump_id,
                              void* context,
                              EXCEPTION_POINTERS* exinfo,
                              MDRawAssertionInfo* assertion,
                              bool succeeded)
{
  // return true if fully handled
  // return false, treat as unhandled and pass onto another handler, eventually reporting to the system

  std::cout << "Breakpad client minidump callback called" << std::endl;
  if (0 == dump_path) {
    std::cout << "\tOut of process server detected" << std::endl;
  }
  else {
    {
      std::cout << "\tdump_path     = " << dump_path << " ";
      const wchar_t* wc = dump_path;
      while (*wc != L'\0')
      {
        const char c = static_cast<char>(std::wctob(*wc));
        std::cout << c;
        ++wc;
      }
      std::cout << std::endl;
    }
    {
      std::cout << "\tminidump_id   = " << minidump_id << " ";
      const wchar_t* wc = minidump_id;
      while (*wc != L'\0')
      {
        const char c = static_cast<char>(std::wctob(*wc));
        std::cout << c;
        ++wc;
      }
      std::cout << std::endl;
    }
  }
  std::cout << "\tcontext       = " << context << std::endl;
  std::cout << "\texinfo        = " << exinfo << std::endl;
  std::cout << "\tassertion     = " << assertion << std::endl;
  std::cout << "\tsucceeded     = " << succeeded << std::endl;
  return true;
}

::HANDLE StartCrashReportingProcess(const std::string &executablePath)
{
  // start the child process for reporting crashes
  ::PROCESS_INFORMATION processInfo;
  ::STARTUPINFO startUpInfo;

  ::ZeroMemory(&processInfo, sizeof(::PROCESS_INFORMATION));
  ::ZeroMemory(&startUpInfo, sizeof(::STARTUPINFO));

  const char *applicationName = NULL;
  char *commandLine = const_cast<char *>(executablePath.c_str()); // annoying cast, as CreateProcess wants a non-const command line char*
  const ::LPSECURITY_ATTRIBUTES processAttributes = NULL;
  const ::LPSECURITY_ATTRIBUTES threadAttributes = NULL;
  const ::BOOL inheritHandles = FALSE;
  const ::DWORD creationFlags = 0;
  const ::LPVOID environment = 0;
  const char *currentDirectory = 0;
  ::BOOL success = ::CreateProcess(applicationName, commandLine, processAttributes, threadAttributes, inheritHandles, creationFlags, environment, currentDirectory, &startUpInfo, &processInfo);
  if (FALSE == success)
  {
    throw 1; // TODO: better exceptions please
  }

  // TODO: will have to close the HANDLE in processInfo
  ::HANDLE childProcessHandle = processInfo.hProcess;

  // this is a little dangerous, but since we control the crash reporting process, wait for it to idle before continuing
  const ::DWORD timeoutInMilliseconds = 5 * 1000;
  const ::DWORD idleInputResult = ::WaitForInputIdle(childProcessHandle, timeoutInMilliseconds);
  if (0 == idleInputResult) {
    std::cout << "Crash reporting application successfully waited for idle" << std::endl;
  }
  else if (WAIT_TIMEOUT == idleInputResult) {
    std::cout << "Crash reporting application timed out after " << timeoutInMilliseconds << " milliseconds" << std::endl;
  }
  else {
    std::cerr << "Crash reporting application idle timeout failed" << std::endl;
  }

  return childProcessHandle;
}

} // anonymous namespace

CrashHandler::Parameters::Parameters(const int argc, char **argv)
: _argc(argc), _argv(argv), _forceInProcess(false)
{
    for (int i = 0; i < argc; ++i)
    {
        std::string command(argv[i]);
        if (command == "--inProcess")
        {
            this->_forceInProcess = true;
        }
    }
}

CrashHandler::CrashHandler(const Parameters &params)
: _serverHandle(0), _exceptionHandler(0)
{
    Initialize(params);
}

CrashHandler::~CrashHandler()
{
   if (this->_serverHandle != INVALID_HANDLE_VALUE) {
     ::CloseHandle(this->_serverHandle);
     this->_serverHandle = 0;
   }
 
   if (0 != this->_exceptionHandler) {
     delete this->_exceptionHandler;
     this->_exceptionHandler = 0;
   }
}

void CrashHandler::Initialize(const Parameters &params)
{
    if (!params._forceInProcess) {
        this->_serverHandle = StartCrashReportingProcess(params._oopExecutable);
    }
    else {
        this->_serverHandle = INVALID_HANDLE_VALUE;
    }

    // create the exception handler
    const std::wstring dump_path(params._minidumpDir);
    google_breakpad::ExceptionHandler::FilterCallback filter = BreakpadFilter;
    google_breakpad::ExceptionHandler::MinidumpCallback callback = BreakpadMinidumpCallback;
    void *callback_context = 0;
    google_breakpad::ExceptionHandler::HandlerType handler_types = google_breakpad::ExceptionHandler::HANDLER_ALL;
    MINIDUMP_TYPE dump_type = static_cast<MINIDUMP_TYPE>(MD_NORMAL);
    const wchar_t* pipe_name = !params._forceInProcess ? BREAKPAD_PIPE_NAME : 0;
    const google_breakpad::CustomClientInfo* custom_info = 0; // ?? TODO: what does this do?

    google_breakpad::ExceptionHandler *breakpadExceptionHandler =
      new google_breakpad::ExceptionHandler(dump_path,
                                            filter,
                                            callback,
                                            callback_context,
                                            handler_types,
                                            dump_type,
                                            pipe_name,
                                            custom_info);
    if (0 == breakpadExceptionHandler)
    {
      std::cerr << "Unable to start exception handler" << std::endl;
    }
    else
    {
      const bool isOutOfProcess = breakpadExceptionHandler->IsOutOfProcess();
      if (isOutOfProcess)
      {
        std::cout << "Breakpad exception handler is Out of Process" << std::endl;
      }
      else
      {
        std::cout << "Breakpad exception handler is In Process" << std::endl;
      }
    }

   this->_exceptionHandler = breakpadExceptionHandler;
}
