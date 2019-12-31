#pragma once
// http://msdn.microsoft.com/en-us/library/windows/desktop/aa363874%28v=vs.85%29.aspx
// http://msdn.microsoft.com/en-us/library/windows/desktop/aa364233%28v=vs.85%29.aspx
#include "thTypes.h"
#include "thConfig.h"

class thFile
{
public:
    enum class DesiredAccess
    {
        generic_read = 0,   // Read access
        generic_write,      // Write access
        generic_execute,    // Execute access
        generic_all         // All possible access rights
    };

    enum class CreationDisposition
    {
        create_always = 0, // Creates a new file, always
        create_new,        // Creates a new file, only if it does not already exist
        open_always,       // Opens a file, always
        open_existing,     // Opens a file or device, only if it exists
        truncate_existing  // Opens a file and truncates it so that its size is zero bytes, only if it exists
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

    uint32_t            Open(thString, thFile::DesiredAccess, thFile::CreationDisposition);
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
