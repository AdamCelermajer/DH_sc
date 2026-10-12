// Stand-in for IStreamBase, the stream interface that StreamBuffer implements
// and BufferedStream reads from. Owned by another category; declared here so
// the net-transport code compiles on its own. Slot numbers are the byte offsets
// of the virtual calls in the original vtable.
#pragma once

#include <cstdint>

class IStreamBase {
public:
    virtual ~IStreamBase() = default;
    virtual std::uint32_t size() const = 0;                              // +0x08
    virtual bool canRead() const = 0;                                    // +0x0c
    virtual bool canWrite() const = 0;                                   // +0x10
    virtual std::uint64_t peek(void* dst, std::uint64_t n) const = 0;    // +0x14
    virtual std::uint64_t read(void* dst, std::uint64_t n) = 0;          // +0x18
    virtual std::uint64_t write(const void* src, std::uint64_t n) = 0;   // +0x1c
    virtual void seek(std::uint64_t pos) = 0;                            // +0x20
    virtual std::uint64_t tell() const = 0;                              // +0x24
    virtual void skip(std::uint64_t n) = 0;                              // +0x28
    virtual void seekWrite(std::uint64_t pos) = 0;                       // +0x2c
    virtual std::uint64_t tellWrite() const = 0;                         // +0x30
    virtual std::uint32_t size(int block) const = 0;                     // +0x34
};
