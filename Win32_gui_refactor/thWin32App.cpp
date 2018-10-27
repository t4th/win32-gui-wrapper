#include "thWin32App.h"
#include "thWindow.h"
#include "thMDIChild.h"

thWin32App::thWin32App() : m_hInstance(0), m_hAccel(0), m_mainWindow(0), m_MDIclient(0)
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
}

thWin32App::~thWin32App()
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
}

void thWin32App::Init(HINSTANCE a_hInstance)
{
    TH_ENTER_FUNCTION;
    m_hInstance = a_hInstance;

#if 0
    m_hAccel = LoadAccelerators(m_hInstance, APPLICATION_NAME);

    if (0 == m_hAccel) {
        MSG_ERROR(TEXT("LoadAccelerators failed with error: 0x%X"), GetLastError());
    }
#endif

    TH_LEAVE_FUNCTION;
}

void thWin32App::Terminate(int a_nExitCode = 0)
{
    TH_ENTER_FUNCTION;

    ::PostQuitMessage(a_nExitCode);

    TH_LEAVE_FUNCTION;
}
extern thMDIClient * gmdic;
int thWin32App::Run()
{
    TH_ENTER_FUNCTION;

    MSG     msg = { 0 };
    BOOL    fMsgTranslated = FALSE;
    HACCEL  hAccel;

    if (0 == m_hInstance) {
        m_hInstance = GetModuleHandle(NULL);
    }

    this->OnCreate();

    // Load accelerator table

    hAccel = LoadAccelerators(m_hInstance, APPLICATION_NAME);

    while (GetMessage(&msg, NULL, 0, 0) != FALSE) {
        fMsgTranslated = FALSE; //http://msdn.microsoft.com/en-us/library/windows/desktop/ms644926%28v=vs.85%29.aspx

        if (gmdic) {
            fMsgTranslated = TranslateMDISysAccel(gmdic->GetHandle(), &msg);
        }

        if (m_mainWindow && FALSE == fMsgTranslated) {
            fMsgTranslated = TranslateAccelerator(m_mainWindow->GetHandle(), hAccel, &msg);
        }

        if (FALSE == fMsgTranslated)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    this->OnDestroy();

    TH_LEAVE_FUNCTION;

    return 0;
}