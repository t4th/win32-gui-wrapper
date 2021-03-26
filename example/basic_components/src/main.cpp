#include "thWin32App.h"

// Enable Visual Styles from Common Controls 6 (ComCtl32).
#pragma comment( linker, "/manifestdependency:\"type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' \
language='*'\"")

thWin32Logger   g_logger;

class MainWindow;
class SecondWindow;

class MyApplication : public thWin32App
{
    public:
        MyApplication();

        std::unique_ptr< MainWindow>      m_mainWindow;
        std::unique_ptr< SecondWindow>    m_secondWindow;

};
class MainWindow
{
    public:
        MainWindow() = delete;
        MainWindow( MyApplication & a_myApp) : m_myApp{ a_myApp}
        {
            // Create window submenu.
            m_windowsSubMenu = std::unique_ptr< thMenu>( new thMenu);

            m_windowsSubMenu->Items.Add( TEXT( "&Cascade"));
            m_windowsSubMenu->Items[ 0]->OnClick = std::bind(
                &MainWindow::MenuCascade_onClick,
                this,
                std::placeholders::_1,
                std::placeholders::_2
                );

            m_windowsSubMenu->Items.Add( TEXT( "&Horizontal tile"));
            m_windowsSubMenu->Items[ 1]->OnClick = std::bind(
                &MainWindow::MenuHorizontal_onClick,
                this,
                std::placeholders::_1,
                std::placeholders::_2
                );
            
            m_windowsSubMenu->Items.Add( TEXT( "-"));

            m_windowsSubMenu->Items.Add( TEXT( "&Vertical tile"));
            m_windowsSubMenu->Items[ 3]->OnClick = std::bind(
                &MainWindow::MenuVertical_onClick,
                this,
                std::placeholders::_1,
                std::placeholders::_2
                );

            m_windowsSubMenu->Items.Add( TEXT( "A&rrange icons"));
            m_windowsSubMenu->Items[ 4]->OnClick = std::bind(
                &MainWindow::MenuArrange_onClick,
                this,
                std::placeholders::_1,
                std::placeholders::_2
                );

            // Create main menu.
            m_mainMenu = std::unique_ptr< thMenu>( new thMenu);
            
            m_mainMenu->Items.Add( TEXT( "&Open file"));
            m_mainMenu->Items[ 0]->OnClick = std::bind(
                &MainWindow::FileOpen_onClick,
                this,
                std::placeholders::_1,
                std::placeholders::_2
                );

            m_mainMenu->Items.Add( TEXT( "&Mdi"));
            m_mainMenu->Items[ 1]->Text = TEXT( "&Add mdi child");
            m_mainMenu->Items[ 1]->OnClick = std::bind(
                &MainWindow::CreateMdiChild_onClick,
                this,
                std::placeholders::_1,
                std::placeholders::_2
                );

            m_mainMenu->Items.Add( TEXT( "&Window"));
            m_mainMenu->Items[ 2]->SubMenu = m_windowsSubMenu.get();

            m_mainMenu->Items.Add( TEXT( "&Second form"));
            //m_mainMenu->Items[ 3]->OnClick = Menu2_onClick5;

            m_mainMenu->Items.Add( TEXT( "&Third form"));
            //m_mainMenu->Items[ 4]->OnClick = Menu2_onClick6;

            // Create main window object.
            m_mainForm = std::unique_ptr< thForm>( new thForm());
            
            m_mainForm->Width = 800;
            m_mainForm->Height = 800;
            m_mainForm->X = 50;
            m_mainForm->Y = 50;
            m_mainForm->Text = TEXT( "Application Example");
            m_mainForm->SetMenu( m_mainMenu.get());
            m_mainForm->OnDestroy = []( thObject *, thEventParams_t)
            {
                // OnDestroy callback must return 1 to close application.
                constexpr const thResult_t quit_application = 1;
                return quit_application;
            };

            // Main button
            m_openButton = std::unique_ptr< thButton>( new thButton( m_mainForm.get(), 0, m_mainForm->Height - 30));
            m_openButton->Text = TEXT( "Open file...");
            m_openButton->Width = m_mainForm->Width;
            m_openButton->Height = 30;
            m_openButton->Anchors.Top = false;
            m_openButton->Anchors.Left = true;
            m_openButton->Anchors.Right = true;
            m_openButton->OnClick = std::bind( &MainWindow::FileOpen_onClick, this, std::placeholders::_1, std::placeholders::_2);

            // Create Mdi client.
            m_mdiClient = std::unique_ptr< thMDIClient>( new thMDIClient( m_mainForm.get(), 0, 0));
            m_mdiClient->Width = m_mainForm->Width;
            m_mdiClient->Height = m_mainForm->Height - 30;
            m_mdiClient->Anchors.Right = true;
            m_mdiClient->Anchors.Bottom = true;

            // Show main window when all initialization is done.
            m_mainForm->Show();
        }

