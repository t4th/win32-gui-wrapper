#pragma once
#include "thTypes.h"
#include "thConfig.h"

class thListBox;
class thListBoxItem;

class thListBoxItems
{
    friend class            thListBox; // share setParent
protected:
private:
    const thListBox *       m_pParent; // dont set it explicitly. Use SetParent instead.

    void                    setParent(thListBox const * const);
public:
                            thListBoxItems();
                            ~thListBoxItems();

    thListBoxItem           operator[] (int);
    void                    Add(const thString &);
    void                    Remove(int);
    int                     GetCount(void) const;
    int                     ItemIndex(void) const;
    void                    SetItemIndex(int);
};

// ListView SubItem abstraction. Single instance - single use.
class thListBoxItem
{
protected:
private:
    const thListBox *       m_pParent; // dont set it explicitly. Use SetParent instead.
    thString                m_text;
    int                     m_nIndex;

                            thListBoxItem(void);
public:
                            thListBoxItem(const thListBox *, int);
                            ~thListBoxItem(void);

    void                    Remove(void);
    thString                GetText(void);
};