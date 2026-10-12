#include "net-transport/buffer.hpp"

#include <algorithm>
#include <cstring>

// 0x316988 _ZN6BufferD1Ev
Buffer::~Buffer() {
    clear();
}

// 0x316568 _ZNK6Buffer4readEjPvj
void Buffer::read(std::uint32_t offset, void* out, std::uint32_t length) const {
    std::uint32_t index = offset / block_size_;
    std::uint32_t in_block = offset % block_size_;
    std::uint32_t count = std::min(length, block_size_ - in_block);
    if (length == 0) {
        return;
    }
    // Original asserts on a null block (debug ASSERT, not reproduced) and then
    // memcpy()s from it; here the copy is skipped instead of dereferencing null.
    const std::uint8_t* block = blocks_[index];
    if (block == nullptr) {
        return;
    }
    std::memcpy(out, block + in_block, count);
}

// 0x316f0c _ZN6Buffer5writeEjPKvj
void Buffer::write(std::uint32_t offset, const void* data, std::uint32_t length) {
    std::uint32_t index = offset / block_size_;
    std::uint32_t in_block = offset % block_size_;
    ensureBuffer(offset, length);
    if (size_ < length + offset) {
        size_ = length + offset;
    }
    std::uint32_t count = std::min(length, block_size_ - in_block);
    if (length == 0) {
        return;
    }
    std::memcpy(blocks_[index] + in_block, data, count);
}

// 0x31692c _ZN6Buffer5clearEv
void Buffer::clear() {
    for (std::uint8_t* block : blocks_) {
        if (block != nullptr) {
            delete[] block;
        }
    }
    blocks_.clear();
    sized_ = false;
    block_size_ = 0x800;
    size_ = 0;
}

// 0x317014 _ZN6Buffer7setSizeEj
void Buffer::setSize(std::uint32_t size) {
    // Original asserts the buffer is empty first (debug ASSERT, not reproduced).
    blocks_.reserve(2);
    std::uint8_t* block = new std::uint8_t[size];
    blocks_.push_back(block);
    blocks_.push_back(nullptr);
    sized_ = true;
    block_size_ = size;
    size_ = size;
}

// 0x316d98 _ZN6Buffer12ensureBufferEjj
void Buffer::ensureBuffer(std::uint32_t offset, std::uint32_t length) {
    std::uint32_t needed = (offset + length) / block_size_ + 2;  // blocks + sentinel
    if (blocks_.size() >= needed) {
        return;
    }
    if (blocks_.capacity() < needed) {
        blocks_.reserve(needed);
    }
    // Drop the trailing sentinel before appending real blocks.
    if (!blocks_.empty()) {
        blocks_.pop_back();
    }
    while (blocks_.size() < needed - 1) {
        blocks_.push_back(new std::uint8_t[block_size_]);
    }
    blocks_.push_back(nullptr);
}
