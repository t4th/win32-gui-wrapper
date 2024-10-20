#pragma once
#include "thWindow.h"
#include "thMenu.h"
#include "thMDIClient.h"

LRESULT CALLBACK MDIChildProc(HWND, UINT, WPARAM, LPARAM);

class thMDIChild : public thWindow
{
    friend LRESULT CALLBACK MDIChildProc(HWND, UINT, WPARAM, LPARAM);
protected:
private:
    static int              m_indexPool;
    thMenu *                m_menu{nullptr};

    virtual void            init();
    void                    registerClass();

    LRESULT                 onCreate();                 // WM_CREATE
    LRESULT                 onNCCreate();               // WM_NCCREATE
    LRESULT                 onClose();                  // WM_CLOSE

    int                     getDebugIndex();
public:
                            thMDIChild() = delete;
                            thMDIChild(thMDIClient * a_pParent = nullptr, int a_posX = CW_USEDEFAULT, int a_posY = CW_USEDEFAULT);
                            ~thMDIChild();

#if 0
    /* Menu */
    void                    SetMenu(thMenu * const);
    BOOL                    IsMenuEnabled(void);
    void                    ClearMenu(void);
#endif

    thEventCallbackFunc_t   OnClose{nullptr};
};