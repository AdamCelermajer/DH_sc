// Batch v1-vendor-gameswf-pack14-1a21ee-0 (vendor-gameswf). 20 functions of gameswf::as_transform, the bitmap glyph
// providers, the RenderFX search index hash and the ear-clip / grid index helpers.
//
// Reconstructed and compiled here (5):
//   0x7aa8f0 hash<tu_string, array<RenderFX::SearchIndex::Entry>*, string_hash_functor<tu_string>>::set_raw_capacity
//   0x7c4e34 bitmap_glyph_provider::~bitmap_glyph_provider() (D2); 0x7c63f8 (D1) is the same body in C++
//   0x7a9664 default_bitmap_glyph_provider::~default_bitmap_glyph_provider() (D1)
//   0x7a9634 default_bitmap_glyph_provider::get_font_entity_impl: new default_bitmap_font_entity (constructor: pack19)
// PARTIAL (6), written but not complete (see each comment):
//   0x7a7a8c, 0x7a7bd8 as_transform C1/C2; 0x7a72e8 as_transform D1; 0x7a75c0 as_transform::init. Behind
//     DH2_AS_OBJECT_HEADER_OK: gameswf/gameswf_object.h does not compile in this tree (see as_stage.hpp).
//   0x7a9698 bitmap_glyph_provider C2: the new bitmap_glyph_texture_cache is not wired (see its comment).
//   0x7c6048 bitmap_glyph_provider::get_font_entity: the scan over the loaded entities is not written.
// SKIPPED (3): deleting destructors 0x7a7344 (as_transform D0), 0x7a99f8 (default_bitmap_glyph_provider D0) and
//   0x7c646c (bitmap_glyph_provider D0). The compiler emits them from delete; they run D1 then free the object.
// UPSTREAM (6), no body here: grid_index_point / grid_index_box<float,bool>::get_containing_cell_clamped (0x7b054c,
//   0x7b11e4), ear_clip_wrapper<float>::vertex_in_cone (0x7b063c), any_edge_intersects (0x7b157c), debug_make_square
//   (0x7b3cd8) and debug_make_x (0x7b4130). Their bodies match the upstream templates in base/grid_index.h and
//   base/ear_clip_triangulate_impl.h; the debug helpers sit under #ifdef DEBUG_MARKUP upstream.
//
// The as_transform layout (weak movie clip at +0x38) differs from upstream's gc_ptr members. The upstream class and
// ctor are marked #if 0 /* DH2 */ and replaced by vendor-gameswf/include/vendor-gameswf/as_transform.hpp.
// The bitmap glyph provider has no upstream source; its header is vendor-gameswf/include/vendor-gameswf/bitmap_glyph_provider.hpp.
// The hash specialization is declared in vendor-gameswf/include/vendor-gameswf/gameswf_dh2_hash.hpp so that every
// translation unit uses it instead of the generic upstream member (which uses a 16-entry minimum).

#include <cmath>
#include <cstddef>
#include <cstring>

#include "base/container.h"
#include "base/smart_ptr.h"
#include "vendor-gameswf/bitmap_glyph_provider.hpp"  // bitmap_glyph_provider, default_bitmap_glyph_provider (DH2)
#include "vendor-gameswf/gameswf_alloc.hpp"          // gameswf::malloc_internal, gameswf::free_internal
#include "vendor-gameswf/gameswf_dh2_hash.hpp"       // the set_raw_capacity specialization declared there
#include "vendor-gameswf/texture_cache.hpp"          // gameswf::texture_cache

#if defined(DH2_AS_OBJECT_HEADER_OK)
#include "gameswf/gameswf_as_classes/as_color_transform.h"  // as_color_transform (upstream; DH2 ctor is not declared)
#include "gameswf/gameswf_as_classes/as_matrix.h"           // as_matrix (upstream; DH2 ctor is not declared)
#include "vendor-gameswf/as_transform.hpp"                  // gameswf::as_transform (DH2 layout)
#include "vendor-gameswf/as_value.hpp"                      // gameswf::as_value
#endif

