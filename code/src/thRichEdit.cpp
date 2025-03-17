#include "thRichEdit.h"
#include "SciLexer.h"
#include "Scintilla.h"

/* Defines */
#define CLASS_NAME TEXT("Scintilla")
#define WIN32_CLASS_NAME TEXT("Scintilla")
#define DEFAULT_TEXT TEXT("")

#define DEFAULT_WIDTH  75
#define DEFAULT_HEIGHT 75

/* Local Memory */
int thRichEdit::m_indexPool = 1;


/* Prototypes */
LRESULT CALLBACK ChildWindProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);

static const char g_cppKeyWords[] =

// Standard
"asm auto bool break case catch char class const "
"const_cast continue default delete do double "
"dynamic_cast else enum explicit extern false finally "
"float for friend goto if inline int long mutable "
"namespace new operator private protected public "
"register reinterpret_cast register return short signed "
"sizeof static static_cast struct switch template "
"this throw true try typedef typeid typename "
"union unsigned using virtual void volatile "
"wchar_t while "

// Extended
"__asm __asume __based __box __cdecl __declspec "
"__delegate delegate depreciated dllexport dllimport "
"event __event __except __fastcall __finally __forceinline "
"__int8 __int16 __int32 __int64 __int128 __interface "
"interface __leave naked noinline __noop noreturn "
"nothrow novtable nullptr safecast __stdcall "
"__try __except __finally __unaligned uuid __uuidof "
"__virtual_inheritance";

// Scintilla Colors structure
struct SScintillaColors
{
    int         iItem;
    COLORREF    rgb;
};

// A few basic colors
const COLORREF black = RGB(0, 0, 0);
const COLORREF white = RGB(255, 255, 255);
const COLORREF green = RGB(0, 255, 0);
const COLORREF red = RGB(255, 0, 0);
const COLORREF blue = RGB(0, 0, 255);
const COLORREF yellow = RGB(255, 255, 0);
const COLORREF magenta = RGB(255, 0, 255);
const COLORREF cyan = RGB(0, 255, 255);

/// Default color scheme
static SScintillaColors g_rgbSyntaxCpp[] =
{
    { SCE_C_COMMENT, green },
    { SCE_C_COMMENTLINE, green },
    { SCE_C_COMMENTDOC, green },
    { SCE_C_NUMBER, magenta },
    { SCE_C_STRING, yellow },
    { SCE_C_CHARACTER, yellow },
    { SCE_C_UUID, cyan },
    { SCE_C_OPERATOR, red },
    { SCE_C_PREPROCESSOR, cyan },
    { SCE_C_WORD, cyan },
    { -1, 0 }
};

void thRichEdit::registerClass()
{
    // Register Scintilla classes.
    // It must be done once in a lifetime of an application.
    static bool once = true;

    if ( true == once)
    {
        HINSTANCE   application_instance = GetModuleHandle( NULL);

        if ( 0 != Scintilla_RegisterClasses( application_instance))
        {
            once = false;
        }
        else
        {
            MSG_ERROR( TEXT( "Scintilla_RegisterClasses failed with error = 0x%X"), GetLastError());
        }
    }
}

void thRichEdit::SetAStyle(int style, COLORREF fore, COLORREF back = RGB(0, 0, 0), int size = -1, const char *face = 0)
{
    SendMessage(this->m_hWinHandle, SCI_STYLESETFORE, style, fore);
    SendMessage(this->m_hWinHandle, SCI_STYLESETBACK, style, back);
    if (size >= 1)
        SendMessage(this->m_hWinHandle, SCI_STYLESETSIZE, style, size);
    if (face)
        SendMessage(this->m_hWinHandle, SCI_STYLESETFONT, style, reinterpret_cast<LPARAM>(face));
}
//typedef int(*SCINTILLA_FUNC) (void*, int, int, int);
//typedef void * SCINTILLA_PTR;
//SCINTILLA_FUNC _pScintillaFunc;
//SCINTILLA_PTR  _pScintillaPtr;
//LRESULT execute(UINT Msg, WPARAM wParam = 0, LPARAM lParam = 0)  {
//    return _pScintillaFunc(_pScintillaPtr, static_cast<int>(Msg), static_cast<int>(wParam), static_cast<int>(lParam));
//};

