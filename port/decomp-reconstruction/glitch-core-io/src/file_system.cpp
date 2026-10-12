#include "glitch-core-io/io/file_system.hpp"
#include "glitch-core-io/io/memory_read_file.hpp"

#include <cstdlib>

#ifdef _WIN32
#include <direct.h>
#define GLITCH_CHDIR _chdir
#else
#include <unistd.h>
#define GLITCH_CHDIR chdir
#endif

namespace glitch::io {

std::string CFileSystem::WorkingDirectory;

// 0x56c214 _ZN6glitch2io11CFileSystem19getWorkingDirectoryEv
const char* CFileSystem::getWorkingDirectory() {
    return WorkingDirectory.c_str();
}

// 0x56c528 _ZN6glitch2io11CFileSystem24changeWorkingDirectoryToEPKc
// The original keeps the new path in a fixed static buffer (its size is not recovered);
// a std::string is used here.
bool CFileSystem::changeWorkingDirectoryTo(const char* directory) {
    const bool changed = GLITCH_CHDIR(directory) == 0;
    if (changed) WorkingDirectory = directory;
    return changed;
}

// 0x56c4a4 _ZNK6glitch2io11CFileSystem14createFileListEv
CFileList* CFileSystem::createFileList() const {
    return new CFileList();
}

// 0x56c570 _ZN6glitch2io11CFileSystem18createAndWriteFileEPKcb
IWriteFile* CFileSystem::createAndWriteFile(const char* name, bool append) {
    return createWriteFile(name, append);
}

// 0x56c57c _ZN6glitch2io11CFileSystem20createMemoryReadFileEPviPKcb
IReadFile* CFileSystem::createMemoryReadFile(void* data, int length, const char* name, bool takeOwnership) {
    if (!data) return nullptr;
    return new CMemoryReadFile(data, length, name, takeOwnership);
}

// 0x56c414 _ZN6glitch2io11CFileSystem15createXMLWriterEPNS0_10IWriteFileE
IXMLWriter* CFileSystem::createXMLWriter(IWriteFile* file) {
    return new CXMLWriter(file);
}

// 0x56c278 _ZN6glitch2io11CFileSystem15createXMLWriterEPKc
// The file is opened for writing by name (the virtual createAndWriteFile with append false),
// the writer takes its own reference, and the local reference is released.
IXMLWriter* CFileSystem::createXMLWriter(const char* name) {
    IWriteFile* file = createAndWriteFile(name, false);
    IXMLWriter* writer = createXMLWriter(file);
    if (file) file->drop();
    return writer;
}

// 0x56c3ec _ZN6glitch2io11CFileSystem21createEmptyAttributesEPNS_5video12IVideoDriverE
CAttributes* CFileSystem::createEmptyAttributes(glitch::video::IVideoDriver* driver) {
    return new CAttributes(driver);
}

// 0x56c348 _ZNK6glitch2io11CFileSystem15getAbsolutePathERKSbIcSt11char_traitsIcENS_4core10SAllocatorIcLNS_6memory13E_MEMORY_HINTE0EEEE
std::string CFileSystem::getAbsolutePath(const std::string& path) const {
    return path;
}

// 0x56c2d8 _ZN6glitch2io16createFileSystemEv
IFileSystem* createFileSystem() {
    return new CFileSystem();
}

}  // namespace glitch::io
