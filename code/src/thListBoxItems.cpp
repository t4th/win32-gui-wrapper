#include "thListBoxItems.h"
#include "thListBox.h"

void thListBoxItems::setParent(thListBox const * const a_pParent) {
    TH_ENTER_FUNCTION;
    if (a_pParent && (NULL == m_pParent)) {
        this->m_pParent = a_pParent;
    }
    TH_LEAVE_FUNCTION;
}

thListBoxItems::thListBoxItems()
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

thListBoxItems::~thListBoxItems()
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

thListBoxItem thListBoxItems::operator[] (int a_dItemIndex)
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
    return thListBoxItem(this->m_pParent, a_dItemIndex);
}

void thListBoxItems::Add(const thString & a_newItemValue)
{
    TH_ENTER_FUNCTION;
    int dResult = 0;

    dResult = ListBox_AddString(this->m_pParent->GetHandle(), a_newItemValue.c_str());

    if (LB_ERRSPACE == dResult) {

    }
    TH_LEAVE_FUNCTION;
}

void thListBoxItems::Remove(int _index)
{
    TH_ENTER_FUNCTION;
    int dResult = 0;

    dResult = ListBox_DeleteString(this->m_pParent->GetHandle(), _index);

    if (CB_ERR == dResult) {
        MSG_ERROR(TEXT("ListBox_DeleteString failed with error = 0x%X"), GetLastError());
    }
    TH_LEAVE_FUNCTION;
}

int thListBoxItems::GetCount(void) const
{
    TH_ENTER_FUNCTION;
    int nResult = 0;

    nResult = ListBox_GetCount(this->m_pParent->GetHandle());

    TH_LEAVE_FUNCTION;
    return nResult;
}

int thListBoxItems::ItemIndex(void) const
{
    TH_ENTER_FUNCTION;
    int nResult = 0;

    nResult = ListBox_GetCurSel(this->m_pParent->GetHandle());

    if (CB_ERR == nResult) {
        MSG_ERROR(TEXT("ListBox_GetCurSel failed with error = 0x%X"), GetLastError());
    }

    TH_LEAVE_FUNCTION;
    return nResult;
}

void thListBoxItems::SetItemIndex(int a_nItemIndex)
{
    TH_ENTER_FUNCTION;
    int nResult = 0;

    nResult = ListBox_SetCurSel(this->m_pParent->GetHandle(), a_nItemIndex);

    if (CB_ERR == nResult && -1 != a_nItemIndex) {
        MSG_ERROR(TEXT("ListBox_SetCurSel failed with error = 0x%X"), GetLastError());
    }

    TH_LEAVE_FUNCTION;
}

thListBoxItem::~thListBoxItem(void)
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

thListBoxItem::thListBoxItem(const thListBox * a_pParent, int a_nItem)
{
    TH_ENTER_FUNCTION;
    m_nIndex = a_nItem;

    if (a_pParent) {
        m_pParent = a_pParent;
    }

    TH_LEAVE_FUNCTION;
}

void thListBoxItem::Remove(void)
{
    TH_ENTER_FUNCTION;
    int result = CB_ERR;

    result = ListBox_DeleteString(this->m_pParent->GetHandle(), m_nIndex);

    if (CB_ERR == result) {
        MSG_ERROR(TEXT("ListBox_DeleteString failed with error = 0x%X"), GetLastError());
    }

    TH_LEAVE_FUNCTION;
}

thString thListBoxItem::GetText(void)
{
    TH_ENTER_FUNCTION;
    thString    result;
    int         nLen = 0;

    nLen = ListBox_GetTextLen(this->m_pParent->GetHandle(), m_nIndex);

    if (nLen > 0)
    {
        std::unique_ptr<TCHAR[]> buffer( new TCHAR[nLen + 1]); //+1 for '\0'

        if (CB_ERR == ListBox_GetText(this->m_pParent->GetHandle(), m_nIndex, buffer.get())) {
            MSG_ERROR(TEXT("ListBox_GetLBText failed with error = 0x%X"), GetLastError());
        }
        else {
            result = thString(buffer.get(), nLen);
        }
    }

    TH_LEAVE_FUNCTION;
    return result;
}