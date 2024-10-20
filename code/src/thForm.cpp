#include "thForm.h"

/* Defines */
#define CLASS_NAME TEXT( "thForm")
#define WIN32_CLASS_NAME CLASS_NAME
#define DEFAULT_TEXT TEXT( "Caption")

#define DEFAULT_WIDTH  200
#define DEFAULT_HEIGHT 200

/* Local Memory */
int thForm::m_indexPool = 1;

/* Prototypes */
LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

// TODO: fix this.
// Main window pointer workaround.
std::vector< thForm*> g_forms;

// keep dx and dy after thForm resize
typedef struct
{
    int Width;
    int Height;
} thDifference_t;

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
    TH_ENTER_OBJECT_FUNCTION;

    static bool once = true; // Register this class once in a lifetime of application. Lazy implementation.

    if ( true == once)
    {
        WNDCLASSEX window_class_ex = { 0 };

        window_class_ex.cbSize =         sizeof( window_class_ex);
        window_class_ex.style =          0; //CS_HREDRAW | CS_VREDRAW;
        window_class_ex.lpfnWndProc =    static_cast< WNDPROC>( WinProc);
        window_class_ex.cbClsExtra =     NULL;
        window_class_ex.cbWndExtra =     sizeof( thForm*); // 4 bytes for 'this' pointer
        window_class_ex.hInstance =      this->m_sWindowArgs.hInstance;
        window_class_ex.hIcon =          NULL;
        window_class_ex.hCursor =        LoadCursor( NULL, IDC_ARROW);
        window_class_ex.hbrBackground =  reinterpret_cast< HBRUSH>( COLOR_WINDOW);
        window_class_ex.lpszMenuName =   NULL;
        window_class_ex.lpszClassName =  this->m_sWindowArgs.lpClassName;
        window_class_ex.hIconSm =        NULL;
    
        if ( 0 != RegisterClassEx( &window_class_ex))
        {
            once = false;
        }
        else
        {
            MSG_WARNING( TEXT( "RegisterClassEx error: 0x%X"), GetLastError());
        }
    }
    TH_LEAVE_OBJECT_FUNCTION;
}

thForm::thForm( thWindow * a_pParent, int a_posX, int a_posY)
    :
    thWindow( a_pParent, a_posX, a_posY),
    m_menu( nullptr),
    m_hMDIClient( nullptr),
    Resizable( *this)
{
    TH_ENTER_FUNCTION;
    
    this->init();
    this->registerClass();

    g_forms.push_back( this);

    this->create();

    TH_LEAVE_FUNCTION;
}

thForm::~thForm()
{
    TH_ENTER_OBJECT_FUNCTION;

    auto position = 0;

    for ( const auto i: g_forms)
    {
        if ( i == this)
        {
            g_forms.erase( g_forms.begin() + position);
            break;
        }

        position++;
    }

    TH_LEAVE_OBJECT_FUNCTION;
}

LRESULT thForm::onCreate()
{
    TH_ENTER_OBJECT_FUNCTION;
    LRESULT tResult = 0;

    TH_LEAVE_OBJECT_FUNCTION;
    return tResult;
}

LRESULT thForm::onClose()
{
    TH_ENTER_OBJECT_FUNCTION;

    LRESULT tResult = 0;

    if ( OnClose)
    {
        tResult = OnClose( this, {});
    }

    TH_LEAVE_OBJECT_FUNCTION;
    return tResult;
}

// thForm use client size instead of window size
// Because of that it need specialized onGetMinMax
LRESULT thForm::onGetMinMax( LPARAM a_lParam)
{
    //TH_ENTER_OBJECT_FUNCTION;
    LRESULT      tResult = 1;
    MINMAXINFO * sInfo = 0;

    //TODO: change this->Constraints variables to window sizes as user is expecting client sizes
#if 0
    sInfo = reinterpret_cast <MINMAXINFO*>( a_lParam);

    if ( sInfo)
    {
        if ( this->Constraints.MaxWidth)
        {
            sInfo->ptMaxTrackSize.x = this->Constraints.MaxWidth;
            tResult = 0;
        }

        if ( this->Constraints.MaxHeight) {
            sInfo->ptMaxTrackSize.y = this->Constraints.MaxHeight;
            tResult = 0;
        }

        if ( this->Constraints.MinWidth) {
            sInfo->ptMinTrackSize.x = this->Constraints.MinWidth;
            tResult = 0;
        }

        if ( this->Constraints.MinHeight) {
            sInfo->ptMinTrackSize.y = this->Constraints.MinHeight;
            tResult = 0;
        }
    }
#endif 
    //TH_LEAVE_OBJECT_FUNCTION;
    return tResult;
}

LRESULT thForm::processMenuCommandMessage( HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    TH_ENTER_OBJECT_FUNCTION;

    LRESULT     tResult = 0;

    if ( m_menu)
    {
        tResult = m_menu->processCommandMessage( a_hwnd, a_uMsg, a_wParam, a_lParam);
    }

    if ( FALSE == tResult)
    {
        if (PopupMenu)
        {
            tResult = PopupMenu->processCommandMessage( a_hwnd, a_uMsg, a_wParam, a_lParam);
        }
    }

    TH_LEAVE_OBJECT_FUNCTION;
    return tResult;
}

int thForm::getDebugIndex()
{
    TH_ENTER_OBJECT_FUNCTION;

    int dReturn = this->m_indexPool;
    this->m_indexPool++;

    TH_LEAVE_OBJECT_FUNCTION;
    return dReturn;
}

void thForm::SetMenu( thMenu * const a_pMenu)
{
    TH_ENTER_OBJECT_FUNCTION;

    if ( NULL == a_pMenu)
    {
        MSG_ERROR( TEXT( "Empty input pointer!"));
        return;
    }

    if ( a_pMenu != m_menu)
    {
        if ( NULL != a_pMenu->m_hMenuHandle)
        {
            m_menu = a_pMenu;

            if ( FALSE == ::SetMenu( this->m_hWinHandle, a_pMenu->m_hMenuHandle))
            {
                MSG_ERROR( TEXT( "SetMenu failed with error =  0x%X"), GetLastError());
            }
            else
            {
                m_menu->m_hParentWindow = this->m_hWinHandle;
                DrawMenuBar( this->m_hWinHandle);
            }
        }
        else
        {
            MSG_ERROR( TEXT( "Provided thMenu is invalid (wrong handle)"));
        }
    }

    TH_LEAVE_OBJECT_FUNCTION;
}

void thForm::ClearMenu()
{
    TH_ENTER_OBJECT_FUNCTION;

    if ( FALSE == ::SetMenu(this->m_hWinHandle, NULL))
    {
        MSG_ERROR( TEXT( "SetMenu failed with error =  0x%X"), GetLastError());
    }
    else
    {
        this->m_menu->m_hParentWindow = NULL;
        m_menu = NULL;
    }

    TH_LEAVE_OBJECT_FUNCTION;
}

BOOL thForm::IsMenuEnabled()
{
    TH_ENTER_OBJECT_FUNCTION;

    BOOL result = false;

    if ( GetMenu( this->m_hWinHandle))
    {
        result = true;
    }

    TH_LEAVE_OBJECT_FUNCTION;
    return result;
}