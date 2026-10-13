// Batch v1-vendor-gameswf-pack10-9a32ad-0 (vendor-gameswf, part 1 of 1). Eighteen binary functions, one ledger row each
// (ledger.d/v1-vendor-gameswf-pack10-9a32ad-0.csv).
//
// Reconstructed and compiled here (3):
//   hash<tu_stringi, tu_string, stringi_hash_functor<tu_stringi>>::set_raw_capacity(int)  0x78c6cc
//   hash<texture_cache::region*, region*, fixed_size_hash<region*>>::set_raw_capacity(int)  0x7939dc
//   html_reader::parse_tag(stringi_hash<tu_string>&, char const*)                            0x78e060
//   (the two set_raw_capacity declarations are in vendor-gameswf/gameswf_hash_dh2.hpp and texture_cache.hpp; the
//   parse_tag declaration is in vendor-gameswf/html_reader.hpp).
//
// Written behind VENDOR_GAMESWF_DH2_CORE_HEADERS, not compiled in the tree (1):
//   text_character_def::~text_character_def() D1  0x78be3c. gameswf_text.h needs gc_ptr, and gc_ptr fails to compile
//   here (DECLARE_GC_TYPES). The owner header carries the declaration `virtual ~text_character_def();` (DH2 line).
//
// Partial, no body here (6):
//   text_character_def::display(character*)  0x790a7c. The binary is one call, display_glyph_records(&m_matrix, inst,
//     &m_text_glyph_records, m_root_def, nullptr, 0, 0, 0). The upstream static display_glyph_records takes four
//     arguments and the upstream display() has a glyph-provider preamble that the binary does not have. The DH2
//     display_glyph_records signature is not in the tree.
//   edit_text_character_def::~edit_text_character_def() D1 0x78c4dc and D2 0x78c57c (same body). The binary releases
//     m_default_text (tag byte at +0x7c) and then m_var_name (tag byte at +0x34), then the base. The upstream empty
//     destructor (gameswf_text.cpp:550) relies on implicit member destruction, but the DH2 layout places fields
//     differently from upstream (see the edit_text_character_def layout note below), so no body is written.
//   edit_text_character_def::edit_text_character_def(player*, int, int) C1 0x78d970 and C2 0x78dad8 (same body).
//     The DH2 layout differs from upstream: an rgba at +0x44 (set to white, alpha & 0xFE), nine bool flags at +0x48..+0x50,
//     m_font_id -1 at +0x54, m_font at +0x58, m_text_height 240.0 at +0x5c, an rgba at +0x60 (opaque black), and fields
//     at +0x8c and +0x9c that the upstream class does not have. The upstream ctor writes black for the color, so it is
//     not the binary body. Layout request.
//   html_reader::parse(edit_text_character*)  0x78e354. About 20 KB of Hex-Rays output. It uses
//     edit_text_character::text_attributes (not declared in the tree), the html_reader array layout, the DH2 edit_text
//     layout (html flag at +0x138, font pointer at +0x178 and others), as_object::get_player, font::copy_from, the
//     append_text / append_image helpers and the tolower_tab_ table. Not rebuilt.
//
// Upstream, no body here (4):
//   cxform::concatenate(cxform const&)              0x794d38  gameswf_types.cpp:372
//   cxform::transform(rgba) const                   0x794f8c  gameswf_types.cpp:389
//   cxform::clamp()                                  0x795130  gameswf_types.cpp:459
//   rect::enclose_transformed_rect(matrix, rect)    0x795600  gameswf_types.cpp:632
//   These match upstream on every value the DH2 layout can hold: cxform and matrix entries are tu_float, whose
//   operator= maps values outside +/-3.4028e38 (and NaN) to 0, the same guard the binary applies to each result.
//   The binary's clamp return value is a leftover comparison flag, so void is kept. The binary's transform packs the
//   four channels into an int, the same bits as upstream rgba. Each row names its upstream definition.
//
// Upstream, no body here (2): text_character_def ctor C1 0x78dc40 and C2 0x78dd68 (gameswf_text.cpp:251). The
//   binary sets m_root_def, leaves the matrix as the identity, and zeroes the array and flags. The upstream member
//   initializers and the default matrix constructor (identity) do the same. The binary does not write m_rect, which
//   upstream zeroes. Layout note: the binary puts m_is_glyphs_drawn at +0x5c, not after m_sharpness as upstream does.
//
// Skipped (2): the deleting destructors D0 at 0x78bde0 (text_character_def) and 0x78c560 (edit_text_character_def).
//   Compiler-generated: each runs the complete destructor, then operator delete (LC_API_FREE_0).
//
// Layout requests (the integrator applies them to the owner headers): see the ledger notes. Each name used here is
// a field the upstream headers already declare, except html_reader, which is declared in vendor-gameswf/html_reader.hpp.

#include <cmath>
#include <cstddef>
#include <cstring>

