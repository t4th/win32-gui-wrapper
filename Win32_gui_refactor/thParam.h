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
    friend class    thWindow; // share setParent
    friend class    thForm; // share setParent
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