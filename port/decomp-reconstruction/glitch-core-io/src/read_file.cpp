#include "glitch-core-io/io/read_file.hpp"

#include <cstdint>

namespace glitch::io {

// 0x570734 _ZN6glitch2io9CReadFileC1EPKcb (C2 at 0x570874 is the same body)
CReadFile::CReadFile(const char* name, bool obfuscatedFlag) : fileName(name ? name : "") {
    obfuscated = obfuscatedFlag;
    openFile();
    // The original clears the flag for files shorter than the 4-byte key.
    if (obfuscated && size < 4) obfuscated = false;
}

// 0x570660 _ZN6glitch2io9CReadFileD1Ev
CReadFile::~CReadFile() {
    if (file) std::fclose(file);  // the original drops the shared handle
}

// 0x570808 _ZNK6glitch2io9CReadFile5cloneEv
CReadFile* CReadFile::clone() const {
    CReadFile* copy = new CReadFile(fileName.c_str(), false);
    copy->obfuscated = obfuscated;
    copy->seek(getPos(), false);
    return copy;
}

// 0x5703e4 _ZNK6glitch2io9CReadFile6isOpenEv
bool CReadFile::isOpen() const {
    return file != nullptr;
}

// 0x57044c _ZNK6glitch2io9CReadFile7getSizeEv
long CReadFile::getSize() const {
    return size;
}

// 0x570454 _ZNK6glitch2io9CReadFile11getFileNameEv
const std::string& CReadFile::getFileName() const {
    return fileName;
}

// 0x57045c _ZNK6glitch2io9CReadFile11getFullPathEv
// No CFileSystem here, so the full path is the name the reader was given.
const std::string& CReadFile::getFullPath() const {
    return fileName;
}

// 0x570488 _ZN6glitch2io9CReadFile4seekElb
// The pseudocode passes the bool straight to fseek as the origin: true is
// SEEK_CUR (1), false is SEEK_SET (0).
bool CReadFile::seek(long offset, bool relative) {
    if (!isOpen()) return false;
    return std::fseek(file, offset, relative ? SEEK_CUR : SEEK_SET) == 0;
}

// 0x5705a8 _ZNK6glitch2io9CReadFile6getPosEv
// The original returns ftell of the handle's FILE* (handle +0x04) with no null test. PARTIAL: the
// port keeps the FILE* at +0x0c and returns 0 when the file is closed (the original has no such case).
long CReadFile::getPos() const {
    return file ? std::ftell(file) : 0;
}

// 0x5705b4 _ZN6glitch2io9CReadFile4readEPvj
// Obfuscated files: the first four bytes are adjusted by (~pos - i) for
// byte i, where pos is the position before the read.
void CReadFile::read(void* buffer, unsigned int count) {
    if (!isOpen()) return;
    const std::uint32_t pos = static_cast<std::uint32_t>(getPos());
    std::fread(buffer, 1, count, file);
    if (obfuscated && static_cast<std::int32_t>(pos) < 4) {
        std::uint32_t n = 4 - pos;
        if (count <= n) n = count;
        auto* bytes = static_cast<std::uint8_t*>(buffer);
        std::uint32_t key = ~pos;
        for (std::uint32_t i = 0; i < n; ++i) {
            bytes[i] = static_cast<std::uint8_t>(bytes[i] + static_cast<std::uint8_t>(key - i));
        }
    }
}

// 0x5704cc _ZN6glitch2io9CReadFile8openFileEv
// PARTIAL: the original takes the handle from CFileSystem::open and then reads
// the obfuscation flag from the shared CFile (+0x24). That flag is not
// reconstructed; the constructor argument is kept.
void CReadFile::openFile() {
    if (file) {
        std::fclose(file);
        file = nullptr;
    }
    if (fileName.empty()) return;
    file = std::fopen(fileName.c_str(), "rb");
    if (file) {
        std::fseek(file, 0, SEEK_END);
        size = std::ftell(file);
        std::fseek(file, 0, SEEK_SET);
    }
}

// The base slot +0x10 is abstract in the original. The port's default forwards to the
// +0x14 overload at the current position, which is what CReadFile does.
void IReadFile::readAsync(void* buffer, unsigned int count, ReadAsyncCallBack callBack, void* context) {
    readAsync(buffer, count, getPos(), callBack, context);
}

// 0x5703f4 _ZN6glitch2io9CReadFile9readAsyncEPvjPFviiPNS0_9IReadFileES2_ES2_
// Forwards to the position overload (vtable +0x14) with the current position.
void CReadFile::readAsync(void* buffer, unsigned int count, ReadAsyncCallBack callBack, void* context) {
    readAsync(buffer, count, getPos(), callBack, context);
}

// 0x570444 _ZN6glitch2io9CReadFile9readAsyncEPvjlPFviiPNS0_9IReadFileES2_ES2_
// The original body is `return 1`: the request is accepted and no callback is run here.
bool CReadFile::readAsync(void* /*buffer*/, unsigned int /*count*/, long /*position*/,
                          ReadAsyncCallBack /*callBack*/, void* /*context*/) {
    return true;
}

// 0x5707b4 _ZN6glitch2io14createReadFileEPKc
IReadFile* createReadFile(const char* name) {
    auto* file = new CReadFile(name, false);
    if (file->isOpen()) return file;
    file->drop();
    return nullptr;
}

}  // namespace glitch::io
