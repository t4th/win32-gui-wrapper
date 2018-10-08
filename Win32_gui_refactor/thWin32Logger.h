#pragma once
#include "thLogger.h"

class thWin32Logger : public thLogger
{
protected:
private:
    FILE *      m_pStream;

    enum        EN_COLORS {
                    DARKBLUE = 1,
                    DARKGREEN,
                    DARKTEAL,
                    DARKRED,
                    DARKPINK,
                    DARKYELLOW,
                    GRAY,
                    DARKGRAY,
                    BLUE,
                    GREEN,
                    TEAL,
                    RED,
                    PINK,
                    YELLOW,
                    WHITE
                };
public:
                    thWin32Logger();
                    ~thWin32Logger();

    static void     Printf(const wchar_t *, ...);
    static void     Printf(const char *, ...);
    static void     ErrorPrintf(const wchar_t *, ...);
    static void     WarningPrintf(const wchar_t *, ...);
    static void     SuccessPrintf(const wchar_t *, ...);
};
