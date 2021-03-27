//http://msdn.microsoft.com/en-us/library/windows/desktop/hh298386.aspx
// styles http://msdn.microsoft.com/en-us/library/windows/desktop/bb760439%28v=vs.85%29.aspx

//howto embed other windows to toolbar
//http://msdn.microsoft.com/en-us/library/windows/desktop/hh298391%28v=vs.85%29.aspx

#include "thToolbar.h"

/* Defines */
#define CLASS_NAME TEXT("thToolbar")
#define WIN32_CLASS_NAME TEXT("ToolbarWindow32")
#define DEFAULT_TEXT TEXT("Caption")

#define DEFAULT_WIDTH  CW_USEDEFAULT
#define DEFAULT_HEIGHT CW_USEDEFAULT

/* Local Memory */
int thToolbarItem::m_indexPool = 1;
int thToolbar::m_indexPool = 1;

/* Prototypes */

thToolbar::thToolbar(thWindow * a_pParent, int a_posX, int a_posY)
    :
    thWindow(a_pParent, a_posX, a_posY),
    Items(*this)
{
    TH_ENTER_FUNCTION;
    BOOL fResult = FALSE;
    INITCOMMONCONTROLSEX iccx = { 0 };

    iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
    iccx.dwICC = ICC_BAR_CLASSES;
    if (FALSE == InitCommonControlsEx(&iccx)) {
        MSG_ERROR(TEXT("InitCommonControlsEx ICC_BAR_CLASSES failed with error = 0x%X"), GetLastError());
    }

    //this->Events = { 0 };

    this->m_name = CLASS_NAME;

    this->m_sWindowArgs.dwExStyle =     0; // Comctl32, ver 6+
    this->m_sWindowArgs.lpClassName =   WIN32_CLASS_NAME;
    this->m_sWindowArgs.lpWindowName =  DEFAULT_TEXT;
    this->m_sWindowArgs.dwStyle =       WS_VISIBLE | WS_CHILD |  TBSTYLE_FLAT | TBSTYLE_TOOLTIPS; //WS_TABSTOP, CCS_NODIVIDER
    this->m_sWindowArgs.nWidth =        DEFAULT_WIDTH;
    this->m_sWindowArgs.nHeight =       DEFAULT_HEIGHT;
    this->m_sWindowArgs.hMenu =         reinterpret_cast<HMENU>(this->m_id);;
    this->m_sWindowArgs.lpParam =       this;

    this->create();

    this->Anchors.Right = true;

    fResult = SetWindowSubclass(this->m_hWinHandle, ChildWindProc, 0, (DWORD_PTR)this);

    if (FALSE == fResult) {
        MSG_ERROR(TEXT("SetWindowSubclass failed with error = 0x%X"), GetLastError());
    }

    TH_LEAVE_FUNCTION;
}

thToolbar::~thToolbar()
{
    TH_ENTER_OBJECT_FUNCTION;
    TH_LEAVE_OBJECT_FUNCTION;
}

int thToolbar::getDebugIndex()
{
    TH_ENTER_OBJECT_FUNCTION;
    int dReturn = this->m_indexPool;
    this->m_indexPool++;
    TH_LEAVE_OBJECT_FUNCTION;
    return dReturn;
}

LRESULT thToolbar::processCommandMessage(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    TH_ENTER_OBJECT_FUNCTION;
    LRESULT tResult = 0; // should return 1 if used by app

    thToolbarItem * pItem = this->Items.findItemById(LOWORD(a_wParam));

    if (pItem) {
        tResult = pItem->processCommandMessage(a_uMsg, a_wParam, a_lParam);
    }

    TH_LEAVE_OBJECT_FUNCTION;
    return tResult;
}

// http://msdn.microsoft.com/en-us/library/windows/desktop/bb760435%28v=vs.85%29.aspx
LRESULT thToolbar::processNotifyMessage(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    //TH_ENTER_OBJECT_FUNCTION;
    LRESULT tResult = 0; // should return 1 if used by app
    NMHDR * pData = 0;

    pData = reinterpret_cast<NMHDR*>(a_lParam);

    if (pData->idFrom == static_cast<WORD>(this->m_id)) {
        //MSG_LOG(TEXT("%s - Not supported %X"), this->m_name.c_str(), pData->code);
        tResult = 1;
    }
    else {
        thToolbarItem * pItem = this->Items.findItemById(pData->idFrom);

        if (pItem) {
            tResult = pItem->processNotifyMessage(a_uMsg, a_wParam, a_lParam);
        }
    }

    //TH_LEAVE_OBJECT_FUNCTION;
    return tResult;
}

/* thToolbarText */

thToolbarText::thToolbarText(thToolbarItem & a_parent)
    :
    m_parent(a_parent)
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

