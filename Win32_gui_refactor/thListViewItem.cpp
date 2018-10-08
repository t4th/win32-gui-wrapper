#include "thListViewItem.h"
#include "thListView.h"

thListViewItem::thListViewItem()
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
}

thListViewItem::thListViewItem(const thListView * a_pParent, const thString & a_itemText) : m_index(-1)
{
    TH_ENTER_FUNCTION;

    if (a_pParent) {
        LVITEM    lvItem = { 0 }; // columns struct: https://msdn.microsoft.com/en-us/library/windows/desktop/bb774743%28v=vs.85%29.aspx
        LRESULT   result = FALSE;
        int       dItemCount = 0;

        m_text = a_itemText;
        m_pParent = a_pParent;
        this->SubItems.SetParent(this);

        dItemCount = ListView_GetItemCount(this->m_pParent->GetHandle());

        lvItem.mask = LVIF_TEXT; // set text mask
        lvItem.cchTextMax = m_text.size(); // buffer size
        lvItem.iItem = dItemCount; // insert new item at the end
        lvItem.iSubItem = 0; // must be 0
        lvItem.pszText = const_cast<LPWSTR>(m_text.c_str());

        result = ListView_InsertItem(this->m_pParent->GetHandle(), &lvItem);

        if (-1 != result) {
            m_index = result;
        }
        else {
            MSG_ERROR(L"ListView_InsertItem failed with error = 0x%X", GetLastError());
        }
    }

    TH_LEAVE_FUNCTION;
}

thListViewItem::~thListViewItem()
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
}

const thListView * thListViewItem::GetParent() const
{
    TH_ENTER_FUNCTION;
    const thListView * pParent = 0;

    pParent = this->m_pParent;
    TH_LEAVE_FUNCTION;
    return pParent;
}

int thListViewItem::GetIndex() const
{
    TH_ENTER_FUNCTION;
    int nIndex = -1;

    nIndex = this->m_index;
    TH_LEAVE_FUNCTION;
    return nIndex;
}

void thListViewItem::SetText(thString a_itemText)
{
    TH_ENTER_FUNCTION;
    LVITEM      lvItem = { 0 };
    LRESULT     result = FALSE;

    m_text = a_itemText;

    lvItem.mask = LVIF_TEXT; // set text mask
    lvItem.cchTextMax = m_text.size(); // buffer size
    lvItem.iItem = m_index; // insert new item at the end
    lvItem.iSubItem = 0;
    lvItem.pszText = const_cast<LPWSTR>(m_text.c_str());

    result = ListView_SetItem(this->m_pParent->GetHandle(), &lvItem);

    if (TRUE == result) {
    }
    else {
        MSG_ERROR(L"ListView_SetItem failed with error = 0x%X", GetLastError());
    }
    TH_LEAVE_FUNCTION;
}

// sub item list
thListViewSubItemList::thListViewSubItemList()
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
}

thListViewSubItemList::~thListViewSubItemList()
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
}

void thListViewSubItemList::SetParent(thListViewItem * a_pParent)
{
    TH_ENTER_FUNCTION;

    m_pParent = a_pParent;
    TH_LEAVE_FUNCTION;
}

thListViewSubItem thListViewSubItemList::operator[](int a_nColumn)
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
    return thListViewSubItem(m_pParent, a_nColumn);
}


// sub item
thListViewSubItem::thListViewSubItem()
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
}

thListViewSubItem::thListViewSubItem(const thListViewItem * a_pParent, int a_nColumn)
{
    TH_ENTER_FUNCTION;
    m_nParentColumn = a_nColumn;

    if (a_pParent) {
        m_pParent = a_pParent;
    }
    TH_LEAVE_FUNCTION;
}

thListViewSubItem::~thListViewSubItem()
{
}

void thListViewSubItem::SetText(thString a_itemText)
{
    TH_ENTER_FUNCTION;
    LVITEM      lvItem = { 0 };
    LRESULT     result = FALSE;

    m_text = a_itemText;

    lvItem.mask = LVIF_TEXT; // set text mask
    lvItem.cchTextMax = m_text.size(); // buffer size
    lvItem.iItem = m_pParent->GetIndex(); //parent item index
    lvItem.iSubItem = m_nParentColumn;
    lvItem.pszText = const_cast<LPWSTR>(m_text.c_str());

    result = ListView_SetItem(this->m_pParent->GetParent()->GetHandle(), &lvItem);

    if (TRUE == result) {
    }
    else {
        MSG_ERROR(L"ListView_SetItem failed with error = 0x%X", GetLastError());
    }
    TH_LEAVE_FUNCTION;
}