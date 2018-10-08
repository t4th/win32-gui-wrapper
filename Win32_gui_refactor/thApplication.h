#pragma once
#include "thTypes.h"
#include "thConfig.h"

class thApplication
{
protected:
private:
public:
    thApplication();
    virtual ~thApplication();

    //virtual void Terminate(int) = 0;
    virtual int Run() = 0;
};

