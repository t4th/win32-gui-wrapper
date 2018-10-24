## Experimental win32 GUI wrapper in C++
This is fun little project I did when learning win32 GUI API.

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
form->OnDestroy = form_onDestroy; // callback

button = new thButton(form, 0, form->Height - 30);
button->Width = form->Width;
button->Height = 30;
button->Anchors.Top = FALSE;
button->Anchors.Left = TRUE;
button->Anchors.Right = TRUE;

form->Show();
```
## Summary
I was pretty satisfied with the result, but it was just prove of concept learning material, and it was never supposed to reach 1.0.
##### Good
- It looks nice and is intuitive to use
- As native GUI it is very responsive
##### Bad
- Lots of bad programming habits issues: naming and capsulation
- C++ API is mixed with native Windows types
- Only basic features
