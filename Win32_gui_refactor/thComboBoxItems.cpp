#include "thComboBoxItems.h"
#include "thComboBox.h"

thComboBoxItems::thComboBoxItems(thComboBox & a_pParent) : m_pParent(a_pParent)
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

thComboBoxItems::~thComboBoxItems()
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

thComboBoxItem thComboBoxItems::operator[] (int a_dItemIndex)
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
    return thComboBoxItem(m_pParent, a_dItemIndex);
}

void thComboBoxItems::Add(const thString & a_newItemValue)
{
    TH_ENTER_FUNCTION;
    int dResult = 0;

    dResult = ComboBox_AddString(this->m_pParent.GetHandle(), a_newItemValue.c_str());

    if (LB_ERRSPACE == dResult) {

    }
    TH_LEAVE_FUNCTION;
}

void thComboBoxItems::Remove(int _index)
{
    TH_ENTER_FUNCTION;
    int dResult = 0;

    dResult = ComboBox_DeleteString(this->m_pParent.GetHandle(), _index);

    if (CB_ERR == dResult) {
        MSG_ERROR(TEXT("ComboBox_DeleteString failed with error = 0x%X"), GetLastError());
    }
    TH_LEAVE_FUNCTION;
}

int thComboBoxItems::GetCount(void) const
{
    TH_ENTER_FUNCTION;
    int nResult = 0;

    nResult = ComboBox_GetCount(this->m_pParent.GetHandle());

    TH_LEAVE_FUNCTION;
    return nResult;
}

int thComboBoxItems::ItemIndex(void) const
{
    TH_ENTER_FUNCTION;
    int nResult = 0;

    nResult = ComboBox_GetCurSel(this->m_pParent.GetHandle());

    if (CB_ERR == nResult) {
        MSG_ERROR(TEXT("ComboBox_GetCurSel failed with error = 0x%X"), GetLastError());
    }

    TH_LEAVE_FUNCTION;
    return nResult;
}

void thComboBoxItems::SetItemIndex(int a_nItemIndex)
{
    TH_ENTER_FUNCTION;
    int nResult = 0;

    nResult = ComboBox_SetCurSel(this->m_pParent.GetHandle(), a_nItemIndex);

    if (CB_ERR == nResult && -1 != a_nItemIndex) {
        MSG_ERROR(TEXT("ComboBox_SetCurSel failed with error = 0x%X"), GetLastError());
    }

    TH_LEAVE_FUNCTION;
}

thComboBoxItem::~thComboBoxItem(void)
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

thComboBoxItem::thComboBoxItem(thComboBox & a_Parent, int a_Item) :
    m_pParent(a_Parent),
    m_nIndex(a_Item)
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
}

void thComboBoxItem::Remove(void)
{
    TH_ENTER_FUNCTION;
    int result = CB_ERR;

    result = ComboBox_DeleteString(this->m_pParent.GetHandle(), m_nIndex);

    if (CB_ERR == result) {
        MSG_ERROR(TEXT("ComboBox_DeleteString failed with error = 0x%X"), GetLastError());
    }

    TH_LEAVE_FUNCTION;
}

thString thComboBoxItem::GetText(void)
{
    TH_ENTER_FUNCTION;
    thString    result;
    TCHAR *     pBuffer = 0;
    int         nLen = 0;

    nLen = ComboBox_GetLBTextLen(this->m_pParent.GetHandle(), m_nIndex);

    if (nLen > 0)
    {
        pBuffer = new TCHAR[nLen + 1]; //+1 for '\0'

        if (CB_ERR == ComboBox_GetLBText(this->m_pParent.GetHandle(), m_nIndex, pBuffer)) {
            MSG_ERROR(TEXT("ComboBox_GetLBText failed with error = 0x%X"), GetLastError());
        }
        else {
            result = thString(pBuffer, nLen);
        }

        delete[] pBuffer;
    }

    TH_LEAVE_FUNCTION;
    return result;
}