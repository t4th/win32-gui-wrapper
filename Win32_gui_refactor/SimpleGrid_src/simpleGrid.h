//////////////////////////////////////////////////////////////////////////////
///
/// @file simpleGrid.h
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
///
/// @par Disclamer:
///         This source is distributed in the hope that it will be useful,
///         but WITHOUT ANY WARRANTY; without even the implied warranty of
///         MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
/// 
/// @date 04-07-14
/// 
/// @todo 
///
/// @bug 
///
//////////////////////////////////////////////////////////////////////////////

//DWM 1.5: (1) Added message SG_SELECTCELL and macro def. SimpleGrid_SelectCell()
//         (2) Renumbered messages
//         (3) Redefined message SG_SETCURSORPOS and macro def. SimpleGrid_SetCursorPos()
//              so that the column argument is passed as the WPARAM and row argument 
//              as LPARAM in order that they might be consistent with the rest 
//              of the messages.
//DWM 1.6: Added fOverwrite option to SimpleGrid_SelectCell().

#ifndef SIMPLEGRID_H 
#define SIMPLEGRID_H

#define WC_SIMPLEGRIDA  "SimpleGridCtl"
#define WC_SIMPLEGRIDW  L"SimpleGridCtl"

#ifdef UNICODE
#define WC_SIMPLEGRID  WC_SIMPLEGRIDW
#else
#define WC_SIMPLEGRID  WC_SIMPLEGRIDA
#endif

/****************************************************************************/
// Public errors

#define SG_ERROR -1

/****************************************************************************/
// Public structures

/// @var SGCOLUMN
/// @brief An column object

/// @var LPSGCOLUMN
/// @brief Pointer to a column

/// @struct tagSGCOLUMN
/// @brief This is the data associated with a grid column
typedef struct tagSGCOLUMN {
    DWORD dwType;           ///< Column type
    LPTSTR lpszHeader;      ///< Column header text
    LPVOID pOptional;       ///< Optional data (ex: combobox choices)
} SGCOLUMN, *LPSGCOLUMN;

/// @var SGITEM
/// @brief A grid item structure

/// @var LPSGITEM
/// @brief Pointer to a grid item structure

/// @struct tagSGITEM
/// @brief This is the address data associated with a grid cell
typedef struct tagSGITEM{
    int col;                 ///< Column number
    int row;                 ///< Row number
    LPARAM lpCurValue;       ///< Item (cell) value
} SGITEM, *LPSGITEM;

/// @var NMGRID
/// @brief Contains information used in processing simple grid notifications
///         with the exception of SGN_KEYDOWN.

/// @var LPNMGRID
/// @brief Pointer to simple grid notification message data

/// @struct tagNMGRID
/// @brief This is the data associated with a simple grid notification
typedef struct tagNMGRID {
    NMHDR hdr;               ///< Notification message header
    int col;                 ///< Column number
    int row;                 ///< Row number
    DWORD dwType;            ///< Column type (thus cell type) identifier
} NMGRID, *LPNMGRID;

/// @var NMSGKEYDOWN
/// @brief Contains information used in processing the SGN_KEYDOWN notification.

/// @var LPNMSGKEYDOWN
/// @brief Pointer to SGN_KEYDOWN message data

/// @struct tagNMSGKEYDOWN
/// @brief This is the data associated with the SGN_KEYDOWN notification
typedef struct tagNMSGKEYDOWN {
    NMHDR hdr;               ///< Notification message header
    int col;                 ///< Column number
    int row;                 ///< Row number
    DWORD dwType;            ///< Column type (thus cell type) identifier
    WORD wVKey;              ///< Virtual key code
} NMSGKEYDOWN, *LPNMSGKEYDOWN;

/// @var NMSGFOCUS
/// @brief Contains information used in processing the SGN_GOTFOCUS 
///         and SGN_LOSTFOCUS notification.

/// @var LPNMSGFOCUS
/// @brief Pointer to SGN_GOTFOCUS and SGN_LOSTFOCUS message data

/// @struct tagNMSGFOCUS
/// @brief This is the data associated with the SGN_GOTFOCUS and SGN_LOSTFOCUS notification
typedef struct tagNMSGFOCUS {
    NMHDR hdr;               ///< Notification message header
    int col;                 ///< Column number
    int row;                 ///< Row number
    DWORD dwType;            ///< Column type (thus cell type) identifier
    HWND hFocus;             ///< Handle of window receiving or loosing focus 
} NMSGFOCUS, *LPNMSGFOCUS;

/****************************************************************************/
/// @name Grid Column types.
/// @{

#define GCT_EDIT        1   ///< Grid column type: Edit
#define GCT_COMBO       2   ///< Grid column type: Combobox
#define GCT_BUTTON      3   ///< Grid column type: Button
#define GCT_CHECK       4   ///< Grid column type: Checkbox
#define GCT_LINK        5   ///< Grid column type: Hyperlink
#define GCT_IMAGE       6   ///< Grid column type: Image

/// @}

/****************************************************************************/
/// @name Grid Selection mode options.
/// @{

#define GSO_ROWHEADER   1   ///< Grid selection option: Row Header click
#define GSO_CELL        2   ///< Grid selection option: Single cell
#define GSO_FULLROW     3   ///< Grid selection option: Full row select

/// @}

/****************************************************************************/
/// @name Grid Text alignments.
/// @{

#define GSA_LEFT        1   ///< Item Text alignment: Left justified
#define GSA_GENERAL     2   ///< Item Text alignment: Auto selected
#define GSA_RIGHT       3   ///< Item Text alignment: Right justified

/// @}

/****************************************************************************/
/// @name Grid notifications.
/// @{

/// @par Notification 
///        @b SGN_SELCHANGE code
///
/// @par
///        Notifies a simple grid control's parent window that the selection of 
///         an item has changed.  This notification code is sent in the form of 
///         a @b WM_NOTIFY message.
///
/// @par lParam 
///       The pointer an @ref NMGRID struct containing notification data.
///
/// @returns No return value. 
#define SGN_SELCHANGE   0x0003

