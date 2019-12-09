#include "thForm.h"

/* Defines */
#define CLASS_NAME TEXT("thForm")
#define WIN32_CLASS_NAME CLASS_NAME
#define DEFAULT_TEXT TEXT("Caption")

#define DEFAULT_WIDTH  200
#define DEFAULT_HEIGHT 200

/* Local Memory */
int thForm::m_indexPool = 1;

/* Prototypes */

// keep dx and dy after thForm resize
typedef struct
{
    int Width;
    int Height;
} thDifference_t;

thForm::thForm() : m_menu(NULL), m_hMDIClient(NULL), OnClose(NULL), thWindow(NULL, CW_USEDEFAULT, CW_USEDEFAULT),
Resizable(*this)
// m_dOldWidth(0), m_dOldHeight(0)
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
}

void thForm::init()
{
    TH_ENTER_FUNCTION;

    this->m_name = CLASS_NAME;

    this->m_sWindowArgs.dwExStyle =      0; //http://msdn.microsoft.com/en-us/library/windows/desktop/ff700543%28v=vs.85%29.aspx
    this->m_sWindowArgs.lpClassName =    WIN32_CLASS_NAME;
    this->m_sWindowArgs.lpWindowName =   DEFAULT_TEXT;
    // (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX); for non resizing instead of WS_OVERLAPPEDWINDOW (remove WS_THICKFRAME - xor)
    this->m_sWindowArgs.dwStyle =        WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW; //http://msdn.microsoft.com/en-us/library/windows/desktop/ms632600%28v=vs.85%29.aspx
    this->m_sWindowArgs.nWidth =         DEFAULT_WIDTH;
    this->m_sWindowArgs.nHeight =        DEFAULT_HEIGHT;
    this->m_sWindowArgs.hMenu =          0;
    this->m_sWindowArgs.lpParam =        this;
    TH_LEAVE_FUNCTION;
}

void thForm::registerClass()
{
    TH_ENTER_FUNCTION;
    WNDCLASSEX wClass = { 0 };

    wClass.cbSize =         sizeof(WNDCLASSEX);
    wClass.style =          0;//CS_HREDRAW | CS_VREDRAW;
    wClass.lpfnWndProc =    static_cast<WNDPROC>(WinProc);
    wClass.cbClsExtra =     NULL;
    wClass.cbWndExtra =     sizeof(thForm*); // 4 bytes for 'this' pointer
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
    }
    TH_LEAVE_FUNCTION;
}

thForm::thForm(thWindow * a_pParent = NULL, int a_posX = CW_USEDEFAULT, int a_posY = CW_USEDEFAULT)
    :
    thWindow(a_pParent, a_posX, a_posY),
    m_menu(NULL),
    m_hMDIClient(NULL),
    OnClose(NULL),
    Resizable(*this)
// m_dOldWidth(0), m_dOldHeight(0),
{
    TH_ENTER_FUNCTION;
    
    this->init();
    this->registerClass();
    this->create();

#if 0
    m_dOldHeight = this->Height;
    m_dOldWidth = this->Width;
#endif

    TH_LEAVE_FUNCTION;
}

thForm::~thForm()
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

LRESULT thForm::onCreate()
{
    TH_ENTER_FUNCTION;
    LRESULT tResult = 0;


    TH_LEAVE_FUNCTION;
    return tResult;
}

//LRESULT thForm::onNCCreate()
//{
//    TH_ENTER_FUNCTION;
//    LRESULT tResult = 0;
//    TH_LEAVE_FUNCTION;
//    return tResult;
//}

LRESULT thForm::onClose()
{
//  TH_ENTER_FUNCTION;
    MSG_LOG(TEXT("%s::onClose() - Enter"), this->m_name.c_str());
    LRESULT tResult = 0;

    if (OnClose) {
        tResult = OnClose(this);
    }

    MSG_LOG(TEXT("%s::onClose() - Leave"), this->m_name.c_str());
//  TH_LEAVE_FUNCTION;
    return tResult;
}

