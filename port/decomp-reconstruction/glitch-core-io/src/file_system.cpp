#include "glitch-core-io/io/file_system.hpp"
#include "glitch-core-io/io/memory_read_file.hpp"

#include <cstdio>
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

namespace {

// Position of the last '/' or '\\' as the original computes it: the larger of the two
// reverse searches, with npos read as -1 (signed compare). Returns -1 when neither is found.
long lastSeparator(const std::string& path) {
    const std::size_t slash = path.rfind('/');
    const std::size_t back = path.rfind('\\');
    const long a = slash == std::string::npos ? -1 : static_cast<long>(slash);
    const long b = back == std::string::npos ? -1 : static_cast<long>(back);
    return a < b ? b : a;
}

}  // namespace

// 0x56c6b4 _ZNK6glitch2io11CFileSystem10getFileDirERKSbIcSt11char_traitsIcENS_4core10SAllocatorIcLNS_6memory13E_MEMORY_HINTE0EEEE
std::string CFileSystem::getFileDir(const std::string& path) const {
    const long sep = lastSeparator(path);
    if (sep >= 0 && static_cast<std::size_t>(sep) < path.size()) {
        return path.substr(0, static_cast<std::size_t>(sep));
    }
    return ".";
}

// 0x56c740 _ZNK6glitch2io11CFileSystem15getFileBasenameERKSbIcSt11char_traitsIcENS_4core10SAllocatorIcLNS_6memory13E_MEMORY_HINTE0EEEEb
std::string CFileSystem::getFileBasename(const std::string& path, bool keepExtension) const {
    const long sep = lastSeparator(path);
    const std::size_t size = path.size();
    // Without the extension the end is the last '.' (when there is one).
    std::size_t end = size;
    if (!keepExtension) {
        const std::size_t dot = path.rfind('.');
        if (dot != std::string::npos) end = dot;
    }
    const std::size_t start = sep >= 0 ? static_cast<std::size_t>(sep) + 1 : 0;
    if (start >= end) return std::string();
    return path.substr(start, end - start);
}

// 0x56cb78 _ZN6glitch2io11CFileSystem5clearEv
// PARTIAL: the original drops every reference in its three archive lists and empties them.
// The port keeps no archive lists, so there is nothing to drop here.
void CFileSystem::clear() {}

// 0x56cc44 _ZN6glitch2io11CFileSystemD1Ev and 0x56ccc4 _ZN6glitch2io11CFileSystemD2Ev
// PARTIAL: clear() and then the release of the list buffers, which are not modelled.
CFileSystem::~CFileSystem() { clear(); }

// 0x56df38 _ZNK6glitch2io11CFileSystem9existFileEPKc
// PARTIAL: the archive lists are not searched. The last step of the original opens the file
// for reading and releases it again when it opens; that step is kept.
int CFileSystem::existFile(const char* name) const {
    std::FILE* file = std::fopen(name, "rb");
    if (file == nullptr) return 0;
    std::fclose(file);
    return 1;
}

// 0x34df7c _ZNK6glitch2io11CFileSystem9existFileERKSbIcSt11char_traitsIcENS_4core10SAllocatorIcLNS_6memory13E_MEMORY_HINTE0EEEE
int CFileSystem::existFile(const std::string& name) const { return existFile(name.c_str()); }

// 0x56c2d8 _ZN6glitch2io16createFileSystemEv
IFileSystem* createFileSystem() {
    return new CFileSystem();
}

}  // namespace glitch::io
