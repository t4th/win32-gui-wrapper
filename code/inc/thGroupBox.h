#pragma once
#include "thButton.h"

class thGroupBox : public thWindow
{
protected:
private:
    static int              m_indexPool;

    int                     getDebugIndex();
    LRESULT                 processCommandMessage(HWND, UINT, WPARAM, LPARAM);
    LRESULT                 processNotifyMessage(HWND, UINT, WPARAM, LPARAM);
public:
                            thGroupBox() = delete;
                            thGroupBox(thWindow * a_pParent, int a_posX = CW_USEDEFAULT, int a_posY = CW_USEDEFAULT);
                            ~thGroupBox();

};