/// @par Notification 
///        @b SGN_EDITBEGIN code
///
/// @par
///        Notifies a simple grid control's parent window that an edit operation 
///         was initiated in an editable item.  This notification code is sent in 
///         the form of a @b WM_NOTIFY message.
///
/// @par lParam 
///       The pointer an @ref NMGRID struct containing notification data.
///
/// @returns No return value. 
#define SGN_EDITBEGIN   0x0004

/// @par Notification 
///        @b SGN_EDITEND code
///
/// @par
///        Notifies a simple grid control's parent window that an edit operation 
///         was compleated in an editable item.  This notification code is sent in 
///         the form of a @b WM_NOTIFY message.
///
/// @par lParam 
///       The pointer an @ref NMGRID struct containing notification data.
///
/// @returns No return value. 
#define SGN_EDITEND     0x0005

/// @par Notification 
///        @b SGN_KEYDOWN code
///
/// @par
///        Notifies a simple grid control's parent window that a key has been pressed.
///         This notification code is sent in the form of a @b WM_NOTIFY message.
///
/// @par lParam 
///       The pointer an @ref NMSGKEYDOWN struct containing notification data.
///
/// @returns No return value. 
#define SGN_KEYDOWN     0x0006

/// @par Notification 
///        @b SGN_GOTFOCUS code
///
/// @par
///        Notifies a simple grid control's parent window that the grid now 
///         has keyboard and mouse focus.  This notification code is sent in 
///         the form of a @b WM_NOTIFY message.
///
/// @par lParam 
///       The pointer an @ref NMSGFOCUS struct containing notification data.
///
/// @returns No return value.
#define SGN_GOTFOCUS    0x0012

/// @par Notification 
///        @b SGN_LOSTFOCUS code
///
/// @par
///        Notifies a simple grid control's parent window that the grid no longer 
///         has keyboard and mouse focus.  This notification code is sent in 
///         the form of a @b WM_NOTIFY message.
///
/// @par lParam 
///       The pointer an @ref NMSGFOCUS struct containing notification data.
///
/// @returns No return value.
#define SGN_LOSTFOCUS   0x0013

/// @par Notification 
///        @b SGN_ITEMCLICK code
///
/// @par
///        Notifies a simple grid control's parent window that an item in the 
///         received a mouse click.  This notification code is sent in 
///         the form of a @b WM_NOTIFY message.
///
/// @par lParam 
///       The pointer an @ref NMGRID struct containing notification data.
///
/// @returns No return value. 
#define SGN_ITEMCLICK   0x0015

/// @}

/****************************************************************************/
/// @name Simple Grid messages.
/// @{

#define SG_ADDCOLUMN WM_USER + 0x01             ///<SimpleGrid_AddColumn()
#define SG_ADDROW WM_USER + 0x02                ///<SimpleGrid_AddRow()
#define SG_ENABLEEDIT WM_USER + 0x03            ///<SimpleGrid_EnableEdit()
#define SG_EXTENDLASTCOLUMN WM_USER + 0x04      ///<SimpleGrid_ExtendLastColumn()
#define SG_GETCOLCOUNT WM_USER + 0x05           ///<SimpleGrid_GetColCount()
#define SG_GETCOLUMNHEADERTEXT WM_USER + 0x06   ///<SimpleGrid_GetColumnHeaderText()
#define SG_GETCOLUMNHEADERTEXTLENGTH WM_USER + 0x07 ///<SimpleGrid_GetColumnHeaderTextLen()
#define SG_GETCOLUMNTYPE WM_USER + 0x08         ///<SimpleGrid_GetColumnType()
#define SG_GETCOLWIDTH WM_USER + 0x09           ///<SimpleGrid_GetColWidth()
#define SG_GETCURSORCOL WM_USER + 0x0A          ///<SimpleGrid_GetCursorCol()
#define SG_GETCURSORROW WM_USER + 0x0B          ///<SimpleGrid_GetCursorRow()
#define SG_GETHEADERROWHEIGHT WM_USER + 0x0C    ///<SimpleGrid_GetHeaderRowHeight()
#define SG_GETIMAGELIST WM_USER + 0x0D          ///<SimpleGrid_GetImageList()
#define SG_GETITEMDATA WM_USER + 0x0E           ///<SimpleGrid_GetItemData()
#define SG_GETITEMDATALENGTH WM_USER + 0x0F     ///<SimpleGrid_GetItemDataLen()
#define SG_GETITEMPROTECTION WM_USER + 0x10     ///<SimpleGrid_GetItemProtection()
#define SG_GETROWCOUNT WM_USER + 0x11           ///<SimpleGrid_GetRowCount()
#define SG_GETROWHEADERTEXT WM_USER + 0x12      ///<SimpleGrid_GetRowHeaderText()
#define SG_GETROWHEADERTEXTLENGTH WM_USER + 0x13///<SimpleGrid_GetRowHeaderTextLen()
#define SG_GETROWHEIGHT WM_USER + 0x14          ///<SimpleGrid_GetRowHeight()
#define SG_REFRESHGRID WM_USER + 0x15           ///<SimpleGrid_RefreshGrid()
#define SG_RESETCONTENT WM_USER + 0x16          ///<SimpleGrid_ResetContent()
#define SG_SELECTCELL WM_USER + 0x17            ///<SimpleGrid_SelectCell()
#define SG_SETALLOWCOLRESIZE WM_USER + 0x18     ///<SimpleGrid_SetAllowColResize()
#define SG_SETCOLAUTOWIDTH WM_USER + 0x19       ///<SimpleGrid_SetColAutoWidth()
#define SG_SETCOLSNUMBERED WM_USER + 0x1A       ///<SimpleGrid_SetColsNumbered()
#define SG_SETCOLUMNHEADERTEXT WM_USER + 0x1B   ///<SimpleGrid_SetColumnHeaderText()
#define SG_SETCOLWIDTH WM_USER + 0x1C           ///<SimpleGrid_SetColWidth()
#define SG_SETCURSORPOS WM_USER + 0x1D          ///<SimpleGrid_SetCursorPos()
#define SG_SETDOUBLEBUFFER WM_USER + 0x1E       ///<SimpleGrid_SetDoubleBuffer()
#define SG_SETELLIPSIS WM_USER + 0x1F           ///<SimpleGrid_SetEllipsis()
#define SG_SETGRIDLINECOLOR WM_USER + 0x20      ///<SimpleGrid_SetGridLineColor()
#define SG_SETHEADERROWHEIGHT WM_USER + 0x21    ///<SimpleGrid_SetHeaderRowHeight()
#define SG_SETHEADINGFONT WM_USER + 0x22        ///<SimpleGrid_SetHeadingFont()
#define SG_SETHILIGHTCOLOR WM_USER + 0x23       ///<SimpleGrid_SetHilightColor()
#define SG_SETHILIGHTTEXTCOLOR WM_USER + 0x24   ///<SimpleGrid_SetHilightTextColor()
#define SG_SETIMAGELIST WM_USER + 0x25          ///<SimpleGrid_SetImageList()
#define SG_SETITEMDATA WM_USER + 0x26           ///<SimpleGrid_SetItemData()
#define SG_SETITEMTEXTALIGNMENT  WM_USER + 0x27 ///<SimpleGrid_SetItemTextAlignment()
#define SG_SETITEMPROTECTION WM_USER + 0x28     ///<SimpleGrid_SetItemProtection()
#define SG_SETPROTECTCOLOR WM_USER + 0x29       ///<SimpleGrid_SetProtectColor()
#define SG_SETROWHEADERTEXT WM_USER + 0x2A      ///<SimpleGrid_SetRowHeaderText()
#define SG_SETROWHEADERWIDTH WM_USER + 0x2B     ///<SimpleGrid_SetRowHeaderWidth()
#define SG_SETROWHEIGHT WM_USER + 0x2C          ///<SimpleGrid_SetRowHeight()
#define SG_SETROWSNUMBERED WM_USER + 0x2D       ///<SimpleGrid_SetRowsNumbered()
#define SG_SETSELECTIONMODE WM_USER + 0x2E      ///<SimpleGrid_SetSelectionMode()
#define SG_SETTITLEFONT WM_USER + 0x2F          ///<SimpleGrid_SetTitleFont()
#define SG_SETTITLEHEIGHT WM_USER + 0x30        ///<SimpleGrid_SetTitleHeight()
#define SG_SHOWINTEGRALROWS WM_USER + 0x31      ///<SimpleGrid_ShowIntegralRows()
#define SG_RIGHTJUSTIFYNUMERICS  WM_USER + 0x32 ///<SimpleGrid_RightJustifyNumerics()

