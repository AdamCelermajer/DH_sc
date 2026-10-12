// glitch::core::detail::SSharedStringHeapEntry::SData::release (0x6a4d9c) over the table layout in
// shared_string_heap.hpp. The table is passed explicitly; the original reads the anonymous-namespace global.
#pragma once

#include "glitch-core-io/core/shared_string_heap.hpp"

namespace glitch::core::detail {

// 0x6a4d9c _ZN6glitch4core6detail22SSharedStringHeapEntry5SData7releaseEPS3_
void releaseSharedString(SSharedStringHeapTable& table, SSharedStringData* data);

}  // namespace glitch::core::detail
