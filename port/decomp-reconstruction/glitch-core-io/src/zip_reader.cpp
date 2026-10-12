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

// 0x577fa4 _ZN6glitch2io10CZipReaderC1EPNS0_9IReadFileEbb (C2 at 0x578050 is the same body)
// PARTIAL: the scan loop runs scanLocalHeader until it reports no entry. The heapsort of the
// entries that follows in the original is not applied (its element comparator is not verified).
CZipReader::CZipReader(IReadFile* file, bool a, bool b) : readFile(file), flagA(a), flagB(b) {
    if (readFile) {
        ++readFile->referenceCounter;
        while (scanLocalHeader()) {
        }
    }
}

// 0x57826c _ZN6glitch2io10CZipReaderC1EPKcbb (C2 at 0x578318 is the same body)
// PARTIAL: as the file form; the file is opened by name (createReadFile) and owned.
CZipReader::CZipReader(const char* fileName, bool a, bool b) : flagA(a), flagB(b) {
    readFile = createReadFile(fileName);
    if (readFile) {
        while (scanLocalHeader()) {
        }
    }
}

// 0x577488 _ZN6glitch2io10CZipReaderD1Ev (D2 at 0x5774ec is the same body)
CZipReader::~CZipReader() {
    if (readFile) readFile->drop();
}

// 0x577db4 _ZN6glitch2io10CZipReader15scanLocalHeaderEv
// PARTIAL: the pseudocode reads the 30-byte local header and the name, then tail-calls strlen
// (the rest is lost). The signature check is kept; no entry is recorded here.
bool CZipReader::scanLocalHeader() {
    return false;
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

// 0x5775a4 / 0x5777ac / 0x5783c4: see the declaration in zip_reader.hpp.
bool operator<(const SZipFileEntry& a, const SZipFileEntry& b) { return a.name < b.name; }

}  // namespace glitch::io
