#include "core-platform/fs_callbacks.hpp"

#include <cstdlib>

#include "core-platform/deps/application.hpp"

// 0x42b260 _Z12fs_open_funcPKcS0_
// Partial: the file is opened through the application's file system (vtable slot
// +0x88), its contents are read into a new 12-byte handle by BufferedStream, and
// the stream is then closed (vtable slot +0x78). The stream and BufferedStream
// types are declared in deps/application.hpp; their bodies belong to other
// categories. The mode argument is not used by the original.
void* fs_open_func(const char* name, const char*) {
    IStreamBase* stream = ApplicationOpenStream(name);
    if (stream == nullptr) {
        return nullptr;
    }
    FsFileHandle* handle = static_cast<FsFileHandle*>(std::malloc(sizeof(FsFileHandle)));
    handle->data_ = nullptr;
    handle->size_ = 0;
    handle->pos_ = 0;
    BufferedStreamFillBuffer(handle, stream);
    ApplicationCloseStream(stream);
    return handle;
}
