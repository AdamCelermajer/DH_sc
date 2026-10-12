// Minimal stand-in for the vox file-system types used by the stream cursors.
// The real FileSystemInterface lives in the vox::FileSystem category; only the
// slots that the reconstructed stream code calls are declared here.
#pragma once

#include <cstddef>

namespace vox {

enum VoxFileAccessMode : int {
    kVoxFileRead = 6,  // mode value passed by StreamCFile::Init / Cursor::Init
};

// File handle returned by FileSystemInterface::OpenFile.
// Vtable slots used by the stream code: +0x8 Read, +0xc Seek, +0x10 Tell.
class FileInterface {
public:
    virtual ~FileInterface() = default;  // 0x8939e4 (D1), 0x893e60 (D0)

    void* GetFilePtr() const { return file_handle_; }  // 0x8939e8: word at +0x4
    virtual int Read(void* dst, std::size_t size, std::size_t count) = 0;  // +0x8
    virtual int Seek(int offset, int whence) = 0;                          // +0xc
    virtual int Tell() = 0;                                                // +0x10

protected:
    void* file_handle_ = nullptr;  // +0x04 (the native handle the implementation stores)
};

// Vtable slots used by the stream code: +0x8 OpenFile, +0xc CloseFile.
class FileSystemInterface {
public:
    virtual ~FileSystemInterface() = default;
    virtual FileInterface* OpenFile(const char* name, VoxFileAccessMode mode) = 0;  // +0x8
    virtual void CloseFile(FileInterface* file) = 0;                                // +0xc

    // Process-wide instance, as FileSystemInterface::GetInstance() in the original.
    static FileSystemInterface* GetInstance();
};

// Test hook: installs the instance returned by GetInstance.
void SetFileSystemInstanceForTests(FileSystemInterface* fs);

}  // namespace vox
