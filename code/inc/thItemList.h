#pragma once

#include "thTypes.h"
#include "thConfig.h"
#include "thWindow.h"

template < class Titem, class Tparent>
class thItemList
{
protected:
private:
    Tparent &           m_pParent;
    std::vector< std::unique_ptr< Titem>> m_items;
public:
    thItemList() = delete;

    thItemList( Tparent & a_pParent) : m_pParent (a_pParent)
    {
    }

    ~thItemList() = default;

    Titem & operator[] ( int a_nIndex)
    {
        return *m_items.at( a_nIndex);
    }

    void Add( const thString & a_itemName)
    {
        m_items.push_back( std::make_unique< Titem>( m_pParent, a_itemName));
    }

    void Remove( int a_nIndex)
    {
        m_items.erase( a_nIndex);
    }

    size_t  GetCount() const
    {
        return m_items.size();
    }
};

// ItemList2 is a list for Items. It creates copy of an Items.
template < class Titem>
class thItemList2
{
protected:
private:
    std::vector< Titem> m_items;
public:
    thItemList2() = default;
    ~thItemList2() = default;

    Titem & operator[] (int a_nIndex)
    {
        return m_items.at(a_nIndex);
    }

    void Add( Titem & a_item)
    {
        m_items.push_back(a_item);
    }

    void Remove( int a_nIndex)
    {
        m_items.erase( a_nIndex);
    }

    size_t GetCount() const
    {
        return m_items.size();
    }
};