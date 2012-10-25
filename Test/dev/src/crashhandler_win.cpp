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
} // anonymous namespace

CrashHandler::Parameters::Parameters(const int argc, char **argv)
: _argc(argc), _argv(argv), _forceInProcess(false)
{
}

CrashHandler::CrashHandler(const Parameters &params)
{
    Initialize(params);
}

CrashHandler::~CrashHandler()
{
}

void CrashHandler::Initialize(const Parameters &params)
{
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
}