thToolbarText::~thToolbarText(){
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

void thToolbarText::operator=(thString in) {
    TH_ENTER_FUNCTION;
    m_parent.m_changedata.cbSize = sizeof(m_parent.m_changedata);
    m_parent.m_changedata.dwMask = TBIF_TEXT;
    m_parent.m_text = in;
    m_parent.m_changedata.pszText = (LPTSTR)m_parent.m_text.c_str();

    SendMessage(this->m_parent.m_parent.m_hWinHandle,
        TB_SETBUTTONINFO,
        (WPARAM)this->m_parent.m_data.idCommand,
        (LPARAM)&this->m_parent.m_changedata);

    TH_LEAVE_FUNCTION;
}

/* thToolbarItem */

thToolbarItem::thToolbarItem(thToolbar & a_parent)
    :
    m_parent(a_parent),
    m_data{},
    m_changedata{},
    Text(*this)
{
    TH_ENTER_OBJECT_FUNCTION;
    m_data.idCommand = (int)this->m_id;
    m_data.iBitmap = 0;
    m_data.fsState = TBSTATE_ENABLED;
    m_data.fsStyle = BTNS_BUTTON; //BTNS_CHECK http://msdn.microsoft.com/en-us/library/windows/desktop/bb760439%28v=vs.85%29.aspx
    m_data.iString = (INT_PTR)TEXT("TBButton");

    //http://stackoverflow.com/questions/16883251/size-of-button-in-a-toolbar-win32-program
    SendMessage(m_parent.m_hWinHandle, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
    //SendMessage(this->hWinHandle, TB_SETBUTTONSIZE, 0, MAKELPARAM(12, 12));
    SendMessage(m_parent.m_hWinHandle, TB_SETBITMAPSIZE, 0, MAKELPARAM(0, 0));
    LRESULT result = SendMessage(m_parent.m_hWinHandle, TB_ADDBUTTONS, 1, (LPARAM)&(m_data));
    TH_LEAVE_OBJECT_FUNCTION;
}

thToolbarItem::~thToolbarItem() {
    TH_ENTER_OBJECT_FUNCTION;
    TH_LEAVE_OBJECT_FUNCTION;
}

// thToolbarItem dont have any children windows on it, so tResult must return 1 (nonzero).
// if not, endless recursion loop will happen due to SendMessage in thWindow processCommandMessage.
LRESULT thToolbarItem::processCommandMessage(UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    TH_ENTER_OBJECT_FUNCTION;
    LRESULT tResult = 1; // should return 1 if used by app

    MSG_LOG(TEXT("Toolbar Item CLICKED, Id=%d"), this->m_id);

    // for now: only button click event
    if (nullptr != OnClick) {
        OnClick(this, { a_uMsg, a_wParam, a_lParam });
    }

    TH_LEAVE_OBJECT_FUNCTION;
    return tResult;
}

// thToolbarItem dont have any children windows on it, so tResult must return 1 (nonzero).
// if not, endless recursion loop will happen due to SendMessage in thWindow processNotifyMessage.
LRESULT thToolbarItem::processNotifyMessage(UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    TH_ENTER_OBJECT_FUNCTION;
    LRESULT tResult = 1;
    NMHDR * pData = 0;

    pData = reinterpret_cast<NMHDR*>(a_lParam);

    //MSG_LOG(TEXT("%s - Not supported %X"), this->m_name.c_str(), pData->code);

    TH_LEAVE_OBJECT_FUNCTION;
    return tResult;
}

/* thToolbarItemList */

thToolbarItemList::thToolbarItemList(thToolbar & a_parent)
    :
    m_parent(a_parent),
    LastIndex(-1)
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
}

thToolbarItemList::~thToolbarItemList() {
    TH_ENTER_FUNCTION;
    std::vector<thToolbarItem *>::iterator i;

    i = this->m_items.begin();

    for (; i != this->m_items.end(); i++) {
        delete *i;
    }

    TH_LEAVE_FUNCTION;
}

thToolbarItem * thToolbarItemList::findItemById(UINT_PTR a_searchedId) {
    //TH_ENTER_OBJECT_FUNCTION;
    std::vector<thToolbarItem*>::iterator i;
    thToolbarItem * pFoundItem = nullptr;

    i = this->m_items.begin();

    for (; i != m_items.end(); i++) {
        if (a_searchedId == static_cast<WORD>((*i)->m_id)) {
            pFoundItem = (*i);
            break;
        }
    }

    //TH_LEAVE_OBJECT_FUNCTION;
    return pFoundItem;
}

void thToolbarItemList::Add(thString a_text) {
    TH_ENTER_FUNCTION;
    thToolbarItem * newToolbar = new thToolbarItem(m_parent);

    if (newToolbar) {
        MSG_SUCCESS(TEXT("Toolbar item created with id=%d"), newToolbar->m_id);
        m_items.push_back(newToolbar);
        newToolbar->Text = a_text;
        LastIndex++;
    }
    TH_LEAVE_FUNCTION;
}

#if 0
void thToolbarItemList::Remove(uint32_t a_u32ItemIndex) {
    TH_ENTER_OBJECT_FUNCTION;
    if (a_u32ItemIndex < m_items.size()) {
        std::vector<thToolbarItem *>::iterator i;

        if (m_items.at(a_u32ItemIndex)) {
            //todo: remove item from toolbar
            delete m_items.at(a_u32ItemIndex);
        }

        i = m_items.begin() + a_u32ItemIndex; //unsigned to iterator workaround
        m_items.erase(i);
        LastIndex--;
    }
    TH_LEAVE_OBJECT_FUNCTION;
}
#endif

size_t thToolbarItemList::Size() {
    TH_ENTER_FUNCTION;
    size_t size = 0;

    size = this->m_items.size();

    TH_LEAVE_FUNCTION;
    return size;
}

thToolbarItem * const thToolbarItemList::operator[](uint32_t a_u32ItemIndex) {
    TH_ENTER_FUNCTION;
    thToolbarItem * p = 0;
    if (a_u32ItemIndex < m_items.size()) {
        if (m_items.at(a_u32ItemIndex)) {
            p = m_items.at(a_u32ItemIndex);
        }
    }
    TH_LEAVE_FUNCTION;
    return p;
}