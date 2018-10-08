#pragma once
#include "thTypes.h"
#include "thConfig.h"

class thWindow;

class thFont
{
protected:
private:
    const thWindow *    m_pParent;
    LOGFONT             m_sLogicalFont; // handle to a logical font
    HFONT               m_sFont;

    void                set(void);
public:
                        thFont();
    virtual             ~thFont();

    // public interface
    //Name
    void                SetName(thString);
    //Size
    void                SetSize(uint32_t);

    // non public interface
    void                SetParent(const thWindow * const);
};

