// https://msdn.microsoft.com/en-us/library/windows/desktop/ms646829%28v=vs.85%29.aspx

#pragma once
#include "thTypes.h"
#include "thConfig.h"
#include "thItemList.h"

class thWindow;

// Generalization of all dialog boxes
class thCommonDialog
{
protected:
    HWND            m_hParent;

    virtual bool_t  show(void) = 0;
private:
public:
                    thCommonDialog();
    virtual         ~thCommonDialog();

    bool_t          Show(thWindow * const);
};

// ------------------- thOpenDialog - start -------------------
class thDialogFilterItem
{
public:
    thString    Name;      // Name displayedin in Open/save Dialog Filter ComboBox
    thString    Filter;    // Filter used for given Filter Name selected in Open/save File dialogBox

                thDialogFilterItem( thString name, thString filter) : Name(name), Filter(filter) {}
                thDialogFilterItem() = delete;
                ~thDialogFilterItem() {}
};

// Wrap Item list template so its easier to debug
class thDialogFilter : public thItemList2 < thDialogFilterItem >
{
};

// http://msdn.microsoft.com/en-us/library/windows/desktop/ms646927%28v=vs.85%29.aspx
class thOpenDialog : public thCommonDialog
{
protected:
private:
    thString        getFilterString();
    bool_t          show();
public:
                    thOpenDialog();
    virtual         ~thOpenDialog();

    /* Filter cannot be string, because default string c-tor cut input array on end-of-string sign '\0'.
       Default win32 filter is separated by '\0' so string c-tor will cut all the data after first '\0'.
    */
    thDialogFilter  Filter;
    thString        FileName;
};

// ------------------- thOpenDialog - end -------------------