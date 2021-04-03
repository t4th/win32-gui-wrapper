#pragma once
#include "thWindow.h"
#include "thListBoxItems.h"

class thListBox : public thWindow
{
protected:
    virtual int             getDebugIndex();
private:
    static int              m_indexPool;

    LRESULT                 processCommandMessage(HWND, UINT, WPARAM, LPARAM);
    LRESULT                 processNotifyMessage(HWND, UINT, WPARAM, LPARAM);
public:
                            thListBox() = delete;
                            thListBox(thWindow * a_pParent, int a_posX = CW_USEDEFAULT, int a_posY = CW_USEDEFAULT);
                            ~thListBox();

    thListBoxItems          Items;

    // User has double-clicked an item in a list box.
    thEventCallbackFunc_t   OnDoubleClicked{nullptr};

    // Listbox lost focus
    thEventCallbackFunc_t   OnKillFocus{nullptr};

    // user has canceled the selection in a list box
    thEventCallbackFunc_t   OnItemSelectedCancel{nullptr};

    // selection in a list box has changed as a result of user input
    thEventCallbackFunc_t   OnSelectionChange{nullptr};

    // list box has received the keyboard focus
    thEventCallbackFunc_t   OnSetFocus{nullptr};
};

