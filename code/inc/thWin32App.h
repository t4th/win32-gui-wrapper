#pragma once
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

// Class abstracting the application, which provide implementation of the main loop.
class thWin32App
{
public:
                 thWin32App() = default;
    virtual      ~thWin32App() = default;

    virtual void Terminate( int) final;
    virtual int  Run() final;

private:

};

