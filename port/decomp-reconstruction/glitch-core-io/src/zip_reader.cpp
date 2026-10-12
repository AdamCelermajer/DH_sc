#include "glitch-core-io/io/zip_reader.hpp"

#include <cstring>

namespace glitch::io {

// 0x576e78 _ZN6glitch2io13SZipFileEntryC1ERKS1_
SZipFileEntry::SZipFileEntry(const SZipFileEntry& other)
    : path(other.path),
      name(other.name),
      extra(other.extra),
      f48(other.f48),
      f4c(other.f4c),
      f50(other.f50),
      f54(other.f54),
      f58(other.f58),
      f5c(other.f5c),
      f60(other.f60),
      f64(other.f64),
      f68(other.f68) {}

// 0x577284 _ZN6glitch2io13SZipFileEntryaSERKS1_
SZipFileEntry& SZipFileEntry::operator=(const SZipFileEntry& other) {
    if (this != &other) {
        path = other.path;
        name = other.name;
        extra = other.extra;
    }
    f48 = other.f48;
    f4c = other.f4c;
    f50 = other.f50;
    f54 = other.f54;
    f58 = other.f58;
    f5c = other.f5c;
    f60 = other.f60;
    f64 = other.f64;
    f68 = other.f68;
    return *this;
}

// 0x576cf0 _ZN6glitch2io10CZipReader12getFileCountEv
int CZipReader::getFileCount() {
    return static_cast<int>(entries.size());
}

// 0x576d1c _ZNK6glitch2io10CZipReader11getFileInfoEi
const SZipFileEntry* CZipReader::getFileInfo(int index) const {
    return entries.data() + index;
}

// 0x576d2c _ZN6glitch2io10CZipReader7isValidEPNS0_9IReadFileE
bool CZipReader::isValid(IReadFile* file) {
    const long position = file->getPos();
    file->seek(0, false);
    std::uint32_t signature = 0;
    file->read(&signature, 4);
    file->seek(position, false);
    return signature == 0x04034b50u;
}

// 0x576ef4 _ZN6glitch2io10CZipReader7isValidEPKc
bool CZipReader::isValid(const char* fileName) {
    IReadFile* file = createReadFile(fileName);
    if (!file) return false;
    const bool valid = isValid(file);
    file->drop();
    return valid;
}

// 0x577b50 _ZN6glitch2io10CZipReader22deletePathFromFilenameERSbIcSt11char_traitsIcENS_4core10SAllocatorIcLNS_6memory13E_MEMORY_HINTE0EEEE
void CZipReader::deletePathFromFilename(std::string& fileName) {
    // Scan from the last character back; the first separator found decides the cut.
    for (std::size_t i = fileName.size(); i-- > 0;) {
        if (fileName[i] == '/' || fileName[i] == '\\') {
            if (i != 0) fileName = fileName.substr(i + 1);
            return;
        }
    }
}

}  // namespace glitch::io
