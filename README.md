## Experimental win32 GUI wrapper in C++
This is fun little project I did when learning win32 GUI API and C++.
I was pretty satisfied with the result, but it was just prove of concept learning material, and it was never supposed to reach 1.0.
Still it is now good source of some win32 gui API examples. I remember being the most proud of re-sizing and MDI :).

## Update
1/7/2021
I have created this code when I was pretty much C++ beginner and Win32 was already called ancient and even though I got some start ;).
Since I made it was almost almost 10 years ago my coding style and knowledge evolved accordingly and I decided to clean it up a bit.
There are quite the bugs and bad practices in this project, but hopefully I will clean some in free time.

## Goal
My original goal was to wrap native Windows C code with C++ classes like in Borland C++ Builder (now Embarcadero).

For example:

```c++
thForm *   form = 0;
thButton * button = 0

form = new thForm(0, 0, 0);
form->Width = 800;
form->Height = 800;
form->X = 50;
form->Y = 50;
form->Text = L"Window caption name";
form->SetMenu(menu1);
form->OnDestroy = form_onDestroy; // register callback

button = new thButton(form, 0, form->Height - 30);
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
- documentation and pictures
- examples (calculator, uart)
- error handling
- callback should be changed (std::function or something)

## How to use
Project is using external components which are built into static libraries. Win32 Gui itself is also built into library.
Since this is Windows only, use free Visual Studio 2019 to open solution file Win32_gui_wrapper.sln, build (ctrl+shift+b) all and Run (F5).
Usage examples can be selected in solution explorer from within VS.

## External
Program uses Scintilla as thRichEdit component and SimpleGrid and thStringGrid.
