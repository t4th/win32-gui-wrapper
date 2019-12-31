#pragma once
#include "thTypes.h"
#include "thConfig.h"

class thComboBox;
class thComboBoxItem;

class thComboBoxItems
{
    friend class            thComboBox; // share setParent
protected:
private:
    thComboBox &            m_pParent;

public:
                            thComboBoxItems() = delete;
                            thComboBoxItems(thComboBox & a_pParent);
                            ~thComboBoxItems();

    thComboBoxItem          operator[] (int);
    void                    Add(const thString &);
    void                    Remove(int);
    int                     GetCount(void) const;
    int                     ItemIndex(void) const;
    void                    SetItemIndex(int);
};

// ComboBox SubItem abstraction. Single instance - single use.
class thComboBoxItem
{
protected:
private:
    thComboBox &            m_pParent;
    thString                m_text;
    int                     m_nIndex;

public:
                            thComboBoxItem() = delete;
                            thComboBoxItem(thComboBox & a_Parent, int a_Item);
                            ~thComboBoxItem();

    void                    Remove(void);
    thString                GetText(void);
};