#include "base/container.h"
#include "vendor-gameswf/gameswf_alloc.hpp"     // gameswf::malloc_internal, gameswf::free_internal
#include "vendor-gameswf/gameswf_hash_dh2.hpp"  // hash<tu_stringi, tu_string> declaration; dh2_hash::bucket_count
#include "vendor-gameswf/html_reader.hpp"       // gameswf::html_reader
#include "vendor-gameswf/texture_cache.hpp"     // gameswf::texture_cache::region and the region table declaration

#if defined(VENDOR_GAMESWF_DH2_CORE_HEADERS)
#include "gameswf/gameswf_text.h"               // gameswf::text_character_def (gc_ptr; does not compile in this tree)
#endif

// ---------------------------------------------------------------------------------------------------------------------
// hash<> rebuild, shared shape for the two tables below (binary: the table of set_raw_capacity, one body each).
//   new_size <= 0: clear() and return.
//   buckets = dh2_hash::bucket_count(new_size). If the table exists with that many buckets, return (no change).
//   Otherwise build a table of sizeof(table) + sizeof(entry) * buckets bytes with every next_in_chain set to -2 and
//   entry_count 0, mask buckets - 1. Each live entry of the old table (next != -2 and hash != -1) is add()ed to the new
//   table; its key and value are then destroyed (the binary frees their heap text) and the slot is marked empty (next
//   -2, hash 0). The old table is freed and the new one is installed.
// The binary passes one argument to malloc_internal in the decompile. The DH2 allocator ignores the hint, so
// MEM_UNRECOVERED is passed (vendor-gameswf/gameswf_alloc.hpp). free_internal ignores its size argument.
// ---------------------------------------------------------------------------------------------------------------------

// 0x78c6cc gameswf::hash<gameswf::tu_stringi, gameswf::tu_string, gameswf::stringi_hash_functor<gameswf::tu_stringi> >::set_raw_capacity(int)
template <>
void hash<tu_stringi, tu_string, stringi_hash_functor<tu_stringi> >::set_raw_capacity(int new_size)
{
	if (new_size <= 0) {
		clear();  // binary: the clear() at LABEL_21
		return;
	}

	const int buckets = gameswf::dh2_hash::bucket_count(new_size);
	if (m_table != nullptr && m_table->m_size_mask + 1 == buckets) {
		return;  // no change in raw capacity
	}

	hash<tu_stringi, tu_string, stringi_hash_functor<tu_stringi> > rebuilt;  // holds the new table until it is installed
	rebuilt.m_table = static_cast<table*>(gameswf::malloc_internal(
		static_cast<unsigned int>(sizeof(table) + sizeof(entry) * buckets), swf::MEM_UNRECOVERED));
	rebuilt.m_table->m_entry_count = 0;
	rebuilt.m_table->m_size_mask = buckets - 1;
	for (int i = 0; i < buckets; i++) {
		rebuilt.E(i).m_next_in_chain = -2;  // binary: *(v12[0] + 8 + 48 * i) = -2
	}

	if (m_table != nullptr) {
		// Binary loop: skip empty slots (next == -2) and tombstones (hash == -1).
		for (int i = 0, n = m_table->m_size_mask; i <= n; i++) {
			entry& e = E(i);
			if (e.is_empty() || e.is_tombstone()) {
				continue;
			}
			rebuilt.add(e.first, e.second);
			e.first.~tu_stringi();  // binary: free the key's heap text if its tag byte is -1
			e.second.~tu_string();  // binary: free the value's heap text if its tag byte is -1
			e.m_next_in_chain = -2;
			e.m_hash_value = 0;     // binary: v11[1] = 0
		}
		gameswf::free_internal(m_table, static_cast<unsigned int>(sizeof(table) + sizeof(entry) * (m_table->m_size_mask + 1)));
	}

	m_table = rebuilt.m_table;  // binary: *v2 = v12[0]; v12[0] = 0
	rebuilt.m_table = nullptr;
}

// 0x7939dc gameswf::hash<gameswf::texture_cache::region*, gameswf::texture_cache::region*, gameswf::fixed_size_hash<gameswf::texture_cache::region*> >::set_raw_capacity(int)
// The keys and values are pointers, so the binary releases nothing when it empties an old slot.
template <>
void ::hash<gameswf::texture_cache::region*, gameswf::texture_cache::region*,
            ::fixed_size_hash<gameswf::texture_cache::region*> >::set_raw_capacity(int new_size)
{
	using region_hash = ::hash<gameswf::texture_cache::region*, gameswf::texture_cache::region*,
	                           ::fixed_size_hash<gameswf::texture_cache::region*> >;

	if (new_size <= 0) {
		clear();
		return;
	}

	const int buckets = gameswf::dh2_hash::bucket_count(new_size);
	if (m_table != nullptr && m_table->m_size_mask + 1 == buckets) {
		return;
	}

	region_hash rebuilt;
	rebuilt.m_table = static_cast<table*>(gameswf::malloc_internal(
		static_cast<unsigned int>(sizeof(table) + sizeof(entry) * buckets), swf::MEM_UNRECOVERED));
	rebuilt.m_table->m_entry_count = 0;
	rebuilt.m_table->m_size_mask = buckets - 1;
	for (int i = 0; i < buckets; i++) {
		rebuilt.E(i).m_next_in_chain = -2;
	}

	if (m_table != nullptr) {
		for (int i = 0, n = m_table->m_size_mask; i <= n; i++) {
			entry& e = E(i);
			if (e.is_empty() || e.is_tombstone()) {
				continue;
			}
			rebuilt.add(e.first, e.second);
			e.m_next_in_chain = -2;
			e.m_hash_value = 0;
		}
		gameswf::free_internal(m_table, static_cast<unsigned int>(sizeof(table) + sizeof(entry) * (m_table->m_size_mask + 1)));
	}

	m_table = rebuilt.m_table;
	rebuilt.m_table = nullptr;
}

