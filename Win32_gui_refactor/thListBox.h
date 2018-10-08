#pragma once
#include "thWindow.h"
#include "thListBoxItems.h"

class thListBox : public thWindow
{
protected:
    virtual int             getDebugIndex();
private:
    static int              m_indexPool;

                            thListBox();
    LRESULT                 processCommandMessage(HWND, UINT, WPARAM, LPARAM);
    LRESULT                 processNotifyMessage(HWND, UINT, WPARAM, LPARAM);
public:
                            thListBox(thWindow *, int, int);
                            ~thListBox();

    thListBoxItems          Items;

    thEventCallbackFunc_t   OnDoubleClicked; // User has double-clicked an item in a list box.
    thEventCallbackFunc_t   OnKillFocus; // Listbox lost focus
    thEventCallbackFunc_t   OnItemSelectedCancel; // user has canceled the selection in a list box
    thEventCallbackFunc_t   OnSelectionChange; // selection in a list box has changed as a result of user input
    thEventCallbackFunc_t   OnSetFocus; // list box has received the keyboard focus
};

