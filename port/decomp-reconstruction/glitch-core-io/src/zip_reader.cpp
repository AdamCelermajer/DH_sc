#include "glitch-core-io/io/zip_reader.hpp"

#include <algorithm>
#include <cstring>

#include "glitch-core-io/core/sort.hpp"
#include "glitch-core-io/io/limit_read_file.hpp"
#include "glitch-core-io/os/logging.hpp"

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

namespace {
// The directory is ordered with the shared heapsort (core/sort.hpp), as the original's
// glitch::core::heapsort<SZipFileEntry> is. With duplicate names the order, and so the index
// findFile returns, depends on the algorithm, so std::sort would not match.
void sortEntries(std::vector<SZipFileEntry>& entries) {
    const int count = static_cast<int>(entries.size());
    if (count > 1) glitch::core::heapsort(entries.data(), count);
}
}  // namespace

// 0x577fa4 _ZN6glitch2io10CZipReaderC1EPNS0_9IReadFileEbb (C2 at 0x578050 is the same body)
// Scans the local headers until scanLocalHeader reports the end, then sorts the entries by name.
CZipReader::CZipReader(IReadFile* file, bool a, bool b) : readFile(file), flagA(a), flagB(b) {
    if (readFile) {
        ++readFile->referenceCounter;
        while (scanLocalHeader()) {
        }
        sortEntries(entries);
    }
}

// 0x57826c _ZN6glitch2io10CZipReaderC1EPKcbb (C2 at 0x578318 is the same body)
// As the file form; the file is opened by name (createReadFile) and owned by the reader. A null
// file leaves the directory empty.
CZipReader::CZipReader(const char* fileName, bool a, bool b) : flagA(a), flagB(b) {
    readFile = createReadFile(fileName);
    if (readFile) {
        while (scanLocalHeader()) {
        }
        sortEntries(entries);
    }
}

// 0x577488 _ZN6glitch2io10CZipReaderD1Ev (D2 at 0x5774ec is the same body)
CZipReader::~CZipReader() {
    if (readFile) readFile->drop();
}

namespace {
std::uint16_t readU16(const std::uint8_t* p) {
    return static_cast<std::uint16_t>(p[0] | (p[1] << 8));
}
std::uint32_t readU32(const std::uint8_t* p) {
    return static_cast<std::uint32_t>(p[0]) | (static_cast<std::uint32_t>(p[1]) << 8) |
           (static_cast<std::uint32_t>(p[2]) << 16) | (static_cast<std::uint32_t>(p[3]) << 24);
}
}  // namespace

// 0x577bc8 _ZN6glitch2io10CZipReader15extractFilenameEPNS0_13SZipFileEntryE
// The raw name is entry.path (+0x00). The pseudocode tests '/' only. A '/' at index 0 is not a split
// (the whole name stays, as in the original's LABEL_23 path).
void CZipReader::extractFilename(SZipFileEntry& entry) {
    if (entry.path.empty()) return;
    if (flagA) {
        for (char& c : entry.path) {
            if (c >= 'A' && c <= 'Z') c = static_cast<char>(c + 32);
        }
    }
    const std::size_t sep = entry.path.find_last_of('/');
    if (sep == std::string::npos || sep == 0) {
        entry.name = entry.path;
        entry.extra.clear();
    } else {
        entry.name = entry.path.substr(sep + 1);
        entry.extra = entry.path.substr(0, sep + 1);
    }
    if (!flagB) entry.name = entry.path;
}

// 0x577db4 _ZN6glitch2io10CZipReader15scanLocalHeaderEv
// The header is the 30-byte local file header. The original reads the name into a 1024-byte stack
// buffer, so longer names overflow there; this port reads any length. The name is cut at its first
// NUL, as the original's strlen does.
bool CZipReader::scanLocalHeader() {
    if (!readFile) return false;
    std::uint8_t header[30] = {};
    readFile->read(header, 30);
    if (readU32(header) != 0x04034b50u) return false;

    SZipFileEntry entry;
    const std::uint16_t flags = readU16(header + 6);
    const std::uint16_t nameLength = readU16(header + 26);
    const std::uint16_t extraLength = readU16(header + 28);

    std::string raw(nameLength, '\0');
    if (nameLength) readFile->read(&raw[0], nameLength);
    raw.resize(std::strlen(raw.c_str()));
    entry.path = raw;
    extractFilename(entry);
    if (extraLength) readFile->seek(extraLength, true);
    if (flags & 0x0008) readFile->read(header + 14, 12);

    entry.f48 = static_cast<std::int32_t>(readFile->getPos());
    entry.f54 = readU16(header + 8);
    entry.f58 = static_cast<std::int32_t>(readU32(header + 18));
    entry.f5c = static_cast<std::int32_t>(readU32(header + 22));
    readFile->seek(entry.f58, true);
    entries.push_back(entry);
    return true;
}

