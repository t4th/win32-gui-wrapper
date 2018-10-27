#pragma once

#include "thTypes.h"
#include "thConfig.h"

class thWindow;

/*
    http://stackoverflow.com/questions/4122214/why-operator-doesnt-get-inherited-from-a-template-class
*/
template <typename T>
class thParam
{
    // share setParent
    friend class    thWindow;
    friend class    thForm;
    friend class    thButton;
protected:
    thWindow *      m_pParent; // dont set it explicitly. Use setParent instead.

    inline virtual const T getValue(void) const = 0;
    inline virtual void    setValue(T) = 0;

    void setParent(thWindow * a_pParent) {
        TH_ENTER_FUNCTION;
        if (a_pParent && (NULL == m_pParent)) {
            this->m_pParent = a_pParent;
        }
        TH_LEAVE_FUNCTION;
    }
private:
public:
                    thParam() : m_pParent(NULL) {}
                    ~thParam() {}

    inline thParam & operator=(const T & a_arg)
    {
        this->setValue(a_arg);

        return *this;
    }
    inline thParam & operator=(const thParam & a_arg)
    {
        T newVal = a_arg.getValue();
        this->setValue(newVal);
 
        return *this;
    }
 
    inline T  operator+(const T & a_arg)
    {
        T val = this->getValue() + a_arg;
        return  val;
    }

    inline thParam & operator+=(const T & a_arg)
    {
        T newValue = this->getValue() + a_arg;
 
        this->setValue(newValue);
 
        return *this;
    }
 
    inline thParam & operator-=(const T & a_arg)
    {
        T newValue = this->getValue() - a_arg;
 
        this->setValue(newValue);
 
        return *this;
    }
 
    inline operator T() const
    {
        T returnValue = getValue();
        return returnValue;
    }
};


/*
Generalization of win32 styles.
In this class, each style provided as TMask is of type bool, which can be read, set or clear.
Interacting is supposed to be programmer-friendly thanks to provided operator overloadings.

- TStyleType should be: GWL_EXSTYLE or GWL_STYLE
- TMask is style mask to be used
*/
template <int TStyleType, int TMask>
class thSetGetStyle : public thParam<bool_t>
{
protected:
private:
    const bool_t    getValue(void) const{
        TH_ENTER_FUNCTION;
        bool_t      fResult = false;
        LONG_PTR    lpWindowStyle = 0;

        // GetWindowLongPtr return 0 on error
        lpWindowStyle = GetWindowLongPtr(this->m_pParent->GetHandle(), TStyleType);

        if (lpWindowStyle) {
            if (TMask == (lpWindowStyle & TMask)) {
                fResult = true;
            }
        }
        else {
            MSG_WARNING(TEXT("GetWindowLongPtr failed with error: 0x%X"), GetLastError());
        }

        TH_LEAVE_FUNCTION;
        return fResult;
    }
    void            setValue(bool_t a_fResizeable)
    {
        TH_ENTER_FUNCTION;
        BOOL        fResult = FALSE;
        LONG_PTR    lpWindowStyle = 0;
        LONG_PTR    lpResult = 0;

        // GetWindowLongPtr return 0 on error
        lpWindowStyle = GetWindowLongPtr(this->m_pParent->GetHandle(), TStyleType);

        if (lpWindowStyle) {
            if (a_fResizeable) {
                lpWindowStyle |= TMask;
            }
            else {
                lpWindowStyle &= ~TMask;
            }

            // SetWindowLongPtr return 0 on error
            lpResult = SetWindowLongPtr(this->m_pParent->GetHandle(), TStyleType, lpWindowStyle);

            if (0 == lpResult) {
                MSG_WARNING(TEXT("SetWindowLongPtr failed with error: 0x%X"), GetLastError());
            }
        }
        else {
            MSG_WARNING(TEXT("GetWindowLongPtr failed with error: 0x%X"), GetLastError());
        }

        TH_LEAVE_FUNCTION;
    }
public:
    thSetGetStyle()
    {
        TH_ENTER_FUNCTION;

        TH_LEAVE_FUNCTION;
    }
    ~thSetGetStyle()
    {
        TH_ENTER_FUNCTION;

        TH_LEAVE_FUNCTION;
    }

    thSetGetStyle & operator =(const bool_t & a_arg)
    {
        TH_ENTER_FUNCTION;
        thParam<bool_t>::operator=(a_arg);

        TH_LEAVE_FUNCTION;
        return *this;
    }
    operator bool_t() const
    {
        TH_ENTER_FUNCTION;
        bool_t val = thParam<bool_t>::operator bool_t();
        TH_LEAVE_FUNCTION;
        return val;
    }
};
