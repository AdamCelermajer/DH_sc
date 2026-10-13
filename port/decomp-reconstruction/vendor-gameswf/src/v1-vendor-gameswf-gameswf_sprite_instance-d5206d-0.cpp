// v1-vendor-gameswf-gameswf_sprite_instance-d5206d-0.cpp
//
// Track B batch v1-vendor-gameswf-gameswf_sprite_instance-d5206d-0: gameswf::sprite_instance, part 1 of 2.
// Gameloft (DH2) bodies for the r1714 class in
// vendor-gameswf/upstream/gameswf-r1714/gameswf/gameswf_sprite.h.
//
// The DH2 sprite_instance does not match the r1714 layout. Its own members sit at other offsets
// (display list at +0xa8, action list at +0xbc, init-actions block at +0xdc, environment pointer at
// +0xe0, transform at +0xf4), and the DH2 character base is not declared in full. Functions that
// need those members have no body here: they are partial and the gap is named in the ledger notes
// and in the batch layout requests. Partial rows with a body (get_character, _typeof) are complete
// but are not compile-verified (see the batch report).
//
// Upstream rows: no body here, the upstream definition stands.
//   0x77e874 add_action_buffer   gameswf_sprite.cpp, m_action_list.push_back(a)
//   0x77f614 has_keypress_event  gameswf_sprite.cpp, get_member("onKeyPress", &unused)
//
// Upstream definitions wrapped in #if 0 in upstream/gameswf-r1714/gameswf/gameswf_sprite.h:
//   get_character (inline body, search of the display list)

#include "gameswf/gameswf_sprite.h"

