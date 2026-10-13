// Batch v1-vendor-gameswf-pack6-a8bd9d-0 (category vendor-gameswf, 20 functions in one part).
//
// Owners and where their layouts live:
//   gameswf::as_stage              include/vendor-gameswf/as_stage.hpp (new, Gameloft)
//   gameswf::player_context        include/vendor-gameswf/player_context.hpp (new, Gameloft)
//   gameswf::render_cache          include/vendor-gameswf/render_cache.hpp (new, Gameloft)
//   gameswf::render::bogus_bi      gameswf/gameswf_render.h (DH2 declaration, upstream definition #if 0)
//   gameswf::render (3 factories)  gameswf/gameswf_render.h (DH2 declarations); create_bitmap_info_empty
//                                  replaces the upstream one in gameswf_render.cpp (#if 0)
//   gameswf::string_hash<U> and stringi_hash<U> dtors, hash<...>::set_raw_capacity
//                                  upstream base/container.h (templates)
//
// Not reconstructed here (see ledger rows for the reason): the bodies of as_stage::get_member,
// player_context destructors, render_cache::is_valid and ~render_cache (partial), and
// hash<tu_stringi, as_standard_member, ...>::set_raw_capacity (partial, no body). The deleting destructors
// (0x76d490, 0x76d578, 0x773cf8) are compiler-generated and skipped.

#include <cstdint>

#include "base/container.h"
#include "gameswf/gameswf.h"
#include "gameswf/gameswf_render.h"
#include "vendor-gameswf/as_stage.hpp"  // the class is defined only when DH2_AS_OBJECT_HEADER_OK is set
#include "vendor-gameswf/glyph_provider.hpp"
#include "vendor-gameswf/player_context.hpp"
#include "vendor-gameswf/ref_counted.hpp"  // gameswf::free_internal
#include "vendor-gameswf/render_cache.hpp"

