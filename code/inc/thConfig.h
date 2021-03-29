#pragma once
#include "thTypes.h"
#include "thWin32Logger.h"

/* Debug levels:
2 - all logs (messages and enter/leave functions)
1 - only messages
0 - no logs
*/
#define TH_DEBUG_LEVEL 0

/* Defines */

/* Log and debug */
#if TH_DEBUG_LEVEL > 0
#pragma comment( linker, "/subsystem:console" )
#define _CONSOLE

#define MSG_SUCCESS    thWin32Logger::SuccessPrintf
#define MSG_WARNING    thWin32Logger::WarningPrintf
#define MSG_ERROR      thWin32Logger::ErrorPrintf
#define MSG_LOG        thWin32Logger::Printf
#else
#pragma comment( linker, "/subsystem:windows" )
#define MSG_SUCCESS
#define MSG_WARNING
#define MSG_ERROR
#define MSG_LOG
#endif

#if TH_DEBUG_LEVEL > 1
#define TH_ENTER_OBJECT_FUNCTION thWin32Logger::PrintfObjectEnter( _T( "%s::%s - "), this->m_name.c_str(), _T(__FUNCTION__)); thString __name__ = this->m_name;
#define TH_LEAVE_OBJECT_FUNCTION thWin32Logger::PrintfObjectLeave( _T( "%s::%s - "), __name__.c_str(), _T(__FUNCTION__))

#define TH_ENTER_FUNCTION thWin32Logger::PrintfObjectEnter( _T( "%s - "), _T( __FUNCTION__))
#define TH_LEAVE_FUNCTION thWin32Logger::PrintfObjectLeave( _T( "%s - "), _T( __FUNCTION__))

#else
#define TH_ENTER_OBJECT_FUNCTION 
#define TH_LEAVE_OBJECT_FUNCTION 

#define TH_ENTER_FUNCTION
#define TH_LEAVE_FUNCTION
#endif

/* Options */
#define ID_POOL_START        100
#define TH_ID_MDI_FIRSTCHILD 50000

/* Default application font settings */
#define TH_DEF_APP_FONT_NAME TEXT("Calibri")

/* Size in Points */
#define TH_DEF_APP_FONT_SIZE 9