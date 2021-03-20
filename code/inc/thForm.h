#pragma once
#include "thWindow.h"
#include "thMenu.h"
#include "thWindowStyles.h"

class thForm : public thWindow
{
    friend LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);
    friend class thMDIClient;
protected:
private:
    static int              m_indexPool;
    thMenu *                m_menu;
    HWND                    m_hMDIClient;

    virtual void            init();
    void                    registerClass();

    LRESULT                 onCreate();                 // WM_CREATE
//    LRESULT                 onNCCreate();               // WM_NCCREATE
    LRESULT                 onClose();                  // WM_CLOSE
    LRESULT                 onGetMinMax(LPARAM);    // WM_GETMINMAXINFO
#if 0
    virtual LRESULT         onResize(HWND, WPARAM, LPARAM); // WM_SIZE
#endif

    virtual LRESULT         processMenuCommandMessage(HWND, UINT, WPARAM, LPARAM);

    int                     getDebugIndex();

#if 0
    int                     m_dOldWidth;
    int                     m_dOldHeight;
#endif
public:
                            thForm() = delete;
                            thForm(thWindow *, int, int);
                            ~thForm();

    thResizable             Resizable;

                            /* Menu */
    void                    SetMenu(thMenu * const);
    BOOL                    IsMenuEnabled(void);
    void                    ClearMenu(void);

    thEventCallbackFunc_t   OnClose{nullptr};
};

