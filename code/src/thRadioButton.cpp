#include "thRadioButton.h"

/* Defines */
#define CLASS_NAME TEXT("thRadioButton")
#define WIN32_CLASS_NAME TEXT("BUTTON")
#define DEFAULT_TEXT TEXT("Caption")

#define DEFAULT_WIDTH  75
#define DEFAULT_HEIGHT 25
/* Local Memory */
int thRadioButton::m_indexPool = 1;

/* Prototypes */
LRESULT CALLBACK ChildWindProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);

thRadioButton::thRadioButton(thWindow * a_pParent, int a_posX , int a_posY)
    :
    thWindow(a_pParent, a_posX, a_posY)
{
    TH_ENTER_FUNCTION;

    this->m_name = CLASS_NAME;

    this->m_sWindowArgs.dwExStyle =     0;
    this->m_sWindowArgs.lpClassName =   WIN32_CLASS_NAME;
    this->m_sWindowArgs.lpWindowName =  DEFAULT_TEXT;
    this->m_sWindowArgs.dwStyle =       WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_NOTIFY;
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

thRadioButton::~thRadioButton()
{
    TH_ENTER_OBJECT_FUNCTION;
    TH_LEAVE_OBJECT_FUNCTION;
}

int thRadioButton::getDebugIndex()
{
    TH_ENTER_OBJECT_FUNCTION;
    int dReturn = this->m_indexPool;
    this->m_indexPool++;
    TH_LEAVE_OBJECT_FUNCTION;
    return dReturn;
}

LRESULT thRadioButton::processCommandMessage(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    TH_ENTER_OBJECT_FUNCTION;
    LRESULT tResult = 0; // should return 1 if not used (no CB registered)

    if (LOWORD(a_wParam) == static_cast<WORD>(this->m_id)) {
        switch (HIWORD(a_wParam)) {
        case BN_CLICKED:
            MSG_LOG(TEXT("BN_CLICKED"));
            if (NULL != OnClick) {
                OnClick(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
#if 0
        case BN_PAINT:
            MSG_LOG(TEXT("BN_PAINT"));
            if (NULL != OnPaint) {
                OnPaint(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case BN_HILITE: //PUSHED
            MSG_LOG(TEXT("PUSHED"));
            if (NULL != OnPushed) {
                OnPushed(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case BN_UNHILITE: //UNPUSHED
            MSG_LOG(TEXT("UNPUSHED"));
            if (NULL != OnUnpushed) {
                OnUnpushed(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
#endif
        case BN_DISABLE:
            MSG_LOG(TEXT("BN_DISABLE"));
            if (NULL != OnDisable) {
                OnDisable(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case BN_DOUBLECLICKED:
            MSG_LOG(TEXT("BN_DOUBLECLICKED"));
            if (NULL != OnDoubleClicked) {
                OnDoubleClicked(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case BN_SETFOCUS:
            MSG_LOG(TEXT("BN_SETFOCUS"));
            if (NULL != OnSetFocus) {
                OnSetFocus(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case BN_KILLFOCUS:
            MSG_LOG(TEXT("BN_KILLFOCUS"));
            if (NULL != OnKillFocus) {
                OnKillFocus(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        default:
            MSG_ERROR(TEXT("Not supported %X"), a_uMsg);
            break;
        }
    }
    else { //search through children
        thRadioButton * pFoundChildren = reinterpret_cast<thRadioButton*>(findChildrenByID(LOWORD(a_wParam)));

        if (pFoundChildren) {
            tResult = pFoundChildren->processCommandMessage(a_hwnd, a_uMsg, a_wParam, a_lParam);
        }
    }

    TH_LEAVE_OBJECT_FUNCTION;
    return tResult;
}

LRESULT thRadioButton::processNotifyMessage(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    //TH_ENTER_OBJECT_FUNCTION;
    LRESULT tResult = 0;
    NMHDR * pData = reinterpret_cast<NMHDR*>(a_lParam);

#if 0
    if (pData) {
        switch (pData->code) {
        case BCN_DROPDOWN:
            MSG_LOG(TEXT("BCN_DROPDOWN - %s"), this->m_name.c_str());
            break;
        case BCN_HOTITEMCHANGE: //mouse is enetring button window
            MSG_LOG(TEXT("BCN_HOTITEMCHANGE - %s"), this->m_name.c_str());
            tResult = 1;
            break;
        case NM_CUSTOMDRAW:
            MSG_LOG(TEXT("LNM_CUSTOMDRAW - %s"), this->m_name.c_str());
            tResult = 1;
            break;
        default:
            MSG_ERROR(TEXT("WM_NOTIFY: hwndFrom=0x%X, idFrom=%d, code=0x%X"), pData->hwndFrom, pData->idFrom, pData->code);
            break;
        }
    }
#endif

    //TH_LEAVE_OBJECT_FUNCTION;
    return tResult;
}