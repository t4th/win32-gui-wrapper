//////////////////////////////////////////////////////////////////////////////
///
/// @file simpleGrid.c
///
/// @brief A simple low overhead grid control in Win32 SDK C.
///
/// @author David MacDermot
///
/// @par Comments:
///         This grid originated with source code by David Hillard, published
///         on August 22nd, 2002 with his award winning article,
///         hosted on Codeguru and titled: "Win32 Grid Control with Low Overhead (BABYGRID)."
///         Over the course of rewriting the grid into something more to my liking;
///         the code went through many modifications but some pieces of the original
///         may remain.  Mr. Hillard's code provided a great starting point for this
///         project.
///
/// @par Disclaimer:
///         This source is distributed in the hope that it will be useful,
///         but WITHOUT ANY WARRANTY; without even the implied warranty of
///         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
/// 
/// @date 04-08-14
/// 
/// @todo 
///
/// @bug 
///
//////////////////////////////////////////////////////////////////////////////

//DWM 1.1: Removed unused field "editstring" from INSTANCEDATA and all references to it.
//DWM 1.2: Fixed potential bug condition under x64
//DWM 1.3: Rewrote Grid_OnResetContent() and removed unused method Vector_Clear().
//DWM 1.4: Added code to clean up any active in-cell editor to the Grid_OnResetContent() method.
//DWM 1.5: (1) Added message handler for new message SG_SELECTCELL
//         (2) Redefined message handler for SG_SETCURSORPOS so that the WPARAM is treated 
//             as the column argument and the LPARAM is treated as the row argument in order 
//             that they might be consistent with the rest of the messages.
//         (3) Changed the behavior of the grid so that it doesn't initiate a cell edit if
//             just any key is pressed.  Now if the windows key is pressed the selected cell 
//             will not go into edit mode when the windows menu launches.
//         (4) Fixed the Factory method New_SimpleGrid() so that it should work if it is  
//             compiled into a DLL.
//         (5) Fixed bug where mousing away from a cell during edit resulted in the cell not 
//             being updated.
//DWM 1.6: Changes labled thus.
//DWM 1.7: Changes labled thus.
//DWM 1.8: Refactored Notify calls from FORWARD_WM_NOTIFY(GetParent(hwnd)..., to FORWARD_WM_NOTIFY(g_lpInst->hwndParent..., 
//          on principle.
//DWM 1.9: Changes labled thus.
//         (1) Fixed bug where edited text would be reset after resizing a column.
//         (2) Refactored portions to make source easier to port to MSVC++.
//         (3) Editor and button fonts now set to parent grid font.
//DWM 2.0: Changes labeled thus.
//         (1) Fixed bug introduced in 1.9 where button not destroyed  when resizing a button column.

//#define UNICODE
//#define _UNICODE

#define WIN32_LEAN_AND_MEAN /* speed up compilations */
#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <uxtheme.h>
#include <shellapi.h>
#include <tchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <wctype.h>
#include <math.h>
#include "SimpleGrid.h"

#define _tmemcpy wmemcpy
#define _tmemcmp wmemcmp
#define _tmemmove wmemmove
#define _tmemset wmemset

/****************************************************************************/
// Constants
#define VECTOR_INITIAL_CAPACITY 16  ///< Constant
#define VECTOR_CAPACITY_DELTA 16    ///< Constant
#define INITIAL_COL_WIDTH 50        ///< Constant
#define HEIGHT_DESC 80              ///< Constant

//DWM 1.6:Added
#define CONTEXTCODE_ALT 0x2000      ///< Constant

//MAXCOLUMNS is set to limit column header text to 4 Hexavigesimal digits
// if grid is configured to display numbered columns.  If the desired max
// is increased then the call to Alphabetize() in the DisplayColumn() method
// needs to be adjusted accordingly.
#define MAXCOLUMNS 475255           ///< Constant

#define GCT_ROWHEADER -1            ///< Constant

#define ID_EDIT 2001                ///<An Id for an editor
#define ID_COMBO 2002               ///<An Id for an editor
#define ID_BUTTON 2003              ///<An Id for the button

#define EN_EDITENDOK 777            ///<Private notification for cell edit control

#define WPRC _T("Wprc")             ///<Generic property tag
#define EDITMODE _T("EDITMODE")     ///<Edit mode property tag

#define CHECKED _T("T")             ///< GCT_CHECK checked 
#define UNCHECKED _T("F")           ///< GCT_CHECK unchecked

/****************************************************************************/
// Forward declarations
static LRESULT CALLBACK Grid_Proc(HWND, UINT, WPARAM, LPARAM);
static LRESULT Grid_OnSetColWidth(HWND, WPARAM, LPARAM);

/// @name Macroes
/// @{

/// @def IsEmptyString(lpsz)
///
/// @brief Test a string.
///
/// @param lpsz The string to test.
///
/// @returns TRUE if empty
#define IsEmptyString(lpsz) (BOOL)(0 == lpsz[0])

/// @def NELEMS(a)
///
/// @brief Computes number of elements of an array.
///
/// @param a An array.
#define NELEMS(a) (sizeof(a) / sizeof((a)[0]))

/// @def HEIGHT(rect)
///
/// @brief Given a RECT, Computes height.
///
/// @param rect A RECT struct.
#define HEIGHT(rect) ((LONG)(rect.bottom - rect.top))

/// @def WIDTH(rect)
///
/// @brief Given a RECT, Computes width.
///
/// @param rect A RECT struct.
#define WIDTH(rect) ((LONG)(rect.right - rect.left))

/// @def String_Replace(lpszTarget, lpszReplace)
///
/// @brief Replace an allocated string.
///
/// @param lpszTarget The existing allocated string.
/// @param lpszReplace The new string.
#define String_Replace(lpszTarget, lpszReplace) \
    (free((lpszTarget)), (lpszTarget) = NewString(lpszReplace))

/// @def StringArray_Replace(lpszTarget, lpszReplace)
///
/// @brief Replace an allocated string array.
///
/// @param lpszTarget The existing allocated string array.
/// @param lpszReplace The new string array.
#define StringArray_Replace(lpszTarget, lpszReplace) \
    (free((lpszTarget)), (lpszTarget) = NewStringArray(lpszReplace))

/// @}

/// @brief Default window procedure for the grid child windows.
///
/// @param hwnd Handle of grid or child.
/// @param msg Which message?
/// @param wParam Message parameter.
/// @param lParam Message parameter.
///
/// @returns LRESULT depends on message.
static LRESULT DefProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    return CallWindowProc((WNDPROC)GetProp(hwnd, WPRC), hwnd, msg, wParam, lParam);
}

#pragma region Utility

/// @brief Determine whether a string of data represents a number.
///
/// @param data A string to analyze.
///
/// @returns TRUE if is numeric (decimal or floating point) otherwise FALSE.
static BOOL IsNumeric(LPTSTR data)
{
    int j, k, numberofperiods;
    k = _tcslen(data);
    TCHAR tbuffer[256 + 1];
    //TCHAR * tbuffer = new TCHAR(k + 1);
    BOOL DIGIT, ALPHA, WHITESPACE;
    _tcscpy(tbuffer, data);
    k = _tcslen(tbuffer);
    _tcsupr(tbuffer);
    DIGIT = FALSE;
    ALPHA = FALSE;
    WHITESPACE = FALSE;

    numberofperiods = 0;
    for (j = 0; j < k; j++)
    {
        if (_istalpha(tbuffer[j])) ALPHA = TRUE;

        if (_istdigit(tbuffer[j])) DIGIT = TRUE;

        if (_istspace(tbuffer[j])) WHITESPACE = TRUE;

        if (tbuffer[j] == '.') numberofperiods++;

        if (tbuffer[j] == '+' && j > 0) ALPHA = TRUE;

        if (tbuffer[j] == '-' && j > 0) ALPHA = TRUE;
    }

    if ((ALPHA) || (WHITESPACE)) return FALSE;

    if ((DIGIT) && (!ALPHA) && (!WHITESPACE)) {
        return !(numberofperiods > 1);
    }
    return FALSE;
}

/// @brief Safe malloc
///
/// @param size The desired allocation size.
///
/// @returns A pointer to the new allocation.
static PVOID checked_malloc(const size_t size)
{
    PVOID data;

    data = malloc(size);
    if (data == NULL)
    {
        fprintf(stderr, "\nOut of memory.\n");
        fflush(stderr);
        exit(EXIT_FAILURE);
    }

    return data;
}

/// @brief Draw a line.
///
/// @param hdc A handle to a device context.
/// @param x1 From point x-coordinate.
/// @param y1 From point y-coordinate.
/// @param x2 To point x-coordinate.
/// @param y2 To point y-coordinate.
///
/// @returns VOID.
static VOID DrawLine(HDC hdc, LONG x1, LONG y1, LONG x2, LONG y2)
{
    MoveToEx(hdc, x1, y1, NULL);
    LineTo(hdc, x2, y2);
}

/// @brief Draw specified borders of a rectangle.
///
/// @param hdc A handle to a device context.
/// @param lprc The address of a RECT structure with drawing coordinates.
/// @param dwBorder specifies which borders to draw.
/// @param clr The desired line color value.
///
/// @returns VOID.
static VOID DrawBorder(HDC hdc, LPRECT lprc, DWORD dwBorder, COLORREF clr)
{
    LOGPEN oLogPen;

    HPEN hOld;
    GetObject(hOld = (HPEN)SelectObject(hdc, GetStockObject(BLACK_PEN)), //DWM 1.9 Added cast
        sizeof(oLogPen), &oLogPen);
    oLogPen.lopnColor = clr;

    //Don't attempt to delete stock object
    SelectObject(hdc, CreatePenIndirect(&oLogPen));

    if (dwBorder & BF_LEFT)
        DrawLine(hdc, lprc->left, lprc->top, lprc->left, lprc->bottom);
    if (dwBorder & BF_TOP)
        DrawLine(hdc, lprc->left, lprc->top, lprc->right, lprc->top);
    if (dwBorder & BF_RIGHT)
        DrawLine(hdc, lprc->right, lprc->top, lprc->right, lprc->bottom);
    if (dwBorder & BF_BOTTOM)
        DrawLine(hdc, lprc->left, lprc->bottom, lprc->right, lprc->bottom);

    DeleteObject(SelectObject(hdc, hOld));
}

/// @brief Draw the grid Title with the specified font.
///
/// @param hwnd Handle of the grid
/// @param fUnderline True to set underlined font
///
/// @returns a new font with new font weight
static HFONT Font_SetUnderline(HWND hwnd, BOOL fUnderline)
{
    HFONT hFont;
    LOGFONT  lf = {0};

    // Get a handle to the control's font object
    hFont = (HFONT)SendMessage(hwnd, WM_GETFONT, 0, 0);

    // Pull the handle into a Logical Font UDT type
    GetObject(hFont , sizeof(LOGFONT), &lf);

    // Set underline
    lf.lfUnderline = fUnderline;

    // Create a new font based off the logical font UDT
    return CreateFontIndirect(&lf);
}

/// @brief Convert a number to an alphabetic representation.
///
/// @par Example:
///       The number 728 would be converted to "AAZ"
///
/// @note Numeric values less than 1 return FALSE without converting
///        the numeric value.  Also if the buffer is not big enough 
///        to contain the converted value, this function returns FALSE. 
///
/// @param num The number to convert
/// @param buf A buffer to receive text
/// @param iSize The size of the buffer
///
/// @returns TRUE if successful, otherwise false
static BOOL Alphabetize(INT num, LPTSTR buf, INT iSize)
{
    //fill buffer with spaces initially
    for(int i = 0; i < iSize; ++i) buf[i] = ' ';

    //overwrite spaces with characters
    for(int i = iSize - 1; 0 <= i && 0 < num ; --i)
    {
        --num;
        buf[i] = (char)('A' + num % 26);
        num /= 26;
    }
    return 0 >= num;
}

#pragma endregion Utility

#pragma region Vector

/// @var VECTOR
/// @brief A flexable array of objects 

/// @var LPVECTOR
/// @brief Pointer to a VECTOR object

/// @struct tagVECTOR
/// @brief A data structure to hold a flexable array of objects
typedef struct tagVECTOR {
    int _current_capacity;  ///< Current size of storage allocation
    PVOID *_data;           ///< Allocated space for item storage
    int _size;              ///< Number if items in storage
} VECTOR , *LPVECTOR;

/// @brief VECTOR object destructor.
///
/// @note Vector item allocations must be freed individually as
///        this method only frees the list of item pointers.
///
/// @param pVector a pointer to a VECTOR instance
///
/// @returns VOID
static VOID Vector_Free(const LPVECTOR pVector)
{
    free(pVector->_data);
    free(pVector);
}

/// @brief Gets the number of objects allocated in the array
///
/// @param pVector a pointer to a VECTOR instance
///
/// @returns The number of objects
static INT Vector_Size(const LPVECTOR pVector)
{
    return NULL == pVector ? 0 : pVector->_size;
}

/// @brief Create an new instance of VECTOR
///
/// @returns A pointer to the new VECTOR allocation
static LPVECTOR New_Vector(VOID)
{
    LPVECTOR pVector;

    pVector = (LPVECTOR)checked_malloc(sizeof(VECTOR));

    pVector->_current_capacity = VECTOR_INITIAL_CAPACITY;

    pVector->_data = (PVOID*) checked_malloc(sizeof(PVOID) * pVector->_current_capacity);//DWM 1.9 Added cast

    pVector->_size = 0;

    return pVector;
}

/// @brief Add an item to a VECTOR
///
/// @param pVector a pointer to a VECTOR instance
/// @param object a pointer to an item to store in the vector
///
/// @returns TRUE if successful
static BOOL Vector_Add(const LPVECTOR pVector, PVOID object)
{
    int old_size = Vector_Size(pVector);
    int new_capacity;
    PVOID *new_data;//DWM 1.9: Added more explicit pointer

    (pVector->_size)++;
    if (old_size == pVector->_current_capacity)
    {
        new_capacity = pVector->_current_capacity + VECTOR_CAPACITY_DELTA;
        new_data = (PVOID *) checked_malloc(sizeof(PVOID) * new_capacity);//DWM 1.9: Added cast

        memcpy(new_data, pVector->_data, sizeof(PVOID) * old_size);
        free(pVector->_data);
        (pVector->_data) = new_data;
        pVector->_current_capacity = new_capacity;
    }
    (pVector->_data)[old_size] = object;
    return TRUE;
}

/// @brief Access an item stored in the VECTOR
///
/// @param pVector a pointer to a VECTOR instance
/// @param index the array index associated with the desired item
///
/// @returns The desired item if successful, otherwise NULL
static PVOID Vector_Get(const LPVECTOR pVector, const int index)
{
    if (index < Vector_Size(pVector))
    {
        return pVector->_data[index];
    }
    else
    {
        return NULL;
    }
}

#pragma endregion Vector

#pragma region Instance Data

/// @var GRIDCOLUMN
/// @brief An column object

/// @var LPGRIDCOLUMN
/// @brief Pointer to a column

/// @struct tagGRIDCOLUMN
/// @brief This is the data associated with a grid column
typedef struct tagGRIDCOLUMN {
    DWORD dwType;           ///< Column type
    INT iWidth;             ///< Column width
    LPVOID pOptional;       ///< Optional data (ex: combobox choices)
    LPVECTOR items;         ///< The row cells associated with this column
} GRIDCOLUMN   , *LPGRIDCOLUMN;

/// @var GRIDITEM
/// @brief An item object

/// @var LPGRIDITEM
/// @brief Pointer to an item

/// @struct tagGRIDITEM
/// @brief This is the data associated with a grid item
typedef struct tagGRIDITEM {
    LPTSTR lpszCurValue;    ///< Item (cell) value
    LPTSTR lpszMisc;        ///< Item (cell) specific data string
    DWORD dwAllignment;     ///< Item (cell) text allignment
    BOOL fProtected;        ///< Item (cell) protection status
} GRIDITEM   , *LPGRIDITEM;

/// @var INSTANCEDATA
/// @brief Data for this instance of the control

/// @var LPINSTANCEDATA
/// @brief Pointer instance data

/// @struct tagINSTANCEDATA
/// @brief This is the data associated with an instance of the grid
typedef struct tagINSTANCEDATA {
    HINSTANCE hInstance;            ///< Handle to this instance
    HWND hwndParent;                ///< Handle of grid's parent
    HWND hwndControl;               ///< Handle to the current cell control
    HMENU gridmenu;                 ///< The child-window identifier of this grid
    LPVECTOR data;                  ///< A collection of columns of cells
    LPTSTR title;                   ///< The grid title string
    INT gridwidth;                  ///< Grid width
    INT gridheight;                 ///< Grid height
    INT homerow;                    ///< The index of the home row
    INT homecol;                    ///< The index of the home col
    INT rowheight;                  ///< The height of the rows
    INT leftvisiblecol;             ///< The index of the leftmost visible col
    INT rightvisiblecol;            ///< The index of the rightmost visible col
    INT topvisiblerow;              ///< The index of the topmost visible row
    INT bottomvisiblerow;           ///< The index of the bottommost visible row
    INT headerrowheight;            ///< The height of the header row
    INT cursorrow;                  ///< The row index of the cursor
    INT cursorcol;                  ///< The col index of the cursor
    INT visiblecolumns;             ///< The number of visible columns
    INT titleheight;                ///< The height of the grid title
    INT fontascentheight;           ///< The ascent height of the current font
    INT cWheelRemainder;            ///< Mouse wheel scroll units
    COLORREF clrProtect;            ///< Protected cell background color value
    COLORREF clrHighlight;          ///< Highlight cell background color value
    COLORREF clrGridline;           ///< Grid lines color value
    COLORREF clrHighlightText;      ///< Highlight cell text color value
    COLORREF clrBackground;         ///< The normal background color
    COLORREF clrLink;               ///< The normal link color
    DWORD selectionMode;            ///< The Selection mode (full row, cell, or row header click)
    BOOL HIGHLIGHTFULLROW;          ///< Draw highlight full row flag
    BOOL GRIDHASFOCUS;              ///< Grid has focus flag
    RECT activecellrect;            ///< The boundaries of the active cell
    HFONT hfont;                    ///< The handle to the cell text font
    HFONT hcolumnheadingfont;       ///< The handle of the font for column headers
    HFONT htitlefont;               ///< The handle of the font for the grid title
    BOOL ROWSNUMBERED;              ///< Numbered row option flag
    BOOL COLUMNSNUMBERED;           ///< Numbered col option flag
    BOOL EDITABLE;                  ///< The grid editable flag 
    BOOL EXTENDLASTCOLUMN;          ///< Extend last column flag
    BOOL HSCROLL;                   ///< Horizontal scrolling flag
    BOOL VSCROLL;                   ///< Vertical scrolling flag
    BOOL SHOWINTEGRALROWS;          ///< Show integral rows flag
    BOOL REMEMBERINTEGRALROWS;      ///< Remember the integral rows flag during resizing
    BOOL SIZING;                    ///< Sizing flag
    BOOL ELLIPSIS;                  ///< Draw ellipsis flag
    BOOL COLAUTOWIDTH;              ///< Column auto width flag
    BOOL COLUMNSIZING;              ///< Column resizing flag
    BOOL ALLOWCOLUMNRESIZING;       ///< Allow column resizing flag
    BOOL FORCELEFTJUSTIFY;          ///< Numeric types Left justified
    BOOL DOUBLEBUFFER;              ///< Use double buffering while painting
    BOOL LAUNCLINK;                 ///< Link launch flag
    INT columntoresize;             ///< Field to hold the index of the column to resize
    INT columntoresizeinitsize;     ///< Field to hold the initial size of a column to resize
    INT columntoresizeinitx;        ///< Field to hold the x coord of the mouse button during resize 
    INT cursortype;                 ///< Text, pointer, or column resize cursor
} INSTANCEDATA , *LPINSTANCEDATA;

