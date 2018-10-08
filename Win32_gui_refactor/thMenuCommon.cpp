
//http://www.functionx.com/win32/Lesson07.htm
#include "thMenuCommon.h"

/* thMenuCommon */
thMenuCommon::thMenuCommon() : m_hMenuHandle(NULL), m_hParentWindow(NULL)
{
    TH_ENTER_FUNCTION;
    this->Items.setParent(this);
    TH_LEAVE_FUNCTION;
}

thMenuCommon::~thMenuCommon()
{
    TH_ENTER_FUNCTION;
    // if menu has no parent it need to be destroyed manually
    DestroyMenu(this->m_hMenuHandle);

    TH_LEAVE_FUNCTION;
}

// wParam - the zero - based index of the item selected
// lParam - a handle to the menu for the item selected
LRESULT thMenuCommon::processCommandMessage(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    TH_ENTER_FUNCTION;
    LRESULT         tResult = 0;
    UINT            uItemID = 0xFFFFFFFFul;
    thMenuItem *    pFoundChildren = NULL;

    uItemID = LOWORD(a_wParam);

    pFoundChildren = this->Items.findItemById(uItemID);

    if (pFoundChildren) {
        tResult = 1; // always return 1 if children has been found (since all windows has unique Id)
        pFoundChildren->processCommandMessage(a_uMsg, a_wParam, a_lParam);
    }
#if 0
    uItemID = GetMenuItemID(
        (HMENU)a_lParam,
        (UINT)a_wParam
        );

    if (uItemID != -1 && uItemID != NULL) {
        thMenuItem * pFoundChildren = NULL;

        pFoundChildren = this->Items.findItemById(uItemID);

        if (pFoundChildren) {
            tResult = 1; // always return 1 if children has been found (since all windows has unique Id)
            pFoundChildren->processCommandMessage(a_uMsg, a_wParam, a_lParam);
        }
    }
#endif

    TH_LEAVE_FUNCTION;
    return tResult;
}

/* thMenuText */

