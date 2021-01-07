#include "thWin32App.h"
#include "thWindow.h"
#include "thMDIChild.h"

// TODO: Fix this workarounds. Those two global variables should be held in thWin32App.
// Note: Win32 application assumes there is always only one MDI client object.
extern thMDIClient *            g_pMDI_client;
extern std::vector< thForm*>    g_forms;

namespace
{
    void translateMessage( MSG a_message)
    {
            BOOL message_translated = FALSE;

            // Capture default shortcuts for MDI client (like ctrl + tab).
            if ( g_pMDI_client)
            {
                HWND mdi_client_handle = g_pMDI_client->GetHandle();
            
                message_translated = TranslateMDISysAccel( mdi_client_handle, &a_message);
            }

            if ( FALSE == message_translated)
            {
                // Each thForm key press must be translated separately.
                // Otherwise TAB key wont switch Focus, etc.
                for ( const auto i: g_forms)
                {
                    HWND form_handle = i->GetHandle();
                    message_translated = IsDialogMessage( form_handle, &a_message);

                    if ( FALSE != message_translated)
                    {
                        break;
                    }
                }
            }

            if ( FALSE == message_translated)
            {
                TranslateMessage( &a_message);
                DispatchMessage( &a_message);
            }
    }
}

void thWin32App::Terminate( int a_nExitCode = 0)
{
    TH_ENTER_FUNCTION;

    ::PostQuitMessage( a_nExitCode);

    TH_LEAVE_FUNCTION;
}

int thWin32App::Run()
{
    TH_ENTER_FUNCTION;

    bool        exit_application = false;
    MSG         message = { 0 };

    this->OnCreate();

    while ( false == exit_application)
    {
        BOOL is_message_received = GetMessage( &message, NULL, 0, 0);

        switch ( is_message_received)
        {
        case 0:
            // WM_QUIT message.
            exit_application = true;
            break;
        case -1:
            // This is hilarious, but win32 BOOL can actually return -1,
            // which indicate that error occurred.
            break;
        default:
            // Normal message.
            translateMessage( message);
            break;
        }
    };

    TH_LEAVE_FUNCTION;

    return 0;
}
