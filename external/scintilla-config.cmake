
# TODO: No Sources ? Define as Interface Library only for IncludePath.
add_library(${CMAKE_FIND_PACKAGE_NAME} EXCLUDE_FROM_ALL)

#${CMAKE_CURRENT_LIST_DIR}/scintilla/src/
SET (BASESOURCES
    ${CMAKE_CURRENT_LIST_DIR}/scintilla/src/AutoComplete.cxx
    ${CMAKE_CURRENT_LIST_DIR}/scintilla/src/CallTip.cxx
    ${CMAKE_CURRENT_LIST_DIR}/scintilla/src/CaseConvert.cxx
    ${CMAKE_CURRENT_LIST_DIR}/scintilla/src/CaseFolder.cxx
    ${CMAKE_CURRENT_LIST_DIR}/scintilla/src/CellBuffer.cxx
    ${CMAKE_CURRENT_LIST_DIR}/scintilla/src/CharClassify.cxx
    ${CMAKE_CURRENT_LIST_DIR}/scintilla/src/ContractionState.cxx
    ${CMAKE_CURRENT_LIST_DIR}/scintilla/src/Decoration.cxx
    ${CMAKE_CURRENT_LIST_DIR}/scintilla/src/Document.cxx
    ${CMAKE_CURRENT_LIST_DIR}/scintilla/src/Editor.cxx
    ${CMAKE_CURRENT_LIST_DIR}/scintilla/src/KeyMap.cxx
    ${CMAKE_CURRENT_LIST_DIR}/scintilla/src/Indicator.cxx
    ${CMAKE_CURRENT_LIST_DIR}/scintilla/src/LineMarker.cxx
    ${CMAKE_CURRENT_LIST_DIR}/scintilla/src/PerLine.cxx
    ${CMAKE_CURRENT_LIST_DIR}/scintilla/src/PositionCache.cxx
    ${CMAKE_CURRENT_LIST_DIR}/scintilla/src/RESearch.cxx
    ${CMAKE_CURRENT_LIST_DIR}/scintilla/src/RunStyles.cxx
    ${CMAKE_CURRENT_LIST_DIR}/scintilla/src/Selection.cxx
    ${CMAKE_CURRENT_LIST_DIR}/scintilla/src/Style.cxx
    ${CMAKE_CURRENT_LIST_DIR}/scintilla/src/UniConversion.cxx
    ${CMAKE_CURRENT_LIST_DIR}/scintilla/src/ViewStyle.cxx
    ${CMAKE_CURRENT_LIST_DIR}/scintilla/src/XPM.cxx
    ${CMAKE_CURRENT_LIST_DIR}/scintilla/src/ScintillaBase.cxx
    ${CMAKE_CURRENT_LIST_DIR}/scintilla/lexlib/PropSetSimple.cxx
)

target_sources(${CMAKE_FIND_PACKAGE_NAME} PRIVATE
${BASESOURCES}
${CMAKE_CURRENT_LIST_DIR}/scintilla/win32/PlatWin.cxx # TODO: Select QT,GTK,Cocoa,Win
${CMAKE_CURRENT_LIST_DIR}/scintilla/win32/ScintillaWin.cxx
#${CMAKE_CURRENT_LIST_DIR}/scintilla/win32/ScintRes.rc  #version Infos if build as DLL
)

# Add Parsers TODO: make optional
target_compile_definitions(${CMAKE_FIND_PACKAGE_NAME} PUBLIC SCI_LEXER)
target_sources(${CMAKE_FIND_PACKAGE_NAME} PRIVATE
${CMAKE_CURRENT_LIST_DIR}/scintilla/src/Catalogue.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/src/ExternalLexer.cxx

${CMAKE_CURRENT_LIST_DIR}/scintilla/lexlib/Accessor.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexlib/CharacterSet.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexlib/CharacterCategory.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexlib/LexerBase.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexlib/LexerModule.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexlib/LexerSimple.cxx
#${CMAKE_CURRENT_LIST_DIR}/scintilla/lexlib/LexerNoExceptions.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexlib/StyleContext.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexlib/WordList.cxx

${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexNimrod.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexNsis.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexOpal.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexOScript.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexOthers.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexPascal.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexPB.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexPerl.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexPLM.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexPO.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexPOV.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexPowerPro.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexPowerShell.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexProgress.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexPS.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexPython.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexR.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexRebol.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexRuby.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexRust.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexScriptol.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexSmalltalk.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexSML.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexSorcus.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexSpecman.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexSpice.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexSQL.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexSTTXT.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexTACL.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexTADS3.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexTAL.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexTCL.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexTCMD.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexTeX.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexTxt2tags.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexVB.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexVerilog.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexVHDL.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexVisualProlog.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexYAML.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexA68k.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexAbaqus.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexAda.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexAPDL.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexAsm.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexAsn1.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexASY.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexAU3.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexAVE.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexAVS.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexBaan.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexBash.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexBasic.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexBullant.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexCaml.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexCLW.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexCmake.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexCOBOL.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexCoffeeScript.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexConf.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexCPP.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexCrontab.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexCsound.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexCSS.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexD.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexDMAP.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexECL.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexEiffel.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexErlang.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexEScript.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexFlagship.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexForth.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexFortran.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexGAP.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexGui4Cli.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexHaskell.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexHTML.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexInno.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexKix.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexKVIrc.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexLaTeX.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexLisp.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexLout.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexLua.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexMagik.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexMarkdown.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexMatlab.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexMetapost.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexMMIXAL.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexModula.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexMPT.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexMSSQL.cxx
${CMAKE_CURRENT_LIST_DIR}/scintilla/lexers/LexMySQL.cxx
)


target_include_directories(${CMAKE_FIND_PACKAGE_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/scintilla/include)
target_include_directories(${CMAKE_FIND_PACKAGE_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/scintilla/lexlib)
target_include_directories(${CMAKE_FIND_PACKAGE_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/scintilla/src)
target_link_libraries(${CMAKE_FIND_PACKAGE_NAME} PRIVATE Imm32.lib )
