#pragma once
#include <Windows.h>
#include <Windowsx.h>
#include <Commctrl.h> /* win32 common controls interface */

#pragma comment(lib, "Comctl32.lib") /* win32 common controls */

#include <vector>

/* String type */
#include "thString.h"

/* Types */
typedef bool                bool_t;

typedef unsigned long long  uint64_t;
typedef long long           sint64_t;

typedef unsigned int        uint32_t;
typedef int                 sint32_t;

typedef unsigned short      uint16_t;
typedef short               sint16_t;

typedef unsigned char       uint8_t;
typedef char                sint8_t;