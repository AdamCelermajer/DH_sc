// gameswf::permanent_string_cache as the DH2 binary has it (Gameloft). Not in gameswf r1714.
//
// The binary calls the hash methods on the cache object itself, and the cache destructor ends with
// hash::clear() on the same object. So the cache is the upstream hash<> with the table pointer at +0x00,
// and it derives from that hash at offset 0. Table header: int entry count (+0x00), int size mask (+0x04).
// Entries are 16 bytes from +0x08: int next in chain (+0x00), size_t hash (+0x04), key (+0x08), value (+0x0c).
//
// The key is a string_pointer (one tu_string pointer, 4 bytes). The value is a permanent_string*. Equality and
// the hash are on the text. Their bodies are not in this tree (see the ledger notes).
//
// Bodies: vendor-gameswf/src/v1-vendor-gameswf-pack3-a95b99-0.cpp (clear, destructor). get() is partial.

#pragma once

#include <cstddef>

#include "base/container.h"                 // hash<> (upstream, global namespace)
#include "vendor-gameswf/ref_counted.hpp"   // destruct<T> (declared here, defined by another batch)

namespace gameswf {

// Interned string. The binary's permanent string is a 20-byte DH2 tu_string with the interned bit at +0x13
// (see the DH2 tu_string notes). The layout is not declared in this tree, so only the name is used here.
struct permanent_string;

// Key of the cache: one pointer to a tu_string (the binary passes the address of a tu_string* to find_index).
struct string_pointer {
	::tu_string* m_string;  // +0x00

	// Text equality (the binary compares the strings behind the pointers). Not defined in this batch.
	bool operator==(const string_pointer& other) const;
};

// Hash functor of the cache: hashes the text behind string_pointer::m_string. Not defined in this batch.
template <class T>
struct string_pointer_hash_functor {
	std::size_t operator()(const T& key) const;
};

class permanent_string_cache
	: public ::hash<string_pointer, permanent_string*, string_pointer_hash_functor<string_pointer> > {
public:
	// 0x75c2cc gameswf::permanent_string_cache::get(gameswf::tu_string const&)   [partial, not written]
	::tu_string* get(const ::tu_string& name);

	// 0x76c8ac gameswf::permanent_string_cache::clear()
	void clear();

	// 0x76c990 gameswf::permanent_string_cache::~permanent_string_cache()   [D1]
	~permanent_string_cache();
};

}  // namespace gameswf