static LPINSTANCEDATA g_lpInst;     ///< instance data (this) pointer

/// @brief Get the Instance data associated with this instance.
///
/// @param hControl Handle to Current instance.
/// @param ppInstanceData - Pointer to the address of an INSTANCEDATA struct. 
///
/// @returns TRUE if successful
static BOOL Control_GetInstanceData(HWND hControl, LPINSTANCEDATA *ppInstanceData)
{
    *ppInstanceData = (LPINSTANCEDATA)GetProp(hControl, (LPCTSTR)_T("lpInsData"));
    if (NULL != *ppInstanceData)
        return TRUE;
    return FALSE;
}

/// @brief Allocate the Instance data associated with this instance.
///
/// @param hControl Handle to current instance.
/// @param pInstanceData Pointer to an INSTANCEDATA struct
///
/// @returns TRUE if successful
static BOOL Control_CreateInstanceData(HWND hControl, LPINSTANCEDATA pInstanceData)
{
    LPINSTANCEDATA pInst = (LPINSTANCEDATA)checked_malloc(sizeof(INSTANCEDATA));
    memmove(pInst, pInstanceData, sizeof(INSTANCEDATA));
    return SetProp(hControl, (LPCTSTR)_T("lpInsData"), pInst);
}

/// @brief Free the instance data allocation of an instance of the Grid Control.
///
/// @param hControl Handle to current instance.
///
/// @returns TRUE if successful
static BOOL Control_FreeInstanceData(HWND hControl)
{
    LPINSTANCEDATA pInst;
    if (Control_GetInstanceData(hControl, &pInst))
    {

        if(NULL != pInst->title) free(pInst->title);
        free((LPINSTANCEDATA)pInst);
        RemoveProp(hControl, (LPCTSTR)_T("lpInsData"));
        return TRUE;
    }
    return FALSE;
}

#pragma endregion Instance Data

#pragma region memory management

/// @brief Allocate and store a string.
///
/// @param str The string to store.
///
/// @returns a Pointer to the allocated string.
static LPTSTR NewString(LPTSTR str)
{
    if (NULL == str || _T('\0') == *str)
        str = _T("");
    LPTSTR tmp = (LPTSTR)calloc(_tcslen(str) + 1, sizeof(TCHAR));

    if (NULL == tmp)
    {
        return (LPTSTR)calloc(1, sizeof(TCHAR));
    }
    return _tmemmove(tmp, str, _tcslen(str));
}

/// @brief Allocate and store a string array (double-null-terminated string).
///
/// @param szzStr The double-null-terminated string to store.
///
/// @returns a Pointer to the allocated string array.
static LPTSTR NewStringArray(LPTSTR szzStr)
{
    if(NULL == szzStr || _T('\0') == *szzStr) szzStr = _T("");

    //Determine total buffer length
    INT iLen = 0;
    //Walk the buffer to find the terminating empty string.
    for (LPTSTR p = szzStr; *p; (p += _tcslen(p) + 1, iLen = p - szzStr)) ;

    //Allocate for array
    LPTSTR tmp = (LPTSTR)calloc(iLen + 1, sizeof(TCHAR));

    if(NULL == tmp)
    {
        return (LPTSTR)calloc(1, sizeof(TCHAR)); 
    }
    return _tmemmove(tmp, szzStr, iLen);
}

/// @brief Append a string to an allocated string array
///
/// @param lpszzTarget The existing allocated string array.
/// @param szSource The string to append.
///
/// @returns VOID.
VOID StringArray_Append(LPTSTR lpszzTarget, LPTSTR szSource)
{
    //Walk the item list to determine the length of the buffer
    LPTSTR p1, p2;
    p1 = p2 = lpszzTarget;
    for (; *p2; p2 += _tcslen(p2) + 1);
    int count = p2 - p1;

    //Add space for the string we wish to append
    count += _tcslen(szSource) + 1;

    lpszzTarget = (LPTSTR) realloc(lpszzTarget, (count + 1) * sizeof(TCHAR));
    _tmemmove(lpszzTarget + count - 1, szSource, _tcslen(szSource) + 1);
    _tmemset(lpszzTarget + count - 1, _T('\0'), 1); // Ensure array terminator
}

/// @brief Allocate and populate a grid item data structure.
///
/// @param szCurValue The item's current value.
///
/// @returns a Pointer to the allocated grid item.
static LPGRIDITEM New_Item(LPTSTR szCurValue)
{
    LPGRIDITEM lpItem = (LPGRIDITEM)calloc(1, sizeof(GRIDITEM));

    lpItem->lpszMisc = NULL; // <- used to store pointer to data allocated
                             // to lpItem->lpszCurValue (if it is an array)
    lpItem->dwAllignment = GSA_GENERAL;
    lpItem->lpszCurValue = NewString(szCurValue);
    return lpItem;
}

/// @brief Free the grid's item data structure.
///
/// @param lpItem A pointer to an item object.
///
/// @returns VOID.
static VOID Item_Delete(LPGRIDITEM lpItem)
{
    if (NULL != lpItem->lpszCurValue)
        free(lpItem->lpszCurValue);
    free(lpItem);
}

/// @brief Allocate and populate a grid column data structure.
///
/// @param lpColumn The column data.
/// @param iWidth The column width.
/// @param lpVector The column vector.
///
/// @returns a Pointer to the allocated grid column.
static LPGRIDCOLUMN New_Column(LPSGCOLUMN lpColumn, INT iWidth, LPVECTOR lpVector)
{
    LPGRIDCOLUMN lpCol = (LPGRIDCOLUMN)calloc(1, sizeof(GRIDCOLUMN));

    lpCol->dwType = lpColumn->dwType;
    switch(lpCol->dwType)
    {
        case GCT_ROWHEADER:
            lpCol->pOptional = NULL;
            break;
        case GCT_EDIT:
            lpCol->pOptional = NULL;
            break;
        case GCT_COMBO:
            lpCol->pOptional = NewStringArray((LPTSTR)lpColumn->pOptional);
            break;
        case GCT_BUTTON:
            lpCol->pOptional = NULL;
            break;
        case GCT_CHECK:
            lpCol->pOptional = NULL;
            break;
        case GCT_LINK:
            lpCol->pOptional = NULL;
            break;
        case GCT_IMAGE:
            lpCol->pOptional = (HIMAGELIST)lpColumn->pOptional;
            break;
    }
    lpCol->iWidth = iWidth;
    lpCol->items = lpVector;
    return lpCol;
}

/// @brief Free the grid's column data structure.
///
/// @param lpColumn A pointer to a LPGRIDCOLUMN object.
///
/// @returns VOID.
static VOID Column_Delete(LPGRIDCOLUMN lpColumn)
{
    if (NULL != lpColumn->items)
    {
        int nCells = Vector_Size(lpColumn->items);
        for (int cell = 0; cell < nCells; ++cell)
        {
            Item_Delete((LPGRIDITEM)Vector_Get(lpColumn->items, cell));
        }
        Vector_Free(lpColumn->items);
    }
    switch (lpColumn->dwType)
    {
        case GCT_EDIT:
        case GCT_BUTTON:
        case GCT_CHECK:
        case GCT_LINK:
            break;
        case GCT_COMBO:
            if (NULL != lpColumn->pOptional)
            {
                free(lpColumn->pOptional);
            }
            break;
        case GCT_IMAGE:
            if (NULL != lpColumn->pOptional)
            {
                ImageList_Destroy((HIMAGELIST)lpColumn->pOptional);
            }
            break;
    }
    free(lpColumn);
}

#pragma endregion memory management

#pragma region private methods

/// @brief wrapper to access column count (including the row header column 0)
///
/// @returns The number of columns in the underlying data table
static int ColCount(VOID)
{
    return Vector_Size(g_lpInst->data);
}

/// @brief wrapper to access row count (including the column header row 0)
///
/// @returns The number of rows in the underlying data table
static int RowCount(VOID)
{
    LPGRIDCOLUMN lpgc = (LPGRIDCOLUMN)Vector_Get(g_lpInst->data, 0);
    if(NULL == lpgc)
        return 0;

    LPVECTOR temp = lpgc->items;
    return NULL == temp ? 0 : Vector_Size(temp);
}

/// @brief wrapper to access column width
///
/// @param col The column number
///
/// @returns The width of the column
static int GetColWidth(INT col)
{
    LPGRIDCOLUMN lpgc = (LPGRIDCOLUMN)Vector_Get(g_lpInst->data, col);
    if(NULL == lpgc)
        return 0;

    return lpgc->iWidth;
}

/// @brief wrapper to access column type
///
/// @param col The column number
///
/// @returns The type of the column
static DWORD GetColType(INT col)
{
    LPGRIDCOLUMN lpgc = (LPGRIDCOLUMN)Vector_Get(g_lpInst->data, col);
    if(NULL == lpgc)
        return 0;

    return lpgc->dwType;
}

/// @brief Wrapper to access column optional parameter
///
/// @param col The column number
///
/// @returns The column optional parameter
static LPVOID GetColOptional(INT col)
{
    LPGRIDCOLUMN lpgc = (LPGRIDCOLUMN)Vector_Get(g_lpInst->data, col);
    if(NULL == lpgc)
        return NULL;

    return lpgc->pOptional;
}

/// @brief wrapper to set column optional parameter
///
/// @param col The column number
/// @param object Object to store
///
/// @returns VOID
static VOID SetColOptional(INT col, PVOID object)
{
    LPGRIDCOLUMN lpgc = (LPGRIDCOLUMN)Vector_Get(g_lpInst->data, col);
    if(NULL != lpgc)
        lpgc->pOptional = object;
}

/// @brief Gets the contents of a cell.
///
/// @param col The column that the cell belongs to
/// @param row The row index of the cell in the column
///
/// @returns The item and data associated with the cell
static LPGRIDITEM GetCellData(int col, int row)
{
    if(-1 < col && -1 < row)
    {
        LPGRIDCOLUMN lpgc = (LPGRIDCOLUMN)Vector_Get(g_lpInst->data, col);
        
        if(NULL != lpgc)
        {
            return (LPGRIDITEM)Vector_Get(lpgc->items, row);
        }
    }
    // If we get here we failed
    return NULL;
}

/// @brief Calculate the number of visible columns.
///
/// @note Columns are "hidden" when it's width is 0.
///
/// @returns The number of visible columns
static int HomeColumnNthVisible(VOID)
{
    int j, hc, count;
    count = 0;
    hc = g_lpInst->homecol;
    for (j = 1; j <= hc; j++)
    {
        if (0 < GetColWidth(j))
        {
            count++;
        }
    }
    return count;
}

/// @brief Get the next adjacent column or previous adjacent column.
///
/// @param startcol The number of the column to begin search.
/// @param fNextCol TRUE to get the next column
///                  FALSE to get the previous column.
///
/// @returns The number of the desired adjacent column or
///           0 if no more cols in that direction.
static int GetAdjacentCol(int startcol, BOOL fNextCol)
{
    //calls with direction == 1 for right, direction == -1 for left
    //returns 0 if no more cols in that direction, else column number
    int j = startcol;
    int ReturnValue;
    int colCount = ColCount();
    if (fNextCol)
    {
        j++;
    }
    else
    {
        j--;
    }

    while ((j < colCount) && (j > 0) && (0 == GetColWidth(j)))
    {
        if (fNextCol)
        {
            j++;
        }
        else
        {
            j--;
        }
    }
    if ((j < colCount) && (0 < GetColWidth(j)))
    {
        ReturnValue = j;
    }
    else
    {
        ReturnValue = 0;
    }
    return ReturnValue;
}

/// @brief Calculates the rectangle of a cell location.
///
/// @param hwnd Handle of the grid
/// @param col The column that the cell belongs to
/// @param row The row index of the cell in the column
/// @param prc A pointer to a RECT to recieve coordinates
///
/// @returns VOID
static VOID GetCellRect(HWND hwnd, int col, int row, PRECT prc)
{
    int offset;
    int j;
    //col and row must be greater than zero

    //get column offset
    //first get col 0 width
    offset = GetColWidth(0);
    for (j = g_lpInst->homecol; j < col; j++)
    {
        offset += GetColWidth(j);
    }
    prc->left = offset;
    prc->right = offset + GetColWidth(col);

    if (g_lpInst->EXTENDLASTCOLUMN)
    {
        //see if this is the last column
        if (!GetAdjacentCol(col, TRUE))
        {
            //extend this column
            RECT trect = {0,0,0,0};
            int temp;
            GetClientRect(hwnd, &trect);
            temp = (offset + (trect.right - prc->left)) - prc->left;
            if (temp > GetColWidth(col))
            {
                prc->right = offset + (trect.right - prc->left);
            }
        }
    }

    //now get the top and bottom of the rect
    offset = g_lpInst->headerrowheight + g_lpInst->titleheight;
    for (j = g_lpInst->homerow; j < row; j++)
    {
        offset += g_lpInst->rowheight;
    }
    prc->top = offset;
    prc->bottom = offset + g_lpInst->rowheight;
}

/// @brief Cause the grid to be repainted.
///
/// @param hwnd Handle of the grid
///
/// @returns VOID
static VOID RefreshGrid(HWND hwnd)
{
    RECT rect = {0,0,0,0};
    GetClientRect(hwnd, &rect);
    InvalidateRect(hwnd, &rect, FALSE);
}

/// @brief Gets the row of the mouse pointer.
///
/// @param y The y coordinate of the mouse pointer
///
/// @returns The row number of the mouse pointer if successful, otherwise SG_ERROR
static int GetRowOfMouse(int y)
{
    int ReturnValue;
    if (y <= (g_lpInst->titleheight))
    {
        return -1;
    }
    if ((y >= g_lpInst->titleheight) && (y <= g_lpInst->headerrowheight + g_lpInst->titleheight))
    {
        return 0;   //Row 0
    }


    y = y - (g_lpInst->headerrowheight + g_lpInst->titleheight);
    y = y / g_lpInst->rowheight;
    ReturnValue = g_lpInst->homerow + y;
    if (ReturnValue > RowCount() - 1)
    {
        SetLastError(ERROR_INVALID_INDEX);
        ReturnValue = SG_ERROR;
    }
    return ReturnValue;
}

/// @brief Gets the column of the mouse pointer.
///
/// @param x The x coordinate of the mouse pointer
///
/// @returns The col number of the mouse pointer if successful, otherwise SG_ERROR
static int GetColOfMouse(int x)
{
    int ReturnValue;
    int j;
    int colCount = ColCount();

    if (x <= GetColWidth(0))
    {
        return 0;   //Col 0 row headers
    }

    x -= GetColWidth(0);

    j = g_lpInst->homecol;
    while ((j < colCount) && (x > 0))
    {
        x -= GetColWidth(j);
        j++;
    }
    j--;

    ReturnValue = j;
    if (g_lpInst->EXTENDLASTCOLUMN)
    {
        if (j > colCount)
        {
            ReturnValue = colCount;
        }
    }
    else
    {
        if (j > colCount)
        {
            SetLastError(ERROR_INVALID_INDEX);
            ReturnValue = SG_ERROR;
        }
    }
    return ReturnValue;
}

