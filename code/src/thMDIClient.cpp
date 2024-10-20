#include "thMDIClient.h"
#include "thForm.h"

/* Defines */
#define CLASS_NAME TEXT("thMDIClient")
#define WIN32_CLASS_NAME TEXT("MDICLIENT")
#define DEFAULT_TEXT TEXT("Caption")

#define DEFAULT_WIDTH  200
#define DEFAULT_HEIGHT 200

/* Local Memory */
int thMDIClient::m_indexPool = 1;

// TODO: fix this workaound, because Win32 message dispatcher was designed for only 1 MDI Client.
thMDIClient * g_pMDI_client = nullptr;

/* Prototypes */
LRESULT CALLBACK ChildWindProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);

// TODO: Creating protection for creating only 1 MDI client.

thMDIClient::thMDIClient( thForm * a_pParent, int a_posX, int a_posY)
    : thWindow(a_pParent, a_posX, a_posY)
{
    TH_ENTER_FUNCTION;
    BOOL fResult = FALSE;
    this->m_name = CLASS_NAME;

    //this->Events = { 0 };
    CLIENTCREATESTRUCT cs = { 0 };
    cs.idFirstChild = TH_ID_MDI_FIRSTCHILD;

    this->m_sWindowArgs.dwExStyle =     WS_EX_CLIENTEDGE; //WS_EX_CLIENTEDGE
    this->m_sWindowArgs.lpClassName =   WIN32_CLASS_NAME;
    this->m_sWindowArgs.lpWindowName =  DEFAULT_TEXT;
    this->m_sWindowArgs.dwStyle =       WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL;
    this->m_sWindowArgs.nWidth =        DEFAULT_WIDTH;
    this->m_sWindowArgs.nHeight =       DEFAULT_HEIGHT;
    this->m_sWindowArgs.hMenu =         0;//this->m_id;
    this->m_sWindowArgs.lpParam =       &cs;

    this->create();

    if ( a_pParent)
    {
        a_pParent->m_hMDIClient = this->m_hWinHandle;
    }

    g_pMDI_client = this;

    fResult = SetWindowSubclass(this->m_hWinHandle, ChildWindProc, 0, (DWORD_PTR)this);

    if ( FALSE == fResult)
    {
        MSG_ERROR(TEXT("SetWindowSubclass failed with error = 0x%X"), GetLastError());
    }

    TH_LEAVE_FUNCTION;
}

thMDIClient::~thMDIClient()
{
    TH_ENTER_FUNCTION;
#if 0
    // when parent change this should be done
    thForm * pParent = NULL;

    pParent = dynamic_cast<thForm*>(m_pParent);
    if (pParent) {
        pParent->m_hMDIClient = NULL;
    }
#endif
    TH_LEAVE_FUNCTION;
}

int thMDIClient::getDebugIndex()
{
    TH_ENTER_FUNCTION;
    int dReturn = this->m_indexPool;
    this->m_indexPool++;
    TH_LEAVE_FUNCTION;
    return dReturn;
}

void thMDIClient::Cascade()
{
    SendMessage(this->m_hWinHandle, WM_MDICASCADE, 0, 0);
}

void thMDIClient::TileHorizontal()
{
    SendMessage(this->m_hWinHandle, WM_MDITILE, MDITILE_HORIZONTAL, 0);
}

void thMDIClient::TileVertical()
{
    SendMessage(this->m_hWinHandle, WM_MDITILE, MDITILE_VERTICAL, 0);
}

void thMDIClient::ArrangeIcons()
{
    SendMessage(this->m_hWinHandle, WM_MDIICONARRANGE, 0, 0);
}