#include "crashhandler.h"
#include "plugininterface.h"

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

class LoadPlugin
{
public:
    LoadPlugin(const std::string &pathname)
    {
        _handle = ::LoadLibrary(pathname.c_str());
        if (0 == _handle)
        {
            throw 1;
        }
        
        this->_getInterfaceFn = reinterpret_cast<GetInterfaceFn>(::GetProcAddress(this->_handle, "GetInterface"));
        if (0 == this->_getInterfaceFn)
        {
            throw 1;
        }
    }

    ~LoadPlugin()
    {
        if (0 != this->_handle)
        {
            ::CloseHandle(this->_handle);
            this->_handle = 0;
        }
    }

    IPlugin *GetInterface()
    {
        return (*this->_getInterfaceFn)();
    }

private:
    ::HMODULE _handle;
    GetInterfaceFn _getInterfaceFn;
};

enum Mode
{
    MODE_INVALID = -1,
    MODE_NULLPOINTER = 0,
    MODE_STACKOVERFLOW = 1,
};

std::string ModeDescription(Mode m)
{
    switch (m)
    {
    case MODE_INVALID:
        return "Invalid mode";
        
    case MODE_NULLPOINTER:
        return "Null pointer";

    case MODE_STACKOVERFLOW:
        return "Stack overflow";
    }

    return "";
}

int main(int argc, char* argv[])
{
    bool usePlugin = false;
    Mode mode = MODE_INVALID;
    for (int i = 0; i < argc; ++i)
    {
        if (std::string(argv[i]) == "--nullpointer")
        {
            mode = MODE_NULLPOINTER;
        }
        else if (std::string(argv[i]) == "--stackoverflow")
        {
            mode = MODE_STACKOVERFLOW;
        }
        else if (std::string(argv[i]) == "--plugin")
        {
            usePlugin = true;
        }
    }
    if (MODE_INVALID == mode)
    {
        std::cerr << "No crash mode has been set" << std::endl;
        return -1;
    }

    CrashHandler::Parameters params(argc, argv);
    params._minidumpDir = L"c:\\temp";
    params._oopExecutable = "CrashServer.exe";
    CrashHandler crashHandler(params);

    int result = 0;

    if (usePlugin)
    {
        std::cout << "Loading plugin..." << std::endl;
        LoadPlugin plugin("CrashPlugin.dll");
        IPlugin *instance = plugin.GetInterface();

        std::cout << "Initiating plugin crash for " << ModeDescription(mode) << std::endl;

        instance->Execute(mode);
    }
    else
    {
        std::cout << "Initiating main executable crash for " << ModeDescription(mode) << std::endl;

        if (MODE_NULLPOINTER == mode)
        {
            *((int*)(0)) = 3;
        }
        else if (MODE_STACKOVERFLOW == mode)
        {
            result = CrashAndBurn(0);
        }
    }

    std::cout << "Finished..." << std::endl;

    return result;
}
