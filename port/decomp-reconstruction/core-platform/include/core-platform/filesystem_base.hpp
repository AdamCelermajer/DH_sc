// FileSystemBase helpers. The glitch::io::CFileSystem base and the Savegame
// lookups belong to other categories and are not reconstructed here.
#pragma once

#include <string>

namespace glitch::io {
// CFileSystem::WorkingDirectory, the buffer strcpy'd by changeWorkingDirectoryTo.
extern char CFileSystem_WorkingDirectory[512];
}  // namespace glitch::io

class FileSystemBase {
public:
    // 0x34e0fc: adds a running key to the first (4 - index) bytes. The fourth
    // parameter (mangled as void*) is not used by the body.
    static void decodeObfuscatedData(int index, void* data, int size, void* unused);

    // 0x34e1d4: chdir, and on success remember the new working directory.
    bool changeWorkingDirectoryTo(const char* path);

    // 0x34e1ac: returns a copy of the path. The Win32 file system overrides the
    // separator handling in FileSystemWin32::formatFilePath.
    std::string formatFilePath(const std::string& path) const;

    // 0x34df94: stdio existence check. Partial: the original calls a virtual
    // existence test of the derived file system, which is not in this tree.
    bool doesFileExists(const char* name) const;

    // 0x34dfe0: partial, same check as doesFileExists (the resource lookup is in the
    // derived class).
    bool doesResourceExists(const char* name) const;

    // 0x34e150: partial, same check after the savegame job flush (not reconstructed).
    bool doesSavefileExists(const char* name) const;

    // 0x34e96c: partial. The name substitutions of the original are not visible in
    // the pseudocode; the name is returned unchanged.
    std::string ApplyFilenameHacks(const char* name) const;

    // 0x34e5f4: partial. Returns the path unchanged; the derived normalisation and
    // glitch::io::CFileSystem::getAbsolutePath are not reconstructed.
    std::string getAbsolutePath(const std::string& path) const;

    // 0x34e688: partial. Builds the path (RES_PATH prefix unless the name has a
    // ".config" or ".glsl" part or starts with "/sdcard/"), then creates the file.
    // The original delegates creation to glitch::io::CFileSystem.
    void createAndOpenFile(const char* name);
};
