#include "thWin32App.h"
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

#if 1
#pragma comment( linker, "/manifestdependency:\"type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' \
language='*'\"")
#endif

// https://code.msdn.microsoft.com/CppWindowsCommonControls-9ea0de64
// for linux http://www.gtk.org/

// new example
// entry point

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
    thWin32App MyApp;

    MyApp.Init(hInst);

    return MyApp.Run();
}

thWin32Logger   m_oLogger;

#if 0

namespace MainWindow
{
    thForm *    pForm;
    thButton *  pComboBoxButton;

    LRESULT     Form_onDestroy(thWindow *);
    LRESULT        ComboBoxButton_onClick(thObject * const, thEventParams_t);
}

namespace ComboBoxWindow
{
    thForm *     pForm;
    thComboBox * pComboBox;
    thEditBox *  pInputEditBox;
    thLabel *    pSelectionLabel;
    thButton *   pAddButton;
    thButton *   pRemoveButton;

    LRESULT     Form_onClose(thWindow *);
    LRESULT        ComboBox_onSelectChange(thObject * const, thEventParams_t);
    LRESULT        RemoveButton_onClick(thObject * const, thEventParams_t);
    LRESULT        AddButton_onClick(thObject * const, thEventParams_t);
}

// Application entry point
void thWin32App::OnCreate()
{
    // create main window form
    MainWindow::pForm = new thForm(0, 0, 0);
    MainWindow::pForm->Text = TEXT("Example application");
    MainWindow::pForm->X = 500;
    MainWindow::pForm->Y = 200;
    MainWindow::pForm->Width = 300;
    MainWindow::pForm->Height = 600;
#if 0 //not supported atm for thForm
    MainWindow::pForm->Constraints.MaxHeight = 600;
    MainWindow::pForm->Constraints.MinHeight = 600;
    MainWindow::pForm->Constraints.MaxWidth = 300;
    MainWindow::pForm->Constraints.MinWidth = 300;
#endif
    MainWindow::pForm->OnDestroy = MainWindow::Form_onDestroy; // onDestroy event
    MainWindow::pForm->Show();

    // create ComboBox example window
    ComboBoxWindow::pForm = new thForm(MainWindow::pForm, 0, 0);
    ComboBoxWindow::pForm->Text = TEXT("Example application");
    ComboBoxWindow::pForm->X = 500;
    ComboBoxWindow::pForm->Y = 200;
    ComboBoxWindow::pForm->Width = 300;
    ComboBoxWindow::pForm->Height = 300;
    ComboBoxWindow::pForm->OnClose = ComboBoxWindow::Form_onClose;

    // create ComboBox in ComboBox example window
    ComboBoxWindow::pComboBox = new thComboBox(ComboBoxWindow::pForm, 20, 20);
    ComboBoxWindow::pComboBox->Width = 180;
    ComboBoxWindow::pComboBox->Anchors.Right = TRUE;
    ComboBoxWindow::pComboBox->Items.Add(TEXT("Test Item 1"));
    ComboBoxWindow::pComboBox->Items.SetItemIndex(0);
    ComboBoxWindow::pComboBox->Items.Add(TEXT("Test Item 2"));
    ComboBoxWindow::pComboBox->Items.Add(TEXT("Test Item 3"));
    ComboBoxWindow::pComboBox->OnSelectChange = ComboBoxWindow::ComboBox_onSelectChange;

    ComboBoxWindow::pRemoveButton = new thButton(ComboBoxWindow::pForm, 210, 20);
    ComboBoxWindow::pRemoveButton->Width = 70;
    ComboBoxWindow::pRemoveButton->Text = TEXT("Remove");
    ComboBoxWindow::pRemoveButton->Anchors.Left = FALSE;
    ComboBoxWindow::pRemoveButton->OnClick = ComboBoxWindow::RemoveButton_onClick;

    ComboBoxWindow::pInputEditBox = new thEditBox(ComboBoxWindow::pForm, 20, 50);
    ComboBoxWindow::pInputEditBox->Text = TEXT("Test Item 4");
    ComboBoxWindow::pInputEditBox->Width = 180;
    ComboBoxWindow::pInputEditBox->Anchors.Right = TRUE;

    ComboBoxWindow::pAddButton = new thButton(ComboBoxWindow::pForm, 210, 50);
    ComboBoxWindow::pAddButton->Width = 70;
    ComboBoxWindow::pAddButton->Text = TEXT("Add");
    ComboBoxWindow::pAddButton->Anchors.Left = FALSE;
    ComboBoxWindow::pAddButton->OnClick = ComboBoxWindow::AddButton_onClick;

    ComboBoxWindow::pSelectionLabel = new thLabel(ComboBoxWindow::pForm, 20, 70);
    ComboBoxWindow::pSelectionLabel->Text = ComboBoxWindow::pComboBox->Items[0].GetText();
    ComboBoxWindow::pSelectionLabel->Width = 180;
    ComboBoxWindow::pSelectionLabel->Anchors.Right = TRUE;

    // create ComboBox example button
    MainWindow::pComboBoxButton = new thButton(MainWindow::pForm, 20, 20);
    MainWindow::pComboBoxButton->Text = TEXT("ComboBox example");
    MainWindow::pComboBoxButton->Width = 260;
    MainWindow::pComboBoxButton->Anchors.Right = TRUE;
    MainWindow::pComboBoxButton->OnClick = MainWindow::ComboBoxButton_onClick; // onClick event
}