        // Define MDI window with thRichEdit component.
        struct TextMdiData
        {
            std::unique_ptr< thMDIChild> m_MdiChild{};
            std::unique_ptr< thRichEdit> m_RichEdit{};
        };

        // Store reference to MyApplication so callbacks can access other windows objects.
        MyApplication &                                 m_myApp;

        std::unique_ptr< thForm>                        m_mainForm;

        std::unique_ptr< thMenu>                        m_mainMenu;
        std::unique_ptr< thMenu>                        m_windowsSubMenu;

        std::unique_ptr< thMDIClient>                   m_mdiClient;
        std::unique_ptr< thButton>                      m_openButton;

        std::vector< std::unique_ptr< thMDIChild>>      m_emptyMdiChilds;
        std::vector< TextMdiData>                       m_textMdiChilds;

    private:
        thResult_t CreateMdiChild_onClick( thObject * const sender, thEventParams_t info)
        {
            // Create new thMDIChild object. Assign mdiclient as parent.
            m_emptyMdiChilds.push_back( std::unique_ptr< thMDIChild>(
                new thMDIChild( m_mdiClient.get()))
            );
    
            m_emptyMdiChilds.back()->OnDestroy = std::bind(
                &MainWindow::MdiChild_onDestroy,
                this,
                std::placeholders::_1,
                std::placeholders::_2
                );

            m_emptyMdiChilds.back()->Text =  TEXT( "MDI");
    
            return 1;
        }

        // This is common callback for ALL MdiChildren.
        thResult_t MdiChild_onDestroy( thObject * pOwner, thEventParams_t info)
        {
            int position = 0;
    
            // Find MDIChild that was just destroyed.
            for ( const auto & i : m_textMdiChilds)
            {
                if ( i.m_MdiChild.get() == pOwner)
                {
                    m_textMdiChilds.erase( m_textMdiChilds.begin() + position);
                    break;
                }
                ++position;
            }

            return 0;
        }
        
        thResult_t ShowSecondForm_onClick( thObject * sender, thEventParams_t info)
        {
            return 1;
        }

        thResult_t MenuCascade_onClick( thObject * sender, thEventParams_t info)
        {
            m_mdiClient->Cascade();
            return 1;
        }

        thResult_t MenuHorizontal_onClick( thObject * sender, thEventParams_t info)
        {
            m_mdiClient->TileHorizontal();
            return 1;
        }

        thResult_t MenuVertical_onClick( thObject * sender, thEventParams_t info)
        {
            m_mdiClient->TileVertical();
            return 1;
        }

        thResult_t MenuArrange_onClick( thObject * sender, thEventParams_t info)
        {
            m_mdiClient->ArrangeIcons();
            return 1;
        }

        thResult_t Menu2_onClick5_onClick( thObject * sender, thEventParams_t info)
        {
            m_mdiClient->Show();
            return 1;
        }

        thResult_t Menu2_onClick6_onClick(thObject * sender, thEventParams_t info)
        {
            m_mdiClient->Show();
            return 1;
        }

        thResult_t FileOpen_onClick( thObject * sender, thEventParams_t info);
};

class SecondWindow
{
    public:
        SecondWindow() = delete;
        SecondWindow( MyApplication & a_myApp) : m_myApp{ a_myApp}
        {
            thForm & mainForm = *m_myApp.m_mainWindow.get()->m_mainForm;

            m_mainForm = std::unique_ptr< thForm>( new thForm( &mainForm));

            m_mainForm->Width = 300;
            m_mainForm->Height = 300;
            m_mainForm->X = mainForm.Width + 520;
            m_mainForm->Y = 100;
            m_mainForm->Text = TEXT( "Second Form");
            m_mainForm->Resizable = false;
            m_mainForm->OnClose = std::bind(
                &SecondWindow::Form_onClose,
                this,
                std::placeholders::_1,
                std::placeholders::_2
                );


            //tb1 = new thToolbar(form2, 0, 0);
            //tb1->Items.Add(TEXT("destroyss"));
            //tb1->Items[0]->Text = TEXT("destroy");
            //tb1->Items[0]->OnClick = Toolbar1_onClick;
            //
            //tb1->Items.Add(TEXT("test2"));
            //tb1->Items[1]->Text = TEXT("test222");
            //
            //tb1->Items.Add(TEXT("test3"));
            ////tb1->Items[2]->Text = TEXT("test3");
            ////tb1->Items[2]->OnClick = Button2_onClick;
            //
            ////button2 = new thButton(tb1, 70, 0);
            //
            //rb1 = new thRadioButton(form2, 5, 35 + button->Height + 5);
            //rb1->Width = 200;
            //rb1->Font.SetName(TEXT("Times New Roman"));
            //rb1->Font.SetSize(10);
            //rb2 = new thRadioButton(form2, 5, rb1->Y * 2);
            //rb2->Font.SetName(TEXT("Gill Sans Ultra Bold Condensed"));
            //rb2->Font.SetSize(10);
            //rb2->Width = 200;
            //cb1 = new thCheckBox(form2, 5, rb1->Y * 3);
            //cb1->Font.SetName(TEXT("Wide Latin"));
            //cb1->Font.SetSize(10);
            //cb1->Width = 200;
        }

