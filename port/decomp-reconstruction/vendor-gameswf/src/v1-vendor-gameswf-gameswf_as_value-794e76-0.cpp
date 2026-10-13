// Batch v1-vendor-gameswf-gameswf_as_value-794e76-0: gameswf::as_value (DH2 version).
//
// The class is declared in vendor-gameswf/include/vendor-gameswf/as_value.hpp, which replaces the upstream
// struct in gameswf/gameswf_value.h. Bodies follow the Hex-Rays pseudocode and the ARM listings. Where the
// listing and the pseudocode disagree, the listing decides. A function is PARTIAL in its comment and its
// ledger row when a callee or layout it needs is not in the tree.
//
// Blocker for the object branches: gameswf/gameswf_object.h and gameswf/gameswf_action.h do not compile with
// this toolchain (gc_ptr<as_object> is instantiated while as_object is incomplete, tu_gc_singlethreaded_refcount.h
// lines 120 and 123). Those headers are not included here. The code below therefore needs only gameswf_value.h,
// which does compile, and the calls on an as_object are either reached through the ref_counted base or left as
// marked stubs.
//
// Value layout (see as_value.hpp): +0x00 unk_00, +0x01 m_type, +0x04 payload union, +0x08 m_owner.
// Types: 0 undefined, 1 boolean, 2 number, 3 borrowed string, 4 owned string, 5 object, 6 property.

#include <cstdio>
#include <new>

#include "core-platform/memory.hpp"          // operator new(std::size_t, swf::MemHint), 0x752ba8
#include "gameswf/gameswf_value.h"           // as_property (upstream); includes vendor-gameswf/as_value.hpp
#include "vendor-gameswf/gameswf_alloc.hpp"  // free_internal(void*, unsigned int), 0x752b38

