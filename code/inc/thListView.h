#pragma once
#include "thWindow.h"
#include "thListViewColumn.h"
#include "thListViewItem.h"
#include "thItemList.h"

class thListView : public thWindow
{
public:
    typedef enum
    {
        view_details =      LV_VIEW_DETAILS,
        view_icon =         LV_VIEW_ICON,
        view_list =         LV_VIEW_LIST,
        view_smallicon =    LV_VIEW_SMALLICON,
        view_tile =         LV_VIEW_TILE
    } eViewType_t;
protected:
    virtual int                     getDebugIndex();
private:
    static int                      m_indexPool;

                                    thListView();
    LRESULT                         processCommandMessage(HWND, UINT, WPARAM, LPARAM);
    LRESULT                         processNotifyMessage(HWND, UINT, WPARAM, LPARAM);
public:
                                    thListView(thWindow *, int, int);
                                    ~thListView();

    thItemList<thListViewColumn, thListView>    Columns;
    thItemList<thListViewItem, thListView>      Items;

    void                            SetView(eViewType_t);
};