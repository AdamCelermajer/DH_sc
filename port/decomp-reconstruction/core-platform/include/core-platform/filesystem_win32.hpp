// FileSystemWin32 and its _FileHandle: the original Windows file system, on
// stdio. Member offsets follow the original; each path buffer is 0x104 bytes:
// root +0x2c, working/RES copy +0x130, resources +0x234, data +0x338, savefiles +0x43c.
#pragma once

#include <cstddef>
#include <cstdio>
#include <string>
#include <vector>

class IFileStream;

class FileSystemWin32 {
public:
    class _FileHandle {
    public:
        // 0x34fc18 / 0x35003c: opens dir+name (leading "./" stripped by the caller).
        // writable=false opens read-only. readwrite selects the write mode.
        _FileHandle(const char* dir, const char* name, bool writable, bool readwrite);
        ~_FileHandle();

        std::size_t read(void* buf, std::size_t n);         // 0x34eec8
        std::size_t write(const void* buf, std::size_t n);  // 0x34ee74
        int peek(void* buf, std::size_t n) const;           // 0x34ef58
        long long tell() const;                             // 0x34edf0
        void seek(unsigned long long pos);                  // 0x34ee10
        long long size() const;                             // 0x34ee20
        bool canRead() const { return canRead_; }           // 0x34eb94
        bool canWrite() const { return canWrite_; }         // 0x34eb9c

        std::FILE* file_ = nullptr;   // +0x4
        bool canRead_ = false;        // +0xc
        bool canWrite_ = false;       // +0xd
        bool obfuscated_ = false;     // +0x24 of the original file object (decode on read)
    };

    FileSystemWin32();   // 0x34f6f4 / 0x34f788: calls _getPaths()

    const char* getRootPath() const;         // 0x34eba4 (+0x2c)
    const char* getResourcesPath() const;    // 0x34ebac (+0x234)
    const char* getSavefilesPath() const;    // 0x34ebb4 (+0x43c)
    void copySavefile(const char* name);     // 0x34ebc0: no-op
    void closeFile(IFileStream*& stream);    // 0x34ebcc: deletes the stream and clears it
    void getFolders(const char* dir, std::vector<std::string>& out) const;  // 0x34ebfc: no-op
    bool _getFileInfo(const char* a, const char* b, void* info) const;  // 0x34ebc4
    void isFileNewerThan(const char* a, const char* b) const;          // 0x34ec00 (empty)

    _FileHandle* openSavefile(const char* name, bool writable);  // 0x34fee4
    _FileHandle* openResource(const char* name);                 // 0x34ff28
    _FileHandle* openTraceFile(const char* name);                // 0x34ff84
    _FileHandle* openFile(const char* name, bool writable, bool readwrite);  // 0x34ffb0
    _FileHandle* _createHandle(const char* dir, const char* name, bool writable, bool readwrite);  // 0x34fe50

    bool backupSavefile(const char* src, const char* dst) const;  // 0x34f10c
    void deleteSavefile(const char* name) const;                  // 0x34f274
    std::string formatFilePath(const std::string& path) const;    // 0x34f8a4
    void getFiles(const char* dir, std::vector<std::string>& out) const;  // 0x3505a0
    void getFilesMatching(const char* dir, const char* pattern, std::vector<std::string>& out) const;  // 0x35041c

private:
    void _getPaths();                                  // 0x34f068
    static void makeFullPath(const char* a, const char* b, char* out, int size);  // 0x34f34c

    char rootPath_[0x104];       // +0x2c
    char cwdPath_[0x104];        // +0x130
    char resourcesPath_[0x104];  // +0x234
    char dataPath_[0x104];       // +0x338
    char savefilesPath_[0x104];  // +0x43c
};
