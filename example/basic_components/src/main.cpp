#include "thWin32App.h"

// Enable Visual Styles from Common Controls 6 (ComCtl32).
#pragma comment( linker, "/manifestdependency:\"type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' \
language='*'\"")

thWin32Logger   g_logger;

thWin32App      myApp;

// Windows application entry point.
int main()
{
    return myApp.Run();
}

namespace local_data
{
    // TODO: Make these smart pointers.
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

    // Hold Empty window MDIChild objects.
    std::vector< std::unique_ptr< thMDIChild>> mdiChilds;

    // Define single Text window MDIChild object.
    struct sTextMDIdata_t
    {
        std::unique_ptr< thMDIChild> m_MdiChild{};
        std::unique_ptr< thRichEdit> m_RichEdit{};
    };

    // Hold Text window MDIChild objects.
    std::vector< std::unique_ptr< sTextMDIdata_t>> textMdiChilds;
}

thResult_t MDIChild_onDestroy( thObject * pOwner, thEventParams_t info);
thResult_t Text_MDIChild_onDestroy( thObject * pOwner, thEventParams_t info);
thResult_t Menu1_onClick( thObject * const sender, thEventParams_t info);
thResult_t Toolbar1_onClick(thObject * const sender, thEventParams_t info);
thResult_t Form_onClose(thObject * pOwner, thEventParams_t info);
thResult_t Form3_onDestroy(thObject * pOwner, thEventParams_t info);
thResult_t Menu2_FileOpen_onClick(thObject * const sender, thEventParams_t info);
thResult_t Menu2_onClick1(thObject * const sender, thEventParams_t info);
thResult_t Menu2_onClick2(thObject * const sender, thEventParams_t info);
thResult_t Menu2_onClick3(thObject * const sender, thEventParams_t info);
thResult_t Menu2_onClick4(thObject * const sender, thEventParams_t info);
thResult_t Menu2_onClick5(thObject * const sender, thEventParams_t info);
thResult_t Menu2_onClick6(thObject * const sender, thEventParams_t info);
thResult_t ComboBox1_onSelChange(thObject * const sender, thEventParams_t info);
thResult_t Button_onClick(thObject * const sender, thEventParams_t info);
thResult_t Button2_onClick(thObject * const sender, thEventParams_t info);