/// @brief Determines if a cells coordinates are out of range.
///
/// @param cell A cell struct pointer (x and y coordinate)
///
/// @returns TRUE if the cell is out of range (doesn't exist), otherwise FALSE
static BOOL OutOfRange(LPSGITEM cell)
{
    if ((cell->row > RowCount()) || (cell->col > ColCount()))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/// @brief Wrapper for setting cell coordinates.
///
/// @returns VOID
static VOID CalcVisibleCellBoundaries(VOID)
{
    int gridx, gridy;
    int j;
    int cols = ColCount() - 1;  //Exclude header
    int rows = RowCount() - 1;  //Exclude header
    gridx = g_lpInst->gridwidth;
    gridy = g_lpInst->gridheight;

    j = g_lpInst->homecol;
    g_lpInst->leftvisiblecol = g_lpInst->homecol;
    g_lpInst->topvisiblerow = g_lpInst->homerow;
    //calc columns visible
    //first subtract the width of col 0;
    gridx = gridx - GetColWidth(0);
    do
    {
        gridx = gridx - GetColWidth(j);
        j++;
    }
    while ((j < cols) && (gridx >= 0));

    if (j > cols)
    {
        j = cols;
    }
    g_lpInst->rightvisiblecol = j;

    //calc rows visible;
    gridy = gridy - g_lpInst->headerrowheight;
    j = g_lpInst->homerow;
    do
    {
        gridy = gridy - g_lpInst->rowheight;
        j++;
    }
    while ((gridy > 0) && (j < rows));

    if (j > rows)
    {
        j = rows;
    }
    g_lpInst->bottomvisiblerow = j;
}

/// @brief Determine if it is necessary to show the VScroll and then
///         show it if necessary.
///
/// @param hwnd Handle of the grid
///
/// @returns VOID
static VOID ShowVscroll(HWND hwnd)
{
    //if more rows than can be visible on grid, display vertical scrollbar
    //otherwise, hide it.
    RECT gridrect = {0,0,0,0};
    int totalpixels;
    int rowsvisibleonscreen;
    int rows = RowCount();
    GetClientRect(hwnd, &gridrect);
    totalpixels = gridrect.bottom;
    totalpixels -= g_lpInst->titleheight;
    totalpixels -= g_lpInst->headerrowheight;
    totalpixels -= (g_lpInst->rowheight * rows);
    rowsvisibleonscreen = (gridrect.bottom - (g_lpInst->headerrowheight + g_lpInst->titleheight)) / g_lpInst->rowheight;
    if (totalpixels < 0)
    {
        //show vscrollbar
        ShowScrollBar(hwnd, SB_VERT, TRUE);
        SetScrollRange(hwnd, SB_VERT, 1, (rows - rowsvisibleonscreen) + 1, TRUE);
        g_lpInst->VSCROLL = TRUE;
    }
    else
    {
        //hide vscrollbar
        ShowScrollBar(hwnd, SB_VERT, FALSE);
        g_lpInst->VSCROLL = FALSE;
    }

}

/// @brief Determine if it is necessary to show the HScroll and then
///         show it if necessary.
///
/// @param hwnd Handle of the grid
///
/// @returns VOID
static VOID ShowHscroll(HWND hwnd)
{
    //if more rows than can be visible on grid, display vertical scrollbar
    //otherwise, hide it.
    RECT gridrect = {0,0,0,0};
    int totalpixels;
    int colswithwidth;
    int cols = ColCount();
    int j;
    GetClientRect(hwnd, &gridrect);
    totalpixels = gridrect.right;
    totalpixels -= GetColWidth(0);
    colswithwidth = 0;
    for (j = 1; j < cols; j++)
    {
        totalpixels -= GetColWidth(j);
        if (0 < GetColWidth(j))
        {
            colswithwidth++;
        }
    }
    if (totalpixels < 0)
    {
        //show hscrollbar
        ShowScrollBar(hwnd, SB_HORZ, TRUE);
        SetScrollRange(hwnd, SB_HORZ, 1, colswithwidth, TRUE);
        g_lpInst->HSCROLL = TRUE;
    }
    else
    {
        //hide hscrollbar
        ShowScrollBar(hwnd, SB_HORZ, FALSE);
        g_lpInst->HSCROLL = FALSE;
    }

}

/// @brief Draw the grid Title with the specified font.
///
/// @param hwnd Handle of the grid
/// @param hfont Handle of the desired font
/// @param hdc Handle of the current device context
/// @param rc A rectangle of the bounds of the client area
///
/// @returns VOID
static VOID DisplayTitle(HWND hwnd, HFONT hfont, HDC hdc, RECT rc)
{
    HFONT hOldfont;

    SetBkMode(hdc, TRANSPARENT);
    hOldfont = (HFONT)SelectObject(hdc, hfont);
    rc.bottom = g_lpInst->titleheight;
    DrawEdge(hdc, &rc, EDGE_ETCHED, BF_MIDDLE | BF_RECT | BF_ADJUST);
    DrawTextEx(hdc, g_lpInst->title, -1, &rc, DT_END_ELLIPSIS | DT_CENTER | DT_WORDBREAK | DT_NOPREFIX, NULL);
    SelectObject(hdc, hOldfont);
}

/// @brief Draw a column of cells in the grid.
///
/// @param hwnd Handle of the grid
/// @param col The index of the column to draw
/// @param offset number of pixles from the left to begin drawing
/// @param hfont The handle of the desired cell font
/// @param hcolumnheadingfont The handle of the font used for the headers
/// @param hdc Handle of the current device context
///
/// @returns VOID
static VOID DisplayColumn(HWND hwnd, int col, int offset, HFONT hfont, HFONT hcolumnheadingfont, HDC hdc)
{
    RECT rect = {0,0,0,0};
    RECT rectsave = {0,0,0,0};
    HFONT holdfont;
    HBRUSH hbrush, holdbrush;
    HPEN hpen, holdpen;
    LPGRIDITEM lpgi;
    BOOL isProtected;
    BOOL isNumeric = FALSE;
    int row, iColumnType;

    //max 1024 chars display in an excel worksheet cell so
    // I'll use that for a max buffer size
    static TCHAR buffer[1025]; 
    memset(&buffer, 0, sizeof(buffer));

    if (0 == GetColWidth(col))
    {
        return;
    }
    iColumnType = GetColType(col);

    SetBkMode(hdc, TRANSPARENT);
    ShowHscroll(hwnd);
    ShowVscroll(hwnd);

    rect.left = offset;
    rect.top = g_lpInst->titleheight;
    rect.right = GetColWidth(col) + offset;
    rect.bottom = g_lpInst->headerrowheight + g_lpInst->titleheight;

    if (g_lpInst->EXTENDLASTCOLUMN)
    {
        //see if this is the last column
        if (!GetAdjacentCol(col, TRUE))
        {
            //extend this column
            RECT trect = {0,0,0,0};

            GetClientRect(hwnd, &trect);

            rect.right = offset + (trect.right - rect.left);
        }
    }
    else //Not extended column
    {
        if (!GetAdjacentCol(col, TRUE))
        {
            //repaint right side of grid
            RECT trect = {0,0,0,0};
            GetClientRect(hwnd, &trect);
            trect.left = offset + (rect.right - rect.left);
            holdbrush = (HBRUSH)SelectObject(hdc, GetStockObject(GRAY_BRUSH));
            holdpen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
            Rectangle(hdc, trect.left, trect.top + g_lpInst->titleheight, trect.right + 1, trect.bottom + 1);
            //Don't attempt to delete stock objects
            SelectObject(hdc, holdbrush);
            SelectObject(hdc, holdpen);
        }
    }

    //
    //display column header cell (Row 0)
    //
    row = 0;

    if (col > 0)
    {
        if (g_lpInst->COLUMNSNUMBERED)  //Print hexavigesimal digits
        {
            Alphabetize(col, buffer, 4); //Max 4 place column number EX: ABCD
        }
        else //Print column header text (if any)
        {
            lpgi = GetCellData(col, row);
            _tcscpy(buffer, NULL == lpgi ? _T("") : lpgi->lpszCurValue);
        }
    }
    rectsave = rect;

    SetTextColor(hdc, IsWindowEnabled(hwnd) ? 
        GetSysColor(COLOR_WINDOWTEXT) : GetSysColor(COLOR_GRAYTEXT));

    DrawEdge(hdc, &rect, EDGE_ETCHED, BF_MIDDLE | BF_RECT | BF_ADJUST);

    holdfont = (HFONT)SelectObject(hdc, hcolumnheadingfont);
    DrawTextEx(hdc, buffer, -1, &rect, DT_END_ELLIPSIS | DT_CENTER | DT_WORDBREAK | DT_NOPREFIX, NULL);
    SelectObject(hdc, holdfont);//Don't delete 

    rect = rectsave;

    //
    // Display visible cells
    //
    row = g_lpInst->topvisiblerow;
    //set font for grid body
    SelectObject(hdc, hfont);

    while (row <= g_lpInst->bottomvisiblerow)
    {
        rect.top = rect.bottom;
        rect.bottom = rect.top + g_lpInst->rowheight;
        rectsave = rect;
        lpgi = GetCellData(col, row);
        isProtected = NULL == lpgi ? 0 : lpgi->fProtected;
        _tcscpy(buffer, NULL == lpgi ? _T("") : lpgi->lpszCurValue);

        // Draw Cell if not row header
        if(GCT_ROWHEADER != iColumnType)// no need to draw row header twice
        {
            //set cursor (or entire cursor row) to different display color
            if (((g_lpInst->HIGHLIGHTFULLROW && row == g_lpInst->cursorrow) ||
                (! g_lpInst->HIGHLIGHTFULLROW && row == g_lpInst->cursorrow && col == g_lpInst->cursorcol))
                && g_lpInst->GRIDHASFOCUS)
            {
                switch(iColumnType)
                {
                    case GCT_ROWHEADER:
                    case GCT_BUTTON:
                        SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
                        break;
                    default:
                        SetTextColor(hdc, g_lpInst->clrHighlightText);
                        break;
                }
                hbrush = CreateSolidBrush(g_lpInst->clrHighlight);
            }
            else //Non hilight colors
            {
                if (isProtected)
                {
                    SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));
                    hbrush = CreateSolidBrush(g_lpInst->clrProtect);
                }
                else //Normal in cell text color
                {
                    if (GCT_LINK == iColumnType)
                        SetTextColor(hdc, g_lpInst->clrLink);
                    else
                        SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));

                    hbrush = CreateSolidBrush(g_lpInst->clrBackground);
                }
            }
            hpen = CreatePen(PS_SOLID, 1, g_lpInst->clrGridline);
            holdbrush = (HBRUSH)SelectObject(hdc, hbrush);
            holdpen = (HPEN)SelectObject(hdc, hpen);
            Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
            DeleteObject(SelectObject(hdc, holdpen));
            DeleteObject(SelectObject(hdc, holdbrush));
        }

        // Draw row header as well as controls and text over other cells
        switch(iColumnType)
        {
            case GCT_ROWHEADER:
            {
                if (g_lpInst->ROWSNUMBERED)
                {
                    isNumeric = TRUE;
                    wsprintf(buffer, _T("%d"), row);
                }
                else
                    isNumeric = FALSE;

                SetTextColor(hdc, IsWindowEnabled(hwnd) ? 
                    GetSysColor(COLOR_WINDOWTEXT) : GetSysColor(COLOR_GRAYTEXT));

                DrawEdge(hdc, &rect, EDGE_ETCHED, BF_MIDDLE | BF_RECT | BF_ADJUST);

                UINT uFormat = DT_END_ELLIPSIS | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX;
                uFormat |= isNumeric ? DT_RIGHT : DT_LEFT;

                DrawTextEx(hdc, buffer, -1, &rect, uFormat, NULL);
            }
                break;
            case GCT_BUTTON:
            {
                InflateRect(&rect, -2, -2);
                DrawFrameControl(hdc, &rect, DFC_BUTTON, DFCS_BUTTONPUSH);
                if (g_lpInst->ELLIPSIS)
                {
                    DrawTextEx(hdc, buffer, -1, &rect, DT_END_ELLIPSIS | DT_LEFT | DT_VCENTER | DT_CENTER | DT_SINGLELINE | DT_NOPREFIX, NULL);
                }
                else
                {
                    DrawTextEx(hdc, buffer, -1, &rect, DT_LEFT | DT_VCENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_NOPREFIX, NULL);
                }
            }
                break;
            case GCT_COMBO:
            {
                //Calculate and draw drop down button
                RECT rectButton = {0,0,0,0};
                CopyRect(&rectButton, &rect);
                InflateRect(&rectButton, -2, -2);
                rectButton.left = rectButton.right - GetSystemMetrics(SM_CXVSCROLL);
                rectButton.top += (HEIGHT(rectButton) - GetSystemMetrics(SM_CYVSCROLL)) / 2;
                rectButton.bottom = rectButton.top + GetSystemMetrics(SM_CYVSCROLL);
                DrawFrameControl(hdc, &rectButton, DFC_SCROLL, DFCS_SCROLLCOMBOBOX);

                rect.right = rectButton.left - 1;
                rect.left += 5;

                if (g_lpInst->ELLIPSIS)
                {
                    DrawTextEx(hdc, buffer, -1, &rect, DT_END_ELLIPSIS | DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX, NULL);
                }
                else
                {
                    DrawTextEx(hdc, buffer, -1, &rect, DT_LEFT | DT_VCENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_NOPREFIX, NULL);
                }
            }
                break;
            case GCT_CHECK:
            {
                //Calculate and draw checkbox
                RECT rectCheck = {0,0,0,0};
                CopyRect(&rectCheck, &rect);

                rectCheck.left += (WIDTH(rect) - GetSystemMetrics(SM_CXVSCROLL)) /2;
                rectCheck.top += (HEIGHT(rect) - GetSystemMetrics(SM_CYVSCROLL)) /2;
                rectCheck.right = rectCheck.left + GetSystemMetrics(SM_CXVSCROLL);
                rectCheck.bottom = rectCheck.top + GetSystemMetrics(SM_CYVSCROLL);

                DrawFrameControl(hdc, &rectCheck, DFC_BUTTON, DFCS_BUTTONCHECK |
                    (0 == _tcsicmp(buffer, CHECKED) ? DFCS_CHECKED : 0));
            }
                break;
            case GCT_IMAGE:
            {
                //Calculate and draw image
                RECT rectImage = {0,0,0,0};
                LONG index = 0;
                LPTSTR end;
                UINT style = ILD_NORMAL;;
                CopyRect(&rectImage, &rect);
                HIMAGELIST hImageList = (HIMAGELIST)GetColOptional(col);
                int imageWidth, imageHeight;
                ImageList_GetIconSize(hImageList, &imageWidth, &imageHeight);

                rectImage.left += (WIDTH(rect) - imageWidth) /2;
                rectImage.top += (HEIGHT(rect) - imageHeight) /2;
                rectImage.right = rectImage.left + imageWidth;
                rectImage.bottom = rectImage.top + imageHeight;

                index = _tcstol(buffer, &end, 10);
                if(0 < _tcslen(end)) // we failed but I don't know how this could have happened
                {
                    index = 0;
                }

                if (g_lpInst->GRIDHASFOCUS)
                {
                    if ((g_lpInst->HIGHLIGHTFULLROW && row == g_lpInst->cursorrow) || 
                        (col == g_lpInst->cursorcol && row == g_lpInst->cursorrow))
                    {
                        style = ILD_SELECTED;
                    }
                }
                ImageList_DrawEx(hImageList, index, hdc, 
                    rectImage.left, rectImage.top, WIDTH(rectImage), HEIGHT(rectImage), 
                    g_lpInst->clrBackground, CLR_DEFAULT, style | ILD_TRANSPARENT);
            }
                break;
            case GCT_LINK:
            {
                InflateRect(&rect, -2, -2);

                // Draw the link underlined and in the link color
                HFONT oldFont;
                oldFont = (HFONT)SelectObject (hdc, Font_SetUnderline(hwnd, TRUE));

                if (g_lpInst->ELLIPSIS)
                    DrawTextEx(hdc, buffer, -1, &rect, DT_END_ELLIPSIS | DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX, NULL);
                else
                    DrawTextEx(hdc, buffer, -1, &rect, DT_LEFT | DT_VCENTER | DT_WORDBREAK | DT_EDITCONTROL | DT_NOPREFIX, NULL);

                DeleteObject(SelectObject(hdc, oldFont));
            }
                break;
            default: //Text
            {
                InflateRect(&rect, -2, -2);

                DWORD dwAllignment =  NULL == lpgi ? GSA_GENERAL : lpgi->dwAllignment;
                if (g_lpInst->ELLIPSIS)
                {
                    UINT uFormat =  DT_NOPREFIX | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS;
                    uFormat |= GSA_LEFT == dwAllignment ? DT_LEFT :
                        GSA_RIGHT == dwAllignment ? DT_RIGHT : // must be GSA_GENERAL
                         IsNumeric(buffer) ? DT_RIGHT : DT_LEFT;
                    DrawTextEx(hdc, buffer, -1, &rect, uFormat, NULL);
                }
                else
                {
                    UINT uFormat = DT_NOPREFIX | DT_WORDBREAK | DT_EDITCONTROL;
                    uFormat |= GSA_LEFT == dwAllignment ? DT_LEFT :
                        GSA_RIGHT == dwAllignment ? DT_RIGHT : // must be GSA_GENERAL
                         IsNumeric(buffer) ? DT_RIGHT : DT_LEFT;
                    DrawTextEx(hdc, buffer, -1, &rect, uFormat, NULL);
                }
            }
        }
        rect = rectsave;
        row++;
    }   //end while r<=bottomvisiblerow

    //repaint bottom of grid
    RECT trect = {0,0,0,0};
    GetClientRect(hwnd, &trect);
    trect.top = rect.bottom;
    trect.left = rect.left;
    trect.right = rect.right;

    holdbrush = (HBRUSH)SelectObject(hdc, GetStockObject(GRAY_BRUSH));
    holdpen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
    Rectangle(hdc, trect.left, trect.top, trect.right + 1, trect.bottom + 1);
    SelectObject(hdc, holdbrush);
    SelectObject(hdc, holdpen);
}

/// @brief Sets the home row field of the grid based on current cursor position.
///
/// @param hwnd Handle of the grid
/// @param col The column that the current cell belongs to
/// @param row The row index of the current cell in the column 
///
/// @returns VOID
static VOID SetHomeRow(HWND hwnd, int col, int row)
{
    RECT gridrect = {0,0,0,0};
    RECT cellrect = {0,0,0,0};
    //get rect of grid window
    GetClientRect(hwnd, &gridrect);
    //get rect of current cell
    GetCellRect(hwnd, col, row, &cellrect);
    if ((cellrect.bottom > gridrect.bottom) && ((cellrect.bottom - cellrect.top) < (gridrect.bottom - (g_lpInst->headerrowheight + g_lpInst->titleheight))))
    {
        while (cellrect.bottom > gridrect.bottom)
        {
            g_lpInst->homerow++;
            if (row == RowCount())
            {
                gridrect.top = gridrect.bottom - (g_lpInst->rowheight);
                InvalidateRect(hwnd, &gridrect, TRUE);
            }
            else
            {
                InvalidateRect(hwnd, &gridrect, FALSE);
            }
            GetCellRect(hwnd, col, row, &cellrect);
        }
    }
    else
    {
        if ((cellrect.bottom - cellrect.top) >= (gridrect.bottom - (g_lpInst->headerrowheight + g_lpInst->titleheight)))
        {
            g_lpInst->homerow++;
        }
    }
    GetCellRect(hwnd, col, row, &cellrect);
    {
        while ((row < g_lpInst->homerow))
        {
            g_lpInst->homerow--;
            InvalidateRect(hwnd, &gridrect, FALSE);
            GetCellRect(hwnd, col, row, &cellrect);
        }
    }
    //set the vertical scrollbar position
    SetScrollPos(hwnd, SB_VERT, g_lpInst->homerow, TRUE);
}

/// @brief Sets the home column field of the grid based on current cursor position.
///
/// @param hwnd Handle of the grid
/// @param col The column that the current cell belongs to
/// @param row The row index of the current cell in the column 
///
/// @returns VOID
static VOID SetHomeCol(HWND hwnd, int col, int row)
{
    RECT gridrect = {0,0,0,0};
    RECT cellrect = {0,0,0,0};
    //get rect of grid window
    GetClientRect(hwnd, &gridrect);
    //get rect of current cell
    GetCellRect(hwnd, col, row, &cellrect);
    //determine if scroll left or right is needed
    while ((cellrect.right > gridrect.right) && (cellrect.left != GetColWidth(0)))
    {
        //scroll right is needed
        g_lpInst->homecol++;

        GetCellRect(hwnd, col, row, &cellrect);
        InvalidateRect(hwnd, &gridrect, FALSE);
    }
    GetCellRect(hwnd, col, row, &cellrect);
    while ((g_lpInst->cursorcol < g_lpInst->homecol) && (g_lpInst->homecol > 1))

    {
        //scroll left is needed
        g_lpInst->homecol--;

        GetCellRect(hwnd, col, row, &cellrect);
        InvalidateRect(hwnd, &gridrect, FALSE);

        int k;
        k = HomeColumnNthVisible();
        SetScrollPos(hwnd, SB_HORZ, k, TRUE);
    }
}

/// @brief Sets the visible columns field to the number of visible columns.
///
/// @param hwnd Handle of the grid
///
/// @returns VOID
static void SetVisibleColumns(HWND hwnd)
{
    int j;
    int cols = ColCount();
    int value;
    value = 0;
    for (j = 1; j < cols; j++)
    {
        if (0 < GetColWidth(j))
        {
            value++;
        }
    }
    g_lpInst->visiblecolumns = value;
    SetScrollRange(hwnd, SB_HORZ, 1, value, TRUE);
}

/// @brief Gets the last visible column based upon scroll position.
///
/// @param hwnd Handle of the grid
/// @param n The horizontal scroll position
///
/// @returns The last visible column
static int GetNthVisibleColumn(HWND hwnd, int n)
{
    int j, count;
    int cols = ColCount();
    int value;
    j = 1;
    count = 0;
    value = n - 1;
    while (j < cols)
    {
        if (0 < GetColWidth(j))
        {
            count++;
            if (count == n)
            {
                value = j;
            }
        }
        j++;
    }
    return value;
}

/// @brief If there are \n codes in the string, find the longest line
///
/// @param hdc Handle of the device context
/// @param text The text to analyze
/// @param size Pointer to a SIZE struct to receive text height and width
///
/// @returns The length of the longest line of multiline text
static int FindLongestLine(HDC hdc, LPTSTR text, PSIZE size)
{
    int longest, lines, j, len;
    len = _tcslen(text);
    TCHAR temptext[256 + 1];
    //TCHAR * temptext = new TCHAR(len + 1);
    LPTSTR p;
    longest = 0;
    lines = 1;
    for (j = 0; j < len; j++)
    {
        if (text[j] == _T('\n'))
        {
            lines++;
        }
    }
    _tcscpy(temptext, text);
    p = _tcstok(temptext, _T("\n"));
    while (p)
    {
        GetTextExtentPoint32(hdc, p, _tcslen(p), size);
        if (size->cx > longest)
        {
            longest = size->cx;
        }
        p = _tcstok(_T('\0'), _T("\n"));
    }
    //delete temptext;
    return longest;
}

