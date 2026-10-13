// Batch v1-vendor-gameswf-pack5-64b2da-0 (vendor-gameswf, owner gameswf::movie_definition and the gameswf::hash<> instantiations).
//
// Reconstructed (4, compiled here): set_raw_capacity of hash<int, tu_string>, hash<tu_stringi, bool>,
//   hash<as_object*, bool> and hash<tu_stringi, as_value>.
// PARTIAL, written behind VENDOR_GAMESWF_DH2_CORE_HEADERS and not compiled in the tree (6): set_raw_capacity
//   of hash<int, smart_ptr<sound_sample / bitmap_character_def / font / character_def>>, of
//   hash<tu_stringi, smart_ptr<character_def>> and of hash<tu_stringi, as_object::as_watch>. Releasing a
//   smart_ptr<T> or an as_watch needs T complete, and those headers do not compile in the tree yet.
// PARTIAL, not written (1): set_raw_capacity of hash<stringi_pointer, int, string_pointer_hash_functor<stringi_pointer>>
//   (0x767824). stringi_pointer is not declared in the tree (see vendor-gameswf/src/v1-vendor-gameswf-sprite_definition-9a9a59-0.cpp).
// Upstream, no body here (8):
//   movie_definition D1 (0x765164) and D0 (0x765398): compiler-generated. gameswf::movie_definition has no data members
//   and derives from character_def, whose destructor is virtual. The binary stores the movie_definition vtable,
//   calls character_def's destructor, and D0 frees the object.
//   hash<...>::entry constructors and copy constructors (0x767418, 0x767454, 0x76a010, 0x76a050, 0x76a098, 0x76a0e0):
//   base/container.h, the inline entry(key, value, next, hash) and entry(entry const&).
//
// Shared rehash body: dh2_hash_access::set_raw_capacity. The binary's algorithm is the upstream generic member
// (base/container.h) with these changes, shared by all eleven instantiations:
//   - table minimum 4 entries (upstream 16);
//   - power of two = 1 << (int)(float)(logf(n - 1) / ln 2 + 1.0), in single precision (n == 1 gives 4). ln 2 is the
//     float 0x3F317218 (dh2_hash_access::kLn2), not the decimal 0.69315f, which is 0x3F317247 and gives half the table
//     for every request 2^k + 1 (5 gives 4 instead of 8);
//   - table storage from malloc_internal(sizeof(table) + sizeof(entry) * n) (16 * n + 8 bytes on ARM; the entry
//     size is the host's, so the table is host-width, see TRACK-B decision 2), next_in_chain = -2 on every entry;
//   - each live entry is add()ed to the new table, then its key and value are destroyed (entry::clear()) and the
//     slot is marked empty; the old table is freed with free_internal.
// Key and value destruction is the upstream ~T and ~U. The binary frees a tu_string key's heap text with
// free_internal and calls drop_refs on an as_value value; upstream's destructors reach the same storage through
// tu_free (base/container.h) and as_value::~as_value, which calls drop_refs.
//
// The binary passes one argument to malloc_internal and free_internal in the decompile. The MemHint is ignored by the
// binary, and free_internal ignores its size argument, so 0 is passed.

#include <cmath>
#include <cstddef>

#include "base/container.h"
#include "base/smart_ptr.h"
#include "vendor-gameswf/gameswf_alloc.hpp"  // gameswf::malloc_internal, gameswf::free_internal, swf::MEM_UNRECOVERED
#include "vendor-gameswf/gameswf_dh2_hash.hpp"

#if defined(VENDOR_GAMESWF_DH2_CORE_HEADERS)
#include "gameswf/gameswf_character.h"  // gameswf::character_def
#include "gameswf/gameswf_font.h"       // gameswf::font
#include "gameswf/gameswf_impl.h"       // gameswf::bitmap_character_def
#include "gameswf/gameswf_object.h"     // gameswf::as_object::as_watch
#include "gameswf/gameswf_sound.h"      // gameswf::sound_sample
#endif

