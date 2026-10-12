// FileSystemBase helpers. The glitch::io::CFileSystem base and the Savegame
// lookups belong to other categories and are not reconstructed here.
#pragma once

namespace glitch::io {
// CFileSystem::WorkingDirectory, the buffer strcpy'd by changeWorkingDirectoryTo.
extern char CFileSystem_WorkingDirectory[512];
}  // namespace glitch::io

class FileSystemBase {
public:
    // 0x34e0fc: XOR-style de-obfuscation of the first (4 - index) bytes. Only the
    // three visible arguments are recovered (see NOTES.md).
    static void decodeObfuscatedData(int index, void* data, int size);

    // 0x34e1d4: chdir, and on success remember the new working directory.
    bool changeWorkingDirectoryTo(const char* path);
};
