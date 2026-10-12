// glitch::io::CUnZipReader (folder archive). See include/glitch-core-io/io/unzip_reader.hpp.
#include "glitch-core-io/io/unzip_reader.hpp"

namespace glitch::io {

// 0x5780fc _ZN6glitch2io12CUnZipReaderC1EPNS0_11IFileSystemEPKcbb (C2 at 0x5781b4 is the same body)
// PARTIAL: the base is built with no file (the decompile passes 0), so the directory is empty.
CUnZipReader::CUnZipReader(IFileSystem* fs, const char* name, bool flagA, bool flagB)
    : CZipReader(static_cast<IReadFile*>(nullptr), flagA, flagB), fileSystem(fs), folderName(name ? name : "") {}

// 0x577534 _ZN6glitch2io12CUnZipReaderD1Ev
// PARTIAL: the folder name is released by its own member destructor and the base drops its file
// (the original frees the name buffer explicitly when it is not the inline one).
CUnZipReader::~CUnZipReader() = default;

// 0x576dc0 _ZN6glitch2io12CUnZipReader14buildDirectoryEv
void CUnZipReader::buildDirectory() {}

// 0x576dc4 _ZN6glitch2io12CUnZipReader8findFileEPKc
int CUnZipReader::findFile(const char* name) {
    IReadFile* file = openFile(name);
    if (file == nullptr) return -1;
    file->drop();
    return 1;
}

// 0x577a48 _ZN6glitch2io12CUnZipReader8openFileEPKc
// PARTIAL: the path composition from the folder name and the requested name is not recovered
// (the decompile ends in a tail call to strlen), so this returns no file.
IReadFile* CUnZipReader::openFile(const char* name) {
    (void)name;
    return nullptr;
}

}  // namespace glitch::io
