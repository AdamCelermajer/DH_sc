// Batch v1-vendor-gameswf-pack16-133bd3-0 (vendor-gameswf, gameswf r1714 with Gameloft changes).
//
// Compiled by default:
//   gameswf::profiler::set_enabled, begin_event, end_event, dump (include/vendor-gameswf/profiler.hpp).
//   The binary bodies are bare `bx lr`.
//
// Behind VENDOR_GAMESWF_DH2_CORE_HEADERS (PARTIAL, not compiled by check_tu.sh): gameswf/gameswf_abc.h
//   does not compile in this tree. gameswf_action.h:203 (counted_buffer is not declared) and
//   base/tu_gc_singlethreaded_refcount.h:120 (as_object is not a gc_object_collector_base) stop it, the
//   same blocker as as_stage in pack6. The bodies are:
//     metadata_info::read (empty; upstream body is assert(0 && "todo"), wrapped in #if 0 in gameswf_abc.cpp)
//     abc_def::find_instance (DH2 addition, declared in gameswf_abc.h with a // DH2: comment)
//
// Upstream, with no body here (the binary matches the r1714 definition or is compiler-generated):
//   0x7b7bf0  zlib_adapter::make_inflater      (base/zlib_adapter.cpp, global namespace in upstream)
//   0x7b91fc  metadata_info::~metadata_info()  D1, compiler-generated (no user dtor in r1714)
//   0x7b94c4, 0x7b99ec  abc_def::~abc_def()    D1 and D2 (gameswf_abc.cpp, empty user dtor)
//   0x7b92c8  script_info::~script_info()      D1, compiler-generated (no user dtor in r1714)
//   0x7b98d4  array<int>::~array()             template instance (base/container.h, clear() -> resize(0))
//
// Skipped (compiler-generated deleting destructors, D1 then operator delete / LC_API_FREE_0):
//   0x7b9230 metadata_info D0, 0x7b96f0 abc_def D0, 0x7b926c class_info D0, 0x7b9370 script_info D0.
//
// No body (PARTIAL, layout not recovered; see the ledger notes and the layout requests):
//   0x7b8e04, 0x7b8efc  abc_def::abc_def(player*). The binary zeroes ref_counted and the members, then
//                       one byte after each array member at +0x18, +0x28 ... +0xc8. The owners of those
//                       bytes are not in the upstream header, so the upstream empty ctor stays active.
//   0x7b931c            class_info::~class_info() D1. The binary clears the trait array at +0x10 and
//                       calls no weak_ptr destructor, but upstream class_info has m_abc (weak_ptr) at
//                       +0x0c, so the DH2 layout around +0x0c..+0x10 is not recovered.

#include "vendor-gameswf/profiler.hpp"

#if defined(VENDOR_GAMESWF_DH2_CORE_HEADERS)
#include <cstdint>

#include "gameswf/gameswf_abc.h"
#endif

namespace gameswf {

// 0x7b793c gameswf::profiler::set_enabled(bool)
// Binary: bx lr. The argument is not read.
void profiler::set_enabled(bool enabled) {
	(void)enabled;
}

// 0x7b7940 gameswf::profiler::begin_event(char const*)
// Binary: bx lr. The name is not read.
void profiler::begin_event(const char* name) {
	(void)name;
}

// 0x7b7944 gameswf::profiler::end_event()
// Binary: bx lr.
void profiler::end_event() {
}

// 0x7b7948 gameswf::profiler::dump()
// Binary: bx lr. Nothing is written.
void profiler::dump() {
}

#if defined(VENDOR_GAMESWF_DH2_CORE_HEADERS)

// 0x7b7fb0 gameswf::metadata_info::read(gameswf::stream*, gameswf::abc_def*)
// Binary: bx lr. The DH2 body does nothing; the r1714 body asserts "todo".
void metadata_info::read(stream* in, abc_def* abc) {
	(void)in;
	(void)abc;
}

// 0x7b7fb4 gameswf::abc_def::find_instance(gameswf::tu_string const&) const
// Binary (3 instructions): R3 = [this+0x9c] (the m_instance buffer), R0 = [R3], return R0. The name
// argument is not read and the table is not searched. The returned value is the first word of
// m_instance[0] (its raw element bits). The binary has no size check. On the 64-bit host that word is
// the low half of a pointer, so this body is partial: the meaning of the value is not known.
int abc_def::find_instance(const tu_string& name) const {
	(void)name;
	return static_cast<int>(reinterpret_cast<std::uintptr_t>(m_instance[0].get()));
}

#endif  // VENDOR_GAMESWF_DH2_CORE_HEADERS

}  // namespace gameswf