thRichEdit::thRichEdit(thWindow * a_pParent, int a_posX, int a_posY)
    :
    thWindow(a_pParent, a_posX, a_posY)
{
    TH_ENTER_FUNCTION;
    BOOL    fResult = FALSE;
    this->m_name = CLASS_NAME;

    this->m_sWindowArgs.dwExStyle = 0;
    this->m_sWindowArgs.lpClassName =   WIN32_CLASS_NAME;
    this->m_sWindowArgs.lpWindowName =  DEFAULT_TEXT;
    this->m_sWindowArgs.dwStyle =       WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN;
    //    | ES_AUTOHSCROLL | WS_HSCROLL;
    //ES_READONLY WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL
    this->m_sWindowArgs.nWidth =        DEFAULT_WIDTH;
    this->m_sWindowArgs.nHeight =       DEFAULT_HEIGHT;
    this->m_sWindowArgs.hMenu =         reinterpret_cast<HMENU>(this->m_id);;
    this->m_sWindowArgs.lpParam =       this;

    this->registerClass();

    this->create();

    //_pScintillaFunc = (SCINTILLA_FUNC)::SendMessage(m_hWinHandle, SCI_GETDIRECTFUNCTION, 0, 0);
    //_pScintillaPtr = (SCINTILLA_PTR)::SendMessage(m_hWinHandle, SCI_GETDIRECTPOINTER, 0, 0);
    //
    //if (!_pScintillaFunc)
    //{
    //    MSG_ERROR(TEXT("ScintillaEditView::init : SCI_GETDIRECTFUNCTION message failed"));
    //}
    //
    //if (!_pScintillaPtr)
    //{
    //    MSG_ERROR(TEXT("ScintillaEditView::init : SCI_GETDIRECTPOINTER message failed"));
    //}


    // CPP lexer
    SendMessage(this->m_hWinHandle, SCI_SETLEXER, SCLEX_CPP, 0);

    // Set number of style bits to use
    SendMessage(this->m_hWinHandle, SCI_SETSTYLEBITS, 5, 0);

    // Set tab width
    SendMessage(this->m_hWinHandle, SCI_SETTABWIDTH, 4, 0);

    // Set SCI_SETMARGINWIDTHN
    //SendMessage(this->m_hWinHandle, SCI_SETMARGINWIDTHN, STYLE_LINENUMBER, 0);
        
    // Set SETMARGINTYPEN
    //SendMessage(this->m_hWinHandle, SCI_SETMARGINTYPEN, 0, 200);

    // Use CPP keywords
    SendMessage(this->m_hWinHandle, SCI_SETKEYWORDS, 0, (LPARAM)g_cppKeyWords);

    // Set up the global default style. These attributes are used wherever no explicit choices are made.
    SetAStyle(STYLE_DEFAULT, white, black, 10, "Courier New");

    // Set caret foreground color
    SendMessage(this->m_hWinHandle, SCI_SETCARETFORE, RGB(255, 255, 255), 0);

    // Set all styles
    SendMessage(this->m_hWinHandle, SCI_STYLECLEARALL, 0, 0);

    // Set selection color
    SendMessage(this->m_hWinHandle, SCI_SETSELBACK, TRUE, RGB(0, 0, 255));

    // Set background color
    //SendMessage(m_hWinHandle, SCI_STYLESETBACK, STYLE_DEFAULT, (LPARAM)RGB(0, 0, 0));

    //SendMessage(this->m_hWinHandle, SCI_SETTEXT, 0, (LPARAM)"asd111");
    // Set syntax colors
    for (long i = 0; g_rgbSyntaxCpp[i].iItem != -1; i++)
        SetAStyle(g_rgbSyntaxCpp[i].iItem, g_rgbSyntaxCpp[i].rgb);

    fResult = SetWindowSubclass(this->m_hWinHandle, ChildWindProc, 0, (DWORD_PTR)this);

    if (FALSE == fResult) {
        MSG_ERROR(TEXT("SetWindowSubclass failed with error = 0x%X"), GetLastError());
    }

    TH_LEAVE_FUNCTION;
}

thRichEdit::~thRichEdit()
{
    TH_ENTER_OBJECT_FUNCTION;
    TH_LEAVE_OBJECT_FUNCTION;
}

int thRichEdit::getDebugIndex()
{
    TH_ENTER_OBJECT_FUNCTION;
    int dReturn = this->m_indexPool;
    this->m_indexPool++;
    TH_LEAVE_OBJECT_FUNCTION;
    return dReturn;
}

LRESULT thRichEdit::onSetText(LPARAM a_lParam)
{
    TH_ENTER_OBJECT_FUNCTION;
    LRESULT tResult = 0;
    std::wstring text((wchar_t*)a_lParam);

    //SendMessage(this->m_hWinHandle, SCI_SETCODEPAGE, 0, SC_CP_UTF8);
    SendMessage(this->m_hWinHandle, SCI_SETTEXT, 0, (LPARAM)(WStringToString(text).c_str()));
    //SendMessage(this->m_hWinHandle, SCI_EMPTYUNDOBUFFER, 0, 0);
    
    tResult = 1;

    TH_LEAVE_OBJECT_FUNCTION;
    return tResult;
}

// return 0 will process this message further in Z-axis
// return 1 will stop processing this msg
LRESULT thRichEdit::onContextMenu(WPARAM a_wParam, LPARAM a_lParam)
{
    LRESULT tResult = 0; // use default scintila popup menu
    return tResult;
}

LRESULT thRichEdit::processCommandMessage(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    TH_ENTER_OBJECT_FUNCTION;
    LRESULT tResult = 0; // should return 1 if not used (no CB registered)

    TH_LEAVE_OBJECT_FUNCTION;
    return tResult;
}

LRESULT thRichEdit::processNotifyMessage(HWND a_hwnd, UINT a_uMsg, WPARAM a_wParam, LPARAM a_lParam)
{
    //TH_ENTER_OBJECT_FUNCTION;
    LRESULT tResult = 0;
    NMHDR * pData = 0;

    pData = reinterpret_cast<NMHDR*>(a_lParam);

    if (pData) {
    //    MSG_ERROR(TEXT("WM_NOTIFY: hwndFrom=0x%X, idFrom=%d, code=0x%X"), pData->hwndFrom, pData->idFrom, pData->code);
    }

    //TH_LEAVE_OBJECT_FUNCTION;
    return tResult;
}