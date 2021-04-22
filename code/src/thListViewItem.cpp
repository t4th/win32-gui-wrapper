#include "thListViewItem.h"
#include "thListView.h"

thListViewItem::thListViewItem( thListView & a_Parent, const thString & a_itemText)
    :
    m_Parent( a_Parent),
    SubItems( *this),
    m_index( -1)
{
    TH_ENTER_FUNCTION;

    LVITEM lvItem = { 0 }; // columns struct: https://msdn.microsoft.com/en-us/library/windows/desktop/bb774743%28v=vs.85%29.aspx

    m_text = a_itemText;

    int dItemCount = ListView_GetItemCount( this->m_Parent.GetHandle());

    lvItem.mask = LVIF_TEXT; // set text mask
    lvItem.cchTextMax = static_cast< int>( m_text.size()); // buffer size
    lvItem.iItem = dItemCount; // insert new item at the end
    lvItem.iSubItem = 0; // must be 0
    lvItem.pszText = const_cast< LPTSTR>( m_text.c_str());

    LRESULT result = ListView_InsertItem( this->m_Parent.GetHandle(), &lvItem);

    if ( -1 != result)
    {
        m_index = static_cast< int>( result);
    }
    else
    {
        MSG_ERROR( TEXT( "ListView_InsertItem failed with error = 0x%X"), GetLastError());
    }

    TH_LEAVE_FUNCTION;
}

thListView & thListViewItem::GetParent() const
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
    return this->m_Parent;
}

int thListViewItem::GetIndex() const
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
    return this->m_index;
}

void thListViewItem::SetText( const thString & a_itemText)
{
    TH_ENTER_FUNCTION;

    LVITEM lvItem = { 0 };

    m_text = a_itemText;

    lvItem.mask = LVIF_TEXT; // set text mask
    lvItem.cchTextMax = static_cast< int>( m_text.size()); // buffer size
    lvItem.iItem = m_index; // insert new item at the end
    lvItem.iSubItem = 0;
    lvItem.pszText = const_cast< LPTSTR>( m_text.c_str());

    LRESULT result = ListView_SetItem( this->m_Parent.GetHandle(), &lvItem);

    if ( TRUE == result)
    {
    }
    else
    {
        MSG_ERROR( TEXT( "ListView_SetItem failed with error = 0x%X"), GetLastError());
    }

    TH_LEAVE_FUNCTION;
}

// Sub item list.
thListViewSubItemList::thListViewSubItemList( thListViewItem & a_Parent)
    :
    m_Parent( a_Parent)
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
}

thListViewSubItem thListViewSubItemList::operator[]( int a_nColumn)
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
    return thListViewSubItem( m_Parent, a_nColumn);
}


// Sub item definitiopns.
thListViewSubItem::thListViewSubItem( thListViewItem & a_Parent, int a_nColumn)
    :
    m_Parent{ a_Parent},
    m_nParentColumn{ a_nColumn}
{
    TH_ENTER_FUNCTION;
    
    TH_LEAVE_FUNCTION;
}


void thListViewSubItem::SetText( const thString & a_itemText)
{
    TH_ENTER_FUNCTION;

    LVITEM lvItem = { 0 };

    m_text = a_itemText;

    lvItem.mask = LVIF_TEXT; // set text mask
    lvItem.cchTextMax = static_cast< int>( m_text.size()); // buffer size
    lvItem.iItem = m_Parent.GetIndex(); //parent item index
    lvItem.iSubItem = m_nParentColumn;
    lvItem.pszText = const_cast< LPTSTR>( m_text.c_str());

    LRESULT result = ListView_SetItem( this->m_Parent.GetParent().GetHandle(), &lvItem);

    if ( TRUE == result)
    {
    }
    else
    {
        MSG_ERROR( TEXT( "ListView_SetItem failed with error = 0x%X"), GetLastError());
    }

    TH_LEAVE_FUNCTION;
}