namespace gameswf {

namespace {

// PARTIAL. The binary's set_tu_string tests bit 0 of byte +0x13 of the DH2 tu_string. That tu_string is 20 bytes
// with the heap flag at +0x00 and the heap pointer at +0x0c. The tree's tu_string (base/container.h) is a
// different layout and has no such bit. Until the DH2 tu_string is declared, the value always takes the copy
// branch. The observable difference is only that a borrowed string is not copied.
bool tu_string_is_borrowed(const tu_string&) {
	return false;
}

// PARTIAL. The binary calls add_ref and drop_ref on an as_object. Those are ref_counted members, and as_object
// derives from ref_counted through a single chain (as_object : as_object_interface : ref_counted, upstream
// gameswf_object.h and gameswf.h). The ref_counted subobject is then at the object's address. This is an ABI
// assumption, not confirmed against the as_object vtable.
ref_counted* object_as_ref(as_object* obj) {
	return reinterpret_cast<ref_counted*>(obj);
}

}  // namespace

// 0x439d50 gameswf::as_value::as_value(int)
// +0x00 = 0, +0x01 = 2 (number), +0x04 = (double)val. The binary does not write +0x08 for numbers. It is set
// to null here so that no member is left uninitialised.
as_value::as_value(int val) : unk_00(0), m_type(NUMBER), m_number(static_cast<double>(val)), m_owner(nullptr) {}

// 0x769540 gameswf::as_value::as_value(gameswf::as_value const&, gameswf::as_value const&)
// Property value. +0x00 = 0, +0x01 = 6 (property), +0x04 = 0 (no target), +0x08 = a new as_property built from
// getter and setter. The binary allocates 0x14 bytes with operator new(size, swf::MemHint 0). This build
// allocates sizeof(as_property) with the same hint, which is the 32-bit size on the target and the host size here.
// The new property takes one reference.
as_value::as_value(const as_value& getter, const as_value& setter)
    : unk_00(0), m_type(PROPERTY), m_object(nullptr), m_owner(nullptr) {
	void* mem = operator new(sizeof(as_property), static_cast<swf::MemHint>(0));
	as_property* prop = new (mem) as_property(getter, setter);
	m_owner = prop;
	prop->add_ref();
}

// 0x796fd0 gameswf::as_value::get_member(gameswf::tu_string const&, gameswf::as_value*)
// PARTIAL. Boolean (1) asks the BOOLEAN builtin table (id 3), number (2) the NUMBER table (id 2), and string (3, 4)
// the STRING table (id 4), through gameswf::get_builtin(builtin_object, tu_stringi const&, as_value*). An object (5)
// forwards to its own get_member(tu_stringi const&, as_value*) (vtable +0x20, checked against the DH2 vtable dump).
// Both need declarations from gameswf_action.h and gameswf_object.h, which do not compile in this tree (see the
// file comment). Those branches return false here. The binary's listing types the result as int*; the callees
// return bool in the tree, so bool is used.
bool as_value::get_member(const tu_string& name, as_value* val) {
	(void)name;
	(void)val;
	// STUB, all branches. Boolean: get_builtin(BUILTIN_BOOLEAN_METHOD, name.to_tu_stringi(), val). Number:
	// BUILTIN_NUMBER_METHOD. String: BUILTIN_STRING_METHOD. Object (non-null): m_object->get_member(...).
	// Each needs a declaration from gameswf_action.h or gameswf_object.h, which do not compile here.
	return false;
}

// 0x797124 gameswf::as_value::drop_refs()
// Releases what the current type holds and clears the pointer. The type byte is not changed here, as in the
// binary. Type 4 destroys its tu_string and frees the block with free_internal(ptr, 0). The binary frees the
// heap buffer first with free_internal and then the block; the tree's tu_string destructor frees its own heap
// buffer, so the buffer is released by the same destructor call. PARTIAL for types 5 and 6 (their target is
// released through object_as_ref).
void as_value::drop_refs() {
	switch (m_type) {
	case STRING_BORROWED:
		if (m_owner != nullptr) {
			m_owner->drop_ref();
			m_owner = nullptr;
		}
		break;
	case STRING:
		if (m_string != nullptr) {
			m_string->~tu_string();
			free_internal(m_string, 0);
			m_string = nullptr;
		}
		break;
	case OBJECT:
		if (m_object != nullptr) {
			object_as_ref(m_object)->drop_ref();
			m_object = nullptr;
		}
		break;
	case PROPERTY:
		if (m_owner != nullptr) {
			m_owner->drop_ref();
			m_owner = nullptr;
		}
		if (m_object != nullptr) {
			object_as_ref(m_object)->drop_ref();
			m_object = nullptr;
		}
		break;
	default:
		break;
	}
}

// 0x7972d8 gameswf::as_value::set_tu_string(gameswf::tu_string const&)
// PARTIAL: the borrow branch depends on the DH2 tu_string flag (see tu_string_is_borrowed). Nothing changes when
// the value already holds this same owned string. Otherwise the old value is dropped. A borrowed string is stored
// as type 3 with no owner. Any other string is copied into a new block as type 4.
void as_value::set_tu_string(const tu_string& str) {
	if (m_type != STRING || m_string != &str) {
		drop_refs();
		if (tu_string_is_borrowed(str)) {
			m_type = STRING_BORROWED;
			m_owner = nullptr;
			m_string = const_cast<tu_string*>(&str);
		} else {
			m_type = STRING;
			void* mem = operator new(sizeof(tu_string), static_cast<swf::MemHint>(0));
			m_string = new (mem) tu_string(str);
		}
	}
}

// 0x797350 gameswf::as_value::set_string(char const*)
// An owned string (type 4) is overwritten in place: the new text is built in a temporary and assigned. Any other
// type is dropped, and an owned string of the given text is created.
void as_value::set_string(const char* str) {
	if (m_type == STRING) {
		tu_string tmp(str);
		*m_string = tmp;  // the listing frees the temporary's heap buffer after the assignment (tmp dtor)
	} else {
		drop_refs();
		m_type = STRING;
		void* mem = operator new(sizeof(tu_string), static_cast<swf::MemHint>(0));
		m_string = new (mem) tu_string(str);
	}
}

// 0x797408 gameswf::as_value::as_value(wchar_t const*)  [C1]
// 0x797448 gameswf::as_value::as_value(wchar_t const*)  [C2, same listing]
// PARTIAL. The listing stores type 4, then calls set_string("") before +0x04 has been written, and only then
// encodes the wide text into +0x04. In set_string that first call reads +0x04 as a live tu_string pointer, which
// is not valid at that point. This build allocates the empty string first and encodes into it. The uninitialised
// read is not reproduced. Both symbols share this one definition.
as_value::as_value(const wchar_t* wstr)
    : unk_00(0), m_type(STRING), m_string(nullptr), m_owner(nullptr) {
	void* mem = operator new(sizeof(tu_string), static_cast<swf::MemHint>(0));
	m_string = new (mem) tu_string("");
	tu_string::encode_utf8_from_wchar(m_string, wstr);
}

// 0x797654 gameswf::as_value::_typeof() const
// PARTIAL. Names for types 0 to 4 are fixed, and a null object is "null". A property asks its current value, read
// through get_property. A non-null object asks itself (vtable +0x18, as_object::_typeof), which needs a complete
// as_object and is stubbed here, returning nullptr. Unknown types give nullptr.
const char* as_value::_typeof() const {
	switch (m_type) {
	case UNDEFINED:
		return "undefined";
	case BOOLEAN:
		return "boolean";
	case NUMBER:
		return "number";
	case STRING_BORROWED:
	case STRING:
		return "string";
	case OBJECT:
		if (m_object != nullptr) {
			// STUB: m_object->_typeof() (vtable +0x18). Not reachable until gameswf_object.h compiles.
			return nullptr;
		}
		return "null";
	case PROPERTY: {
		as_value prop;
		get_property(&prop);
		return prop._typeof();
	}
	default:
		return nullptr;
	}
}

// 0x797b7c gameswf::as_value::to_string(gameswf::tu_string&) const
// PARTIAL on two points. The listing types the result as char*, but its string case returns the held tu_string and
// the other cases return buf, so this build returns const tu_string&. A non-null object asks itself for its text
// (vtable +0x0c, as_object::to_string), which needs a complete as_object and is stubbed here (buf left empty).
// Undefined, boolean, number and null write their text into buf. Numbers use "%.14g", as in the binary.
const tu_string& as_value::to_string(tu_string& buf) const {
	switch (m_type) {
	case UNDEFINED:
		buf = "undefined";
		return buf;
	case BOOLEAN:
		buf = m_bool ? "true" : "false";
		return buf;
	case NUMBER: {
		char s[50];
		snprintf(s, sizeof(s), "%.14g", m_number);
		buf = s;
		return buf;
	}
	case STRING_BORROWED:
	case STRING:
		return *m_string;
	case OBJECT:
		if (m_object != nullptr) {
			// STUB: buf = m_object->to_string() (vtable +0x0c). Not reachable until gameswf_object.h compiles.
			buf = "";
			return buf;
		}
		buf = "null";
		return buf;
	case PROPERTY: {
		as_value prop;
		get_property(&prop);
		tu_string tmp;
		buf = prop.to_string(tmp);
		return buf;
	}
	default:
		buf = "";
		return buf;
	}
}

// 0x797dc0 gameswf::as_value::operator==(gameswf::as_value const&) const
// Abstract equality as the DH2 listing has it. Undefined equals undefined and an empty object. A boolean compares
// with a boolean as bytes, with a number through to_bool(), and with a string through to_number(). The listing
// compares a boolean with a number through to_bool(), not to_number(). That is kept. A number compares numerically
// with numbers, and with strings and booleans through to_number(). Strings compare as tu_strings with other
// strings and numerically with numbers and booleans. An object equals the same object, or undefined when empty.
// A property compares by its current value. Other combinations are not equal.
bool as_value::operator==(const as_value& a2) const {
	switch (m_type) {
	case UNDEFINED:
		if (a2.m_type == UNDEFINED) {
			return true;
		}
		if (a2.m_type == OBJECT) {
			return a2.m_object == nullptr;
		}
		return false;
	case BOOLEAN:
		switch (a2.m_type) {
		case BOOLEAN:
			return m_bool == a2.m_bool;
		case NUMBER:
			return m_bool == a2.to_bool();
		case STRING_BORROWED:
		case STRING:
			return to_number() == a2.to_number();
		default:
			return false;
		}
	case NUMBER:
		switch (a2.m_type) {
		case BOOLEAN:
		case STRING_BORROWED:
		case STRING:
			return a2.to_number() == m_number;
		case NUMBER:
			return m_number == a2.m_number;
		default:
			return false;
		}
	case STRING_BORROWED:
	case STRING:
		switch (a2.m_type) {
		case BOOLEAN:
			return to_number() == a2.to_number();
		case NUMBER:
			return to_number() == a2.m_number;
		case STRING_BORROWED:
		case STRING:
			return *m_string == *a2.m_string;
		default:
			return false;
		}
	case OBJECT:
		if (a2.m_type == OBJECT && m_object == a2.m_object) {
			return true;
		}
		if (m_object != nullptr) {
			return false;
		}
		return a2.m_type == UNDEFINED;
	case PROPERTY: {
		as_value prop;
		get_property(&prop);
		return prop == a2;
	}
	default:
		return false;
	}
}

// 0x7babb0 gameswf::as_value::as_value(gameswf::as_object*)
// PARTIAL: a non-null object takes one reference through object_as_ref (see there).
// +0x00 = 0, +0x01 = 5 (object), +0x04 = obj.
as_value::as_value(as_object* obj) : unk_00(0), m_type(OBJECT), m_object(obj), m_owner(nullptr) {
	if (obj != nullptr) {
		object_as_ref(obj)->add_ref();
	}
}

// 0x798040 gameswf::as_value::operator!=(gameswf::as_value const&) const
// Same body as upstream gameswf_value.cpp (`return ! (*this == v);`). The upstream definition is used, so this
// file does not define it.
//
// 0x7ba984 gameswf::as_value::operator/=(double)
// Same body as the upstream inline in gameswf_value.h (`set_double(to_number() / v)`). The inline is kept
// verbatim in as_value.hpp, so this file does not define it.

}  // namespace gameswf
