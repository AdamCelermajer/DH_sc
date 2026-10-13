// Track B batch v1-vendor-gameswf-gameswf_character-670fee-1: gameswf::character, part 2 of 3.
//
// Gameloft (DH2) bodies for the r1714 class in
// vendor-gameswf/upstream/gameswf-r1714/gameswf/gameswf_character.h.
//
// Rows in this batch:
//   upstream        set_userdata, set_variable (both overloads), attach_display_callback, display
//                   (inline in gameswf_character.h, assert(0) compiled out in the DH2 build) and
//                   enumerate (tail call to as_object::enumerate, gameswf_character.cpp). No body here:
//                   a second definition of an inline member would not compile.
//   reconstructed   construct, get_world_cxform_root_change, get_world_matrix_root_change,
//                   get_root_movie.
//   partial         advance, goto_frame, detach_scene_node, attach_scene_node, set_display_callback,
//                   do_display_callback, set_cxform, set_name, ~character (D1). Their bodies need
//                   layout that is not declared yet (see the layout requests below), so they have no
//                   code here. Each comment block gives the binary behaviour.
//   skipped         ~character (D0), the compiler-generated deleting destructor.
//
// Header changes (gameswf_character.h, marked DH2 and batch -1):
//   - get_root_movie: the upstream const inline is wrapped in #if 0, and the binary's non-const
//     declaration is added (the mangled name has no K).
//   - construct (virtual), get_world_cxform_root_change and get_world_matrix_root_change are declared.
//   - m_cxform_changed (+0x9a) is declared next to m_matrix_changed (+0x99).
//
// Layout requests (not declared in this batch):
//   R1  gameswf::character::custom (108 bytes, owner gameswf::character::custom): +0x00 local colour
//       transform (32 bytes, 8 floats, mult/add pairs), +0x4c name (tu_string, 20 bytes), +0x60 display
//       callback void (*)(render_state&, void*), +0x64 user pointer, +0x68 gameswf::scene_node*.
//   R2  gameswf::character: +0x44 name pointer (const tu_string*; interned names are stored as the
//       argument's address, others as the custom block's name), +0x48 local cxform pointer (set_cxform
//       points it at custom+0x00).
//   R3  gameswf::character +0x9d: need-advance flag (advance clears it; notify_need_advance sets it).
//   R4  gameswf::scene_node (not in the vendored tree, gameswf_scene_node.cpp absent): child array
//       data at +0x238, count at +0x23c, capacity at +0x240 (gameswf::array<character*>), object size
//       0x270, static NODE_NAME, a constructor taking (int from character +0x30, ISceneNode*, width, height).
//   R5  gameswf::render_state (not in the tree): 60 bytes. character* at +0x00, matrix (6 floats) at
//       +0x04, cxform (8 floats) at +0x1c. Inferred from do_display_callback.
//   R6  gameswf::s_render_handler vtable slots +0xb0 and +0xb4 (called before and after the display
//       callback, return value of the second is returned). Names unknown.
//   R7  gameswf::free_internal(void*, unsigned int), the Gameloft allocator (gameswf_allocator.cpp absent).
//   R8  The DH2 constructor must initialise m_cxform_changed and m_custom (the in-class initialisers
//       for both are in the header; the constructor is in batch -2 and must agree).
//   R9  goto_frame returns 0 in the binary (its return type is not recoverable from the mangled name;
//       the upstream declaration is void).
//   R10 set_display_callback takes void (*)(render_state&, void*) in the DH2 build, not void (*)(void*).
//   R11 get_root_movie is non-const in the DH2 build. gameswf_button.cpp:243 declares a const override,
//       which no longer overrides the virtual; the button batch must resolve it.
//
// Only the reconstructed functions have bodies below. The character object needs a player and the
// upstream gameswf units to construct, so there is no standalone test for them.

#include "gameswf/gameswf_character.h"

