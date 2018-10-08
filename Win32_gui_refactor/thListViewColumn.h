#pragma once
#include "thTypes.h"
#include "thConfig.h"
#include "thWindow.h"

class thListView;

class thListViewColumn
{
protected:
private:
    const thListView *  m_pParent; // set in c-tor
    thString            m_text;
    int                 m_index;

                        thListViewColumn();
public:
                        thListViewColumn(const thListView *, const thString &);
                        ~thListViewColumn();

    int                 GetIndex(void) const;
    //void                Text() {}
};