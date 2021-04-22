#pragma once
#include "thTypes.h"
#include "thConfig.h"
#include "thWindow.h"

class thListView;

class thListViewColumn
{
protected:
private:
    thListView &        m_Parent;
    thString            m_text;
    int                 m_index;

public:
                        thListViewColumn() = delete;
                        thListViewColumn( thListView &, const thString &);
                        ~thListViewColumn() = default;

    int                 GetIndex() const;
    //void                Text() {}
};