#pragma once
#include "thWindow.h"
#include "thMenuCommon.h"

class thWindow;

class thPopupMenu : public thMenuCommon
{
    friend class thWindow;
    friend class thForm;
private:
    /* index used to numerate this type of Window - it only
    goes uppward, even if some elements are deleted */
    static uint32_t     u32Index;

    void                Show(HWND, POINT &);
protected:
public:
                        thPopupMenu(void);
                        ~thPopupMenu(void);

    void                Show(thWindow *, POINT &);
};