// 0x7aa8f0 gameswf::hash<gameswf::tu_string, gameswf::array<RenderFX::SearchIndex::Entry>*, gameswf::string_hash_functor<gameswf::tu_string> >::set_raw_capacity(int)
// Binary: entry 32 bytes (next_in_chain at +0, hash value at +4, key tu_string at +8, value pointer at +28). The value
// has no destructor. The key's heap text is freed when its tag byte (+8) is -1. The table is malloc_internal(16 * n + 8)
// on ARM; here it is sizeof(table) + sizeof(entry) * n (TRACK-B decision 2: host width).
// The algorithm is the same as dh2_hash_access::set_raw_capacity (vendor-gameswf/src/v1-vendor-gameswf-pack5-64b2da-0.cpp),
// written here in full because that template is defined only in the pack5 translation unit. Member explicit
// specializations may use the private members of hash, so no friend is needed.
template <>
void hash<tu_string, array<RenderFX::SearchIndex::Entry>*, string_hash_functor<tu_string> >::set_raw_capacity(int new_size)
{
	using self_t = hash<tu_string, array<RenderFX::SearchIndex::Entry>*, string_hash_functor<tu_string> >;
	using entry_t = typename self_t::entry;
	using table_t = typename self_t::table;

	if (new_size <= 0) {
		// Binary: clear() and return.
		clear();
		return;
	}

	// Binary: v3 = 1 << (int)(float)((float)(logf((float)(a2 - 1)) / ln2) + 1.0); then v3 = max(v3, 4), ln2 = 0x3F317218.
	// The division is single precision (__aeabi_fdiv by the float 0x3F317218; the decimal 0.69315 is a different value). For new_size == 1
	// logf(0) is -inf, the shift count is then zero in the low byte, and the minimum of 4 applies.
	int size = 4;
	if (new_size > 1) {
		const float quotient = std::logf(static_cast<float>(new_size - 1)) / 0.693147182464599609375f;  // 0x3F317218 (ln 2)
		const int bits = static_cast<int>(quotient + 1.0f);  // single precision: __aeabi_fadd with 0x3F800000 (1.0f)
		const unsigned int pow2 = (bits >= 0 && bits < 32) ? (1u << bits) : 0u;
		if (pow2 >= 4u) {
			size = static_cast<int>(pow2);
		}
	}

	// Binary: if the table exists and already has this many slots, nothing is done.
	if (m_table != nullptr && size == m_table->m_size_mask + 1) {
		return;
	}

	// Binary: malloc_internal(size * entry + table header); entry count 0, size mask size - 1, next_in_chain = -2 on every slot.
	self_t new_hash;
	new_hash.m_table = static_cast<table_t*>(gameswf::malloc_internal(
		static_cast<unsigned int>(sizeof(table_t) + sizeof(entry_t) * static_cast<std::size_t>(size)), swf::MEM_UNRECOVERED));
	new_hash.m_table->m_entry_count = 0;
	new_hash.m_table->m_size_mask = size - 1;
	for (int i = 0; i < size; i++) {
		new_hash.E(i).m_next_in_chain = -2;
	}

	if (m_table != nullptr) {
		// Binary: in index order, each live entry (not empty, not tombstone) is added to the new table, then its key
		// and value are destroyed and its slot marked empty. The old table is freed with free_internal.
		for (int i = 0, n = m_table->m_size_mask; i <= n; i++) {
			entry_t* e = &E(i);
			if (!e->is_empty() && !e->is_tombstone()) {
				new_hash.add(e->first, e->second);
				e->clear();
			}
		}
		gameswf::free_internal(m_table, 0);
	}

	// Binary: the new table is taken over; the temporary is left empty so its destructor frees nothing.
	m_table = new_hash.m_table;
	new_hash.m_table = nullptr;
}

