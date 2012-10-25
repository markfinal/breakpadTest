#include "pipename_win.h"

#include "client/windows/crash_generation/crash_generation_server.h"
#include "client/windows/crash_generation/client_info.h"

#include <iostream>
#include <fstream>

// define this to use a Windows application (linker SUBSYSTEM:Windows) rather than a Console application
// note that there are some issues killing the process when it is a Console application
#define WINDOWS_APP

namespace
{

  void onClientConnectedCallback(void* context, const google_breakpad::ClientInfo* client_info)
  {
    std::cout << "Breakpad server onClientConnectedCallback" << std::endl;
    std::cout << "\tcontext     = " << context << std::endl;
    std::cout << "\tclient_info = " << client_info << std::endl;
    std::cout << "\t\tPID       = " << client_info->pid() << std::endl;
  }

  void onClientDumpRequestCallback(void* context,
                                   const google_breakpad::ClientInfo* client_info,
                                   const std::wstring* file_path)
  {
    std::cout << "Breakpad server onClientDumpRequestCallback" << std::endl;
    std::cout << "\tcontext     = " << context << std::endl;
    std::cout << "\tclient_info = " << client_info << std::endl;
    std::cout << "\t\tPID       = " << client_info->pid() << std::endl;

    std::cout << "\tfile_path   = " << file_path << " ";
    for (std::wstring::const_iterator i = file_path->begin(); i != file_path->end(); ++i)
    {
      char c = static_cast<char>(std::wctob(*i));
      std::cout << c;
    }
    std::cout << std::endl;
  }

  void onClientExitedCallback(void* context,
                              const google_breakpad::ClientInfo* client_info)
  {
    std::cout << "Breakpad server onClientExitedCallback" << std::endl;
    std::cout << "\tcontext     = " << context << std::endl;
    std::cout << "\tclient_info = " << client_info << std::endl;
    std::cout << "\t\tPID       = " << client_info->pid() << std::endl;

    // TODO I'd rather do this, but it never gets into the message loop
    //::PostQuitMessage(0);
    std::cout << "Exiting reporter since client has exited" << std::endl;
    exit(0);
  }

  void onClientUploadRequestCallback(void* context,
                                     const DWORD crash_id)
  {
    std::cout << "Breakpad server onClientUploadRequestCallback" << std::endl;
    std::cout << "\tcontext  = " << context << std::endl;
    std::cout << "\tcrash_id = " << crash_id << std::endl;
  }

} // anonymous namespace

#ifdef WINDOWS_APP
int APIENTRY WinMain(::HINSTANCE /*instance*/,
                     ::HINSTANCE /*previous_instance*/,
                     ::LPTSTR /*command_line*/,
                     int /*command_show*/)
#else
int main()
#endif
{
  const wchar_t *pipeName = BREAKPAD_PIPE_NAME;
  const ::LPSECURITY_ATTRIBUTES securityAttributes = NULL;
  void* connect_context = 0;
  void* dump_context = 0;
  void* exit_context = 0;
  const bool generate_dumps = true;
  const std::wstring dump_path = L"c:\\temp";

  std::cout << "---------------------------------------------------------------------------------------" << std::endl;
  std::cout << "Creating CrashGenerationServer" << std::endl;

  void* upload_context = 0;
  google_breakpad::CrashGenerationServer server(pipeName,
                                                securityAttributes,
                                                onClientConnectedCallback,
                                                connect_context,
                                                onClientDumpRequestCallback,
                                                dump_context,
                                                onClientExitedCallback,
                                                exit_context,
                                                onClientUploadRequestCallback,
                                                upload_context,
                                                generate_dumps,
                                                &dump_path);

  std::cout << "Starting CrashGenerationServer..." << std::endl;

  if (!server.Start()) {
      std::cerr << "failed to start" << std::endl;
    return -1;
  }

  std::cout << "\tsuccess!" << std::endl;

#ifdef WINDOWS_APP
  ::MSG msg;
  for (;;)
  {
    std::cout << "alive... " << std::endl;

    ::BOOL messageAvailable = ::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE);
    if (messageAvailable == 0) {
      Sleep(1000);
      continue;
    }

    ::BOOL success = GetMessage(&msg, NULL, 0, 0);
    if (0 != success) {
        std::cerr << "Unable to get next Windows message. Exiting" << std::endl;
      break;
    }

    if (-1 == success) {
        std::cerr << "Windowing message pump error" << std::endl;
      return -1;
    }
    else {
      std::cout << "WM message " << msg.message << std::endl;

      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  const int exitCode = (int)msg.wParam;
  std::cout << "Exiting..." << exitCode << std::endl;
  return exitCode;
#else
  for (;;)
  {
    std::cout << "alive... " << std::endl;
    ::Sleep(1000);
  }
#endif
}
