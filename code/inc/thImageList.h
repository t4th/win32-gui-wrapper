#pragma once
#include "thObject.h"

// http://msdn.microsoft.com/en-us/library/windows/desktop/ms648051%28v=vs.85%29.aspx#_win32_Creating_an_Icon
// http://msdn.microsoft.com/en-us/library/windows/desktop/ms648059%28v=vs.85%29.aspx

/*
class thFile
{
protected:
    HANDLE  m_hHandle;
private:
public:
    thFile() : m_hHandle(NULL) {}
    ~thFile() {}
};

class thIcon : public thFile
{

};

https://learn.microsoft.com/en-us/windows/win32/menurc/icons
*/

class thImageList : public thObject
{
private:
    /* index used to numerate this type of Window - it only
    goes uppward, even if some elements are deleted */
    static uint32_t     u32Index;

    HIMAGELIST          m_hListHandle;
protected:
public:
                        thImageList(void);
                        ~thImageList(void);

#if _WIN32 || _WIN64
                        //windowns only
    //void                AddIcon(int, thIcon &);
#endif
};
