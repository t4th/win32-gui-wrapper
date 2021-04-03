#include "thListBox.h"

/* Defines */
#define CLASS_NAME TEXT("thListBox")
#define WIN32_CLASS_NAME TEXT("LISTBOX")
#define DEFAULT_TEXT TEXT("Caption")

#define DEFAULT_WIDTH  75
#define DEFAULT_HEIGHT 25

/* Local Memory */
int thListBox::m_indexPool = 1;

/* Definitions */
thListBox::thListBox(thWindow * a_pParent, int a_posX, int a_posY)
    :
    thWindow(a_pParent, a_posX, a_posY)
{
    TH_ENTER_FUNCTION;

    Items.setParent(this);

    this->m_name =          CLASS_NAME;

    this->m_sWindowArgs.dwExStyle =     WS_EX_CLIENTEDGE;
    this->m_sWindowArgs.lpClassName =   WIN32_CLASS_NAME;
    this->m_sWindowArgs.lpWindowName =  DEFAULT_TEXT;
    this->m_sWindowArgs.dwStyle =       WS_VISIBLE | WS_CHILD;
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

thListBox::~thListBox()
{
    TH_ENTER_OBJECT_FUNCTION;
    TH_LEAVE_OBJECT_FUNCTION;
}

int thListBox::getDebugIndex()
{
    TH_ENTER_OBJECT_FUNCTION;
    int dReturn = this->m_indexPool;
    this->m_indexPool++;
    TH_LEAVE_OBJECT_FUNCTION;
    return dReturn;
}

// all msgs: https://msdn.microsoft.com/en-us/library/windows/desktop/ff485968%28v=vs.85%29.aspx
LRESULT thListBox::processCommandMessage(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    TH_ENTER_OBJECT_FUNCTION;
    LRESULT tResult = 0; // should return 1 if not used (no CB registered)

    if (LOWORD(a_wParam) == static_cast<WORD>(this->m_id)) {
        switch (HIWORD(a_wParam)) {
        case LBN_DBLCLK:
            MSG_LOG(TEXT("LBN_DBLCLK"));
            if (nullptr != OnDoubleClicked) {
                OnDoubleClicked(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case LBN_KILLFOCUS:
            MSG_LOG(TEXT("LBN_KILLFOCUS"));
            if (nullptr != OnKillFocus) {
                OnKillFocus(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case LBN_SELCANCEL:
            MSG_LOG(TEXT("LBN_SELCANCEL"));
            if (nullptr != OnItemSelectedCancel) {
                OnItemSelectedCancel(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case LBN_SELCHANGE:
            MSG_LOG(TEXT("LBN_SELCHANGE"));
            if (nullptr != OnSelectionChange) {
                OnSelectionChange(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case LBN_SETFOCUS:
            MSG_LOG(TEXT("LBN_SETFOCUS"));
            if (nullptr != OnSetFocus) {
                OnSetFocus(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        default:
            MSG_LOG(TEXT("Not supported %X"), a_uMsg);
            break;
        }
    }
    else { //search through children
        thListBox *  pFoundChildren = reinterpret_cast<thListBox*>(findChildrenByID(LOWORD(a_wParam)));

        if (pFoundChildren) {
            tResult = pFoundChildren->processCommandMessage(a_hwnd, a_uMsg, a_wParam, a_lParam);
        }
    }

    TH_LEAVE_OBJECT_FUNCTION;
    return tResult;
}

LRESULT thListBox::processNotifyMessage(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    //TH_ENTER_OBJECT_FUNCTION;
    LRESULT tResult = 0;
    NMHDR * pData = 0;

    pData = reinterpret_cast<NMHDR*>(a_lParam);

    if (pData) {
        //MSG_ERROR(TEXT("WM_NOTIFY: hwndFrom=0x%X, idFrom=%d, code=0x%X"), pData->hwndFrom, pData->idFrom, pData->code);
    }

    //TH_LEAVE_OBJECT_FUNCTION;
    return tResult;
}