// ---------------------------------------------------------------------------------------------------------------------
// html_reader::parse_tag
// ---------------------------------------------------------------------------------------------------------------------

namespace gameswf {

// 0x78e060 gameswf::html_reader::parse_tag(gameswf::stringi_hash<gameswf::tu_string>&, char const*)
//
// Binary (Hex-Rays, 704 bytes):
//   '/' first: return 0.
//   name_end = first ' ', else first '/', else end of string. attrs.set("name", [tag, name_end)).
//   eq = first '='. None: return 1.
//   Loop, with cur = the '=' being read (binary v7; v8 is the same position, the end of the name):
//     start of the name = the character after the last ' ' before cur (binary's backward scan for v10).
//     q1 = first '"' at or after cur, q2 = first '\'' at or after cur. Neither: break (return 0).
//     open = the earlier of q1 and q2 (the one present, if only one is). close = next occurrence of *open after open.
//     No close: break (return 0).
//     attrs.set(name = [start, cur), value = (open, close)).
//     cur = next '=' at or after close. None: return 1.
// The binary scans back from cur without a bound, which reads before the string when no ' ' precedes the '='. Here the
// scan stops at the start of the tag, so the name then begins at the tag. Only malformed input differs.
// The space-before-'=' branch in the Hex-Rays output cannot run: cur always points at a '='.
int html_reader::parse_tag(stringi_hash<tu_string>& attrs, const char* tag)
{
	if (*tag == '/') {
		return 0;  // closing tag
	}

	const char* name_end = std::strchr(tag, ' ');
	if (name_end == nullptr) {
		name_end = std::strchr(tag, '/');
		if (name_end == nullptr) {
			name_end = tag + std::strlen(tag);
		}
	}
	attrs.set(tu_stringi("name"), tu_string(tag, static_cast<int>(name_end - tag)));

	const char* eq = std::strchr(tag, '=');
	if (eq == nullptr) {
		return 1;
	}

	std::ptrdiff_t cur = eq - tag;  // index of the '=' being read
	for (;;) {
		// Name start: the character after the last ' ' before cur. Index -1 means there is no space.
		std::ptrdiff_t space = cur - 1;
		while (space >= 0 && tag[space] != ' ') {
			--space;
		}
		const std::ptrdiff_t name_start = space + 1;

		const char* q1 = std::strchr(tag + cur, '"');
		const char* q2 = std::strchr(tag + cur, '\'');
		if (q1 == nullptr && q2 == nullptr) {
			return 0;  // binary: break, no quote after the '='
		}

		const char* open;
		if (q1 != nullptr && q2 != nullptr) {
			open = (q1 >= q2) ? q2 : q1;  // binary: v16 = min(v11, v12)
		} else {
			open = (q1 != nullptr) ? q1 : q2;
		}

		const char* close = std::strchr(open + 1, *open);
		if (close == nullptr) {
			return 0;  // binary: break, no matching quote
		}

		attrs.set(tu_stringi(tu_string(tag + name_start, static_cast<int>(cur - name_start))),
		          tu_string(open + 1, static_cast<int>(close - open - 1)));

		const char* next_eq = std::strchr(close, '=');
		if (next_eq == nullptr) {
			return 1;  // binary: LABEL_30
		}
		cur = next_eq - tag;
	}
}

}  // namespace gameswf

// ---------------------------------------------------------------------------------------------------------------------
// text_character_def::~text_character_def, D1 (0x78be3c). Behind VENDOR_GAMESWF_DH2_CORE_HEADERS: not compiled here.
//
// Binary: the vtable store (the compiler's), then array<text_glyph_record>::resize(this+0x4c, 0) and
// reserve(this+0x4c, 0), then the base destructor. The DH2 array's destructor frees the buffer, which the upstream
// array::~array (clear(), resize(0) only, base/container.h:216) does not, so the reserve(0) is written here. The base
// destructor runs after this body, as usual.
// ---------------------------------------------------------------------------------------------------------------------

#if defined(VENDOR_GAMESWF_DH2_CORE_HEADERS)
namespace gameswf {

// 0x78be3c gameswf::text_character_def::~text_character_def()   [D1]
text_character_def::~text_character_def()
{
	m_text_glyph_records.resize(0);
	m_text_glyph_records.reserve(0);
}

}  // namespace gameswf
#endif  // VENDOR_GAMESWF_DH2_CORE_HEADERS
