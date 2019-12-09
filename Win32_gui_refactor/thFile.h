#pragma once
// http://msdn.microsoft.com/en-us/library/windows/desktop/aa363874%28v=vs.85%29.aspx
// http://msdn.microsoft.com/en-us/library/windows/desktop/aa364233%28v=vs.85%29.aspx
#include "thTypes.h"
#include "thConfig.h"

class thFile
{
public:
    enum eDesiredAccess_t
    {
        generic_read = GENERIC_READ,    // Read access
        generic_write = GENERIC_WRITE,   // Write access
        generic_execute = GENERIC_EXECUTE, // Execute access
        generic_all = GENERIC_ALL      // All possible access rights
    };

    enum eCreationDisposition
    {
        create_always = CREATE_ALWAYS,    // Creates a new file, always
        create_new = CREATE_NEW,       // Creates a new file, only if it does not already exist
        open_always = OPEN_ALWAYS,      // Opens a file, always
        open_existing = OPEN_EXISTING,    // Opens a file or device, only if it exists
        truncate_existing = TRUNCATE_EXISTING // Opens a file and truncates it so that its size is zero bytes, only if it exists
    };
protected:
    HANDLE              m_hHandle;
    thString            m_filePath;
    thString            m_fileDirectory;
    thString            m_fileName;
    thString            m_fileExtension;
private:
public:
                        thFile();
    virtual             ~thFile();

    uint32_t            Open(thString, thFile::eDesiredAccess_t, thFile::eCreationDisposition);
    uint32_t            Write(uint8_t * const, uint32_t);
    uint32_t            Read(uint8_t * const, uint32_t, uint32_t &);

    bool_t              IsOpen(void);
    void                Close(void);
    // deletefile

    thString            GetFileName(void) const;
    thString            GetFilePath(void) const;
    thString            GetFileDirectory(void) const;
    uint64_t            GetFileSize(void) const; // return file size in bytes
};

