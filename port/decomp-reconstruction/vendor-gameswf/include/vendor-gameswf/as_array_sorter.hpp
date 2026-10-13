// gameswf::standard_array_sorter and gameswf::custom_array_sorter as the DH2 binary has them (Gameloft).
// Neither class is in gameswf r1714, so both are declared here, not in the upstream tree.
//
// Layout, 32-bit offsets as comments only (host pointers are 8 bytes; never do arithmetic on these offsets):
//   standard_array_sorter  pad_00 +0x00..+0x0b  not read by operator(); field identity unknown
//                          m_options +0x0c      int, as_global_array option bits: CASEINSENSITIVE 1,
//                                               DESCENDING 2, NUMERIC 0x10 (upstream gameswf_as_classes/as_array.h)
//   custom_array_sorter    m_compare_fn +0x00   as_value*, the comparison function value. The binary loads
//                                               this one word twice: as the method and as the this value.
//                          m_env +0x04          as_environment*. The environment's first member is the
//                                               array<as_value> stack (the binary calls array methods on it).
//
// Bodies: vendor-gameswf/src/v1-vendor-gameswf-pack11-4edfaf-0.cpp. standard_array_sorter::operator() is
// compiled there. custom_array_sorter::operator() is behind VENDOR_GAMESWF_DH2_CORE_HEADERS (see that file).

#pragma once

#include <cstdint>

#include "vendor-gameswf/as_value.hpp"  // as_value (DH2), compiles on its own

namespace gameswf {

struct as_environment;  // gameswf_environment.h (upstream); the comparator only stores and passes the pointer

// DH2: comparator for Array.sort with the NUMERIC, CASEINSENSITIVE or DESCENDING option bits. 0x7986e4.
struct standard_array_sorter {
	std::uint8_t pad_00[12];  // +0x00..+0x0b: not read by operator(); names unknown
	int m_options;            // +0x0c: as_global_array option bits

	// Returns true when lhs sorts before rhs. The binary returns int 0 or 1; declared bool here.
	bool operator()(const as_value& lhs, const as_value& rhs);  // 0x7986e4
};

// DH2: comparator that calls a script function with the two values pushed on the environment stack. 0x799514.
struct custom_array_sorter {
	as_value* m_compare_fn;     // +0x00: the comparison function value
	as_environment* m_env;      // +0x04: environment whose stack holds the call arguments

	// True when the script function returns exactly 1.
	bool operator()(const as_value& lhs, const as_value& rhs);  // 0x799514
};

}  // namespace gameswf
