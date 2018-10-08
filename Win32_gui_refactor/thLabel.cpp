#include "thLabel.h"

/* Defines */
#define CLASS_NAME L"thLabel"
#define WIN32_CLASS_NAME L"STATIC"
#define DEFAULT_TEXT L"Caption"

#define DEFAULT_WIDTH  75
#define DEFAULT_HEIGHT 25

/* Local Memory */
int thLabel::m_indexPool = 1;

/* Prototypes */

thLabel::thLabel() : thWindow(NULL, CW_USEDEFAULT, CW_USEDEFAULT)
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

thLabel::thLabel(thWindow * a_pParent, int a_posX = CW_USEDEFAULT, int a_posY = CW_USEDEFAULT) : thWindow(a_pParent, a_posX, a_posY)
{
    TH_ENTER_FUNCTION;
    BOOL fResult = FALSE;

    this->OnClick =         NULL; // = STN_CLICKED,
    this->OnDoubleClicked = NULL; // = STN_DBLCLK,
    this->OnDisable =       NULL; // = STN_DISABLE,
    this->OnEnable =        NULL; // = STN_ENABLE,

    this->m_name =          CLASS_NAME;

    this->m_sWindowArgs.dwExStyle =     0;
    this->m_sWindowArgs.lpClassName =   WIN32_CLASS_NAME;
    this->m_sWindowArgs.lpWindowName =  DEFAULT_TEXT;
    this->m_sWindowArgs.dwStyle =       WS_VISIBLE | WS_CHILD | SS_NOTIFY;
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

thLabel::~thLabel()
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

int thLabel::getDebugIndex()
{
    TH_ENTER_FUNCTION;
    int dReturn = this->m_indexPool;
    this->m_indexPool++;
    TH_LEAVE_FUNCTION;
    return dReturn;
}

LRESULT thLabel::processCommandMessage(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    TH_ENTER_FUNCTION;
    LRESULT tResult = 0; // should return 1 if not used (no CB registered)

    if (LOWORD(a_wParam) == reinterpret_cast<WORD>(this->m_id)) {
        switch (HIWORD(a_wParam)) {
        case STN_CLICKED:
            MSG_LOG(L"STN_CLICKED");
            if (NULL != OnClick) {
                OnClick(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case STN_DBLCLK:
            MSG_LOG(L"STN_DBLCLK");
            if (NULL != OnDoubleClicked) {
                OnDoubleClicked(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case STN_DISABLE: //PUSHED
            MSG_LOG(L"STN_DISABLE");
            if (NULL != OnDisable) {
                OnDisable(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        case STN_ENABLE: //UNPUSHED
            MSG_LOG(L"STN_ENABLE");
            if (NULL != OnEnable) {
                OnEnable(this, { a_uMsg, a_wParam, a_lParam });
            }
            tResult = 1;
            break;
        default:
            MSG_LOG(L"Not supported %X", a_uMsg);
            break;
        }
    }
    else { //search through children
        thLabel *  pFoundChildren = NULL;

        pFoundChildren = reinterpret_cast<thLabel*>(findChildrenByID(LOWORD(a_wParam)));

        if (pFoundChildren) {
            tResult = pFoundChildren->processCommandMessage(a_hwnd, a_uMsg, a_wParam, a_lParam);
        }
    }

    TH_LEAVE_FUNCTION;
    return tResult;
}

LRESULT thLabel::processNotifyMessage(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    //TH_ENTER_FUNCTION;
    LRESULT tResult = 0;
    NMHDR * pData = 0;

    pData = reinterpret_cast<NMHDR*>(a_lParam);

    if (pData) {
        MSG_ERROR(L"WM_NOTIFY: hwndFrom=0x%X, idFrom=%d, code=0x%X", pData->hwndFrom, pData->idFrom, pData->code);
    }

    //TH_LEAVE_FUNCTION;
    return tResult;
}