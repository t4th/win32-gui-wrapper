#include "thWin32Logger.h"
#include <stdarg.h>
#include <iostream>

/* Max number of characters in line */
//#define LOGGER_MAX_CHARS 400

thWin32Logger::thWin32Logger() : m_pStream(NULL)
{
    BOOL    fReturn = FALSE;
    errno_t err = 0;

    fReturn = AllocConsole();

    if (0 != fReturn) {
        err = freopen_s(&m_pStream, "CONIN$", "r", stdin);
        err |= freopen_s(&m_pStream, "CONOUT$", "w", stdout);
        err |= freopen_s(&m_pStream, "CONOUT$", "w", stderr);

        if (0 != err) {
            fclose(m_pStream);
            m_pStream = NULL;
        }
        else {
            std::wcout << "WIN32_Logger Initialized Succesfully!" << std::endl;
        }
    }
}


thWin32Logger::~thWin32Logger()
{
    if (m_pStream) {
        fclose(m_pStream);
    }
}

void thWin32Logger::Printf(const TCHAR * format, ...)
{
    va_list args;
    HANDLE  hConsole = 0;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, thWin32Logger::EN_COLORS::WHITE);

    va_start(args, format);
    _vtprintf(format, args);
    va_end(args);

    std::wcout << std::endl;
}

void thWin32Logger::ErrorPrintf(const TCHAR * format, ...)
{
    va_list args;
    HANDLE  hConsole = 0;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, thWin32Logger::EN_COLORS::RED);

    va_start(args, format);
    _vtprintf(format, args);
    va_end(args);

    std::wcout << std::endl;
}

void thWin32Logger::WarningPrintf(const TCHAR * format, ...)
{
    va_list args;
    HANDLE  hConsole = 0;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, thWin32Logger::EN_COLORS::YELLOW);

    va_start(args, format);
    _vtprintf(format, args);
    va_end(args);

    std::wcout << std::endl;
}

void thWin32Logger::SuccessPrintf(const TCHAR * format, ...)
{
    va_list args;
    HANDLE  hConsole = 0;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, thWin32Logger::EN_COLORS::GREEN);

    va_start(args, format);
    _vtprintf(format, args);
    va_end(args);

    std::wcout << std::endl;
}
