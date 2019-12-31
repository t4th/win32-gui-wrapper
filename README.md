## Experimental win32 GUI wrapper in C++
This is fun little project I did when learning win32 GUI API and C++.

## Goal
My goal was to wrap native Windows C code with C++ classes like in Borland C++ Builder (now Embarcadero).
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
button->->OnClick = button_onClick;  // register callback

form->Show();
```
## Summary
I was pretty satisfied with the result, but it was just prove of concept learning material, and it was never supposed to reach 1.0. Still it is now good source of win32 API examples. I remember being the most proud of re-sizing and MDI :).

##### Good
- It looks nice and is intuitive to use
- As native GUI it is very responsive
##### Bad
- Lots of bad programming habits issues: naming and capsulation
- C++ API is mixed with native Windows types
- Only basic features
- No good error handling in operator overloads, since I didn't want to use try-catch like Borland C++
- C++ Builder, QT or other popular frameworks use code generation for static object creation. In my code I had to workaround such problems with dynamic objects and elements, like event callbacks

## TODO
- error handling
- fix types
- callback should be changed (std::function or something)
