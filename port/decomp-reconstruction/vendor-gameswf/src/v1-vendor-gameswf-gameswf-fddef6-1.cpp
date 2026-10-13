// Batch v1-vendor-gameswf-gameswf-fddef6-1 (part 2 of 8, class owner: gameswf).
//
// Compiled here (no gameswf.h dependency):
//   get/set_verbose_dlist, register_texture_loader_callback, define_video_loader (thunk),
//   define_native_image_loader (partial), destructp<T>, destruct<gameswf::image_rgb>, destruct<gameswf::image_rgba>,
//   destruct<ref_counted>, destruct<jpeg::input>, hash<int, loader_function>::set_raw_capacity.
// Written but not compiled (PARTIAL, behind VENDOR_GAMESWF_DH2_CORE_HEADERS): substitute_bitmap_character,
//   define_debug_id_loader, destruct<shape_character_def>, destruct<execute_tag>. They need gameswf.h or
//   gameswf_movie_def.h, which do not compile in the tree yet (see the header comment).
// Upstream r1714 code, ledgered as "upstream" without a body here: null_loader, end_loader,
//   jpeg_tables_loader, frame_label_loader.
// Not written: destruct<gameswf::character::custom> (class not declared in the tree).
//
// Naming: the binary puts base-library types in namespace gameswf (gameswf::tu_string, gameswf::image_rgb,
// gameswf::hash). The vendored source has tu_string and hash in the global namespace, and the upstream names
// are used here. gameswf::image_rgb and gameswf::image_rgba are the Gameloft classes, not image::rgb and
// image::rgba from upstream base/image.h: they derive from gameswf::image_base (vendor-gameswf/image_base.hpp).

#include "vendor-gameswf/gameswf_dh2_runtime.hpp"
#include "vendor-gameswf/image_rgb.hpp"
#include "vendor-gameswf/image_rgba.hpp"

#include <cmath>
#include <cstdint>

#include "base/jpeg.h"
#include "base/tu_file.h"
#include "gameswf/gameswf_stream.h"

#if defined(VENDOR_GAMESWF_DH2_CORE_HEADERS)
#include "gameswf/gameswf.h"
#include "gameswf/gameswf_impl.h"
#include "gameswf/gameswf_movie_def.h"
#include "gameswf/gameswf_shape.h"
#endif

namespace gameswf {

// 0x759b84 / 0x759be4: gameswf::s_verbose_dlist, one byte in .bss (zero at load).
bool s_verbose_dlist = false;

// 0x759cdc: gameswf::s_texture_loader_callback, one word in .bss (zero at load).
texture_loader_callback s_texture_loader_callback = nullptr;

// 0x759b84 gameswf::get_verbose_dlist()
// binary: return (unsigned __int8) s_verbose_dlist;
bool get_verbose_dlist() {
	return s_verbose_dlist;
}

// 0x759be4 gameswf::set_verbose_dlist(bool)
// binary: s_verbose_dlist = verbose;
void set_verbose_dlist(bool verbose) {
	s_verbose_dlist = verbose;
}

// 0x759c74 gameswf::null_loader(stream*, int, movie_definition_sub*)
//   upstream: gameswf/gameswf_impl.cpp, empty body. Ledgered upstream; no body here.

// 0x759cdc gameswf::register_texture_loader_callback(glitch::video::ITexture* (*)(char const*, int, int))
// binary: s_texture_loader_callback = callback; the function returns nothing.
void register_texture_loader_callback(texture_loader_callback callback) {
	s_texture_loader_callback = callback;
}

// 0x759d9c gameswf::define_debug_id_loader(gameswf::stream*, int, gameswf::movie_definition_sub*)
// 0x759cfc gameswf::substitute_bitmap_character(gameswf::tu_string&, gameswf::bitmap_character_def*, gameswf::movie_definition_sub*)
// and the destruct<shape_character_def> / destruct<execute_tag> instances are below, behind
// VENDOR_GAMESWF_DH2_CORE_HEADERS.

// 0x75a4b8 gameswf::define_video_loader(gameswf::stream*, int, gameswf::movie_definition_sub*)
// Thunk in the binary: forwards to stream::read_u16 and drops the value (no video character is created).
// Replaces the upstream body in gameswf/gameswf_impl.cpp, which is disabled with #if 0.
void define_video_loader(stream* in, int tag_type, movie_definition_sub* m) {
	(void)tag_type;
	(void)m;
	in->read_u16();
}

// 0x75fbf8 gameswf::define_native_image_loader(gameswf::stream*, int, gameswf::movie_definition_sub*)
// PARTIAL. The header is read in the binary's order: u16 character id, u16 (first int argument of
// render::create_bitmap_info_native), u16 (second int argument), u8 (inflate flag), u32 (data size).
// MISSING, each a separate gap:
//   - the image data: membuf sized to the u32 value, then inflate_wrapper(tu_file, membuf, size) when the
//     flag is set, otherwise tu_file::read_fully(membuf, -1). inflate_wrapper is not in the tree.
//   - render::create_bitmap_info_native(int, int, membuf const*) (glitch backend, not in the tree).
//   - the movie definition's two fields at +0x18 and +0x1c (binary: a reference-counted pointer checked
//     through its flag byte at +4, and a second pointer passed to bitmap_character's constructor).
//   - bitmap_character(movie definition, bitmap_info*) constructor arguments, then
//     m->add_bitmap_character(character id, character) (vtable +0x9c, add_bitmap_character(int, bitmap_character_def*)).
void define_native_image_loader(stream* in, int tag_type, movie_definition_sub* m) {
	(void)tag_type;
	(void)m;
	Uint16 character_id = in->read_u16();
	Uint16 arg_a = in->read_u16();      // binary v7: first int of create_bitmap_info_native (name not recovered)
	Uint16 arg_b = in->read_u16();      // binary v8: second int of create_bitmap_info_native (name not recovered)
	Uint8 inflate_flag = in->read_u8(); // binary u8: nonzero means inflate
	Uint32 data_size = in->read_u32();  // binary u32: membuf size
	(void)character_id;
	(void)arg_a;
	(void)arg_b;
	(void)inflate_flag;
	(void)data_size;
	// MISSING: image data read and bitmap creation (see the comment above).
}

// 0x75cb14 gameswf::destruct<gameswf::character::custom>(gameswf::character::custom const*)
// PARTIAL, not written: gameswf::character::custom is not declared anywhere in the tree, so its members
// cannot be named. Binary (Hex-Rays and asm agree):
//   if (p) {
//     if (the tu_string at +0x4c has tag byte -1) free_internal(heap pointer at +0x58, capacity at +0x54);
//     array<gameswf::filter> at +0x3c: resize(0), then reserve(0);
//     free_internal(p, 0);
//   }
// Layout request: declare character::custom (its +0x3c array<filter> member and its +0x4c tu_string member).

}  // namespace gameswf

