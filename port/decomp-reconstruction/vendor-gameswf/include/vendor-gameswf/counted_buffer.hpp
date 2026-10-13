// gameswf::counted_buffer as the DH2 binary has it. Replaces the upstream stub in
// vendor-gameswf/upstream/gameswf-r1714/gameswf/gameswf_action.h (wrapped in #if 0 there). The upstream base
// gc_object is not in the binary's layout: the reference count is a plain int at +0x10, and the class has no
// vtable (neither the destructor nor drop_ref stores one).
//
// Layout (32-bit offsets; host widths apply to pointers):
//   +0x00..+0x0f  ::membuf (base/membuf.h, 16 bytes in the binary, no vptr)
//   +0x10         m_ref_count     drop_ref() decrements it and destroys the object at zero
//   +0x14         unk_14          pointer to 4-byte words; freed with free_internal(p, 4 * unk_1c) by the destructor
//                                 when unk_20 is zero, then cleared
//   +0x18         unk_18          signed. The destructor zeroes unk_14[unk_18 .. -1] when it is negative, then sets it to 0
//   +0x1c         unk_1c          word count of unk_14 (the free size is 4 * unk_1c); cleared by the owned path
//   +0x20         unk_20          byte. Zero means the destructor owns unk_14
// Object size beyond +0x20 is not recovered.
//
// The fields are public so that the layout can be checked by tests. unk_* names follow the TRACK-B convention
// for fields whose meaning is not recovered.
//
// Bodies: vendor-gameswf/src/v1-vendor-gameswf-pack2-6d7f71-0.cpp (0x75b974 D1, 0x75b9f4 drop_ref).

#pragma once

#include <cstdint>

#include "base/membuf.h"  // ::membuf

namespace gameswf {

class counted_buffer : public ::membuf {
public:
	~counted_buffer();  // 0x75b974 (D1). Non-virtual: the binary has no vtable for this class.
	void drop_ref();    // 0x75b9f4

	int m_ref_count;        // +0x10
	std::uint32_t* unk_14;  // +0x14
	int unk_18;             // +0x18
	int unk_1c;             // +0x1c
	std::uint8_t unk_20;    // +0x20
};

}  // namespace gameswf
