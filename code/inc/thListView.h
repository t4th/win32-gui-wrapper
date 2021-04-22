#pragma once
#include "thWindow.h"
#include "thListViewColumn.h"
#include "thListViewItem.h"
#include "thItemList.h"

class thListView : public thWindow
{
public:
    enum class eViewType_t
    {
        view_details,
        view_icon,
        view_list,
        view_smallicon,
        view_tile
    };
protected:
    virtual int                     getDebugIndex();
private:
    static int                      m_indexPool;

    LRESULT                         processCommandMessage( HWND, UINT, WPARAM, LPARAM);
    LRESULT                         processNotifyMessage( HWND, UINT, WPARAM, LPARAM);
public:
                                    thListView() = delete;
                                    thListView( thWindow * a_pParent, int a_posX = CW_USEDEFAULT, int a_posY = CW_USEDEFAULT);
                                    ~thListView() = default;

    thItemList< thListViewColumn, thListView>    Columns;
    thItemList< thListViewItem, thListView>      Items;

    void                            SetView( eViewType_t a_eViewType);
};