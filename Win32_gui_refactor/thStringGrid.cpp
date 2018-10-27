#include "thStringGrid.h"
#include "SimpleGrid_src\simpleGrid.h"

/* Defines */
#define CLASS_NAME TEXT("thStringGrid")
#define WIN32_CLASS_NAME WC_SIMPLEGRID
#define DEFAULT_TEXT TEXT("")

#define DEFAULT_WIDTH  200
#define DEFAULT_HEIGHT 200

/* Local Memory */
int thStringGrid::m_indexPool = 1;

/* Prototypes */

thStringGrid::thStringGrid() : thWindow(NULL, CW_USEDEFAULT, CW_USEDEFAULT)
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

#define NELEMS(a)  (sizeof(a) / sizeof((a)[0]))
BOOL Main_OnCreate(HWND hGrid, LPCREATESTRUCT lpCreateStruct, DWORD id)
{
    // Initialize grid class and instanciate a new instance
    //  with the grid factory method.
    //HWND hGrid = New_SimpleGrid(hwnd, id);
    //ShowWindow(hGrid, SW_SHOW);

    //set resizeable grid columns
    SimpleGrid_SetAllowColResize(hGrid, FALSE);

    //set the header row to initial height of 21 pixels
    SimpleGrid_SetHeaderRowHeight(hGrid, 21);
    //last column standard width
    SimpleGrid_ExtendLastColumn(hGrid, FALSE);
    //vertical scroll set to non integral rows
    SimpleGrid_ShowIntegralRows(hGrid, FALSE);
    
    //on row header selection hilight full row, otherwise individual cell
    //SimpleGrid_SetSelectionMode(hGrid, GSO_ROWHEADER);

#if 0
    //Include a title for this grid
    HFONT hFont = CreateFont(20, 0, 0, 0, FW_EXTRABOLD, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        PROOF_QUALITY, VARIABLE_PITCH | FF_MODERN, TEXT("ARIEL"));

    SimpleGrid_SetTitleFont(hGrid, hFont);
    SimpleGrid_SetTitleHeight(hGrid, 21);
    SimpleGrid_SetTitle(hGrid, TEXT("Grid's window text displayed here."));
#endif

    //Add three columns and four rows

    //LPTSTR blankText = TEXT(""); //just some blank text

    //use column header text
    SimpleGrid_SetColsNumbered(hGrid, FALSE);

    //Column type, Column header text,  Optional data (ex: combobox choices)
    SGCOLUMN lpColumns[] = {
        GCT_EDIT, TEXT("1"), NULL,
        GCT_EDIT, TEXT("2"), NULL,
        GCT_EDIT, TEXT("3"), NULL,
        GCT_EDIT, TEXT("4"), NULL
    };

    //Add the columns
    for (int k = NELEMS(lpColumns), m = 0; 0 < k; --k, ++m)
        SimpleGrid_AddColumn(hGrid, &lpColumns[m]);

    // SimpleGrid_SetRowsNumbered

    //Add the rows
    for (int i = 0; i < 16; ++i)
        SimpleGrid_AddRow(hGrid, 0);

    SimpleGrid_EnableEdit(hGrid, FALSE);

    return TRUE;
}

thStringGrid::thStringGrid(thWindow * a_pParent, int a_posX = CW_USEDEFAULT, int a_posY = CW_USEDEFAULT)
    : thWindow(a_pParent, a_posX, a_posY)
{
    TH_ENTER_FUNCTION;
    BOOL fResult = FALSE;
    INITCOMMONCONTROLSEX iccx = { 0 };

    iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
    iccx.dwICC = ICC_WIN95_CLASSES/*|ICC_COOL_CLASSES|ICC_DATE_CLASSES|
                                 ICC_PAGESCROLLER_CLASS|ICC_USEREX_CLASSES*/;
    if (FALSE == InitCommonControlsEx(&iccx)) {
        MSG_ERROR(TEXT("InitCommonControlsEx ICC_BAR_CLASSES failed with error = 0x%X"), GetLastError());
    }
    
    InitSimpleGrid(this->m_sWindowArgs.hInstance); //thWindow c-tor init this

    this->m_name = CLASS_NAME;

    this->m_sWindowArgs.dwExStyle =     0;
    this->m_sWindowArgs.lpClassName =   WIN32_CLASS_NAME;
    this->m_sWindowArgs.lpWindowName =  DEFAULT_TEXT;
    this->m_sWindowArgs.dwStyle =       WS_TABSTOP | WS_CHILD | WS_VISIBLE;
    this->m_sWindowArgs.nWidth =        DEFAULT_WIDTH;
    this->m_sWindowArgs.nHeight =       DEFAULT_HEIGHT;
    this->m_sWindowArgs.hMenu =         this->m_id;
    this->m_sWindowArgs.lpParam =       this;

    this->create();

    Main_OnCreate(m_hWinHandle, 0, 0);

    fResult = SetWindowSubclass(this->m_hWinHandle, ChildWindProc, 0, (DWORD_PTR)this);
    
    if (FALSE == fResult) {
        MSG_ERROR(TEXT("SetWindowSubclass failed with error = 0x%X"), GetLastError());
    }

    TH_LEAVE_FUNCTION;
}

thStringGrid::~thStringGrid()
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

int thStringGrid::getDebugIndex()
{
    TH_ENTER_FUNCTION;
    int dReturn = this->m_indexPool;
    this->m_indexPool++;
    TH_LEAVE_FUNCTION;
    return dReturn;
}

LRESULT thStringGrid::processCommandMessage(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    TH_ENTER_FUNCTION;
    LRESULT tResult = 0; // should return 1 if not used (no CB registered)

    if (LOWORD(a_wParam) == reinterpret_cast<WORD>(this->m_id)) {
        switch (HIWORD(a_wParam)) {
        case 0: // workaround for Warning 1 warning C4065: switch statement contains 'default' but no 'case' labels
        default:
            MSG_LOG(TEXT("Not supported %X"), a_uMsg);
            break;
        }
    }
#if 0
    else { //search through children
        thStringGrid *  pFoundChildren = NULL;

        pFoundChildren = reinterpret_cast<thStringGrid*>(findChildrenByID(LOWORD(a_wParam)));

        if (pFoundChildren) {
            tResult = pFoundChildren->processCommandMessage(a_hwnd, a_uMsg, a_wParam, a_lParam);
        }
    }
#endif

    TH_LEAVE_FUNCTION;
    return tResult;
}

LRESULT thStringGrid::processNotifyMessage(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    //TH_ENTER_FUNCTION;
    LRESULT tResult = 0;
    NMHDR * pData = 0;

    pData = reinterpret_cast<NMHDR*>(a_lParam);


#if 0
    if (pData) {
        switch (pData->code) {
        case BCN_DROPDOWN:
            MSG_LOG(TEXT("BCN_DROPDOWN - %s"), this->m_name.c_str());
            tResult = 1;
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

    //TH_LEAVE_FUNCTION;
    return tResult;
}