/// @brief Calculate and increase the parent column width based on the length of
///         the item text.
///
/// @param hwnd The handle of the grid
/// @param col The column of the item
/// @param row The row of the item
/// @param lpszValue The item text
///
/// @returns VOID
static VOID AdjustParentColWidth(HWND hwnd, INT col, INT row, LPTSTR lpszValue)
{
    HDC hdc;
    SIZE size = {0,0};
    int required_width;
    int current_width;
    int required_height;
    int current_height;
    int longestline;
    HFONT holdfont;
    hdc = GetDC(hwnd);
    if (0 == row)
    {
        holdfont = (HFONT)SelectObject(hdc, g_lpInst->hcolumnheadingfont);
    }
    else
    {
        holdfont = (HFONT)SelectObject(hdc, g_lpInst->hfont);
    }
    //if there are \n codes in the string, find the longest line
    longestline = FindLongestLine(hdc, lpszValue, &size);

    required_width = longestline + 5;
    required_height = size.cy;
    //count lines
    {
        int count = 1;
        //TCHAR * tbuffer = new TCHAR(_tcslen(lpszValue) + 1);
       TCHAR tbuffer[256 + 1];
        _tcscpy(tbuffer, lpszValue);
        for (int j = 0; j < NELEMS(tbuffer); j++)
        {
            if (tbuffer[j] == _T('\n'))
            {
                count++;
            }
        }
        if ((!g_lpInst->ELLIPSIS) || (row == 0))
        {
            required_height *= count;
        }
        required_height += 5;
        //delete tbuffer;
    }
    SelectObject(hdc, holdfont);
    ReleaseDC(hwnd, hdc);

    current_width = GetColWidth(col);
    if (0 == row)
    {
        current_height = g_lpInst->headerrowheight;
        if (required_height > current_height)
        {
            SimpleGrid_SetHeaderRowHeight(hwnd, required_height);
        }
    }
    else
    {
        current_height = g_lpInst->rowheight;
        if (required_height > current_height)
        {
            SimpleGrid_SetRowHeight(hwnd, required_height);
        }

    }
    if (required_width > current_width)
    {
        Grid_OnSetColWidth(hwnd, col, required_width);
    }
}

#pragma endregion private methods

#pragma region notifications

static NMGRID g_nmGrid; ///< Grid notification re-usable instance
static NMSGFOCUS g_nmSGFocus; ///< Grid focus notification re-usable instance

/// @brief Notify Parent that the selection has changed.
///
/// @param hwnd Handle of the grid
///
/// @returns VOID
static void NotifySelChange(HWND hwnd)
{
    //Destroy previous editor
    if (NULL != g_lpInst->hwndControl)
    {
        DestroyWindow(g_lpInst->hwndControl);
        g_lpInst->hwndControl = NULL;
    }

    g_nmGrid.col = g_lpInst->cursorcol - 1;
    g_nmGrid.row = g_lpInst->cursorrow - 1;
    g_nmGrid.dwType = GetColType(g_lpInst->cursorcol);
    g_nmGrid.hdr.hwndFrom = hwnd;
    g_nmGrid.hdr.idFrom = GetDlgCtrlID(g_nmGrid.hdr.hwndFrom);
    g_nmGrid.hdr.code = SGN_SELCHANGE;
    FORWARD_WM_NOTIFY(g_lpInst->hwndParent, g_nmGrid.hdr.idFrom, &g_nmGrid, SNDMSG);
}

/// @brief Notify Parent that a cell edit has begun.
///
/// @param hwnd Handle of the grid
///
/// @returns VOID
static void NotifyEditBegin(HWND hwnd)
{
    g_nmGrid.col = g_lpInst->cursorcol - 1;
    g_nmGrid.row = g_lpInst->cursorrow - 1;
    g_nmGrid.dwType = GetColType(g_lpInst->cursorcol);
    g_nmGrid.hdr.hwndFrom = hwnd;
    g_nmGrid.hdr.idFrom = GetDlgCtrlID(g_nmGrid.hdr.hwndFrom);
    g_nmGrid.hdr.code = SGN_EDITBEGIN;
    FORWARD_WM_NOTIFY(g_lpInst->hwndParent, g_nmGrid.hdr.idFrom, &g_nmGrid, SNDMSG);
}

/// @brief Notify Parent that a cell edit has ended.
///
/// @param hwnd Handle of the grid
///
/// @returns VOID
static void NotifyEditEnd(HWND hwnd)
{
    //Destroy previous editor
    if (NULL != g_lpInst->hwndControl)//DWM 1.9: Added
    {
        DestroyWindow(g_lpInst->hwndControl);
        g_lpInst->hwndControl = NULL;
    }
    g_nmGrid.col = g_lpInst->cursorcol - 1;
    g_nmGrid.row = g_lpInst->cursorrow - 1;
    g_nmGrid.dwType = GetColType(g_lpInst->cursorcol);
    g_nmGrid.hdr.hwndFrom = hwnd;
    g_nmGrid.hdr.idFrom = GetDlgCtrlID(g_nmGrid.hdr.hwndFrom);
    g_nmGrid.hdr.code = SGN_EDITEND;
    FORWARD_WM_NOTIFY(g_lpInst->hwndParent, g_nmGrid.hdr.idFrom, &g_nmGrid, SNDMSG);
}

/// @brief Notify Parent that a grid cell was clicked.
///
/// @param hwnd Handle of the grid
///
/// @returns VOID
static void NotifyCellClick(HWND hwnd)
{
    memset(&g_nmGrid,0,sizeof( g_nmGrid));
    g_nmGrid.col = g_lpInst->cursorcol - 1;
    g_nmGrid.row = g_lpInst->cursorrow - 1;
    g_nmGrid.dwType = GetColType(g_lpInst->cursorcol);
    g_nmGrid.hdr.hwndFrom = hwnd;
    g_nmGrid.hdr.idFrom = GetDlgCtrlID(g_nmGrid.hdr.hwndFrom);
    g_nmGrid.hdr.code = SGN_ITEMCLICK;
    FORWARD_WM_NOTIFY(g_lpInst->hwndParent, g_nmGrid.hdr.idFrom, &g_nmGrid, SNDMSG);
}

/// @brief Notify Parent that a key was pressed.
///
/// @param hwnd Handle of the grid
/// @param vk Virtual key code
///
/// @returns VOID
static void NotifyKeydown(HWND hwnd, UINT vk)
{
    static NMSGKEYDOWN nmSgkd;
    nmSgkd.col = g_lpInst->cursorcol - 1;
    nmSgkd.row = g_lpInst->cursorrow - 1;
    nmSgkd.dwType = GetColType(g_lpInst->cursorcol);
    nmSgkd.wVKey = vk;
    nmSgkd.hdr.hwndFrom = hwnd;
    nmSgkd.hdr.idFrom = GetDlgCtrlID(nmSgkd.hdr.hwndFrom);
    nmSgkd.hdr.code = SGN_KEYDOWN;
    FORWARD_WM_NOTIFY(g_lpInst->hwndParent, nmSgkd.hdr.idFrom, &nmSgkd, SNDMSG);
}

/// @brief Notify Parent that the grid got focus.
///
/// @param hwnd Handle of the grid
/// @param hwndOldFocus Handle of window losing focus
///
/// @returns VOID
static void NotifyGotFocus(HWND hwnd, HWND hwndOldFocus)
{
    g_nmSGFocus.col = g_lpInst->cursorcol - 1;
    g_nmSGFocus.row = g_lpInst->cursorrow - 1;
    g_nmSGFocus.dwType = GetColType(g_lpInst->cursorcol);
    g_nmSGFocus.hFocus = hwndOldFocus;
    g_nmSGFocus.hdr.hwndFrom = hwnd;
    g_nmSGFocus.hdr.idFrom = GetDlgCtrlID(g_nmGrid.hdr.hwndFrom);
    g_nmSGFocus.hdr.code = SGN_GOTFOCUS;
    FORWARD_WM_NOTIFY(g_lpInst->hwndParent, g_nmSGFocus.hdr.idFrom, &g_nmSGFocus, SNDMSG);
}

/// @brief Notify Parent that the grid lost focus.
///
/// @param hwnd Handle of the grid
/// @param hwndNewFocus Handle of window receiving focus
///
/// @returns VOID
static void NotifyLostFocus(HWND hwnd, HWND hwndNewFocus)
{
    g_nmSGFocus.col = g_lpInst->cursorcol - 1;
    g_nmSGFocus.row = g_lpInst->cursorrow - 1;
    g_nmSGFocus.dwType = GetColType(g_lpInst->cursorcol);
    g_nmSGFocus.hFocus = hwndNewFocus;
    g_nmSGFocus.hdr.hwndFrom = hwnd;
    g_nmSGFocus.hdr.idFrom = GetDlgCtrlID(g_nmGrid.hdr.hwndFrom);
    g_nmSGFocus.hdr.code = SGN_LOSTFOCUS;
    FORWARD_WM_NOTIFY(g_lpInst->hwndParent, g_nmSGFocus.hdr.idFrom, &g_nmSGFocus, SNDMSG);
}

#pragma endregion notifications

#pragma region Edit Control

/// @brief Center the text in an edit control.
///
/// @param hEdit The handle of an edit control.
/// @note This only works with mulitline edit controls (ES_MULTILINE).
///
/// @returns VOID.
VOID Edit_CenterTextVertically(HWND hEdit)
{
    RECT rcTxt = {0,0,0,0};
    RECT rcEdt = {0,0,0,0};
    HDC hdc;

    //calculate client area height needed for a font
    hdc = GetDC(hEdit);
    DrawText(hdc, TEXT("Ky"), 2, &rcTxt, DT_CALCRECT | DT_LEFT);
    ReleaseDC(hEdit, hdc);

    // Set top and left margins
    GetClientRect(hEdit, &rcEdt);
    rcEdt.left += 4;
    rcEdt.top = ((rcEdt.bottom - (rcTxt.bottom - rcTxt.top)) / 2);

    Edit_SetRect(hEdit, &rcEdt);
}

/// @brief Window procedure for the edit control.
///
/// @param hEdit Handle of editor.
/// @param msg Which message?
/// @param wParam Message parameter.
/// @param lParam Message parameter.
///
/// @returns LRESULT depends on message.
static LRESULT CALLBACK Edit_Proc(HWND hEdit, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hGrid = GetParent(hEdit);

    // Note: Instance data is attached to Edit's parent
    Control_GetInstanceData(hGrid, &g_lpInst);

    if (WM_DESTROY == msg)  // Unsubclass the Edit Control
    {
        SetWindowLongPtr(hEdit, GWLP_WNDPROC, (DWORD_PTR)GetProp(hEdit, WPRC));//DWM 1.2: fixed cast
        RemoveProp(hEdit, WPRC);
        RemoveProp(hEdit, EDITMODE);//DWM 1.7: Added
        return 0;
    }
    else if (WM_MOUSEWHEEL == msg)
    {
        FORWARD_WM_CHAR(hEdit, VK_RETURN, 0, SNDMSG);
    }
    else if (WM_CHAR == msg && VK_RETURN == wParam)
    {
        FORWARD_WM_COMMAND(hGrid,GetDlgCtrlID(hEdit),hEdit,EN_EDITENDOK,SNDMSG);
        return TRUE;    // handle Enter (NO BELL)
    }
    else if (WM_KEYDOWN == msg)
    {
        switch (wParam)
        {
            case VK_LEFT:
            case VK_RIGHT:
                if(CHECKED == (LPTSTR)GetProp(hEdit, EDITMODE)) break;//DWM 1.7: Added
                //else fallthrough
            case VK_NEXT:
            case VK_PRIOR:
            case VK_DOWN:
            case VK_UP:
            case VK_TAB:
                //Finish edit
                FORWARD_WM_CHAR(hEdit, VK_RETURN, 0, SNDMSG);
                //Forward to Grid proc
                return SNDMSG(hGrid, msg, wParam, lParam);

            case VK_ESCAPE:
                ShowWindow(hEdit, SW_HIDE);
                SetFocus(hGrid);
                return FALSE;
        }
    }
    return DefProc(hEdit, msg, wParam, lParam);
}

/// @brief Create an Edit control to edit GCT_EDIT fields.
///
/// @param hInstance The handle of an instance.
/// @param hwndParent The handle of the parent (the visible listbox).
/// @param id An id tag for this control.
/// @param fEditMode TRUE if EDIT control in edit mode FALSE for overwrite.
///
/// @returns HWND A handle to the edit control.
static HWND CreateEdit(HINSTANCE hInstance, HWND hwndParent, INT id, BOOL fEditMode)//DWM 1.7: Added fEditMode argument
{
    DWORD dwStyle, dwExStyle;
    HWND hwnd;

    dwStyle = WS_CHILD | ES_LEFT | ES_WANTRETURN | ES_MULTILINE | ES_AUTOHSCROLL;

    dwExStyle = WS_EX_LEFT | WS_EX_TRANSPARENT; // | WS_EX_CLIENTEDGE;

    hwnd = CreateWindowEx(dwExStyle, 
        WC_EDIT, 
        TEXT(""), 
        dwStyle, 
        CW_USEDEFAULT, 
        CW_USEDEFAULT, 
        CW_USEDEFAULT, 
        CW_USEDEFAULT, 
        hwndParent, 
        (HMENU)id, 
        hInstance, 
        NULL);

    if (!hwnd)
        return NULL;

    //DWM 1.9: Display the same font in editor as rest of grid
    SendMessage(hwnd, WM_SETFONT, (WPARAM)SendMessage(hwndParent, WM_GETFONT, 0L, 0L), 0L);

    //DWM 1.7: Store EditMode flag
    SetProp(hwnd, EDITMODE, fEditMode ? CHECKED : UNCHECKED);

    // Subclass Editor and save the OldProc
    SetProp(hwnd, WPRC, (HANDLE)GetWindowLongPtr(hwnd, GWLP_WNDPROC));
    SubclassWindow(hwnd, Edit_Proc);

    return hwnd;
}

#pragma endregion Edit Control

#pragma region Button Control

/// @brief Window procedure for the button control.
///
/// @param hButton Handle of button control.
/// @param msg Which message?
/// @param wParam Message parameter.
/// @param lParam Message parameter.
///
/// @returns LRESULT depends on message.
static LRESULT CALLBACK Button_Proc(HWND hButton, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hGrid = GetParent(hButton);

    // Note: Instance data is attached to buttons's parent
    Control_GetInstanceData(hGrid, &g_lpInst);

    if (WM_DESTROY == msg)  // Unsubclass the button control
    {
        SetWindowLongPtr(hButton, GWLP_WNDPROC, (DWORD_PTR)GetProp(hButton, WPRC));//DWM 1.2: fixed cast
        RemoveProp(hButton, WPRC);
        return 0;
    }
    else if (WM_MOUSEWHEEL == msg)
    {
        FORWARD_WM_KEYDOWN(hButton, VK_ESCAPE, 0, 0, SNDMSG);
    }
    else if (WM_GETDLGCODE == msg)
    {
        return DLGC_WANTALLKEYS;
    }
    else if (WM_KEYUP == msg && VK_RETURN == wParam)
    {
        FORWARD_WM_KEYUP(hButton, VK_SPACE, 0, 0, SNDMSG);
        return TRUE;
    }
    else if (WM_KEYDOWN == msg)
    {
        switch (wParam)
        {
            case VK_RETURN:
                FORWARD_WM_KEYDOWN(hButton, VK_SPACE, 0, 0, SNDMSG);
                return TRUE;

            case VK_NEXT:
            case VK_PRIOR:
            case VK_DOWN:
            case VK_UP:
            case VK_LEFT:
            case VK_RIGHT:
            case VK_TAB:
                //Forward to Grid proc
                return SNDMSG(hGrid, msg, wParam, lParam);

            case VK_ESCAPE:
                ShowWindow(hButton, SW_HIDE);
                SetFocus(hGrid);
                return FALSE;
        }
    }
    return DefProc(hButton, msg, wParam, lParam);
}

/// @brief Create button control for GCT_BUTTON column.
///
/// @param hInstance The handle of an instance.
/// @param hwndParent The handle of the parent (the visible listbox).
/// @param id An id tag for this control.
///
/// @returns HWND A handle to the button control.
static HWND CreateButton(HINSTANCE hInstance, HWND hwndParent, INT id)
{
    DWORD dwStyle;
    HWND hwnd;

    dwStyle = WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON;

    hwnd = CreateWindowEx(0,
        WC_BUTTON,
        TEXT("..."),
        dwStyle,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        hwndParent,
        (HMENU)id,
        hInstance,
        NULL);

    if (!hwnd)
        return NULL;

    //Disable visual styles for the time being since the button doesn't match the
    // psuedo button in this grid when drawn using the Vista and later styles.
    SetWindowTheme(hwnd, L" ", L" ");

    //DWM 1.9: Display the same font in buttons as rest of grid
    SendMessage(hwnd, WM_SETFONT, (WPARAM)SendMessage(hwndParent, WM_GETFONT, 0L, 0L), 0L);

    SetProp(hwnd, WPRC, (HANDLE)GetWindowLongPtr(hwnd, GWLP_WNDPROC));
    SubclassWindow(hwnd, Button_Proc);

    return hwnd;
}

#pragma endregion Button Control

#pragma region Combo Box

/// @brief Calculate the position of the combobox within a rectangle.
///
/// @param hCombo The handle of a combobox control.
/// @param prc The address of a rectangle with the bounds in which to center the combobox.
///
/// @returns VOID.
VOID Combo_CalculateVerticalPos(HWND hCombo, PRECT prc)
{
    RECT rcCombo = {0,0,0,0};
    GetClientRect(hCombo, &rcCombo);

    int hight = HEIGHT((*prc));
    int hightCombo = HEIGHT(rcCombo);

    prc->top += (hight - hightCombo) / 2;
}

/// @brief Window procedure for the combobox control.
///
/// @param hCombo Handle of the combobox.
/// @param msg Which message?
/// @param wParam Message parameter.
/// @param lParam Message parameter.
///
/// @par Comments
///       The combobox control wraps a listbox (drop down) and an edit control
///       child (if it is an editable list box).  The edit control child needs
///       to be subclassed in order to handle keyboard events.  Each child control
///       posts some notification to the parent combobox control and I use this
///       behavior to capture and subclass the child edit control.  This edit control is
///       subclassed to this very procedure and so 'hwnd' could be parent combobox or
///       child edit control.  Care then, must be taken to differentiate between
///       child and parent.  I do this by getting the class name and restricting
///       the handling of certain messages to one or the other control type.
/// @par Warning
///       The drop down list control of a combobox is not a child of the combobox
///       it is the child of the desktop so that the list is not clipped by the
///       combobox's client area.  Do not sub class it to this procedure since
///       doing so will cause the instance data pointer to be reset to NULL!  The
///       instance data pointer is attached to the Grid's window.
///
/// @returns LRESULT depends on message.
static LRESULT CALLBACK ComboBox_Proc(HWND hCombo, UINT msg, WPARAM wParam, LPARAM lParam)
{
    HWND hGrid = GetParent(hCombo);

    // Note: Instance data is attached to combo's parent
    Control_GetInstanceData(hGrid, &g_lpInst);

    if (WM_DESTROY == msg)  //Unsubclass the combobox or child edit control
    {
        SetWindowLongPtr(hCombo, GWLP_WNDPROC, (DWORD_PTR)GetProp(hCombo, WPRC));//DWM 1.2: fixed cast
        RemoveProp(hCombo, WPRC);
        return 0;
    }
    else if (WM_PAINT == msg) // Obliterate border
    {
        // First let the system do its thing
        DefProc(hCombo, msg, wParam, lParam);

        // Next obliterate the border
        HDC hdc = GetWindowDC(hCombo);
        RECT rect = {0,0,0,0};

        GetClientRect(hCombo, &rect);

        rect.bottom -= 2;
        DrawBorder(hdc, &rect, BF_TOPLEFT | BF_BOTTOM, g_lpInst->clrBackground);
        rect.top += 1;
        rect.left += 1;
        DrawBorder(hdc, &rect, BF_TOPLEFT, g_lpInst->clrBackground);

        ReleaseDC(hCombo, hdc);
        return TRUE;
    }
    else if (WM_GETDLGCODE == msg)
    {
        return DLGC_WANTALLKEYS;
    }
    else if (WM_CHAR == msg && VK_RETURN == wParam)
    {
        ShowWindow(hCombo, SW_HIDE);
        return TRUE;    // handle Enter (NO BELL)
    }
    else if (WM_CHAR == msg && VK_TAB == wParam)
    {
        if (GetKeyState(VK_SHIFT) & 0x8000)
        {
            FORWARD_WM_CHAR(hCombo, VK_RETURN, 0, SNDMSG);
        }
        else //Focus to grid parent
        {
            ShowWindow(hCombo, SW_HIDE);
            SetFocus(GetParent(hGrid));
        }
        return TRUE;
    }
    else if (WM_KEYDOWN == msg)
    {
        switch (wParam)
        {
            case VK_ESCAPE:
                ShowWindow(hCombo, SW_HIDE);
                SetFocus(hGrid);
                return FALSE;
        }
    }
    return DefProc(hCombo, msg, wParam, lParam);
}

