#include "vox-audio/deps/vox_file_system.hpp"

namespace {
vox::FileSystemInterface* g_file_system = nullptr;
}  // namespace

namespace vox {

// Stand-in: the real singleton is created by the file-system category. Tests
// install one with SetInstance.
FileSystemInterface* FileSystemInterface::GetInstance() { return g_file_system; }

void SetFileSystemInstanceForTests(FileSystemInterface* fs) { g_file_system = fs; }

}  // namespace vox
