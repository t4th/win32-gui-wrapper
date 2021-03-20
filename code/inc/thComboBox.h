#pragma once
#include "thWindow.h"
#include "thComboBoxItems.h"

class thComboBox : public thWindow
{
protected:
    virtual int             getDebugIndex();
private:
    static int              m_indexPool;

    LRESULT                 onErrorSpace(HWND, UINT, WPARAM, LPARAM);
    virtual LRESULT         onResize(HWND, WPARAM, LPARAM);
    LRESULT                 processCommandMessage(HWND, UINT, WPARAM, LPARAM);
    LRESULT                 processNotifyMessage(HWND, UINT, WPARAM, LPARAM);
public:
                            thComboBox() = delete;
                            thComboBox(thWindow *, int, int);
                            ~thComboBox();

    thComboBoxItems         Items;
    
    // Called when the user changes the current selection in the list box of a combo box
    thEventCallbackFunc_t   OnSelectChange{nullptr};

    // Called when the user double-clicks a string in the list box of a combo box
    thEventCallbackFunc_t   OnDoubleClicked{nullptr};

    // Called when a combo box receives the keyboard focus
    thEventCallbackFunc_t   OnSetFocus{nullptr};

    // Called when a combo box loses the keyboard focus
    thEventCallbackFunc_t   OnKillFocus{nullptr};

    // Called after the user has taken an action that may have altered the text in the edit control portion of a combo box
    thEventCallbackFunc_t   OnEditChange{nullptr};

    // Called when the edit control portion of a combo box is about to display altered text
    thEventCallbackFunc_t   OnEditUpdate{nullptr};

    // Called when the combo box is about to be made visible
    thEventCallbackFunc_t   OnDropdown{nullptr};

    // Called when the combo box has been closed
    thEventCallbackFunc_t   OnCloseUp{nullptr};

    // Called when the user selects a list item, or selects an item and then closes the list
    thEventCallbackFunc_t   OnItemSelected{nullptr};

    // Called when the user selects an item, but then selects another control or closes the dialog box
    thEventCallbackFunc_t   OnItemSelectedCancel{nullptr};
};

