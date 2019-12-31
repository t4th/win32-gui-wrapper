#include "thCommonDialog.h"
#include "thWindow.h"

// thOpenDialog

const thString OPEN_DIALOG_DEFAULT_FILTER = TEXT("All Files\0*.*\0");

// thCommonDialog
thCommonDialog::thCommonDialog() : m_hParent(NULL)
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

thCommonDialog::~thCommonDialog()
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

// Show Open Dialog on Window pointed by a_parentWindow.
// if a_parentWindow == 0, dialog has no parent.
// return non-zero value when successfull
bool_t thCommonDialog::Show(thWindow * const a_parentWindow)
{
    bool_t fResult = false;

    if (a_parentWindow) {
        this->m_hParent = a_parentWindow->GetHandle();
    }
    else {
        this->m_hParent = NULL;
    }

    fResult = this->show();

    return fResult;
}

thOpenDialog::thOpenDialog()
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

thOpenDialog::~thOpenDialog()
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

// Make string required by OPENFILENAME structure from thDialogFilter Items
thString thOpenDialog::getFilterString()
{
    TH_ENTER_FUNCTION;
    thString filter;
    TCHAR end_of_string = TEXT('\0');

    // if filter is not provided, default one is used - All\0*.*\0
    if (0 == this->Filter.GetCount()) {
        filter.append(OPEN_DIALOG_DEFAULT_FILTER);
        MSG_WARNING(TEXT("No filters provided -> using default filter \"All\\0*.*\\0\""));
    }
    else {
        for (int i = 0; i < this->Filter.GetCount(); i++) {
            filter.append(this->Filter[i].Name);
            filter.append(&end_of_string, 1);
            filter.append(this->Filter[i].Filter);
            filter.append(&end_of_string, 1);
        }
    }

    TH_LEAVE_FUNCTION;
    return filter;
}

// return non-zero value when successfull
bool_t thOpenDialog::show()
{
    TH_ENTER_FUNCTION;
    bool_t fResult = FALSE;
    thString filter = getFilterString();

    OPENFILENAME     ofn = { 0 };
    TCHAR            szFileName[MAX_PATH] = TEXT("");

    ofn.lStructSize =       sizeof (ofn);
    ofn.hwndOwner =         this->m_hParent;
    ofn.lpstrFile =         szFileName;
    ofn.lpstrFile[0] =      L'\0';
    ofn.nMaxFile =          sizeof(szFileName);
    ofn.lpstrFilter =       filter.c_str(); //Filter;// TEXT("All\0*.*\0Text\0*.TXT\0");
    ofn.nFilterIndex =      1;
    ofn.lpstrFileTitle =    NULL;
    ofn.nMaxFileTitle =     0;
    ofn.lpstrInitialDir =   NULL;
    ofn.Flags =             OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn)) {
        this->FileName = szFileName;
        fResult = TRUE;
    }
    else {
        this->FileName.clear();
        MSG_WARNING(TEXT("GetOpenFileName error or cancel. GetLastError = 0x%X"), CommDlgExtendedError());
    }

    TH_LEAVE_FUNCTION;
    return fResult;
}
