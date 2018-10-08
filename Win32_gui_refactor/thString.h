#pragma once
#include "thTypes.h"
#include <string>

typedef std::wstring thString;

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

