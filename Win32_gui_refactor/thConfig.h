#pragma once
#include "thTypes.h"
#include "thWin32Logger.h"

#ifndef UNICODE
#error define UNICODE in project settigs!
#endif

/* Debug levels:
2 - all logs (messages and enter/leave functions)
1 - only messages
0 - no logs
*/
#define TH_DEBUG_LEVEL 1

/* Defines */

/* Log and debug */
#if TH_DEBUG_LEVEL > 0
#define MSG_SUCCESS    thWin32Logger::SuccessPrintf
#define MSG_WARNING    thWin32Logger::WarningPrintf
#define MSG_ERROR      thWin32Logger::ErrorPrintf
#define MSG_LOG        thWin32Logger::Printf
#else
#define MSG_SUCCESS
#define MSG_WARNING
#define MSG_ERROR
#define MSG_LOG
#endif

#if TH_DEBUG_LEVEL > 1
#define TH_ENTER_FUNCTION thWin32Logger::Printf("%s - Enter", __FUNCTION__)
#define TH_LEAVE_FUNCTION thWin32Logger::Printf("%s - Leave", __FUNCTION__)
#else
#define TH_ENTER_FUNCTION
#define TH_LEAVE_FUNCTION
#endif

/* Options */
#define TH_ID_MDI_FIRSTCHILD 50000

/* Default application font settings */
#define TH_DEF_APP_FONT_NAME L"Microsoft Sans Serif"
/* Size in Points */
#define TH_DEF_APP_FONT_SIZE 9