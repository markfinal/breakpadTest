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

int main(int argc, char* argv[])
{
    CrashHandler::Parameters params(argc, argv);
    params._minidumpDir = L"c:\\temp";
    params._oopExecutable = "CrashServer.exe";
    CrashHandler crashHandler(params);

    int result = 0;

    std::cout << "Loading plugin..." << std::endl;
    LoadPlugin plugin("CrashPlugin.dll");
    IPlugin *instance = plugin.GetInterface();

    std::cout << "Initiating crash..." << std::endl;

#if 1
    instance->DoStuff();
#else
#if 1
    result = CrashAndBurn(0);
#else
    *((int*)(0)) = 3;
#endif
#endif

    std::cout << "Finished..." << std::endl;

    return result;
}
