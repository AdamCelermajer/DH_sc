#include "core-platform/filesystem_win32.hpp"

#include "core-platform/deps/io_stream.hpp"
#include "core-platform/filesystem_base.hpp"

#include <cerrno>
#include <cstring>
#include <filesystem>

#ifdef _WIN32
#include <direct.h>
#define DH2_UNLINK _unlink
#else
#include <unistd.h>
#define DH2_UNLINK unlink
#endif

// Stand-in for the RES_PATH global (set by the platform layer, owned elsewhere).
const char* RES_PATH = "./";

// 0x34eba4 FileSystemWin32::getRootPath() const
// Partial: returns the path buffer; the original returns the string object address.
const char* FileSystemWin32::getRootPath() const { return rootPath_; }

// 0x34ebac FileSystemWin32::getResourcesPath() const
const char* FileSystemWin32::getResourcesPath() const { return resourcesPath_; }

// 0x34ebb4 FileSystemWin32::getSavefilesPath() const
const char* FileSystemWin32::getSavefilesPath() const { return savefilesPath_; }

// 0x34ebc0 FileSystemWin32::copySavefile(char const*): empty in the original.
void FileSystemWin32::copySavefile(const char*) {}

// 0x34ebc4 FileSystemWin32::_getFileInfo(char const*, char const*, void*) const
bool FileSystemWin32::_getFileInfo(const char*, const char*, void*) const { return true; }

// 0x34ec00 FileSystemWin32::isFileNewerThan(char const*, char const*) const: no-op.
// Partial: the original returns nothing visible; false is returned here.
bool FileSystemWin32::isFileNewerThan(const char*, const char*) const { return false; }

// 0x34f6f4 / 0x34f788 FileSystemWin32::FileSystemWin32()
FileSystemWin32::FileSystemWin32() { _getPaths(); }

// 0x34f068 FileSystemWin32::_getPaths()
void FileSystemWin32::_getPaths() {
    std::strcpy(rootPath_, RES_PATH);
    std::strcpy(cwdPath_, RES_PATH);
    std::snprintf(dataPath_, sizeof dataPath_, "%sdata/", RES_PATH);
    std::strcpy(savefilesPath_, RES_PATH);
}

// 0x34fc18 / 0x35003c FileSystemWin32::_FileHandle::_FileHandle(char const*, char const*, bool, bool)
// Partial: the original opens through glitch::io::CFileSystem with a shared
// object count. Here stdio is used and the open mode strings are inferred:
// read-only is "rb"; a write handle is "wb" or "r+b" (readwrite).
FileSystemWin32::_FileHandle::_FileHandle(const char* dir, const char* name, bool writable, bool readwrite) {
    std::string path = dir;
    path += name;
    if (std::strstr(name, ".savegame") != nullptr) {
        path = std::string(RES_PATH) + name;
    }
    const char* mode = "rb";
    if (writable) {
        mode = readwrite ? "r+b" : "wb";
    }
    file_ = std::fopen(path.c_str(), mode);
    canRead_ = true;
    canWrite_ = writable;
}

// 0x34eb9c etc. dtors (0x34fb7c, 0x34fbbc, 0x34fbd8) are compiler-generated.
FileSystemWin32::_FileHandle::~_FileHandle() {
    if (file_ != nullptr) {
        std::fclose(file_);
    }
}

// 0x34eec8 FileSystemWin32::_FileHandle::read(void*, unsigned long long)
// Partial: the obfuscation decode for the first bytes is applied as in the
// original (position below 4 and the obfuscated flag set).
std::size_t FileSystemWin32::_FileHandle::read(void* buf, std::size_t n) {
    if (!canRead_) {
        return 0;
    }
    long pos = std::ftell(file_);
    std::size_t got = std::fread(buf, 1, n, file_);
    if (obfuscated_ && pos < 4) {
        FileSystemBase::decodeObfuscatedData(static_cast<int>(pos), buf, static_cast<int>(n));
    }
    return got;
}

// 0x34ee74 FileSystemWin32::_FileHandle::write(void const*, unsigned long long)
std::size_t FileSystemWin32::_FileHandle::write(const void* buf, std::size_t n) {
    if (!canWrite_) {
        return 0;
    }
    return std::fwrite(buf, 1, n, file_);
}

// 0x34ef58 FileSystemWin32::_FileHandle::peek(void*, unsigned long long) const
// Partial: same decode rule as read(); the decode is applied to the peeked bytes.
int FileSystemWin32::_FileHandle::peek(void* buf, std::size_t n) const {
    if (!canRead_) {
        return 0;
    }
    long pos = std::ftell(file_);
    std::size_t got = std::fread(buf, 1, n, file_);
    if (obfuscated_ && pos < 4) {
        FileSystemBase::decodeObfuscatedData(static_cast<int>(pos), buf, static_cast<int>(n));
    }
    std::fseek(file_, -static_cast<long>(got), SEEK_CUR);
    return static_cast<int>(got);
}

// 0x34edf0 FileSystemWin32::_FileHandle::tell() const
long long FileSystemWin32::_FileHandle::tell() const { return static_cast<long long>(std::ftell(file_)); }

// 0x34ee10 FileSystemWin32::_FileHandle::seek(unsigned long long)
void FileSystemWin32::_FileHandle::seek(unsigned long long pos) {
    std::fseek(file_, static_cast<long>(pos), SEEK_SET);
}

// 0x34ee20 FileSystemWin32::_FileHandle::size() const
long long FileSystemWin32::_FileHandle::size() const {
    long saved = std::ftell(file_);
    std::fseek(file_, 0, SEEK_END);
    long end = std::ftell(file_);
    std::fseek(file_, saved, SEEK_SET);
    return static_cast<long long>(end);
}