// thForm use client size instead of window size
// Because of that it need specialized onGetMinMax
LRESULT thForm::onGetMinMax(LPARAM a_lParam)
{
    //TH_ENTER_FUNCTION;
    LRESULT      tResult = 1;
    MINMAXINFO * sInfo = 0;

    //TODO: change this->Constraints variables to window sizes as user is expecting client sizes
#if 0
    sInfo = reinterpret_cast<MINMAXINFO*>(a_lParam);

    if (sInfo) {
        if (this->Constraints.MaxWidth) {
            sInfo->ptMaxTrackSize.x = this->Constraints.MaxWidth;
            tResult = 0;
        }

        if (this->Constraints.MaxHeight) {
            sInfo->ptMaxTrackSize.y = this->Constraints.MaxHeight;
            tResult = 0;
        }

        if (this->Constraints.MinWidth) {
            sInfo->ptMinTrackSize.x = this->Constraints.MinWidth;
            tResult = 0;
        }

        if (this->Constraints.MinHeight) {
            sInfo->ptMinTrackSize.y = this->Constraints.MinHeight;
            tResult = 0;
        }
    }
#endif 
    //TH_LEAVE_FUNCTION;
    return tResult;
}

#if 0
// Children windows enumeration callback
// to stop enumeration, it must return FALSE.
// pointer typedef: WNDENUMPROC
// a_lParam - low-order word is new width dx, high-order word is new height dy
BOOL CALLBACK EnumChildProc(HWND a_hWnd, LPARAM a_lParam)
{
    BOOL        fResult = TRUE;// TRUE;
    LONG_PTR    pUserData = NULL;
    thWindow *  pWindow = NULL;

    //MSG_LOG(TEXT("EnumChildProc %X"), a_hWnd);

    pUserData = GetWindowLongPtr(a_hWnd, GWLP_USERDATA);
    pWindow = reinterpret_cast<thWindow *>(pUserData);

    if (pWindow && a_lParam) {
        thDifference_t * pDifference = NULL;

        pDifference = (thDifference_t*)a_lParam;

        if (TRUE == pWindow->Anchors.Right) {
            pWindow->Width += pDifference->Width;
        }

        if (TRUE == pWindow->Anchors.Bottom) {
            pWindow->Height += pDifference->Height;
        }

        //MSG_SUCCESS(TEXT("w=%d, h=%d :: new w=%d, h=%d"), dWidth, dHeight, dNewWidth, dNewHeight);
    }

    return fResult;
}

// If an application processes this message, it should return zero.
// a_wParam - The type of resizing requested
// a_lParam - low-order word is new width, high-order word is new height
LRESULT thForm::onResize(HWND a_hwnd, WPARAM a_wParam, LPARAM a_lParam)
{
    //TH_ENTER_FUNCTION;
    LRESULT tResult = 0;

    // new width and height
    int dNewWidth = 0;
    int dNewHeight = 0;

    dNewWidth = LOWORD(a_lParam);
    dNewHeight = HIWORD(a_lParam);

    // find difference
    thDifference_t sDifference = { 0 };

    if (dNewWidth > m_dOldWidth) { // increase
        sDifference.Width = dNewWidth - m_dOldWidth;
        this->m_dOldWidth += sDifference.Width;
    }
    else { // decrease
        sDifference.Width = m_dOldWidth - dNewWidth;
        this->m_dOldWidth -= sDifference.Width;
        sDifference.Width = 0 - sDifference.Width;
    }

    if (dNewHeight > m_dOldHeight) { // increase
        sDifference.Height = dNewHeight - m_dOldHeight;
        this->m_dOldHeight += sDifference.Height;
    }
    else { // decrease
        sDifference.Height = m_dOldHeight - dNewHeight;
        this->m_dOldHeight -= sDifference.Height;
        sDifference.Height = 0 - sDifference.Height;
    }

    //MSG_SUCCESS(TEXT("w=%d, h=%d"), sDifference.Width, sDifference.Height);

    EnumChildWindows(this->m_hWinHandle, EnumChildProc, (LPARAM)&sDifference);

    //TH_LEAVE_FUNCTION;
    return tResult;
}
#endif

LRESULT thForm::processMenuCommandMessage(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
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

int thForm::getDebugIndex()
{
    TH_ENTER_FUNCTION;
    int dReturn = this->m_indexPool;
    this->m_indexPool++;
    TH_LEAVE_FUNCTION;
    return dReturn;
}

void thForm::SetMenu(thMenu * const a_pMenu)
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

void thForm::ClearMenu(void)
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

BOOL thForm::IsMenuEnabled(void)
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