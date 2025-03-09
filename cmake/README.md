Usage of win32-gui wrapper in cmake projects
--------------------------------------------


Sample adding this to your Application:
(adopt win32-gui-wrapper to the path where you place this library in your tree.)

simplegrid is optional. If not FOUND global, win32-gui-wrapper thStringGrid is not usable.
scintilla is optional. If not FOUND global, win32-gui-wrapper thRichtextEdit is not usable.

```
find_package(simplegrid GLOBAL CONFIG REQUIRED NO_DEFAULT_PATH PATHS win32-gui-wrapper/external/simple_grid)
find_package(scintilla GLOBAL CONFIG REQUIRED NO_DEFAULT_PATH PATHS win32-gui-wrapper/external/)
find_package(win32-gui-wrapper CONFIG REQUIRED NO_DEFAULT_PATH PATHS win32-gui-wrapper/cmake)

```
