#include "thPopupMenu.h"

/* Defines */
#define CLASS_NAME L"thPopupMenu"
#define MAX_NUMBER_OF_DIGITS_FOR_INDEX 6

/* Static elements */
uint32_t thPopupMenu::u32Index = 1;

thPopupMenu::thPopupMenu(void)
{
    TH_ENTER_FUNCTION;
    this->m_hMenuHandle = 0;

    this->m_hMenuHandle = CreatePopupMenu();

    if (NULL != m_hMenuHandle) {
        BOOL fResult = FALSE;

        this->m_name = CLASS_NAME;

        /* 1st: set MNS_NOTIFYBYPOS style so menu commands are processed by WM_MENUCOMMAND instead of WM_COMMAND */
#if 0
        MENUINFO menuInfo = { 0 };

        menuInfo.cbSize = sizeof(menuInfo);
        fResult = GetMenuInfo(m_hMenuHandle, &menuInfo);

        if (FALSE == fResult) {
            MSG_ERROR(L"GetMenuInfo failed with error = 0x%X", GetLastError());
        }
        else {
            menuInfo.fMask = MIM_STYLE;
            menuInfo.dwStyle |= MNS_NOTIFYBYPOS;

            fResult = SetMenuInfo(m_hMenuHandle, &menuInfo);

            if (FALSE == fResult) {
                MSG_ERROR(L"SetMenuInfo failed with error = 0x%X", GetLastError());
            }
        }
#endif
        /* Create Default Menu Name */
        wchar_t szNameIndex[MAX_NUMBER_OF_DIGITS_FOR_INDEX] = { 0 }; // up to 5 digtits

        if (-1 == swprintf(szNameIndex, MAX_NUMBER_OF_DIGITS_FOR_INDEX, L"%d", u32Index)) {
            MSG_ERROR(L"CRITICAL_ERROR - window index buffer overflow");
        }
        else {
            this->m_name += thString(szNameIndex);
        }
        u32Index++;
        MSG_SUCCESS(L"Created new popup menu: %s with ID=%d", this->m_name.c_str(), this->m_id);
    }
    else {
        MSG_ERROR(L"Failed to create popup menu with CreateMenu error = 0x%X", GetLastError());
    }
    TH_LEAVE_FUNCTION;
}

thPopupMenu::~thPopupMenu(void)
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
}

void thPopupMenu::Show(HWND a_parent, POINT & a_point)
{
    TH_ENTER_FUNCTION;
    BOOL fResult = false;

    //fResult = SetForegroundWindow(a_parent->m_hWinHandle);

    //if (false == fResult) {
    //    MSG_ERROR(L"SetForegroundWindow failed with error = 0x%X", GetLastError());
    //}

    //fResult = false;
    fResult = TrackPopupMenu(
        this->m_hMenuHandle,
        TPM_TOPALIGN | TPM_LEFTALIGN,
        a_point.x,
        a_point.y,
        0,
        a_parent,
        NULL);

    if (false == fResult) {
        MSG_ERROR(L"TrackPopupMenu failed with error = 0x%X", GetLastError());
    }
    TH_LEAVE_FUNCTION;
}

void thPopupMenu::Show(thWindow * a_parent, POINT & a_point)
{
    TH_ENTER_FUNCTION;
    BOOL fResult = false;

    if (a_parent) {
        //fResult = SetForegroundWindow(a_parent->m_hWinHandle);

        //if (false == fResult) {
        //    MSG_ERROR(L"SetForegroundWindow failed with error = 0x%X", GetLastError());
        //}

        //fResult = false;
        fResult = TrackPopupMenu(
            this->m_hMenuHandle,
            TPM_TOPALIGN | TPM_LEFTALIGN,
            a_point.x,
            a_point.y,
            0,
            a_parent->GetHandle(),
            NULL);

        if (false == fResult) {
            MSG_ERROR(L"TrackPopupMenu failed with error = 0x%X", GetLastError());
        }
    }
    else {
        MSG_ERROR(L"Empty pointer as argument!");
    }
    TH_LEAVE_FUNCTION;
}