#include "glitch-core-io/io/file_system.hpp"
#include "glitch-core-io/io/memory_read_file.hpp"
#include "glitch-core-io/io/pak_reader.hpp"
#include "glitch-core-io/io/unzip_reader.hpp"
#include "glitch-core-io/io/zip_reader.hpp"

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

// 0x56c058 _ZN6glitch2io11CFileSystemC2Ev and 0x56c0a8 _ZN6glitch2io11CFileSystemC1Ev
// The three archive lists start empty (their default construction zeroes them, as the original
// does at +0x08..+0x2c). The refcount is set to 0, not the base default of 1, because the
// original's constructor stores 0 at +0x04 and createFileSystem returns that object as it is.
CFileSystem::CFileSystem() {
    referenceCounter = 0;
}

// 0x56c214 _ZN6glitch2io11CFileSystem19getWorkingDirectoryEv
const char* CFileSystem::getWorkingDirectory() {
    return WorkingDirectory.c_str();
}

// 0x56c528 _ZN6glitch2io11CFileSystem24changeWorkingDirectoryToEPKc
// chdir; on success the path is copied with strcpy (LC_API_STRCPY -> PLT strcpy) into the 1024-byte
// static buffer. The port keeps a std::string, so a path of 1024 bytes or more is kept whole where
// the original would overflow the buffer (undefined in the original, not reproduced).
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
// Mirrors the listing: with keepExtension false the extension length is size - (last '.') when a
// dot exists, else 0. A separator before size gives substr(sep + 1, count) with count =
// size - ext - (sep + 1) in unsigned 32-bit arithmetic. When the dot lies before the separator
// that count wraps, and the substring is clamped to the end of the path (the rest after the
// separator). With no separator in range the result is the path minus its extension.
std::string CFileSystem::getFileBasename(const std::string& path, bool keepExtension) const {
    const long sep = lastSeparator(path);
    const std::size_t size = path.size();
    std::size_t extLength = 0;
    if (!keepExtension) {
        const std::size_t dot = path.rfind('.');
        if (dot != std::string::npos) extLength = size - dot;
    }
    if (sep >= 0 && static_cast<std::size_t>(sep) < size) {
        const std::size_t start = static_cast<std::size_t>(sep) + 1;
        const std::size_t end = size - extLength;
        if (end < start) return path.substr(start);  // the original's count wraps and clamps
        return path.substr(start, end - start);
    }
    if (extLength != 0) return path.substr(0, size - extLength);
    return path;
}

// 0x56cb78 _ZN6glitch2io11CFileSystem5clearEv
// Each list is walked from its first element; every reader is dropped (the reference the list
// held), then the list is emptied. The walk re-reads the size after each drop, as the original does.
void CFileSystem::clear() {
    for (std::size_t i = 0; i < zipReaders.size(); ++i) zipReaders[i]->drop();
    zipReaders.clear();
    for (std::size_t i = 0; i < pakReaders.size(); ++i) pakReaders[i]->drop();
    pakReaders.clear();
    for (std::size_t i = 0; i < unZipReaders.size(); ++i) unZipReaders[i]->drop();
    unZipReaders.clear();
}

// 0x56cc44 _ZN6glitch2io11CFileSystemD1Ev and 0x56ccc4 _ZN6glitch2io11CFileSystemD2Ev
// clear() drops the readers. The three lists are declared zip, pak, unzip, so their storage is
// released afterwards in the original's order (unzip, pak, zip).
CFileSystem::~CFileSystem() { clear(); }

// 0x56df38 _ZNK6glitch2io11CFileSystem9existFileEPKc
// The three archive lists are searched in order (zip, pak, unzip): the first reader that finds the
// name answers 1. Otherwise the file is opened for reading. PARTIAL: the original's last step is
// CFileSystem::open(name, "rb") (0x56dc40, not reconstructed: it returns a shared CFile and applies
// the obfuscation map); here fopen stands in for it.
int CFileSystem::existFile(const char* name) const {
    for (CZipReader* zip : zipReaders) {
        if (zip->findFile(name) != -1) return 1;
    }
    for (CPakReader* pak : pakReaders) {
        if (pak->findFile(name) != -1) return 1;
    }
    for (CUnZipReader* unzip : unZipReaders) {
        if (unzip->findFile(name) != -1) return 1;
    }
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
