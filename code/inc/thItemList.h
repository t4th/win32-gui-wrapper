#pragma once

#include "thTypes.h"
#include "thConfig.h"
#include "thWindow.h"

// ItemList for Items that need to know its parent (in most cases, children that need parent Handle).
// Item must have Titem(Tparent, thString) c-tor. Items are allocated dynamiclly.
// TODO: removed 'new' and 'delete'. Make operator[] return reference instead of pointer
template <class Titem, class Tparent>
class thItemList
{
protected:
private:
    const Tparent * m_pParent;
    std::vector<Titem*> m_items;
public:
    void SetParent(const Tparent * a_pParent)
    {
        m_pParent = a_pParent;
    }

    thItemList()
    {

    }

    ~thItemList()
    {
        typename std::vector<Titem*>::iterator i;

        i = m_items.begin();

        for (; i != m_items.end(); i++) {
            delete (*i);
        }
    }

    Titem * operator[] (int a_nIndex)
    {
        return m_items.at(a_nIndex);
    }

    void Add(const thString & a_itemName)
    {
        Titem * pNewItem = NULL;

        pNewItem = new Titem(m_pParent, a_itemName);

        if (pNewItem) {
            m_items.push_back(pNewItem);
        }
        else {
            MSG_ERROR(TEXT("new failed"));
        }
    }

    void Remove(int a_nIndex)
    {
        delete m_items.at(a_nIndex);
        m_items.erase(a_nIndex);
    }

    size_t  GetCount(void) const
    {
        return m_items.size();
    }
};

// ItemList2 is a list for Items. It creates copy of an Items.
template <class Titem>
class thItemList2
{
protected:
private:
    std::vector<Titem> m_items;
public:
    thItemList2()
    {
    }

    ~thItemList2()
    {
    }

    Titem & operator[] (int a_nIndex)
    {
        return m_items.at(a_nIndex);
    }

    void Add(Titem & a_item)
    {
        m_items.push_back(a_item);
    }

    void Remove(int a_nIndex)
    {
        m_items.erase(a_nIndex);
    }

    size_t  GetCount(void) const
    {
        return m_items.size();
    }
};