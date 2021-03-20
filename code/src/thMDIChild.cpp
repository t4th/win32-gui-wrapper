#include "thMDIChild.h"

/* Defines */
#define CLASS_NAME TEXT("thMDIChild")
#define WIN32_CLASS_NAME CLASS_NAME
#define DEFAULT_TEXT TEXT("thMDIChild")

#define DEFAULT_WIDTH  200
#define DEFAULT_HEIGHT 200

/* Local Memory */
int thMDIChild::m_indexPool = 1;

void thMDIChild::init()
{
    TH_ENTER_FUNCTION;
    this->m_name = CLASS_NAME;

    this->m_sWindowArgs.dwExStyle =      WS_EX_MDICHILD; //http://msdn.microsoft.com/en-us/library/windows/desktop/ff700543%28v=vs.85%29.aspx
    this->m_sWindowArgs.lpClassName =    WIN32_CLASS_NAME;
    this->m_sWindowArgs.lpWindowName =   DEFAULT_TEXT;
    this->m_sWindowArgs.dwStyle =        WS_VISIBLE | WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN; //http://msdn.microsoft.com/en-us/library/windows/desktop/ms632600%28v=vs.85%29.aspx
    this->m_sWindowArgs.nWidth =         DEFAULT_WIDTH;
    this->m_sWindowArgs.nHeight =        DEFAULT_HEIGHT;
    this->m_sWindowArgs.hMenu =          0;
    this->m_sWindowArgs.lpParam =        this;
    TH_LEAVE_FUNCTION;
}

void thMDIChild::registerClass()
{
    TH_ENTER_FUNCTION;
    static bool once = true; // Register this class once in a lifetime of application. Lazy implementation.

    if ( true == once)
    {
        WNDCLASSEX wClass = { 0 };

        wClass.cbSize =         sizeof(WNDCLASSEX);
        wClass.style =          0;//CS_HREDRAW | CS_VREDRAW;
        wClass.lpfnWndProc =    static_cast<WNDPROC>(MDIChildProc);
        wClass.cbClsExtra =     NULL;
        wClass.cbWndExtra =     sizeof(thMDIChild*); // 4 bytes for 'this' pointer
        wClass.hInstance =      this->m_sWindowArgs.hInstance;
        wClass.hIcon =          NULL;
        wClass.hCursor =        LoadCursor(NULL, IDC_ARROW);
        wClass.hbrBackground =  (HBRUSH)COLOR_WINDOW;
        wClass.lpszMenuName =   NULL;
        wClass.lpszClassName =  this->m_sWindowArgs.lpClassName;
        wClass.hIconSm =        NULL;
    
        if (0 == RegisterClassEx(&wClass)) {
            MSG_WARNING(TEXT("RegisterClassEx error: 0x%X"), GetLastError());
            //todo: add UnregisterClass 
        }
        else {
            MSG_SUCCESS(TEXT("Successfully registerd class with RegisterClassEx"));
            once = false;
            //fMainApplicationWindow = true; //firstly created form is the main form
        }
    }
    TH_LEAVE_FUNCTION;
}

thMDIChild::thMDIChild(thMDIClient * a_pParent = NULL, int a_posX = CW_USEDEFAULT, int a_posY = CW_USEDEFAULT)
    :
    m_menu(NULL),
    thWindow(a_pParent, a_posX, a_posY)
{
    TH_ENTER_FUNCTION;
    
    this->init();
    this->registerClass();
    this->create();

    TH_LEAVE_FUNCTION;
}

thMDIChild::~thMDIChild()
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

LRESULT thMDIChild::onCreate()
{
    TH_ENTER_FUNCTION;
    LRESULT tResult = 0;
    TH_LEAVE_FUNCTION;
    return tResult;
}

// NCCreate must return 1! If not, CreateWindowEx won't create window and will return NULL
LRESULT thMDIChild::onNCCreate()
{
    TH_ENTER_FUNCTION;
    LRESULT tResult = 1;
    TH_LEAVE_FUNCTION;
    return tResult;
}

LRESULT thMDIChild::onClose()
{
    MSG_LOG(TEXT("%s::onClose() - Enter"), this->m_name.c_str());
    LRESULT tResult = 0;

    if (OnClose) {
        tResult = OnClose(this,{});
    }

    MSG_LOG(TEXT("%s::onClose() - Leave"), this->m_name.c_str());
    return tResult;
}

#if 0
// If an application processes this message, it should return zero. 
LRESULT thMDIChild::onResize(HWND a_hwnd)
{
    //TH_ENTER_FUNCTION;
    LRESULT tResult = 1;
    //TH_LEAVE_FUNCTION;
    return tResult;
}
#endif

#if 0
LRESULT thMDIChild::processCommandMessage(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    TH_ENTER_FUNCTION;
    LRESULT     tResult = 0;
    
//    tResult = SendMessage((HWND)a_lParam, WM_COMMAND, a_wParam, a_lParam);

    TH_LEAVE_FUNCTION;
    return tResult;
}
#endif

