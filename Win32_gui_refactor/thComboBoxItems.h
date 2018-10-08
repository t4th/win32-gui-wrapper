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
    const thComboBox *      m_pParent; // dont set it explicitly. Use SetParent instead.

    void                    setParent(thComboBox const * const);
public:
                            thComboBoxItems();
                            ~thComboBoxItems();

    thComboBoxItem          operator[] (int);
    void                    Add(const thString &);
    void                    Remove(int);
    int                     GetCount(void) const;
    int                     ItemIndex(void) const;
    void                    SetItemIndex(int);
};

// ListView SubItem abstraction. Single instance - single use.
class thComboBoxItem
{
protected:
private:
    const thComboBox *      m_pParent; // dont set it explicitly. Use SetParent instead.
    thString                m_text;
    int                     m_nIndex;

                            thComboBoxItem(void);
public:
                            thComboBoxItem(const thComboBox *, int);
                            ~thComboBoxItem(void);

    void                    Remove(void);
    thString                GetText(void);
};