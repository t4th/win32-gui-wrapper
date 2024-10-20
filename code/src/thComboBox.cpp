
// styles http://msdn.microsoft.com/en-us/library/windows/desktop/bb775796%28v=vs.85%29.aspx
// msgs http://msdn.microsoft.com/en-us/library/windows/desktop/bb775792%28v=vs.85%29.aspx
// macro http://msdn.microsoft.com/en-us/library/windows/desktop/bb856478%28v=vs.85%29.aspx
// auto complete http://www.codeproject.com/Articles/17069/Win-SDK-C-Autocomplete-Combobox-Made-Easy
#include "thComboBox.h"

/* Defines */
#define CLASS_NAME TEXT("thComboBox")
#define WIN32_CLASS_NAME TEXT("COMBOBOX")
#define DEFAULT_TEXT TEXT("Caption")

#define DEFAULT_WIDTH  125
// combobox height is height including dropdown list!
#define DEFAULT_HEIGHT CW_USEDEFAULT

/* Local Memory */
int thComboBox::m_indexPool = 1;

/* Prototypes */
LRESULT CALLBACK ChildWindProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);

thComboBox::thComboBox(thWindow * a_pParent, int a_posX, int a_posY)
    :
    thWindow(a_pParent, a_posX, a_posY),
    Items(*this)
{
    TH_ENTER_FUNCTION;

    this->m_name = CLASS_NAME;

    this->m_sWindowArgs.dwExStyle =     0;
    this->m_sWindowArgs.lpClassName =   WIN32_CLASS_NAME;
    this->m_sWindowArgs.lpWindowName =  DEFAULT_TEXT;
    this->m_sWindowArgs.dwStyle =       WS_TABSTOP | WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST; // CBS_DROPDOWN for editable
    this->m_sWindowArgs.nWidth =        DEFAULT_WIDTH;
    this->m_sWindowArgs.nHeight =       DEFAULT_HEIGHT;
    this->m_sWindowArgs.hMenu =         reinterpret_cast<HMENU>(this->m_id);
    this->m_sWindowArgs.lpParam =       this;

    this->create();

    BOOL fResult = SetWindowSubclass(this->m_hWinHandle, ChildWindProc, 0, (DWORD_PTR)this);

    if (FALSE == fResult) {
        MSG_ERROR(TEXT("SetWindowSubclass failed with error = 0x%X"), GetLastError());
    }

    TH_LEAVE_FUNCTION;
}

thComboBox::~thComboBox()
{
    TH_ENTER_OBJECT_FUNCTION;
    TH_LEAVE_OBJECT_FUNCTION;
}

int thComboBox::getDebugIndex()
{
    TH_ENTER_OBJECT_FUNCTION;
    int dReturn = this->m_indexPool;
    this->m_indexPool++;
    TH_LEAVE_OBJECT_FUNCTION;
    return dReturn;
}

LRESULT thComboBox::onErrorSpace(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    TH_ENTER_OBJECT_FUNCTION;
    LRESULT tResult = 0;
    TH_LEAVE_OBJECT_FUNCTION;
    return tResult;
}

// If an application processes this message, it should return zero.
// a_wParam - low-order word is new width
// a_lParam - high-order word is new height
LRESULT thComboBox::onResize(HWND a_hwnd, WPARAM a_wParam, LPARAM a_lParam)
{
    //TH_ENTER_OBJECT_FUNCTION;
    LRESULT tResult = 0;
    //TH_LEAVE_OBJECT_FUNCTION;
    return tResult;
}

LRESULT thComboBox::processCommandMessage(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    TH_ENTER_OBJECT_FUNCTION;
    LRESULT tResult = 0; // should return 1 if not used (no CB registered)

    if (LOWORD(a_wParam) == static_cast<WORD>(this->m_id)) {
        switch (HIWORD(a_wParam)) {
        case (WORD)CBN_ERRSPACE:
            MSG_LOG(TEXT("CBN_ERRSPACE"));
            this->onErrorSpace(a_hwnd, a_uMsg, a_wParam, a_lParam);
            tResult = 1;
            break;
        case CBN_SELCHANGE:
            MSG_LOG(TEXT("CBN_SELCHANGE"));
            if (nullptr != OnSelectChange) {
                OnSelectChange(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case CBN_DBLCLK:
            MSG_LOG(TEXT("CBN_DBLCLK"));
            if (nullptr != OnDoubleClicked) {
                OnDoubleClicked(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case CBN_SETFOCUS: //UNPUSHED
            MSG_LOG(TEXT("CBN_SETFOCUS"));
            if (nullptr != OnSetFocus) {
                OnSetFocus(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case CBN_KILLFOCUS:
            MSG_LOG(TEXT("CBN_KILLFOCUS"));
            if (nullptr != OnSetFocus) {
                OnSetFocus(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case CBN_EDITCHANGE:
            MSG_LOG(TEXT("CBN_EDITCHANGE"));
            if (nullptr != OnEditChange) {
                OnEditChange(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case CBN_EDITUPDATE:
            MSG_LOG(TEXT("CBN_EDITUPDATE"));
            if (nullptr != OnEditUpdate) {
                OnEditUpdate(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case CBN_DROPDOWN:
            MSG_LOG(TEXT("CBN_DROPDOWN"));
            if (nullptr != OnDropdown) {
                OnDropdown(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case CBN_CLOSEUP:
            MSG_LOG(TEXT("CBN_CLOSEUP"));
            if (nullptr != OnCloseUp) {
                OnCloseUp(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case CBN_SELENDOK:
            MSG_LOG(TEXT("CBN_SELENDOK"));
            if (nullptr != OnItemSelected) {
                OnItemSelected(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case CBN_SELENDCANCEL:
            MSG_LOG(TEXT("CBN_SELENDCANCEL"));
            if (nullptr != OnItemSelectedCancel) {
                OnItemSelectedCancel(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        default:
            MSG_LOG(TEXT("Not supported %X"), a_uMsg);
            break;
        }
    }
    else { //search through children
        thComboBox *  pFoundChildren = nullptr;

        pFoundChildren = reinterpret_cast<thComboBox*>(findChildrenByID(LOWORD(a_wParam)));

        if (pFoundChildren) {
            tResult = pFoundChildren->processCommandMessage(a_hwnd, a_uMsg, a_wParam, a_lParam);
        }
    }

    TH_LEAVE_OBJECT_FUNCTION;
    return tResult;
}

LRESULT thComboBox::processNotifyMessage(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    //TH_ENTER_OBJECT_FUNCTION;
    LRESULT tResult = 0;
    NMHDR * pData = 0;

    pData = reinterpret_cast<NMHDR*>(a_lParam);

    MSG_LOG(TEXT("Not supported %X"), pData->code);

    //TH_LEAVE_OBJECT_FUNCTION;
    return tResult;
}