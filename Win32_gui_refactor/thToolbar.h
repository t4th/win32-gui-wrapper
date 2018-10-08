#pragma once
#include "thWindow.h"
#include "thForm.h"

class thForm;
class thToolbar;
class thToolbarItem;

class thToolbarText
{
    friend class                thToolbarItem;
private:
    thToolbarItem *             m_pParent;
    void                        setParent(thToolbarItem*);
public:
                                thToolbarText();
                                ~thToolbarText();

    void                        operator=(thString);
};

class thToolbarItem : public thObject
{
    friend class                thToolbarItemList;
    friend class                thToolbarText;
    friend class                thToolbar;
private:
    thToolbar *                 m_pParent;
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

                                thToolbarItem();
protected:
public:
                                thToolbarItem(thToolbar *);
                                ~thToolbarItem();
    //bitmap
    //state
    //style
    //id <- private
    //caption
    thToolbarText               Text;

    thEventCallbackFunc_t       OnClick;
    //int                         RegisterCallback(uint32_t, eventCallbackFunc_t);
};

class thToolbarItemList
{
    friend class                thToolbar;
private:
    thToolbar *                  m_pParent;
    std::vector<thToolbarItem *> m_items;

    thToolbarItem *              findItemById(UINT_PTR);
    void                         setParent(thToolbar *);
public:
                                 thToolbarItemList();
                                 ~thToolbarItemList();

    void                         Add(thString);
 // void                         Insert(thToolbarItem &);
 // void                         Remove(uint32_t);
    uint32_t                     Size();
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

                        thToolbar();
    LRESULT             processCommandMessage(HWND, UINT, WPARAM, LPARAM);
    LRESULT             processNotifyMessage(HWND, UINT, WPARAM, LPARAM);
public:
                        thToolbar(thWindow *, int, int);
    virtual             ~thToolbar(void);

    thToolbarItemList   Items;
};