#if 0
// wParam - the zero - based index of the item selected
// lParam - a handle to the menu for the item selected
LRESULT thMDIChild::processMenuCommandMessage(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    TH_ENTER_FUNCTION;
    LRESULT     tResult = 0;

    if (m_menu) {
        tResult = m_menu->processCommandMessage(a_hwnd, a_uMsg, a_wParam, a_lParam);
    }

    if (FALSE == tResult) {
        if (PopupMenu) {
            tResult = PopupMenu->processCommandMessage(a_hwnd, a_uMsg, a_wParam, a_lParam);
        }
    }

    TH_LEAVE_FUNCTION;
    return tResult;
}
#endif

int thMDIChild::getDebugIndex()
{
    TH_ENTER_FUNCTION;
    int dReturn = this->m_indexPool;
    this->m_indexPool++;
    TH_LEAVE_FUNCTION;
    return dReturn;
}

#if 0
void thMDIChild::SetMenu(thMenu * const a_pMenu)
{
    TH_ENTER_FUNCTION;

    if (NULL != a_pMenu) {
        if (a_pMenu != m_menu) {
            if (NULL != a_pMenu->m_hMenuHandle) {
                BOOL fResult = false;

                m_menu = a_pMenu;
                fResult = ::SetMenu(this->m_hWinHandle, a_pMenu->m_hMenuHandle);

                if (false == fResult) {
                    MSG_ERROR(TEXT("SetMenu failed with error =  0x%X"), GetLastError());
                }
                else {
                    //todo:
                    //this->addChildrenWindow(pMenu);
                    //DrawMenuBar(this->hWinHandle);
                    m_menu->m_hParentWindow = this->m_hWinHandle;
                    DrawMenuBar(this->m_hWinHandle);
                }
            }
            else {
                MSG_ERROR(TEXT("Provided thMenu is invalid (wrong handle)"));
            }
        }
    }
    else {
        MSG_ERROR(TEXT("Empty input pointer!"));
    }

    TH_LEAVE_FUNCTION;
}

void thMDIChild::ClearMenu(void)
{
    TH_ENTER_FUNCTION;
    BOOL fResult = false;

    fResult = ::SetMenu(this->m_hWinHandle, NULL);

    if (false == fResult) {
        MSG_ERROR(TEXT("SetMenu failed with error =  0x%X"), GetLastError());
    }
    else {
        this->m_menu->m_hParentWindow = NULL;
        m_menu = NULL;
    }

    TH_LEAVE_FUNCTION;
}

BOOL thMDIChild::IsMenuEnabled(void)
{
    TH_ENTER_FUNCTION;
    BOOL    fResult = false;
    HMENU   hMenu = 0;

    hMenu = GetMenu(this->m_hWinHandle);

    if (hMenu) {
        fResult = true;
    }

    TH_LEAVE_FUNCTION;
    return fResult;
}
#endif

LRESULT CALLBACK MDIChildProc(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    LONG_PTR        pUserData = NULL;
    LRESULT         tResult = 0;
    thMDIChild *    pMDIChild = NULL;

    pUserData = GetWindowLongPtr(a_hwnd, GWLP_USERDATA);
    pMDIChild = reinterpret_cast<thMDIChild *>(pUserData);

    if (WM_NCCREATE == a_uMsg) {
        LPCREATESTRUCT cs = (LPCREATESTRUCT)a_lParam;
        if (cs->lpCreateParams) {
            MDICREATESTRUCT * pMDIinfo = NULL;

            pMDIinfo = reinterpret_cast<MDICREATESTRUCT*>(cs->lpCreateParams);
            SetWindowLongPtr(a_hwnd, GWLP_USERDATA, pMDIinfo->lParam);
            pMDIChild = reinterpret_cast<thMDIChild*>(pMDIinfo->lParam);
        }
        else {
            MSG_ERROR(TEXT("CRITICAL ERROR - no 'this' pointer in CreateWindowEx Param!"));
        }
    }

    if (pMDIChild) {
        tResult = pMDIChild->processMessage(a_hwnd, a_uMsg, a_wParam, a_lParam);

        // msgs that must be passed through or msgs not handled
        // https://msdn.microsoft.com/en-us/library/windows/desktop/ms644925%28v=vs.85%29.aspx
        if (0 == tResult || WM_SIZE == a_uMsg
            || WM_CHILDACTIVATE == a_uMsg || WM_GETMINMAXINFO == a_uMsg
            || WM_MENUCHAR == a_uMsg || WM_MOVE == a_uMsg
            || WM_SETFOCUS == a_uMsg || WM_SYSCOMMAND == a_uMsg
            ) {
            tResult = DefMDIChildProc(a_hwnd, a_uMsg, a_wParam, a_lParam);
        }
    }
    else {
        tResult = DefMDIChildProc(a_hwnd, a_uMsg, a_wParam, a_lParam);
    }

    return tResult;
}