namespace gameswf
{

// 0x77de28 _ZN7gameswf15sprite_instance13get_characterEi
// Virtual override (vtable +0x7c). Returns null without searching the display list. The base
// character::get_character also returns NULL (gameswf_character.h:244).
character* sprite_instance::get_character(int character_id)
{
	return nullptr;
}

// 0x77de68 _ZN7gameswf15sprite_instance7_typeofEv
// Virtual (vtable +0x18). The ActionScript type name, the same string as upstream type_of().
const char* sprite_instance::_typeof()
{
	return "movieclip";
}

// 0x77df9c _ZN7gameswf15sprite_instance22get_frameid_from_labelERKNS_10tu_stringiE
// Partial, no body. Sets the result to -1, calls m_def's virtual slot +0x60 with the label and &result,
// returns the result when the call succeeds and -1 otherwise. Gap: movie_definition_sub's slot +0x60 is
// pure in the DH2 vtable (decomp_vtable.py), so the implementation is not identified, and the call
// passes the tu_stringi object where upstream get_labeled_frame takes const char*.

// 0x77e02c _ZN7gameswf15sprite_instance14get_drag_stateEPNS_9character10drag_stateE
// Partial, no body. Copies eight words from root +0x58 (m_root at +0xa4) into *ds, then returns the
// fifth word (root +0x68). Gap: the DH2 root layout (root drag-state field, 32 bytes at +0x58) and the
// DH2 drag_state field order. Upstream root has m_drag_state (gameswf_root.h:76) but the offset is not
// verified.

// 0x77e184 _ZN7gameswf15sprite_instance19update_world_cxformEv
// Partial, no body. Calls the base character::update_world_cxform() (not declared in the DH2
// gameswf_character.h), then calls update_world_cxform() (vtable +0x11c) on every non-null entry of the
// display list: items at this+0xa8, count at this+0xac. Gap: the DH2 display_list layout and the base
// declaration (layout requests L2, L3).

// 0x77e1d0 _ZN7gameswf15sprite_instance19update_world_matrixEv
// Partial, no body. Calls the base character::update_world_matrix() (not declared in the DH2
// gameswf_character.h), then update_world_matrix() (vtable +0x118) on every non-null display-list entry
// (same loop as update_world_cxform). Returns the value of the last loop step (the last entry's call
// result, or the entry itself when it is null), int in the binary. Gap: the same as update_world_cxform
// (layout requests L2, L3).

// 0x77e874 _ZN7gameswf15sprite_instance17add_action_bufferEPNS_13action_bufferE
// Upstream: gameswf_sprite.cpp, m_action_list.push_back(a). No body here.

// 0x77efd8 _ZN7gameswf15sprite_instance9constructEv
// Partial, no body. If the construct flag at +0xeb is clear: instanciate the registered class
// (character_def::instanciate_registered_class(m_def, this), not declared), call display_list::construct
// on the display list at +0xa8, and set the flag. Gap: the flag, instanciate_registered_class and the
// DH2 display_list (layout requests L1, L3, L8).

// 0x77f614 _ZN7gameswf15sprite_instance18has_keypress_eventEv
// Upstream: gameswf_sprite.cpp, get_member("onKeyPress", &unused) through the virtual get_member
// (vtable +0x20). The binary makes the same call. No body here.

// 0x77f8d0 _ZNK7gameswf15sprite_instance12get_variableEPKc
// Partial, no body; the upstream definition in gameswf_sprite.cpp stays in place. The DH2 body reads
// the environment through get_environment() (vtable +0x58), not m_as_environment, and calls
// as_environment::get_variable with a result block and a third argument (passed 0). The DH2 header
// declares two arguments (gameswf_environment.h:179). The result goes through a function-local static
// as_value, and the return value is its to_string(). Gap: the DH2 as_environment::get_variable
// signature (layout request L6).

// 0x77fdd0 _ZN7gameswf15sprite_instance14set_drag_stateERKNS_9character10drag_stateE
// Partial, no body. Copies eight words from the argument into root +0x58 (m_root at +0xa4). If the
// first word (the drag target) is non-null, calls notify_need_advance() on it (declared in the DH2
// gameswf_character.h:464). Gap: the root drag-state field (layout request L4).

// 0x77fe60 _ZN7gameswf15sprite_instance17notify_set_memberERKNS_10tu_stringiERKNS_8as_valueE
// Partial, no body. Hex-Rays lost the as_value argument (only the name is in the signature). If the
// name is exactly "onEnterFrame", sets the sprite flag at +0xe9 and calls notify_need_advance(). If the
// name starts with "on", compares it case-insensitively with the eight gameswf::FN_NAMES entries
// (stride 4, offsets 0..28); on a match sets the character flag at +0x9c; no match returns. Gap: the
// flags (layout requests L1, L2, L10).

// 0x77ff40 _ZN7gameswf15sprite_instance13get_transformEv
// Partial, no body. Returns the cached as_transform at +0xf4. If none, refreshes the player link
// (proxy +0x2c, player* +0x30), creates an as_transform (0x40 bytes, as_transform(player*, character*)),
// and stores it in the smart pointer at +0xf4. Gap: the as_transform constructor and the DH2 member
// (layout requests L1, L7).

// 0x780594 _ZN7gameswf15sprite_instance10this_aliveEv
// Partial, no body. Clears the player link when its proxy's flag byte (+4) is clear. Then, when the
// character's snapshot word (unk_34, set from player +0x30 by player::create_*) differs from the player's
// garbage count, calls as_object::this_alive(this). Then, for each non-null display-list entry whose own
// snapshot word differs, calls its this_alive() (vtable +0x44). The binary reads player +0x30 after the
// link is cleared, and Hex-Rays shows no null test there; unresolved. Gap: layout requests L1, L2, L3.

// 0x78069c _ZN7gameswf15sprite_instance15do_init_actionsEv
// Partial, no body. If the init-actions block at +0xdc is set: add_ref, execute_actions(get_environment(),
// *block), clear the block array, drop_ref. Gap: the nested type sprite_instance::init_actions (array of
// action_buffer* plus members at +0x0c, +0x10 and +0x1c; layout request L9).

// 0x780718 _ZN7gameswf15sprite_instanceC1EPNS_6playerEPNS_20movie_definition_subEPNS_4rootEPNS_9characterEi
// Partial, no body. The DH2 constructor sets the character base with the four-argument form
// character(player, parent, id, 2), then m_def (+0xa0, add_ref when set), m_root (+0xa4), the sprite
// flags and zeroed members. When m_def's virtual slot +0x88 (unnamed in the DH2 vtable) returns non-zero,
// it builds the init-actions block sized from m_def->get_frame_count() (slot +0x38). It ends with
// set_ctor(player + 0x88). Gap: the four-argument character constructor (not declared, layout request L2),
// the DH2 sprite layout (L1), the init-actions type (L9), the player member at +0x88 (L11). The upstream
// constructor in gameswf_sprite.cpp:32 stays in place.

// 0x780bb0 _ZN7gameswf15sprite_instance15get_environmentEv
// Partial, no body. Returns the environment at +0xe0. If none, creates an as_environment (0x6c bytes,
// as_environment(player*) with the player link), stores it and calls set_target(this). Gap: the DH2
// member at +0xe0 and the as_environment constructor (layout requests L1, L6). The upstream inline
// (return &m_as_environment) in gameswf_sprite.h stays in place.

// 0x780f3c _ZN7gameswf15sprite_instance7recycleEPNS_9characterEi
// Partial, no body. Calls the base character::recycle(parent, id) (DH2 declared, body partial in the
// character batch), then destroys the init-actions block (0x780ea0, another batch) and the environment,
// resets the sprite flags (+0xe7, +0xea, +0xeb, +0xec), clears the two action arrays (+0xbc, +0xcc),
// resets the smart pointers at +0xf0 and +0xf4, and clears and frees the frame-script hash at +0xf8.
// Gap: layout requests L1 to L9.

// 0x781074 _ZN7gameswf15sprite_instanceC2EPNS_6playerEPNS_20movie_definition_subEPNS_4rootEPNS_9characterEi
// Partial, no body. C2 variant with the same body as C1 (0x780718). Gap: as for C1.

// 0x78146c _ZN7gameswf15sprite_instanceD2Ev
// Partial, no body. The DH2 destructor clears and frees the frame-script hash (+0xf8), destroys the
// init-actions block (+0xdc) and the environment (+0xe0), drops the frame script (+0xfc), and releases
// the smart pointers at +0xf4 and +0xf0. It then clears the goto-frame list (+0xcc) and the action list
// (+0xbc), destroys the display list at +0xa8, drops m_def (+0xa0), and calls character::~character.
// Upstream destructor (gameswf_sprite.cpp:60, delete m_script) stays in place. Gap: layout requests L1,
// L2, L3, L8, L9.

}  // namespace gameswf
