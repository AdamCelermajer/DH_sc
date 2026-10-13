// v1-vendor-gameswf-gameswf_character-670fee-2.cpp
//
// Track B batch v1-vendor-gameswf-gameswf_character-670fee-2: gameswf::character, part 3 of 3.
// Gameloft (DH2) bodies for the r1714 class in
// vendor-gameswf/upstream/gameswf-r1714/gameswf/gameswf_character.h.
//
// The DH2 character does not match the r1714 layout. Its local colour transform, matrix and
// effect are pointers that default to shared identities (+0x48, +0x4c, +0x50), the custom block
// sits at +0x54, and the world caches are inline (+0x58 cxform, +0x78 matrix). Functions that need those
// members have no body here. They are marked partial and their layout requests are listed in
// the ledger notes and the batch report. Header additions are marked "// DH2:" in
// gameswf_character.h.
//
// Rows in this batch that need no body here:
//   0x754d70 get_mouse_state   upstream (inline virtual in gameswf_character.h, slot 0x70)
//   0x7804d4 get_parent        upstream (inline in gameswf_character.h, weak_ptr::get_ptr)

#include "gameswf/gameswf_character.h"

namespace gameswf
{

// 0x754824 _ZN7gameswf9character14get_drag_stateEPNS0_10drag_stateE
// The parent's virtual slot 0xdc fills *ds and its return value is passed back. The chain ends
// at a sprite_instance (0x77e02c), which overrides the slot. The binary has no null check on the
// parent, and neither does this body: a parentless character reaches this only when it does not
// override the slot.
int character::get_drag_state(drag_state* ds)
{
	character* parent = get_parent();	// dead parent link cleared by the weak_ptr check (+0x3c/+0x40)
	return parent->get_drag_state(ds);
}

// 0x7750e8 _ZN7gameswf9character19notify_need_advanceEv
// Partial: the parent walk is written. The need-advance flag at +0x9d (layout request L17) is
// not declared yet, so the two flag stores are left out.
void character::notify_need_advance()
{
	// DH2: this->need_advance (+0x9d) = 1 is set before the walk. Not written: member not declared.
	character* node = this;
	while (character* parent = node->get_parent())	// get_parent() clears a dead link, as the binary does
	{
		// DH2: parent->need_advance (+0x9d) = 1. Not written: member not declared.
		node = parent;
	}
}

// 0x754a1c _ZN7gameswf9characterC1EPNS_6playerEPS0_ii
// 0x754b28 _ZN7gameswf9characterC2EPNS_6playerEPS0_ii
// Partial, no body. The DH2 constructor takes (player, parent, id, flag byte) and initializes the
// DH2 layout: as_object base, m_id (+0x38), weak parent (+0x3c), m_name (+0x44), identity pointers
// (+0x48 cxform, +0x4c matrix, +0x50 effect), custom = 0 (+0x54), identity world cxform (+0x58),
// identity world matrix (+0x78), zeroed words (+0x90..+0x96), flag bytes (+0x98..+0x9d).
// Needs layout requests L3 to L17.

// 0x754c34 _ZN7gameswf9character19update_world_matrixEv
// Partial, no body. Virtual slot 0x118. If the parent link is live, this+0x78 takes the parent's
// world matrix (the DH2 get_world_matrix returns a pointer); otherwise it is set to identity. Then
// this+0x78 is concatenated with the local matrix pointer (+0x4c) and the dirty flag (+0x99) is
// cleared. Needs L5, L9, L14 and get_world_matrix_root_change (0x753ee8, queued in this class).

// 0x75488c _ZN7gameswf9character19update_world_cxformEv
// Partial, no body. Virtual slot 0x11c. If the parent link is live, this+0x58 takes the parent's
// world cxform (the DH2 get_world_cxform returns a pointer); otherwise it is set to identity (mult 1,
// add 0 per channel). Then it is concatenated with the local cxform pointer (+0x48), unless that
// points at cxform::identity, and the dirty flag (+0x9a) is cleared. Needs L4, L8, L15 and
// get_world_cxform_root_change (0x753e34, queued in this class).

// 0x754958 _ZN7gameswf9character7recycleEPS0_i
// Partial, no body. Virtual slot 0x64. Sets the parent link and id (+0x38), zeroes +0x90, +0x94
// and +0x96, sets m_name to the empty name, resets the local matrix, cxform and effect pointers to
// their identities (setting the dirty flags when they change), and sets the flag bytes (+0x99,
// +0x9a, +0x9b, +0x9d). Needs L3 to L17.

// 0x755cf8 _ZN7gameswf9character10set_effectERKNS_6effectE
// Partial, no body. gameswf::effect is not in r1714 and is owned by another batch (pack1). The
// body allocates the custom block (+0x54) if it is absent, copies the effect's flags into custom+0x38
// and its filter array (44-byte elements, 11 words each) into custom+0x3c/+0x40, then points the
// effect pointer (+0x50) at custom+0x38. Needs L6, L7 and the effect type.

// 0x75dd74 _ZN7gameswf9characterD2Ev
// Partial, no body. Resets the vtable, destroys the custom block at +0x54 if present (L7), releases
// the parent proxy at +0x3c and runs as_object::~as_object. The upstream implicit destructor
// covers the last two steps. The custom destroy needs L7.

// 0x75e3a8 _ZN7gameswf9character13do_mouse_dragEv
// Partial, no body. Returns int: the result of the drag_state query when this is not the drag
// target. It reads the drag state through slot 0xdc, this character's world matrix through
// get_world_matrix (the centred branch also reads the parent's), the root movie through slot 0x134
// (get_root_movie) and its mouse state through slot 0x70, the scene node at custom+0x68
// (get_world_mouse), and it writes the drag state through
// slot 0xe0 and the matrix through set_matrix (0x4121f8). It uses the DH2 drag_state byte layout
// (layout request L18). Needs L7, L9 and L18.

}  // namespace gameswf
