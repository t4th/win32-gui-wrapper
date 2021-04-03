#pragma once
#include "thWindow.h"
#include "thWindowStyles.h"

/*
    based on simpleGrid
    preprocessor: _CRT_SECURE_NO_WARNINGS
    external dependancies: uxtheme.lib
*/

class thStringGrid : public thWindow
{
protected:
    virtual int             getDebugIndex();
private:
    static int              m_indexPool;

    LRESULT                 processCommandMessage(HWND, UINT, WPARAM, LPARAM);
    LRESULT                 processNotifyMessage(HWND, UINT, WPARAM, LPARAM);
public:
                            thStringGrid() = delete;
                            thStringGrid(thWindow * a_pParent, int a_posX = CW_USEDEFAULT, int a_posY = CW_USEDEFAULT);
                            ~thStringGrid();

    //thBS_DefaultPush        DefaultPushStyle; // Change Button look

    //thEventCallbackFunc_t   OnClick; // = BN_CLICKED,
    //thEventCallbackFunc_t   OnPaint; // = BN_PAINT,
    //thEventCallbackFunc_t   OnHilite; // = BN_HILITE,
    //thEventCallbackFunc_t   OnUnhilite; // = BN_UNHILITE,
    //thEventCallbackFunc_t   OnDisable; // = BN_DISABLE,
    //thEventCallbackFunc_t   OnPushed; // = BN_HILITE,
    //thEventCallbackFunc_t   OnUnpushed; // = BN_UNHILITE,
    //thEventCallbackFunc_t   OnDoubleClicked; // = BN_DOUBLECLICKED,
    //thEventCallbackFunc_t   OnSetFocus; // = BN_SETFOCUS,
    //thEventCallbackFunc_t   OnKillFocus; // = BN_KILLFOCUS

};