/// @}

/****************************************************************************/
/// @name Macroes
/// @{

/// @def SimpleGrid_AddColumn(hGrid,lpszHeader)
///
/// @brief Add a column to the grid.
///
/// @param hGrid The handle of the grid.
/// @param lpColumn Pointer to an SGCOLUMN object.
///
/// @returns The index of the added column if successful, otherwise SG_ERROR 
#define SimpleGrid_AddColumn(hGrid, lpColumn) ((int)(DWORD)SNDMSG((hGrid),SG_ADDCOLUMN, 0,(LPARAM)(LPSGCOLUMN)(lpColumn)))

/// @def SimpleGrid_AddRow(hGrid,lpszHeader)
///
/// @brief Add a row to the grid.
///
/// @param hGrid The handle of the grid.
/// @param lpszHeader Row header text string. 
///
/// @returns The index of the added row if successful, otherwise SG_ERROR  
#define SimpleGrid_AddRow(hGrid,lpszHeader) ((int)(DWORD)SNDMSG((hGrid),SG_ADDROW,0,(LPARAM)(lpszHeader)))

/// @def SimpleGrid_AddRowData(hGrid, szHeader, dwAlignment, aryData, count)
///
/// @brief Add a row of data to the grid.
///
/// @note This macro combines several messages in order to simplify the addition of a row
///        of data to a grid.  It is primarily indended to be used to add a row of values to
///        columns of type GCT_EDIT.
///        
/// @param hGrid The handle of the grid.
/// @param szHeader Row header text string. 
/// @param dwAlignment The desired text alignment flag: @b GSA_LEFT, @b GSA_GENERAL, or @b GSA_RIGHT
/// @param aryData An array of values (cast to generic type @b LPARAM).
/// @param count The number of elements in @b aryData
///
/// @returns The return value is not meaningful.  
#define SimpleGrid_AddRowData(hGrid, szHeader, dwAlignment, aryData, count) { \
            SGITEM _sgi; _sgi.row = (int)(DWORD)SNDMSG((hGrid),SG_ADDROW,0,(LPARAM)(szHeader)); \
            if(SG_ERROR != _sgi.row) { for(int i = 0; i < count; ++i){ _sgi.col = i; _sgi.lpCurValue = (LPARAM) aryData[i]; \
                    if(SG_ERROR == (int)SNDMSG((hGrid),SG_SETITEMDATA, 0, (LPARAM)(LPSGITEM)&_sgi)) break; \
                    SNDMSG((hGrid),SG_SETITEMTEXTALIGNMENT,(WPARAM)(DWORD)(dwAlignment),(LPARAM)(LPSGITEM)&_sgi); }}}

/// @def SimpleGrid_EnableEdit(hGrid,fSet)
///
/// @brief Enable or disable editing in the grid.
///
/// @param hGrid The handle of the grid.
/// @param fSet TRUE to enable editing in grid, FALSE to disable. 
///
/// @returns The return value is not meaningful. 
#define SimpleGrid_EnableEdit(hGrid,fSet) (BOOL)SNDMSG((hGrid),SG_ENABLEEDIT,(WPARAM)(BOOL)(fSet),0L)

/// @def SimpleGrid_ExtendLastColumn(hGrid,fSet)
///
/// @brief Set the grid to extend the last column to the end of the client area.
///
/// @param hGrid The handle of the grid.
/// @param fSet TRUE to extend the last column, otherwise FALSE. 
///
/// @returns The return value is not meaningful.  
#define SimpleGrid_ExtendLastColumn(hGrid,fSet) (BOOL)SNDMSG((hGrid),SG_EXTENDLASTCOLUMN,(BOOL)(fSet),0L)