// Application exit point
void thWin32App::OnDestroy()
{
    delete MainWindow::pForm;
    delete MainWindow::pComboBoxButton;
    delete ComboBoxWindow::pForm;
    delete ComboBoxWindow::pComboBox;
    delete ComboBoxWindow::pInputEditBox;
    delete ComboBoxWindow::pSelectionLabel;
    delete ComboBoxWindow::pAddButton;
    delete ComboBoxWindow::pRemoveButton;
}

// Event Callbacks:
// Terminate application when window is destroyed
static LRESULT MainWindow::Form_onDestroy(thWindow *) {
    thWin32App::Terminate(0);
    return 0;
}

LRESULT MainWindow::ComboBoxButton_onClick(thObject * const sender, thEventParams_t info){
    if (ComboBoxWindow::pForm) {
        ComboBoxWindow::pForm->Show();
    }
    return 1;
}

LRESULT ComboBoxWindow::Form_onClose(thWindow * sender) {
    sender->Hide();
    return 1; // return 0 would destroy the window
}

LRESULT ComboBoxWindow::ComboBox_onSelectChange(thObject * const sender, thEventParams_t info){
    int nSelectedItem = ComboBoxWindow::pComboBox->Items.ItemIndex();
    ComboBoxWindow::pSelectionLabel->Text = ComboBoxWindow::pComboBox->Items[nSelectedItem].GetText();
    return 1;
}

LRESULT ComboBoxWindow::RemoveButton_onClick(thObject * const sender, thEventParams_t info){
    ComboBoxWindow::pComboBox->Items.Remove(ComboBoxWindow::pComboBox->Items.ItemIndex());
    ComboBoxWindow::pComboBox->Items.SetItemIndex(-1); // refresh comboBox
    return 1;
}

LRESULT ComboBoxWindow::AddButton_onClick(thObject * const sender, thEventParams_t info){
    ComboBoxWindow::pComboBox->Items.Add(ComboBoxWindow::pInputEditBox->Text);
    return 1;
}


// old example
#else

thForm *        form = 0;
thForm *        form2 = 0;
thForm *        form3 = 0;
thMDIClient *   mdiclient = 0;

thListView *    thListView1 = 0;
thButton *      button = 0;
thButton *      button4 = 0;
thButton *      button2 = 0;
thButton *      button3 = 0;
thRadioButton * rb1 = 0;
thRadioButton * rb2 = 0;
thCheckBox *    cb1 = 0;
thMenu *        menu1 = 0;
thMenu *        menu2 = 0;
thPopupMenu *   pop1 = 0;
thPopupMenu *   pop2 = 0;
thLabel *       label1 = 0;
thComboBox *    combo1 = 0;
thToolbar *     tb1 = 0;
thEditBox *     edit1 = 0;
thListBox *     listbox1 = 0;

LRESULT Button_onClick(thObject * const sender, thEventParams_t info);
LRESULT Button2_onClick(thObject * const sender, thEventParams_t info);

std::vector<thMDIChild*> mdiChilds;

// hold mdi child pointer and RichEdit pointer
struct sTextMDIdata_t
{
    thMDIChild * pMdi;
    thRichEdit * pRichEdit;

