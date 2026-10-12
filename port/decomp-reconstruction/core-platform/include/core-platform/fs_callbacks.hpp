// gameswf file callbacks operating on a file handle with size and position.
#pragma once

// Handle layout inferred from the callbacks: size at +0x4, position at +0x8.
struct FsFileHandle {
    int reserved_;  // +0x0, not used by the callbacks
    int size_;      // +0x4
    int pos_;       // +0x8
};

int fs_tell_func(const void* handle);                 // 0x42b11c
int fs_write_func(const void* data, int size, void* handle);  // 0x42b0c0
int fs_seek_to_end_func(void* handle);                // 0x42b0f8
bool fs_get_eof_func(void* handle);                   // 0x42b12c
int fs_seek_func(int pos, void* handle);              // 0x42b0c8
