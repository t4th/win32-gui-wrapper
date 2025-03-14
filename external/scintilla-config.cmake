
# TODO: No Sources ? Define as Interface Library only for IncludePath.
add_library(${CMAKE_FIND_PACKAGE_NAME} EXCLUDE_FROM_ALL)

#${CMAKE_CURRENT_LIST_DIR}/scintilla/src/
SET (BASESOURCES
	${CMAKE_CURRENT_LIST_DIR}/scintilla/src/AutoComplete.cxx
	${CMAKE_CURRENT_LIST_DIR}/scintilla/src/CallTip.cxx
	${CMAKE_CURRENT_LIST_DIR}/scintilla/src/CaseConvert.cxx
	${CMAKE_CURRENT_LIST_DIR}/scintilla/src/CaseFolder.cxx
	${CMAKE_CURRENT_LIST_DIR}/scintilla/src/CellBuffer.cxx
	${CMAKE_CURRENT_LIST_DIR}/scintilla/lexlib/CharacterSet.cxx
	${CMAKE_CURRENT_LIST_DIR}/scintilla/src/CharClassify.cxx
	${CMAKE_CURRENT_LIST_DIR}/scintilla/src/ContractionState.cxx
	${CMAKE_CURRENT_LIST_DIR}/scintilla/src/Decoration.cxx
	${CMAKE_CURRENT_LIST_DIR}/scintilla/src/Document.cxx
	${CMAKE_CURRENT_LIST_DIR}/scintilla/src/Editor.cxx
	${CMAKE_CURRENT_LIST_DIR}/scintilla/src/KeyMap.cxx
	${CMAKE_CURRENT_LIST_DIR}/scintilla/src/Indicator.cxx
	${CMAKE_CURRENT_LIST_DIR}/scintilla/src/LineMarker.cxx
	${CMAKE_CURRENT_LIST_DIR}/scintilla/src/PerLine.cxx
	${CMAKE_CURRENT_LIST_DIR}/scintilla/win32/PlatWin.cxx
	${CMAKE_CURRENT_LIST_DIR}/scintilla/src/PositionCache.cxx
	${CMAKE_CURRENT_LIST_DIR}/scintilla/lexlib/PropSetSimple.cxx
	${CMAKE_CURRENT_LIST_DIR}/scintilla/src/RESearch.cxx
	${CMAKE_CURRENT_LIST_DIR}/scintilla/src/RunStyles.cxx
	${CMAKE_CURRENT_LIST_DIR}/scintilla/src/Selection.cxx
	${CMAKE_CURRENT_LIST_DIR}/scintilla/src/Style.cxx
	${CMAKE_CURRENT_LIST_DIR}/scintilla/src/UniConversion.cxx
	${CMAKE_CURRENT_LIST_DIR}/scintilla/src/ViewStyle.cxx
	${CMAKE_CURRENT_LIST_DIR}/scintilla/src/XPM.cxx
	${CMAKE_CURRENT_LIST_DIR}/scintilla/src/ScintillaBase.cxx
	#${CMAKE_CURRENT_LIST_DIR}/scintilla/lexlib/CharacterCategory.cxx
)

target_sources(${CMAKE_FIND_PACKAGE_NAME} PRIVATE
${BASESOURCES}
${CMAKE_CURRENT_LIST_DIR}/scintilla/win32/ScintillaWin.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/win32/ScintRes.rc
)

target_include_directories(${CMAKE_FIND_PACKAGE_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/scintilla/include)
target_include_directories(${CMAKE_FIND_PACKAGE_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/scintilla/lexlib)
target_include_directories(${CMAKE_FIND_PACKAGE_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/scintilla/src)
target_compile_definitions(${CMAKE_FIND_PACKAGE_NAME} PRIVATE WIN32 _WIN32 UNICODE _UNICODE)
target_link_libraries(${CMAKE_FIND_PACKAGE_NAME} PRIVATE Imm32.lib KERNEL32.lib USER32.lib GDI32.lib IMM32.lib OLE32.LIB)