/// @def SimpleGrid_GetColCount(hGrid)
///
/// @brief Get the number of columns in grid.
///
/// @param hGrid The handle of the grid.
///
/// @returns The column count 
#define SimpleGrid_GetColCount(hGrid) (int)SNDMSG((hGrid),SG_GETCOLCOUNT,0,0L)

/// @def SimpleGrid_GetColumnHeaderText(hGrid, iCol, lpszText)
///
/// @brief Get the text displayed in the column header.
///
/// @param hGrid The handle of the grid.
/// @param iCol The number of the column.
/// @param lpszBuf A buffer to receive the column header text.
///
/// @returns ERROR_SUCCESS otherwise SG_ERROR if desired cell is out of bounds 
#define SimpleGrid_GetColumnHeaderText(hGrid, iCol, lpszBuf) (int)SNDMSG((hGrid),SG_GETCOLUMNHEADERTEXT,(WPARAM)(int)(iCol),(LPARAM)(lpszBuf))

/// @def SimpleGrid_GetColumnHeaderTextLen(hGrid, iCol)
///
/// @brief Retrieve the length of a string of column header text.
///
/// @param hGrid The handle of the grid.
/// @param iCol The number of the column
///
/// @returns The length of the string, in characters, excluding the terminating null character
///           if successfull, othewise SG_ERROR 
#define SimpleGrid_GetColumnHeaderTextLen(hGrid, iCol)  ((int)(DWORD)SNDMSG((hGrid),SG_GETCOLUMNHEADERTEXTLENGTH,(WPARAM)(int)(iCol),0))

/// @def SimpleGrid_GetColumnType(hGrid,iCol)
///
/// @brief Get the type of data represented in this cell.
///
/// @par Comment:
///      The returned type will be one of the following: GDT_TEXT, GDT_NUMERIC,
///       GDT_BTRUE, GDT_BFALSE, GDT_GRAPHIC
///
/// @param hGrid The handle of the grid.
/// @param iCol The number of a column
///
/// @returns The data type otherwise SG_ERROR if desired cell is out of bounds
#define SimpleGrid_GetColumnType(hGrid, iCol) (int)SNDMSG((hGrid),SG_GETCOLUMNTYPE, (WPARAM)(iCol), 0L)

/// @def SimpleGrid_GetColWidth(hGrid,iCol)
///
/// @brief Get the width (in pixels) of a column.
///
/// @param hGrid The handle of the grid.
/// @param iCol The index of the desired column. 
///
/// @returns The width of the desired column (in pixels), otherwise SG_ERROR if desired column is out of bounds.
#define SimpleGrid_GetColWidth(hGrid,iCol) (int)SNDMSG((hGrid),SG_GETCOLWIDTH,(WPARAM)(int)iCol,0L)

/// @def SimpleGrid_GetCursorCol(hGrid)
///
/// @brief Get the column of the cursor.
///
/// @param hGrid The handle of the grid.
///
/// @returns The number of the column the cursor currently occupies 
#define SimpleGrid_GetCursorCol(hGrid) (int)SNDMSG((hGrid),SG_GETCURSORCOL,0,0L)

/// @def SimpleGrid_GetCursorRow(hGrid)
///
/// @brief Get the row of the cursor.
///
/// @param hGrid The handle of the grid.
///
/// @returns The number of the row the cursor currently occupies 
#define SimpleGrid_GetCursorRow(hGrid) (int)SNDMSG((hGrid),SG_GETCURSORROW,0,0L)

/// @def SimpleGrid_GetHeaderRowHeight(hGrid)
///
/// @brief Get the height (in pixels) of the header row.
///
/// @param hGrid The handle of the grid.
///
/// @returns The height (in pixels) of the header row.  
#define SimpleGrid_GetHeaderRowHeight(hGrid) (int)SNDMSG((hGrid),SG_GETHEADERROWHEIGHT,0,0L)

/// @def SimpleGrid_GetImageColumnImageList(hGrid, col)
///
/// @brief Get the image list associated with an image column.
///
/// @param hGrid The handle of the grid.
/// @param iCol The index of a column of type GCT_IMAGE.
///
/// @returns The image list associated with the column if successful, otherwise NULL.  
#define SimpleGrid_GetImageColumnImageList(hGrid, iCol)  (HIMAGELIST)SNDMSG((hGrid),SG_GETIMAGELIST,(WPARAM)(iCol),0L)

/// @def SimpleGrid_GetItemData(hGrid, pItem)
///
/// @brief Get the content of an individual cell.
///
/// @param hGrid The handle of the grid.
/// @param pItem A pointer to a SGIETEM struct
///
/// @returns ERROR_SUCCESS otherwise SG_ERROR if desired cell is out of bounds
#define SimpleGrid_GetItemData(hGrid, pItem) (int)SNDMSG((hGrid),SG_GETITEMDATA, 0, (LPARAM)(pItem))

/// @def SimpleGrid_GetItemText(hGrid, iCol, iRow, pszText)
///
/// @brief Get the text of an individual item.
///
/// @note If the item belongs to a column of type GCT_EDIT, GCT_COMBO, 
///        or GCT_BUTTON the buffer will be filled with the item text.  
///        If the item belongs to a column of type GCT_LINK the 
///        buffer will be filled with a contiguous array buffer 
///        consisting of an array of two strings: display text and 
///        link URL.
///
/// @param hGrid The handle of the grid.
/// @param iCol The column index of the item.
/// @param iRow The row index of the item.
/// @param pszText Pointer to a buffer to recieve text.
///
/// @returns The return value is not meaningful.
#define SimpleGrid_GetItemText(hGrid, iCol, iRow, pszText)  { \
            SGITEM _sgi; _sgi.col = iCol; _sgi.row = iRow; _sgi.lpCurValue = (LPARAM) pszText; \
                SNDMSG((hGrid),SG_GETITEMDATA, 0, (LPARAM)(LPSGITEM)&_sgi); }

