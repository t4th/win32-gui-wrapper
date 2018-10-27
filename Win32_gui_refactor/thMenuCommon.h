#pragma once
#include "thObject.h"

class thWindow;
class thMenu;
class thMenuCommon;

class thMenuText
{
    friend class        thMenuItem;
private:
    thMenuItem *        m_pParent;
    void                setParent(thMenuItem*);
public:
                        thMenuText();
                        ~thMenuText();

    void                operator=(thString);
};

class thMenuSubMenu
{
    friend class        thMenuItem;
private:
    thMenuItem *        m_pParent;
    void                setParent(thMenuItem*);
public:
                        thMenuSubMenu();
                        ~thMenuSubMenu();

    void                operator=(thMenuCommon *);
};

//add image 
#if 0
void bitmap(string filename)
{
    HBITMAP bitimage = (HBITMAP)LoadImage(NULL, filename.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

    if (primeiromenu == true)
        SetMenuItemBitmaps(GetMenu(WindowMain), menuposition, MF_BYPOSITION, (HBITMAP)bitimage, (HBITMAP)bitimage);
    else
        SetMenuItemBitmaps(MenuHandle, menuposition, MF_BYPOSITION, (HBITMAP)bitimage, (HBITMAP)bitimage);
}
#endif

class thMenuItem : public thObject
{
    friend class            thMenuItemList;
    friend class            thMenuSubMenu;
    friend class            thMenuText;
    friend class            thMenuCommon;
private:
    thMenuCommon *          m_pParent;
    static int              m_indexPool;
    thMenuCommon *          m_popupMenu;
    //    void GetInfo()
    //    {
    //        this->m_changedata.cbSize = sizeof(m_changedata);
    //        this->m_changedata.dwMask = TBIF_IMAGE | TBIF_TEXT | TBIF_STATE | TBIF_STYLE | TBIF_LPARAM | TBIF_COMMAND | TBIF_SIZE;
    //        SendMessage(this->parent->getHandle(), TB_GETBUTTONINFO, (WPARAM)this->id, (LPARAM)&this->m_changedata);
    //    }
    LRESULT                 processCommandMessage(UINT, WPARAM, LPARAM);
    LRESULT                 processNotifyMessage(UINT, WPARAM, LPARAM);

                            thMenuItem();
protected:
public:
                            thMenuItem(thMenuCommon *);
                            ~thMenuItem();
    //bitmap
    //state
    //style
    //id <- private
    //caption
    thMenuText              Text;
    thMenuSubMenu           SubMenu;

    thEventCallbackFunc_t   OnClick;
};

class thMenuItemList
{
    friend class                thMenuCommon;
private:
    thMenuCommon *              m_pParent;
    std::vector<thMenuItem *>   m_items;

    thMenuItem *                findItemById(UINT_PTR);
    void                        setParent(thMenuCommon *);
public:
                                thMenuItemList();
                                thMenuItemList(thMenuCommon *);
                                ~thMenuItemList();

    void                        Add(thString);
 // void                        Insert(thMenuItem&);
 // void                        Remove(uint32_t);
    uint32_t                    Size();
    int                         LastIndex;

    thMenuItem * const          operator[](uint32_t);
};

class thMenuCommon : public thObject
{
    friend class        thMenuText;
    friend class        thMenuSubMenu;
    friend class        thMenuItemList;
private:
protected:
    //HMENU               m_hMenuHandle;
    HWND                m_hParentWindow;
    LRESULT             processCommandMessage(HWND, UINT, WPARAM, LPARAM);
public:
                        thMenuCommon();
    virtual             ~thMenuCommon();

    thMenuItemList      Items;
    HMENU               m_hMenuHandle;
};
