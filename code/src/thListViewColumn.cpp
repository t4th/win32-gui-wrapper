#include "thListViewColumn.h"
#include "thListView.h"

thListViewColumn::thListViewColumn( thListView & a_Parent, const thString & a_itemText)
    :
    m_Parent( a_Parent)
{
    TH_ENTER_FUNCTION;

    LVCOLUMN lvCol = { 0 }; // columns struct: https://msdn.microsoft.com/en-us/library/windows/desktop/bb774743%28v=vs.85%29.aspx

    m_text = a_itemText;

    // get number of columns
    HWND hHeader = ListView_GetHeader( this->m_Parent.GetHandle());
    int nColumnCount = Header_GetItemCount( hHeader);

    if ( -1 == nColumnCount)
    {
        nColumnCount = 0;
    }

    lvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM; // Type of mask
    lvCol.cx = 0x42; // Width of the column, in pixels
    lvCol.pszText = const_cast< LPTSTR>( m_text.c_str());

    LRESULT result = ListView_InsertColumn( this->m_Parent.GetHandle(), nColumnCount, &lvCol);

    if ( -1 != result)
    {
    }
    else
    {
        MSG_ERROR( TEXT( "LVM_INSERTCOLUMN failed with error = 0x%X"), GetLastError());
    }

    TH_LEAVE_FUNCTION;
}

int thListViewColumn::GetIndex() const
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
    return m_index;
}