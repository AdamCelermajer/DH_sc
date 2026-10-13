// Batch v1-vendor-gameswf-gameswf_sprite_instance-d5206d-1: gameswf::sprite_instance, part 2 of 2 (DH2).
//
// Four Gameloft bodies. In the upstream source (vendor-gameswf/upstream/gameswf-r1714/gameswf/gameswf_sprite.cpp)
// the definitions of the destructor, do_actions() and goto_frame(int) are wrapped in #if 0 (DH2). The
// declarations these bodies use are in gameswf_sprite.h with // DH2: comments. The frame-walk arithmetic
// is in vendor-gameswf/include/vendor-gameswf/gameswf_sprite_goto.hpp, which the test checks.
//
// Layout. The DH2 sprite_instance is not the upstream layout. Offsets below are the 32-bit binary's:
//   +0xa0  m_def                     gc_ptr<movie_definition_sub> upstream (released by the implicit member dtor)
//   +0xa8  m_display_list            display_list, destroyed by the implicit member dtor (binary: explicit call)
//   +0xbc  m_action_list             pending actions, run by do_actions()
//   +0xcc  m_goto_frame_action_list  goto actions; goto_frame() replaces it, then appends the target frame's actions
//   +0xdc  m_init_actions            DH2 nested init_actions block (opaque here)
//   +0xe0  (as_environment*)         heap pointer in the binary; upstream embeds m_as_environment instead
//   +0xe4  m_current_frame           int16 in the binary; declared int upstream
//   +0xe6  m_update_frame            bool
//   +0xf0  unk_f0, +0xf4 unk_f4      DH2 ref-counted pointers, released by the destructor; not identified
//   +0xf8  m_script                  hash, cleared and freed by the destructor
//   +0xfc  m_frame_script            gc_ptr<as_function> upstream; smart_ptr in the binary
// The byte at +0x9d (character base, not declared here) is set to 1 by do_actions() when actions run.

#include "gameswf/gameswf_sprite.h"
#include "vendor-gameswf/as_value.hpp"
#include "vendor-gameswf/gameswf_alloc.hpp"
#include "vendor-gameswf/gameswf_sprite_goto.hpp"