namespace gameswf {

// ---------------------------------------------------------------------------------------------------------------
// gameswf::as_stage
// BLOCKED: everything in this section is compiled only with DH2_AS_OBJECT_HEADER_OK, because gameswf_object.h does
// not compile in this tree (see as_stage.hpp). The rows are partial until the build can include it.
// ---------------------------------------------------------------------------------------------------------------

#if defined(DH2_AS_OBJECT_HEADER_OK)

// 0x76c728 gameswf::as_stage::is(int) const
// Returns true for class id 0 (AS_OBJECT, the base id) and for 21, the binary's id for as_stage. The id 21 has no
// enumerator in gameswf.h (as_classes), so it is written as a literal.
bool as_stage::is(int class_id) const {
    return class_id == 21 || class_id == AS_OBJECT;
}

// 0x76ccbc gameswf::as_stage::~as_stage()   [D1, complete-object destructor]
// The binary stores the as_stage vtable and then calls gameswf::as_object::~as_object. C++ runs the base
// destructor after this body and sets the vptr itself, so the body is empty.
as_stage::~as_stage() {
}

// 0x76d490 gameswf::as_stage::~as_stage()   [D0, deleting destructor]: not rebuilt.
// Compiler-generated: runs D1, then operator delete (LC_API_FREE_0 in the binary). C++ delete produces it.

// 0x76d724 gameswf::as_stage::get_member(gameswf::tu_stringi const&, gameswf::as_value*)
// PARTIAL. The name dispatch matches the binary: a case-insensitive comparison with "width", then with
// "height"; any other name returns 0. For those names the binary sets *val to the root movie's get_width() or
// get_height() and returns 1. The root movie is the player's weak pointer at player +0x48 (proxy) and +0x4c
// (cached character). The player layout is not in the tree, so the value is not produced here and the call
// returns 0 for both names. The binary's dead-proxy branch (weak pointer released, then a second read through
// a null pointer in the decompiled code) is unresolved, which is one more reason this stays partial.
bool as_stage::get_member(const tu_stringi& name, as_value* val) {
    (void)val;
    const char* s = name.c_str();
    const bool is_width = tu_string::stricmp(s, "width") == 0;
    const bool is_height = !is_width && tu_string::stricmp(s, "height") == 0;
    if (!is_width && !is_height) {
        return false;
    }
    return false;
}

// 0x76f018 gameswf::as_stage::as_stage(gameswf::player*)
// Runs the as_object constructor, sets the vptr, then sets the members "width" and "height" to the number 1.
// The binary builds each value in a temporary as_value (type 2, number 1.0) and drops its references after the
// call. This body uses one value for both calls, which has the same effect.
as_stage::as_stage(player* p) : as_object(p) {
    as_value one;
    one.set_double(1.0);
    as_object::set_member(tu_stringi("width"), one);
    as_object::set_member(tu_stringi("height"), one);
}

#endif  // DH2_AS_OBJECT_HEADER_OK

// ---------------------------------------------------------------------------------------------------------------
// gameswf::player_context
// ---------------------------------------------------------------------------------------------------------------

// 0x76ce38 gameswf::player_context::player_context()   [C1]
// 0x76ce90 gameswf::player_context::player_context()   [C2]
// The binary runs the ref_counted constructor, then zeroes +0x0c, +0x10, +0x14 (array data), +0x18 (size),
// +0x1c (buffer size), +0x20 (byte), +0x24 and +0x28, and stores the vptr. The member initializers in the
// header do the same.
player_context::player_context() {
}

// 0x76cee8 gameswf::player_context::~player_context()   [D2, base-object destructor]
// 0x76d4cc gameswf::player_context::~player_context()   [D1, complete-object destructor]
// Same code for both. PARTIAL:
//   - The glyph provider at +0x0c: ~glyph_provider, then free. Done.
//   - The bitmap glyph provider at +0x10 is destroyed with gameswf::destruct<bitmap_glyph_provider> (0x76c9ac).
//     That type is absent from the tree, and the destruct template for it is not defined in this tree, so the
//     call is not made here and the object is not released by this function.
//   - unk_14 (array<player*>): when its size field is negative, the binary zeroes the elements from the size up
//     to -1. The upstream array never has a negative size, so that loop is not reproduced. The normal path (size
//     0 or more) is the array's clear(), which matches the binary's reserve(0) with the size set to 0.
player_context::~player_context() {
    if (m_glyph_provider) {
        m_glyph_provider->~glyph_provider();
        free_internal(m_glyph_provider, 0);
    }
    unk_14.clear();
}

// 0x76d578 gameswf::player_context::~player_context()   [D0, deleting destructor]: not rebuilt.
// Compiler-generated: runs D1, then operator delete (LC_API_FREE_0 in the binary).

// ---------------------------------------------------------------------------------------------------------------
// hash and string_hash templates (upstream base/container.h)
// ---------------------------------------------------------------------------------------------------------------

// 0x76d9d0 gameswf::string_hash<gameswf::tu_loadlib*>::~string_hash()
// Upstream: the implicit destructor of string_hash<U> runs hash::~hash(), which calls clear() (container.h).
// The binary calls hash<tu_string, tu_loadlib*, string_hash_functor<tu_string>>::clear directly, which is the
// same code after inlining. No DH2 body. Ledger row: upstream.

// 0x76dba8 gameswf::stringi_hash<gameswf::as_standard_member>::~stringi_hash()
// Upstream: same as string_hash above, with stringi_hash<U> and clear(). No DH2 body. Ledger row: upstream.

// 0x76dbd0 gameswf::hash<gameswf::tu_stringi, gameswf::as_standard_member, gameswf::stringi_hash_functor<gameswf::tu_stringi>>::set_raw_capacity(int)
// PARTIAL, no body. The binary's version differs from base/container.h hash::set_raw_capacity: it rounds the size
// up to a power of two with 1 << (int)(logf(n - 1) / ln 2 + 1.0), ln 2 the float 0x3F317218, its minimum table size is 4 (the upstream
// minimum is 16), it allocates 32 * size + 8 bytes with malloc_internal and 32-byte entries (next at +0, hash at
// +4, key tu_stringi at +8, value at +0x1c), and it frees the key's heap buffer when the key's first byte is -1.
// The upstream member is the one set_raw_capacity for every hash instantiation (add() calls it), and
// as_standard_member is also used by upstream code (gameswf_player.cpp and others). Replacing it for one
// instantiation would change the others, and the table member is private to hash. The binary body is not
// installed here. Needs a decision from the integrator (see the ledger row).

// ---------------------------------------------------------------------------------------------------------------
// gameswf::render_cache
// ---------------------------------------------------------------------------------------------------------------

// 0x7738a4 gameswf::render_cache::is_valid(gameswf::character*)
// PARTIAL. Returns false, so the caller rebuilds the cache. The binary: checks the character's weak player pointer
// (+0x2c/+0x30), reads player +0xac (a player_context*), calls get_world_cxform() and get_world_matrix() (results
// unused), then compares the texture cache's m_last_clock (glyph provider +0x28 -> texture_cache +0x08) and the
// second clock (the object at bitmap glyph provider +0x0c, its words at +0x08 and +0x0c). The comparison itself is dh2::render_cache_clocks_valid in
// render_cache_clocks.hpp (tested). Not reproduced: player +0xac and the bitmap glyph provider are not in the tree.
bool render_cache::is_valid(character* ch) {
    (void)ch;
    return false;
}

// 0x78ba28 gameswf::render_cache::~render_cache()
// PARTIAL. Order and effects of the binary: unk_40 (array<uint16_t>) is cleared; the owned block at +0x30 has its
// +0x04 cleared, and when its flag at +0x3c is zero, its +0x08 is cleared, the block is freed and its pointer
// cleared; the same for the block at +0x20 (flag at +0x2c); then unk_10 (array<entry>) is cleared. Not reproduced:
// when a size field is negative, the binary zeroes the elements from the size up to -1 (the same pattern as
// player_context; the upstream array never has a negative size).
render_cache::~render_cache() {
    unk_40.clear();

    unk_30.unk_04 = 0;
    if (unk_30.flag_0c == 0) {
        unk_30.unk_08 = 0;
        if (unk_30.unk_00) {
            free_internal(unk_30.unk_00, 0);
        }
        unk_30.unk_00 = nullptr;
    }

    unk_20.unk_04 = 0;
    if (unk_20.flag_0c == 0) {
        unk_20.unk_08 = 0;
        if (unk_20.unk_00) {
            free_internal(unk_20.unk_00, 0);
        }
        unk_20.unk_00 = nullptr;
    }

    unk_10.clear();
}

// ---------------------------------------------------------------------------------------------------------------
// gameswf::render::bogus_bi and the render factories
// ---------------------------------------------------------------------------------------------------------------

namespace render {

// 0x773974 gameswf::render::bogus_bi::~bogus_bi()   [D1, complete-object destructor]
// The binary stores a vtable and calls gameswf::ref_counted::~ref_counted. C++ does the same through the base
// destructors, so the body is empty.
bogus_bi::~bogus_bi() {
}

// 0x773cf8 gameswf::render::bogus_bi::~bogus_bi()   [D0, deleting destructor]: not rebuilt.
// Compiler-generated: runs D1, then operator delete (LC_API_FREE_0 in the binary).

// 0x773c2c gameswf::render::create_bitmap_info_empty()
// Replaces the upstream definition (gameswf_render.cpp, #if 0). With a render handler it asks the handler for a
// 2 x 2 alpha bitmap (virtual slot +0x0c, create_bitmap_info_alpha) whose data is the int 255, so the bytes are
// ff 00 00 00 on a little-endian target. Without a handler it returns a new bogus_bi. The binary uses its own
// operator new with a memory hint; a plain new gives the same object.
bitmap_info* create_bitmap_info_empty() {
    int pixel = 255;
    if (render_handler* handler = get_render_handler()) {
        return handler->create_bitmap_info_alpha(2, 2, reinterpret_cast<unsigned char*>(&pixel));
    }
    return new bogus_bi;
}

// 0x7739dc gameswf::render::create_bitmap_info_native(int, int, gameswf::membuf const*)
// Forwards its three arguments to the handler's virtual slot +0x1c (create_bitmap_info_native). Without a
// handler it returns a new bogus_bi.
bitmap_info* create_bitmap_info_native(int a1, int a2, const membuf* a3) {
    if (render_handler* handler = get_render_handler()) {
        return handler->create_bitmap_info_native(a1, a2, a3);
    }
    return new bogus_bi;
}

// 0x773a5c gameswf::render::create_bitmap_info_texture(glitch::video::ITexture*)
// Forwards the texture to the handler's virtual slot +0x18 (create_bitmap_info_texture). Without a handler it
// returns a new bogus_bi.
bitmap_info* create_bitmap_info_texture(glitch::video::ITexture* texture) {
    if (render_handler* handler = get_render_handler()) {
        return handler->create_bitmap_info_texture(texture);
    }
    return new bogus_bi;
}

}  // namespace render

}  // namespace gameswf
