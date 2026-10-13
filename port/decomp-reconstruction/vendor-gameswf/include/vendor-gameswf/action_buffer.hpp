// gameswf::action_buffer as the DH2 binary has it (Gameloft). Replaces the upstream struct in
// vendor-gameswf/upstream/gameswf-r1714/gameswf/gameswf_action.h (wrapped in #if 0 there). Upstream keeps the byte
// code in gc_ptr<counted_buffer> and has an array<tu_string> dictionary. The binary keeps a plain counted_buffer*
// with a manual reference count, and the constructor writes three words only, so no dictionary member is at +0x04.
//
// Layout, 12 bytes (32-bit offsets; the pointer is host width):
//   +0x00  m_buffer                  counted_buffer*, reference counted by hand (count 1 after the constructor)
//   +0x04  m_decl_dict_processed_at  int, -1 after the constructor (upstream's initial value)
//   +0x08  m_action_offset           int, 0 after the constructor; returned by do_action::get_action_offset
//
// The dictionary that upstream read() and process_decl_dict() use is not located in this layout yet (layout request).
// Until it is, process_decl_dict, operator= and the PUSH dictionary branches of action_buffer::execute in
// gameswf_action.cpp are wrapped in #if 0 with DH2 comments.
//
// Bodies: vendor-gameswf/src/v1-vendor-gameswf-pack17-c88d12-0.cpp (0x7bae08, 0x7bae5c, 0x7c0690).
// Declared here with their upstream bodies in gameswf_action.cpp: read (355), load_as_plugin (523), enumerate (505).
// Declared here, body not reconstructed (needs m_dictionary): process_decl_dict (393), operator= (558).

#pragma once

#include "base/container.h"                  // array<>
#include "vendor-gameswf/counted_buffer.hpp" // gameswf::counted_buffer (DH2)

namespace gameswf {

struct as_environment;    // gameswf_environment.h (upstream; not compilable in the tree yet)
struct as_object;         // gameswf_environment.h (upstream)
struct as_value;          // as_value.hpp (DH2)
struct with_stack_entry;  // gameswf_environment.h (upstream); used by value in array<> only inside the .cpp
struct player;            // gameswf.h (upstream)
struct stream;            // gameswf.h (upstream)

struct action_buffer {
	action_buffer();  // 0x7bae08 (C1), 0x7bae5c (C2). Same body.

	// 0x7c0690. Runs the whole buffer with an empty with-stack. Written behind VENDOR_GAMESWF_DH2_CORE_HEADERS.
	void execute(as_environment* env);

	// Upstream signature (gameswf_action.cpp:563). Defined by another batch.
	void execute(as_environment* env, int start_pc, int exec_bytes, as_value* retval,
	             const array<with_stack_entry>& initial_with_stack, bool is_function2) const;

	// DH2: upstream members (gameswf_action.h, pristine), declared so the upstream definitions in gameswf_action.cpp match.
	void read(stream* in);  // gameswf_action.cpp:355
	static as_object* load_as_plugin(player* plugin_player, const tu_string& classname,
	                                 const array<as_value>& params);  // gameswf_action.cpp:523
	void operator=(const action_buffer& ab);  // gameswf_action.cpp:558. Body #if 0 there (needs m_dictionary).

	counted_buffer* m_buffer;             // +0x00
	int m_decl_dict_processed_at;         // +0x04
	int m_action_offset;                  // +0x08 (do_action::get_action_offset returns this word at do_action +0x0c)

private:
	void process_decl_dict(int start_pc, int stop_pc);  // gameswf_action.cpp:393. Body #if 0 there (needs m_dictionary).
	static void enumerate(as_environment* env, as_object* object);  // gameswf_action.cpp:505
};

}  // namespace gameswf
