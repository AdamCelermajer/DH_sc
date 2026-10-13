// gameswf::as_function as the DH2 binary has it (Gameloft additions to the upstream struct in
// vendor-gameswf/upstream/gameswf-r1714/gameswf/gameswf_function.h). The binary adds a "prototype" object (m_prototype,
// binary +0x38), _typeof(), clear_refs(), get_member() and set_member() overrides, and a destructor that releases the
// prototype. Upstream's as_function has none of these.
//
// BLOCKED: gameswf/gameswf_object.h does not compile in this tree, and as_function derives from as_object, so this
// class is defined only when DH2_AS_OBJECT_HEADER_OK is set by the build (same rule as as_stage.hpp). The upstream
// as_function struct in gameswf_function.h must be wrapped in #if 0 before this header is used with it; that is not
// done yet.
//
// The binary's offsets (+0x38 for m_prototype, and +0x2c..+0x34 used by this_alive) are measured from the start of the
// object, so they depend on sizeof(as_object), which is not recovered. Offsets are therefore not stated in C++ members.
//
// Bodies: vendor-gameswf/src/v1-vendor-gameswf-pack17-c88d12-0.cpp (behind the same macro).

#pragma once

#if defined(DH2_AS_OBJECT_HEADER_OK)

#include "gameswf/gameswf_object.h"  // as_object (upstream, with DH2 declarations)
#include "base/smart_ptr.h"         // smart_ptr<T>

namespace gameswf {

struct as_function : public as_object {
	~as_function() override;  // 0x7c2fc4 (D1). Empty body: m_prototype is released by its own destructor. 0x7c316c (D0) is compiler-generated.

	const char* _typeof();    // 0x7c2894, returns "function"

	void clear_refs(hash<as_object*, bool>* visited_objects, as_object* this_ptr) override;  // 0x7c28c8
	bool get_member(const tu_stringi& name, as_value* val) override;                       // 0x7c2990
	bool set_member(const tu_stringi& name, const as_value& val) override;                 // 0x7c29f0
	void this_alive() override;                                                             // 0x7c40a4 (body not written)

	smart_ptr<as_object> m_prototype;  // binary +0x38: the object behind the "prototype" name
};

}  // namespace gameswf

#endif  // DH2_AS_OBJECT_HEADER_OK
