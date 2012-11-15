#include "crashplugin.h"

namespace
{

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning( disable : 4717 ) // warning C4717: '`anonymous namespace'::StackOverflow' : recursive on all control paths, function will cause runtime stack overflow
#pragma warning( disable : 4718 ) // warning C4718: '`anonymous namespace'::StackOverflow' : recursive call has no side effects, deleting
#endif
 
int StackOverflow(int x)
{
    return StackOverflow(x + 1) + StackOverflow(x + 1);
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

} // anonymous namespace

IPlugin *GetInterface()
{
    IPlugin *plugin = new CrashPlugin;
    return plugin;
}

void CrashPlugin::Execute(int mode)
{
    if (0 == mode)
    {
        (*(int*)3) = 0;
    }
    else if (1 == mode)
    {
        StackOverflow(0);
    }
}
