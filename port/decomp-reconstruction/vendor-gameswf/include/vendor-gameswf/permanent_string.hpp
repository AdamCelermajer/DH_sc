// gameswf::permanent_string (Gameloft, not in gameswf r1714).
//
// Only the layout that the destructor reads is recovered: a flag byte at +0x00 and a buffer pointer
// at +0x0c. When the flag is 0xff the buffer is heap memory owned by the object, and ~permanent_string
// frees it. The other fields are not identified and are not declared here. The constructors, the
// string accessors and permanent_string_cache are not in batch v1-vendor-gameswf-pack0-157e56-0.
//
// Body: vendor-gameswf/src/v1-vendor-gameswf-pack0-157e56-0.cpp.

#pragma once

#include <cstdint>

namespace gameswf {

class permanent_string {
public:
	// 0x752e64 gameswf::permanent_string::~permanent_string() (D1, no vtable).
	~permanent_string();

	int8_t unk_0x00 = 0;        // +0x00: flag byte; -1 (0xff) = the buffer at +0x0c is owned heap memory
	uint8_t pad_0x01[0x0b];     // +0x01..+0x0b: not identified
	void* unk_0x0c = nullptr;   // +0x0c: buffer pointer (host width; the 32-bit offset is kept as a comment)
};

}  // namespace gameswf
