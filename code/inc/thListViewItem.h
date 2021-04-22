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
    thListViewItem &        m_Parent;
    thString                m_text;

public:
                            thListViewSubItemList( thListViewItem & a_Parent);
                            ~thListViewSubItemList() = default;

    thListViewSubItem       operator[] (int);
};

//  ListView SubItem abstraction. Instances are kept in ListView ItemList template.
class thListViewItem
{
protected:
private:
    thListView &            m_Parent;
    thString                m_text;
    int                     m_index;
public:

                            thListViewItem() = delete;
                            thListViewItem( thListView & a_Parent, const thString & a_itemText);

    thListView &            GetParent() const;
    int                     GetIndex() const;

    thListViewSubItemList   SubItems;

    void                    SetText( const thString & a_itemText);

#if 0
    thString                GetText();
#endif
};


// ListView SubItem abstraction. Single instance - single use.
class thListViewSubItem
{
protected:
private:
    thListViewItem &        m_Parent;
    thString                m_text;
    int                     m_nParentColumn;
public:
                            thListViewSubItem() = delete;
                            thListViewSubItem( thListViewItem & a_Parent, int a_nColumn);

    void                    SetText( const thString & a_itemText);

#if 0
    thString                GetText();
#endif
};