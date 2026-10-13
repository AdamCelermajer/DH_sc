// gameswf DH2 hash<> sizing: explicit set_raw_capacity specializations for the instantiations of batch
// v1-vendor-gameswf-pack5-64b2da-0. Declarations, the ln 2 constant and the table-size accessor. Bodies:
// vendor-gameswf/src/v1-vendor-gameswf-pack5-64b2da-0.cpp.
//
// The binary's hash<>::set_raw_capacity is the upstream algorithm (base/container.h, the generic member) with
// three changes: the table minimum is 4 entries (upstream 16), the power of two is taken from single-precision
// logf / ln 2 (the float 0x3F317218, dh2_hash_access::kLn2), and the table storage is malloc_internal / free_internal.
// Each instantiation is its own binary
// function, so each one gets an explicit specialization here, and all of them run the same shared body
// (dh2_hash_access::set_raw_capacity). The generic upstream member still serves every other instantiation.
//
// These declarations must be seen by every translation unit that adds to one of these tables, or that TU
// instantiates the upstream generic member instead. Declaring a specialization needs only the names of the
// value types. The definitions that release a value of incomplete type are behind VENDOR_GAMESWF_DH2_CORE_HEADERS.
//
// Not declared here: hash<stringi_pointer, int, string_pointer_hash_functor<stringi_pointer> >::set_raw_capacity
// (0x767824). stringi_pointer is not declared anywhere in the tree.
#pragma once

#include "base/container.h"                   // hash<>, fixed_size_hash<>, tu_stringi, stringi_hash_functor (global)
#include "base/smart_ptr.h"                   // smart_ptr<T> (global in upstream)
#include "gameswf/gameswf.h"                  // gameswf::sound_sample, font, character_def, as_object (forward)
#include "vendor-gameswf/as_value.hpp"        // gameswf::as_value (DH2 layout)

namespace gameswf {

struct bitmap_character_def;  // gameswf_impl.h (upstream); forward only

}  // namespace gameswf

// DH2: the shared rehash body. A friend of hash<> (base/container.h, marked DH2 there), so it may use the table.
struct dh2_hash_access {
	// DH2: ln 2 as the binary's single-precision constant, bit pattern 0x3F317218 (the binary loads it with
	// MOV R1,#0x3F317218 before __aeabi_fdiv). Hex-Rays prints it rounded as 0.69315. The decimal 0.69315f is 0x3F317247,
	// a different float: it gives half the table for every request 2^k + 1 (5 gives 4 instead of 8, 33 gives 32 instead of 64).
	static constexpr float kLn2 = 0.693147182464599609375f;

	template <class H>
	static void set_raw_capacity(H& self, int new_size);

	// DH2: slot count of the table (0 when there is no table). The table is private to hash<>; the tests read it here.
	template <class H>
	static int table_size(const H& self) {
		return self.m_table == nullptr ? 0 : self.m_table->m_size_mask + 1;
	}
};

// 0x7665dc gameswf::hash<int, gameswf::smart_ptr<gameswf::sound_sample>, gameswf::fixed_size_hash<int> >::set_raw_capacity(int)
template <>
void hash<int, smart_ptr<gameswf::sound_sample>, fixed_size_hash<int> >::set_raw_capacity(int new_size);

// 0x766960 gameswf::hash<int, gameswf::smart_ptr<gameswf::bitmap_character_def>, gameswf::fixed_size_hash<int> >::set_raw_capacity(int)
template <>
void hash<int, smart_ptr<gameswf::bitmap_character_def>, fixed_size_hash<int> >::set_raw_capacity(int new_size);

// 0x766d10 gameswf::hash<int, gameswf::smart_ptr<gameswf::font>, gameswf::fixed_size_hash<int> >::set_raw_capacity(int)
template <>
void hash<int, smart_ptr<gameswf::font>, fixed_size_hash<int> >::set_raw_capacity(int new_size);

// 0x767094 gameswf::hash<int, gameswf::smart_ptr<gameswf::character_def>, gameswf::fixed_size_hash<int> >::set_raw_capacity(int)
template <>
void hash<int, smart_ptr<gameswf::character_def>, fixed_size_hash<int> >::set_raw_capacity(int new_size);

// 0x767e40 gameswf::hash<int, gameswf::tu_string, gameswf::fixed_size_hash<int> >::set_raw_capacity(int)
template <>
void hash<int, tu_string, fixed_size_hash<int> >::set_raw_capacity(int new_size);

// 0x768254 gameswf::hash<gameswf::tu_stringi, bool, gameswf::stringi_hash_functor<gameswf::tu_stringi> >::set_raw_capacity(int)
template <>
void hash<tu_stringi, bool, stringi_hash_functor<tu_stringi> >::set_raw_capacity(int new_size);

// 0x7687ac gameswf::hash<gameswf::tu_stringi, gameswf::smart_ptr<gameswf::character_def>, gameswf::stringi_hash_functor<gameswf::tu_stringi> >::set_raw_capacity(int)
template <>
void hash<tu_stringi, smart_ptr<gameswf::character_def>, stringi_hash_functor<tu_stringi> >::set_raw_capacity(int new_size);

// 0x769610 gameswf::hash<gameswf::as_object*, bool, gameswf::fixed_size_hash<gameswf::as_object*> >::set_raw_capacity(int)
template <>
void hash<gameswf::as_object*, bool, fixed_size_hash<gameswf::as_object*> >::set_raw_capacity(int new_size);

// 0x76a2f8 gameswf::hash<gameswf::tu_stringi, gameswf::as_value, gameswf::stringi_hash_functor<gameswf::tu_stringi> >::set_raw_capacity(int)
template <>
void hash<tu_stringi, gameswf::as_value, stringi_hash_functor<tu_stringi> >::set_raw_capacity(int new_size);

// DH2 (batch v1-vendor-gameswf-pack14-1a21ee-0): the RenderFX search index table. The value is a pointer and the key is
// a tu_string, so the body needs only the forward declaration of the entry type. Body: vendor-gameswf/src/v1-vendor-gameswf-pack14-1a21ee-0.cpp.
namespace RenderFX {
namespace SearchIndex {
struct Entry;
}  // namespace SearchIndex
}  // namespace RenderFX

// 0x7aa8f0 gameswf::hash<gameswf::tu_string, gameswf::array<RenderFX::SearchIndex::Entry>*, gameswf::string_hash_functor<gameswf::tu_string> >::set_raw_capacity(int)
template <>
void hash<tu_string, array<RenderFX::SearchIndex::Entry>*, string_hash_functor<tu_string> >::set_raw_capacity(int new_size);

#if defined(VENDOR_GAMESWF_DH2_CORE_HEADERS)
// 0x76a6ec gameswf::hash<gameswf::tu_stringi, gameswf::as_object::as_watch, gameswf::stringi_hash_functor<gameswf::tu_stringi> >::set_raw_capacity(int)
// Needs gameswf::as_object complete (as_object::as_watch is nested in it). gameswf/gameswf_object.h does not compile in the tree.
#include "gameswf/gameswf_object.h"
template <>
void hash<tu_stringi, gameswf::as_object::as_watch, stringi_hash_functor<tu_stringi> >::set_raw_capacity(int new_size);
#endif
