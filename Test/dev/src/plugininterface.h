#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

class IPlugin
{
public:
    virtual void DoStuff() = 0;
};

typedef IPlugin* (*GetInterfaceFn)();

extern "C"
{

#ifdef OPUS_DYNAMICLIBRARY
extern __declspec(dllexport) IPlugin *GetInterface();
#else
extern __declspec(dllimport) IPlugin *GetInterface();
#endif

}

#endif // PLUGININTERFACE_H