namespace gameswf
{

// Upstream (gameswf_impl.cpp:96). Same prototype as gameswf_sprite.cpp:28.
void execute_actions(as_environment* env, const array<action_buffer*>& action_list);

// The six-argument gameswf::call_method(const as_value&, as_environment*, const as_value&, int, int, const char*) is
// declared in gameswf_action.h (DH2, binary 0x7ba904). do_actions() calls it with the frame script as the method, the
// sprite as this, nargs 0, first argument 0 and the debug name "???". Its body is not in the tree.

// 0x7815d0 _ZN7gameswf15sprite_instanceD1Ev
// Binary order: hash m_script (+0xf8) cleared and freed; init_actions (+0xdc) destroyed and freed; as_environment
// (+0xe0) destroyed and freed; frame script (+0xfc) released; +0xf4 and +0xf0 released; goto list (+0xcc) and action
// list (+0xbc) cleared, buffers released; display_list (+0xa8) destroyed; m_def (+0xa0) released; ~character.
// This body does the explicit parts in that order. The implicit member destructors then run for m_display_list
// and m_def, and the base destructor runs last.
// PARTIAL:
//   1. +0xe0 is a heap as_environment in the binary, destroyed and freed here. Upstream embeds m_as_environment,
//      so that step is left to the implicit member destructor (not the same order).
//   2. init_actions is an opaque declaration (layout not recovered). Its destructor (0x780ea0) is not in this batch.
//   3. unk_f0 and unk_f4 are not identified.
//   4. clear() releases the buffer. The binary sets the size to 0 and calls reserve(0); the same release.
sprite_instance::~sprite_instance()
{
	if (m_script != nullptr)  // +0xf8
	{
		m_script->clear();
		free_internal(m_script, 0);
	}
	if (m_init_actions != nullptr)  // +0xdc
	{
		m_init_actions->~init_actions();
		free_internal(m_init_actions, 0);
	}
	// +0xe0 as_environment: see PARTIAL 1.
	m_frame_script = nullptr;  // +0xfc
	if (unk_f4 != nullptr)  // +0xf4
	{
		unk_f4->drop_ref();
	}
	if (unk_f0 != nullptr)  // +0xf0
	{
		unk_f0->drop_ref();
	}
	m_goto_frame_action_list.clear();  // +0xcc
	m_action_list.clear();             // +0xbc
}

// 0x781a3c _ZN7gameswf15sprite_instance10goto_frameEi
// Moves to target_frame_number (0-based). Walks the display list from the current frame (reverse tags going back,
// state-only tags going forward), runs the target frame's tags, then records the frame's actions.
// PARTIAL:
//   1. The binary returns int (1 when the frame moved, 0 when the request was rejected). The base class declares
//      void goto_frame(int) (gameswf_character.h), so the value is not returned here. Callers of the binary that read r0
//      need the int return, which requires changing the base declaration (integrator decision).
//   2. The binary calls execute_frame_tags through vtable +0xc8 both times: the state-only walk (r2 = 1) and the target
//      frame (r2 = 0). The port's execute_frame_tags is non-virtual upstream (gameswf_sprite.h:145), so both calls are
//      direct and a subclass override is not reached.
//   3. The current frame is int16 at +0xe4 in the binary and int here.
//   4. The pending list is emptied with size = 0 and keeps its buffer in the binary. array::clear() (resize(0)) releases
//      the buffer, so the capacity policy differs. The contents are identical.
void sprite_instance::goto_frame(int target_frame_number)
{
	if (dh2::sprite_goto_rejected(target_frame_number, m_current_frame, m_def->get_frame_count()))
	{
		m_update_frame = true;  // +0xe6
		return;
	}

	// The pending list (+0xbc) becomes the goto list (+0xcc). The binary copies it over the goto list, then
	// empties the pending list before walking the frames.
	m_goto_frame_action_list = m_action_list;
	m_action_list.clear();  // +0xc0 = 0

	const int current = m_current_frame;
	if (target_frame_number < current)
	{
		const dh2::frame_run run = dh2::sprite_goto_reverse_run(current, target_frame_number);
		for (int k = 0; k < run.count; ++k)
		{
			execute_frame_tags_reverse(run.first - k);
		}
	}
	else
	{
		const dh2::frame_run run = dh2::sprite_goto_forward_run(current, target_frame_number);
		for (int k = 0; k < run.count; ++k)
		{
			execute_frame_tags(run.first + k, true);  // vtable +0xc8 in the binary (r2 = 1); see PARTIAL 2
		}
	}

	// Actions queued while walking are dropped here (the binary zeroes +0xc0 again).
	m_action_list.clear();
	execute_frame_tags(target_frame_number, false);  // vtable +0xc8 in the binary
	m_current_frame = target_frame_number;  // +0xe4 (int16 in the binary)
	m_update_frame = true;                  // +0xe6

	// The target frame's actions are appended to the goto list.
	if (m_action_list.size() > 0)
	{
		m_goto_frame_action_list.append(m_action_list);
	}
	m_action_list.clear();  // +0xc0 = 0
	notify_need_advance();
}

// 0x781eac _ZN7gameswf15sprite_instance10do_actionsEv
// Runs the pending actions (+0xbc), then the frame script (+0xfc). The binary copies the pending list and empties it
// before the run, so actions queued during the run wait for the next pass.
// PARTIAL:
//   1. The byte at +0x9d (character base) set to 1 when the list runs is not declared (layout request for character).
//   2. The six-argument call_method (gameswf_action.h, DH2 0x7ba904) has no body in the tree.
//   3. The frame script keeps its upstream gc_ptr type; the binary uses smart_ptr.
//   4. The binary copies the pending list into a local array. For counts up to 31 that array uses a 128-byte stack
//      buffer (32 entries); for 32 or more it is heap-backed. The port copies into an ordinary array<action_buffer*>.
//      The entries and their order are the same; only the buffer placement differs.
//   5. The binary empties the pending list with size = 0 and keeps its buffer. The port's clear() (array::clear, which
//      calls resize(0)) releases it, so the next append reallocates. The contents are identical.
void sprite_instance::do_actions()
{
	if (m_action_list.size() > 0)
	{
		// Binary: *(char*)(this + 0x9d) = 1. Not declared here (see PARTIAL 1).
		add_ref();  // binary: ref_counted::add_ref(this), keeps the sprite alive during the run
		array<action_buffer*> list = m_action_list;  // binary: local copy of +0xbc
		m_action_list.clear();                       // binary: +0xc0 = 0 before the run
		execute_actions(get_environment(), list);     // get_environment is vtable +0x58
		list.clear();
		drop_ref();
	}

	if (m_frame_script.get_ptr() != nullptr)  // +0xfc
	{
		add_ref();
		{
			as_value method(m_frame_script.get_ptr());  // type-5 value holding the script (the ctor adds a ref)
			as_environment* env = get_environment();
			as_value this_value(this);                  // type-5 value holding this (the ctor adds a ref)
			gameswf::call_method(method, env, this_value, 0, 0, "???");  // the result is discarded
		}
		m_frame_script = nullptr;  // binary: smart_ptr::set_ref(this + 0xfc, 0)
		drop_ref();
	}
}

// 0x7830ec _ZN7gameswf15sprite_instance8hit_testEff
// Forwards to hit_test(float, float, bool) at 0x782f6c with shape flag 1. The asm sets R3 = 1 (the fourth argument)
// and tail-branches to _ZN7gameswf15sprite_instance8hit_testEffb.
// PARTIAL, UNRESOLVED CALLEE: 0x782f6c is in no planned batch and the tree has no definition of
// hit_test(float, float, bool). ledger.csv row 681 classes 0x782f6c as upstream by name only; the upstream overload
// takes double. The call is kept as the binary has it, and no stand-in is written. The thunk links only once that
// callee is defined.
// PARTIAL, COMPILE: the file does not compile yet: the shared upstream gameswf.h chain (DECLARE_GC_TYPES) and
// gameswf_abc.h fail with the DH2 types (see the batch report).
bool sprite_instance::hit_test(float x, float y)
{
	return hit_test(x, y, true);
}

}  // namespace gameswf