// 0x34fe50 FileSystemWin32::_createHandle(char const*, char const*, bool, bool)
// Strips a leading "./" or ".\" from the name, as the original does, then opens.
FileSystemWin32::_FileHandle* FileSystemWin32::_createHandle(const char* dir, const char* name,
                                                             bool writable, bool readwrite) {
    if (name[0] == '.' && (name[1] == '/' || name[1] == '\\')) {
        name += 2;
    }
    auto* h = new _FileHandle(dir, name, writable, readwrite);
    if (h->file_ == nullptr) {
        delete h;
        return nullptr;
    }
    return h;
}

// 0x34fee4 FileSystemWin32::openSavefile(char const*, bool)
// The original flushes pending Savegame jobs first; that call is not part of this build.
FileSystemWin32::_FileHandle* FileSystemWin32::openSavefile(const char* name, bool writable) {
    return _createHandle(savefilesPath_, name, writable, false);
}

// 0x34ff28 FileSystemWin32::openResource(char const*)
FileSystemWin32::_FileHandle* FileSystemWin32::openResource(const char* name) {
    _FileHandle* h = _createHandle(resourcesPath_, name, false, false);
    if (h == nullptr) {
        h = _createHandle(dataPath_, name, false, false);
    }
    return h;
}

// 0x34ff84 FileSystemWin32::openTraceFile(char const*)
FileSystemWin32::_FileHandle* FileSystemWin32::openTraceFile(const char* name) {
    return _createHandle(savefilesPath_, name, true, true);
}

// 0x34ffb0 FileSystemWin32::openFile(char const*, bool, bool)
// Partial: the original first tries an uncompiled-data path when the application
// reports it is using uncompiled data; that path string is not recovered.
FileSystemWin32::_FileHandle* FileSystemWin32::openFile(const char* name, bool writable, bool readwrite) {
    return _createHandle(rootPath_, name, writable, readwrite);
}

// 0x34f34c FileSystemWin32::makeFullPath(char const*, char const*, char*, int)
// The narrow overload only clears the output buffer in the original.
void FileSystemWin32::makeFullPath(const char*, const char*, char* out, int size) {
    std::memset(out, 0, static_cast<std::size_t>(size));
}

// 0x34f274 FileSystemWin32::deleteSavefile(char const*) const
// Partial: the trace-switch logging on failure (DebugSwitches) is omitted.
void FileSystemWin32::deleteSavefile(const char* name) const {
    char path[260];
    std::snprintf(path, sizeof path, "%s%s", RES_PATH, name);
    DH2_UNLINK(path);
}

// 0x34f10c FileSystemWin32::backupSavefile(char const*, char const*) const
// Removes any existing destination (ENOENT is fine), then renames src to dst.
// Partial: the trace-switch logging on failure is omitted.
bool FileSystemWin32::backupSavefile(const char* src, const char* dst) const {
    char from[260];
    char to[260];
    std::snprintf(from, sizeof from, "%s%s", RES_PATH, src);
    std::snprintf(to, sizeof to, "%s%s", RES_PATH, dst);
    int rc = DH2_UNLINK(to);
    if (rc == 0 || errno == ENOENT) {
        return std::rename(from, to) == 0;
    }
    return false;
}

// 0x34f8a4 FileSystemWin32::formatFilePath(std::string const&) const
// Partial: the original rewrites '/' separators into '\\' with a string-manipulation
// loop; this version performs the same replacement and leaves other text alone.
std::string FileSystemWin32::formatFilePath(const std::string& path) const {
    std::string out = path;
    for (char& c : out) {
        if (c == '/') {
            c = '\\';
        }
    }
    return out;
}

// 0x3505a0 FileSystemWin32::getFiles(char const*, std::vector<std::string>&) const
// Partial: lists regular files in dir via <filesystem>; the original uses the Win32 finder.
void FileSystemWin32::getFiles(const char* dir, std::vector<std::string>& out) const {
    namespace fs = std::filesystem;
    std::error_code ec;
    for (const auto& entry : fs::directory_iterator(dir, ec)) {
        if (entry.is_regular_file(ec)) {
            out.push_back(entry.path().filename().string());
        }
    }
}

// 0x35041c FileSystemWin32::getFilesMatching(char const*, char const*, std::vector<std::string>&) const
// Partial: the pattern supports a leading or trailing '*' only (no full wildcard set).
void FileSystemWin32::getFilesMatching(const char* dir, const char* pattern,
                                       std::vector<std::string>& out) const {
    std::vector<std::string> all;
    getFiles(dir, all);
    std::string pat = pattern;
    for (const std::string& f : all) {
        bool ok = pat.empty() || pat == "*";
        if (!ok && pat.front() == '*') {
            std::string suffix = pat.substr(1);
            ok = f.size() >= suffix.size() && f.compare(f.size() - suffix.size(), suffix.size(), suffix) == 0;
        } else if (!ok && pat.back() == '*') {
            std::string prefix = pat.substr(0, pat.size() - 1);
            ok = f.compare(0, prefix.size(), prefix) == 0;
        } else if (!ok) {
            ok = f == pat;
        }
        if (ok) {
            out.push_back(f);
        }
    }
}

// 0x34ebcc FileSystemWin32::closeFile(IFileStream*&)
// The original calls vtable slot +0x4 (the deleting destructor) and clears the pointer.
void FileSystemWin32::closeFile(IFileStream*& stream) {
    if (stream != nullptr) {
        delete stream;
        stream = nullptr;
    }
}

// 0x34ebfc FileSystemWin32::getFolders(char const*, std::vector<std::string>&) const
// Empty in the original.
void FileSystemWin32::getFolders(const char*, std::vector<std::string>&) const {}