namespace gameswf {

// 0x7a9698 gameswf::bitmap_glyph_provider::bitmap_glyph_provider(int, int, bool) (C2)
// PARTIAL: the binary makes the bitmap_glyph_texture_cache (operator new(0x40), then its constructor with the two sizes) and
// stores it at +0x0c. The class is in the tree now (batch pack18), but the call is not written: ~bitmap_glyph_provider
// releases the cache with free_internal, and new does not pair with free_internal on the host, so m_texture_cache stays null.
bitmap_glyph_provider::bitmap_glyph_provider(int texture_width, int texture_height, bool flag)
	: m_flag(flag)
{
	// Binary: vptr, +0x08 = flag, +0x04 (the hash table) = 0, +0x0c = 0. The cache is made only when both sizes are positive.
	if (texture_width > 0 && texture_height > 0) {
		// MISSING: operator new(0x40), bitmap_glyph_texture_cache(texture_width, texture_height), store at +0x0c.
	}
}

// 0x7c4e34 gameswf::bitmap_glyph_provider::~bitmap_glyph_provider() (D2). 0x7c63f8 (D1) has the same body, so one
// definition serves both. The deleting destructor 0x7c646c is compiler-generated.
bitmap_glyph_provider::~bitmap_glyph_provider()
{
	// Binary: clear the entity hash at +0x04 (the table is freed by clear, not by the member destructor below).
	m_font_entities.clear();
	if (m_texture_cache != nullptr) {
		m_texture_cache->reset();
		texture_cache* cache = m_texture_cache;
		cache->~texture_cache();
		gameswf::free_internal(cache, 0);
	}
	// The member m_font_entities is destroyed after this body. Its destructor clears the hash again, as the binary does.
}

// 0x7c6048 gameswf::bitmap_glyph_provider::get_font_entity(gameswf::tu_string const&, bool, bool)
// PARTIAL: the binary first scans the raw entries of the entity hash for an entity whose name at +0x10 equals the font
// file name (and, on a match, stores that entity under the new key and returns it). The scan reads the hash's raw table,
// and the DH2 hash in this tree has no iteration API for it, so it is not written. This body creates a new entity in
// that case, which differs from the binary when a matching entity is already loaded under another key.
gameswf::ref_counted* bitmap_glyph_provider::get_font_entity(const ::tu_string& name, bool is_bold, bool is_italic)
{
	// Binary: key = name, plus "B" when bold, plus "I" when italic.
	::tu_string key(name);
	if (is_bold) {
		key += "B";
	}
	if (is_italic) {
		key += "I";
	}

	// Binary: hash get on the key. A hit returns the entity without any other work.
	smart_ptr<bitmap_font_entity> entity;
	const bool found = m_font_entities.get(::tu_stringi(key), &entity);
	if (!found) {
		char fontfile[256];
		std::memset(fontfile, 0, sizeof(fontfile));  // binary: LC_API_MEMSET_0(buffer, 0, 256)
		if (get_fontfile(name.c_str(), is_bold, is_italic, fontfile, static_cast<int>(sizeof(fontfile))) &&
		    (std::strstr(fontfile, ".fnt") != nullptr || std::strstr(fontfile, ".FNT") != nullptr)) {
			// MISSING: the scan over loaded entities (see the comment above).
			// Binary: the virtual create, vtable slot +0x08 (get_font_entity_impl), on the font file name.
			entity = get_font_entity_impl(::tu_string(fontfile));
		}
		// Binary: store under the key, even when the entity is null. operator[] inserts the key when it is absent.
		m_font_entities[::tu_stringi(key)] = entity;
	}
	// The entity is borrowed from m_font_entities; the local smart_ptr releases its own reference on return.
	return entity.get_ptr();
}

// 0x7a9664 gameswf::default_bitmap_glyph_provider::~default_bitmap_glyph_provider() (D1). The deleting destructor 0x7a99f8 is
// compiler-generated.
default_bitmap_glyph_provider::~default_bitmap_glyph_provider()
{
	// Binary: vptr = the default vtable (0x991138), then the base destructor 0x7c4e34. C++ runs the base destructor
	// after this body, and the base destructor body is the one written above.
}

// 0x7a9634 gameswf::default_bitmap_glyph_provider::get_font_entity_impl(gameswf::tu_string const&)
// The constructor default_bitmap_font_entity(this, name) is batch v1-vendor-gameswf-pack19-4d4603-0 (0x7c663c). The binary
// allocates 0x64 bytes; this build allocates sizeof(default_bitmap_font_entity), the host size (TRACK-B decision 2).
default_bitmap_font_entity* default_bitmap_glyph_provider::get_font_entity_impl(const ::tu_string& name)
{
	return new default_bitmap_font_entity(this, name);
}

}  // namespace gameswf

