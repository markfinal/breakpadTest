#ifndef CRASHPLUGIN_H
#define CRASHPLUGIN_H

#include "plugininterface.h"

class CrashPlugin : public IPlugin
{
public:
    virtual void Execute(int);
};

#endif // CRASHPLUGIN_H