    sTextMDIdata_t() : pMdi(0), pRichEdit(0)
    {}
};

std::vector<sTextMDIdata_t*> textMdiChilds; // list of mdi children which hold memo component

// on MDI destory event, clean up
LRESULT MDIChild_onDestroy(thWindow * pOwner) {
    LRESULT result = 0;

    std::vector<thMDIChild*>::iterator i;

    i = mdiChilds.begin();

    for (; i != mdiChilds.end(); i++) {
        if (pOwner == (*i)) {
            delete pOwner; // delete Owner class which is created with 'new thMDIChild'
            mdiChilds.erase(i);
            break;
        }
    }

    return result;
}

//on MDI destory event, it should destroy childen components
LRESULT Text_MDIChild_onDestroy(thWindow * pOwner) {
    LRESULT result = 0;

    std::vector<sTextMDIdata_t*>::iterator i;

    i = textMdiChilds.begin();

    for (; i != textMdiChilds.end(); i++) {
        if (pOwner == (*i)->pMdi) {
            delete (*i)->pRichEdit; // delete richEdit
            delete (*i)->pMdi;  // delete mdi child itself
            delete (*i);        // delete structure that hold pointers to memo and mdi child
            textMdiChilds.erase(i);
            break;
        }
    }

    return result;
}

//create mdi children
LRESULT Menu1_onClick(thObject * const sender, thEventParams_t info){
    thMDIChild * pNewchild = 0;

    pNewchild = new thMDIChild(mdiclient, CW_USEDEFAULT, CW_USEDEFAULT);

    if (pNewchild) {
        pNewchild->OnDestroy = MDIChild_onDestroy; // bind onDestroy callback
        pNewchild->Text = TEXT("MDI");
        mdiChilds.push_back(pNewchild);
    }
    return 1;
}

LRESULT Toolbar1_onClick(thObject * const sender, thEventParams_t info){
    button->Text = TEXT("aa");
    return 1;
}

// onClose event: instead of destorying form, hide it.
LRESULT Form_onClose(thWindow * pOwner)
{
    TH_ENTER_FUNCTION;
    LRESULT tResult = 0;
    pOwner->Hide();
    tResult = 1;    // returning 1 wont destroy object
    TH_LEAVE_FUNCTION;
    return tResult;
}

// closing main application form will close application
LRESULT Form3_onDestroy(thWindow * pOwner)
{
    TH_ENTER_FUNCTION;
    LRESULT tResult = 0;

    // clean up MDI childs classes (this is already handled internally
    // but for sake of readibility is here too)
    std::vector<thMDIChild*>::iterator i;

    i = mdiChilds.begin();

    for (; i != mdiChilds.end(); i++) {
        delete (*i);
    }

    ::PostQuitMessage(0); //close app

    TH_LEAVE_FUNCTION;
    return tResult;
}

typedef struct
{
    uint8_t code[4];
    int     size;
} BOM_LOOKUP_t;

BOM_LOOKUP_t BOM_lookup[5] =
{
    { { 0x00, 0x00, 0xFE, 0xFF }, 4 }, // UTF-32, big-endian 
    { { 0xFF, 0xFE, 0x00, 0x00 }, 4 }, // UTF - 32, little - endian
    { { 0xFE, 0xFF, 0x00, 0x00 }, 2 }, // UTF - 16, big - endian
    { { 0xFF, 0xFE, 0x00, 0x00 }, 2 }, // UTF - 16, little - endian
    { { 0xEF, 0xBB, 0xBF, 0x00 }, 3 }, // UTF - 8
};

int Get_BOM_HeaderSize(uint8_t* p)
{
    int bom_size = 2;

    for (int i = 0; i < 5; i++) {
        if (0 == memcmp(p, BOM_lookup[i].code, BOM_lookup[i].size))
        {
            bom_size = BOM_lookup[i].size;
            break;
        }
    }

    return bom_size;
}