/// @brief Create combobox control configured as a dropdown list.
///
/// @param hInstance The handle of an instance.
/// @param hwndParent The handle of the parent (the visible listbox).
/// @param id An id tag for this control.
///
/// @returns HWND A handle to the control.
static HWND CreateCombo(HINSTANCE hInstance, HWND hwndParent, INT id)
{
    DWORD dwStyle, dwExStyle;
    HWND hwnd;

    dwStyle = WS_CHILD | CBS_NOINTEGRALHEIGHT | CBS_DROPDOWNLIST;

    dwExStyle = WS_EX_LEFT;

    hwnd = CreateWindowEx(dwExStyle,
        WC_COMBOBOX,
        NULL,
        dwStyle,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        hwndParent,
        (HMENU)id,
        hInstance,
        NULL);

    if (!hwnd)
        return NULL;

    //Disable visual styles for the time being since the combo looks bad
    // in this grid when drawn using the Vista and later styles.
    SetWindowTheme(hwnd, L" ", L" ");

    //DWM 1.9: Display the same font in editor as rest of grid
    SendMessage(hwnd, WM_SETFONT, (WPARAM)SendMessage(hwndParent, WM_GETFONT, 0L, 0L), 0L);

    SetProp(hwnd, WPRC, (HANDLE)GetWindowLongPtr(hwnd, GWLP_WNDPROC));
    SubclassWindow(hwnd, ComboBox_Proc);

    return hwnd;
}

#pragma endregion Combo Box

#pragma region create destroy

/// @brief Handles WM_CREATE message.
///
/// @param hwnd Handle of grid.
/// @param lpCreateStruct Pointer to a structure with creation data.
///
/// @returns BOOL If an application processes this message,
///                it should return TRUE to continue creation of the window.
static BOOL Grid_OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
    BOOL fSuccess = FALSE;

    INSTANCEDATA inst;
    memset(&inst, 0, sizeof(INSTANCEDATA));

    inst.hInstance = lpCreateStruct->hInstance;
    inst.hwndParent = lpCreateStruct->hwndParent;
    inst.gridmenu = lpCreateStruct->hMenu;

    //Create the data table
    inst.data = New_Vector();

    //Add Column 0 (Row headers)
    LPVECTOR temp = New_Vector();
    //First element (the column name) Should be blank for row header column
    fSuccess = Vector_Add(temp, New_Item(_T("")));

    if (fSuccess)
    {
        SGCOLUMN sgc;
        sgc.dwType = GCT_ROWHEADER;
        sgc.lpszHeader = _T("");
        sgc.pOptional = NULL;
        fSuccess = Vector_Add(inst.data, New_Column(&sgc, INITIAL_COL_WIDTH, temp));
    }

    if (fSuccess)
    {
        inst.homerow = 1;
        inst.homecol = 1; 
        inst.rowheight = 21;
        inst.headerrowheight = 21;
        inst.ROWSNUMBERED = TRUE;
        inst.COLUMNSNUMBERED = TRUE;
        inst.EDITABLE = TRUE;
        inst.cursorcol = 1;
        inst.cursorrow = 1;
        inst.HIGHLIGHTFULLROW = FALSE;
        inst.DOUBLEBUFFER = TRUE;
        inst.cWheelRemainder = 0;
        inst.clrProtect = GetSysColor(COLOR_WINDOW);
        inst.clrHighlight = GetSysColor(COLOR_HIGHLIGHT);
        inst.clrLink = GetSysColor(COLOR_HIGHLIGHT);
        inst.LAUNCLINK = TRUE;
        inst.clrGridline = GetSysColor(COLOR_BTNFACE);
        inst.clrHighlightText = GetSysColor(COLOR_HIGHLIGHTTEXT);
        inst.clrBackground = GetSysColor(COLOR_WINDOW);
        inst.title = NewString(_T(""));
        inst.titleheight = 0;
        inst.EXTENDLASTCOLUMN = TRUE;
        inst.SHOWINTEGRALROWS = TRUE;
        inst.SIZING = FALSE;
        inst.ELLIPSIS = TRUE;
        inst.COLAUTOWIDTH = FALSE;
        inst.COLUMNSIZING = FALSE;
        inst.ALLOWCOLUMNRESIZING = FALSE;
        inst.FORCELEFTJUSTIFY = FALSE;
        inst.cursortype = 0;
        inst.hcolumnheadingfont = NULL;
        inst.htitlefont = NULL;

        fSuccess = Control_CreateInstanceData(hwnd, &inst);
    }
    return fSuccess;
}

/// @brief Handles WM_DESTROY message.
///
/// @param hwnd Handle of Grid.
///
/// @returns VOID.
static VOID Grid_OnDestroy(HWND hwnd)
{
    if (NULL != g_lpInst)
    {
        int nCols = Vector_Size(g_lpInst->data);
        for (int col = 0; col < nCols; ++col)
        {
            Column_Delete((LPGRIDCOLUMN)Vector_Get(g_lpInst->data, col));
        }
        Vector_Free(g_lpInst->data);
        Control_FreeInstanceData(hwnd);
    }
}

/// @brief Initialize and register the simple grid class.
///
/// @param hInstance Handle of application instance.
///
/// @returns ATOM If the function succeeds, the atom that uniquely identifies
///                the class being registered, otherwise 0.
ATOM InitSimpleGrid(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(wcex);
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hCursor = NULL;
    wcex.hIcon = NULL;
    wcex.hIconSm = NULL;
    wcex.hbrBackground = NULL;
    wcex.lpszMenuName = NULL;
    wcex.style = CS_BYTEALIGNCLIENT;
    wcex.lpfnWndProc = (WNDPROC)Grid_Proc;
    wcex.hInstance = hInstance;
    wcex.lpszClassName = WC_SIMPLEGRID;

    // Register our new class
    return RegisterClassEx(&wcex);
}

/// @brief Create an new instance of the simple grid.
///
/// @param hParent Handle of the grid's parent.
/// @param dwID The ID for this control.
///
/// @returns HWND If the function succeeds, the grid handle, otherwise NULL.
HWND New_SimpleGrid(HWND hParent, DWORD dwID)
{
    static ATOM aControl = 0;
    static HWND hControl;

    HINSTANCE hinst;

    //Get hinstance if this code is compiled into and called from a dll as well as if it were
    // compiled into the executable.  (XP and later)
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,(LPCTSTR)New_SimpleGrid, &hinst);

    //HINSTANCE hinst = (HINSTANCE)GetWindowLongPtr(hParent, GWLP_HINSTANCE);

    //Only need to register the property grid once
    if (!aControl)
        aControl = InitSimpleGrid(hinst);

    hControl = CreateWindowEx(0, WC_SIMPLEGRID, _T(""), WS_CHILD | WS_TABSTOP, 0, 0, 0, 0, hParent, (HMENU)dwID, hinst, NULL);

    return hControl;
}

#pragma endregion create destroy

#pragma region event handlers

/// @brief Handles WM_HSCROLL message.
///
/// @param hwnd Handle of grid.
/// @param hwndCtl If this message is sent by a scroll bar control,
///                 this parameter is the handle to the scroll bar control.
///                However in this case the message was sent by a standard
///                 scroll bar, thus this parameter is NULL.
/// @param code The scroll bar value that indicates the user's scrolling request.
/// @param pos The current position of the scroll box if the @b code is
///             SB_THUMBPOSITION or SB_THUMBTRACK
///
/// @returns VOID
static VOID Grid_OnHScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
    SetFocus(hwnd);
    if (code == SB_LINERIGHT || code == SB_PAGERIGHT)
    {
        int cp, np;
        cp = GetScrollPos(hwnd, SB_HORZ);
        SetScrollPos(hwnd, SB_HORZ, cp + 1, TRUE);
        cp = GetScrollPos(hwnd, SB_HORZ);
        np = GetNthVisibleColumn(hwnd, cp);
        g_lpInst->homecol = np;
        SetScrollPos(hwnd, SB_HORZ, cp, TRUE);
        RefreshGrid(hwnd);
    }
    if (code == SB_LINELEFT || code == SB_PAGELEFT)
    {
        int cp, np;
        cp = GetScrollPos(hwnd, SB_HORZ);
        SetScrollPos(hwnd, SB_HORZ, cp - 1, TRUE);
        cp = GetScrollPos(hwnd, SB_HORZ);
        np = GetNthVisibleColumn(hwnd, cp);
        g_lpInst->homecol = np;
        SetScrollPos(hwnd, SB_HORZ, cp, TRUE);
        RefreshGrid(hwnd);
    }
    if (code == SB_THUMBTRACK)
    {
        int np;
        np = GetNthVisibleColumn(hwnd, pos);
        SetScrollPos(hwnd, SB_HORZ, np, TRUE);
        g_lpInst->homecol = np;
        SetScrollPos(hwnd, SB_HORZ, pos, TRUE);
        RefreshGrid(hwnd);
    }
}

/// @brief Handle the selection of a grid column of type GCT_EDIT.
///
/// @param hwnd The handle of the grid.
/// @param rc RECT containing desired coordinates for the edit control.
/// @param text The initial character or text for the editor.
/// @param fEditMode TRUE if EDIT control in edit mode FALSE for overwrite.
///
/// @returns VOID.
static VOID Grid_OnSelectEdit(HWND hwnd, RECT rc, LPTSTR text, BOOL fEditMode)//DWM 1.7: Added fEditMode argument
{
    //Adjust rect so text will center better
    rc.top += 1;
    InflateRect(&rc, -1,-1);

    //display edit box
    if (NULL == g_lpInst->hwndControl)
        g_lpInst->hwndControl = CreateEdit(g_lpInst->hInstance, hwnd, ID_EDIT, fEditMode);

    MoveWindow(g_lpInst->hwndControl, rc.left, rc.top, WIDTH(rc), HEIGHT(rc), TRUE);

    //Set the text in the edit box to the initial key
    Edit_SetText(g_lpInst->hwndControl, text);
    ShowWindow(g_lpInst->hwndControl, SW_SHOW);
    SetFocus(g_lpInst->hwndControl);
    Edit_CenterTextVertically(g_lpInst->hwndControl);

    //DWM 1.6: Was Edit_SetSel(g_lpInst->hwndControl, 1, -1);
    FORWARD_WM_KEYDOWN(g_lpInst->hwndControl,VK_END,0,0,SNDMSG);

    NotifyEditBegin(hwnd);
}

/// @brief Handle the selection of a grid column of type GCT_BUTTON.
///
/// @param hwnd The handle of the grid.
/// @param rc RECT containing desired coordinates for the button control.
/// @param text The initial text for the button.
///
/// @returns VOID.
static VOID Grid_OnSelectButton(HWND hwnd, RECT rc, LPTSTR text)
{
    InflateRect(&rc, -1,-1);

    if (NULL == g_lpInst->hwndControl)
        g_lpInst->hwndControl = CreateButton(g_lpInst->hInstance, hwnd, ID_BUTTON);

    MoveWindow(g_lpInst->hwndControl, rc.left, rc.top, WIDTH(rc), HEIGHT(rc), TRUE);

    //Set the text in the edit box to the initial key
    Button_SetText(g_lpInst->hwndControl, text);
    ShowWindow(g_lpInst->hwndControl, SW_SHOW);
    SetFocus(g_lpInst->hwndControl);
}

/// @brief Handle the selection of a grid column of type GCT_COMBO.
///
/// @param hwnd The handle of the grid.
/// @param rc RECT containing desired coordinates for the button control.
/// @param szzItems The drop down item choices 
///                  (double null terminated string array).
/// @param szSelected The currently selected item (if any).
///
/// @returns VOID.
static VOID Grid_OnSelectComboBox(HWND hwnd, RECT rc, LPTSTR szzItems, LPTSTR szSelected)
{
    InflateRect(&rc, -1, -1);

    if (NULL == g_lpInst->hwndControl)
        g_lpInst->hwndControl = CreateCombo(g_lpInst->hInstance, hwnd, ID_COMBO);

    ComboBox_ResetContent(g_lpInst->hwndControl);

    Combo_CalculateVerticalPos(g_lpInst->hwndControl, &rc);
    rc.bottom += 100;

    MoveWindow(g_lpInst->hwndControl, rc.left, rc.top, WIDTH(rc), HEIGHT(rc), TRUE);

    //Walk the item list and add each string until the empty string
    for (LPTSTR p = szzItems; *p; p += _tcslen(p) + 1)
    {
        if (CB_ERR == ComboBox_FindStringExact(g_lpInst->hwndControl, 0, p))
            ComboBox_AddString(g_lpInst->hwndControl, p);
    }

    ShowWindow(g_lpInst->hwndControl, SW_SHOW);
    SetFocus(g_lpInst->hwndControl);

    //Jump to the property's current value in the combo box
    INT itm = ComboBox_FindStringExact(g_lpInst->hwndControl, 0, szSelected);
    if (itm != CB_ERR)
        ComboBox_SetCurSel(g_lpInst->hwndControl, itm);
    else
    {
        ComboBox_SetCurSel(g_lpInst->hwndControl, 0);
        ComboBox_SetText(g_lpInst->hwndControl, szSelected);
        ComboBox_SetEditSel(g_lpInst->hwndControl, 0, -1);
    }
    NotifyEditBegin(hwnd);
}

/// @brief Handles WM_KEYDOWN message.
///
/// @param hwnd  Handle of grid.
/// @param vk The virtual key code.
/// @param fDown TRUE for keydown (always TRUE).
/// @param cRepeat The number of times the keystroke is repeated
///         as a result of the user holding down the key.
/// @param flags Indicate OEM scan codes etc.
///
/// @returns VOID.
static void Grid_OnKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
    int rows = RowCount() - 1;  //Exclude header row

    NotifyKeydown(hwnd, vk);

    switch (vk)
    {
        case VK_NEXT:
        case VK_PRIOR:
        case VK_DOWN:
        case VK_UP:
        {
            RECT gridrect = {0,0,0,0};
            int rpp;
            if (rows == 0)
                break;

            if (g_lpInst->cursorrow == (VK_NEXT == vk || VK_DOWN == vk ? rows : 1))
                break;

            //get rows per page
            GetClientRect(hwnd, &gridrect);
            rpp = (gridrect.bottom - (g_lpInst->headerrowheight + g_lpInst->titleheight)) / g_lpInst->rowheight;

            if(VK_NEXT == vk || VK_DOWN == vk)
            {
                g_lpInst->cursorrow += (VK_NEXT == vk ? rpp : 1);
                if (g_lpInst->cursorrow > rows)
                    g_lpInst->cursorrow = rows;
                else
                {
                    NotifySelChange(hwnd);
                }
            }
            else //VK_PRIOR || VK_UP
            {
                g_lpInst->cursorrow -= (VK_PRIOR == vk ? rpp : 1);
                if (g_lpInst->cursorrow < 1)
                    g_lpInst->cursorrow = 1;
                else
                {
                    NotifySelChange(hwnd);
                }
            }

            SetHomeRow(hwnd, g_lpInst->cursorcol, g_lpInst->cursorrow);
            SetHomeCol(hwnd, g_lpInst->cursorcol, g_lpInst->cursorrow);
            RefreshGrid(hwnd);
        }
            break;
        case VK_RIGHT:
            if(GSO_ROWHEADER == g_lpInst->selectionMode &&
                g_lpInst->HIGHLIGHTFULLROW)
            {
                //We selected off the row header so turn the hilight off
                g_lpInst->HIGHLIGHTFULLROW = FALSE;
                SetHomeRow(hwnd, g_lpInst->cursorcol, g_lpInst->cursorrow);
                SetHomeCol(hwnd, g_lpInst->cursorcol, g_lpInst->cursorrow);
                RefreshGrid(hwnd);
                break;
            }
            // Everything else
            // FALL THROUGH
        case VK_LEFT:
        {
            int k = GetAdjacentCol(g_lpInst->cursorcol, VK_RIGHT == vk);

            if (!k) break;

            g_lpInst->cursorcol = k;

            NotifySelChange(hwnd);

            SetHomeRow(hwnd, g_lpInst->cursorcol, g_lpInst->cursorrow);
            SetHomeCol(hwnd, g_lpInst->cursorcol, g_lpInst->cursorrow);
            RefreshGrid(hwnd);
        }
            break;
        default:
        {
            //DWM 1.5: Exclude certain keys from triggering edit by default
            if((VK_F1 <= vk && vk <= VK_F24) ||
                VK_TAB == vk || //DWM 1.7: Added
                VK_RETURN == vk ||
                VK_MENU == vk ||
                VK_PAUSE == vk ||
                VK_CAPITAL == vk ||
                VK_INSERT == vk ||
                VK_NUMLOCK == vk ||
                VK_LWIN == vk ||
                VK_RWIN == vk ||
                VK_APPS == vk) break;

            if(! g_lpInst->EDITABLE)
                break; //Grid editing is disabled

            if(GSO_ROWHEADER == g_lpInst->selectionMode && g_lpInst->HIGHLIGHTFULLROW)
                break; // we are on row headers so ignore

            LPGRIDITEM lpgi = GetCellData(g_lpInst->cursorcol, g_lpInst->cursorrow);
            if(TRUE == (NULL == lpgi ? FALSE : lpgi->fProtected))
                break; // no edit for protected cells

            RECT cellrect = {0,0,0,0};
            GetCellRect(hwnd, g_lpInst->cursorcol, g_lpInst->cursorrow, &cellrect);

            INT iRtn = 0;
            TCHAR buf[5] = {0};
            BYTE kbd[256] = {0};
            GetKeyboardState(kbd);

#ifdef _UNICODE
            iRtn = ToUnicode(vk, flags, kbd, buf, NELEMS(buf), 0);
#else
            iRtn = ToAscii(vk, flags, kbd, (LPWORD)buf, 0);
#endif
            if(iRtn < 0) iRtn = 0;

            // buf probably won't be null terminated so make sure it is.
            if(iRtn < NELEMS(buf)) buf[iRtn] = 0;

            switch(GetColType(g_lpInst->cursorcol))
            {
                case GCT_EDIT://DWM 1.6: Added CONTEXTCODE_ALT check
                {
                    BOOL fEditMode = CONTEXTCODE_ALT & flags;
                    Grid_OnSelectEdit(hwnd, cellrect, //if CONTEXTCODE_ALT then edit mode
                        fEditMode ? lpgi->lpszCurValue : buf, fEditMode);//DWM 1.7: Added fEditMode
                }
                    break;
                case GCT_COMBO:
                {
                    Grid_OnSelectComboBox(hwnd, cellrect, 
                        (LPTSTR) GetColOptional(g_lpInst->cursorcol), 
                        lpgi->lpszCurValue);
                }
                    break;
                case GCT_BUTTON:
                {
                    Grid_OnSelectButton(hwnd, cellrect, 
                        lpgi->lpszCurValue);
                    FORWARD_WM_KEYDOWN(g_lpInst->hwndControl,vk,cRepeat,flags,SNDMSG);

                    //Cell click handled by Grid_OnCommand()
                }
                    break;
                case GCT_CHECK:
                {
                    if(0 == _tcsicmp(lpgi->lpszCurValue, CHECKED))
                        String_Replace(lpgi->lpszCurValue, UNCHECKED);
                    else
                        String_Replace(lpgi->lpszCurValue, CHECKED);

                    NotifyCellClick(hwnd);
                }
                    break;
                case GCT_LINK:
                {
                    if(!IsEmptyString(lpgi->lpszMisc) && (VK_RETURN == vk || VK_SPACE == vk))
                        ShellExecute(NULL, _T("open"), lpgi->lpszMisc, NULL,NULL, SW_SHOW);

                    NotifyCellClick(hwnd);
                }
                    break;
                case GCT_IMAGE:
                    NotifyCellClick(hwnd);
                    break;
            }
        }
    }
}

