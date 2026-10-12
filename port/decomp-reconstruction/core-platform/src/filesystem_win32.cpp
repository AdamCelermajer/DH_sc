#include "core-platform/filesystem_win32.hpp"

// 0x34edf0 FileSystemWin32::_FileHandle::tell() const
long long FileSystemWin32::_FileHandle::tell() const {
    return static_cast<long long>(std::ftell(file_));
}

// 0x34ee10 FileSystemWin32::_FileHandle::seek(unsigned long long)
// The original seeks from the start (SEEK_SET) with the value truncated to long.
void FileSystemWin32::_FileHandle::seek(unsigned long long pos) {
    std::fseek(file_, static_cast<long>(pos), SEEK_SET);
}

// 0x34ee20 FileSystemWin32::_FileHandle::size() const
// Saves the position, seeks to the end, reads the position, and restores it.
long long FileSystemWin32::_FileHandle::size() const {
    long saved = std::ftell(file_);
    std::fseek(file_, 0, SEEK_END);
    long end = std::ftell(file_);
    std::fseek(file_, saved, SEEK_SET);
    return static_cast<long long>(end);
}

// 0x34eba4 FileSystemWin32::getRootPath() const
// Partial: the original returns the address of the path string object, here the
// path buffer at +0x2c is returned instead.
const char* FileSystemWin32::getRootPath() const { return rootPath_; }

// 0x34ebac FileSystemWin32::getResourcesPath() const
// Partial, same caveat as getRootPath().
const char* FileSystemWin32::getResourcesPath() const { return resourcesPath_; }

// 0x34ebb4 FileSystemWin32::getSavefilesPath() const
// Partial, same caveat as getRootPath().
const char* FileSystemWin32::getSavefilesPath() const { return savefilesPath_; }

// 0x34ebc0 FileSystemWin32::copySavefile(char const*): empty in the original.
void FileSystemWin32::copySavefile(const char*) {}

// 0x34ebc4 FileSystemWin32::_getFileInfo(char const*, char const*, void*) const
bool FileSystemWin32::_getFileInfo(const char*, const char*, void*) const { return true; }

// 0x34ec00 FileSystemWin32::isFileNewerThan(char const*, char const*) const: empty in the original.
bool FileSystemWin32::isFileNewerThan(const char*, const char*) const { return false; }
