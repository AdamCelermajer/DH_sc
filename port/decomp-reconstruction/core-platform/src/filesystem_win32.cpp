#include "core-platform/filesystem_win32.hpp"

#include "core-platform/deps/debug_switches.hpp"
#include "core-platform/deps/io_stream.hpp"
#include "core-platform/filesystem_base.hpp"

#include <cerrno>
#include <cstring>
#include <filesystem>
#include <utility>

#ifdef _WIN32
#include <direct.h>
#define DH2_UNLINK _unlink
#else
#include <unistd.h>
#define DH2_UNLINK unlink
#endif

// Stand-in for the RES_PATH global (set by the platform layer, owned elsewhere).
const char* RES_PATH = "./";

namespace {
// The original reads the "isTracingFileSystem" switch after a failed savefile
// operation and discards it, so only the lookups are kept here.
void traceFileSystemFailure() {
    DebugSwitches::s_inst.load();
    (void)DebugSwitches::s_inst.GetSwitch("isTracingFileSystem");
}
}  // namespace

// 0x34eba4 FileSystemWin32::getRootPath() const
// Returns the root path buffer at +0x2c (Hex-Rays: this + 44).
const char* FileSystemWin32::getRootPath() const { return rootPath_; }

// 0x34ebac FileSystemWin32::getResourcesPath() const
const char* FileSystemWin32::getResourcesPath() const { return resourcesPath_; }

// 0x34ebb4 FileSystemWin32::getSavefilesPath() const
const char* FileSystemWin32::getSavefilesPath() const { return savefilesPath_; }

// 0x34ebc0 FileSystemWin32::copySavefile(char const*): empty in the original.
void FileSystemWin32::copySavefile(const char*) {}

// 0x34ebc4 FileSystemWin32::_getFileInfo(char const*, char const*, void*) const
bool FileSystemWin32::_getFileInfo(const char*, const char*, void*) const { return true; }

// 0x34ec00 FileSystemWin32::isFileNewerThan(char const*, char const*) const: empty.
void FileSystemWin32::isFileNewerThan(const char*, const char*) const {}

// 0x34f6f4 / 0x34f788 FileSystemWin32::FileSystemWin32()
// Zeroes the five 0x104-byte path buffers (at +0x2c, +0x130, +0x234, +0x338, +0x43c),
// then fills them in _getPaths().
FileSystemWin32::FileSystemWin32() {
    std::memset(rootPath_, 0, sizeof rootPath_);
    std::memset(cwdPath_, 0, sizeof cwdPath_);
    std::memset(resourcesPath_, 0, sizeof resourcesPath_);
    std::memset(dataPath_, 0, sizeof dataPath_);
    std::memset(savefilesPath_, 0, sizeof savefilesPath_);
    _getPaths();
}

// 0x34f068 FileSystemWin32::_getPaths()
void FileSystemWin32::_getPaths() {
    std::strcpy(rootPath_, RES_PATH);
    std::strcpy(cwdPath_, RES_PATH);
    std::snprintf(dataPath_, sizeof dataPath_, "%sdata/", RES_PATH);
    std::strcpy(savefilesPath_, RES_PATH);
}

// 0x34fc18 _ZN15FileSystemWin3211_FileHandleC1EPKcS2_bb
// C2 at 0x35003c (_ZN15FileSystemWin3211_FileHandleC2EPKcS2_bb) is the same body.
// Partial: the original opens through glitch::io::CFileSystem::open and keeps the
// returned CFile; here fopen is used with the same modes. The original also lowers
// the path when the byte_99F7D4 flag is set; that flag is not in this tree.
// Modes from Hex-Rays: read-only "rb"; writable with readwrite "a+b"; writable
// without readwrite "w+b". Both writable modes set canRead and canWrite; read-only
// sets canRead only.
FileSystemWin32::_FileHandle::_FileHandle(const char* dir, const char* name, bool writable, bool readwrite) {
    std::string path = dir;
    path += name;
    if (std::strstr(name, ".savegame") != nullptr) {
        path = std::string(RES_PATH) + name;
    }
    const char* mode = "rb";
    if (writable) {
        mode = readwrite ? "a+b" : "w+b";
    }
    file_ = std::fopen(path.c_str(), mode);
    canRead_ = true;
    canWrite_ = writable;
}

// 0x34fb7c _ZN15FileSystemWin3211_FileHandleD1Ev
// D2 at 0x34fbd8 (_ZN15FileSystemWin3211_FileHandleD2Ev) has the same body.
// Partial: the original drops the glitch CFile (ISharedObject::drop); here the FILE
// is closed directly. The deleting destructor D0 at 0x34fbbc
// (_ZN15FileSystemWin3211_FileHandleD0Ev) is compiler-generated from this one; the
// original calls CustomFree after it, which is not reproduced.
FileSystemWin32::_FileHandle::~_FileHandle() {
    if (file_ != nullptr) {
        std::fclose(file_);
    }
}

