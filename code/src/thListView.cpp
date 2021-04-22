#include "thListView.h"

#include < array>

/* Defines */
#define CLASS_NAME TEXT( "thListView")
#define WIN32_CLASS_NAME WC_LISTVIEW
#define DEFAULT_TEXT TEXT( "")

constexpr int DEFAULT_WIDTH = 150;
constexpr int DEFAULT_HEIGHT = 150;

/* Local Memory */
int thListView::m_indexPool = 1;

thListView::thListView( thWindow * a_pParent, int a_posX, int a_posY)
    :
    thWindow( a_pParent, a_posX, a_posY),
    Columns( *this),
    Items( *this)
{
    TH_ENTER_FUNCTION;
    INITCOMMONCONTROLSEX iccx = { 0 };

    iccx.dwSize = sizeof( INITCOMMONCONTROLSEX);
    iccx.dwICC = ICC_BAR_CLASSES;

    if ( FALSE == InitCommonControlsEx( &iccx))
    {
        MSG_ERROR( TEXT( "InitCommonControlsEx ICC_BAR_CLASSES failed with error = 0x%X"), GetLastError());
    }

    this->m_name = CLASS_NAME;

    this->m_sWindowArgs.dwExStyle =     WS_EX_CLIENTEDGE;
    this->m_sWindowArgs.lpClassName =   WIN32_CLASS_NAME;
    this->m_sWindowArgs.lpWindowName =  DEFAULT_TEXT;
    this->m_sWindowArgs.dwStyle =       WS_VISIBLE | WS_CHILD | LVS_REPORT;// | LVS_EDITLABELS;
    this->m_sWindowArgs.nWidth =        DEFAULT_WIDTH;
    this->m_sWindowArgs.nHeight =       DEFAULT_HEIGHT;
    this->m_sWindowArgs.hMenu =         reinterpret_cast< HMENU>( this->m_id);;
    this->m_sWindowArgs.lpParam =       this;

    this->create();

    ListView_SetExtendedListViewStyle( this->m_hWinHandle, LVS_EX_FULLROWSELECT);

    BOOL fResult = SetWindowSubclass( this->m_hWinHandle, ChildWindProc, 0, reinterpret_cast< DWORD_PTR>( this));

    if ( FALSE == fResult)
    {
        MSG_ERROR( TEXT( "SetWindowSubclass failed with error = 0x%X"), GetLastError());
    }

    TH_LEAVE_FUNCTION;
}

int thListView::getDebugIndex()
{
    TH_ENTER_OBJECT_FUNCTION;

    int dReturn = this->m_indexPool;
    this->m_indexPool++;

    TH_LEAVE_OBJECT_FUNCTION;
    return dReturn;
}

void thListView::SetView( eViewType_t a_eViewType)
{
    constexpr std::array< DWORD, 5U> underlying_value{
        LV_VIEW_DETAILS, LV_VIEW_ICON, LV_VIEW_LIST, LV_VIEW_SMALLICON, LV_VIEW_TILE
    };

    constexpr LRESULT operation_successful = 1U;

    LRESULT result = ListView_SetView( this->m_hWinHandle, underlying_value[ static_cast< int>( a_eViewType)]);

    if ( operation_successful != result)
    {
        MSG_ERROR( TEXT( "ListView_SetView failed - provided view is invalid"));
    }
}

LRESULT thListView::processCommandMessage( HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    constexpr LRESULT message_not_proccessed{ 0U};

    TH_ENTER_OBJECT_FUNCTION;

    TH_LEAVE_OBJECT_FUNCTION;
    return message_not_proccessed;
}

LRESULT thListView::processNotifyMessage(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    constexpr LRESULT message_not_proccessed{ 0U};

    NMHDR * pData = reinterpret_cast< NMHDR*>( a_lParam);

    if ( pData)
    {
        // Hide notification messages spam.
        #if 0
        MSG_ERROR( TEXT( "WM_NOTIFY: hwndFrom=0x%X, idFrom=%d, code=0x%X"), pData->hwndFrom, pData->idFrom, pData->code);
        #endif
    }

    return message_not_proccessed;
}