namespace gameswf
{

// Upstream rows, no body here:
//   0x752d38 set_userdata(void*)                       inline virtual, assert(0) compiled out
//   0x752d3c set_variable(char const*, char const*)    inline virtual, assert(0) compiled out
//   0x752d40 set_variable(char const*, wchar_t const*) inline virtual, assert(0) compiled out
//   0x752d54 attach_display_callback(...)              inline virtual, assert(0) compiled out
//   0x752d58 display()                                 inline virtual {}
//   0x753260 enumerate(as_environment*)                gameswf_character.cpp, tail call to as_object::enumerate

// 0x752d84 _ZN7gameswf9character9constructEv
// Virtual (the character vtable at 0x98fb38 holds it). The binary body is bx lr.
void character::construct()
{
}

// 0x753e34 _ZN7gameswf9character28get_world_cxform_root_changeEv
// Walks the parent chain from this. The result is the topmost character, this included, whose
// cxform changed flag (+0x9a) is set, or NULL when none is set. The binary tests the parent link
// (+0x3c proxy, +0x40 pointer) in the loop. A dead link is dropped by the weak_ptr check here, and the
// walk ends at the same character as the binary does. The binary's own null test on this is not
// reproduced: a non-static member function is never called on NULL.
character* character::get_world_cxform_root_change()
{
	character* root = m_cxform_changed ? this : nullptr;
	character* node = this;
	character* parent = node->m_parent.get_ptr();
	while (parent != nullptr)
	{
		node = parent;
		if (node->m_cxform_changed)
		{
			root = node;
		}
		parent = node->m_parent.get_ptr();
	}
	return root;
}

// 0x753ee8 _ZN7gameswf9character28get_world_matrix_root_changeEv
// Same walk as get_world_cxform_root_change, on the matrix changed flag (+0x99).
character* character::get_world_matrix_root_change()
{
	character* root = m_matrix_changed ? this : nullptr;
	character* node = this;
	character* parent = node->m_parent.get_ptr();
	while (parent != nullptr)
	{
		node = parent;
		if (node->m_matrix_changed)
		{
			root = node;
		}
		parent = node->m_parent.get_ptr();
	}
	return root;
}

// 0x7547b4 _ZN7gameswf9character14get_root_movieEv
// Forwards to the parent through the weak link (+0x3c/+0x40, virtual slot 0x134). When the parent
// has died, the weak_ptr check clears the link and the call goes through a NULL parent, as it does
// in the binary (its tail at 0x754804). The proxy release on the dead path is the smart_ptr release
// (R7 for the allocator).
character* character::get_root_movie()
{
	return m_parent->get_root_movie();
}

// Partial rows, no body here. Each block records the DH2 behaviour and what is missing.

// 0x752d88 _ZN7gameswf9character7advanceEf
// PARTIAL. Binary: clears the byte at +0x9d (R3) and returns. The upstream inline advance(float) {}
// stays active in the header. Missing: R3 (need-advance flag not declared).

// 0x752d94 _ZN7gameswf9character10goto_frameEi
// PARTIAL. Binary: returns 0 (R9). The upstream inline goto_frame(int) {} stays active. Missing:
// the return type.

// 0x7531e4 _ZN7gameswf9character17detach_scene_nodeEv
// PARTIAL. Binary: if custom (+0x54) is set and its scene node (custom+0x68) is set, removes this
// from the node's child array (R4) by identity: slot 0 is checked first, then slots 1..count-1, and
// the matching slot is removed with gameswf::array::remove. Then custom+0x68 is cleared. Returns void
// (the binary's r0 is a leftover). Missing: R1 (custom+0x68), R4 (child array), and the array remove.

// 0x753c10 _ZN7gameswf9character17attach_scene_nodeEPN6glitch5scene10ISceneNodeENS1_4core11dimension2dIiEEb
// PARTIAL. Binary: creates custom if absent (R1). Looks up a scene node by name on the ISceneNode
// (glitch::scene::ISceneNode::getSceneNodeFromName, gameswf::scene_node::NODE_NAME). When it exists
// and the bool argument is set, its child array is emptied. Otherwise a new gameswf::scene_node is
// built from (character +0x30, the ISceneNode, the dimension) and attached by ISceneNode virtual slot
// 0x5c. Then custom+0x68 is set to the node, and this is appended to the node's child array, growing
// the capacity by 1.5x when full. Missing: R1, R4, the ISceneNode slot name, getSceneNodeFromName.

// 0x753508 _ZN7gameswf9character20set_display_callbackEPFvRNS_12render_stateEPvES3_
// PARTIAL. Binary: creates custom if absent (R1), stores the callback at custom+0x60 and the user
// pointer at custom+0x64. Signature per R10. The upstream inline set_display_callback(void(*)(void*),
// void*) stays active in the header. Missing: R1 (+0x60, +0x64), R5, R10.

// 0x753f9c _ZN7gameswf9character19do_display_callbackEv
// PARTIAL. Binary: calls s_render_handler virtual slot +0xb0 (R6), builds a render_state from this,
// the world matrix and the world cxform (get_world_matrix and get_world_cxform), calls custom+0x60
// with the state and custom+0x64, then returns the result of s_render_handler slot +0xb4 (R6).
// Missing: R1, R5, R6, the world-matrix and world-cxform forms used by the DH2 build (batch -2).

// 0x75355c _ZN7gameswf9character10set_cxformERKNS_6cxformE
// PARTIAL. Binary: creates custom if absent (R1), copies the argument's eight floats into
// custom+0x00..+0x1f, sets the local cxform pointer (R2, +0x48) to custom, and sets the cxform
// changed flag (+0x9a). Missing: R1 (+0x00), R2 (+0x48). The upstream inline set_cxform stays active.

// 0x7535b8 _ZN7gameswf9character8set_nameERKNS_9tu_stringE
// PARTIAL. Binary: if the argument has the interned bit set (its byte +0x13 & 1), stores the argument's
// address at +0x44. Otherwise creates custom if absent (R1), assigns the argument to custom+0x4c with
// tu_string::operator=, and stores custom+0x4c at +0x44 (R2). Returns void. Missing: R1 (+0x4c),
// R2 (+0x44), and the tu_string layout (byte +0x13 is the interned bit).

// 0x753470 _ZN7gameswf9characterD1Ev
// PARTIAL. Binary: sets the vptr to the character vtable (off_98f9f0), and if custom is set, calls
// custom::~custom and gameswf::free_internal (R1, R7). Then the m_parent weak proxy is released (the
// member destructor), then as_object::~as_object. The compiler-generated destructor has no custom
// release. Missing: R1, R7.

// 0x7534ec _ZN7gameswf9characterD0Ev
// SKIPPED. Compiler-generated deleting destructor. It runs D1 (above) and then calls LC_API_FREE_0
// (operator delete). The compiler emits it from ~character(), which is not declared here.

}  // namespace gameswf
