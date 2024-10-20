#include "thWin32App.h"

// Enable Visual Styles from Common Controls 6 (ComCtl32).
#pragma comment( linker, "/manifestdependency:\"type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' \
language='*'\"")


// Keep pointers as globals so it is easier to access different
// elements in callback. To see how to wrap everything in a class
// see other examples.
// In this example RAW pointers and manual memory management is used,
// but you can use smart pointers.
thForm* form;
thLabel* label;
thButton* button;

// Windows application entry point.
#if TH_DEBUG_LEVEL == 0
int WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
#else
 int main()
#endif
 {
     thWin32App app; // application instance

     // create main window
     form = new thForm();

     form->Width = 300;
     form->Height = 200;
     form->Text = TEXT("Application Example");

     // register callback needed to quit application when main window is destroyed
     form->OnDestroy = [](thObject*, thEventParams_t)
         {
             constexpr const thResult_t quit_application = 1;
             return quit_application;
         };

     // create label
     label = new thLabel(form);
     label->X = 100;
     label->Text = TEXT("hello");

     // create button
     button = new thButton(form);
     button->Text = TEXT("press me!");
     button->X = 100;
     button->Y = label->Height; // put button just bellow label

     // register callback when button is clicked
     button->OnClick = [](thObject * sender, thEventParams_t)
         {
            // you can get pointer to sender object
            // for example: you can have 1 callback for multiple buttons
            thButton * p = dynamic_cast<thButton*>(sender);

            if (p) {
                static bool state = false;

                p->Text = (state)?TEXT("press me!"): TEXT("press me!!!");
                label->Text = (state) ? TEXT("hello world") : TEXT("hello");

                state = !state;
            }

            return 1; // if callback has been served, return 1
         };

     // use the same callback for double click, so you can press button faster
     button->OnDoubleClicked = button->OnClick;

     form->Show();

     app.Run(); // this function exits when main window is destroyed and return 1

     delete label;
     delete button;
     delete form;

     return 0;
 }
