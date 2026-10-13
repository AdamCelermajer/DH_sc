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
        // The canonical buffer is char[1024]; copy at most 1023 characters, as CFileSystem does.
        std::strncpy(glitch::io::CFileSystem::WorkingDirectory, path, sizeof(glitch::io::CFileSystem::WorkingDirectory) - 1);
        glitch::io::CFileSystem::WorkingDirectory[sizeof(glitch::io::CFileSystem::WorkingDirectory) - 1] = '\0';
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

// 0x34df94 _ZN14FileSystemBase14doesFileExistsEPKc
// Partial: the original opens the name through the virtual slot at +0x88 (FileSystemWin32
// openFile(name, false, false)) and closes it through +0x78 (closeFile); those virtual
// calls are not in this tree, so a stdio existence check stands in for them.
bool FileSystemBase::doesFileExists(const char* name) { return fileExistsOnDisk(name); }

// 0x34dfe0 _ZN14FileSystemBase18doesResourceExistsEPKc
// Partial: the original looks the name up through the virtual slot at +0x90 (openResource)
// and closes it through +0x78; that lookup is in the derived class and is not in this
// tree. A stdio existence check stands in for it.
bool FileSystemBase::doesResourceExists(const char* name) { return fileExistsOnDisk(name); }

// 0x34e150 _ZN14FileSystemBase18doesSavefileExistsEPKc
// Partial: the original calls Savegame::FlushJobs first (Savegame is not in this
// tree, so it is omitted) and then the virtual slot at +0x94 (openSavefile(name, false)).
// A stdio existence check stands in for the virtual call.
bool FileSystemBase::doesSavefileExists(const char* name) { return fileExistsOnDisk(name); }

// 0x34e96c _ZNK14FileSystemBase18ApplyFilenameHacksEPKc
// Partial: the original takes the working-directory prefix (virtual slot +0x2c,
// CFileSystem::getWorkingDirectory), strips it from the name when present, rewrites
// ".tga" names through CFileSystem::getFileBasename, and lower-cases the result with
// ToLowerCase. Those helpers are not in this tree, so the name is returned unchanged.
std::string FileSystemBase::ApplyFilenameHacks(const char* name) const { return std::string(name); }

// 0x34e5f4 _ZNK14FileSystemBase15getAbsolutePathERKSbIcSt11char_traitsIcEN6glitch4core10SAllocatorIcLNS2_6memory13E_MEMORY_HINTE0EEEE
// Partial: the original calls a virtual on this for the normalised path (slot +0xb8)
// and then glitch::io::CFileSystem::getAbsolutePath; neither is in this tree, so the
// path is returned unchanged.
std::string FileSystemBase::getAbsolutePath(const std::string& path) const { return path; }

// 0x34e688 _ZN14FileSystemBase17createAndOpenFileEPKc
// Partial: the original normalises the name through the virtual at +0xb8 before the
// checks below; that is not reproduced, so the name is used as given. A name without
// ".config" or ".glsl" that does not start with "/sdcard/" is opened under RES_PATH.
// The original then calls glitch::io::CFileSystem::createAndOpenFile (and, when
// Application::IsUsingUncompiledData is set, retries under "Q:\\"); here the file is
// created with stdio and the uncompiled-data branch is omitted.
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