// open file in new MDI child int ASCII/UNICODE format
LRESULT Menu2_FileOpen_onClick(thObject * const sender, thEventParams_t info){
    thOpenDialog openDialog;
    thDialogFilterItem all(TEXT("All"), TEXT("*.*"));
    thDialogFilterItem text;
    text.Name = TEXT("Text");
    text.Filter = TEXT("*.TXT");

    openDialog.Filter.Add(all);
    openDialog.Filter.Add(text);


    // show Open File Dialog.
    // if successed return true
    if (openDialog.Show(form3))
    {
        sTextMDIdata_t * pNewchild = 0;

        pNewchild = new sTextMDIdata_t;

        if (pNewchild){
            // Create MDI child Window
            pNewchild->pMdi = new thMDIChild(mdiclient, CW_USEDEFAULT, CW_USEDEFAULT);

            if (pNewchild->pMdi) {
                pNewchild->pMdi->Text = openDialog.FileName;
                pNewchild->pMdi->Width = (int)((double)form3->Width * 0.8);
                pNewchild->pMdi->Height = (int)((double)form3->Height * 0.8);
                pNewchild->pMdi->OnDestroy = Text_MDIChild_onDestroy; // register onDestroy Event

                // Create RichEdit component in new MDI child Window
                pNewchild->pRichEdit = new thRichEdit(pNewchild->pMdi, 0, 0);

                if (pNewchild->pRichEdit) {
                    pNewchild->pRichEdit->Width = pNewchild->pMdi->Width - 20;
                    pNewchild->pRichEdit->Height = pNewchild->pMdi->Height - 50;
                    pNewchild->pRichEdit->Anchors.Right = true;
                    pNewchild->pRichEdit->Anchors.Bottom = true;

                    // Open file selected in openDialog
                    thFile file;
                    file.Open(openDialog.FileName, thFile::generic_read, thFile::open_existing);

                    if (file.IsOpen()) {
                        uint32_t u32BufferSize = 0; // in bytes

                        u32BufferSize = (uint32_t)file.GetFileSize();

                        if (u32BufferSize) {
                            uint8_t * buffer = NULL; // file read buffer

                            buffer = new uint8_t[u32BufferSize];

                            if (buffer) {
                                uint32_t u32Result = 0; // file read result
                                uint32_t u32BytesRead = 0;

                                u32Result = file.Read((uint8_t*)buffer, u32BufferSize, u32BytesRead);

                                if (0 == u32Result) {
                                    // http://www.catch22.net/tuts/unicode-text-processing
                                    BOOL fResult = FALSE;

                                    fResult = IsTextUnicode(buffer, (int)u32BytesRead, NULL);

                                    if (fResult) {
                                        // UNICODE
                                        MSG_LOG(TEXT("UNICODE"));
                                        //uint32_t bom_size = 0;
                                        //bom_size = Get_BOM_HeaderSize(buffer);

                                        std::wstring text((wchar_t*)buffer, u32BytesRead); // interpret as wide string; - 10 bytes of BOM header
                                        #ifdef UNICODE
                                            pNewchild->pRichEdit->Text = text;
                                        #else
                                            pNewchild->pRichEdit->Text = WStringToString(text);
                                        #endif
                                    }
                                    else {
                                        // ASCII
                                        MSG_LOG(TEXT("ASCII"));
                                        std::string text((char*)buffer, u32BytesRead); // interpret as ANSI string
                                        #ifdef UNICODE
                                            pNewchild->pRichEdit->Text = StringToWString(text);
                                        #else
                                            pNewchild->pRichEdit->Text = text;
                                        #endif
                                    }

                                }
                            }

                            delete[] buffer;
                        }
                    }

                    textMdiChilds.push_back(pNewchild);
                }
            }
        }
    }
    return 1;
}

LRESULT Menu2_onClick1(thObject * const sender, thEventParams_t info){
    mdiclient->Cascade();
    return 1;
}

LRESULT Menu2_onClick2(thObject * const sender, thEventParams_t info){
    mdiclient->TileHorizontal();
    return 1;
}

LRESULT Menu2_onClick3(thObject * const sender, thEventParams_t info){
    mdiclient->TileVertical();
    return 1;
}

LRESULT Menu2_onClick4(thObject * const sender, thEventParams_t info){
    mdiclient->ArrangeIcons();
    return 1;
}

LRESULT Menu2_onClick5(thObject * const sender, thEventParams_t info){
    form->Show();
    return 1;
}

LRESULT Menu2_onClick6(thObject * const sender, thEventParams_t info){
    form2->Show();
    return 1;
}

