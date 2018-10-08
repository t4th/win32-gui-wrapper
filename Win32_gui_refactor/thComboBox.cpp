
// styles http://msdn.microsoft.com/en-us/library/windows/desktop/bb775796%28v=vs.85%29.aspx
// msgs http://msdn.microsoft.com/en-us/library/windows/desktop/bb775792%28v=vs.85%29.aspx
// macro http://msdn.microsoft.com/en-us/library/windows/desktop/bb856478%28v=vs.85%29.aspx
// auto complete http://www.codeproject.com/Articles/17069/Win-SDK-C-Autocomplete-Combobox-Made-Easy
#include "thComboBox.h"

/* Defines */
#define CLASS_NAME L"thComboBox"
#define WIN32_CLASS_NAME L"COMBOBOX"
#define DEFAULT_TEXT L"Caption"

#define DEFAULT_WIDTH  125
// combobox height is height including dropdown list!
#define DEFAULT_HEIGHT CW_USEDEFAULT

/* Local Memory */
int thComboBox::m_indexPool = 1;

/* Prototypes */

thComboBox::thComboBox() : thWindow(NULL, CW_USEDEFAULT, CW_USEDEFAULT)
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

thComboBox::thComboBox(thWindow * a_pParent, int a_posX = CW_USEDEFAULT, int a_posY = CW_USEDEFAULT) : thWindow(a_pParent, a_posX, a_posY)
{
    TH_ENTER_FUNCTION;
    BOOL fResult = FALSE;

    Items.setParent(this);

    OnSelectChange = NULL;
    OnDoubleClicked = NULL;
    OnSetFocus = NULL;
    OnKillFocus = NULL;
    OnEditChange = NULL;
    OnEditUpdate = NULL;
    OnDropdown = NULL;
    OnCloseUp = NULL;
    OnItemSelected = NULL;
    OnItemSelectedCancel = NULL;

    this->m_name = CLASS_NAME;

    this->m_sWindowArgs.dwExStyle =     0;
    this->m_sWindowArgs.lpClassName =   WIN32_CLASS_NAME;
    this->m_sWindowArgs.lpWindowName =  DEFAULT_TEXT;
    this->m_sWindowArgs.dwStyle =       WS_TABSTOP | WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST; // CBS_DROPDOWN for editable
    this->m_sWindowArgs.nWidth =        DEFAULT_WIDTH;
    this->m_sWindowArgs.nHeight =       DEFAULT_HEIGHT;
    this->m_sWindowArgs.hMenu =         this->m_id;
    this->m_sWindowArgs.lpParam =       this;

    this->create();

    fResult = SetWindowSubclass(this->m_hWinHandle, ChildWindProc, 0, (DWORD_PTR)this);

    if (FALSE == fResult) {
        MSG_ERROR(L"SetWindowSubclass failed with error = 0x%X", GetLastError());
    }

    TH_LEAVE_FUNCTION;
}

thComboBox::~thComboBox()
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

int thComboBox::getDebugIndex()
{
    TH_ENTER_FUNCTION;
    int dReturn = this->m_indexPool;
    this->m_indexPool++;
    TH_LEAVE_FUNCTION;
    return dReturn;
}

LRESULT thComboBox::onErrorSpace(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    TH_ENTER_FUNCTION;
    LRESULT tResult = 0;
    TH_LEAVE_FUNCTION;
    return tResult;
}

// If an application processes this message, it should return zero.
// a_wParam - low-order word is new width
// a_lParam - high-order word is new height
LRESULT thComboBox::onResize(HWND a_hwnd, WPARAM a_wParam, LPARAM a_lParam)
{
    //TH_ENTER_FUNCTION;
    LRESULT tResult = 0;
    //TH_LEAVE_FUNCTION;
    return tResult;
}

LRESULT thComboBox::processCommandMessage(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    TH_ENTER_FUNCTION;
    LRESULT tResult = 0; // should return 1 if not used (no CB registered)

    if (LOWORD(a_wParam) == reinterpret_cast<WORD>(this->m_id)) {
        switch (HIWORD(a_wParam)) {
        case (WORD)CBN_ERRSPACE:
            MSG_LOG(L"CBN_ERRSPACE");
            this->onErrorSpace(a_hwnd, a_uMsg, a_wParam, a_lParam);
            tResult = 1;
            break;
        case CBN_SELCHANGE:
            MSG_LOG(L"CBN_SELCHANGE");
            if (NULL != OnSelectChange) {
                OnSelectChange(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case CBN_DBLCLK:
            MSG_LOG(L"CBN_DBLCLK");
            if (NULL != OnDoubleClicked) {
                OnDoubleClicked(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case CBN_SETFOCUS: //UNPUSHED
            MSG_LOG(L"CBN_SETFOCUS");
            if (NULL != OnSetFocus) {
                OnSetFocus(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case CBN_KILLFOCUS:
            MSG_LOG(L"CBN_KILLFOCUS");
            if (NULL != OnSetFocus) {
                OnSetFocus(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case CBN_EDITCHANGE:
            MSG_LOG(L"CBN_EDITCHANGE");
            if (NULL != OnEditChange) {
                OnEditChange(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case CBN_EDITUPDATE:
            MSG_LOG(L"CBN_EDITUPDATE");
            if (NULL != OnEditUpdate) {
                OnEditUpdate(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case CBN_DROPDOWN:
            MSG_LOG(L"CBN_DROPDOWN");
            if (NULL != OnDropdown) {
                OnDropdown(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case CBN_CLOSEUP:
            MSG_LOG(L"CBN_CLOSEUP");
            if (NULL != OnCloseUp) {
                OnCloseUp(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case CBN_SELENDOK:
            MSG_LOG(L"CBN_SELENDOK");
            if (NULL != OnItemSelected) {
                OnItemSelected(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case CBN_SELENDCANCEL:
            MSG_LOG(L"CBN_SELENDCANCEL");
            if (NULL != OnItemSelectedCancel) {
                OnItemSelectedCancel(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        default:
            MSG_LOG(L"Not supported %X", a_uMsg);
            break;
        }
    }
    else { //search through children
        thComboBox *  pFoundChildren = NULL;

        pFoundChildren = reinterpret_cast<thComboBox*>(findChildrenByID(LOWORD(a_wParam)));

        if (pFoundChildren) {
            tResult = pFoundChildren->processCommandMessage(a_hwnd, a_uMsg, a_wParam, a_lParam);
        }
    }

    TH_LEAVE_FUNCTION;
    return tResult;
}

LRESULT thComboBox::processNotifyMessage(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    //TH_ENTER_FUNCTION;
    LRESULT tResult = 0;
    NMHDR * pData = 0;

    pData = reinterpret_cast<NMHDR*>(a_lParam);

    MSG_LOG(L"Not supported %X", pData->code);

    //TH_LEAVE_FUNCTION;
    return tResult;
}