// 0x576cf0 _ZN6glitch2io10CZipReader12getFileCountEv
int CZipReader::getFileCount() {
    return static_cast<int>(entries.size());
}

// 0x576d1c _ZNK6glitch2io10CZipReader11getFileInfoEi
const SZipFileEntry* CZipReader::getFileInfo(int index) const {
    return entries.data() + index;
}

// 0x576f18 _ZN6glitch2io10CZipReader8openFileEi
// PARTIAL: deflate entries (method 8) need zlib, which is not vendored in this tree, so they log and
// give null here; the original inflates them into a memory file (its fifth argument to
// createMemoryReadFile is never set in the pseudocode). Other methods log the name and give null.
IReadFile* CZipReader::openFile(int index) {
    const SZipFileEntry& entry = entries[static_cast<std::size_t>(index)];
    if (entry.f54 != 0) {
        if (entry.f54 == 8) {
            glitch::os::Printer::log("deflated zip entries are not supported by this port", glitch::os::ELL_ERROR);
            return nullptr;
        }
        glitch::os::Printer::log("file has unsupported compression method.", entry.name.c_str(),
                                 glitch::os::ELL_ERROR);
        return nullptr;
    }
    readFile->seek(entry.f48, false);
    if (unk_10 & 1) return new CLimitReadFile(readFile, entry.f5c, entry.name.c_str(), entry.name.c_str());
    return new CLimitReadFile(readFile, entry.f5c, entry.name.c_str());
}

// 0x578624 _ZN6glitch2io10CZipReader8openFileEPKc
IReadFile* CZipReader::openFile(const char* name) {
    const int index = findFile(name);
    if (index == -1) return nullptr;
    return openFile(index);
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
// PARTIAL: the original passes the opened file to isValid(IReadFile*) and drops it with no null
// test, so a missing file reads through a null pointer there. This port gives false instead.
bool CZipReader::isValid(const char* fileName) {
    IReadFile* file = createReadFile(fileName);
    if (!file) return false;
    const bool valid = isValid(file);
    file->drop();
    return valid;
}

// 0x577b50 _ZN6glitch2io10CZipReader22deletePathFromFilenameERSbIcSt11char_traitsIcENS_4core10SAllocatorIcLNS_6memory13E_MEMORY_HINTE0EEEE
// Scans back from the end; the first '/' or '\\' found decides the cut. A separator at index 0 is not
// a cut (the name is kept), and with none the name is kept too.
void CZipReader::deletePathFromFilename(std::string& fileName) {
    // Scan from the last character back; the first separator found decides the cut.
    for (std::size_t i = fileName.size(); i-- > 0;) {
        if (fileName[i] == '/' || fileName[i] == '\\') {
            if (i != 0) fileName = fileName.substr(i + 1);
            return;
        }
    }
}

// 0x5775a4 / 0x5777ac / 0x5783c4: the inlined comparator, as for SPakFileEntry (signed chars, then
// lengths). See the declaration in zip_reader.hpp.
namespace {
bool signedCharNameLess(const std::string& a, const std::string& b) {
    const std::size_t common = a.size() < b.size() ? a.size() : b.size();
    for (std::size_t i = 0; i < common; ++i) {
        const auto ca = static_cast<signed char>(a[i]);
        const auto cb = static_cast<signed char>(b[i]);
        if (ca != cb) return ca < cb;
    }
    return a.size() < b.size();
}
}  // namespace

bool operator<(const SZipFileEntry& a, const SZipFileEntry& b) { return signedCharNameLess(a.name, b.name); }

// 0x578520 _ZN6glitch2io10CZipReader8findFileEPKc
// The key's name (+0x18) is the argument: lower-cased with flagA, then cut to its base name with
// flagB, in that order as the pseudocode does. A null name is treated as empty (the original would
// call strlen on it). The search is glitch::core::binary_search (lower_bound, then an equality test).
int CZipReader::findFile(const char* name) {
    SZipFileEntry key;
    key.name = name != nullptr ? name : "";
    if (flagA) {
        for (char& c : key.name) {
            if (c >= 'A' && c <= 'Z') c = static_cast<char>(c + ' ');
        }
    }
    if (flagB) deletePathFromFilename(key.name);
    return glitch::core::binary_search(entries, key);
}

}  // namespace glitch::io