/// @def SimpleGrid_GetItemDataLen(hGrid, iCol, iRow)
///
/// @brief Retrieve the length of data associated with a grid item.
///
/// @note If the item belongs to a column of type GCT_EDIT, GCT_COMBO, 
///        or GCT_BUTTON the returned value is the length of the item 
///        text.  If the item belongs to a column of type GCT_LINK the 
///        returned value is the length of the contiguous array buffer 
///        consisting of an array of two strings: display text and 
///        link URL.  If the item belongs to a column of type GCT_CHECK or 
///        GCT_IMAGE then 0 is returned since this data is not a string.
///
/// @param hGrid The handle of the grid.
/// @param iCol The number of the column
/// @param iRow The number of the row
///
/// @returns The length of the data, in characters, excluding the terminating null character
///           if successfull, othewise SG_ERROR 
#define SimpleGrid_GetItemDataLen(hGrid, iCol, iRow) ((int)(DWORD)SNDMSG((hGrid),SG_GETITEMDATALENGTH,(WPARAM)(int)(iCol), (LPARAM)(int)(iRow)))

/// @def SimpleGrid_GetItemProtection(hGrid, pItem)
///
/// @brief Get the item's protection flag.
///
/// @param hGrid The handle of the grid.
/// @param iCol The number of the column
/// @param iRow The number of the row
///
/// @returns SG_ERROR if desired cell is out of bounds, TRUE if it is protected, otherwise FALSE
#define SimpleGrid_GetItemProtection(hGrid, iCol, iRow) (int)SNDMSG((hGrid),SG_GETITEMPROTECTION, (WPARAM)(int)(iCol), (LPARAM)(int)(iRow)) 

/// @def SimpleGrid_GetRowCount(hGrid)
///
/// @brief Get the number of rows in grid.
///
/// @param hGrid The handle of the grid.
///
/// @returns The row count 
#define SimpleGrid_GetRowCount(hGrid) (int)SNDMSG((hGrid),SG_GETROWCOUNT,0,0L)

/// @def SimpleGrid_GetRowHeaderText(hGrid, iRow, lpszBuf)
///
/// @brief Get the text displayed in the row header.
///
/// @param hGrid The handle of the grid.
/// @param iRow The number of the row.
/// @param lpszBuf A buffer to receive the row header text.
///
/// @returns ERROR_SUCCESS otherwise SG_ERROR if desired cell is out of bounds 
#define SimpleGrid_GetRowHeaderText(hGrid, iRow, lpszBuf) (int)SNDMSG((hGrid),SG_GETROWHEADERTEXT,(WPARAM)(int)(iCol),(LPARAM)(lpszBuf))

/// @def SimpleGrid_GetRowHeaderTextLen(hGrid, iRow)
///
/// @brief Retrieve the length of a string of column header text.
///
/// @param hGrid The handle of the grid.
/// @param iRow The number of the row.
///
/// @returns The length of the string, in characters, excluding the terminating null character
///           if successfull, othewise SG_ERROR 
#define SimpleGrid_GetRowHeaderTextLen(hGrid, iRow)  ((int)(DWORD)SNDMSG((hGrid),SG_GETROWHEADERTEXTLENGTH,(WPARAM)(int)(iCol),0L))

/// @def SimpleGrid_GetRowHeight(hGrid)
///
/// @brief Get the height (in pixels) of the rows.
///
/// @param hGrid The handle of the grid.
///
/// @returns The height (in pixels) of the rows 
#define SimpleGrid_GetRowHeight(hGrid) (int)SNDMSG((hGrid),SG_GETROWHEIGHT,0,0L) 

/// @def SimpleGrid_GetTitle(hwndCtl,lpch,cchMax)
///
/// @brief Get the title text of the grid.
///
/// @param hGrid The handle of the grid.
/// @param lpch The address of a character buffer for text.
/// @param cchMax The maximum number of characters to copy.
///
/// @returns If the function succeeds, the return value is the length, in characters, 
///           of the copied string, not including the terminating null character. If the 
///           window has no title bar or text, if the title bar is empty, or if the window 
///           or control handle is invalid, the return value is zero. 
#define SimpleGrid_GetTitle(hGrid,lpch,cchMax) GetWindowText((hGrid),(lpch),(cchMax))

/// @def SimpleGrid_GetTitleLength(hGrid)
///
/// @brief Get the length, in characters, of the grid’s title bar text.
///
/// @param hGrid The handle of the grid.
///
/// @returns If the function succeeds, the return value is the length, 
///           in characters, of the text otherwise 0. 
#define SimpleGrid_GetTitleLength(hGrid) GetWindowTextLength((hGrid))

/// @def SimpleGrid_RefreshGrid(hGrid)
///
/// @brief Cause the grid to redraw itself.
///
/// @param hGrid The handle of the grid.
///
/// @returns The return value is not meaningful. 
#define SimpleGrid_RefreshGrid(hGrid) (int)SNDMSG((hGrid),SG_REFRESHGRID,0,0L)

/// @def SimpleGrid_ResetContent(hGrid)
///
/// @brief Remove all data, rows, and columns from the grid.
///
/// @param hGrid The handle of the grid.
///
/// @returns The return value is not meaningful. 
#define SimpleGrid_ResetContent(hGrid) (BOOL)SNDMSG((hGrid),SG_RESETCONTENT,0,0L)

/// @def SimpleGrid_SelectCell(hGrid, iCol, iRow, fOverwritemode)
///
/// @brief Select a cell and set the focus for editing it's contents.
///
/// @param hGrid The handle of the grid.
/// @param iCol The number of the column
/// @param iRow The number of the row
/// @param fOverwritemode TRUE to overwrite cell contents, FALSE to edit
///         the current contents of the cell.
///
/// @returns ERROR_SUCCESS otherwise SG_ERROR if desired cell is out of bounds 
#define SimpleGrid_SelectCell(hGrid, iCol, iRow, fOverwritemode)  { \
            SGITEM _sgi; _sgi.col = iCol; _sgi.row = iRow; \
                SNDMSG((hGrid),SG_SELECTCELL, (WPARAM)(BOOL)fOverwritemode, (LPARAM)(LPSGITEM)&_sgi); }