// 0x76493c gameswf::hash<int, void (*)(gameswf::stream*, int, gameswf::movie_definition_sub*), gameswf::fixed_size_hash<int> >::set_raw_capacity(int)
// (hash is global in the vendored source, so this specialization sits outside namespace gameswf.)
// The binary's table is the upstream layout: a table header of entry count (+0) and size mask (+4), then
// 16-byte entries (next_in_chain, hash value, key, value); next_in_chain == -2 marks empty, hash value -1 a tombstone.
// Differences from upstream base/container.h:1294 (min 16): the DH2 minimum is 4, and the power of two is
// 1 << (int)(logf(new_size - 1) / ln2 + 1.0f) in single precision, where ln2 is the float with bit pattern
// 0x3F317218 (the binary: MOV R1,#0x3F317218 before __aeabi_fdiv). That is 0.693147180559945f, the same value
// as LN_2 in base/utility.h. Hex-Rays prints it as 0.69315, which is a different float (0x3F317247).
// Explicit specialization for this instantiation only; declared in gameswf_dh2_runtime.hpp.
template <>
void hash<int, gameswf::loader_function, fixed_size_hash<int> >::set_raw_capacity(int new_size) {
	if (new_size <= 0) {
		// Binary: clear() and return.
		clear();
		return;
	}

	// Binary: for new_size == 1, log(0) gives -inf, which lands below the minimum of 4 (ARM shifts to 0 or 1).
	// The explicit guard gives the same result.
	int size = 4;
	if (new_size > 1) {
		// ln 2 as float, bit pattern 0x3F317218 (see the comment above the specialization).
		float q = std::logf(static_cast<float>(new_size - 1)) / 0.693147180559945f;
		int bits = static_cast<int>(q + 1.0f);
		unsigned int pow2 = (bits >= 0 && bits < 32) ? (1u << bits) : 0u;
		size = static_cast<int>(pow2);
		if (size < 4) {
			size = 4;  // DH2 minimum (upstream: 16)
		}
	}

	if (m_table != nullptr && size == m_table->m_size_mask + 1) {
		// Binary: no change in raw capacity; no work.
		return;
	}

	// Binary: malloc_internal(16 * size + 8, MemHint 0), then entry count = 0, size mask = size - 1, and
	// every entry's next_in_chain = -2.
	hash<int, gameswf::loader_function, fixed_size_hash<int> > new_hash;
	new_hash.m_table = static_cast<table*>(gameswf::malloc_internal(sizeof(table) + sizeof(entry) * size, swf::MEM_UNRECOVERED));
	new_hash.m_table->m_entry_count = 0;
	new_hash.m_table->m_size_mask = size - 1;
	for (int i = 0; i < size; i++) {
		new_hash.E(i).m_next_in_chain = -2;
	}

	if (m_table != nullptr) {
		// Binary: move the live entries (not empty, not tombstone) into the new table, clear each old entry,
		// then free the old table with size sizeof(table) + sizeof(entry) * (mask + 1) (16 * mask + 24 on ARM).
		for (int i = 0, n = m_table->m_size_mask; i <= n; i++) {
			entry* e = &E(i);
			if (!e->is_empty() && !e->is_tombstone()) {
				new_hash.add(e->first, e->second);
				e->clear();
			}
		}
		gameswf::free_internal(m_table, static_cast<unsigned int>(sizeof(table) + sizeof(entry) * (m_table->m_size_mask + 1)));
	}

	// Binary: steal the new table; the local hash holds nothing afterwards.
	m_table = new_hash.m_table;
	new_hash.m_table = nullptr;
}

