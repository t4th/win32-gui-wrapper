#pragma once
#include "thTypes.h"
#include "thConfig.h"

#include <functional>

/* Callback event parameters */

// TODO: dont use windows specyfic types in output API!
typedef struct
{
    UINT    msg;
    WPARAM  wParam;
    LPARAM  lParam;
} thEventParams_t;

typedef LRESULT thResult_t;

class thObject;

typedef std::function<thResult_t (thObject * const sender, thEventParams_t info)> thEventCallbackFunc_t;

class thObject
{
protected:
    DWORD                           m_id;   // windows limit this value to WORD size, but DWORD is used for easy limit check
    thString                        m_name; // set in c-tor of last class in derivating chain
private:
    static DWORD                    m_idPool;
public:
                                    thObject();
    virtual                         ~thObject();
};