// The shared body. H is one of the hash<> instantiations above. Reaches the table's private members through the
// friend declared in base/container.h.
template <class H>
void dh2_hash_access::set_raw_capacity(H& self, int new_size) {
	using entry = typename H::entry;
	using table = typename H::table;

	if (new_size <= 0) {
		// Binary: clear() and return.
		self.clear();
		return;
	}

	// Binary: v3 = 1 << (int)(float)((float)(logf((float)(a2 - 1)) / ln2) + 1.0); then v3 = max(v3, 4). ln2 is the
	// single-precision constant 0x3F317218 (MOV R1,#0x3F317218 before __aeabi_fdiv), which is kLn2 below.
	// new_size == 1 gives logf(0) = -inf in the binary. The ARM shift then uses the low byte of the converted value,
	// which is 0, so the result is 1 and the minimum makes it 4. The explicit guard gives the same result.
	int size = 4;
	if (new_size > 1) {
		const float q = std::logf(static_cast<float>(new_size - 1)) / kLn2;
		const int bits = static_cast<int>(q + 1.0f);
		const unsigned int pow2 = (bits >= 0 && bits < 32) ? (1u << bits) : 0u;
		size = static_cast<int>(pow2);
		if (size < 4) {
			size = 4;  // DH2 minimum (upstream: 16)
		}
	}

	// Binary: if the table exists and already has this many slots, there is nothing to do.
	if (self.m_table != nullptr && size == self.m_table->m_size_mask + 1) {
		return;
	}

	// Binary: malloc_internal(16 * size + 8): entry count 0, size mask size - 1, and next_in_chain = -2 on every entry.
	H new_hash;
	new_hash.m_table = static_cast<table*>(gameswf::malloc_internal(
		static_cast<unsigned int>(sizeof(table) + sizeof(entry) * static_cast<std::size_t>(size)), swf::MEM_UNRECOVERED));
	new_hash.m_table->m_entry_count = 0;
	new_hash.m_table->m_size_mask = size - 1;
	for (int i = 0; i < size; i++) {
		new_hash.E(i).m_next_in_chain = -2;
	}

	if (self.m_table != nullptr) {
		// Binary: in index order, each live entry (not empty, not tombstone) goes into the new table. Its key and
		// value are then destroyed and the slot is marked empty.
		for (int i = 0, n = self.m_table->m_size_mask; i <= n; i++) {
			entry* e = &self.E(i);
			if (!e->is_empty() && !e->is_tombstone()) {
				new_hash.add(e->first, e->second);
				e->clear();
			}
		}
		// Binary: free_internal(old table).
		gameswf::free_internal(self.m_table, 0);
	}

	// Binary: the new table is stolen. The temporary is left empty, so its destructor does nothing.
	self.m_table = new_hash.m_table;
	new_hash.m_table = nullptr;
}

// 0x767e40 gameswf::hash<int, gameswf::tu_string, gameswf::fixed_size_hash<int> >::set_raw_capacity(int)
// Binary: entry 32 bytes (key int at +8, value tu_string at +12). The value's destructor frees heap text when its
// tag byte at +12 is -1. That is tu_string::~tu_string here.
template <>
void hash<int, tu_string, fixed_size_hash<int> >::set_raw_capacity(int new_size) {
	dh2_hash_access::set_raw_capacity(*this, new_size);
}

// 0x768254 gameswf::hash<gameswf::tu_stringi, bool, gameswf::stringi_hash_functor<gameswf::tu_stringi> >::set_raw_capacity(int)
// Binary: entry 32 bytes (key tu_stringi at +8, value bool at +28). The key's heap text is freed; the bool has no destructor.
template <>
void hash<tu_stringi, bool, stringi_hash_functor<tu_stringi> >::set_raw_capacity(int new_size) {
	dh2_hash_access::set_raw_capacity(*this, new_size);
}

// 0x769610 gameswf::hash<gameswf::as_object*, bool, gameswf::fixed_size_hash<gameswf::as_object*> >::set_raw_capacity(int)
// Binary: entry 16 bytes (key pointer at +8, value bool at +12). Neither has a destructor; nothing is released per entry.
template <>
void hash<gameswf::as_object*, bool, fixed_size_hash<gameswf::as_object*> >::set_raw_capacity(int new_size) {
	dh2_hash_access::set_raw_capacity(*this, new_size);
}

