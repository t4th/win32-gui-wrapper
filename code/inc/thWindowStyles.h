#pragma once
#include "thSetGetStyle.h"

/*
        thForm window styles
*/

/*
 thResizable can be used to check if Window is resizable and to turn on/off resizablity.

 Example:
 if (thForm->Resizable) {}
 thForm->Resizable = true; thForm->Resizable = false;
*/
class thResizable : public thSetGetStyle < GWL_STYLE, WS_THICKFRAME >
{
public:
    thResizable() = delete;

    thResizable(thWindow & a_pParent) : thSetGetStyle(a_pParent)
    {}

    ~thResizable() {}
    thResizable & operator =(const bool_t & a_arg)
    {
        TH_ENTER_FUNCTION;
        thSetGetStyle < GWL_STYLE, WS_THICKFRAME >::operator=(a_arg);
        TH_LEAVE_FUNCTION;
        return *this;
    }
    operator bool_t() const
    {
        bool_t fResult = thSetGetStyle < GWL_STYLE, WS_THICKFRAME >::operator bool_t();
        TH_ENTER_FUNCTION;
        TH_LEAVE_FUNCTION;
        return fResult;
    }
};

/*
        thButton window styles (BS prefix - Button Style)
*/

// Change push button style to DefaultPushButton
class thBS_DefaultPush : public thSetGetStyle < GWL_STYLE, BS_DEFPUSHBUTTON >
{
public:
    thBS_DefaultPush() = delete;

    thBS_DefaultPush(thWindow & a_pParent) :
        thSetGetStyle(a_pParent)
    {}

    ~thBS_DefaultPush() {}
    thBS_DefaultPush & operator =(const bool_t & a_arg)
    {
        TH_ENTER_FUNCTION;
        thSetGetStyle < GWL_STYLE, BS_DEFPUSHBUTTON >::operator=(a_arg);
        TH_LEAVE_FUNCTION;
        return *this;
    }
    operator bool_t() const
    {
        bool_t fResult = thSetGetStyle < GWL_STYLE, BS_DEFPUSHBUTTON >::operator bool_t();
        TH_ENTER_FUNCTION;
        TH_LEAVE_FUNCTION;
        return fResult;
    }
};

#if 0
// Places text at the bottom of the button rectangle.
class thBS_Bottom : public thSetGetStyle < GWL_STYLE, BS_BOTTOM >
{
public:
    thBS_Bottom() {}
    ~thBS_Bottom() {}
    thBS_Bottom & operator =(const bool_t & a_arg)
    {
        TH_ENTER_FUNCTION;
        thSetGetStyle < GWL_STYLE, BS_BOTTOM >::operator=(a_arg);
        TH_LEAVE_FUNCTION;
        return *this;
    }
    operator bool_t() const
    {
        bool_t fResult = thSetGetStyle < GWL_STYLE, BS_BOTTOM >::operator bool_t();
        TH_ENTER_FUNCTION;
        TH_LEAVE_FUNCTION;
        return fResult;
    }
};

// Centers text horizontally in the button rectangle.
class thBS_Center : public thSetGetStyle < GWL_STYLE, BS_CENTER >
{
public:
    thBS_Center() {}
    ~thBS_Center() {}
    thBS_Center & operator =(const bool_t & a_arg)
    {
        TH_ENTER_FUNCTION;
        thSetGetStyle < GWL_STYLE, BS_CENTER >::operator=(a_arg);
        TH_LEAVE_FUNCTION;
        return *this;
    }
    operator bool_t() const
    {
        bool_t fResult = thSetGetStyle < GWL_STYLE, BS_CENTER >::operator bool_t();
        TH_ENTER_FUNCTION;
        TH_LEAVE_FUNCTION;
        return fResult;
    }
};

// Specifies that the button is two-dimensional; it does not use the default shading to create a 3-D image. 
class thBS_Flat : public thSetGetStyle < GWL_STYLE, BS_FLAT >
{
public:
    thBS_Flat() {}
    ~thBS_Flat() {}
    thBS_Flat & operator =(const bool_t & a_arg)
    {
        TH_ENTER_FUNCTION;
        thSetGetStyle < GWL_STYLE, BS_FLAT >::operator=(a_arg);
        TH_LEAVE_FUNCTION;
        return *this;
    }
    operator bool_t() const
    {
        bool_t fResult = thSetGetStyle < GWL_STYLE, BS_FLAT >::operator bool_t();
        TH_ENTER_FUNCTION;
        TH_LEAVE_FUNCTION;
        return fResult;
    }
};

// Wraps the button text to multiple lines if the text string is too long to fit on a single line in the button rectangle.
class thBS_Multiline : public thSetGetStyle < GWL_STYLE, BS_MULTILINE >
{
public:
    thBS_Multiline() {}
    ~thBS_Multiline() {}
    thBS_Multiline & operator =(const bool_t & a_arg)
    {
        TH_ENTER_FUNCTION;
        thSetGetStyle < GWL_STYLE, BS_MULTILINE >::operator=(a_arg);
        TH_LEAVE_FUNCTION;
        return *this;
    }
    operator bool_t() const
    {
        bool_t fResult = thSetGetStyle < GWL_STYLE, BS_MULTILINE >::operator bool_t();
        TH_ENTER_FUNCTION;
        TH_LEAVE_FUNCTION;
        return fResult;
    }
};
#endif