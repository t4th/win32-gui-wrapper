#include "thWin32App.h"

// Enable Visual Styles from Common Controls 6 (ComCtl32).
#pragma comment( linker, "/manifestdependency:\"type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' \
language='*'\"")

class MainWindow;
class SecondWindow;
class ThirdWindow;

// Define application as a whole.
class MyApplication : public thWin32App
{
    public:
        MyApplication();

        std::unique_ptr< MainWindow>      m_mainWindow;
        std::unique_ptr< SecondWindow>    m_secondWindow;
        std::unique_ptr< ThirdWindow>     m_thirdWindow;
};

// Define main program window. It is responsible for creating all window items and memory managment.
class MainWindow
{
    public:
        MainWindow() = delete;
        MainWindow( MyApplication & a_myApp);

        // Define MDI window with thRichEdit component.
        struct TextMdiData
        {
            std::unique_ptr< thMDIChild> m_MdiChild{};
            std::unique_ptr< thRichEdit> m_RichEdit{};
            std::unique_ptr< thLabel>    m_Label{};
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
        thResult_t CreateMdiChild_onClick( thObject * sender, thEventParams_t info);
        thResult_t MdiChild_onDestroy( thObject * pOwner, thEventParams_t info);
        thResult_t ShowSecondForm_onClick( thObject * sender, thEventParams_t info);
        thResult_t ShowThirdForm_onClick( thObject * sender, thEventParams_t info);
        thResult_t MenuCascade_onClick( thObject * sender, thEventParams_t info);
        thResult_t MenuHorizontal_onClick( thObject * sender, thEventParams_t info);
        thResult_t MenuVertical_onClick( thObject * sender, thEventParams_t info);
        thResult_t MenuArrange_onClick( thObject * sender, thEventParams_t info);
        thResult_t FileOpen_onClick( thObject * sender, thEventParams_t info);
};

// Define second program window. It is responsible for creating all window items and memory managment.
class SecondWindow
{
    public:
        SecondWindow() = delete;
        SecondWindow( MyApplication & a_myApp);

        // Store reference to MyApplication so callbacks can access other windows objects.
        MyApplication &                 m_myApp;
    
        std::unique_ptr< thForm>        m_mainForm;
        std::unique_ptr< thToolbar>     m_toolbar;
        std::unique_ptr< thRadioButton> m_radioButton0;
        std::unique_ptr< thRadioButton> m_radioButton1;
        std::unique_ptr< thCheckBox>    m_checkBox0;
        std::unique_ptr< thLabel>       m_label;
        
    private:
        thResult_t Form_onClose( thObject * pOwner, thEventParams_t info);
        thResult_t ToolbarButton_onClick( thObject * sender, thEventParams_t info);
};

// Define third program window. It is responsible for creating all window items and memory managment.
class ThirdWindow
{
    public:
        ThirdWindow() = delete;
        ThirdWindow( MyApplication & a_myApp);

        // Store reference to MyApplication so callbacks can access other windows objects.
        MyApplication &                 m_myApp;
        
        std::unique_ptr< thForm>        m_mainForm;
        std::unique_ptr< thComboBox>    m_comboBox;
        std::unique_ptr< thEditBox>     m_editBox;
        std::unique_ptr< thButton>      m_addItemButton;
        std::unique_ptr< thButton>      m_addSubtemButton;
        std::unique_ptr< thListBox>     m_listBox;
        std::unique_ptr< thListView>    m_listView;

        std::unique_ptr< thPopupMenu>   m_popupMenu;
        std::unique_ptr< thPopupMenu>   m_popupSubMenu;