/// @brief Handles WM_COMMAND message.
///
/// @param hwnd  Handle of grid.
/// @param id The id of the sender.
/// @param hwndCtl The hwnd of the sender.
/// @param codeNotify The notification code sent.
///
/// @returns VOID.
void Grid_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch(id)
    {
        case ID_BUTTON:
        {
            if(BN_CLICKED == codeNotify)//One of the buttons was clicked fire notify
            {
                NotifyCellClick(hwnd);
            }
        }
            break;
        case ID_COMBO:
        {
            if(CBN_KILLFOCUS == codeNotify)//A selection was made update the cell text
            {
                LPGRIDITEM lpi = GetCellData(g_lpInst->cursorcol, g_lpInst->cursorrow);
                if (NULL != lpi)
                {
                    int len = ComboBox_GetTextLength(hwndCtl) + 1;
                    //TCHAR buf[len];
                    TCHAR * buf = new TCHAR(len);
                    ComboBox_GetText(hwndCtl, buf, len);
                    String_Replace(lpi->lpszCurValue, buf);
                    delete buf;
                }
                //adjust the column width if COLAUTOWIDTH==TRUE
                if ((g_lpInst->COLAUTOWIDTH) || 0 == g_lpInst->cursorrow)
                {
                    AdjustParentColWidth(hwnd, g_lpInst->cursorcol, g_lpInst->cursorrow, lpi->lpszCurValue);
                }
                ShowWindow(hwndCtl, SW_HIDE);
                SetFocus(hwnd);
                NotifyEditEnd(hwnd);
            }
        }
            break;
        case ID_EDIT:
        {
            if(EN_EDITENDOK == codeNotify)
            {
                LPGRIDITEM lpi = GetCellData(g_lpInst->cursorcol, g_lpInst->cursorrow);
                if (NULL != lpi)
                {
                    INT len = Edit_GetTextLength(hwndCtl);
                    TCHAR buf[256 + 1];
                    //TCHAR * buf = new TCHAR(len+1);
                    Edit_GetText(hwndCtl, buf, NELEMS(buf));
                    String_Replace(lpi->lpszCurValue, buf);
                    //delete buf;
                }
                ShowWindow(hwndCtl, SW_HIDE);
                //adjust the column width if COLAUTOWIDTH==TRUE
                if ((g_lpInst->COLAUTOWIDTH) || 0 == g_lpInst->cursorrow)
                {
                    AdjustParentColWidth(hwnd, g_lpInst->cursorcol, g_lpInst->cursorrow, lpi->lpszCurValue);
                }
                SetFocus(hwnd);
                NotifyEditEnd(hwnd);
            }
        }
            break;
    }
}

/// @brief Handle WM_KILLFOCUS message.
///
/// @param hwnd Handle of grid.
/// @param hwndNewFocus Handle of the window that recieved focus.
///
/// @returns VOID.
static VOID Grid_OnKillFocus(HWND hwnd, HWND hwndNewFocus)
{
    g_lpInst->GRIDHASFOCUS = FALSE;

    NotifyLostFocus(hwnd, hwndNewFocus);
    RefreshGrid(hwnd);
}

/// @brief Handles WM_LBUTTONDOWN message.
///
/// @param hwnd Handle of grid.
/// @param fDoubleClick TRUE if this is a double click event.
/// @param x The xpos of the mouse.
/// @param y The ypos of the mouse.
/// @param keyFlags Set if certain keys down at time of click.
///
/// @returns VOID.
static VOID Grid_OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
    int r, c;
    BOOL fClickSelect = FALSE;

    //check for column sizing
    if (g_lpInst->cursortype == 2)
    {
        int t, z;
        //start column sizing
        if (!g_lpInst->COLUMNSIZING)
        {
            g_lpInst->REMEMBERINTEGRALROWS = g_lpInst->SHOWINTEGRALROWS;
        }
        g_lpInst->COLUMNSIZING = TRUE;
        g_lpInst->SHOWINTEGRALROWS = FALSE;

        //DWM 1.9: Removed unnecessary call to hide editor
        //DWM 2.0: Added code to remove button during resize
        if (NULL != g_lpInst->hwndControl)
        {
            if(ID_BUTTON == GetWindowLong(g_lpInst->hwndControl, GWLP_ID))
            {
                DestroyWindow(g_lpInst->hwndControl);
                g_lpInst->hwndControl = NULL;
            }
        }

        g_lpInst->columntoresizeinitx = x;
        t = GetColOfMouse(x + 10);
        z = GetColOfMouse(x - 10);
        c = GetColOfMouse(x);
        if (t != c)
        {
            //resizing column c
            g_lpInst->columntoresize = c;
        }
        if (z != c)
        {
            //resizing hidden column to the left of cursor
            if (c == -1)
            {
                c = ColCount();
            }
            else
            {
                c -= 1;
            }
            g_lpInst->columntoresize = c;
        }

        g_lpInst->columntoresizeinitsize = GetColWidth(c);
    }

    SetFocus(hwnd);

    BOOL NCC = FALSE;

    //DWM 1.5: Finish edit
    FORWARD_WM_CHAR(g_lpInst->hwndControl, VK_RETURN, 0, SNDMSG);

    if (GetFocus() == hwnd)
    {
        r = GetRowOfMouse(y);
        c = GetColOfMouse(x);

        if(c == 0 && GSO_ROWHEADER == g_lpInst->selectionMode)
        {
            g_lpInst->HIGHLIGHTFULLROW = TRUE;
            g_lpInst->cursorrow = r;
            RefreshGrid(hwnd);
            return;
        }

        if ((r > 0) && (c > 0))
        {
            if(GSO_ROWHEADER == g_lpInst->selectionMode &&
                g_lpInst->HIGHLIGHTFULLROW)
            {
                //We selected off the row header so turn the hilight off
                g_lpInst->HIGHLIGHTFULLROW = FALSE;
            }
            if (r != g_lpInst->cursorrow)
            {
                g_lpInst->cursorrow = r;
                NCC = TRUE;
            }
            else
            {
                g_lpInst->cursorrow = r;
            }
            if (c != g_lpInst->cursorcol)
            {
                g_lpInst->cursorcol = c;
                NCC = TRUE;
            }
            else
            {
                g_lpInst->cursorcol = c;
            }
        }
        if (NCC)
        {
            NotifySelChange(hwnd);
        }

        SetHomeRow(hwnd, g_lpInst->cursorcol, g_lpInst->cursorrow);
        SetHomeCol(hwnd, g_lpInst->cursorcol, g_lpInst->cursorrow);
        RefreshGrid(hwnd);

        fClickSelect = GCT_BUTTON == GetColType(g_lpInst->cursorcol) ||
                    GCT_COMBO == GetColType(g_lpInst->cursorcol) ||
                    GCT_CHECK == GetColType(g_lpInst->cursorcol);

        // if not resizing or else over a link with hand cursor
        if((fClickSelect && 1 == g_lpInst->cursortype) ||
           ((GCT_LINK == GetColType(g_lpInst->cursorcol) && 3 == g_lpInst->cursortype)))
        {
            //Send keydown to select control
            FORWARD_WM_KEYDOWN(hwnd,VK_SPACE,0,0,SNDMSG);

            //Send click down to control
            FORWARD_WM_LBUTTONDOWN(g_lpInst->hwndControl,fDoubleClick,x,y,keyFlags,SNDMSG);
        }
    }
    else
    {
        SetFocus(hwnd);
    }
}

/// @brief Handles SG_SETCOLWIDTH message.
///
/// @param hwnd The handle of the grid
/// @param wParam The index of the column
/// @param lParam The desired width (in pixels) of the column
///
/// @returns ERROR_SUCCESS otherwise SG_ERROR if desired cell is out of bounds
static LRESULT Grid_OnSetColWidth(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    INT iCol = wParam;
    if (iCol > ColCount())
    {
        SetLastError(ERROR_INVALID_INDEX);
        return SG_ERROR;
    }
    RECT rect = {0,0,0,0};
    ((LPGRIDCOLUMN)Vector_Get(g_lpInst->data, iCol))->iWidth = (INT)lParam;
    GetClientRect(hwnd, &rect);
    InvalidateRect(hwnd, &rect, FALSE);
    SetVisibleColumns(hwnd);
    return ERROR_SUCCESS;
}

/// @brief Handles WM_LBUTTONUP message.
///
/// @param hwnd Handle of grid.
/// @param x The xpos of the mouse.
/// @param y The ypos of the mouse.
/// @param keyFlags Set if certain keys down at time of click.
///
/// @returns VOID.
static VOID Grid_OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags)
{
    if (g_lpInst->COLUMNSIZING)
    {
        g_lpInst->COLUMNSIZING = FALSE;
        SetCursor(LoadCursor(NULL, IDC_ARROW));
        g_lpInst->cursortype = 1;
        g_lpInst->SHOWINTEGRALROWS = g_lpInst->REMEMBERINTEGRALROWS;

        //DWM 1.9: Removed unnecessary call to restore editor
    }
}

/// @brief Handles WM_MOUSEMOVE message.
///
/// @param hwnd Handle of grid.
/// @param x The xpos of the mouse.
/// @param y The ypos of the mouse.
/// @param keyFlags Set if certain keys down at time of move.
///
/// @returns VOID.
static VOID Grid_OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags)
{
    int r, c, t, z;
    r = GetRowOfMouse(y);
    c = GetColOfMouse(x);
    t = GetColOfMouse(x + 10);
    z = GetColOfMouse(x - 10);

    if (g_lpInst->COLUMNSIZING)
    {
        int dx, nx, cr;
        dx = x - g_lpInst->columntoresizeinitx;
        nx = g_lpInst->columntoresizeinitsize + dx;
        if (nx <= 0)
        {
            nx = 0;
        }
        cr = g_lpInst->columntoresize;

        Grid_OnSetColWidth(hwnd, (WPARAM) cr, (LPARAM) nx);
    }
    if ((r == 0) && (c >= -1) && ((t != c) || (z != c)) && (!g_lpInst->COLUMNSIZING))
    {
        if ((g_lpInst->cursortype != 2) && (g_lpInst->ALLOWCOLUMNRESIZING))
        {
            g_lpInst->cursortype = 2;
            SetCursor(LoadCursor(NULL, IDC_SIZEWE));
        }
    }
    else if (GCT_LINK == GetColType(c) && (!g_lpInst->COLUMNSIZING) && 0 < r)
    {
        RECT rcTxt = {0,0,0,0};
        RECT rcHitBox = {0,0,0,0};

        //Get rectangle for text
        LPGRIDITEM lpgi = GetCellData(c, r);

        if(NULL != lpgi)
        {
            HDC hdc = GetDC(hwnd);
            DrawText(hdc, lpgi->lpszCurValue, _tcslen(lpgi->lpszCurValue), &rcTxt, DT_CALCRECT | DT_LEFT);
            ReleaseDC(hwnd, hdc);

            GetCellRect(hwnd, c, r, &rcHitBox);

            //Calculate a hit box for hand cursor
            if(HEIGHT(rcTxt) <= HEIGHT(rcHitBox))
            {
                rcHitBox.top += ((HEIGHT(rcHitBox) / 2) - (HEIGHT(rcTxt) / 2));
                rcHitBox.bottom = rcHitBox.top + HEIGHT(rcTxt);
            }
            if(WIDTH(rcTxt) <= WIDTH(rcHitBox))
            {
                //rcHitBox.left += ((WIDTH(rcHitBox) / 2) - (WIDTH(rcTxt) / 2)); //Centered text
                rcHitBox.right = rcHitBox.left + WIDTH(rcTxt);
            }
            if((rcHitBox.left <= x) && (x <= rcHitBox.right) && 
                (rcHitBox.top <= y) && (y <= rcHitBox.bottom))
            {
                if(3 != g_lpInst->cursortype)
                {
                    g_lpInst->cursortype = 3;
                    SetCursor(LoadCursor(NULL, IDC_HAND));
                }
            }
            else // We wandered out of the box
            {
                if(1 != g_lpInst->cursortype)
                {
                    g_lpInst->cursortype = 1;
                    SetCursor(LoadCursor(NULL, IDC_ARROW));
                }
            }
        }
    }
    else if ((g_lpInst->cursortype != 1) && (!g_lpInst->COLUMNSIZING))
    {
        g_lpInst->cursortype = 1;
        SetCursor(LoadCursor(NULL, IDC_ARROW));
    }
}

/// @brief Handles WM_PAINT message.
///
/// @param hwnd Handle of grid.
///
/// @returns VOID.
static VOID Grid_OnPaint(HWND hwnd)
{
    PAINTSTRUCT ps;
    HDC hdc, hdcOrig = NULL;
    HBITMAP hbmp = NULL;

    CalcVisibleCellBoundaries();

    hdc = BeginPaint(hwnd, &ps);

    if (g_lpInst->DOUBLEBUFFER)
    {
        hdcOrig = hdc;

        hdc = CreateCompatibleDC(hdcOrig);
        if (!hdc)
            return;

        hbmp = CreateCompatibleBitmap(hdcOrig, ps.rcPaint.right, ps.rcPaint.bottom);
        if (!hbmp)
        {
            DeleteDC(hdc);
            return;
        }
    }
    SelectObject(hdc, hbmp);

    //display title
    DisplayTitle(hwnd, g_lpInst->htitlefont, hdc, ps.rcPaint);

    //display Row headers (column 0);
    DisplayColumn(hwnd, 0, 0, g_lpInst->hfont, g_lpInst->hcolumnheadingfont, hdc);
    {
        int c, j, k, offset;
        offset = GetColWidth(0);
        j = g_lpInst->leftvisiblecol;
        k = g_lpInst->rightvisiblecol;
        for (c = j; c <= k; c++)
        {
            DisplayColumn(hwnd, c, offset, g_lpInst->hfont, g_lpInst->hcolumnheadingfont, hdc);
            offset += GetColWidth(c);
        }
    }
    if (hbmp)
    {
        BitBlt(hdcOrig, ps.rcPaint.left, ps.rcPaint.top, WIDTH(ps.rcPaint), 
            HEIGHT(ps.rcPaint), hdc, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);

        DeleteObject(hbmp);
        DeleteDC(hdc);
    }

    EndPaint(hwnd, &ps);
}

/// @brief Handle WM_SETFOCUS message.
///
/// @param hwnd Handle of grid.
/// @param hwndOldFocus Handle if the window that has lost the keyboard focus.
///
/// @returns VOID.
static void Grid_OnSetFocus(HWND hwnd, HWND hwndOldFocus)
{
    g_lpInst->GRIDHASFOCUS = TRUE;
    SetHomeRow(hwnd, g_lpInst->cursorcol, g_lpInst->cursorrow);
    SetHomeCol(hwnd, g_lpInst->cursorcol, g_lpInst->cursorrow);

    NotifyGotFocus(hwnd, hwndOldFocus);
    {
        TEXTMETRIC tm;
        HDC hdc;
        hdc = GetDC(hwnd);
        GetTextMetrics(hdc, &tm);
        ReleaseDC(hwnd, hdc);
        g_lpInst->fontascentheight = (int)tm.tmAscent;
    }
    RefreshGrid(hwnd);
}

/// @brief Handle WM_SETFONT message.
///
/// @param hwnd Handle of grid.
/// @param hfont A handle to the font. If this parameter is NULL,
///               the control uses the default system font to draw text.
/// @param fRedraw If TRUE this control should be redrawn immediately. 
///
/// @returns VOID.
static void Grid_OnSetFont(HWND hwnd, HFONT hfont, BOOL fRedraw)
{
    g_lpInst->hfont = hfont;
    if (!g_lpInst->hcolumnheadingfont)
    {
        g_lpInst->hcolumnheadingfont = hfont;
    }
    if (!g_lpInst->htitlefont)
    {
        g_lpInst->htitlefont = hfont;
    }
    if (fRedraw)
        RefreshGrid(hwnd);
}

/// @brief Handle WM_SETTEXT message.
///
/// @param hwnd Handle of grid.
/// @param lpszText A pointer to a null-terminated string that is the window text.
///
/// @returns VOID.
static VOID Grid_OnSetText(HWND hwnd, LPCTSTR lpszText)
{
    int j, linecount;
    SIZE size = {0,0};
    HDC gdc;
    HFONT holdfont;
    String_Replace(g_lpInst->title, (LPTSTR)lpszText);

    gdc = GetDC(hwnd);
    //get linecount of title;
    if (_tcslen(g_lpInst->title) > 0)
    {
        linecount = 1;
        for (j = 0; j < (int)_tcslen(g_lpInst->title); j++)
        {
            if (g_lpInst->title[j] == _T('\n'))
            {
                linecount++;
            }

        }
        holdfont = (HFONT)SelectObject(gdc, g_lpInst->htitlefont);
        GetTextExtentPoint32(gdc, g_lpInst->title, _tcslen(g_lpInst->title), &size);
        SelectObject(gdc, holdfont);
        g_lpInst->titleheight = (int)((size.cy * 1.2) * linecount);
    }
    else
    {
        //no title
        g_lpInst->titleheight = 0;
    }
    ReleaseDC(hwnd, gdc);
    RefreshGrid(hwnd);
}

/// @brief Handles WM_SIZE message.
///
/// @param hwnd  Handle of grid.
/// @param state Specifies the type of resizing requested.
/// @param cx The width of client area.
/// @param cy The height of client area.
///
/// @returns VOID.
static void Grid_OnSize(HWND hwnd, UINT state, int cx, int cy)
{
    static int cheight;
    static int savewidth, saveheight;
    int intout;

    if (g_lpInst->SIZING)
    {
        g_lpInst->SIZING = FALSE;
        return;
    }
    ShowHscroll(hwnd);
    ShowVscroll(hwnd);

    if ((g_lpInst->SHOWINTEGRALROWS) && (g_lpInst->VSCROLL))
    {
        saveheight = cy;
        savewidth = cx;
        cheight = cy;
        cheight -= g_lpInst->titleheight;
        cheight -= g_lpInst->headerrowheight;

        {
            int sbheight;
            sbheight = GetSystemMetrics(SM_CYHSCROLL);
            if (g_lpInst->HSCROLL)
            {
                cheight -= sbheight;
            }
            if (g_lpInst->VSCROLL)
            {
                RECT grect, prect;
                GetClientRect(hwnd, &grect);
                GetClientRect(GetParent(hwnd), &prect);
                if ((grect.right + sbheight) < prect.right)
                {
                    savewidth += sbheight;
                }
            }

        }
        if (cheight <= g_lpInst->rowheight)
        {
            return;
        }
        else
        {
            //calculate fractional part of cheight/rowheight
            int remainder, nrows;
            nrows = (int)(cheight / g_lpInst->rowheight);
            remainder = cheight - (nrows * g_lpInst->rowheight);
            //make the window remainder pixels shorter
            saveheight -= remainder;
            saveheight += 4;    //+=4
            intout = saveheight;
            WINDOWPLACEMENT wp;
            RECT crect = {0,0,0,0};
            wp.length = sizeof(wp);
            GetWindowPlacement(hwnd, &wp);
            crect = wp.rcNormalPosition;
            crect.bottom = intout;
            crect.right = savewidth;
            g_lpInst->SIZING = TRUE;

            MoveWindow(hwnd, crect.left, crect.top, crect.right, crect.bottom, TRUE);
        }
    }
    // Make sure the extended column gets redrawn
    RECT rect = {0,0,0,0};
    GetClientRect(hwnd, &rect);
    InvalidateRect(hwnd, &rect, FALSE);
}

