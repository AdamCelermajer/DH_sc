#include "glitch-core-io/io/file_list.hpp"

namespace glitch::io {

// 0x6b4188 _ZN6glitch2io9CFileList9FileEntryC1ERKS2_
CFileList::FileEntry::FileEntry(const FileEntry& other)
    : Name(other.Name), FullName(other.FullName), ID(other.ID), isDirectory(other.isDirectory) {}

// 0x6b404c _ZN6glitch2io9CFileList9FileEntryaSERKS2_
CFileList::FileEntry& CFileList::FileEntry::operator=(const FileEntry& other) {
    if (this != &other) {
        Name = other.Name;
        FullName = other.FullName;
    }
    ID = other.ID;
    isDirectory = other.isDirectory;
    return *this;
}

// 0x6b40a0 _ZN6glitch2io9CFileList15getFullFileNameEj (PARTIAL: see the header)
const char* CFileList::getFullFileName(std::uint32_t index) {
    if (index >= Files.size()) return nullptr;
    FileEntry& entry = Files[index];
    entry.FullName = Path;
    if (Path.size() > 3) entry.FullName += "/";
    entry.FullName += entry.Name;
    return entry.FullName.c_str();
}

// 0x6b3e78 _ZNK6glitch2io9CFileList12getFileCountEv
int CFileList::getFileCount() const {
    return static_cast<int>(Files.size());
}

// 0x6b3ea4 _ZNK6glitch2io9CFileList11getFileNameEj
const char* CFileList::getFileName(std::uint32_t index) const {
    if (index >= Files.size()) return nullptr;
    return Files[index].Name.c_str();
}

// 0x6b3ee4 _ZNK6glitch2io9CFileList11isDirectoryEj
bool CFileList::isDirectory(std::uint32_t index) const {
    if (index >= Files.size()) return false;
    return Files[index].isDirectory;
}

}  // namespace glitch::io