LRESULT ComboBox1_onSelChange(thObject * const sender, thEventParams_t info){
    switch (combo1->Items.ItemIndex()) {
    case 0:
        thListView1->SetView(thListView::eViewType_t::view_details);
        break;
    case 1:
        thListView1->SetView(thListView::eViewType_t::view_icon);
        break;
    case 2:
        thListView1->SetView(thListView::eViewType_t::view_list);
        break;
    case 3:
        thListView1->SetView(thListView::eViewType_t::view_smallicon);
        break;
    case 4:
        thListView1->SetView(thListView::eViewType_t::view_tile);
        break;
    default:
        // not used
        break;
    }
    return 1;
}

void thWin32App::OnCreate()
{
    TH_ENTER_FUNCTION;

    // window submenu
    menu2 = new thMenu();
    menu2->Items.Add(TEXT("cascade"));
    menu2->Items[0]->OnClick = Menu2_onClick1;
    menu2->Items.Add(TEXT("horizontal tile"));
    menu2->Items[1]->OnClick = Menu2_onClick2;
    menu2->Items.Add(TEXT("vertical tile"));
    menu2->Items[2]->OnClick = Menu2_onClick3;
    menu2->Items.Add(TEXT("arrange icons"));
    menu2->Items[3]->OnClick = Menu2_onClick4;
    menu2->Items.Add(TEXT("-"));

    // main menu
    menu1 = new thMenu();

    menu1->Items.Add(TEXT("open file"));
    menu1->Items[0]->OnClick = Menu2_FileOpen_onClick;
    menu1->Items.Add(TEXT("test name"));
    menu1->Items[1]->Text = TEXT("Add mdi child");
    menu1->Items[1]->OnClick = Menu1_onClick;
    menu1->Items.Add(TEXT("Window"));
    menu1->Items[2]->SubMenu = menu2;
    menu1->Items.Add(TEXT("form"));
    menu1->Items[3]->OnClick = Menu2_onClick5;
    menu1->Items.Add(TEXT("form2"));
    menu1->Items[4]->OnClick = Menu2_onClick6;

    // popup menus
    pop1 = new thPopupMenu();
    pop1->Items.Add(TEXT("stuff"));
    pop1->Items.Add(TEXT("later"));
    pop2 = new thPopupMenu();
    pop2->Items.Add(TEXT("options"));
    pop2->Items.Add(TEXT("change"));
    pop2->Items[0]->SubMenu = pop1;
    //   pop2->Items[0]->SubMenu = 0;

    // main form
    form3 = new thForm(0, 0, 0);
    form3->Width = 800;
    form3->Height = 800;
    form3->X = 50;
    form3->Y = 50;
    form3->Text = TEXT("test form name 1");
    form3->SetMenu(menu1);
    form3->OnDestroy = Form3_onDestroy;

    button2 = new thButton(form3, 0, form3->Height - 30);
    button2->Width = form3->Width;
    button2->Height = 30;
    button2->Anchors.Top = FALSE;
    button2->Anchors.Left = TRUE;
    button2->Anchors.Right = TRUE;

    mdiclient = new thMDIClient(form3, 0, 0);
    mdiclient->Width = form3->Width;
    mdiclient->Height = form3->Height - 30;
    mdiclient->Anchors.Right = TRUE;
    mdiclient->Anchors.Bottom = TRUE;

    form3->Show();

    // form
    form = new thForm(0, 0, 0);
    form->Width = 500;
    form->Height = 350;
    form->X = 500;
    form->Y = 100;
    form->Text = TEXT("test form name 2");
    form->PopupMenu = pop1;
    form->OnClose = Form_onClose;
    //form->Show();

    combo1 = new thComboBox(form, 2, 2);
    combo1->Width = form->Width - 4;
    combo1->Height = 200;
    combo1->Items.Add(TEXT("Details"));
    combo1->Items.Add(TEXT("Icon"));
    combo1->Items.Add(TEXT("List"));
    combo1->Items.Add(TEXT("Small icon"));
    combo1->Items.Add(TEXT("Tile"));
    combo1->Items.SetItemIndex(0);
    combo1->PopupMenu = pop2;
    combo1->Anchors.Right = true;
    combo1->OnSelectChange = ComboBox1_onSelChange;

    edit1 = new thEditBox(form, 2, 4 + combo1->Height);
    edit1->Text = TEXT("name1");

    button = new thButton(form, 2, 4 + edit1->Y + edit1->Height);
    button->Text = TEXT("Item");
    button->OnClick = Button_onClick;
    button->PopupMenu = pop2;

    button4 = new thButton(form, 2, 4 + button->Y + button->Height);
    button4->OnClick = Button2_onClick;
    button4->Text = TEXT("Subitem");
    //button4->PopupMenu = pop2;

    listbox1 = new thListBox(form, 2, 4 + button4->Y + button4->Height);
    listbox1->Width = button4->Width;
    listbox1->Height = 200;

    thListView1 = new thListView(form, 2 + button->X + 1 + button->Width, 30);
    thListView1->Width = 400;
    thListView1->Height = 300;
    thListView1->Anchors.Right = true;
    thListView1->Anchors.Bottom = true;
    thListView1->Columns.Add(TEXT("Column1"));
    thListView1->Columns.Add(TEXT("Column2"));
    thListView1->Columns.Add(TEXT("Column3"));
    thListView1->Columns.Add(TEXT("Column4"));
    thListView1->SetView(thListView::eViewType_t::view_details);

    form2 = new thForm(form, 0, 0);
    form2->Width = 300;
    form2->Height = 300;
    form2->X = form->Width + 520;
    form2->Y = 100;
    form2->Text = TEXT("test form name 3");
    form2->Resizable = false;
    form2->OnClose = Form_onClose;


    tb1 = new thToolbar(form2, 0, 0);
    tb1->Items.Add(TEXT("destroyss"));
    tb1->Items[0]->Text = TEXT("destroy");
    tb1->Items[0]->OnClick = Toolbar1_onClick;

    tb1->Items.Add(TEXT("test2"));
    tb1->Items[1]->Text = TEXT("test222");

    tb1->Items.Add(TEXT("test3"));
    //tb1->Items[2]->Text = TEXT("test3");
    //tb1->Items[2]->OnClick = Button2_onClick;

    //button2 = new thButton(tb1, 70, 0);

    rb1 = new thRadioButton(form2, 5, 35 + button->Height + 5);
    rb1->Width = 200;
    rb1->Font.SetName(TEXT("Times New Roman"));
    rb1->Font.SetSize(10);
    rb2 = new thRadioButton(form2, 5, rb1->Y * 2);
    rb2->Font.SetName(TEXT("Gill Sans Ultra Bold Condensed"));
    rb2->Font.SetSize(10);
    rb2->Width = 200;
    cb1 = new thCheckBox(form2, 5, rb1->Y * 3);
    cb1->Font.SetName(TEXT("Wide Latin"));
    cb1->Font.SetSize(10);
    cb1->Width = 200;

    //label1 = new thLabel(form, 20, 20);

    TH_LEAVE_FUNCTION;
}