/// @def SimpleGrid_SetAllowColResize(hGrid,fSet)
///
/// @brief Configure column resizing. 
///
/// @param hGrid The handle of the grid.
/// @param fSet TRUE for resizeable columns, FALSE locks in column widths.
///
/// @returns The return value is not meaningful.  
#define SimpleGrid_SetAllowColResize(hGrid,fSet) (BOOL)SNDMSG((hGrid),SG_SETALLOWCOLRESIZE,(BOOL)(fSet),0L)

/// @def SimpleGrid_SetColAutoWidth(hGrid,fSet)
///
/// @brief Configure grid columns to auto adjust to fit contents.
///
/// @note This should be set before adding data to the grid.
///
/// @param hGrid The handle of the grid.
/// @param fSet fSet TRUE to autosize columns to updated content, otherwise FALSE.
///
/// @returns The return value is not meaningful.  
#define SimpleGrid_SetColAutoWidth(hGrid,fSet) (BOOL)SNDMSG((hGrid),SG_SETCOLAUTOWIDTH,(BOOL)(fSet),0L)

/// @def SimpleGrid_SetColsNumbered(hGrid,fSet)
///
/// @brief Sets what is displayed in the Column headers.
///
/// @param hGrid The handle of the grid.
/// @param fSet TRUE to display Column hexavigesimal digits, FALSE to display header text. 
///
/// @returns The return value is not meaningful. 
#define SimpleGrid_SetColsNumbered(hGrid,fSet) (BOOL)SNDMSG((hGrid),SG_SETCOLSNUMBERED,(BOOL)(fSet),0L)

/// @def SimpleGrid_SetColumnHeaderText(hGrid, iCol, lpszText)
///
/// @brief Set the text to be displayed in the column header.
///
/// @param hGrid The handle of the grid.
/// @param iCol The number of the column
/// @param lpszText The text string to display 
///
/// @returns ERROR_SUCCESS otherwise SG_ERROR if desired cell is out of bounds 
#define SimpleGrid_SetColumnHeaderText(hGrid, iCol, lpszText) (int)SNDMSG((hGrid),SG_SETCOLUMNHEADERTEXT,(WPARAM)(int)(iCol),(LPARAM)(lpzText))

/// @def SimpleGrid_SetColWidth(hGrid,iCol,nWidth)
///
/// @brief Set the width (in pixels) of a given column.
///
/// @param hGrid The handle of the grid.
/// @param iCol The index of the column.
/// @param nWidth The desired width (in pixels) of the column.
///
/// @returns ERROR_SUCCESS otherwise SG_ERROR if desired column is out of bounds
#define SimpleGrid_SetColWidth(hGrid,iCol,nWidth) (int)SNDMSG((hGrid),SG_SETCOLWIDTH,(WPARAM)(int)(iCol),(LPARAM)(nWidth))

/// @def SimpleGrid_SetCursorPos(hGrid,iCol,iRow)
///
/// @brief Set the cursor position (or currently selected cell) in the grid.
///
/// @param hGrid The handle of the grid.
/// @param iCol The col index of the cell.
/// @param iRow The row index of the cell. 
///
/// @returns ERROR_SUCCESS otherwise SG_ERROR if desired cell is out of bounds
#define SimpleGrid_SetCursorPos(hGrid,iCol,iRow) (int)SNDMSG((hGrid),SG_SETCURSORPOS,(WPARAM)(int)(iCol),(LPARAM)(iRow))

/// @def SimpleGrid_SetDoubleBuffer(hGrid, fSet)
///
/// @brief Set Double buffer to reduce flicker in a grid with many columns.
///
/// @note By default SimpleGrid uses double buffering.
///
/// @param hGrid The handle of the grid.
/// @param fSet TRUE to use double buffering, FALSE turns double buffering off. 
///
/// @returns The return value is not meaningful.
#define SimpleGrid_SetDoubleBuffer(hGrid, fSet)  (BOOL)SNDMSG((hGrid),SG_SETDOUBLEBUFFER,(WPARAM)(BOOL)(fSet),0L)

/// @def SimpleGrid_SetEllipsis(hGrid,fSet)
///
/// @brief Set the display of text in cell.
///
/// @param hGrid The handle of the grid.
/// @param fSet TRUE to draw elipsis, FALSE to truncate text in short cell. 
///
/// @returns The return value is not meaningful. 
#define SimpleGrid_SetEllipsis(hGrid,fSet) (BOOL)SNDMSG((hGrid),SG_SETELLIPSIS,(WPARAM)(BOOL)(fSet),0L)

/// @def SimpleGrid_SetGridLineColor(hGrid,clrGrdLine)
///
/// @brief Sets the color of the gridlines.
///
/// @param hGrid The handle of the grid.
/// @param clrGrdLine A COLORREF value. 
///
/// @returns The return value is not meaningful. 
#define SimpleGrid_SetGridLineColor(hGrid,clrGrdLine)  (BOOL)SNDMSG((hGrid),SG_SETGRIDLINECOLOR,(WPARAM)(UINT)(clrGrdLine),0L)

/// @def SimpleGrid_SetHeaderRowHeight(hGrid,iHeight)
///
/// @brief Set the height (in pixels) of the header row.
///
/// @param hGrid The handle of the grid.
/// @param iHeight The desired height (in pixels) of the header row. 
///
/// @returns The return value is not meaningful. 
#define SimpleGrid_SetHeaderRowHeight(hGrid,iHeight) (BOOL)SNDMSG((hGrid),SG_SETHEADERROWHEIGHT,(WPARAM)(int)(iHeight),0L)

/// @def SimpleGrid_SetHeadingFont(hGrid,hFont)
///
/// @brief Set the font used in the grid headers.
///
/// @param hGrid The handle of the grid.
/// @param hFont The handle to a font. 
///
/// @returns The return value is not meaningful.
#define SimpleGrid_SetHeadingFont(hGrid,hFont) (int)SNDMSG((hGrid),SG_SETHEADINGFONT,(WPARAM)(UINT)(hFont),0L)

