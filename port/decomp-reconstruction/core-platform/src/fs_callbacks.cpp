#include "core-platform/fs_callbacks.hpp"

namespace {
const FsFileHandle* AsHandle(const void* p) { return static_cast<const FsFileHandle*>(p); }
FsFileHandle* AsHandle(void* p) { return static_cast<FsFileHandle*>(p); }
}  // namespace

// 0x42b11c fs_tell_func(void const*)
int fs_tell_func(const void* handle) {
    if (handle == nullptr) {
        return -1;
    }
    return AsHandle(handle)->pos_;
}

// 0x42b0c0 fs_write_func(void const*, int, void*): writes nothing, reports the size.
int fs_write_func(const void*, int size, void*) { return size; }

// 0x42b0f8 fs_seek_to_end_func(void*)
int fs_seek_to_end_func(void* handle) {
    if (handle == nullptr) {
        return 4;
    }
    FsFileHandle* h = AsHandle(handle);
    if (h->size_ >= 0) {
        h->pos_ = h->size_;
    }
    return 0;
}

// 0x42b12c fs_get_eof_func(void*)
bool fs_get_eof_func(void* handle) {
    if (handle == nullptr) {
        return true;
    }
    FsFileHandle* h = AsHandle(handle);
    return h->size_ <= h->pos_;
}

// 0x42b0c8 fs_seek_func(int, void*)
int fs_seek_func(int pos, void* handle) {
    if (handle == nullptr) {
        return 4;
    }
    FsFileHandle* h = AsHandle(handle);
    if (pos >= 0 && pos <= h->size_) {
        h->pos_ = pos;
    }
    return 0;
}
