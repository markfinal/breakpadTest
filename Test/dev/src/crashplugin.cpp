#include "crashplugin.h"

IPlugin *GetInterface()
{
    IPlugin *plugin = new CrashPlugin;
    return plugin;
}

void CrashPlugin::DoStuff()
{
    (*(int*)3) = 0;
}
