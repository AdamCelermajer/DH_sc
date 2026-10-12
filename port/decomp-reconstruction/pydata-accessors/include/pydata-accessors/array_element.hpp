#pragma once

// Element base of the Arrays::X tables (see NOTES.md, "Arrays element ABI"). An element is a
// polymorphic record whose vptr is at +0x0. Vtable slots: 0 = D1, 1 = D0, 2 = finalize (+0x8),
// 3 = read (+0xc). Each concrete element keeps its own size; the stride of a table is that size.
#include <cstdint>

#include "pydata-accessors/deps/stream_reader.hpp"

namespace pydata {

class ArrayElement {
public:
    virtual ~ArrayElement() = default;
    virtual void finalize() = 0;               // vtable +0x8
    virtual void read(IStreamBase* stream) = 0;  // vtable +0xc
};

}  // namespace pydata
