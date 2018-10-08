#pragma once
#include "thMenuCommon.h"

class thMenu : public thMenuCommon
{
    friend class        thMenuItem;
    friend class        thForm;
private:
    /* index used to numerate this type of Window - it only
    goes uppward, even if some elements are deleted */
    static uint32_t     u32Index;
protected:
public:
                        thMenu(void);
                        ~thMenu(void);
};
