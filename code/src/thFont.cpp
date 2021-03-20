#include "thFont.h"
#include "thWindow.h"

thFont::thFont()
{
}

thFont::~thFont()
{
    if (m_sFont) {
        BOOL fResult = FALSE;

        fResult = DeleteObject(m_sFont);

        if (TRUE != fResult) {
            MSG_ERROR(TEXT("DeleteObject failed with error = 0x%X"), GetLastError());
        }
    }
}

void thFont::set(void)
{
    // delete current font (if exist)
    if (m_sFont) {
        BOOL fResult = FALSE;

        fResult = DeleteObject(m_sFont);

        if (TRUE != fResult) {
            MSG_ERROR(TEXT("DeleteObject failed with error = 0x%X"), GetLastError());
        }
        else {
            m_sFont = 0;
        }
    }

    if (0 == m_sFont) {
        // Create Default Font
        m_sFont = CreateFontIndirect(&this->m_sLogicalFont);

        if (m_sFont) {
            // set new font
            SendMessage(m_pParent->GetHandle(), WM_SETFONT, (WPARAM)m_sFont, (LPARAM)TRUE);
        }
        else {
            MSG_ERROR(TEXT("CreateFontIndirect failed with error = 0x%X"), GetLastError());            
        }
    }
    else {
        MSG_ERROR(TEXT("Failed to set new font!"));
    }
}

void thFont::SetName(thString a_fontName)
{
    SecureZeroMemory(this->m_sLogicalFont.lfFaceName, LF_FACESIZE);
    _tcscpy_s(this->m_sLogicalFont.lfFaceName, LF_FACESIZE, a_fontName.c_str());

    this->set();
}

// set size in point
void thFont::SetSize(uint32_t a_newsize)
{
    HDC     hDevContext;

    hDevContext = GetDC(NULL);

    // Calcuate font size to point format
    this->m_sLogicalFont.lfHeight = -MulDiv(a_newsize, GetDeviceCaps(hDevContext, LOGPIXELSY), 72);
    // If you want constant font size in pixels for all DPI then you should use this:
    // nHeight = MulDiv(PointSize, 96, GetDeviceCaps(hDC, LOGPIXELSY));
    // 96 is current dpi
    ReleaseDC(NULL, hDevContext);

    this->set();
}

void thFont::SetParent(const thWindow * const a_pParent)
{
    if (a_pParent) {
        if (nullptr == m_pParent) {
            m_pParent = a_pParent;
        }
        else {
            MSG_ERROR(TEXT("Parent already set!"));
        }
    }
    else {
        MSG_ERROR(TEXT("Empty pointer as argument!"));
    }
}