thMenuText::thMenuText(){
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

thMenuText::~thMenuText(){
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

void thMenuText::setParent(thMenuItem * a_pParent) {
    TH_ENTER_FUNCTION;
    m_pParent = a_pParent;
    TH_LEAVE_FUNCTION;
}

void thMenuText::operator=(thString a_newText) {
    TH_ENTER_FUNCTION;
    BOOL fResult = FALSE;
    UINT uFlags = MF_STRING | MF_BYCOMMAND;

    if (a_newText.compare(L"-") == 0) {
        uFlags |= MF_SEPARATOR;
    }

    fResult = ModifyMenu(
        this->m_pParent->m_pParent->m_hMenuHandle,
        (UINT)this->m_pParent->m_id,
        uFlags,
        (UINT_PTR)this->m_pParent->m_id,
        a_newText.c_str()
        );

    if (FALSE == fResult) {
        MSG_ERROR(L"ModifyMenu failed with error = 0x%X", GetLastError());
    }

    TH_LEAVE_FUNCTION;
}

/* thMenuSubMenu */

thMenuSubMenu::thMenuSubMenu(){
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

thMenuSubMenu::~thMenuSubMenu(){
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

void thMenuSubMenu::setParent(thMenuItem * a_pParent) {
    TH_ENTER_FUNCTION;
    m_pParent = a_pParent;
    TH_LEAVE_FUNCTION;
}

void thMenuSubMenu::operator=(thMenuCommon * a_pSubMenu) {
    TH_ENTER_FUNCTION;
    if (this->m_pParent->m_pParent != a_pSubMenu && a_pSubMenu) {
        BOOL            fResult = FALSE;
        UINT            uFlags = MF_BYCOMMAND | MF_ENABLED;
        MENUITEMINFO    itemInfo = { 0 };
        WCHAR *         pBuffer = NULL;

        // Retrieving menu item TEXT according to Remarks section in
        // http://msdn.microsoft.com/en-us/library/windows/desktop/ms647980%28v=vs.85%29.aspx
        itemInfo.cbSize = sizeof(MENUITEMINFO);
        itemInfo.fMask = MIIM_STRING;

        // get buffer size
        fResult = GetMenuItemInfo(
            this->m_pParent->m_pParent->m_hMenuHandle,
            (UINT)this->m_pParent->m_id,
            FALSE,
            &itemInfo
            );

        if (itemInfo.cch) {
            itemInfo.cch++;
            pBuffer = new WCHAR[itemInfo.cch];
        }

        itemInfo.dwTypeData = pBuffer;

        if (fResult) {
            fResult = FALSE;

            fResult = GetMenuItemInfo(
                this->m_pParent->m_pParent->m_hMenuHandle,
                (UINT)this->m_pParent->m_id,
                FALSE,
                &itemInfo
                );

            if (fResult) {
                // modify menu item
                UINT_PTR pPopupMenu = NULL;
                fResult = FALSE;

                if (a_pSubMenu) {
                    uFlags |= MF_POPUP;
                    pPopupMenu = (UINT_PTR)a_pSubMenu->m_hMenuHandle;
                }

                fResult = ModifyMenu(
                    this->m_pParent->m_pParent->m_hMenuHandle,
                    (UINT)this->m_pParent->m_id,
                    uFlags,
                    pPopupMenu,
                    itemInfo.dwTypeData
                    );

                if (fResult) {
                    this->m_pParent->m_popupMenu = a_pSubMenu;

                    DrawMenuBar(this->m_pParent->m_pParent->m_hParentWindow);
                }
                else {
                    MSG_ERROR(L"ModifyMenu failed with error = 0x%X", GetLastError());
                }
            }
            else {
                MSG_ERROR(L"GetMenuItemInfo failed with error = 0x%X", GetLastError());
            }
        }
        else {
            MSG_ERROR(L"GetMenuItemInfo failed with error = 0x%X", GetLastError());
        }

        if (pBuffer) {
            delete [] pBuffer;
        }
    }

    TH_LEAVE_FUNCTION;
}

/* thMenuItem */

thMenuItem::thMenuItem() {
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

thMenuItem::thMenuItem(thMenuCommon * a_parent) : m_pParent(a_parent), m_popupMenu(NULL), OnClick(NULL) {
    TH_ENTER_FUNCTION;
    this->Text.setParent(this);
    this->SubMenu.setParent(this);

    TH_LEAVE_FUNCTION;
}

thMenuItem::~thMenuItem() {
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
}

LRESULT thMenuItem::processCommandMessage(UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    TH_ENTER_FUNCTION;
    LRESULT tResult = 0; // should return 1 if used by app

    MSG_LOG(L"Menu Item CLICKED, Id=%d", this->m_id);

    // for now: only button click event
    if (NULL != OnClick) {
        OnClick(this, { a_uMsg, a_wParam, a_lParam });
    }

    TH_LEAVE_FUNCTION;
    return tResult;
}

LRESULT thMenuItem::processNotifyMessage(UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    TH_ENTER_FUNCTION;
    LRESULT tResult = 0;
    NMHDR * pData = 0;

    pData = reinterpret_cast<NMHDR*>(a_lParam);

    //MSG_LOG(L"%s - Not supported %X", this->m_name.c_str(), pData->code);

    TH_LEAVE_FUNCTION;
    return tResult;
}

/* thMenuItemList */

thMenuItemList::thMenuItemList() : m_pParent(0), LastIndex(-1){
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
}

thMenuItemList::~thMenuItemList() {
    TH_ENTER_FUNCTION;
    std::vector<thMenuItem*>::iterator i;

    i = this->m_items.begin();

    for (; i != this->m_items.end(); i++) {
        delete *i;
    }

    TH_LEAVE_FUNCTION;
}

thMenuItem * thMenuItemList::findItemById(UINT_PTR a_searchedId) {
    //TH_ENTER_FUNCTION;
    std::vector<thMenuItem*>::iterator i;
    thMenuItem * pFoundItem = NULL;

    i = this->m_items.begin();

    for (; i != m_items.end(); i++) {
        if (a_searchedId == reinterpret_cast<WORD>((*i)->m_id)) {
            pFoundItem = (*i);
            break;
        }
        else if ((*i)->m_popupMenu) {
            pFoundItem = (*i)->m_popupMenu->Items.findItemById(a_searchedId);
            if (pFoundItem) {
                break;
            }
        }
    }

    //TH_LEAVE_FUNCTION;
    return pFoundItem;
}

void thMenuItemList::setParent(thMenuCommon * a_parent) {
    TH_ENTER_FUNCTION;
    m_pParent = a_parent;
    TH_LEAVE_FUNCTION;
}

// add new menu item.
// If provided name is "-", new item will be created as separator
void thMenuItemList::Add(thString a_newText) {
    TH_ENTER_FUNCTION;
    if (0 != m_pParent) {
        thMenuItem * newMenu = NULL;
        
        newMenu = new thMenuItem(m_pParent);

        if (newMenu) {
            BOOL    fResult = FALSE;
            UINT    uFlags = MF_BYCOMMAND | MF_ENABLED;
            LPCTSTR lpNewItem = NULL;

            MSG_SUCCESS(L"Menu item created with id=%d", newMenu->m_id);
            m_items.push_back(newMenu);
            LastIndex++;

            if (a_newText.compare(L"-") == 0) {
                uFlags |= MF_SEPARATOR;
            }
            else {
                uFlags |= MF_STRING;
                lpNewItem = a_newText.c_str();
            }


            fResult = AppendMenu(
                this->m_pParent->m_hMenuHandle,
                uFlags,
                (UINT)newMenu->m_id,
                lpNewItem
                );

            if (fResult) {
                // update menu in window
                DrawMenuBar(this->m_pParent->m_hParentWindow);
            }
            else {
                MSG_ERROR(L"AppendMenu failed with error = 0x%X", GetLastError());
            }
        }
    }
    TH_LEAVE_FUNCTION;
}

#if 0
void thMenuItemList::Remove(uint32_t a_u32ItemIndex) {
    TH_ENTER_FUNCTION;
    if (a_u32ItemIndex < m_items.size()) {
        std::vector<thMenuItem *>::iterator i;

        if (m_items.at(a_u32ItemIndex)) {
            //todo: remove item from menu
            delete m_items.at(a_u32ItemIndex);
        }

        i = m_items.begin() + a_u32ItemIndex; //unsigned to iterator workaround
        m_items.erase(i);
        LastIndex--;

        fResult = RemoveMenu(
            this->m_pParent->m_pParent->m_hMenuHandle,
            (UINT)this->m_pParent->m_popupMenu->m_id,
            MF_BYCOMMAND
            );

        if (FALSE == fResult){
            MSG_ERROR(L"RemoveMenu failed with error = 0x%X", GetLastError());
        }

        // update menu in window
        if (this->m_pParent->m_hParentWindow) {
            DrawMenuBar(this->m_pParent->m_hParentWindow);
        }
    }
    TH_LEAVE_FUNCTION;
}
#endif

uint32_t thMenuItemList::Size() {
    TH_ENTER_FUNCTION;
    uint32_t u32Size = 0;

    u32Size = this->m_items.size();

    TH_LEAVE_FUNCTION;
    return u32Size;
}

thMenuItem * const thMenuItemList::operator[](uint32_t a_u32ItemIndex) {
    TH_ENTER_FUNCTION;
    thMenuItem * p = 0;
    if (a_u32ItemIndex < m_items.size()) {
        if (m_items.at(a_u32ItemIndex)) {
            p = m_items.at(a_u32ItemIndex);
        }
    }
    TH_LEAVE_FUNCTION;
    return p;
}