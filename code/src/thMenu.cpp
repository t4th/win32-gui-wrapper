#include "thMenu.h"

/* Defines */
#define CLASS_NAME TEXT("thMenu")
#define MAX_NUMBER_OF_DIGITS_FOR_INDEX 6

/* Static elements */
uint32_t thMenu::u32Index = 1;

/* thMenu */

thMenu::thMenu(void)
{
    TH_ENTER_FUNCTION;
    this->m_hMenuHandle = CreateMenu();

    if (NULL != m_hMenuHandle) {

        this->m_name = CLASS_NAME;

    /* 1st: set MNS_NOTIFYBYPOS style so menu commands are processed by WM_MENUCOMMAND instead of WM_COMMAND */
#if 0
        MENUINFO menuInfo = { 0 };
        BOOL fResult = FALSE;
        menuInfo.cbSize = sizeof(menuInfo);
        fResult = GetMenuInfo(m_hMenuHandle, &menuInfo);

        if (fResult) {
            menuInfo.fMask = MIM_STYLE;
            menuInfo.dwStyle |= MNS_NOTIFYBYPOS;

            fResult = SetMenuInfo(m_hMenuHandle, &menuInfo);

            if (FALSE == fResult) {
                MSG_ERROR(TEXT("SetMenuInfo failed with error = 0x%X"), GetLastError());
            }
        }
        else {
            MSG_ERROR(TEXT("GetMenuInfo failed with error = 0x%X"), GetLastError());
        }
#endif
        /* Create Default Menu Name */
        TCHAR szNameIndex[MAX_NUMBER_OF_DIGITS_FOR_INDEX] = { 0 }; // up to 5 digtits

        if (-1 == _stprintf_s(szNameIndex, MAX_NUMBER_OF_DIGITS_FOR_INDEX, TEXT("%d"), u32Index)) {
            MSG_ERROR(TEXT("CRITICAL_ERROR - window index buffer overflow"));
        }
        else {
            this->m_name += thString(szNameIndex);
        }
        u32Index++;
        MSG_SUCCESS(TEXT("Created new menu: %s with ID=%d"), this->m_name.c_str(), this->m_id);
    }
    else {
        MSG_ERROR(TEXT("Failed to create menu with CreateMenu error = 0x%X"), GetLastError());
    }

    TH_LEAVE_FUNCTION;
}

thMenu::~thMenu(void)
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
}
