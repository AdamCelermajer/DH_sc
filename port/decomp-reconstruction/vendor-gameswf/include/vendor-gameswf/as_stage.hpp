// gameswf::as_stage (Gameloft; upstream GameSWF r1714 has no such class).
//
// The Stage object: an as_object with no members of its own. Its width and height read the root movie of
// the player through the inherited weak pointer as_object::m_player (+0x2c proxy, +0x30 cached player).
// The constructor creates the two members "width" and "height" with the number 1.
//
// Virtual slots (vptr offsets): +0x00 D1, +0x04 D0, +0x08 is(int) const, +0x20 get_member(tu_stringi, as_value*).
// The rest is inherited from as_object (gameswf_object.h).
//
// BLOCKED: gameswf/gameswf_object.h does not compile in this tree (gameswf_environment.h:255 uses an incomplete
// as_environment, and gc_ptr<as_object> needs the gc base that DH2 replaced). The class is therefore defined only
// when DH2_AS_OBJECT_HEADER_OK is set by the build. Until then the batch's as_stage bodies are not compiled.
//
// Bodies: vendor-gameswf/src/v1-vendor-gameswf-pack6-a8bd9d-0.cpp

#pragma once

#if defined(DH2_AS_OBJECT_HEADER_OK)

#include "gameswf/gameswf_object.h"  // as_object (upstream, with DH2 declarations)

namespace gameswf {

struct as_stage : public as_object {
    explicit as_stage(player* p);                  // 0x76f018
    ~as_stage() override;                          // 0x76ccbc (D1); 0x76d490 (D0) is compiler-generated

    bool is(int class_id) const override;          // 0x76c728
    bool get_member(const tu_stringi& name, as_value* val) override;  // 0x76d724, partial
};

}  // namespace gameswf

#endif  // DH2_AS_OBJECT_HEADER_OK