/// @def SimpleGrid_SetHilightColor(hGrid, clrHilt)
///
/// @brief Sets the background color of hilighted cells.
///
/// @param hGrid The handle of the grid.
/// @param clrHilt A COLORREF value. 
///
/// @returns The return value is not meaningful. 
#define SimpleGrid_SetHilightColor(hGrid, clrHilt) (BOOL)SNDMSG((hGrid),SG_SETHILIGHTCOLOR,(WPARAM)(UINT)(clrHilt),0L)

/// @def SimpleGrid_SetHilightTextColor(hGrid, clrHlText)
///
/// @brief Sets the text color of hilighted cells.
///
/// @param hGrid The handle of the grid.
/// @param clrHlText A COLORREF value. 
///
/// @returns The return value is not meaningful. 
#define SimpleGrid_SetHilightTextColor(hGrid, clrHlText) (BOOL)SNDMSG((hGrid),SG_SETHILIGHTTEXTCOLOR,(WPARAM)(UINT)(clrHlText),0L)

/// @def SimpleGrid_SetImageColumnImageList(hGrid, icol, himl)
///
/// @brief Set the image list associated with an image column.
///
/// @param hGrid The handle of the grid.
/// @param icol The index of a column of type GCT_IMAGE.
/// @param himl The handle to the image list.
///
/// @returns The previous image list associated with the column if successful, otherwise NULL.  
#define SimpleGrid_SetImageColumnImageList(hGrid, icol, himl)  (HIMAGELIST)SNDMSG((hGrid),SG_SETIMAGELIST,(WPARAM)(iCol),(LPARAM)(HIMAGELIST)(himl))

/// @def SimpleGrid_SetItemData(hGrid, pItem)
///
/// @brief Set the content of an individual cell.
///
/// @param hGrid The handle of the grid.
/// @param pItem A pointer to a SGIETEM struct
///
/// @returns ERROR_SUCCESS otherwise SG_ERROR if desired cell is out of bounds
#define SimpleGrid_SetItemData(hGrid, pItem) (int)SNDMSG((hGrid),SG_SETITEMDATA, 0, (LPARAM)(pItem))

/// @def SimpleGrid_SetItemText(hGrid, iCol, iRow, pszText)
///
/// @brief Set the text of an individual item (cell).
///
/// @note If the item belongs to a column of type GCT_EDIT, GCT_COMBO, 
///        or GCT_BUTTON the string should contain item text.  
///        If the item belongs to a column of type GCT_LINK the 
///        the string should be a contiguous array buffer (or double null terminated
///        string) consisting of an array of two strings: display text and 
///        link URL.
///
/// @param hGrid The handle of the grid.
/// @param iCol The column index of the item.
/// @param iRow The row index of the item.
/// @param pszText The text string.
///
/// @returns The return value is not meaningful.
#define SimpleGrid_SetItemText(hGrid, iCol, iRow, pszText)  { \
            SGITEM _sgi; _sgi.col = iCol; _sgi.row = iRow; _sgi.lpCurValue = (LPARAM) pszText; \
                SNDMSG((hGrid),SG_SETITEMDATA, 0, (LPARAM)(LPSGITEM)&_sgi); }

/// @def SimpleGrid_SetItemTextAlignment(hGrid, pItem, iAlign)
///
/// @brief Set text alignment of an item to Left, general, or right.
///
/// @param hGrid The handle of the grid.
/// @param pItem A pointer to a SGIETEM struct
/// @param iAlign One of the alignments (GSA_LEFT, GSA_GENERAL, or GSA_RIGHT). 
///
/// @returns ERROR_SUCCESS otherwise SG_ERROR if desired cell is out of bounds
#define SimpleGrid_SetItemTextAlignment(hGrid, pItem, iAlign) (int)SNDMSG((hGrid),SG_SETITEMTEXTALIGNMENT,(WPARAM)(INT)(iAlign),(LPARAM)(pItem))

/// @def SimpleGrid_SetItemTextAlignmentEx(hGrid, iCol, iRow, fSet)
///
/// @brief Set text alignment of an item to Left, general, or right.
///
/// @param hGrid The handle of the grid.
/// @param iCol The column index of the item.
/// @param iRow The row index of the item.
/// @param dwAlign One of the alignments (GSA_LEFT, GSA_GENERAL, or GSA_RIGHT). 
///
/// @returns The return value is not meaningful.
#define SimpleGrid_SetItemTextAlignmentEx(hGrid,  iCol, iRow, dwAlign)  { \
            SGITEM _sgi; _sgi.col = iCol; _sgi.row = iRow; \
                SNDMSG((hGrid),SG_SETITEMTEXTALIGNMENT, (WPARAM)(DWORD)(dwAlign), (LPARAM)(LPSGITEM)&_sgi); }

/// @def SimpleGrid_SetItemProtection(hGrid, pItem, fSet)
///
/// @brief Set protection status of an individual cell.
///
/// @param hGrid The handle of the grid.
/// @param pItem A pointer to a SGIETEM struct
/// @param fSet TRUE to protect cell FALSE to allow changes. 
///
/// @returns ERROR_SUCCESS otherwise SG_ERROR if desired cell is out of bounds
#define SimpleGrid_SetItemProtection(hGrid, pItem, fSet) (int)SNDMSG((hGrid),SG_SETITEMPROTECTION,(WPARAM)(fSet),(LPARAM)(pItem))

/// @def SimpleGrid_SetItemProtectionEx(hGrid, iCol, iRow, fSet)
///
/// @brief Set protection status of an individual cell.
///
/// @param hGrid The handle of the grid.
/// @param iCol The column index of the item.
/// @param iRow The row index of the item.
/// @param fSet TRUE to protect cell FALSE to allow changes. 
///
/// @returns The return value is not meaningful.
#define SimpleGrid_SetItemProtectionEx(hGrid, iCol, iRow, fSet)  { \
            SGITEM _sgi; _sgi.col = iCol; _sgi.row = iRow; \
                SNDMSG((hGrid),SG_SETITEMPROTECTION,(WPARAM)(fSet),(LPARAM)(LPSGITEM)&_sgi); }

