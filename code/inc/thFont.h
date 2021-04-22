#pragma once
#include "thTypes.h"
#include "thConfig.h"

class thWindow;

class thFont
{
protected:
private:
    thWindow &          m_Parent;
    LOGFONT             m_sLogicalFont{ 0}; // handle to a logical font
    HFONT               m_sFont{ NULL};

    void                set();
public:
                        thFont() = delete;
                        thFont( thWindow & a_Parent);
    virtual             ~thFont();

    // public interface
    void                SetName( const thString & a_fontName);
    void                SetSize( uint32_t a_newsize);
};