#if defined(DH2_AS_OBJECT_HEADER_OK)
// Everything below needs gameswf::as_object complete. gameswf/gameswf_object.h does not compile in this tree, so these
// bodies are not compiled by the tree check. Status PARTIAL for all of them.

namespace gameswf {

// 0x7a7a8c (C1) and 0x7a7bd8 (C2). Same body in C++. The binary order is kept: as_object base, vptr, the empty weak
// link at +0x38, then the link set to movie_clip, then init().
as_transform::as_transform(player* p, character* movie_clip)
	: as_object(p)
{
	m_movie = movie_clip;  // binary: weak_ptr<character>::operator=(this + 0x38, movie_clip)
	init();
}

// 0x7a72e8 (D1). PARTIAL: the binary releases the proxy at +0x38 with count-- and free_internal when it reaches zero. The
// upstream weak_ptr releases its proxy with delete, and the DH2 smart_ptr<weak_proxy> release is not declared here.
// The member m_movie and the base as_object are destroyed by C++ after this body.
as_transform::~as_transform()
{
}

// 0x7a75c0 gameswf::as_transform::init(void). PARTIAL: four blocks in binary order: colorTransform,
// concatenatedColorTransform, matrix and concatenatedMatrix. Each block first checks the weak player link
// (as_object::m_player, +0x2c proxy and +0x30 player) and the weak movie link (m_movie, +0x38 and +0x3c), as
// weak_ptr::get_ptr does, then builds one as_color_transform or as_matrix and stores it with builtin_member.
// Differences from the binary:
//   - The binary reads the movie's local cxform (+0x48) with no null test. After a dead link it reads address 0x48.
//     Here a null movie gives a null cxform.
//   - get_world_cxform and get_world_matrix are called on the movie without a null test in the binary. The DH2 form of
//     those accessors returns a pointer, which the upstream header does not declare yet (upstream returns by value).
//   - The DH2 constructors as_color_transform(player*, cxform const*) and as_matrix(player*, matrix const*) are not
//     declared by the upstream headers; they must be added there (// DH2) by their own batch.
void as_transform::init()
{
	// Block 1: colorTransform. The local cxform is at character +0x48.
	{
		player* p = m_player.get_ptr();
		character* movie = m_movie.get_ptr();
		const cxform* local = movie != nullptr ? movie->m_local_cxform : nullptr;  // +0x48, not declared in gameswf_character.h yet
		as_color_transform* color = new as_color_transform(p, local);  // binary: operator new(0x58), then the constructor
		as_value value(color);  // binary: type 5 and add_ref; the DH2 as_value(as_object*) takes that reference
		builtin_member(tu_stringi("colorTransform"), value);
	}

	// Block 2: concatenatedColorTransform. The world cxform comes from the movie (pointer form).
	{
		player* p = m_player.get_ptr();
		character* movie = m_movie.get_ptr();
		const cxform* world = movie->get_world_cxform();  // binary: character::get_world_cxform(movie)
		as_color_transform* color = new as_color_transform(p, world);
		as_value value(color);
		builtin_member(tu_stringi("concatenatedColorTransform"), value);
	}

	// Block 3: matrix. The local matrix is at character +0x4c (m_local_matrix).
	{
		player* p = m_player.get_ptr();
		character* movie = m_movie.get_ptr();
		const matrix* local = movie != nullptr ? movie->m_local_matrix : nullptr;  // +0x4c
		as_matrix* mat = new as_matrix(p, local);  // binary: operator new(0x50), then the constructor
		as_value value(mat);
		builtin_member(tu_stringi("matrix"), value);
	}

	// Block 4: concatenatedMatrix. The world matrix comes from the movie (pointer form).
	{
		player* p = m_player.get_ptr();
		character* movie = m_movie.get_ptr();
		const matrix* world = movie->get_world_matrix();  // binary: character::get_world_matrix(movie)
		as_matrix* mat = new as_matrix(p, world);
		as_value value(mat);
		builtin_member(tu_stringi("concatenatedMatrix"), value);
	}
}

}  // namespace gameswf
#endif  // DH2_AS_OBJECT_HEADER_OK
