#pragma once
#include "thApplication.h"
#include "thWin32Logger.h"
#include "thWindow.h"
#include "thForm.h"
#include "thButton.h"
#include "thGroupBox.h"
#include "thRadioButton.h"
#include "thCheckBox.h"
#include "thMenu.h"
#include "thPopupMenu.h"
#include "thLabel.h"
#include "thComboBox.h"
#include "thToolbar.h"
#include "thMDIClient.h"
#include "thMDIChild.h"
#include "thEditBox.h"
#include "thCommonDialog.h"
#include "thFile.h"
#include "thMemo.h"
#include "thRichEdit.h"
#include "thListView.h"
#include "thListBox.h"

class thWin32App : public thApplication
{
public:
                thWin32App() = default;
                ~thWin32App() = default;

    // TODO: rework OnCreate and OnDestroy Callbacks.
    void        OnCreate();
    void        OnDestroy();

    void        Terminate(int);
    int         Run();

private:

};

