// glitch::io::CUnZipReader (folder archive). See include/glitch-core-io/io/unzip_reader.hpp.
#include "glitch-core-io/io/unzip_reader.hpp"

namespace glitch::io {

// 0x5780fc _ZN6glitch2io12CUnZipReaderC1EPNS0_11IFileSystemEPKcbb (C2 at 0x5781b4 is the same body)
// PARTIAL: the base is built with no file (the decompile passes 0), so the directory is empty. The
// original then appends '/' to the folder name unless it ends in '\\' or '/'. That test reads a byte at
// the folder string's end pointer (layout not resolved), so the separator is not added here.
CUnZipReader::CUnZipReader(IFileSystem* fs, const char* name, bool flagA, bool flagB)
    : CZipReader(static_cast<IReadFile*>(nullptr), flagA, flagB), fileSystem(fs), folderName(name ? name : "") {}

// 0x577534 _ZN6glitch2io12CUnZipReaderD1Ev
// PARTIAL: the folder name is released by its own member destructor and the base drops its file
// (the original frees the name buffer explicitly when it is not the inline one).
CUnZipReader::~CUnZipReader() = default;

// 0x576dc0 _ZN6glitch2io12CUnZipReader14buildDirectoryEv
void CUnZipReader::buildDirectory() {}

// 0x576dc4 _ZN6glitch2io12CUnZipReader8findFileEPKc
// PARTIAL: the body matches (openFile, then -1 for no file or 1 with the file released). The original
// reaches openFile through vtable +0x0c; here the call is direct, because openFile(const char*) is not
// virtual in zip_reader.hpp. The result follows openFile, which is partial and returns no file.
int CUnZipReader::findFile(const char* name) {
    IReadFile* file = openFile(name);
    if (file == nullptr) return -1;
    file->drop();
    return 1;
}

// 0x577a48 _ZN6glitch2io12CUnZipReader8openFileEPKc
// PARTIAL: the original copies the folder string, appends the requested name, builds a CUnzipReadFile on
// that path and keeps it only when its vtable +0x3c check passes (else it drops it and returns null). The
// folder string's bounds (the pointers at +0x34 and +0x38) are not resolved, so this returns no file.
IReadFile* CUnZipReader::openFile(const char* name) {
    (void)name;
    return nullptr;
}

}  // namespace glitch::io
