#pragma once
#include "thTypes.h"
#include "thConfig.h"
#include "thWindow.h"
#include "thItemList.h"

class thListView;
class thListViewSubItem;
class thListViewItem;

// subitem list
class thListViewSubItemList
{
protected:
private:
    const thListViewItem *  m_pParent; // dont set it explicitly. Use SetParent instead.
    thString                m_text;

public:
                            thListViewSubItemList();
                            ~thListViewSubItemList();

    void                    SetParent(thListViewItem *);
    thListViewSubItem       operator[] (int);
};

//  ListView SubItem abstraction. Instances are kept in ListView ItemList template.
class thListViewItem
{
protected:
private:
    const thListView *      m_pParent; // dont set it explicitly. Use SetParent instead.
    thString                m_text;
    int                     m_index;

                            thListViewItem();
public:
                            thListViewItem(const thListView *, const thString &);
                            ~thListViewItem();

    const thListView *      GetParent(void) const;
    int                     GetIndex(void) const;

    thListViewSubItemList   SubItems;

    void                    SetText(thString);
    thString                GetText(void);
};


// ListView SubItem abstraction. Single instance - single use.
class thListViewSubItem
{
protected:
private:
    const thListViewItem *  m_pParent; // dont set it explicitly. Use SetParent instead.
    thString                m_text;
    int                     m_nParentColumn;

                            thListViewSubItem(void);
public:
                            thListViewSubItem(const thListViewItem *, int);
                            ~thListViewSubItem(void);

    void                    SetText(thString);
    thString                GetText(void);
};