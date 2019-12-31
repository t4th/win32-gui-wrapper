#include "thListViewColumn.h"
#include "thListView.h"

thListViewColumn::thListViewColumn()
{
    m_index = 0;
}

thListViewColumn::thListViewColumn(const thListView * a_pParent, const thString & a_itemText)
{
    TH_ENTER_FUNCTION;
    if (a_pParent) {
        LVCOLUMN    lvCol = { 0 }; // columns struct: https://msdn.microsoft.com/en-us/library/windows/desktop/bb774743%28v=vs.85%29.aspx
        LRESULT     result = -1;
        int         nColumnCount = 0;
        HWND        hHeader = 0;

        m_text = a_itemText;
        m_pParent = a_pParent;

        // get number of columns
        hHeader = ListView_GetHeader(this->m_pParent->GetHandle());
        nColumnCount = Header_GetItemCount(hHeader);

        if (-1 == nColumnCount) {
            nColumnCount = 0;
        }

        lvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM; // Type of mask
        lvCol.cx = 0x42; // Width of the column, in pixels
        lvCol.pszText = const_cast<LPTSTR>(m_text.c_str());

        result = ListView_InsertColumn(this->m_pParent->GetHandle(), nColumnCount, &lvCol);

        if (-1 != result) {
        }
        else {
            MSG_ERROR(TEXT("LVM_INSERTCOLUMN failed with error = 0x%X"), GetLastError());
        }
    }
    TH_LEAVE_FUNCTION;
}

thListViewColumn::~thListViewColumn()
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

int thListViewColumn::GetIndex(void) const
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
    return m_index;
}