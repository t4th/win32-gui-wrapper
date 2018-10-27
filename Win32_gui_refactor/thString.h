#pragma once
#include "thTypes.h"
#include <string>

#ifdef UNICODE
    typedef std::wstring thString;
    #define NumToString(__toStr__) std::to_wstring(__toStr__)
#else
    typedef std::string thString;
    #define NumToString(__toStr__) std::to_string(__toStr__)
#endif
    
std::string     WStringToString(const std::wstring& wstr);
std::wstring    StringToWString(const std::string& str);

#if 0
class thString : public std::wstring
{
public:
    thString();
    virtual ~thString();
};
#endif
