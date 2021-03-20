#pragma once
#include "thTypes.h"
#include "thConfig.h"

class thWindow;

class thFont
{
protected:
private:
    const thWindow *    m_pParent{nullptr};
    LOGFONT             m_sLogicalFont{0}; // handle to a logical font
    HFONT               m_sFont{NULL};

    void                set(void);
public:
                        thFont();
    virtual             ~thFont();

    // public interface
    void                SetName(thString);
    void                SetSize(uint32_t);

    // non public interface
    void                SetParent(const thWindow * const);
};