void thWin32App::OnCreate()
{
    using namespace local_data;
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

    menu1->Items.Add(TEXT("&open file"));
    menu1->Items[0]->OnClick = Menu2_FileOpen_onClick;
    menu1->Items.Add(TEXT("test name"));
    menu1->Items[1]->Text = TEXT("&Add mdi child");
    menu1->Items[1]->OnClick = Menu1_onClick;
    menu1->Items.Add(TEXT("&Window"));
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
    button2->Anchors.Top = false;
    button2->Anchors.Left = true;
    button2->Anchors.Right = true;

    mdiclient = new thMDIClient(form3, 0, 0);
    mdiclient->Width = form3->Width;
    mdiclient->Height = form3->Height - 30;
    mdiclient->Anchors.Right = true;
    mdiclient->Anchors.Bottom = true;

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
    using namespace local_data;
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

// This is common callback for ALL MDIChilds.
thResult_t MDIChild_onDestroy( thObject * pOwner, thEventParams_t info) {
    thResult_t result = 0;
    int position = 0;
    
    // Find MDIChild that was just destroyed.
    for ( const auto & i : local_data::mdiChilds)
    {
        if ( i.get() == pOwner)
        {
            local_data::mdiChilds.erase( local_data::mdiChilds.begin() + position);
            break;
        }
        ++position;
    }

    return result;
}

// This is common callback for ALL text MDIChilds.
thResult_t Text_MDIChild_onDestroy( thObject * pOwner, thEventParams_t info)
{
    thResult_t result = 0;

    int position = 0;
    
    // Find MDIChild that was just destroyed.
    for ( const auto & i : local_data::textMdiChilds)
    {
        if ( i->m_MdiChild.get() == pOwner)
        {
            local_data::textMdiChilds.erase( local_data::textMdiChilds.begin() + position);
            break;
        }
        ++position;
    }

    return result;
}

thResult_t Menu1_onClick( thObject * const sender, thEventParams_t info)
{
    // Create new thMDIChild object. Assign mdiclient as parent.
    local_data::mdiChilds.push_back( std::unique_ptr< thMDIChild>(
        new thMDIChild( local_data::mdiclient, CW_USEDEFAULT, CW_USEDEFAULT))
    );
    
    local_data::mdiChilds.back().get()->OnDestroy =  MDIChild_onDestroy; // bind onDestroy callback;
    local_data::mdiChilds.back().get()->Text =  TEXT("MDI");
    
    return 1;
}

thResult_t Toolbar1_onClick(thObject * const sender, thEventParams_t info){
    local_data::button->Text = TEXT("New button name");

    return 1;
}

// onClose event: instead of destorying form, hide it.
thResult_t Form_onClose(thObject * pOwner, thEventParams_t info)
{
    thResult_t tResult = 0;

    TH_ENTER_FUNCTION;
    {    
        thWindow * parent = dynamic_cast<thWindow*>(pOwner);

        parent->Hide();

        tResult = 1;    // returning 1 wont destroy object
    }
    TH_LEAVE_FUNCTION;
    return tResult;
}

// Closing main application form will close application.
thResult_t Form3_onDestroy(thObject * pOwner, thEventParams_t info)
{
    TH_ENTER_FUNCTION;
    thResult_t tResult = 0;

    local_data::mdiChilds.clear();

    myApp.Terminate(0); // Close the application.

    TH_LEAVE_FUNCTION;
    return tResult;
}

// open file in new MDI child int ASCII/UNICODE format
thResult_t Menu2_FileOpen_onClick(thObject * const sender, thEventParams_t info)
{
    thDialogFilterItem all( TEXT( "All"), TEXT(" *.*"));
    thDialogFilterItem text( TEXT( "Text"), TEXT(" *.TXT"));
    
    thOpenDialog openDialog;

    openDialog.Filter.Add( all);
    openDialog.Filter.Add( text);

    // Show Open File Dialog and if successed return true.
    if ( openDialog.Show( local_data::form3))
    {
        local_data::textMdiChilds.push_back(
            std::unique_ptr< local_data::sTextMDIdata_t>( new local_data::sTextMDIdata_t())
        );

        local_data::sTextMDIdata_t & textMdiData = *local_data::textMdiChilds.back().get();

        // Create MDI child Window
        textMdiData.m_MdiChild = std::unique_ptr< thMDIChild>(
            new thMDIChild( local_data::mdiclient)
            );

        // Create reference for ease-of-use
        thMDIChild & newMdiChild = *textMdiData.m_MdiChild.get();

        newMdiChild.Width = ( int)( (double) local_data::form3->Width * 0.8f);
        newMdiChild.Height = ( int)( (double) local_data::form3->Height * 0.8f);
        newMdiChild.OnDestroy = Text_MDIChild_onDestroy;

        // Create RichEdit component in new MDI child Window.
        textMdiData.m_RichEdit = std::unique_ptr< thRichEdit>(
            new thRichEdit( &newMdiChild, 0, 0)
            );

        // Create reference for ease-of-use
        thRichEdit & newRichEdit= *textMdiData.m_RichEdit.get();

        newRichEdit.Width = newMdiChild.Width - 20;
        newRichEdit.Height = newMdiChild.Height - 50;
        newRichEdit.Anchors.Right = true;
        newRichEdit.Anchors.Bottom = true;

        thFile file;

        file.Open(
            openDialog.FileName,
            thFile::DesiredAccess::generic_read,
            thFile::CreationDisposition::open_existing
        );

        if ( file.IsOpen())
        {
            // Set caption of MdiChild form.
            newMdiChild.Text = file.GetFileName();

            const auto file_size_in_bytes = file.GetFileSize();
            const size_t buffer_size_in_bytes = static_cast< size_t>( file_size_in_bytes);

            if ( buffer_size_in_bytes)
            {
                std::unique_ptr< uint8_t[]> read_buffer( new uint8_t[ buffer_size_in_bytes]);

                size_t number_of_bytes_read = 0;

                constexpr uint32_t file_read_without_error = 0;

                const uint32_t file_read_result = file.Read( read_buffer.get(), buffer_size_in_bytes, number_of_bytes_read);

                if ( file_read_without_error == file_read_result)
                {
                    BOOL isTextUnicode = IsTextUnicode( read_buffer.get(), static_cast< int>( number_of_bytes_read), NULL);

                    if ( isTextUnicode)
                    {
                        // interpret as wide string; - 10 bytes of BOM header
                        MSG_LOG( TEXT( "UNICODE"));

                        // Brute force memory copy, since its just an examples.
                        thString text( reinterpret_cast< TCHAR*>( read_buffer.get()), number_of_bytes_read);

                        #ifdef UNICODE
                            newRichEdit.Text = text;
                        #else
                            pNewchild->pRichEdit->Text = WStringToString(text);
                        #endif
                    }
                    else
                    {
                        // interpret as ANSI string
                        MSG_LOG( TEXT( "ASCII"));

                        // Brute force memory copy, since its just an examples.
                        std::string text( reinterpret_cast< char*>( read_buffer.get()), number_of_bytes_read);

                        #ifdef UNICODE
                            newRichEdit.Text = StringToWString(text);
                        #else
                            pNewchild->pRichEdit->Text = text;
                        #endif
                    }
                }
            }
        }
        
    }
    return 1;
}

thResult_t Menu2_onClick1(thObject * const sender, thEventParams_t info)
{
    local_data::mdiclient->Cascade();
    return 1;
}

thResult_t Menu2_onClick2(thObject * const sender, thEventParams_t info)
{
    local_data::mdiclient->TileHorizontal();
    return 1;
}

thResult_t Menu2_onClick3(thObject * const sender, thEventParams_t info)
{
    local_data::mdiclient->TileVertical();
    return 1;
}

thResult_t Menu2_onClick4(thObject * const sender, thEventParams_t info)
{
    local_data::mdiclient->ArrangeIcons();
    return 1;
}

thResult_t Menu2_onClick5(thObject * const sender, thEventParams_t info)
{
    local_data::form->Show();
    return 1;
}

thResult_t Menu2_onClick6(thObject * const sender, thEventParams_t info)
{
    local_data::form2->Show();
    return 1;
}

thResult_t ComboBox1_onSelChange(thObject * const sender, thEventParams_t info)
{
    switch ( local_data::combo1->Items.ItemIndex())
    {
    case 0:
        local_data::thListView1->SetView(thListView::eViewType_t::view_details);
        break;
    case 1:
        local_data::thListView1->SetView(thListView::eViewType_t::view_icon);
        break;
    case 2:
        local_data::thListView1->SetView(thListView::eViewType_t::view_list);
        break;
    case 3:
        local_data::thListView1->SetView(thListView::eViewType_t::view_smallicon);
        break;
    case 4:
        local_data::thListView1->SetView(thListView::eViewType_t::view_tile);
        break;
    default:
        // not used
        break;
    }
    return 1;
}



// callbacks
thResult_t Button_onClick(thObject * const sender, thEventParams_t info)
{
    TH_ENTER_FUNCTION;

    local_data::thListView1->Items.Add( local_data::edit1->Text);
    local_data::listbox1->Items.Add( local_data::edit1->Text);

    TH_LEAVE_FUNCTION;
    return 1;
}

thResult_t Button2_onClick(thObject * const sender, thEventParams_t info)
{
    TH_ENTER_FUNCTION;

    for ( int j = 0; j < local_data::thListView1->Items.GetCount(); j++)
    {
        for ( int i = 1; i < local_data::thListView1->Columns.GetCount(); i++)
        {
            local_data::thListView1->Items[ j]->SubItems[ i].SetText( NumToString( i));
        }
    }

    TH_LEAVE_FUNCTION;
    return 1;
}
