// Stream cursors and buffers (vox::StreamInterface family).
#pragma once

#include <cstddef>
#include <cstdint>
#include <string>

#include "vox-audio/deps/vox_file_system.hpp"

namespace vox {

// Abstract read cursor over a stream. Vtable slots (original ABI):
//   +0x08 Init, +0x0c Shutdown, +0x10 Seek, +0x14 IsSeekable,
//   +0x18 Tell, +0x1c Read, +0x20 ReadRef, +0x24 EndOfStream, +0x28 AllowBufferReference.
class StreamCursorInterface {
public:
    virtual ~StreamCursorInterface() = default;
    virtual void Init() {}
    virtual void Shutdown() {}
    virtual int Seek(int offset, int whence) = 0;
    virtual bool EndOfStream() = 0;
    virtual int Tell() = 0;
    virtual int Read(std::uint8_t* dst, int size) = 0;
    virtual int ReadRef(std::uint8_t*& ref, int size) {
        (void)ref;
        (void)size;
        return 0;
    }
    virtual bool IsSeekable() const = 0;
    virtual bool AllowBufferReference() const { return false; }
};

// Stream source (vox::StreamInterface). Vtable slot +0x10 is Size().
class StreamInterface {
public:
    virtual ~StreamInterface() = default;
    virtual int GetType() const = 0;
    virtual int Size() const = 0;
    virtual StreamCursorInterface* CreateNewCursor() = 0;
    virtual void DestroyCursor(StreamCursorInterface* cursor) = 0;
};

class StreamMemoryBuffer;

// Parameter block for StreamMemoryBuffer (original: void* data, int size,
// byte explicit-owns flag at +0x8, byte owns at +0x9).
struct StreamMemoryBufferParams {
    const std::uint8_t* data = nullptr;  // +0x00
    int size = 0;                        // +0x04
    bool explicit_owns = false;          // +0x08
    bool owns = false;                   // +0x09
};

// Cursor over a StreamMemoryBuffer. Layout: +0x00 vtable, +0x04 owner, +0x08 position.
class StreamMemoryBufferCursor final : public StreamCursorInterface {
public:
    explicit StreamMemoryBufferCursor(StreamMemoryBuffer* owner) : owner_(owner) {}
    ~StreamMemoryBufferCursor() override = default;

    int Seek(int offset, int whence) override;
    bool EndOfStream() override;
    int Tell() override { return position_; }
    int Read(std::uint8_t* dst, int size) override;
    int ReadRef(std::uint8_t*& ref, int size) override;
    bool IsSeekable() const override { return true; }
    bool AllowBufferReference() const override { return true; }

private:
    StreamMemoryBuffer* owner_ = nullptr;  // +0x04
    int position_ = 0;                     // +0x08
};

// In-memory stream. Layout: +0x00 vtable, +0x04 size, +0x08 data, +0x0c owns.
class StreamMemoryBuffer final : public StreamInterface {
public:
    explicit StreamMemoryBuffer(const StreamMemoryBufferParams* params);
    ~StreamMemoryBuffer() override;

    int GetType() const override;
    int Size() const override { return size_; }
    StreamCursorInterface* CreateNewCursor() override;
    void DestroyCursor(StreamCursorInterface* cursor) override;

    const std::uint8_t* Data() const { return data_; }

private:
    int size_ = 0;                        // +0x04
    std::uint8_t* data_ = nullptr;        // +0x08
    bool owns_ = true;                    // +0x0c
};

// File-backed stream (vox::StreamCFile). Layout: +0x00 vtable, +0x04 size,
// +0x18 path string (original vox string), +0x20 file system.
class StreamCFile final : public StreamInterface {
public:
    explicit StreamCFile(const char* path);
    ~StreamCFile() override;

    int GetType() const override;
    int Size() const override { return size_; }
    StreamCursorInterface* CreateNewCursor() override;
    void DestroyCursor(StreamCursorInterface* cursor) override;

    void Init();

private:
    friend class StreamCFileCursor;
    int size_ = 0;                         // +0x04
    std::string path_;                     // +0x18
    FileSystemInterface* fs_ = nullptr;    // +0x20
};

// Cursor over a StreamCFile with a 32 KiB read-ahead buffer.
class StreamCFileCursor final : public StreamCursorInterface {
public:
    static constexpr int kBufferSize = 0x8000;

    explicit StreamCFileCursor(StreamCFile* owner);
    ~StreamCFileCursor() override;

    void Init() override;
    void Shutdown() override;
    int Seek(int offset, int whence) override;
    bool EndOfStream() override;
    int Tell() override;
    int Read(std::uint8_t* dst, int size) override;
    bool IsSeekable() const override { return true; }

private:
    StreamCFile* owner_ = nullptr;           // +0x04
    FileInterface* file_ = nullptr;          // +0x08
    int position_ = -1;                      // +0x0c
    std::uint8_t buffer_[kBufferSize] = {};  // +0x10
    int buffer_fill_ = 0;                    // +0x8010
    int buffer_index_ = 0;                   // +0x8014
    int buffer_base_ = 0;                    // +0x8018
};

void* StreamMemoryBufferFactory(void* hint);
void* StreamCFileFactory(void* hint);

}  // namespace vox

// stdio-style helpers over a StreamCursorInterface (global in the original).
int fgetc(vox::StreamCursorInterface* cursor);
std::size_t fread(void* dst, std::size_t size, std::size_t count, vox::StreamCursorInterface* cursor);
int ftell(vox::StreamCursorInterface* cursor);
int fseek(vox::StreamCursorInterface* cursor, int offset, int whence);
int fclose(vox::StreamCursorInterface* cursor);
