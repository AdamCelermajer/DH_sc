// Batch v1-vendor-gameswf-gameswf_sprite_definition-9a9a59-0: gameswf::sprite_definition.
//
// Header edits (upstream/gameswf-r1714/gameswf/gameswf_sprite_def.h, all marked DH2):
//   - Eight inline `{ assert(0); }` overrides are inside #if 0 and declared there instead. Their binary
//     bodies are bx lr (no assert), so the bodies are defined in this file.
//   - get_labeled_frame(tu_stringi const&, int*) is virtual in the binary (vtable +0x60). It is declared in
//     the header with DH2. The upstream const char* inline stays until movie_definition takes tu_stringi.
//   - has_init_actions() is virtual in the binary (vtable +0x88) and is declared in the header with DH2.
//
// Not defined here (upstream inline bodies that match the binary, so their ledger rows point at the header):
//   output_cached_data(tu_file*, cache_options const&)   vtable +0x1c, upstream `{ return; }` = bx lr
//   input_cached_data(tu_file*)                          vtable +0x20, upstream `{ return; }` = bx lr
//
// Not defined here (compiler-generated):
//   ~sprite_definition()  [D0, deleting]  vtable +0x04. Calls D1, then LC_API_FREE_0 (operator delete).
//
// Build status: this translation unit does not build. gameswf_sprite_def.h needs gameswf/gameswf_movie_def.h.
// That header instantiates gc_ptr<T> with DH2 types, and base/tu_gc_singlethreaded_refcount.h:120, :123,
// :136 and :139 then convert them to gc_object_collector_base*. The DH2 ref_counted does not derive from it.
// The same errors stop the player, character and character_def batches, which fail at baseline. Those are the
// only errors check_tu reports for this file, and none comes from the code below. This batch did not touch
// that header, so every function here is partial until the gc configuration is fixed. The bodies follow the
// Hex-Rays pseudocode and the disassembly.

#include "gameswf/gameswf.h"
#include "gameswf/gameswf_movie_def.h"
#include "gameswf/gameswf_sprite_def.h"

namespace gameswf {

// 0x783184 gameswf::sprite_definition::add_abc(gameswf::tu_string&, gameswf::abc_def*)
// vtable +0x70. Empty body (bx lr). The upstream inline assert(0) is debug-only; the binary has no assert.
void sprite_definition::add_abc(tu_string& /*name*/, abc_def* /*abc*/) {
}

// 0x783188 gameswf::sprite_definition::add_symbol_class(int, gameswf::tu_string const&)
// vtable +0x64. Empty body (bx lr).
void sprite_definition::add_symbol_class(int /*character_id*/, const tu_string& /*class_name*/) {
}

// 0x78318c gameswf::sprite_definition::add_scene(int, gameswf::tu_string const&)
// vtable +0x68. Empty body (bx lr).
void sprite_definition::add_scene(int /*character_id*/, const tu_string& /*class_name*/) {
}

// 0x783190 gameswf::sprite_definition::add_frame_label(int, gameswf::tu_string const&)
// vtable +0x6c. Empty body (bx lr).
void sprite_definition::add_frame_label(int /*character_id*/, const tu_string& /*class_name*/) {
}

// 0x7831b0 gameswf::sprite_definition::set_jpeg_loader(gameswf::jpeg::input*)
// vtable +0x90. Empty body (bx lr): the loader pointer is not stored.
void sprite_definition::set_jpeg_loader(jpeg::input* /*j_in*/) {
}

// 0x783214 gameswf::sprite_definition::add_bitmap_info(gameswf::bitmap_info*)
// vtable +0xb0. Empty body (bx lr): the bitmap_info is not stored.
void sprite_definition::add_bitmap_info(bitmap_info* /*bi*/) {
}

// 0x783234 gameswf::sprite_definition::add_import(gameswf::tu_string const&, int, gameswf::tu_string const&)
// vtable +0xac. Empty body (bx lr).
void sprite_definition::add_import(const tu_string& /*source_url*/, int /*id*/, const tu_string& /*symbol*/) {
}

// 0x783238 gameswf::sprite_definition::visit_imported_movies(gameswf::movie_definition::import_visitor*)
// vtable +0x44. Empty body (bx lr): a sprite has no imports to visit.
void sprite_definition::visit_imported_movies(movie_definition::import_visitor* /*v*/) {
}

// 0x7832e4 gameswf::sprite_definition::get_labeled_frame(gameswf::tu_stringi const&, int*)
// vtable +0x60. Hex-Rays: look the label up in m_named_frames (original offset +0x58). If there is no entry,
// return 0. Otherwise, if frame_number is non-null, store the entry's value, and return 1 either way.
// That is exactly the contract of the hash get(key, value*) in base/container.h, which returns false when
// the key is absent, stores the value only when value != NULL, and returns true when found.
// Hash layout check (binary): table header is {int entry_count; int size_mask}, and entries are 16 bytes
// at table+8, so the value sits at table+16*i+20. This matches the upstream hash<> entry layout.
// Note: the binary's keys are stringi_pointer (4 bytes, so 16-byte entries), while the upstream stringi_hash<int>
// keys are tu_stringi. The binary's key wraps a pointer to the label, and string_pointer_hash_functor hashes
// through that pointer, so the lookup is by label text in both. The member is used through the upstream API.
// The container layout difference is a layout request (see the ledger notes).
int sprite_definition::get_labeled_frame(const tu_stringi& label, int* frame_number) {
	return m_named_frames.get(label, frame_number) ? 1 : 0;
}

// 0x7835f0 gameswf::sprite_definition::~sprite_definition()  [D0, deleting destructor]
// Compiler-generated: PUSH {R4,LR}; calls D1 (0x783534) on this; calls LC_API_FREE_0 on this; POP {R4,PC}.
// Not written here. The ledger row is skipped.

// 0x783268 gameswf::sprite_definition::has_init_actions()
// vtable +0x88. MOV R0, #0; BX LR. Sprites have no init actions, so the result is always 0.
int sprite_definition::has_init_actions() {
	return 0;
}

}  // namespace gameswf
