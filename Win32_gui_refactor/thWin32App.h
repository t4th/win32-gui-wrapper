#pragma once
#include "thApplication.h"
#include "thWin32Logger.h"
#include "thWindow.h"

#define APPLICATION_NAME TEXT("Win32_gui_refactor")

class thWin32App : public thApplication
{
protected:
private:
    HINSTANCE       m_hInstance;
    HACCEL          m_hAccel;
    thWindow *      m_mainWindow;
    HWND            m_MDIclient;
public:
                    thWin32App();
                    ~thWin32App();

    void            OnCreate();
    void            OnDestroy();

    void            Init(HINSTANCE);
    static void     Terminate(int);
    int             Run();
};

