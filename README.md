## Experimental win32 GUI wrapper in C++
This is fun little project I did when learning win32 GUI API and C++.
I was pretty satisfied with the result, but it was just **prove of concept** learning material, and it was never supposed to reach 1.0.
Still, it is now good source of some win32 API examples. I remember being the most proud of working re-sizing and MDI :).

## Update
21/03/2021
Due to some new stars I added some architecture overview, smart pointers where applicable and calculator example. Still, this project was done on the fly and it is obvious when reading the source code. There are no clear instance owners and memory leaks are guaranteed.

1/7/2021
I have created this project when I was pretty much C++ beginner and Win32 was already called ancient. Still, I got some stars recently, which motivated me to check it again :). Since I am way more experienced than 10 years ago, I found quite the amount of bugs and bad practices, which hopefully I will fix in my spare time.

## Goal
My original goal was to wrap native Windows C code with C++ classes like in Borland C++ Builder (now Embarcadero).

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
- Lots of bad programming habits issues: naming and capsulation
- C++ API is mixed with native Windows types
- Only basic features (missing accelerators support, etc.)
- No good error handling in operator overloads, since I didn't want to use try-catch like Borland C++
- C++ Builder, QT or other popular frameworks use code generation for static object creation. In my code I had to workaround such problems with dynamic objects and elements, like event callbacks

## TODO
- architecture overview
- documentation
- ~~create examples (calculator)~~
- implement proper error handling
- change function pointer to std::function
- change raw pointers to smart pointers

## How to use
Project is using external components which are built into static libraries. Win32 Gui itself is also built into library.
Since this is Windows only, use free Visual Studio 2019 to open solution file Win32_gui_wrapper.sln:
- select solution platform
- build (ctrl+shift+b) all and Run (F5)
- You must select example project as startup project for Run (F5) to work

Usage examples can be selected in solution explorer from within VS.

## Example
Example int32 calculator.
![Alt arch](/doc/calc_example.png?raw=true)

## External
Program uses Scintilla as thRichEdit component and SimpleGrid and thStringGrid.

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
