#include "thMemo.h"

/* Defines */
#define CLASS_NAME TEXT("thMemo")
#define WIN32_CLASS_NAME TEXT("EDIT")
#define DEFAULT_TEXT TEXT("")

#define DEFAULT_WIDTH  75
#define DEFAULT_HEIGHT 75

/* Local Memory */
int thMemo::m_indexPool = 1;

/* Prototypes */

thMemo::thMemo(thWindow * a_pParent, int a_posX = CW_USEDEFAULT, int a_posY = CW_USEDEFAULT) : thWindow(a_pParent, a_posX, a_posY)
{
    TH_ENTER_FUNCTION;

    this->m_name = CLASS_NAME;

    this->m_sWindowArgs.dwExStyle =     WS_EX_CLIENTEDGE;
    this->m_sWindowArgs.lpClassName =   WIN32_CLASS_NAME;
    this->m_sWindowArgs.lpWindowName =  DEFAULT_TEXT;
    this->m_sWindowArgs.dwStyle =       WS_TABSTOP | WS_VISIBLE | WS_CHILD | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_AUTOHSCROLL;
    //    | ES_AUTOHSCROLL | WS_HSCROLL;
    //ES_READONLY WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL
    this->m_sWindowArgs.nWidth =        DEFAULT_WIDTH;
    this->m_sWindowArgs.nHeight =       DEFAULT_HEIGHT;
    this->m_sWindowArgs.hMenu =         reinterpret_cast<HMENU>(this->m_id);;
    this->m_sWindowArgs.lpParam =       this;

    this->create();

    BOOL fResult = SetWindowSubclass(this->m_hWinHandle, ChildWindProc, 0, (DWORD_PTR)this);

    if (FALSE == fResult) {
        MSG_ERROR(TEXT("SetWindowSubclass failed with error = 0x%X"), GetLastError());
    }

    TH_LEAVE_FUNCTION;
}

thMemo::~thMemo()
{
    TH_ENTER_OBJECT_FUNCTION;
    TH_LEAVE_OBJECT_FUNCTION;
}

void thMemo::ScrollDown()
{
    SendMessage(this->m_hWinHandle, LOWORD(WM_VSCROLL), SB_BOTTOM, 0);
}

int thMemo::getDebugIndex()
{
    TH_ENTER_OBJECT_FUNCTION;
    int dReturn = this->m_indexPool;
    this->m_indexPool++;
    TH_LEAVE_OBJECT_FUNCTION;
    return dReturn;
}

LRESULT thMemo::processCommandMessage(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    TH_ENTER_OBJECT_FUNCTION;
    LRESULT tResult = 0; // should return 1 if not used (no CB registered)



    TH_LEAVE_OBJECT_FUNCTION;
    return tResult;
}

LRESULT thMemo::processNotifyMessage(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    //TH_ENTER_OBJECT_FUNCTION;
    LRESULT tResult = 0;
    NMHDR * pData = 0;

    pData = reinterpret_cast<NMHDR*>(a_lParam);

    if (pData) {
        MSG_ERROR(TEXT("WM_NOTIFY: hwndFrom=0x%X, idFrom=%d, code=0x%X"), pData->hwndFrom, pData->idFrom, pData->code);
    }

    //TH_LEAVE_OBJECT_FUNCTION;
    return tResult;
}