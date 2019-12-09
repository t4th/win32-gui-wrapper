//http://msdn.microsoft.com/en-us/library/windows/desktop/aa364963%28v=vs.85%29.aspx
#include "thFile.h"


thFile::thFile() : m_hHandle(NULL)
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}


thFile::~thFile()
{
    TH_ENTER_FUNCTION;

    if (this->IsOpen()) {
        this->Close();
    }

    TH_LEAVE_FUNCTION;
}

// return 0 if no error
uint32_t thFile::Open(thString a_filePath, thFile::eDesiredAccess_t a_DesiredAccess, thFile::eCreationDisposition a_CreationDisposition)
{
    TH_ENTER_FUNCTION;
    uint32_t u32Result = 0;

    if (NULL == m_hHandle) {
        m_hHandle = CreateFile(a_filePath.c_str(), a_DesiredAccess, 0, NULL, a_CreationDisposition, FILE_ATTRIBUTE_NORMAL, NULL);

        if (INVALID_HANDLE_VALUE == m_hHandle) {
            u32Result = GetLastError();
            MSG_ERROR(TEXT("CloseHandle failed with error = 0x%X"), u32Result);
        }
        else {
            m_filePath = a_filePath;

            wchar_t drive[_MAX_DRIVE]{};
            wchar_t dir[_MAX_DIR]{};
            wchar_t fname[_MAX_FNAME]{};
            wchar_t ext[_MAX_EXT]{};
            errno_t err{};

            // https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/splitpath-s-wsplitpath-s?redirectedfrom=MSDN&view=vs-2019
            err = _tsplitpath_s( a_filePath.c_str(), drive, _MAX_DRIVE, dir, _MAX_DIR, fname,
                _MAX_FNAME, ext, _MAX_EXT );

            if (err != 0)
            {
                MSG_ERROR(TEXT("Error splitting the path. Error code %d."), err);
            }
            else
            {
                m_fileDirectory = thString(dir);
                m_fileName = thString(fname);
                m_fileExtension = thString(ext);
            }

            MSG_SUCCESS(TEXT("File opened successfully"));
        }
    }
    else {
        this->Close();
        u32Result = this->Open(a_filePath, a_DesiredAccess, a_CreationDisposition);
    }

    TH_LEAVE_FUNCTION;
    return u32Result;
}

uint32_t thFile::Write(uint8_t * const a_inputBuffer, uint32_t a_BytesToWrite)
{
    TH_ENTER_FUNCTION;
    uint32_t u32Result = 0;
    BOOL     fResult = FALSE;

    DWORD dwBytesWritten = 0;
    fResult = WriteFile(m_hHandle, a_inputBuffer, a_BytesToWrite, &dwBytesWritten, NULL);

    if (FALSE == fResult) {
        u32Result = GetLastError();
        MSG_ERROR(TEXT("WriteFile failed with error = 0x%X"), u32Result);
    }

    TH_LEAVE_FUNCTION;
    return u32Result;
}

uint32_t thFile::Read(uint8_t * const a_ouputBuffer, uint32_t a_bytesToRead, uint32_t & a_dwBytesRead)
{
    TH_ENTER_FUNCTION;
    uint32_t u32Result = 0;
    BOOL     fResult = FALSE;

    fResult = ReadFile(m_hHandle, a_ouputBuffer, a_bytesToRead, (LPDWORD)&a_dwBytesRead, NULL);

    if (FALSE == fResult) {
        u32Result = GetLastError();
        MSG_ERROR(TEXT("ReadFile failed with error = 0x%X"), u32Result);
    }

    TH_LEAVE_FUNCTION;
    return u32Result;
}

bool_t thFile::IsOpen(void)
{
    bool_t fResult = FALSE;

    if (m_hHandle) {
        fResult = TRUE;
    }

    return fResult;
}

void thFile::Close(void)
{
    TH_ENTER_FUNCTION;
    BOOL fResult = 0;

    fResult = CloseHandle(m_hHandle);

    if (0 == fResult) {
        MSG_ERROR(TEXT("CloseHandle failed with error = 0x%X"), GetLastError());
    }
    else
    {
        m_hHandle = NULL;
    }

    TH_LEAVE_FUNCTION;
}

thString thFile::GetFileName(void) const
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
    return this->m_fileName + this->m_fileExtension;
}

thString thFile::GetFilePath(void) const
{
    TH_ENTER_FUNCTION;
    thString    oFilePath;
    BOOL        fResult = FALSE;

#if 0
    const DWORD numElts = MAX_PATH;

    size_t size = sizeof(FILE_NAME_INFO)+sizeof(WCHAR)*numElts;

        FILE_NAME_INFO *info = reinterpret_cast<FILE_NAME_INFO *>(malloc(size));
        info->FileNameLength = numElts;

    if (INVALID_HANDLE_VALUE != m_hHandle) {
        fResult = GetFileInformationByHandleEx(m_hHandle, FileNameInfo, info, size);

        if (fResult) {
            MSG_SUCCESS(TEXT("File path is: %s"), info->FileName);
                //tFullDirInfo.FileName);
        }
        else {
            MSG_ERROR(TEXT("GetFileInformationByHandleEx failed with error = 0x%X"), GetLastError());
        }
    }

    free(info);
#endif

#if 1
    DWORD       dwResult = 0;
    TCHAR       sBuffer[MAX_PATH] = { 0 };
    
    if (INVALID_HANDLE_VALUE != m_hHandle) {
        dwResult = GetFinalPathNameByHandle(m_hHandle, sBuffer, MAX_PATH, FILE_NAME_OPENED | VOLUME_NAME_DOS);
    
        if (dwResult < MAX_PATH)
        {
            MSG_SUCCESS(TEXT("The final path is: %s"), sBuffer);

            oFilePath = thString(sBuffer);
        }
        else {
            MSG_ERROR(TEXT("The required buffer size is %d"), dwResult);
        }
    }
#endif

    TH_LEAVE_FUNCTION;
    return oFilePath;
}

thString thFile::GetFileDirectory(void) const
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
    return this->m_fileDirectory;
}

uint64_t thFile::GetFileSize(void) const
{
    TH_ENTER_FUNCTION;
    LARGE_INTEGER tSize = { 0 };
    uint64_t u64FileSize = 0;
    BOOL fResult = FALSE;

    fResult = GetFileSizeEx(m_hHandle, &tSize);

    if (fResult) {
        u64FileSize = (uint64_t)tSize.QuadPart;
        MSG_SUCCESS(TEXT("File size: %ull"), u64FileSize);
    }
    else {
        MSG_ERROR(TEXT("GetFileSizeEx failed with error = 0x%X"), GetLastError());
    }

    TH_LEAVE_FUNCTION;
    return u64FileSize;
}
