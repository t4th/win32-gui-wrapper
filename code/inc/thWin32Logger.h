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

    static void     Printf(const TCHAR *, ...);
    static void     ErrorPrintf(const TCHAR *, ...);
    static void     WarningPrintf(const TCHAR *, ...);
    static void     SuccessPrintf(const TCHAR *, ...);

    static void     PrintfObjectEnter(const TCHAR *, ...);
    static void     PrintfObjectLeave(const TCHAR *, ...);
};
