// gameswf::as_environment as the DH2 binary has it (Gameloft). This replaces the upstream struct in
// vendor-gameswf/upstream/gameswf-r1714/gameswf/gameswf_environment.h, which is marked #if 0 there. The upstream
// layout (vm_stack base with a stack size, AVM2 scope stack, gc_ptr target, weak_ptr player) is not the binary's.
//
// Layout, 108 bytes on 32-bit. Offsets are comments only; host pointers are 8 bytes.
//   +0x00  m_stack            array<as_value>: value stack (buffer, m_size, m_buffer_size). pop() uses it.
//   +0x0c  pad_0c             4 bytes, not used by this batch.
//   +0x10  m_global_register  as_value[4] (12 bytes each). The destructor drops the values at +0x10, +0x1c, +0x28, +0x34.
//   +0x40  m_local_register   array<as_value>.
//   +0x4c  pad_4c             4 bytes, not used by this batch.
//   +0x50  m_target           smart_ptr<as_object> (one pointer). set_target stores it; the destructor drops it.
//   +0x54  m_local_frames     array<frame_slot>. frame_slot is 32 bytes: a DH2 tu_string name (20 bytes) and an as_value.
//   +0x60  pad_60             4 bytes, not used by this batch.
//   +0x64  m_player_proxy     weak_proxy*. Mutable: get_player and get_root clear it.
//   +0x68  m_player           player*, valid while m_player_proxy is alive. Mutable, as above.
//
// Bodies: vendor-gameswf/src/v1-vendor-gameswf-pack3-a95b99-0.cpp (pop, get_player). The rest is declared only.

#pragma once

#include <cstdint>

#include "base/container.h"                  // array<T> (upstream, global namespace)
#include "base/smart_ptr.h"                  // smart_ptr<T> (upstream, global namespace)
#include "base/weak_ptr.h"                   // weak_proxy (upstream, global namespace)
#include "vendor-gameswf/as_value.hpp"       // gameswf::as_value (DH2)

namespace gameswf {

struct as_object;   // gameswf_object.h (upstream); not complete in this tree (gc_ptr instantiation blocks it)
struct character;   // gameswf_character.h (upstream); no DH2 declaration in this tree
struct player;      // gameswf_player.h (upstream); no DH2 declaration in this tree
struct root;        // gameswf_root.hpp (DH2)
struct frame_slot;  // DH2: 32 bytes (name + value). Not declared in this tree (its name is a 20-byte DH2 tu_string)

struct as_environment {
	::array<as_value> m_stack;                 // +0x00
	std::uint8_t pad_0c[4];                    // +0x0c
	as_value m_global_register[4];             // +0x10
	::array<as_value> m_local_register;        // +0x40
	std::uint8_t pad_4c[4];                    // +0x4c
	::smart_ptr<as_object> m_target;           // +0x50
	::array<frame_slot> m_local_frames;        // +0x54
	std::uint8_t pad_60[4];                    // +0x60
	mutable weak_proxy* m_player_proxy;        // +0x64
	mutable player* m_player;                  // +0x68

	// 0x75e03c gameswf::as_environment::~as_environment()   [D1]   [partial, not written]
	~as_environment();

	// 0x7997c4 gameswf::as_environment::pop()
	as_value pop();

	// 0x7bc1dc gameswf::as_environment::get_player() const
	player* get_player() const;

	// 0x7cda54 gameswf::as_environment::get_root() const   [partial, not written]
	root* get_root() const;

	// 0x7ccecc gameswf::as_environment::set_target(gameswf::character*)   [partial, not written]
	void set_target(character* target);

	// 0x7d2b94 gameswf::as_environment::add_frame_barrier()   [partial, not written]
	void add_frame_barrier();
};

}  // namespace gameswf