    private:
        // Store reference to MyApplication so callbacks can access other windows objects.
        MyApplication &                 m_myApp;
    
        std::unique_ptr< thForm>        m_mainForm;
        std::unique_ptr< thToolbar>     m_toolbar;
        std::unique_ptr< thRadioButton> m_radioButton0;
        std::unique_ptr< thRadioButton> m_radioButton1;
        std::unique_ptr< thCheckBox>    m_checkBox0;

        // Dont destory window, but just hide it.
        thResult_t Form_onClose( thObject * pOwner, thEventParams_t info)
        {
            TH_ENTER_FUNCTION;

            thWindow * parent = dynamic_cast< thWindow*>( pOwner);
            parent->Hide();

            TH_LEAVE_FUNCTION;
            return 1; // returning 1 wont destroy object;
        }
};

class ThirdWindow;

MyApplication::MyApplication()
{
    m_mainWindow = std::unique_ptr< MainWindow>( new MainWindow{ *this});
    m_secondWindow = std::unique_ptr< SecondWindow>( new SecondWindow{ *this});
}

// Windows application entry point.
int main()
{
    MyApplication app;

    return app.Run();
}

#if 0

namespace local_data
{
    // TODO: Make these smart pointers.
    thForm *        form2 = 0;
    thForm *        form3 = 0;

    thListView *    thListView1 = 0;
    thButton *      button = 0;
    thButton *      button4 = 0;
    thButton *      button2 = 0;
    thButton *      button3 = 0;
    thRadioButton * rb1 = 0;
    thRadioButton * rb2 = 0;
    thCheckBox *    cb1 = 0;
    thPopupMenu *   pop1 = 0;
    thPopupMenu *   pop2 = 0;
    thLabel *       label1 = 0;
    thComboBox *    combo1 = 0;
    thToolbar *     tb1 = 0;
    thEditBox *     edit1 = 0;
    thListBox *     listbox1 = 0;
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



    // popup menus
    pop1 = new thPopupMenu();
    pop1->Items.Add(TEXT("stuff"));
    pop1->Items.Add(TEXT("later"));
    pop2 = new thPopupMenu();
    pop2->Items.Add(TEXT("options"));
    pop2->Items.Add(TEXT("change"));
    pop2->Items[0]->SubMenu = pop1;
    //   pop2->Items[0]->SubMenu = 0;


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


thResult_t Toolbar1_onClick(thObject * const sender, thEventParams_t info){
    local_data::button->Text = TEXT("New button name");

    return 1;
}



thResult_t Toolbar1_onClick(thObject * const sender, thEventParams_t info){
    local_data::button->Text = TEXT("New button name");

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
#endif

thResult_t MainWindow::FileOpen_onClick( thObject * sender, thEventParams_t info)
{
    thDialogFilterItem all( TEXT( "All"), TEXT(" *.*"));
    thDialogFilterItem text( TEXT( "Text"), TEXT(" *.TXT"));
    
    thOpenDialog openDialog;

    openDialog.Filter.Add( all);
    openDialog.Filter.Add( text);

    // Show Open File Dialog and if successed return true.
    if ( openDialog.Show( m_mainForm.get()))
    {
        m_textMdiChilds.push_back( { nullptr, nullptr } );

        TextMdiData & textMdiData = m_textMdiChilds.back();

        // Create MDI child Window
        textMdiData.m_MdiChild = std::unique_ptr< thMDIChild>(
            new thMDIChild( m_mdiClient.get())
            );

        textMdiData.m_MdiChild->Width = ( int)( ( double) m_mdiClient->Width * 0.8f);
        textMdiData.m_MdiChild->Height = ( int)( ( double) m_mdiClient->Height * 0.8f);
        textMdiData.m_MdiChild->OnDestroy = std::bind(
                &MainWindow::MdiChild_onDestroy,
                this,
                std::placeholders::_1,
                std::placeholders::_2
                );

        // Create RichEdit component in new MDI child Window.
        textMdiData.m_RichEdit = std::unique_ptr< thRichEdit>(
            new thRichEdit( textMdiData.m_MdiChild.get(), 0, 0)
            );

        // Create reference for ease-of-use
        thRichEdit & newRichEdit= *textMdiData.m_RichEdit.get();

        newRichEdit.Width = textMdiData.m_MdiChild->Width - 20;
        newRichEdit.Height = textMdiData.m_MdiChild->Height - 50;
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
            textMdiData.m_MdiChild->Text = file.GetFileName();

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