// 0x34eec8 _ZN15FileSystemWin3211_FileHandle4readEPvy
// Partial: the read-gate is the byte at +0x24 of the glitch CFile in the original;
// here it is the obfuscated_ member, set by the caller. The decode count is the
// requested size, clamped by decodeObfuscatedData to 4 - pos.
std::size_t FileSystemWin32::_FileHandle::read(void* buf, std::size_t n) {
    if (!canRead_) {
        return 0;
    }
    long pos = std::ftell(file_);
    std::size_t got = std::fread(buf, 1, n, file_);
    if (obfuscated_ && pos <= 3) {
        FileSystemBase::decodeObfuscatedData(static_cast<int>(pos), buf, static_cast<int>(n), nullptr);
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

// 0x34ef58 _ZNK15FileSystemWin3211_FileHandle4peekEPvy
// Partial: same read-gate as read() (obfuscated_ stands in for the CFile byte at +0x24).
// The original then seeks back by the number of bytes read.
int FileSystemWin32::_FileHandle::peek(void* buf, std::size_t n) const {
    if (!canRead_) {
        return 0;
    }
    long pos = std::ftell(file_);
    std::size_t got = std::fread(buf, 1, n, file_);
    if (obfuscated_ && pos <= 3) {
        FileSystemBase::decodeObfuscatedData(static_cast<int>(pos), buf, static_cast<int>(n), nullptr);
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
// A leading "./" or ".\" is stripped from the directory argument (Hex-Rays: the
// prefix test is on a2, which is the directory), then the handle is opened. A handle
// whose file did not open is released and nullptr is returned.
FileSystemWin32::_FileHandle* FileSystemWin32::_createHandle(const char* dir, const char* name,
                                                             bool writable, bool readwrite) {
    if (dir[0] == '.' && (dir[1] == '/' || dir[1] == '\\')) {
        dir += 2;
    }
    auto* h = new _FileHandle(dir, name, writable, readwrite);
    if (h->file_ == nullptr) {
        delete h;
        return nullptr;
    }
    return h;
}

// 0x34fee4 FileSystemWin32::openSavefile(char const*, bool)
// Partial: the original calls Savegame::FlushJobs first; Savegame is not in this
// tree, so that call is omitted. The handle is then opened in the savefiles directory
// with readwrite = false.
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

// 0x34ffb0 _ZN15FileSystemWin328openFileEPKcbb
// Partial: the original first checks Application::IsUsingUncompiledData(name); when
// that is true it tries _createHandle("Q:\\", name, false, false) and falls back to the
// root path. Application::IsUsingUncompiledData is not in this tree, so the root path
// is always used.
FileSystemWin32::_FileHandle* FileSystemWin32::openFile(const char* name, bool writable, bool readwrite) {
    return _createHandle(rootPath_, name, writable, readwrite);
}

// 0x34f34c FileSystemWin32::makeFullPath(char const*, char const*, char*, int)
// The narrow overload only clears the output buffer in the original.
void FileSystemWin32::makeFullPath(const char*, const char*, char* out, int size) {
    std::memset(out, 0, static_cast<std::size_t>(size));
}

// 0x34f274 _ZNK15FileSystemWin3214deleteSavefileEPKc
// Partial: the header declares void, but the original returns 1 when the file was
// removed and 0 otherwise; that result cannot be reported with this signature.
void FileSystemWin32::deleteSavefile(const char* name) const {
    char path[260];
    std::snprintf(path, sizeof path, "%s%s", RES_PATH, name);
    if (DH2_UNLINK(path) != 0) {
        traceFileSystemFailure();
    }
}

// 0x34f10c _ZNK15FileSystemWin3214backupSavefileEPKcS1_
// Removes any existing destination (ENOENT is fine), then renames src to dst. Returns
// false when the unlink failed for another reason or the rename failed.
bool FileSystemWin32::backupSavefile(const char* src, const char* dst) const {
    char from[260];
    char to[260];
    std::snprintf(from, sizeof from, "%s%s", RES_PATH, src);
    std::snprintf(to, sizeof to, "%s%s", RES_PATH, dst);
    int rc = DH2_UNLINK(to);
    if (rc == 0 || errno == ENOENT) {
        if (std::rename(from, to) != 0) {
            traceFileSystemFailure();
            return false;
        }
        return true;
    }
    traceFileSystemFailure();
    return false;
}

// 0x34f8a4 _ZNK15FileSystemWin3214formatFilePathERKSbIcSt11char_traitsIcEN6glitch4core10SAllocatorIcLNS2_6memory13E_MEMORY_HINTE0EEEE
// Every '/' in the path is replaced by '\\' and the rest is copied unchanged. The
// original builds a glitch string; std::string is returned here.
std::string FileSystemWin32::formatFilePath(const std::string& path) const {
    std::string out = path;
    for (char& c : out) {
        if (c == '/') {
            c = '\\';
        }
    }
    return out;
}

// 0x3505a0 _ZNK15FileSystemWin328getFilesEPKcRSt6vectorISsSaISsEE
// The original does not use dir: it lists RES_PATH and appends every entry name (no
// file-type filter). Partial: the original reads the entry name at a fixed offset in
// the platform dirent, and std::filesystem is used here, so "." and ".." are not listed.
void FileSystemWin32::getFiles(const char* /*dir*/, std::vector<std::string>& out) const {
    namespace fs = std::filesystem;
    std::error_code ec;
    for (const auto& entry : fs::directory_iterator(RES_PATH, ec)) {
        out.push_back(entry.path().filename().string());
    }
}

// 0x35041c _ZNK15FileSystemWin3216getFilesMatchingEPKcS1_RSt6vectorISsSaISsEE
// Lists the entries of dir whose name contains pattern as a substring (an empty
// pattern matches every name). Partial: the entry name is taken from std::filesystem
// rather than the platform dirent offset used by the original, and "." and ".." are
// not listed. The original's int return code on opendir failure cannot be reported
// with the void signature.
void FileSystemWin32::getFilesMatching(const char* dir, const char* pattern,
                                       std::vector<std::string>& out) const {
    namespace fs = std::filesystem;
    const std::string pat(pattern);
    std::error_code ec;
    for (const auto& entry : fs::directory_iterator(dir, ec)) {
        std::string name = entry.path().filename().string();
        if (name.find(pat) != std::string::npos) {
            out.push_back(std::move(name));
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
