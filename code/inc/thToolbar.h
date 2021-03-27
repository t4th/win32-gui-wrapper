#pragma once
#include "thWindow.h"
#include "thForm.h"

class thForm;
class thToolbar;
class thToolbarItem;

class thToolbarText
{
private:
    thToolbarItem &             m_parent;
public:
                                thToolbarText() = delete;
                                thToolbarText(thToolbarItem & a_parent);
                                ~thToolbarText();

    void                        operator=(thString);
};

class thToolbarItem : public thObject
{
    friend class                thToolbarItemList;
    friend class                thToolbarText;
    friend class                thToolbar;
private:
    thToolbar &                 m_parent;
    static int                  m_indexPool;

    TBBUTTON                    m_data;
    TBBUTTONINFO                m_changedata;

    thString                    m_text;
    //    void GetInfo()
    //    {
    //        this->m_changedata.cbSize = sizeof(m_changedata);
    //        this->m_changedata.dwMask = TBIF_IMAGE | TBIF_TEXT | TBIF_STATE | TBIF_STYLE | TBIF_LPARAM | TBIF_COMMAND | TBIF_SIZE;
    //        SendMessage(this->parent->getHandle(), TB_GETBUTTONINFO, (WPARAM)this->id, (LPARAM)&this->m_changedata);
    //    }
    LRESULT                      processCommandMessage(UINT, WPARAM, LPARAM);
    LRESULT                      processNotifyMessage(UINT, WPARAM, LPARAM);

protected:
public:
                                thToolbarItem() = delete;
                                thToolbarItem(thToolbar & a_parent);
                                ~thToolbarItem();
    //bitmap
    //state
    //style
    //id <- private
    //caption
    thToolbarText               Text;

    thEventCallbackFunc_t       OnClick{nullptr};
};

class thToolbarItemList
{
    friend class                thToolbar;
private:
    thToolbar &                  m_parent;
    std::vector<thToolbarItem *> m_items;

    thToolbarItem *              findItemById(UINT_PTR);
public:
                                 thToolbarItemList() = delete;
                                 thToolbarItemList(thToolbar & a_parent);
                                 ~thToolbarItemList();

    void                         Add(thString);
 // void                         Insert(thToolbarItem &);
 // void                         Remove(uint32_t);
    size_t                       Size();
    int                          LastIndex;

    thToolbarItem * const        operator[](uint32_t);
};

class thToolbar : public thWindow
{
    friend class        thToolbarItem;
    friend class        thToolbarText;
protected:
    virtual int         getDebugIndex();
private:
    static int          m_indexPool;

    LRESULT             processCommandMessage(HWND, UINT, WPARAM, LPARAM);
    LRESULT             processNotifyMessage(HWND, UINT, WPARAM, LPARAM);
public:
                        thToolbar() = delete;
                        thToolbar(thWindow * a_pParent, int a_posX = CW_USEDEFAULT, int a_posY = CW_USEDEFAULT);
    virtual             ~thToolbar(void);

    thToolbarItemList   Items;
};