    private:
        thResult_t Form_onClose( thObject * pOwner, thEventParams_t info);
        thResult_t ComboBox1_onSelChange( thObject * sender, thEventParams_t info);
        thResult_t AddItemButton_onClick( thObject * sender, thEventParams_t info);
        thResult_t AddSubtemButton_onClick(thObject * const sender, thEventParams_t info);
};

MyApplication::MyApplication()
{
    m_mainWindow =      std::make_unique< MainWindow>( *this);
    m_secondWindow =    std::make_unique< SecondWindow>( *this);
    m_thirdWindow =     std::make_unique< ThirdWindow>( *this);

    // Show main window when all initialization is done.
    m_mainWindow->m_mainForm->Show();
}

// Windows application entry point.
#if TH_DEBUG_LEVEL == 0
int WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
#else
 int main()
#endif
{
    MyApplication app;

    return app.Run();
}

MainWindow::MainWindow( MyApplication & a_myApp) : m_myApp{ a_myApp}
{
    // Create window submenu.
    m_windowsSubMenu = std::make_unique< thMenu>();

    m_windowsSubMenu->Items.Add( TEXT( "&Cascade"));
    m_windowsSubMenu->Items[ 0]->OnClick = std::bind( &MainWindow::MenuCascade_onClick, this, std::placeholders::_1, std::placeholders::_2);

    m_windowsSubMenu->Items.Add( TEXT( "&Horizontal tile"));
    m_windowsSubMenu->Items[ 1]->OnClick = std::bind( &MainWindow::MenuHorizontal_onClick, this, std::placeholders::_1, std::placeholders::_2);
            
    m_windowsSubMenu->Items.Add( TEXT( "-"));

    m_windowsSubMenu->Items.Add( TEXT( "&Vertical tile"));
    m_windowsSubMenu->Items[ 3]->OnClick = std::bind( &MainWindow::MenuVertical_onClick, this, std::placeholders::_1, std::placeholders::_2);

    m_windowsSubMenu->Items.Add( TEXT( "A&rrange icons"));
    m_windowsSubMenu->Items[ 4]->OnClick = std::bind( &MainWindow::MenuArrange_onClick, this, std::placeholders::_1, std::placeholders::_2);

    // Create main menu.
    m_mainMenu = std::make_unique< thMenu>();
            
    m_mainMenu->Items.Add( TEXT( "&Open file"));
    m_mainMenu->Items[ 0]->OnClick = std::bind( &MainWindow::FileOpen_onClick, this, std::placeholders::_1, std::placeholders::_2);

    m_mainMenu->Items.Add( TEXT( "&Mdi"));
    m_mainMenu->Items[ 1]->Text = TEXT( "&Add mdi child");
    m_mainMenu->Items[ 1]->OnClick = std::bind( &MainWindow::CreateMdiChild_onClick, this, std::placeholders::_1, std::placeholders::_2);

    m_mainMenu->Items.Add( TEXT( "&Window"));
    m_mainMenu->Items[ 2]->SubMenu = m_windowsSubMenu.get();

    m_mainMenu->Items.Add( TEXT( "&Show Second form"));
    m_mainMenu->Items[ 3]->OnClick = std::bind( &MainWindow::ShowSecondForm_onClick, this, std::placeholders::_1, std::placeholders::_2);

    m_mainMenu->Items.Add( TEXT( "Show &Third form"));
    m_mainMenu->Items[ 4]->OnClick = std::bind( &MainWindow::ShowThirdForm_onClick, this, std::placeholders::_1, std::placeholders::_2);

    // Create main window object.
    m_mainForm = std::make_unique< thForm>();
            
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
    m_openButton = std::make_unique< thButton>( m_mainForm.get(), 0, m_mainForm->Height - 30);
    m_openButton->Text = TEXT( "Open file...");
    m_openButton->Width = m_mainForm->Width;
    m_openButton->Height = 30;
    m_openButton->Anchors.Top = false;
    m_openButton->Anchors.Left = true;
    m_openButton->Anchors.Right = true;
    m_openButton->OnClick = std::bind( &MainWindow::FileOpen_onClick, this, std::placeholders::_1, std::placeholders::_2);

    // Create Mdi client.
    m_mdiClient = std::make_unique< thMDIClient>( m_mainForm.get());
    m_mdiClient->Width = m_mainForm->Width;
    m_mdiClient->Height = m_mainForm->Height - 30;
    m_mdiClient->Anchors.Right = true;
    m_mdiClient->Anchors.Bottom = true;
}

thResult_t MainWindow::CreateMdiChild_onClick( thObject * const sender, thEventParams_t info)
{
    // Create new thMDIChild object. Assign mdiclient as parent.
    m_emptyMdiChilds.push_back( std::make_unique< thMDIChild>( m_mdiClient.get()));

    m_emptyMdiChilds.back()->OnDestroy = std::bind( &MainWindow::MdiChild_onDestroy, this, std::placeholders::_1, std::placeholders::_2);
    m_emptyMdiChilds.back()->Text =  TEXT( "Empty MDI");
    
    return 1;
}

// This is common callback for ALL MdiChildren.
thResult_t MainWindow::MdiChild_onDestroy( thObject * pOwner, thEventParams_t info)
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

thResult_t MainWindow::ShowSecondForm_onClick( thObject * sender, thEventParams_t info)
{
    m_myApp.m_secondWindow->m_mainForm->Show();
    return 1;
}

thResult_t MainWindow::ShowThirdForm_onClick( thObject * sender, thEventParams_t info)
{
    m_myApp.m_thirdWindow->m_mainForm->Show();
    return 1;
}

thResult_t MainWindow::MenuCascade_onClick( thObject * sender, thEventParams_t info)
{
    m_mdiClient->Cascade();
    return 1;
}

thResult_t MainWindow::MenuHorizontal_onClick( thObject * sender, thEventParams_t info)
{
    m_mdiClient->TileHorizontal();
    return 1;
}

thResult_t MainWindow::MenuVertical_onClick( thObject * sender, thEventParams_t info)
{
    m_mdiClient->TileVertical();
    return 1;
}

thResult_t MainWindow::MenuArrange_onClick( thObject * sender, thEventParams_t info)
{
    m_mdiClient->ArrangeIcons();
    return 1;
}

thResult_t MainWindow::FileOpen_onClick( thObject * sender, thEventParams_t info)
{
    thDialogFilterItem all( TEXT( "All"), TEXT( " *.*"));
    thDialogFilterItem text( TEXT( "Text"), TEXT( " *.TXT"));
    
    thOpenDialog openDialog;

    openDialog.Filter.Add( all);
    openDialog.Filter.Add( text);

    // Show Open File Dialog and if successed return true.
    if ( openDialog.Show( m_mainForm.get()))
    {
        thFile file;

        file.Open( openDialog.FileName, thFile::DesiredAccess::generic_read, thFile::CreationDisposition::open_existing);

        if ( file.IsOpen())
        {
            // Create new TextMdiData object.
            m_textMdiChilds.push_back( { nullptr, nullptr } );

            TextMdiData & textMdiData = m_textMdiChilds.back();

            // Create MDI child Window
            textMdiData.m_MdiChild = std::make_unique< thMDIChild>( m_mdiClient.get());

            textMdiData.m_MdiChild->Width = ( int)( ( double) m_mdiClient->Width * 0.8f);
            textMdiData.m_MdiChild->Height = ( int)( ( double) m_mdiClient->Height * 0.8f);
            textMdiData.m_MdiChild->OnDestroy = std::bind( &MainWindow::MdiChild_onDestroy, this, std::placeholders::_1, std::placeholders::_2);

            // Create RichEdit component in new MDI child Window.
            textMdiData.m_RichEdit = std::make_unique< thRichEdit>( textMdiData.m_MdiChild.get());

            // Create reference for ease-of-use
            thRichEdit & newRichEdit= *textMdiData.m_RichEdit.get();

            newRichEdit.Width = textMdiData.m_MdiChild->Width - 20;
            newRichEdit.Height = textMdiData.m_MdiChild->Height - 50;
            newRichEdit.Anchors.Right = true;
            newRichEdit.Anchors.Bottom = true;

            // Set caption of MdiChild form.
            textMdiData.m_MdiChild->Text = file.GetFileName();

            // Create label with file path
            textMdiData.m_Label = std::make_unique< thLabel>( textMdiData.m_MdiChild.get());
            textMdiData.m_Label->X = 10;
            textMdiData.m_Label->Y = newRichEdit.Height + 10;
            textMdiData.m_Label->Width = newRichEdit.Width;
            textMdiData.m_Label->Text = file.GetFilePath();
            textMdiData.m_Label->Anchors.Top = false;

            // Open file content.
            const size_t buffer_size_in_bytes = static_cast< size_t>( file.GetFileSize());

            if ( buffer_size_in_bytes)
            {
                std::unique_ptr< uint8_t[]> read_buffer( new uint8_t[ buffer_size_in_bytes]);

                size_t number_of_bytes_read = 0;

                if ( true == file.Read( read_buffer.get(), buffer_size_in_bytes, number_of_bytes_read))
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

SecondWindow::SecondWindow( MyApplication & a_myApp) : m_myApp{ a_myApp}
{
    thForm & parentWindow = *m_myApp.m_mainWindow->m_mainForm;

    m_mainForm = std::make_unique< thForm>( &parentWindow);
    m_mainForm->Width = 300;
    m_mainForm->Height = 300;
    m_mainForm->X = parentWindow.X + 200L;
    m_mainForm->Y = 100;
    m_mainForm->Text = TEXT( "Second Form");
    m_mainForm->Resizable = false;
    m_mainForm->OnClose = std::bind( &SecondWindow::Form_onClose, this, std::placeholders::_1, std::placeholders::_2);

    m_toolbar = std::make_unique< thToolbar>( m_mainForm.get());
    m_toolbar->Items.Add( TEXT( "Button 0"));
    m_toolbar->Items[ 0]->OnClick = std::bind( &SecondWindow::ToolbarButton_onClick, this, std::placeholders::_1, std::placeholders::_2);

    m_toolbar->Items.Add( TEXT( "fake name"));
    m_toolbar->Items[ 1]->Text = TEXT( "Button 1");
    m_toolbar->Items[ 1]->OnClick = std::bind( &SecondWindow::ToolbarButton_onClick, this, std::placeholders::_1, std::placeholders::_2);

    m_toolbar->Items.Add( TEXT( "Button 2"));
    m_toolbar->Items[ 2]->OnClick = std::bind( &SecondWindow::ToolbarButton_onClick, this, std::placeholders::_1, std::placeholders::_2);
            
    m_radioButton0 = std::make_unique< thRadioButton>( m_mainForm.get(), 5, 35);
    m_radioButton0->Text = TEXT( "French Script MT");
    m_radioButton0->Width = 200;
    m_radioButton0->Font.SetName( TEXT( "French Script MT"));
    m_radioButton0->Font.SetSize( 20);
            
    m_radioButton1 = std::make_unique< thRadioButton>( m_mainForm.get(), 5, m_radioButton0->Y * 2);
    m_radioButton1->Text = TEXT( "Gill Sans Ultra Bold Condensed");
    m_radioButton1->Width = 200;
    m_radioButton1->Font.SetName( TEXT( "Gill Sans Ultra Bold Condensed"));
    m_radioButton1->Font.SetSize( 10);

    m_checkBox0 = std::make_unique< thCheckBox>( m_mainForm.get(), 5, m_radioButton0->Y * 3);
    m_checkBox0->Text = TEXT( "Wide Latin");
    m_checkBox0->Font.SetName( TEXT( "Wide Latin"));
    m_checkBox0->Font.SetSize( 13);
    m_checkBox0->Width = 200;

    m_label = std::make_unique< thLabel>( m_mainForm.get(), 5, m_radioButton0->Y * 4);
    m_label->Text = TEXT( "Some random text");
    m_label->Width = 200;
}

// Hide window instead of destroying
thResult_t SecondWindow::Form_onClose( thObject * pOwner, thEventParams_t info)
{
    thWindow * parent = dynamic_cast< thWindow*>( pOwner);
    parent->Hide();

    return 1; // Returning 1 wont destroy object.
}

thResult_t SecondWindow::ToolbarButton_onClick( thObject * sender, thEventParams_t info)
{
    thToolbarItem * const sender_button = dynamic_cast< thToolbarItem *> ( sender);

    size_t number_of_toolbar_buttons = m_toolbar->Items.Size();

    for ( auto i = 0; i < number_of_toolbar_buttons; ++i)
    {
        if ( sender_button == m_toolbar->Items[ i])
        {
            m_label->Text =  TEXT( "Button ");
            m_label->Text += NumToString( i);
            m_label->Text += TEXT( " pressed");
            break;
        }
    }

    return 1;
}

ThirdWindow::ThirdWindow( MyApplication & a_myApp) : m_myApp{ a_myApp}
{
    thForm & parentWindow = *m_myApp.m_mainWindow->m_mainForm;

    // Create main window.
    m_mainForm = std::make_unique< thForm>( &parentWindow);
    m_mainForm->Width = 500;
    m_mainForm->Height = 350;
    m_mainForm->X = 500;
    m_mainForm->Y = 100;
    m_mainForm->Text = TEXT( "Third window");
    m_mainForm->OnClose = std::bind( &ThirdWindow::Form_onClose, this, std::placeholders::_1, std::placeholders::_2);
    
    // Create combo box.
    m_comboBox = std::make_unique< thComboBox>( m_mainForm.get(), 2, 2);
    m_comboBox->Width = m_mainForm->Width - 4;
    m_comboBox->Height = 200;
    m_comboBox->Items.Add( TEXT( "Details"));
    m_comboBox->Items.Add( TEXT( "Icon"));
    m_comboBox->Items.Add( TEXT( "List"));
    m_comboBox->Items.Add( TEXT( "Small icon"));
    m_comboBox->Items.Add( TEXT( "Tile"));
    m_comboBox->Items.SetItemIndex( 0);
    m_comboBox->Anchors.Right = true;
    m_comboBox->OnSelectChange = std::bind( &ThirdWindow::ComboBox1_onSelChange, this, std::placeholders::_1, std::placeholders::_2);
    
    // Create edit box.
    m_editBox = std::make_unique< thEditBox>( m_mainForm.get(), 2, 4 + m_comboBox->Height);
    m_editBox->Text = TEXT( "name1");
    
    // Create button.
    m_addItemButton = std::make_unique< thButton>( m_mainForm.get(), 2, 4 + m_editBox->Y + m_editBox->Height);
    m_addItemButton->Text = TEXT( "Item");
    m_addItemButton->OnClick = std::bind( &ThirdWindow::AddItemButton_onClick, this, std::placeholders::_1, std::placeholders::_2);
    
    // Create button.
    m_addSubtemButton = std::make_unique< thButton>( m_mainForm.get(), 2, 4 + m_addItemButton->Y + m_addItemButton->Height);
    m_addSubtemButton->OnClick = std::bind( &ThirdWindow::AddSubtemButton_onClick, this, std::placeholders::_1, std::placeholders::_2);
    m_addSubtemButton->Text = TEXT( "Subitem");
    
    // Create list box.
    m_listBox = std::make_unique< thListBox>( m_mainForm.get(), 2, 4 + m_addSubtemButton->Y + m_addSubtemButton->Height);
    m_listBox->Width = m_addSubtemButton->Width;
    m_listBox->Height = 200;

    // Create popup submenu
    m_popupSubMenu = std::make_unique< thPopupMenu>();
    m_popupSubMenu->Items.Add( TEXT( "Sub option 0"));
    m_popupSubMenu->Items.Add( TEXT( "Sub option 1"));

    // Create popup menu
    m_popupMenu = std::make_unique< thPopupMenu>();
    m_popupMenu->Items.Add( TEXT( "Sub menu"));
    m_popupMenu->Items[ 0]->SubMenu = m_popupSubMenu.get();
    m_popupMenu->Items.Add( TEXT( "Add item"));
    m_popupMenu->Items[ 1]->OnClick = std::bind( &ThirdWindow::AddItemButton_onClick, this, std::placeholders::_1, std::placeholders::_2);

    // Create list view
    m_listView = std::make_unique< thListView>( m_mainForm.get(), 2 + m_addItemButton->X + 1 + m_addItemButton->Width, 30);
    m_listView->Width = 400;
    m_listView->Height = 300;
    m_listView->Anchors.Right = true;
    m_listView->Anchors.Bottom = true;
    m_listView->Columns.Add( TEXT( "Column1"));
    m_listView->Columns.Add( TEXT( "Column2"));
    m_listView->Columns.Add( TEXT( "Column3"));
    m_listView->Columns.Add( TEXT( "Column4"));
    m_listView->SetView( thListView::eViewType_t::view_details);
    m_listView->PopupMenu = m_popupMenu.get();
}

// Hide window instead of destroying
thResult_t ThirdWindow::Form_onClose( thObject * pOwner, thEventParams_t info)
{
    thWindow * parent = dynamic_cast< thWindow*>( pOwner);
    parent->Hide();

    return 1; // Returning 1 wont destroy object.
}

thResult_t ThirdWindow::ComboBox1_onSelChange( thObject * sender, thEventParams_t info)
{
    switch ( m_comboBox->Items.ItemIndex())
    {
        case 0: m_listView->SetView( thListView::eViewType_t::view_details); break;
        case 1: m_listView->SetView( thListView::eViewType_t::view_icon); break;
        case 2: m_listView->SetView( thListView::eViewType_t::view_list); break;
        case 3: m_listView->SetView( thListView::eViewType_t::view_smallicon); break;
        case 4: m_listView->SetView( thListView::eViewType_t::view_tile); break;
    }

    return 1;
}

thResult_t ThirdWindow::AddItemButton_onClick( thObject * sender, thEventParams_t info)
{
    TH_ENTER_FUNCTION;

    m_listView->Items.Add( m_editBox->Text);
    m_listBox->Items.Add( m_editBox->Text);

    TH_LEAVE_FUNCTION;
    return 1;
}

thResult_t ThirdWindow::AddSubtemButton_onClick(thObject * const sender, thEventParams_t info)
{
    TH_ENTER_FUNCTION;

    for ( int row = 0; row < m_listView->Items.GetCount(); ++row)
    {
        for ( int col = 1; col < m_listView->Columns.GetCount(); ++col)
        {
            m_listView->Items[ row].SubItems[ col].SetText( NumToString( col));
        }
    }

    TH_LEAVE_FUNCTION;
    return 1;
}