/// @brief Handles WM_VSCROLL message.
///
/// @param hwnd Handle of grid.
/// @param hwndCtl If this message is sent by a scroll bar control,
///                 this parameter is the handle to the scroll bar control.
///                However in this case the message was sent by a standard
///                 scroll bar, thus this parameter is NULL.
/// @param code The scroll bar value that indicates the user's scrolling request.
/// @param pos The current position of the scroll box if the @b code is
///             SB_THUMBPOSITION or SB_THUMBTRACK
///
/// @returns VOID
static VOID Grid_OnVScroll(HWND hwnd, HWND hwndCtl, UINT code, int pos)
{
    SetFocus(hwnd);
    if (code == SB_THUMBTRACK)
    {
        RECT gridrect = {0,0,0,0};

        int min, max;
        g_lpInst->homerow = pos;
        SetScrollPos(hwnd, SB_VERT, pos, TRUE);
        GetClientRect(hwnd, &gridrect);
        GetScrollRange(hwnd, SB_VERT, &min, &max);
        if (code == max)
        {
            gridrect.top = gridrect.bottom - (g_lpInst->rowheight);
            InvalidateRect(hwnd, &gridrect, TRUE);
        }
        else
        {
            InvalidateRect(hwnd, &gridrect, FALSE);
        }
    }
    if (code == SB_PAGEDOWN)
    {
        RECT gridrect = {0,0,0,0};

        int min, max, sp, rpp;
        //get rows per page
        GetClientRect(hwnd, &gridrect);
        rpp = (gridrect.bottom - (g_lpInst->headerrowheight + g_lpInst->titleheight)) / g_lpInst->rowheight;
        GetScrollRange(hwnd, SB_VERT, &min, &max);
        sp = GetScrollPos(hwnd, SB_VERT);
        sp += rpp;
        if (sp > max)
        {
            sp = max;
        }
        g_lpInst->homerow = sp;
        SetScrollPos(hwnd, SB_VERT, sp, TRUE);
        SetHomeRow(hwnd, g_lpInst->homecol, sp);
        if (sp == max)
        {
            gridrect.top = gridrect.bottom - (g_lpInst->rowheight);
            InvalidateRect(hwnd, &gridrect, TRUE);
        }
        else
        {
            InvalidateRect(hwnd, &gridrect, FALSE);
        }
    }
    if (code == SB_LINEDOWN)
    {
        RECT gridrect = {0,0,0,0};

        int min, max, sp;
        //get rows per page
        GetClientRect(hwnd, &gridrect);
        GetScrollRange(hwnd, SB_VERT, &min, &max);
        sp = GetScrollPos(hwnd, SB_VERT);
        sp += 1;
        if (sp > max)
        {
            sp = max;
        }
        g_lpInst->homerow = sp;
        SetScrollPos(hwnd, SB_VERT, sp, TRUE);
        SetHomeRow(hwnd, g_lpInst->homecol, sp);
        if (sp == max)
        {
            gridrect.top = gridrect.bottom - (g_lpInst->rowheight);
            InvalidateRect(hwnd, &gridrect, TRUE);
        }
        else
        {
            InvalidateRect(hwnd, &gridrect, FALSE);
        }
    }
    if (code == SB_PAGEUP)
    {
        RECT gridrect = {0,0,0,0};

        int min, max, sp, rpp;
        //get rows per page
        GetClientRect(hwnd, &gridrect);
        rpp = (gridrect.bottom - (g_lpInst->headerrowheight + g_lpInst->titleheight)) / g_lpInst->rowheight;
        GetScrollRange(hwnd, SB_VERT, &min, &max);
        sp = GetScrollPos(hwnd, SB_VERT);
        sp -= rpp;
        if (sp < 1)
        {
            sp = 1;
        }
        g_lpInst->homerow = sp;
        SetScrollPos(hwnd, SB_VERT, sp, TRUE);
        SetHomeRow(hwnd, g_lpInst->homecol, sp);
        if (sp == max)
        {
            gridrect.top = gridrect.bottom - (g_lpInst->rowheight);
            InvalidateRect(hwnd, &gridrect, TRUE);
        }
        else
        {
            InvalidateRect(hwnd, &gridrect, FALSE);
        }
    }
    if (code == SB_LINEUP)
    {
        RECT gridrect = {0,0,0,0};

        int min, max, sp;
        //get rows per page
        GetClientRect(hwnd, &gridrect);
        sp = GetScrollPos(hwnd, SB_VERT);
        GetScrollRange(hwnd, SB_VERT, &min, &max);
        sp -= 1;
        if (sp < 1)
        {
            sp = 1;
        }
        g_lpInst->homerow = sp;
        SetScrollPos(hwnd, SB_VERT, sp, TRUE);
        SetHomeRow(hwnd, g_lpInst->homecol, sp);
        if (sp == max)
        {
            gridrect.top = gridrect.bottom - (g_lpInst->rowheight);
            InvalidateRect(hwnd, &gridrect, TRUE);
        }
        else
        {
            InvalidateRect(hwnd, &gridrect, FALSE);
        }
    }
    RefreshGrid(hwnd);
}

/// @brief Handles WM_MOUSEWHEEL message.
///
/// @param hwnd Handle of grid.
/// @param x The xpos of the mouse.
/// @param y The ypos of the mouse.
/// @param zDelta The delta of the mouse wheel (rotation distance)
/// @param keyFlags Set if certain keys down at time of move.
///
/// @returns VOID.
static VOID Grid_OnMouseWheel(HWND hwnd, int x, int y, int zDelta, UINT keyFlags)
{
    UINT code = 0; 
    UINT uScrollLines = 3;
    SystemParametersInfo(SPI_GETWHEELSCROLLLINES,0, &uScrollLines, 0);

    /* if scrolling changes direction, ignore left overs */
    if ((zDelta < 0 && g_lpInst->cWheelRemainder < 0) ||
        (zDelta > 0 && g_lpInst->cWheelRemainder > 0))
        g_lpInst->cWheelRemainder += zDelta;
    else
        g_lpInst->cWheelRemainder = zDelta;
    if (g_lpInst->cWheelRemainder && uScrollLines)
    {
        int cLineScroll;
        cLineScroll = uScrollLines * (float)g_lpInst->cWheelRemainder / WHEEL_DELTA;
        g_lpInst->cWheelRemainder -= WHEEL_DELTA * cLineScroll / (int)uScrollLines;

        if(zDelta < 0)
        {
            code = keyFlags & MK_SHIFT ?  SB_PAGEDOWN : SB_LINEDOWN;
        }
        else
        {
            code = keyFlags & MK_SHIFT ?  SB_PAGEUP : SB_LINEUP;
        }
        Grid_OnVScroll(hwnd, NULL, code, -cLineScroll);      
    }
}

/// @brief Handles SG_ADDCOLUMN message.
///
/// @param hwnd The handle of the grid
/// @param wParam  The type of column to add
/// @param lParam The Column header text string
///
/// @returns The index of the added column if successful, otherwise SG_ERROR 
static LRESULT Grid_OnAddColumn(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    BOOL fSuccess = TRUE;
    LPSGCOLUMN lpColumn;
    LPVECTOR temp;

    if(MAXCOLUMNS <= ColCount())
        fSuccess = FALSE;

    if (fSuccess)
    {
        //Create column
        lpColumn = (LPSGCOLUMN)lParam;
        temp = New_Vector();

        fSuccess = Vector_Add(temp, New_Item(lpColumn->lpszHeader));
        if (fSuccess)
        {
            //Add rows (if any)
            for (int i = RowCount() - 1; 0 < i; --i)
            {   //Don't include col headers
                fSuccess = Vector_Add(temp, New_Item(_T("")));
                if (!fSuccess)
                    break;
            }
        }
        fSuccess = Vector_Add(g_lpInst->data, New_Column(lpColumn, INITIAL_COL_WIDTH, temp));
    }
    if(fSuccess)
    {
        return ColCount() - 1;
    }
    else
    {
        SetLastError(ERROR_OUTOFMEMORY);
        return SG_ERROR;
    }
}

/// @brief Handles SG_ADDROW message.
///
/// @param hwnd The handle of the grid
/// @param wParam  This parameter is not used
/// @param lParam The Column header text string
///
/// @returns The index of the added column if successful, otherwise SG_ERROR 
static LRESULT Grid_OnAddRow(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    BOOL fSuccess = FALSE;
    LPTSTR tempStr;
    LPVECTOR temp;
    LPGRIDITEM lpgi;
    int nCols = ColCount();

    if (0 < nCols)
    {
        // Add Row header
        temp = ((LPGRIDCOLUMN)Vector_Get(g_lpInst->data, 0))->items;
        tempStr = (LPTSTR)lParam;
        lpgi = New_Item(tempStr);
        fSuccess = Vector_Add(temp, lpgi);
        if (fSuccess)   //Add an empty row
        {
            for (int i = 1; i < nCols; ++i)
            {
                temp = ((LPGRIDCOLUMN)Vector_Get(g_lpInst->data, i))->items;
                lpgi = New_Item(_T(""));
                fSuccess = Vector_Add(temp, lpgi);
                if (!fSuccess)
                {
                    break;
                }
            }
        }
    }
    if(fSuccess)
    {
        return RowCount() - 1;
    }
    else
    {
        SetLastError(ERROR_OUTOFMEMORY);
        return SG_ERROR;
    }
}

/// @brief Handles SG_RESETCONTENT message.
///
/// @param hwnd The handle of the grid
///
/// @returns VOID 
static VOID Grid_OnResetContent(HWND hwnd)
{
    //DWM 1.3: Found bug in original and while fixing it
    // Realized that this could be done in a simpler way than
    // initially envisioned so I rewrote method.

    //Destroy previous editor if any
    if (NULL != g_lpInst->hwndControl) //DWM 1.4: Added this
    {
        DestroyWindow(g_lpInst->hwndControl);
        g_lpInst->hwndControl = NULL;
    }

    // Destroy old data table
    int nCols = Vector_Size(g_lpInst->data);
    for (int col = 0; col < nCols; ++col)
    {
        Column_Delete((LPGRIDCOLUMN)Vector_Get(g_lpInst->data, col));
    }
    Vector_Free(g_lpInst->data);

    //Create new data table
    g_lpInst->data = New_Vector();

    //Add Column 0 (Row headers)
    LPVECTOR temp = New_Vector();
    //First element (the column name) Should be blank for row header column
    BOOL fSuccess = Vector_Add(temp, New_Item(_T("")));

    if (fSuccess)//DWM 1.9 Refactored
    {
        SGCOLUMN sgc;
        sgc.dwType = GCT_ROWHEADER;
        sgc.lpszHeader = _T("");
        sgc.pOptional = NULL;
        fSuccess = Vector_Add(g_lpInst->data, New_Column(&sgc, INITIAL_COL_WIDTH, temp));
    }
    //Reset Cursor and Selection to default
    g_lpInst->cursorrow = 1;
    g_lpInst->cursorcol = 1;
    g_lpInst->homerow = 1;
    g_lpInst->homecol = 1;

    RefreshGrid(hwnd);
}

/// @brief Handles SG_REFRESHGRID message.
///
/// @note: This is called external to the grid and differs from
///         RefreshGrid() in that the call to InvalidateRect() is such that
///         that background is erased when the BeginPaint function is called.
///
/// @param hwnd Handle of the grid
///
/// @returns VOID
static VOID Grid_OnRefresh(HWND hwnd)
{
    RECT rect = {0,0,0,0};

    GetClientRect(hwnd, &rect);
    InvalidateRect(hwnd, &rect, TRUE);
    UpdateWindow(hwnd);
}

/// @brief Handles SG_SETCURSORPOS message.
///
/// @param hwnd The handle of the grid
/// @param wParam The col index of the cell
/// @param lParam The row index of the cell
///
/// @returns ERROR_SUCCESS otherwise SG_ERROR if desired cell is out of bounds
static LRESULT Grid_OnSetCursorPos(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    SGITEM SGitem;
    SGitem.col = (int)wParam;
    SGitem.row = (int)lParam;

    if (OutOfRange(&SGitem))
    {
        SetLastError(ERROR_INVALID_INDEX);
        return SG_ERROR;
    }
    else
    {
        g_lpInst->cursorrow = SGitem.row;
        g_lpInst->cursorcol = SGitem.col;
    }

    SetHomeRow(hwnd, g_lpInst->cursorcol, g_lpInst->cursorrow);
    SetHomeCol(hwnd, g_lpInst->cursorcol, g_lpInst->cursorrow);
    RefreshGrid(hwnd);

    return ERROR_SUCCESS;
}

/// @brief Handles SG_SELECTCELL message.
///
/// @param hwnd The handle of the grid
/// @param col The col index of the cell
/// @param row The row index of the cell
/// @param fOverwritemode TRUE to overwrite cell contents, FALSE to edit 
///         the current contents of the cell.
///
/// @returns ERROR_SUCCESS otherwise SG_ERROR if desired cell is out of bounds
static LRESULT Grid_OnSelectCell(HWND hwnd, INT col, INT row, BOOL fOverwritemode)
{
    // don't include row header column and column header row
    LRESULT lResult = Grid_OnSetCursorPos(hwnd, (WPARAM)col, (LPARAM)row);
    if(ERROR_SUCCESS == lResult)
    {
        //Send keydown to select control
        //The context code bit is always 0 for a WM_KEYDOWN message.  
        // We'll abuse it to signal Editmode (1) or Overwritemode (0)
        FORWARD_WM_KEYDOWN(hwnd,VK_SHIFT,0, 
            fOverwritemode ? 0 : CONTEXTCODE_ALT,SNDMSG);
    }
    return lResult;
}

/// @brief Handles SG_SETITEMPROTECTION message.
///
/// @param hwnd The handle of the grid
/// @param wParam BOOL TRUE to protect cell FALSE to allow changes
/// @param lParam A pointer to a SGITEM struct with column and row indexes
///
/// @returns ERROR_SUCCESS otherwise SG_ERROR if desired cell is out of bounds
static LRESULT Grid_OnSetItemProtection(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    LPGRIDITEM lpgi;

    LPSGITEM lpSGitem = (LPSGITEM)lParam;
    if (OutOfRange(lpSGitem))
    {
        SetLastError(ERROR_INVALID_INDEX);
        return SG_ERROR;
    }

    int col = lpSGitem->col;
    int row = lpSGitem->row;

    lpgi = GetCellData(col, row);
    if(NULL != lpgi)
    {
        lpgi->fProtected = (BOOL)wParam;
        return ERROR_SUCCESS;
    }
    else
    {
        SetLastError(ERROR_INVALID_DATA);
        return SG_ERROR;
    }
}

/// @brief Handles SG_SETITEMDATA message.
///
/// @param hwnd The handle of the grid
/// @param wParam This parameter is not used
/// @param lParam A pointer to a SGITEM struct with column and row indexes.
///
/// @returns ERROR_SUCCESS otherwise SG_ERROR if desired cell is out of bounds
static LRESULT Grid_OnSetItemData(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    LPTSTR lpszValue = TEXT("");
    TCHAR buf[5]; //Allow for indexes up to 9999 (more images than an image list likely can hold)
    LPGRIDITEM lpgi;
    LPSGITEM lpSGitem = (LPSGITEM)lParam;
    DWORD dwColType;

    if (OutOfRange(lpSGitem))
    {
        SetLastError(ERROR_INVALID_INDEX);
        return SG_ERROR;
    }

    int col = lpSGitem->col;
    int row = lpSGitem->row;

    lpgi = GetCellData(col, row);
    if(NULL == lpgi)
    {
        SetLastError(ERROR_INVALID_DATA);
        return SG_ERROR;
    }

    dwColType = GetColType(col);
    switch (dwColType)
    {
        case GCT_BUTTON: //Make sure that the button text is the same as the cell if updated
            if(NULL != g_lpInst->hwndControl)
                SetWindowText(g_lpInst->hwndControl, (LPTSTR) lpSGitem->lpCurValue);
            //Fall through
        case GCT_EDIT:
        case GCT_COMBO:
        case GCT_LINK:
            lpszValue = (LPTSTR) lpSGitem->lpCurValue;
            break;
        case GCT_CHECK:
            lpszValue = ((BOOL) lpSGitem->lpCurValue) ? CHECKED : UNCHECKED;
            break;
        case GCT_IMAGE:
            _sntprintf(buf, NELEMS(buf), _T("%d"), lpSGitem->lpCurValue);//DWM 1.9: was _stprintf
            lpszValue = buf;
            //lpszValue = _ltot(lpSGitem->lpCurValue, buf, 10);
            break;
    }

    if(GCT_LINK == dwColType)
    {
        // The Link display text and hyperlink are allocated in a double null terminated string buf
        // and so are adjacent.  The pointer lpgi->lpszCurValue points to the first element
        // and the beginning of the array.
        StringArray_Replace(lpgi->lpszCurValue, lpszValue);

        //Increment to next string to get pointer to the link
        lpgi->lpszMisc = lpgi->lpszCurValue + _tcslen(lpgi->lpszCurValue) + 1;
    }
    else
    {
        String_Replace(lpgi->lpszCurValue, lpszValue);
    }

    RECT rect = {0,0,0,0};

    GetCellRect(hwnd, col, row, &rect);
    InvalidateRect(hwnd, &rect, FALSE);

    //adjust the column width if COLAUTOWIDTH==TRUE
    if ((g_lpInst->COLAUTOWIDTH) || (lpSGitem->row == 0))
    {
        AdjustParentColWidth(hwnd, lpSGitem->col, lpSGitem->row, lpszValue);
    }
    return ERROR_SUCCESS;
}

/// @brief Handles SG_SETITEMTEXTALIGNMENT message.
///
/// @param hwnd The handle of the grid
/// @param wParam The item text alignment
/// @param lParam A pointer to a SGITEM struct with column and row indexes.
///
/// @returns ERROR_SUCCESS otherwise SG_ERROR if desired cell is out of bounds
static LRESULT Grid_OnSetItemTextAlignment(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    LPSGITEM lpsgi = (LPSGITEM) lParam;
    if(NULL != lpsgi)
    {
        if(GCT_EDIT == GetColType(lpsgi->col))
        {
            LPGRIDITEM lpi = GetCellData(lpsgi->col, lpsgi->row);
            if(NULL != lpi)
            {
                lpi->dwAllignment = (DWORD)wParam;
                return ERROR_SUCCESS;
            }
        }
    }
    // If we get here we failed
    SetLastError(ERROR_INVALID_DATA);
    return SG_ERROR;
}

/// @brief Handles SG_GETITEMDATALENGTH message.
///
/// @param hwnd The handle of the grid
/// @param wParam The column that the item belongs to
/// @param lParam The row index of the item in the column
///
/// @returns The length of the data buffer for the item
static LRESULT Grid_OnGetItemDataLength(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    LPGRIDITEM lpgi;
    lpgi = GetCellData((INT) wParam, (INT) lParam);

    if (NULL != lpgi)
    {
        switch(GetColType((INT) wParam))
        {
            case GCT_EDIT:
            case GCT_COMBO:
            case GCT_BUTTON:
                return _tcslen((LPTSTR) lpgi->lpszCurValue);
            case GCT_LINK:
            {
                // Note: The Link text and hyperlink are allocated in a double null terminated string buf
                // and so are adjacent.  The pointer lpgi->lpszCurValue points to the first element
                // and the beginning of the array.

                //Determine total buffer length
                INT iLen = 0;
                //Walk the buffer to find the terminating empty string.
                for (LPTSTR p = lpgi->lpszCurValue; *p; 
                    (p += _tcslen(p) + 1, iLen = p - lpgi->lpszCurValue)) ;

                return iLen;
            }
            case GCT_CHECK:
            case GCT_IMAGE:
                return 0;
        }
    }
    return 0;
}

