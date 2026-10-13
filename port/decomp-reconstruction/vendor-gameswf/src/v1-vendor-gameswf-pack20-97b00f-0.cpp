// Batch v1-vendor-gameswf-pack20-97b00f-0 (gameswf r1714 in libDungeonHunter2.so, Gameloft build).
// Ledger: vendor-gameswf/ledger.d/v1-vendor-gameswf-pack20-97b00f-0.csv (one row per batch function).
//
// Rows that need no body here (the notes in the ledger give the upstream definition):
//   upstream:
//     button_character_definition D1 0x7c78d4, D2 0x7c797c           gameswf_button.cpp:682 (dtor body, twin rows)
//     canvas D1 0x7c8aa0, D2 0x7c8b00, C1 0x7c8b3c, C2 0x7c8b98      gameswf_canvas.cpp:13 and :23
//     inst_info_avm2 operator= 0x7c9b24, D1 0x7c9bc8                 implicit members, gameswf_disasm.cpp:85
//     hash<tu_string, face_entity>::entry ctor 0x7d081c, copy 0x7d0858  generic entry, base/container.h
//   skipped:
//     canvas D0 0x7c8ae4: deleting destructor. PUSH, BL D1, BL LC_API_FREE_0, POP. Compiler-generated.
//   partial, not written (the owner type is not declared anywhere in the tree; see the ledger notes):
//     button_character_definition C1 0x7c7a08, C2 0x7c7a64          character_def base has no layout header
//     hash<int, inst_info_avm2>::entry copy 0x7c9cd8                  inst_info_avm2 is local to gameswf_disasm.cpp
//     hash<int, inst_info_avm2>::set_raw_capacity 0x7ca120            same
//     hash<ushort, short, font::simple_code_hash>::set_raw_capacity 0x7ce694    gameswf::font has no header
//     hash<kerning_pair, float>::set_raw_capacity 0x7ce998            same
//
// Rebuilt here:
//   hash<tu_string, smart_ptr<face_entity>>::set_raw_capacity 0x7d0a20 (the face table m_faces of glyph_provider).

#include <cmath>
#include <cstddef>

#include "base/container.h"                     // ::hash<>, ::string_hash_functor<>, ::tu_string
#include "base/smart_ptr.h"                     // ::smart_ptr<T>
#include "core-platform/deps/swf_types.hpp"     // swf::MemHint, swf::MEM_UNRECOVERED
#include "vendor-gameswf/gameswf_alloc.hpp"     // gameswf::malloc_internal, gameswf::free_internal
#include "vendor-gameswf/gameswf_hash_dh2.hpp"  // gameswf::dh2_hash::bucket_count
#include "vendor-gameswf/glyph_provider.hpp"    // gameswf::face_entity and the m_faces table declaration

// 0x7d0a20 gameswf::hash<gameswf::tu_string, gameswf::smart_ptr<gameswf::face_entity>, gameswf::string_hash_functor<gameswf::tu_string> >::set_raw_capacity(int)
//   (_ZN7gameswf4hashINS_9tu_stringENS_9smart_ptrINS_11face_entityEEENS_19string_hash_functorIS1_EEE16set_raw_capacityEi)
// Asm checked: the bucket count uses logf, then __aeabi_fdiv by 0x3F317218 (ln 2), then + 1.0 and the float to int
// conversion, so gameswf::dh2_hash::bucket_count applies. Entry size 32 bytes (malloc 32 * n + 8), key at +8, value at +28.
// Steps, as in the binary: new_size <= 0 clears and returns. If the table exists with the same bucket count, return.
// Else build a new table: entry count 0, mask buckets - 1, next = -2 on every entry. Each live entry of the old table
// (next != -2 and hash != -1), in index order, is added to the new table and cleared. The old table is freed (the
// binary's second argument to free_internal is ignored) and the new table is installed.
template <>
void hash<::tu_string, smart_ptr<gameswf::face_entity>, ::string_hash_functor<::tu_string> >::set_raw_capacity(int new_size) {
	if (new_size <= 0) {
		clear();
		return;
	}
	const int buckets = gameswf::dh2_hash::bucket_count(new_size);
	if (m_table != nullptr && m_table->m_size_mask + 1 == buckets) {
		return;  // no change in raw capacity
	}

	hash<::tu_string, smart_ptr<gameswf::face_entity>, ::string_hash_functor<::tu_string> > rebuilt;  // holds the new table
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
			if (!e.is_empty() && !e.is_tombstone()) {
				rebuilt.add(e.first, e.second);
				e.clear();
			}
		}
		gameswf::free_internal(m_table, static_cast<unsigned int>(sizeof(table) + sizeof(entry) * (m_table->m_size_mask + 1)));
	}

	m_table = rebuilt.m_table;
	rebuilt.m_table = nullptr;  // rebuilt's destructor (clear) now does nothing
}
