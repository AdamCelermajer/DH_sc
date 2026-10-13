// gameswf::as_transform as the DH2 binary has it (Gameloft). The upstream class (gameswf_as_classes/as_transform.h) has
// gc_ptr members, m_color_transform and m_movie, that the binary does not have. The binary keeps the movie clip in a
// weak pointer and creates its colorTransform and matrix objects when the constructor runs init().
//
// Layout, 32-bit offsets (host pointers are 8 bytes, so the offsets are comments):
//   +0x00 vptr, as_object base (inherited; as_object::m_player is the weak_ptr<player> at +0x2c proxy, +0x30 player)
//   +0x38 m_movie.proxy  smart_ptr<weak_proxy> (weak_ptr<character>)
//   +0x3c m_movie.ptr    character*, valid while the proxy is alive
//
// BLOCKED: gameswf/gameswf_object.h does not compile in this tree (see as_stage.hpp). The class is defined only when the
// build sets DH2_AS_OBJECT_HEADER_OK. Until then the bodies in v1-vendor-gameswf-pack14-1a21ee-0.cpp are not compiled.
//
// Bodies: vendor-gameswf/src/v1-vendor-gameswf-pack14-1a21ee-0.cpp

#pragma once

#if defined(DH2_AS_OBJECT_HEADER_OK)

#include "base/weak_ptr.h"                    // weak_ptr<T> (upstream)
#include "gameswf/gameswf_object.h"           // as_object (upstream, with DH2 declarations)

namespace gameswf {

struct as_transform : public as_object {
    // Unique id of the class (the upstream header has the same enum). is() compares it with the class id argument.
    enum { m_class_id = AS_TRANSFORM };

    // 0x7a7a8c (C1) and 0x7a7bd8 (C2): one body in C++. Sets m_movie and calls init().
    as_transform(player* p, character* movie_clip);

    // 0x7a72e8 (D1). The deleting destructor 0x7a7344 is compiler-generated.
    ~as_transform() override;

    // Vtable +0x10 (overrides as_object::is(int) const, 0x768b44). Body 0x7a72d0 is not in the tree yet. The ledger row
    // that names upstream as_transform::is is a name match only, since the upstream class has the gc_ptr layout.
    bool is(int class_id) const override;

    // Vtable +0x24 (overrides as_object::set_member(tu_stringi const&, as_value const&), 0x76c1e0). Body 0x7a7360 is not
    // in the tree yet.
    bool set_member(const tu_stringi& name, const as_value& val) override;

    // Vtable +0x28 (overrides as_object::get_member(tu_stringi const&, as_value*), 0x769154). Body 0x7a74b4 is not in the
    // tree yet.
    bool get_member(const tu_stringi& name, as_value* val) override;

private:
    // 0x7a75c0. Builds the four members colorTransform, concatenatedColorTransform, matrix and concatenatedMatrix.
    void init();

    weak_ptr<character> m_movie;  // +0x38 (proxy), +0x3c (character)
};

}  // namespace gameswf

#endif  // DH2_AS_OBJECT_HEADER_OK
