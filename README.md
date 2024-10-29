## Experimental win32 GUI wrapper in C++
This is a fun little project that I created around 2011, while I was learning windows native GUI and C++.
Even though it is just a **prove of concept** learning material that was never supposed to reach 1.0, it is still a good source of win32 API usage examples.
I remember being the most proud about re-sizing of dynamically created objects and working MDI :).


## Goal
Normally win32 applications are built around sequential message dispatcher.
Inspired by Borland C++ Builder I decided to make C++ event driven wrapper.

For example:

```c++
thForm *   form = 0;
thButton * button = 0

form = new thForm( 0, 0, 0); // Parent, X-position, Y-position
form->Width = 800;
form->Height = 800;
form->X = 50;
form->Y = 50;
form->Text = L"Window caption name";
form->SetMenu( menu1);
form->OnDestroy = form_onDestroy; // register callback

button = new thButton( form, 0, form->Height - 30);
button->Width = form->Width;
button->Height = 30;
button->Anchors.Top = false;
button->Anchors.Left = true;
button->Anchors.Right = true;
button->OnClick = button_onClick;  // register callback

form->Show();
```

##### Good
- It looks nice and is intuitive to use
- As native GUI it is very responsive

##### Bad
- C++ API is mixed with native Windows types
- Only basic features (missing accelerators support, etc.)
- No good error handling in operator overloads, since I didn't want to use try-catch like Borland C++
- C++ Builder, QT or other popular frameworks use code generation for static object creation. In my code I had to workaround such problems with dynamic objects and elements, like event callbacks

## TODO
- ~~architecture overview~~
- documentation
- ~~create examples (calculator)~~
- implement proper error handling
- ~~change function pointer to std::function~~
- change raw pointers to smart pointers

## How to use
Project is using external components which are built into static libraries. Win32 Gui itself is also built into library.

Since this is Windows only, use free Visual Studio Community to open solution file Win32_gui_wrapper.sln:
- select solution platform (like x64)
- You **MUST** select example project as startup project for Run (F5) to work (right mouse button on project name in solution explorer, andchoose "Set as startup project" in the menu)
- build (ctrl+shift+b) all and Run (F5)
- amount of logs is configured with TH_DEBUG_LEVEL in thConfig.h. Set it to 1 for optimal logs and 2 for ALL logs. It can slow application startup due to amount of logs.

Note1: win32 code is using many different static libraries (configuration-properties->linker->input):
Win32_gui.lib;SciLexer.lib;imm32.lib;winmm.lib;Ws2_32.lib
Note2: the project is using C++17

I have also added project template in **example\solution_template** which is building code directly from source (not as a lib). This project is not including thObjects based on external source: thRichEdit and thStringGrid.

### Use in your project
Easiest way is to copy example project and modify.

The other way is to just build Win32_gui project into static LIB and then adding it with other required dependencies into desired project.
Simplest window:

```c++
#include "thWin32App.h" // master header of win32 gui wrapper

int main()
{
    thWin32App app; // application instance
    
    thForm * form = new thForm();

    form->Width = 300;
    form->Height = 200;
    form->Text = TEXT( "Application Example"); // window caption

    // register callback needed to quit application when main window is destroyed
    form->OnDestroy = []( thObject *, thEventParams_t)
    {
        constexpr const thResult_t quit_application = 1;
        return quit_application;
    };

    form->Show();

    app.Run(); // this function exits when main window is destroyed and return 1

    delete form;

    return 0;
}
```

## Example
To try examples, in Solution Explorer select choosen one as Startup Project and press F5:
![Alt arch](/doc/doc1.png?raw=true)

Or open **example\solution_template** to experiment from scratch.

### Basic components
Example using different kind of windows objects, like: MDI, buttons, combobox, etc.
![Alt arch](/doc/basic_example.png?raw=true)

### Calculator
Example int32 calculator.
![Alt arch](/doc/calc_example.png?raw=true)

### solution_template
Located in **example\solution_template** is an empty, buildable from source template (not a static lib) to experiment upon.

## External
Program uses Scintilla as thRichEdit component and SimpleGrid as thStringGrid.

## Architecture
This is architecture overview just to have an idea what is going on.

All visual components are based on thWindow class, which is handling common functionalities.
![Alt arch](/doc/arch_1.png?raw=true)

![Alt arch](/doc/arch_2.png?raw=true)

thWindow inherit from thObject so all instances created during runtime can be factorized.

There are also visual components that are not windows:
![Alt arch](/doc/arch_3.png?raw=true)

And there are classes that are not thObjects at all:
![Alt arch](/doc/arch_6.png?raw=true)

Parameters are mostly handled my thParam.
![Alt arch](/doc/arch_4.png?raw=true)

Some objects though need specialized parameters, for example thListView:
![Alt arch](/doc/arch_5.png?raw=true)
