#pragma once
#include "thWindow.h"
#include "thMenu.h"
#include "thMDIClient.h"

class thMDIChild : public thWindow
{
    friend LRESULT CALLBACK MDIChildProc(HWND, UINT, WPARAM, LPARAM);
protected:
private:
    static int          m_indexPool;
    BOOL                m_isFirstRegWin; // is first registered window?
    thMenu *            m_menu;

    virtual void        init();
    void                registerClass();

    LRESULT             onCreate();                 // WM_CREATE
    LRESULT             onNCCreate();               // WM_NCCREATE
    LRESULT             onClose();                  // WM_CLOSE

    //virtual LRESULT processCommandMessage(HWND, UINT, WPARAM, LPARAM);
    //virtual LRESULT processMenuCommandMessage(HWND, UINT, WPARAM, LPARAM);

    int                 getDebugIndex();
                        thMDIChild();
public:
                        thMDIChild(thMDIClient *, int, int);
                        ~thMDIChild();

#if 0
    /* Menu */
    void                SetMenu(thMenu * const);
    BOOL                IsMenuEnabled(void);
    void                ClearMenu(void);
#endif

    thCallbackFunc_t    OnClose;
};