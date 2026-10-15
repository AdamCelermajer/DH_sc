// glitch::core::detail::SSharedStringHeapEntry::SData::get (0x6a5074) and
// SSharedStringHeapEntry::SData::release (0x6a4d9c) over the table layout in
// shared_string_heap.hpp.
//
// The table is passed explicitly: the original reads the anonymous-namespace global
// SharedStringHeap (ELF 0x9f765c, allocated by initSharedStringHeap 0x6a5538). The port keeps one
// file-local table in src/shared_string_heap.cpp and passes it to these two functions, so
// reinterning a name and committing/releasing it always reach the same table.
#pragma once

#include "glitch-core-io/core/shared_string_heap.hpp"

namespace glitch::core::detail {

// 0x6a5074 _ZN6glitch4core6detail22SSharedStringHeapEntry5SData3getEPKcb
//
// Interns a text and returns its record. `noSideEffect` true is the original's `get(text, false)`:
// a pure lookup, which returns null when the text is not already interned. `noSideEffect` false is
// `get(text, true)`: get-or-create plus commit (0x6a4cb8), the form SSharedString::operator=
// (0x5daf94) uses. A null text and an empty text both return null.
//
// The returned record is owned by the table (the original frees it only when its use count reaches
// zero in release; see the note in the .cpp). Returns non-null only with a committed record whose
// text() is the interned text.
SSharedStringData* getSharedString(SSharedStringHeapTable& table, const char* text, bool noSideEffect);

// 0x6a4d9c _ZN6glitch4core6detail22SSharedStringHeapEntry5SData7releaseEPS3_
void releaseSharedString(SSharedStringHeapTable& table, SSharedStringData* data);

}  // namespace glitch::core::detail
