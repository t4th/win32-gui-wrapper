#pragma once
#include "thWindow.h"

class thLabel : public thWindow
{
protected:
    virtual int             getDebugIndex();
private:
    static int              m_indexPool;

    LRESULT                 processCommandMessage(HWND, UINT, WPARAM, LPARAM);
    LRESULT                 processNotifyMessage(HWND, UINT, WPARAM, LPARAM);
public:
                            thLabel() = delete;
                            thLabel(thWindow *, int, int);
                            ~thLabel();

    thEventCallbackFunc_t   OnClick{nullptr};
    thEventCallbackFunc_t   OnDoubleClicked{nullptr};
    thEventCallbackFunc_t   OnDisable{nullptr};
    thEventCallbackFunc_t   OnEnable{nullptr};
};

