// Batch v1-vendor-gameswf-gameswf_character-670fee-0: gameswf::character, part 1 of 3.
//
// Two kinds of row are in this batch.
//
// 1. Upstream rows. The DH2 body is the upstream inline definition in
//    vendor-gameswf/upstream/gameswf-r1714/gameswf/gameswf_character.h (empty, or
//    assert(0) compiled out in the release build). Nothing is redefined here, since a
//    second definition of an inline member would not compile. The ledger rows point
//    at the upstream header.
//
// 2. Gameloft rows. The DH2 binary has a body that upstream does not have, or a
//    different one. Those are defined below. Their declarations, and the DH2 members
//    they use (character::custom, m_custom, m_local_matrix, m_matrix_changed), are in
//    the upstream header with // DH2: comments. The upstream inline set_matrix and the
//    move/replace display-object overloads are wrapped in #if 0 there.
//
// Layout note: set_matrix stores the local matrix in a lazily allocated custom block,
// which the upstream member m_matrix does not reflect. The upstream get_matrix() still
// reads m_matrix, so the two disagree until the integrator reconciles the layout.

#include "gameswf/gameswf_character.h"

namespace gameswf
{

// Upstream rows: no body here, the inline definition in gameswf_character.h is the DH2 body.
// 0x752ca8 gameswf::character::set_play_state(gameswf::character::play_state)   upstream inline, bx lr
// 0x752cc4 gameswf::character::clear_display_objects()   upstream inline, bx lr
// 0x752cc8 gameswf::character::remove_display_object(gameswf::character*)   upstream inline, assert(0) compiled out
// 0x752cf0 gameswf::character::remove_display_object(int, int)   upstream inline, bx lr
// 0x752cf4 gameswf::character::execute_frame_tags(int, bool)   upstream inline, bx lr
// 0x752cf8 gameswf::character::add_action_buffer(gameswf::action_buffer*)   upstream inline, assert(0) compiled out
// 0x752cfc gameswf::character::do_actions(gameswf::array<gameswf::action_buffer*> const&)   upstream inline, assert(0) compiled out
// 0x752d08 gameswf::character::remove_display_object(gameswf::tu_string const&)   upstream inline, assert(0) compiled out
// 0x752d10 gameswf::character::stop_drag()   upstream inline, assert(0) compiled out
// 0x752d14 gameswf::character::call_frame_actions(gameswf::as_value const&)   upstream inline, assert(0) compiled out
// 0x752d18 gameswf::character::set_background_color(gameswf::rgba const&)   upstream inline, bx lr
// 0x752d1c gameswf::character::set_background_alpha(float)   upstream inline, bx lr
// 0x752d28 gameswf::character::set_display_viewport(int, int, int, int)   upstream inline, bx lr

// 0x4121f8 _ZN7gameswf9character10set_matrixERKNS_6matrixE
// Hex-Rays: if this+0x54 is NULL, operator new(0x6c) and character::custom::custom();
// then the 24-byte matrix is copied to custom+0x20, this+0x4c is set to custom+0x20, and
// this+0x99 is set to 1. The DH2 body returns void. The int in Hex-Rays is r0 left over
// from the copy, not a return value.
void character::set_matrix(const matrix& m)
{
	if (m_custom == nullptr)  // +0x54
	{
		m_custom = new custom();  // operator new(0x6c) + custom::custom()
	}
	m_custom->m_matrix = m;  // custom+0x20 (24 bytes)
	m_local_matrix = &m_custom->m_matrix;  // +0x4c
	m_matrix_changed = true;  // +0x99
}

// 0x752c48 _ZN7gameswf9character8hit_testEff
// Binary: MOV R0,#0; BX LR. Always "no hit". Hex-Rays types the result int; it is declared
// bool here like the sibling point_test_local.
bool character::hit_test(float x, float y)
{
	return false;
}

// 0x752ce4 _ZN7gameswf9character19move_display_objectEiPKNS_6cxformEPKNS_6matrixEPKNS_6effectEft
// Binary: BX LR. Empty in the base class; the work is done by subclasses.
void character::move_display_object(int depth, const cxform* color_transform, const matrix* mat, const effect* fx, float ratio, Uint16 clip_depth)
{
}

// 0x752ce8 _ZN7gameswf9character22replace_display_objectEtPKciPKNS_6cxformEPKNS_6matrixEPKNS_6effectEft
// Binary: BX LR. Empty in the base class.
void character::replace_display_object(Uint16 character_id, const char* name, int depth, const cxform* color_transform, const matrix* mat, const effect* fx, float ratio, Uint16 clip_depth)
{
}

// 0x752cec _ZN7gameswf9character22replace_display_objectEPS0_PKciPKNS_6cxformEPKNS_6matrixEPKNS_6effectEft
// Binary: BX LR. Empty in the base class.
void character::replace_display_object(character* ch, const char* name, int depth, const cxform* color_transform, const matrix* mat, const effect* fx, float ratio, Uint16 clip_depth)
{
}

// 0x752d0c _ZN7gameswf9character14set_drag_stateERKNS0_10drag_stateE
// Binary: BX LR. Empty in the base class.
void character::set_drag_state(const drag_state& state)
{
}

// 0x752d2c _ZN7gameswf9character18set_display_boundsEiiii
// Binary: BX LR. Empty in the base class.
void character::set_display_bounds(int x0, int y0, int x1, int y1)
{
}

}  // namespace gameswf
