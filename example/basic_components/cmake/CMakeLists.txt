

cmake_minimum_required(VERSION 4.0)
project(test1 VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")


find_package(simplegrid GLOBAL CONFIG REQUIRED NO_DEFAULT_PATH PATHS ../../../external/simple_grid)
find_package(scintilla GLOBAL CONFIG REQUIRED NO_DEFAULT_PATH PATHS ../../../external/)
find_package(win32-gui-wrapper CONFIG REQUIRED NO_DEFAULT_PATH PATHS ../../../cmake)


add_executable(basic_components ../src/main.cpp)
target_compile_definitions(basic_components PUBLIC UNICODE _UNICODE)
target_link_libraries(basic_components win32-gui-wrapper scintilla)

target_link_options(basic_components PUBLIC  "-Wl,-Bstatic")
#target_link_options(basic_components PUBLIC  "-Wl,--strip-all" )
