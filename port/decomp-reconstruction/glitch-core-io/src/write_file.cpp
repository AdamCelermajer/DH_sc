#include "glitch-core-io/io/write_file.hpp"

namespace glitch::io {

// 0x570b68 _ZN6glitch2io10CWriteFileC1EPKcb (C2 at 0x570c40)
// PARTIAL: the pseudocode copies the name and then tail-calls a noreturn string helper.
// createWriteFile needs the handle to be open right after construction, so the helper is
// taken to be openFile(append).
CWriteFile::CWriteFile(const char* name, bool append) : fileName(name ? name : "") {
    openFile(append);
}

// 0x570a90 _ZN6glitch2io10CWriteFileD1Ev (0x570b08 is the same body)
CWriteFile::~CWriteFile() {
    if (file) std::fclose(file);
}

// 0x5708f8 _ZNK6glitch2io10CWriteFile11getFileNameEv
const std::string& CWriteFile::getFileName() const {
    return fileName;
}

// 0x570900 _ZNK6glitch2io10CWriteFile11getFullPathEv
const std::string& CWriteFile::getFullPath() const {
    return fileName;
}

// 0x57092c _ZN6glitch2io10CWriteFile4seekElb
bool CWriteFile::seek(long offset, bool relative) {
    if (!file) return false;
    return std::fseek(file, offset, relative ? SEEK_CUR : SEEK_SET) == 0;
}

// 0x570950 _ZNK6glitch2io10CWriteFile6getPosEv
// PARTIAL: the pseudocode drops the ftell result (void). Returned here.
long CWriteFile::getPos() const {
    return file ? std::ftell(file) : 0;
}

// 0x57095c _ZN6glitch2io10CWriteFile8openFileEb
// PARTIAL: the original opens through CFileSystem::open; the mode is inferred
// as "wb" or "ab" from the bool, which is not confirmed.
void CWriteFile::openFile(bool append) {
    if (file) {
        std::fclose(file);
        file = nullptr;
    }
    if (fileName.empty()) return;
    file = std::fopen(fileName.c_str(), append ? "ab" : "wb");
    if (file) {
        std::fseek(file, 0, SEEK_END);
        size = std::ftell(file);
        std::fseek(file, 0, SEEK_SET);
    }
}

// 0x570a58 _ZN6glitch2io10CWriteFile5flushEv
void CWriteFile::flush() {
    if (file) std::fflush(file);
}

// 0x570a6c _ZN6glitch2io10CWriteFile5writeEPKvj
std::size_t CWriteFile::write(const void* data, unsigned int count) {
    if (!file) return 0;
    return std::fwrite(data, 1, count, file);
}

// 0x570bf4 _ZN6glitch2io15createWriteFileEPKcb
IWriteFile* createWriteFile(const char* name, bool append) {
    auto* file = new CWriteFile(name, append);
    if (file->isOpen()) return file;
    file->drop();
    return nullptr;
}

}  // namespace glitch::io