void thWin32App::OnDestroy()
{
    TH_ENTER_FUNCTION;

    delete thListView1;
    delete form;
    delete form2;
    delete form3;
    delete mdiclient;
    delete button;
    delete button2;
    delete button3;
    delete button4;
    delete rb1;
    delete rb2;
    delete cb1;
    delete menu1;
    delete menu2;
    delete pop1;
    delete pop2;
    delete label1;
    delete combo1;
    delete tb1;
    delete edit1;
    delete listbox1;

    TH_LEAVE_FUNCTION;
}

// callbacks

LRESULT Button_onClick(thObject * const sender, thEventParams_t info)
{
    TH_ENTER_FUNCTION;
    static int i = 0;

    //combo1->Items.Add(edit1->Text);
    thListView1->Items.Add(edit1->Text);
    listbox1->Items.Add(edit1->Text);

    TH_LEAVE_FUNCTION;
    return 1;
}

LRESULT Button2_onClick(thObject * const sender, thEventParams_t info)
{
    TH_ENTER_FUNCTION;
    for (int j = 0; j < thListView1->Items.GetCount(); j++) {
        for (int i = 1; i < thListView1->Columns.GetCount(); i++) {
            thListView1->Items[j]->SubItems[i].SetText(NumToString(i));
        }
    }
    //    thListView1->Items[0]->SetText(TEXT("asdasd"));

    TH_LEAVE_FUNCTION;
    return 1;
}
#endif