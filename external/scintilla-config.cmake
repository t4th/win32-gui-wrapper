
# TODO: No Sources ? Define as Interface Library only for IncludePath.
add_library(${CMAKE_FIND_PACKAGE_NAME} EXCLUDE_FROM_ALL INTERFACE)
target_include_directories(${CMAKE_FIND_PACKAGE_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/scintilla/include)
#target_compile_definitions(${CMAKE_FIND_PACKAGE_NAME} PRIVATE WIN32 _WIN32 UNICODE _UNICODE)
#target_link_libraries(${CMAKE_FIND_PACKAGE_NAME} PRIVATE Comctl32.lib)
