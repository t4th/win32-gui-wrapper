
# file(GLOB GUI_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_LIST_DIR}/../code/src/*.cpp)
set (GUI_SOURCES 
${CMAKE_CURRENT_LIST_DIR}/../code/src/thApplication.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thButton.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thCheckBox.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thComboBox.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thComboBoxItems.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thCommonDialog.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thEditBox.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thFile.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thFont.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thForm.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thGroupBox.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thHeight.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thImageList.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thLabel.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thListBox.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thListBoxItems.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thListView.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thListViewColumn.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thListViewItem.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thLogger.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thMDIChild.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thMDIClient.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thMemo.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thMenu.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thMenuCommon.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thObject.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thPopupMenu.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thPosX.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thPosY.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thRadioButton.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thString.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thText.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thToolbar.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thWidth.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thWin32App.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thWin32Logger.cpp
${CMAKE_CURRENT_LIST_DIR}/../code/src/thWindow.cpp
)

add_library(${CMAKE_FIND_PACKAGE_NAME} EXCLUDE_FROM_ALL  ${GUI_SOURCES})
target_include_directories(${CMAKE_FIND_PACKAGE_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../code/inc)
target_compile_definitions(${CMAKE_FIND_PACKAGE_NAME} PRIVATE WIN32 _WIN32 UNICODE _UNICODE)
target_link_libraries(${CMAKE_FIND_PACKAGE_NAME} PRIVATE Comctl32.lib)


# Adding thRichEdit only if scintilla is available
if (${scintilla_FOUND})
target_sources(${CMAKE_FIND_PACKAGE_NAME} PRIVATE
${CMAKE_CURRENT_LIST_DIR}/../code/src/thRichEdit.cpp
)
target_link_libraries(${CMAKE_FIND_PACKAGE_NAME} PRIVATE scintilla)
endif()

# Adding thRichEdit only if simpleGrid is available
if (${simplegrid_FOUND})
target_sources(${CMAKE_FIND_PACKAGE_NAME} PRIVATE
${CMAKE_CURRENT_LIST_DIR}/../code/src/thStringGrid.cpp
)
target_link_libraries(${CMAKE_FIND_PACKAGE_NAME} PRIVATE simplegrid)
endif()
