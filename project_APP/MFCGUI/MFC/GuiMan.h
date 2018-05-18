

#ifndef GUIMAN_H
#define GUIMAN_H

#ifndef CPP_API
    #define CPP_API __declspec(dllexport)
#endif

struct CPP_API GuiMan
{
    GuiMan();
    virtual ~GuiMan();
    void initliaze();
};

extern "C" __declspec(dllexport) void GUIMFC(void);



#endif