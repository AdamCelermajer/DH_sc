// FileSystemWin32 and its _FileHandle, the original Windows file system. Only the
// file-handle accessors and path getters are reconstructed so far; member offsets
// follow the original (+0x2c root, +0x234 resources, +0x43c savefiles, each 0x208 bytes).
#pragma once

#include <cstdio>

class FileSystemWin32 {
public:
    class _FileHandle {
    public:
        long long tell() const;          // 0x34edf0
        void seek(unsigned long long pos);  // 0x34ee10
        long long size() const;          // 0x34ee20
        bool canRead() const { return canRead_; }    // 0x34eb94
        bool canWrite() const { return canWrite_; }  // 0x34eb9c

        std::FILE* file_ = nullptr;  // +0x4
        bool canRead_ = false;       // +0xc
        bool canWrite_ = false;      // +0xd
    };

    const char* getRootPath() const;          // 0x34eba4 (+0x2c)
    const char* getResourcesPath() const;     // 0x34ebac (+0x234)
    const char* getSavefilesPath() const;     // 0x34ebb4 (+0x43c)
    void copySavefile(const char* name);      // 0x34ebc0: no-op
    bool _getFileInfo(const char* a, const char* b, void* info) const;  // 0x34ebc4: always true
    bool isFileNewerThan(const char* a, const char* b) const;           // 0x34ec00: no-op

private:
    char rootPath_[0x208];         // +0x2c
    char resourcesPath_[0x208];    // +0x234
    char savefilesPath_[0x208];    // +0x43c
};
