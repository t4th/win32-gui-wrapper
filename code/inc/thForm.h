#pragma once
#include "thWindow.h"
#include "thMenu.h"
#include "thWindowStyles.h"

class thForm : public thWindow
{
    friend LRESULT CALLBACK WinProc( HWND, UINT, WPARAM, LPARAM);
    friend class thMDIClient;
public:
                            thForm( thWindow * a_pParent = nullptr, int a_posX = CW_USEDEFAULT, int a_posY = CW_USEDEFAULT);
                            ~thForm();

    thResizable             Resizable;

                            /* Menu */
    void                    SetMenu( thMenu * const);
    BOOL                    IsMenuEnabled( void);
    void                    ClearMenu( void);

    thEventCallbackFunc_t   OnClose{nullptr};

private:
    static int              m_indexPool;
    thMenu *                m_menu;
    HWND                    m_hMDIClient;

    virtual void            init();
    void                    registerClass();

    LRESULT                 onCreate();                 // WM_CREATE
    LRESULT                 onClose();                  // WM_CLOSE
    LRESULT                 onGetMinMax( LPARAM);    // WM_GETMINMAXINFO

    virtual LRESULT         processMenuCommandMessage( HWND, UINT, WPARAM, LPARAM);

    int                     getDebugIndex();
};