// 0x76a2f8 gameswf::hash<gameswf::tu_stringi, gameswf::as_value, gameswf::stringi_hash_functor<gameswf::tu_stringi> >::set_raw_capacity(int)
// Binary: entry 40 bytes (key tu_stringi at +8, as_value at +28). The value is released with as_value::drop_refs, which is
// as_value's destructor (vendor-gameswf/include/vendor-gameswf/as_value.hpp).
template <>
void hash<tu_stringi, gameswf::as_value, stringi_hash_functor<tu_stringi> >::set_raw_capacity(int new_size) {
	dh2_hash_access::set_raw_capacity(*this, new_size);
}

#if defined(VENDOR_GAMESWF_DH2_CORE_HEADERS)
// The six bodies below release a smart_ptr<T> (drop_ref on T) or an as_watch, so T and as_object must be complete.
// Not compiled in the tree: see the header comment at the top of this file.

// 0x7665dc gameswf::hash<int, gameswf::smart_ptr<gameswf::sound_sample>, gameswf::fixed_size_hash<int> >::set_raw_capacity(int)
template <>
void hash<int, smart_ptr<gameswf::sound_sample>, fixed_size_hash<int> >::set_raw_capacity(int new_size) {
	dh2_hash_access::set_raw_capacity(*this, new_size);
}

// 0x766960 gameswf::hash<int, gameswf::smart_ptr<gameswf::bitmap_character_def>, gameswf::fixed_size_hash<int> >::set_raw_capacity(int)
template <>
void hash<int, smart_ptr<gameswf::bitmap_character_def>, fixed_size_hash<int> >::set_raw_capacity(int new_size) {
	dh2_hash_access::set_raw_capacity(*this, new_size);
}

// 0x766d10 gameswf::hash<int, gameswf::smart_ptr<gameswf::font>, gameswf::fixed_size_hash<int> >::set_raw_capacity(int)
template <>
void hash<int, smart_ptr<gameswf::font>, fixed_size_hash<int> >::set_raw_capacity(int new_size) {
	dh2_hash_access::set_raw_capacity(*this, new_size);
}

// 0x767094 gameswf::hash<int, gameswf::smart_ptr<gameswf::character_def>, gameswf::fixed_size_hash<int> >::set_raw_capacity(int)
template <>
void hash<int, smart_ptr<gameswf::character_def>, fixed_size_hash<int> >::set_raw_capacity(int new_size) {
	dh2_hash_access::set_raw_capacity(*this, new_size);
}

// 0x7687ac gameswf::hash<gameswf::tu_stringi, gameswf::smart_ptr<gameswf::character_def>, gameswf::stringi_hash_functor<gameswf::tu_stringi> >::set_raw_capacity(int)
template <>
void hash<tu_stringi, smart_ptr<gameswf::character_def>, stringi_hash_functor<tu_stringi> >::set_raw_capacity(int new_size) {
	dh2_hash_access::set_raw_capacity(*this, new_size);
}

// 0x76a6ec gameswf::hash<gameswf::tu_stringi, gameswf::as_object::as_watch, gameswf::stringi_hash_functor<gameswf::tu_stringi> >::set_raw_capacity(int)
template <>
void hash<tu_stringi, gameswf::as_object::as_watch, stringi_hash_functor<tu_stringi> >::set_raw_capacity(int new_size) {
	dh2_hash_access::set_raw_capacity(*this, new_size);
}
#endif  // VENDOR_GAMESWF_DH2_CORE_HEADERS

// 0x767824 gameswf::hash<gameswf::stringi_pointer, int, gameswf::string_pointer_hash_functor<gameswf::stringi_pointer> >::set_raw_capacity(int)
// PARTIAL, not written. The binary has the same algorithm with 16-byte entries (key stringi_pointer at +8, value int
// at +12) and 16 * n + 8 bytes of storage. stringi_pointer and string_pointer_hash_functor are not declared in the tree,
// so no explicit specialization can name them. Layout request: declare stringi_pointer (4 bytes, a pointer to the label).