namespace gameswf {

// ---- destruct<T> / destructp<T> ----
// Binary bodies (asm checked):
//   destruct<T>(p):  if (p) { p->vtable slot 0 (virtual destructor); free_internal(p, 0); }
//   destructp<unsigned char>(p, n): if (p) free_internal(p, 0); the second argument is not read.

template <class T>
void destruct(const T* p) {
	if (p != nullptr) {
		p->~T();
		gameswf::free_internal(const_cast<T*>(p), 0);
	}
}

template <class T>
void destructp(const T* p, unsigned int /*count*/) {
	if (p != nullptr) {
		gameswf::free_internal(const_cast<T*>(p), 0);
	}
}

// 0x75a02c gameswf::destructp<unsigned char>(unsigned char const*, unsigned int)
template void destructp<unsigned char>(const unsigned char* p, unsigned int count);

// 0x75a1bc gameswf::destruct<gameswf::image_rgb>(gameswf::image_rgb const*)
// gameswf::image_rgb is the Gameloft class (vendor-gameswf/image_rgb.hpp, derived from gameswf::image_base).
template void destruct<gameswf::image_rgb>(const gameswf::image_rgb* p);

// 0x75a1e8 gameswf::destruct<gameswf::image_rgba>(gameswf::image_rgba const*)
// gameswf::image_rgba is the Gameloft class (vendor-gameswf/image_rgba.hpp, derived from gameswf::image_base).
template void destruct<gameswf::image_rgba>(const gameswf::image_rgba* p);

// 0x75a214 gameswf::destruct<gameswf::ref_counted>(gameswf::ref_counted const*)
template void destruct<ref_counted>(const ref_counted* p);

// 0x763dc0 gameswf::destruct<gameswf::jpeg::input>(gameswf::jpeg::input const*)
template void destruct<jpeg::input>(const jpeg::input* p);

#if defined(VENDOR_GAMESWF_DH2_CORE_HEADERS)
// ---- PARTIAL: written, not compiled in the tree yet (needs gameswf.h / gameswf_impl.h / gameswf_shape.h) ----

// 0x759d9c gameswf::define_debug_id_loader(gameswf::stream*, int, gameswf::movie_definition_sub*)
// PARTIAL. Reads the 16-byte debug id when the movie definition is a movie_definition_sub (is(AS_MOVIE_DEF_SUB),
// vtable +0x08). MISSING: the binary stores the 16 bytes at movie_definition_sub +0xf4 (244). The upstream
// class does not declare that member, so the store is not written. The binary's int result (the read's
// return value on the tail) is dropped because the loader table is void-typed.
void define_debug_id_loader(stream* in, int tag_type, movie_definition_sub* m) {
	(void)tag_type;
	if (m == nullptr || !m->is(AS_MOVIE_DEF_SUB)) {
		return;
	}
	uint8_t debug_id[16];
	in->get_underlying_stream()->read_bytes(debug_id, 16);
	// MISSING: store debug_id into the movie definition's field at +0xf4 (layout request for movie_definition_sub).
}

// 0x759cfc gameswf::substitute_bitmap_character(gameswf::tu_string&, gameswf::bitmap_character_def*, gameswf::movie_definition_sub*)
// Binary: the third argument is not read. Name bytes come from the string's data (c_str here). When the
// callback is set, get_bitmap_info() (bitmap_character_def vtable +0x2c), then get_width() (+0x24) and
// get_height() (+0x28) on the bitmap info are passed to the callback, and a non-null texture is set with
// set_texture() (+0x20). The binary's return value is not a documented result; the function returns void.
void substitute_bitmap_character(::tu_string& name, bitmap_character_def* ch, movie_definition_sub* /*m*/) {
	if (s_texture_loader_callback == nullptr) {
		return;
	}
	bitmap_info* bi = ch->get_bitmap_info();
	int width = bi->get_width();
	int height = bi->get_height();
	glitch::video::ITexture* texture = s_texture_loader_callback(name.c_str(), width, height);
	if (texture != nullptr) {
		bi->set_texture(texture);
	}
}

// 0x75fbf8 is above (partial, compiled). 0x75cb14 is a comment block above.

// 0x7613b8 gameswf::destruct<gameswf::shape_character_def>(gameswf::shape_character_def const*)
template void destruct<shape_character_def>(const shape_character_def* p);

// 0x763e6c gameswf::destruct<gameswf::execute_tag>(gameswf::execute_tag const*)
template void destruct<execute_tag>(const execute_tag* p);
#endif  // VENDOR_GAMESWF_DH2_CORE_HEADERS

}  // namespace gameswf
