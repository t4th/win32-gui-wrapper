#pragma once
#include "thTypes.h"
#include "thConfig.h"

/* Callbcak event parameters */
typedef struct
{
    UINT    msg;
    WPARAM  wParam;
    LPARAM  lParam;
} thEventParams_t;

class thObject;

typedef void(*thEventCallbackFunc_t)(thObject * const sender, thEventParams_t info);

class thObject
{
protected:
    HMENU                           m_id;
    thString                        m_name; // set in c-tor of last class in derivating chain
private:
    static int                      m_idPool;
public:
                                    thObject();
    virtual                         ~thObject();
};

