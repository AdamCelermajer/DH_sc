#include "core-platform/filesystem_base.hpp"

#include "core-platform/debug_out.hpp"

#include <cstdio>
#include <cstring>

#ifdef _WIN32
#include <direct.h>
#define DH2_CHDIR _chdir
#else
#include <unistd.h>
#define DH2_CHDIR chdir
#endif

namespace glitch::io {
char CFileSystem_WorkingDirectory[512] = {};
}  // namespace glitch::io

// Defined in filesystem_win32.cpp (the RES_PATH global set by the platform layer).
extern const char* RES_PATH;

// 0x34e0fc FileSystemBase::decodeObfuscatedData(int, void*, int, void*) (static)
// Hex-Rays: for index <= 3, the count is the size compared as unsigned and clamped to
// 4 - index; each of those bytes gets the running key added, where the key starts at
// ~index (low byte) and decrements per byte. The fourth parameter is not used.
void FileSystemBase::decodeObfuscatedData(int index, void* data, int size, void*) {
    if (index <= 3) {
        unsigned int count = static_cast<unsigned int>(size);
        const unsigned int limit = 4u - static_cast<unsigned int>(index);
        if (count >= limit) {
            count = limit;
        }
        if (static_cast<int>(count) > 0) {
            auto* bytes = static_cast<unsigned char*>(data);
            unsigned char key = static_cast<unsigned char>(~static_cast<unsigned int>(index));
            for (unsigned int i = 0; i != count; ++i) {
                bytes[i] = static_cast<unsigned char>(bytes[i] + key);
                key = static_cast<unsigned char>(key - 1);
            }
        }
    }
}

// 0x34e1d4 FileSystemBase::changeWorkingDirectoryTo(char const*)
bool FileSystemBase::changeWorkingDirectoryTo(const char* path) {
    bool ok = DH2_CHDIR(path) == 0;
    if (ok) {
        std::strcpy(glitch::io::CFileSystem_WorkingDirectory, path);
    }
    return ok;
}

namespace {
// Stand-in existence test: a file exists when it can be opened for reading.
bool fileExistsOnDisk(const char* name) {
    std::FILE* f = std::fopen(name, "rb");
    if (f == nullptr) {
        return false;
    }
    std::fclose(f);
    return true;
}
}  // namespace

// 0x34e1ac FileSystemBase::formatFilePath(std::string const&) const
// Returns a copy of the path; the separator rewrite is FileSystemWin32's.
std::string FileSystemBase::formatFilePath(const std::string& path) const { return path; }

// 0x34df94 FileSystemBase::doesFileExists(char const*) const
// Partial: stdio stand-in for the derived class's existence test (see the header).
bool FileSystemBase::doesFileExists(const char* name) const { return fileExistsOnDisk(name); }

// 0x34dfe0 FileSystemBase::doesResourceExists(char const*) const
// Partial: stdio stand-in; the resource-directory lookup is in the derived class.
bool FileSystemBase::doesResourceExists(const char* name) const { return fileExistsOnDisk(name); }

// 0x34e150 FileSystemBase::doesSavefileExists(char const*) const
// Partial: the Savegame job flush the original runs first is not reconstructed here.
bool FileSystemBase::doesSavefileExists(const char* name) const { return fileExistsOnDisk(name); }

// 0x34e96c FileSystemBase::ApplyFilenameHacks(char const*) const
// Partial: the name substitutions are not visible in the pseudocode; the name is kept.
std::string FileSystemBase::ApplyFilenameHacks(const char* name) const { return std::string(name); }

// 0x34e5f4 FileSystemBase::getAbsolutePath(std::string const&) const
// Partial: the derived normalisation and CFileSystem::getAbsolutePath are not
// reconstructed; the path is returned unchanged.
std::string FileSystemBase::getAbsolutePath(const std::string& path) const { return path; }

// 0x34e688 FileSystemBase::createAndOpenFile(char const*)
// Partial: the derived normalisation (vtable +0xb8) is not reproduced, so the name is
// used as given. A name without ".config" or ".glsl" that does not start with
// "/sdcard/" is opened under RES_PATH. The original then calls
// glitch::io::CFileSystem::createAndOpenFile; here the file is created with stdio.
void FileSystemBase::createAndOpenFile(const char* name) {
    std::string path(name);
    const bool underResPath = std::strstr(path.c_str(), ".config") == nullptr &&
                              std::strstr(path.c_str(), ".glsl") == nullptr &&
                              path.compare(0, 8, "/sdcard/") != 0;
    if (underResPath) {
        path = std::string(RES_PATH) + path;
    }
    std::FILE* f = std::fopen(path.c_str(), "wb");
    if (f == nullptr) {
        _DEBUG_OUT("Could not open file :  %s ", path.c_str());
        return;
    }
    std::fclose(f);
}
