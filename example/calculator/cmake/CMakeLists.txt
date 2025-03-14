

cmake_minimum_required(VERSION 4.0)
project(test1 VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")


find_package(win32-gui-wrapper CONFIG REQUIRED NO_DEFAULT_PATH PATHS ../../../cmake)


add_executable(calculator ../src/main.cpp)
target_compile_definitions(calculator PUBLIC UNICODE _UNICODE)
target_link_libraries(calculator win32-gui-wrapper )

target_link_options(calculator PUBLIC  "-Wl,-Bstatic")
#target_link_options(basic_components PUBLIC  "-Wl,--strip-all" )
