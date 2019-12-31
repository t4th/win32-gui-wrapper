#pragma once
#include "thTypes.h"
#include "thConfig.h"

/* Callback event parameters */
typedef struct
{
    UINT    msg;
    WPARAM  wParam;
    LPARAM  lParam;
} thEventParams_t;

class thObject;

// TODO: dont use windows specyfic types in output API!
typedef LRESULT (*thEventCallbackFunc_t)(thObject * const sender, thEventParams_t info);

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

