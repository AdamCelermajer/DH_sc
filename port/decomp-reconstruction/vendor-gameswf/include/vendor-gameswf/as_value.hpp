// gameswf::as_value as the DH2 binary has it (Gameloft). This replaces the upstream struct in
// vendor-gameswf/upstream/gameswf-r1714/gameswf/gameswf_value.h, which is marked #if 0 there. The upstream
// layout (enum type at +0x00, inline tu_string, gc_ptr members, int flags) is not the binary's.
//
// Layout, offsets as the 32-bit binary has them. Host pointers are 8 bytes, so the offsets are comments only:
//   +0x00  unk_00   uint8   flags. Every constructor clears it and operator= copies it. No function in the
//                           v1-vendor-gameswf-gameswf_as_value batch reads it. Upstream calls this byte
//                           value flags (DONT_ENUM, DONT_DELETE, READ_ONLY); not confirmed here.
//   +0x01  m_type   uint8   value type, see as_value::type.
//   +0x04  payload  union   m_number (double, +0x04..+0x0b), m_bool (one byte at +0x04), m_string
//                           (tu_string*, types 3 and 4), m_object (as_object*, type 5, and the target of
//                           a type 6 property).
//   +0x08  m_owner  ref_counted*  type 3: owner that keeps a borrowed string alive (may be null). Type 6:
//                           the as_property. Other types leave it unwritten.
//
// Type codes, from the switch statements of the batch: 0 undefined, 1 boolean, 2 number, 3 string held by
// pointer without ownership (m_owner may keep the string alive), 4 string owned by the value (heap tu_string),
// 5 object (null allowed), 6 property (target at +0x04, as_property at +0x08).
//
// Bodies: vendor-gameswf/src/v1-vendor-gameswf-gameswf_as_value-794e76-0.cpp

#pragma once

#include <cstdint>

#include "gameswf/gameswf.h"  // as_object, as_function, ref_counted (DH2, ref_counted.hpp), fn_call

class tu_string;  // global in upstream; the binary has gameswf::tu_string (see gameswf_dh2_runtime.hpp)

namespace gameswf {

struct as_property;  // gameswf_value.h (upstream); derives from ref_counted
struct as_string;    // binary: as_value::set_string(as_string*); no definition in the tree yet
struct as_function;  // gameswf.h / gameswf_value.h (upstream)
struct fn_call;      // gameswf.h (upstream)

// DH2: replaces the upstream as_value. The binary's as_value is 12 bytes on 32-bit; see the layout above.
struct as_value {
	// Value type, the byte at +0x01. Types 3 and 4 are both strings: 3 borrows its tu_string, 4 owns it.
	enum type : std::uint8_t
	{
		UNDEFINED = 0,
		BOOLEAN = 1,
		NUMBER = 2,
		STRING_BORROWED = 3,
		STRING = 4,
		OBJECT = 5,
		PROPERTY = 6
	};

	as_value() : unk_00(0), m_type(UNDEFINED), m_number(0.0), m_owner(nullptr) {}
	as_value(const as_value& v) : as_value() { operator=(v); }
	as_value(int val);                                   // 0x439d50
	as_value(const char* str);                           // C1 EPKc, no batch (declared for callers)
	as_value(const wchar_t* wstr);                       // 0x797408 (C1), 0x797448 (C2)
	as_value(as_object* obj);                            // 0x7babb0
	as_value(const as_value& getter, const as_value& setter);  // 0x769540

	~as_value() { drop_refs(); }

	// Releases what the current type holds. Called before any change of type.
	void drop_refs();                                    // 0x797124

	// Conversions.
	const char* to_xstring() const;                      // debugging
	const char* to_string() const;
	const tu_string& to_string(tu_string& buf) const;    // 0x797b7c (return type not confirmed)
	const tu_string& to_tu_string() const;
	double to_number() const;
	int to_int() const { return static_cast<int>(to_number()); }
	float to_float() const { return static_cast<float>(to_number()); }
	bool to_bool() const;
	as_function* to_function() const;
	as_property* to_property() const;
	// Not a binary symbol (likely inlined there). Upstream semantics on the DH2 layout: the object, or null.
	as_object* to_object() const { return m_type == OBJECT ? m_object : nullptr; }

	// Setters. The set_*() forms are preferred; they are type-safe.
	void set_tu_string(const tu_string& str);            // 0x7972d8
	void set_string(const char* str);                    // 0x797350
	void set_string(as_string* str);                     // binary overload; no batch
	void set_double(double val);
	void set_bool(bool val);
	void set_int(int val) { set_double(val); }
	void set_as_object(as_object* obj);
	void set_as_c_function(void (*func)(const fn_call& fn));
	void set_undefined() { drop_refs(); m_type = UNDEFINED; }
	void set_null() { set_as_object(nullptr); }

	void set_property(const as_value& val);
	void get_property(as_value* val) const;
	void get_property(const as_value& primitive, as_value* val) const;
	const as_object* get_property_target() const;        // debugging
	void set_property_target(as_object* new_target);

	// Operators.
	void operator=(const as_value& v);
	bool operator==(const as_value& v) const;            // 0x797dc0
	bool operator!=(const as_value& v) const;            // 0x798040, upstream gameswf_value.cpp
	bool operator<(double v) const { return to_number() < v; }
	void operator+=(double v) { set_double(to_number() + v); }
	void operator-=(double v) { set_double(to_number() - v); }
	void operator*=(double v) { set_double(to_number() * v); }
	void operator/=(double v) { set_double(to_number() / v); }  // 0x7ba984, upstream inline body
	void operator&=(int v) { set_int(int(to_number()) & v); }
	void operator|=(int v) { set_int(int(to_number()) | v); }
	void operator^=(int v) { set_int(int(to_number()) ^ v); }
	void shl(int v) { set_int(int(to_number()) << v); }
	void asr(int v) { set_int(int(to_number()) >> v); }
	void lsr(int v) { set_int(static_cast<unsigned int>(to_number()) >> v); }

	// Type queries.
	bool is_function() const;
	bool is_number() const;
	bool is_bool() const { return m_type == BOOLEAN; }
	bool is_string() const { return m_type == STRING_BORROWED || m_type == STRING; }
	bool is_object() const { return m_type == OBJECT; }
	bool is_property() const { return m_type == PROPERTY; }
	bool is_null() const { return m_type == OBJECT && m_object == nullptr; }
	bool is_undefined() const { return m_type == UNDEFINED; }
	const char* _typeof() const;                         // 0x797654, binary name of type_of
	bool is_instance_of(const as_function* constructor) const;

	// Member lookup on the value's builtin methods or on its object (0x796fd0).
	bool get_member(const tu_string& name, as_value* val);

private:
	std::uint8_t unk_00;     // +0x00 flags (see the comment at the top)
	std::uint8_t m_type;     // +0x01 as_value::type
	union
	{
		double m_number;         // +0x04
		bool m_bool;             // +0x04, one byte
		::tu_string* m_string;   // +0x04, types 3 and 4
		as_object* m_object;     // +0x04, type 5 and the target of type 6
	};
	ref_counted* m_owner;    // +0x08 (type 3 owner, type 6 as_property)
};

}  // namespace gameswf