/// @brief Handles SG_GETITEMDATA message.
///
/// @param hwnd The handle of the grid
/// @param wParam This parameter is not used
/// @param lParam A pointer to a SGITEM struct with column and row indexes.
///
/// @returns ERROR_SUCCESS otherwise SG_ERROR if desired cell is out of bounds
static LRESULT Grid_OnGetItemData(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    LPTSTR end;
    LPGRIDITEM lpgi;
    LPSGITEM lpSGitem = (LPSGITEM)lParam;
    if (OutOfRange(lpSGitem))
    {
        SetLastError(ERROR_INVALID_INDEX);
        return SG_ERROR;
    }

    int col = lpSGitem->col;
    int row = lpSGitem->row;

    lpgi = GetCellData(col, row);
    if(NULL == lpgi)
    {
        SetLastError(ERROR_INVALID_DATA);
        return SG_ERROR;
    }

    switch (GetColType(col))
    {
        case GCT_EDIT:
        case GCT_COMBO:
        case GCT_BUTTON:
        {
            _tcscpy((LPTSTR)lpSGitem->lpCurValue, lpgi->lpszCurValue);
            return _tcslen((LPTSTR)lpSGitem->lpCurValue);
        }
        case GCT_LINK:
        {
            // Note: The Link text and hyperlink are allocated in a double null terminated string buf
            // and so are adjacent.  The pointer lpgi->lpszCurValue points to the first element
            // and the beginning of the array.
            if(NULL == lpgi->lpszCurValue || _T('\0') == *lpgi->lpszCurValue)
                return 0;

            //Determine total buffer length
            INT iLen = 0;

            //Walk the buffer to find the terminating empty string.
            for (LPTSTR p = lpgi->lpszCurValue; *p; 
                (p += _tcslen(p) + 1, iLen = p - lpgi->lpszCurValue));

            _tmemcpy((LPTSTR)lpSGitem->lpCurValue, lpgi->lpszCurValue, iLen + 1); // copy the terminating null also
            return iLen;
        }
        case GCT_CHECK:
            lpSGitem->lpCurValue = (0 == _tcsicmp(lpgi->lpszCurValue, CHECKED));
            break;
        case GCT_IMAGE:
            lpSGitem->lpCurValue = _tcstol(lpgi->lpszCurValue, &end, 10);
            if(0 < _tcslen(end)) // we failed but I don't know how this could have happened
            {
                lpSGitem->lpCurValue = 0;
            }
            break;
    }
    return ERROR_SUCCESS;
}

/// @brief Handles SG_SETHEADERROWHEIGHT message.
///
/// @param hwnd The handle of the grid
/// @param wParam The desired height (in pixels) of the header row
/// @param lParam This parameter is not used
///
/// @returns VOID
static void Grid_OnSetHeaderRowHeight(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    if ((int)wParam >= 0)
    {
        RECT rect = {0,0,0,0};

        g_lpInst->headerrowheight = wParam;
        GetClientRect(hwnd, &rect);
        InvalidateRect(hwnd, &rect, FALSE);
    }
}

/// @brief Handles SG_GETITEMPROTECTION message.
///
/// @param hwnd The handle of the grid
/// @param wParam A pointer to a SGCELL struct with column and row indexes
/// @param lParam This parameter is not used
///
/// @returns SG_ERROR if desired cell is out of bounds, TRUE if it is protected, otherwise FALSE
static LRESULT Grid_OnGetItemProtection(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    LPGRIDITEM lpgi;
    LPSGITEM LPSGitem = (LPSGITEM)wParam;

    if (OutOfRange(LPSGitem))
    {
        SetLastError(ERROR_INVALID_INDEX);
        return SG_ERROR;
    }

    lpgi = GetCellData(LPSGitem->col, LPSGitem->row);
    if(NULL == lpgi)
    {
        SetLastError(ERROR_INVALID_DATA);
        return SG_ERROR;
    } 
    return lpgi->fProtected;
}

/// @brief Handles SG_SETROWHEIGHT message.
///
/// @param hwnd The handle of the grid
/// @param wParam The desired height (in pixels) of the rows
///
/// @returns VOID
static void Grid_OnSetRowHeight(HWND hwnd, WPARAM wParam)
{
    if (wParam < 1)
    {
        wParam = 1;
    }
    g_lpInst->rowheight = wParam;
    SetHomeRow(hwnd, g_lpInst->cursorcol, g_lpInst->cursorrow);
    SetHomeCol(hwnd, g_lpInst->cursorcol, g_lpInst->cursorrow);

    RECT rect = {0,0,0,0};

    GetClientRect(hwnd, &rect);
    InvalidateRect(hwnd, &rect, FALSE);
}

/// @brief Handles SG_SETTITLEHEIGHT message.
///
/// @param hwnd The handle of the grid
/// @param wParam The desired height (in pixels) of the title
/// @param lParam This parameter is not used
///
/// @returns VOID
static void Grid_OnSetTitleHeight(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
    if ((int)wParam < 0)
    {
        wParam = 0;
    }
    g_lpInst->titleheight = wParam;
    SetHomeRow(hwnd, g_lpInst->cursorcol, g_lpInst->cursorrow);
    SetHomeCol(hwnd, g_lpInst->cursorcol, g_lpInst->cursorrow);
    RECT rect = {0,0,0,0};

    GetClientRect(hwnd, &rect);
    InvalidateRect(hwnd, &rect, FALSE);
}

/// @brief Handles SG_SETIMAGELIST message.
///
/// @param wParam The id number of a column of type GCT_IMAGE
/// @param lParam The handle of the new image list
///
/// @returns LRESULT the handle of the old image list if any
static LRESULT Grid_OnSetImageList(WPARAM wParam, LPARAM lParam)
{
    HIMAGELIST himlOld = 0;
    INT col = (INT)wParam;

    if(GCT_IMAGE != GetColType(col))
        return (LRESULT)NULL;

    himlOld = (HIMAGELIST)GetColOptional(col);
    SetColOptional(col, (HIMAGELIST) lParam);
    return (LRESULT)himlOld;
}

/// @brief Handles SG_SETIMAGELIST message.
///
/// @param wParam The id number of a column of type GCT_IMAGE
/// @param lParam This parameter is not used
///
/// @returns LRESULT the handle of the current image list if any
static LRESULT Grid_OnGetImageList(WPARAM wParam, LPARAM lParam)
{
    HIMAGELIST himl = 0;
    INT col = (INT)wParam + 1;

    if(GCT_IMAGE != GetColType(col))
        return (LRESULT)NULL;

    himl = (HIMAGELIST)GetColOptional(col);
    return (LRESULT)himl;
}

#pragma endregion event handlers

/// @brief Window procedure and public interface for the property grid.
///
/// @param hwnd Handle of grid.
/// @param msg Which message?
/// @param wParam Message parameter.
/// @param lParam Message parameter.
///
/// @returns LRESULT depends on message.
static LRESULT CALLBACK Grid_Proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    RECT rect = {0,0,0,0};

    Control_GetInstanceData(hwnd, &g_lpInst);   //Update the instance pointer

    //update the grid width and height variable
    if (NULL != g_lpInst)
    {
        GetClientRect(hwnd, &rect);
        g_lpInst->gridwidth = rect.right - rect.left;
        g_lpInst->gridheight = rect.bottom - rect.top;
    }
    switch (msg)
    {
        HANDLE_MSG(hwnd, WM_CREATE, Grid_OnCreate);
        HANDLE_MSG(hwnd, WM_COMMAND, Grid_OnCommand);
        HANDLE_MSG(hwnd, WM_DESTROY, Grid_OnDestroy);
        HANDLE_MSG(hwnd, WM_HSCROLL, Grid_OnHScroll);
        HANDLE_MSG(hwnd, WM_KEYDOWN, Grid_OnKeyDown);
        HANDLE_MSG(hwnd, WM_KILLFOCUS, Grid_OnKillFocus);
        HANDLE_MSG(hwnd, WM_LBUTTONDOWN, Grid_OnLButtonDown);
        HANDLE_MSG(hwnd, WM_LBUTTONUP, Grid_OnLButtonUp);
        HANDLE_MSG(hwnd, WM_MOUSEMOVE, Grid_OnMouseMove);
        HANDLE_MSG(hwnd, WM_MOUSEWHEEL, Grid_OnMouseWheel);
        HANDLE_MSG(hwnd, WM_PAINT, Grid_OnPaint);
        HANDLE_MSG(hwnd, WM_SETFOCUS, Grid_OnSetFocus);
        HANDLE_MSG(hwnd, WM_SETFONT, Grid_OnSetFont);
        HANDLE_MSG(hwnd, WM_SETTEXT, Grid_OnSetText);
        HANDLE_MSG(hwnd, WM_SIZE, Grid_OnSize);
        HANDLE_MSG(hwnd, WM_VSCROLL, Grid_OnVScroll);
        case WM_GETDLGCODE: // If this control is in a dialog
            return DLGC_WANTALLKEYS; // Send all key presses to this proc

        //
        //Begin Grid specific messages
        //
        case SG_ADDCOLUMN:
            return Grid_OnAddColumn(hwnd, wParam, lParam) - 1; // don't include row header column
        case SG_ADDROW:
            return Grid_OnAddRow(hwnd, wParam, lParam) - 1; //don't include the column header row
        case SG_ENABLEEDIT:
            g_lpInst->EDITABLE = (BOOL)wParam;
            break;
        case SG_EXTENDLASTCOLUMN:
            g_lpInst->EXTENDLASTCOLUMN = (BOOL)wParam;
            RefreshGrid(hwnd);
            break;
        case SG_GETCOLCOUNT:
            return ColCount() - 1; // don't include row header column
        case SG_GETCOLUMNHEADERTEXT:
        {
            DWORD dwRtn = 0;
            SGITEM sgi;
            sgi.row = 0;
            sgi.col = wParam + 1; //Skip the row header column
            dwRtn = Grid_OnGetItemData(hwnd, 0, (LPARAM) & sgi);
            if(ERROR_SUCCESS == dwRtn)
            {
                _tcscpy((LPTSTR)lParam, (LPTSTR)sgi.lpCurValue);
                return _tcslen((LPTSTR)lParam);
            }
            return dwRtn;
        }
        case SG_GETCOLUMNHEADERTEXTLENGTH:
        {
            LPGRIDITEM lpgi; //Skip the row header column
            lpgi = GetCellData((INT) wParam + 1, 0);

            if(NULL != lpgi)
            {
                return _tcslen(lpgi->lpszCurValue);
            }
            // If we got here we failed
            SetLastError(ERROR_INVALID_DATA);
            return SG_ERROR;
        }
        case SG_GETCOLUMNTYPE:
            return GetColType(wParam + 1); //Skip the row header column
        case SG_GETCOLWIDTH:
        {
            INT iCol = wParam + 1; //Skip the row header column
            if (iCol > ColCount())
            {
                SetLastError(ERROR_INVALID_INDEX);
                return SG_ERROR;
            }
            return GetColWidth(iCol);
        }
        case SG_GETCURSORCOL:
            return g_lpInst->cursorcol - 1; // don't include row header column
        case SG_GETCURSORROW:
            return g_lpInst->cursorrow - 1; // don't include row header column
        case SG_GETHEADERROWHEIGHT:
            return g_lpInst->headerrowheight;
        case SG_GETIMAGELIST:
            return Grid_OnGetImageList(wParam, lParam);
        case SG_GETITEMDATA:
        {
            DWORD dwRtn = 0;
            ((LPSGITEM)lParam)->col++; // don't include row header column
            ((LPSGITEM)lParam)->row++; // don't include column header row
            dwRtn = Grid_OnGetItemData(hwnd, wParam, lParam);
            ((LPSGITEM)lParam)->col--; // restore initial index
            ((LPSGITEM)lParam)->row--; // restore initial index
            return dwRtn;
        }
        case SG_GETITEMDATALENGTH:
            return Grid_OnGetItemDataLength(hwnd, wParam + 1, lParam + 1);
        case SG_GETITEMPROTECTION:
            return Grid_OnGetItemProtection(hwnd, wParam + 1, lParam + 1);
        case SG_GETROWCOUNT:    // but don't include col headers
            return RowCount() - 1;
        case SG_GETROWHEADERTEXT:
        {
            DWORD dwRtn = 0;
            SGITEM sgi;
            sgi.row = wParam + 1; //Skip the column header row
            sgi.col = 0;
            dwRtn = Grid_OnGetItemData(hwnd, 0, (LPARAM) & sgi);
            if(ERROR_SUCCESS == dwRtn)
            {
                _tcscpy((LPTSTR)lParam, (LPTSTR)sgi.lpCurValue);
                return _tcslen((LPTSTR)lParam);
            }
            return dwRtn;
        }
        case SG_GETROWHEADERTEXTLENGTH:
        {
            LPGRIDITEM lpgi; //Skip the column header row
            lpgi = GetCellData(0, (INT) wParam + 1);

            if(NULL != lpgi)
            {
                return _tcslen(lpgi->lpszCurValue);
            }
            // If we got here we failed
            SetLastError(ERROR_INVALID_DATA);
            return SG_ERROR;
        }
        case SG_GETROWHEIGHT:
            return g_lpInst->rowheight;
        case SG_REFRESHGRID:
            Grid_OnRefresh(hwnd);
            break;
        case SG_RESETCONTENT:
            Grid_OnResetContent(hwnd);
            break;
        case SG_SELECTCELL:
            return Grid_OnSelectCell(hwnd, 
                ((LPSGITEM)lParam)->col + 1, // don't include row header column
                ((LPSGITEM)lParam)->row + 1, // don't include column header row
                (BOOL)wParam);
            break;
        case SG_SETALLOWCOLRESIZE:
            g_lpInst->ALLOWCOLUMNRESIZING = (BOOL)wParam;
            break;
        case SG_SETCOLAUTOWIDTH:
            g_lpInst->COLAUTOWIDTH = (BOOL)wParam;
            break;
        case SG_SETCOLSNUMBERED:
        {
            g_lpInst->COLUMNSNUMBERED = (BOOL)wParam;

            //adjust the column width if necessary
            LPGRIDITEM lpi = GetCellData(g_lpInst->cursorcol, 0);
            if(NULL != lpi)
            {
                AdjustParentColWidth(hwnd, g_lpInst->cursorcol, 
                    0, (LPTSTR) lpi->lpszCurValue);
            }
            RefreshGrid(hwnd);
        }
            break;
        case SG_SETCOLUMNHEADERTEXT:
        {
            SGITEM sgi;
            sgi.row = 0;
            sgi.col = wParam + 1; //Skip the row header column
            sgi.lpCurValue = lParam;
            return Grid_OnSetItemData(hwnd, 0, (LPARAM) & sgi);
        }
        case SG_SETCOLWIDTH:
            //Skip the row header column
            return Grid_OnSetColWidth(hwnd, wParam + 1, lParam);
        case SG_SETCURSORPOS:
            // don't include row header column and column header row
            return Grid_OnSetCursorPos(hwnd, (WPARAM)(wParam + 1), (LPARAM)(lParam + 1));
        case SG_SETDOUBLEBUFFER:
            g_lpInst->DOUBLEBUFFER = (BOOL)wParam;
            break;
        case SG_SETELLIPSIS:
            g_lpInst->ELLIPSIS = (BOOL)wParam;
            RefreshGrid(hwnd);
            break;
        case SG_SETGRIDLINECOLOR:
            g_lpInst->clrGridline = (COLORREF)wParam;
            RefreshGrid(hwnd);
            break;
        case SG_SETHEADERROWHEIGHT:
            Grid_OnSetHeaderRowHeight(hwnd, wParam, lParam);
            break;
        case SG_SETHEADINGFONT:
            g_lpInst->hcolumnheadingfont = (HFONT)wParam;
            RefreshGrid(hwnd);
            break;
        case SG_SETHILIGHTCOLOR:
            g_lpInst->clrHighlight = (COLORREF)wParam;
            RefreshGrid(hwnd);
            break;
        case SG_SETHILIGHTTEXTCOLOR:
            g_lpInst->clrHighlightText = (COLORREF)wParam;
            RefreshGrid(hwnd);
            break;
        case SG_SETIMAGELIST:
            // don't include row header column
            return Grid_OnSetImageList((WPARAM)(wParam + 1), lParam);
        case SG_SETITEMDATA:
        {
            DWORD dwRtn = 0;
            ((LPSGITEM)lParam)->col++; // don't include row header column
            ((LPSGITEM)lParam)->row++; // don't include column header row
            dwRtn = Grid_OnSetItemData(hwnd, wParam, lParam);
            ((LPSGITEM)lParam)->col--; // restore initial index
            ((LPSGITEM)lParam)->row--; // restore initial index
            return dwRtn;
        }
        case SG_SETITEMTEXTALIGNMENT:
        {
            INT iRtn = 0;
            ((LPSGITEM)lParam)->col++; // don't include row header column
            ((LPSGITEM)lParam)->row++; // don't include column header row
            iRtn = Grid_OnSetItemTextAlignment(hwnd, wParam, lParam);
            ((LPSGITEM)lParam)->col--; // restore initial index
            ((LPSGITEM)lParam)->row--; // restore initial index
            return iRtn;
        }
        case SG_SETITEMPROTECTION:
        {
            DWORD dwRtn = 0;
            ((LPSGITEM)lParam)->col++; // don't include row header column
            ((LPSGITEM)lParam)->row++; // don't include column header row
            dwRtn = Grid_OnSetItemProtection(hwnd, wParam, lParam);
            ((LPSGITEM)lParam)->col--; // restore initial index
            ((LPSGITEM)lParam)->row--; // restore initial index
            return dwRtn;
        }
        case SG_SETPROTECTCOLOR:
            g_lpInst->clrProtect = (COLORREF)wParam;
            RefreshGrid(hwnd);
            break;
        case SG_SETROWHEADERTEXT:
        {
            SGITEM sgi;
            sgi.row = wParam + 1; //Skip the column header row
            sgi.col = 0;
            sgi.lpCurValue = lParam;
            return Grid_OnSetItemData(hwnd, 0, (LPARAM) & sgi);
        }
        case SG_SETROWHEADERWIDTH:
        {
            DWORD dwRtn = 0;
            dwRtn = Grid_OnSetColWidth(hwnd, 0, lParam);
            return dwRtn;
        }
        case SG_SETROWHEIGHT:
            Grid_OnSetRowHeight(hwnd, wParam);
            break;
        case SG_SETROWSNUMBERED:
        {
            g_lpInst->ROWSNUMBERED = (BOOL)wParam;

            //adjust the column width if necessary
            LPGRIDITEM lpi = GetCellData(0, g_lpInst->cursorrow);
            if(NULL != lpi)
            {
                AdjustParentColWidth(hwnd, 0, 
                    g_lpInst->cursorrow, (LPTSTR) lpi->lpszCurValue);
            }
            RefreshGrid(hwnd);
        }
            break;
        case SG_SETSELECTIONMODE:
            g_lpInst->selectionMode = (DWORD) wParam;
            g_lpInst->HIGHLIGHTFULLROW = GSO_FULLROW == g_lpInst->selectionMode;
            RefreshGrid(hwnd);
            break;
        case SG_SETTITLEFONT:
            g_lpInst->htitlefont = (HFONT)wParam;
            RefreshGrid(hwnd);
            break;
        case SG_SETTITLEHEIGHT:
            Grid_OnSetTitleHeight(hwnd, wParam, lParam);
            break;

        case SG_SHOWINTEGRALROWS:
            g_lpInst->SHOWINTEGRALROWS = (BOOL)wParam;
            RefreshGrid(hwnd);
            break;
        }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}