/// @def SimpleGrid_SetProtectColor(hGrid, clrProtect)
///
/// @brief Sets the background color of protected cells.
///
/// @param hGrid The handle of the grid.
/// @param clrProtect A COLORREF value. 
///
/// @returns The return value is not meaningful. 
#define SimpleGrid_SetProtectColor(hGrid, clrProtect) (BOOL)SNDMSG((hGrid),SG_SETPROTECTCOLOR,(WPARAM)(UINT)(clrProtect),0L)

/// @def SimpleGrid_SetRowHeaderText(hGrid, iCol, lpszText)
///
/// @brief Set the text to be displayed in the row header.
///
/// @param hGrid The handle of the grid.
/// @param iRow The number of the column
/// @param lpszText The text string to display 
///
/// @returns ERROR_SUCCESS otherwise SG_ERROR if desired cell is out of bounds 
#define SimpleGrid_SetRowHeaderText(hGrid, iRow, lpszText) (int)SNDMSG((hGrid),SG_SETROWHEADERTEXT,(WPARAM)(int)(iRow),(LPARAM)(lpzText))

/// @def SimpleGrid_SetRowHeaderWidth(hGrid,nWidth)
///
/// @brief Set the width (in pixels) of the row header column.
///
/// @param hGrid The handle of the grid.
/// @param nWidth The desired width (in pixels) of the row headers.
///
/// @returns ERROR_SUCCESS otherwise SG_ERROR if desired column is out of bounds
#define SimpleGrid_SetRowHeaderWidth(hGrid,nWidth) (int)SNDMSG((hGrid),SG_SETROWHEADERWIDTH,0,(LPARAM)(nWidth))

/// @def SimpleGrid_SetRowHeight(hGrid,iHeight)
///
/// @brief Set the height (in pixels) of the rows.
///
/// @param hGrid The handle of the grid.
/// @param iHeight The desired height (in pixels) of the rows. 
///
/// @returns The return value is not meaningful. 
#define SimpleGrid_SetRowHeight(hGrid,iHeight) (BOOL)SNDMSG((hGrid),SG_SETROWHEIGHT,(WPARAM)(int)(iHeight),0L)

/// @def SimpleGrid_SetRowsNumbered(hGrid,fSet)
///
/// @brief Sets what is displayed in the Row headers.
///
/// @param hGrid The handle of the grid.
/// @param fSet TRUE to display row numbers, FALSE to display header text. 
///
/// @returns The return value is not meaningful. 
#define SimpleGrid_SetRowsNumbered(hGrid,fSet) (BOOL)SNDMSG((hGrid),SG_SETROWSNUMBERED,(WPARAM)(BOOL)(fSet),0L)

/// @def SimpleGrid_SetSelectionMode(hGrid,iMode)
///
/// @brief Set whether and how the selected row will be hilighted.
///
/// @param hGrid The handle of the grid.
/// @param iMode One of the following selection mode options: GSO_ROWHEADER, GSO_CELL, or GSO_FULLROW. 
///
/// @returns The return value is not meaningful.
#define SimpleGrid_SetSelectionMode(hGrid,iMode) (BOOL)SNDMSG((hGrid),SG_SETSELECTIONMODE,(WPARAM)(INT)(iMode),0L)

/// @def SimpleGrid_SetTitle(hGrid,lpsz)
///
/// @brief Set the text of the grid title.
///
/// @param hGrid The handle of the grid.
/// @param lpsz The title text. 
///
/// @returns TRUE if successful, otherwise FALSE. 
#define SimpleGrid_SetTitle(hGrid,lpsz) SetWindowText((hGrid),(lpsz))

/// @def SimpleGrid_SetTitleFont(hGrid,hFont)
///
/// @brief Set the font used to display the grid title.
///
/// @param hGrid The handle of the grid.
/// @param hFont The handle to a font. 
///
/// @returns The return value is not meaningful. 
#define SimpleGrid_SetTitleFont(hGrid,hFont) (int)SNDMSG((hGrid),SG_SETTITLEFONT,(WPARAM)(UINT)(hFont),0L)

/// @def SimpleGrid_SetTitleHeight(hGrid,iHeight)
///
/// @brief Set the height of the grid title.
///
/// @param hGrid The handle of the grid.
/// @param iHeight The desired height (in pixels) of the title text. 
///
/// @returns The return value is not meaningful. 
#define SimpleGrid_SetTitleHeight(hGrid,iHeight) (BOOL)SNDMSG((hGrid),SG_SETTITLEHEIGHT,(WPARAM)(int)(iHeight),0L)

/// @def SimpleGrid_ShowIntegralRows(hGrid,fShow)
///
/// @brief sizing and scrolling the client of the control in row height governed steps.
///
/// @param hGrid The handle of the grid.
/// @param fShow  TRUE to display bottom visible row at integral height, 
///               FALSE allows the display of a partial row. 
///
/// @returns The return value is not meaningful. 
#define SimpleGrid_ShowIntegralRows(hGrid,fShow) (BOOL)SNDMSG((hGrid),SG_SHOWINTEGRALROWS,(WPARAM)(BOOL)(fShow),0L)

/// @def SimpleGrid_Enable(hGrid,fEnable)
///
/// @brief Enables or disables mouse and keyboard input to the specified grid.
///
/// @param hGrid The handle of the grid.
/// @param fEnable TRUE to allow input, otherwise FALSE. 
///
/// @returns TRUE If the window was previously disabled, otherwise false. 
#define SimpleGrid_Enable(hGrid,fEnable) EnableWindow((hGrid),(fEnable))

/// @}

/****************************************************************************/
// Exported function prototypes

ATOM InitSimpleGrid(HINSTANCE hInstance);
HWND New_SimpleGrid(HWND hParent, DWORD dwID);

#endif //GENERICCONTROL_H
