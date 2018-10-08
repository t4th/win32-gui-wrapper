#include "thListView.h"

/* Defines */
#define CLASS_NAME L"thListView"
#define WIN32_CLASS_NAME WC_LISTVIEWW
#define DEFAULT_TEXT L""

#define DEFAULT_WIDTH  150
#define DEFAULT_HEIGHT 150

/* Local Memory */
int thListView::m_indexPool = 1;

/* Prototypes */

thListView::thListView() : thWindow(NULL, CW_USEDEFAULT, CW_USEDEFAULT)
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

thListView::thListView(thWindow * a_pParent, int a_posX = CW_USEDEFAULT, int a_posY = CW_USEDEFAULT) : thWindow(a_pParent, a_posX, a_posY)
{
    TH_ENTER_FUNCTION;
    BOOL fResult = FALSE;
    INITCOMMONCONTROLSEX iccx = { 0 };

    iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
    iccx.dwICC = ICC_BAR_CLASSES;
    if (FALSE == InitCommonControlsEx(&iccx)) {
        MSG_ERROR(L"InitCommonControlsEx ICC_BAR_CLASSES failed with error = 0x%X", GetLastError());
    }

    this->Columns.SetParent(this);
    this->Items.SetParent(this);
    this->m_name = CLASS_NAME;

    this->m_sWindowArgs.dwExStyle =     WS_EX_CLIENTEDGE;
    this->m_sWindowArgs.lpClassName =   WIN32_CLASS_NAME;
    this->m_sWindowArgs.lpWindowName =  DEFAULT_TEXT;
    this->m_sWindowArgs.dwStyle =       WS_VISIBLE | WS_CHILD | LVS_REPORT;// | LVS_EDITLABELS;
    this->m_sWindowArgs.nWidth =        DEFAULT_WIDTH;
    this->m_sWindowArgs.nHeight =       DEFAULT_HEIGHT;
    this->m_sWindowArgs.hMenu =         this->m_id;
    this->m_sWindowArgs.lpParam =       this;

    this->create();

    ListView_SetExtendedListViewStyle(this->m_hWinHandle, LVS_EX_FULLROWSELECT);

    fResult = SetWindowSubclass(this->m_hWinHandle, ChildWindProc, 0, (DWORD_PTR)this);

    if (FALSE == fResult) {
        MSG_ERROR(L"SetWindowSubclass failed with error = 0x%X", GetLastError());
    }

    TH_LEAVE_FUNCTION;
}

thListView::~thListView()
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

int thListView::getDebugIndex()
{
    TH_ENTER_FUNCTION;
    int dReturn = this->m_indexPool;
    this->m_indexPool++;
    TH_LEAVE_FUNCTION;
    return dReturn;
}

void thListView::SetView(eViewType_t a_eViewType)
{
    LRESULT result = -1;

    result = ListView_SetView(this->m_hWinHandle, a_eViewType);

    if (-1 == result) {
        MSG_ERROR(L"ListView_SetView failed - provided view is invalid");
    }
}

LRESULT thListView::processCommandMessage(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    TH_ENTER_FUNCTION;
    LRESULT tResult = 0; // should return 1 if not used (no CB registered)

    TH_LEAVE_FUNCTION;
    return tResult;
}

LRESULT thListView::processNotifyMessage(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    //TH_ENTER_FUNCTION;
    LRESULT tResult = 0;
    NMHDR * pData = 0;

    pData = reinterpret_cast<NMHDR*>(a_lParam);

    if (pData) {
        //MSG_ERROR(L"WM_NOTIFY: hwndFrom=0x%X, idFrom=%d, code=0x%X", pData->hwndFrom, pData->idFrom, pData->code);
    }

    //TH_LEAVE_FUNCTION;
    return tResult;
}