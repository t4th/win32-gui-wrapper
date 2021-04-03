//http://msdn.microsoft.com/en-us/library/windows/desktop/aa364963%28v=vs.85%29.aspx
#include "thFile.h"


thFile::thFile()
{
    TH_ENTER_FUNCTION;
    TH_LEAVE_FUNCTION;
}

thFile::thFile(
    thString                    a_filePath,
    thFile::DesiredAccess       a_DesiredAccess,
    thFile::CreationDisposition a_CreationDisposition
)
{
    uint32_t u32Result = this->Open( a_filePath, a_DesiredAccess, a_CreationDisposition);

    if ( u32Result)
    {
        // Result is omitted since its c-tor.
    }
}

thFile::~thFile()
{
    TH_ENTER_FUNCTION;

    if ( this->IsOpen())
    {
        this->Close();
    }

    TH_LEAVE_FUNCTION;
}

// return 0 if no error
bool thFile::Open( thString a_filePath, thFile::DesiredAccess a_DesiredAccess, thFile::CreationDisposition a_CreationDisposition)
{
    constexpr DWORD DesiredAccess[] = {GENERIC_READ, GENERIC_WRITE, GENERIC_EXECUTE, GENERIC_ALL};
    constexpr DWORD CreationDisposition[] = {CREATE_ALWAYS, CREATE_NEW, OPEN_ALWAYS, OPEN_EXISTING, TRUNCATE_EXISTING};

    auto desired_access_index = static_cast< uint8_t>( a_DesiredAccess);
    auto desired_disposition_index = static_cast< uint8_t>( a_CreationDisposition);

    TH_ENTER_FUNCTION;
    bool fileOpened = false;

    if ( NULL == m_hHandle)
    {
        m_hHandle = CreateFile(
            a_filePath.c_str(),
            DesiredAccess[ desired_access_index],
            0,
            NULL,
            CreationDisposition[ desired_disposition_index],
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );

        if ( INVALID_HANDLE_VALUE == m_hHandle)
        {
            uint32_t u32Result = GetLastError();
            MSG_ERROR( TEXT( "CreateFile failed with error = 0x%X"), u32Result);
        }
        else
        {
            fileOpened = true;
            m_filePath = a_filePath;

            wchar_t drive[ _MAX_DRIVE]{};
            wchar_t dir[ _MAX_DIR]{};
            wchar_t fname[ _MAX_FNAME]{};
            wchar_t ext[ _MAX_EXT]{};
            errno_t err{};

            err = _tsplitpath_s(
                a_filePath.c_str(),
                drive,
                _MAX_DRIVE,
                dir,
                _MAX_DIR,
                fname,
                _MAX_FNAME, ext, _MAX_EXT
            );

            if ( 0 != err)
            {
                MSG_ERROR( TEXT( "Error splitting the path. Error code %d."), err);
            }
            else
            {
                m_fileDirectory = std::move( thString( dir));
                m_fileName = std::move( thString( fname));
                m_fileExtension = std::move( thString( ext));
            }

            MSG_SUCCESS( TEXT( "File opened successfully"));
        }
    }
    else
    {
        this->Close();
        fileOpened = this->Open( a_filePath, a_DesiredAccess, a_CreationDisposition);
    }
    
    TH_LEAVE_FUNCTION;

    return fileOpened;
}

bool thFile::Write( uint8_t * const a_inputBuffer, size_t a_BytesToWrite)
{
    TH_ENTER_FUNCTION;
    DWORD dwBytesWritten = 0;
    BOOL fResult = WriteFile( m_hHandle, a_inputBuffer, a_BytesToWrite, &dwBytesWritten, NULL);

    if ( FALSE == fResult)
    {
        uint32_t u32Result = GetLastError();
        MSG_ERROR( TEXT( "WriteFile failed with error = 0x%X"), u32Result);

        TH_LEAVE_FUNCTION;
        return false;
    }
    else
    {
        TH_LEAVE_FUNCTION;
        return true;
    }
}

bool thFile::Read( uint8_t * const a_ouputBuffer, size_t a_bytesToRead, size_t & a_dwBytesRead)
{
    TH_ENTER_FUNCTION;

    BOOL fResult = ReadFile( m_hHandle, a_ouputBuffer, a_bytesToRead, (LPDWORD)&a_dwBytesRead, NULL);

    if ( FALSE == fResult)
    {
        uint32_t u32Result = GetLastError();
        MSG_ERROR( TEXT( "ReadFile failed with error = 0x%X"), u32Result);

        TH_LEAVE_FUNCTION;
        return false;
    }
    else
    {
        TH_LEAVE_FUNCTION;
        return true;
    }
}

bool_t thFile::IsOpen()
{
    bool_t fResult = FALSE;

    if ( m_hHandle)
    {
        fResult = TRUE;
    }

    return fResult;
}

void thFile::Close()
{
    TH_ENTER_FUNCTION;
    BOOL fResult = CloseHandle( m_hHandle);

    if ( 0 == fResult)
    {
        uint32_t u32Result = GetLastError();
        MSG_ERROR( TEXT( "CloseHandle failed with error = 0x%X"), u32Result);
    }
    else
    {
        m_hHandle = NULL;
    }

    TH_LEAVE_FUNCTION;
}

thString thFile::GetFileName() const
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
    return this->m_fileName + this->m_fileExtension;
}

thString thFile::GetFilePath() const
{
    TH_ENTER_FUNCTION;
    thString    oFilePath;
    
    if ( INVALID_HANDLE_VALUE != m_hHandle)
    {
        TCHAR sBuffer[ MAX_PATH];

        DWORD dwResult = GetFinalPathNameByHandle(
            m_hHandle,
            sBuffer,
            MAX_PATH,
            FILE_NAME_OPENED | VOLUME_NAME_DOS
        );
    
        if ( dwResult < MAX_PATH)
        {
            MSG_SUCCESS( TEXT( "The final path is: %s"), sBuffer);

            oFilePath = std::move( thString( sBuffer));
        }
        else {
            MSG_ERROR( TEXT( "The required buffer size is %d"), dwResult);
        }
    }

    TH_LEAVE_FUNCTION;
    return oFilePath;
}

thString thFile::GetFileDirectory() const
{
    TH_ENTER_FUNCTION;

    TH_LEAVE_FUNCTION;
    return this->m_fileDirectory;
}

uint64_t thFile::GetFileSize() const
{
    TH_ENTER_FUNCTION;
    LARGE_INTEGER tSize;
    uint64_t u64FileSize = 0;

    BOOL fResult = GetFileSizeEx( m_hHandle, &tSize);

    if ( fResult)
    {
        u64FileSize = static_cast<uint64_t> ( tSize.QuadPart);
        MSG_SUCCESS( TEXT( "File size: %ull"), u64FileSize);
    }
    else
    {
        MSG_ERROR( TEXT( "GetFileSizeEx failed with error = 0x%X"), GetLastError());
    }

    TH_LEAVE_FUNCTION;
    return u64FileSize;
}
