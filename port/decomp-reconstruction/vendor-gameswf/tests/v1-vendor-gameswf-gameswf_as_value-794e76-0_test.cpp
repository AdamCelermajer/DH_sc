// Test for batch v1-vendor-gameswf-gameswf_as_value-794e76-0 (gameswf::as_value).
//   bash tools/trackb/check_tu.sh --test <this file> port/decomp-reconstruction/vendor-gameswf/src/v1-vendor-gameswf-gameswf_as_value-794e76-0.cpp
//
// The test supplies the callees that are outside this batch. They count calls or do nothing; the string code
// (tu_string members) and the upstream operator!= are not linked here:
//   ref_counted::add_ref / drop_ref (ref_counted batch)  count the calls and record the object address
//   gameswf::free_internal (0x752b38)                     frees with free()
//   operator new(size_t, swf::MemHint) (0x752ba8)         malloc()
//   gameswf::as_property::as_property (upstream)          counts the constructor call
//   as_value::get_property, to_number, to_bool            no-ops (not under test)
//   tu_string members used by the batch                   no-ops (the tu_string batch owns them)

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "core-platform/deps/swf_types.hpp"
#include "vendor-gameswf/as_value.hpp"
#include "vendor-gameswf/gameswf_alloc.hpp"

namespace {

int g_failures = 0;
int g_add_ref = 0;
int g_drop_ref = 0;
int g_property_ctor = 0;
const void* g_last_add_ref = nullptr;
const void* g_last_drop_ref = nullptr;

void check(bool ok, const char* what) {
	if (!ok) {
		++g_failures;
		std::fprintf(stderr, "FAIL: %s\n", what);
	}
}

}  // namespace

// Doubles for the callees the batch reaches.
namespace gameswf {

void ref_counted::add_ref() const {
	++g_add_ref;
	g_last_add_ref = this;
}

void ref_counted::drop_ref() {
	++g_drop_ref;
	g_last_drop_ref = this;
}

void free_internal(void* p, unsigned int) {
	std::free(p);
}

double as_value::to_number() const {
	return 0.0;
}

bool as_value::to_bool() const {
	return false;
}

void as_value::get_property(as_value*) const {}

}  // namespace gameswf

void* operator new(std::size_t size, swf::MemHint) {
	return std::malloc(size);
}

// gameswf::as_property::as_property(as_value const&, as_value const&), complete-object constructor. The batch
// calls it through the upstream struct, whose gc_ptr<as_function> members do not compile in a TU without
// as_function complete, so the symbol is defined by its mangled name here.
void as_property_ctor_double(void*, const void*, const void*)
    __asm__("_ZN7gameswf11as_propertyC1ERKNS_8as_valueES3_");
void as_property_ctor_double(void*, const void*, const void*) {
	++g_property_ctor;
}

void tu_string::resize(int) {}
void tu_string::operator+=(const char*) {}
void tu_string::operator+=(const tu_string&) {}
bool tu_string::operator==(const tu_string&) const {
	return false;
}
void tu_string::encode_utf8_from_wchar(tu_string*, const wchar_t*) {}
int tu_string::utf8_char_count(const char*, int) {
	return 0;
}

namespace {

using gameswf::as_value;

void test_undefined_and_number() {
	as_value n(42);
	check(std::strcmp(n._typeof(), "number") == 0, "number _typeof");
	as_value u;
	check(std::strcmp(u._typeof(), "undefined") == 0, "undefined _typeof");
	check(u == u, "undefined == undefined");
	check(!(n == u), "number is not undefined");
	check(!(u == n), "undefined is not number");
}

void test_numbers() {
	as_value a(7), b(7), c(8);
	check(a == b, "7 == 7");
	check(!(a == c), "7 is not 8");
}

void test_null_object() {
	as_value null_obj(static_cast<gameswf::as_object*>(nullptr));
	check(std::strcmp(null_obj._typeof(), "null") == 0, "null _typeof");
	as_value u;
	check(null_obj == u, "null == undefined");
	check(u == null_obj, "undefined == null");
	as_value n(0);
	check(!(null_obj == n), "null is not a number");
	check(!(n == null_obj), "number is not null");
	check(g_add_ref == 0, "a null object takes no reference");
}

void test_object_reference() {
	alignas(8) char dummy[16];
	auto* obj = reinterpret_cast<gameswf::as_object*>(dummy);
	g_add_ref = 0;
	g_drop_ref = 0;
	{
		as_value v(obj);
		check(g_add_ref == 1, "an object takes one reference");
		check(g_last_add_ref == static_cast<const void*>(dummy),
		      "the reference goes to the object's ref_counted base (same address)");
		check(v == v, "an object equals itself");
	}
	check(g_drop_ref == 1, "destruction drops the object reference once");
	check(g_last_drop_ref == static_cast<const void*>(dummy),
	      "the drop goes to the object's ref_counted base (same address)");
}

void test_property() {
	as_value getter, setter;
	g_add_ref = 0;
	g_drop_ref = 0;
	g_property_ctor = 0;
	{
		as_value p(getter, setter);
		check(g_property_ctor == 1, "the property is built once");
		check(g_add_ref == 1, "the property takes one reference");
		check(p.is_property(), "a property value has type 6");
	}
	check(g_drop_ref == 1, "destruction releases the property");
}

}  // namespace

int main() {
	test_undefined_and_number();
	test_numbers();
	test_null_object();
	test_object_reference();
	test_property();
	if (g_failures != 0) {
		std::printf("%d failure(s)\n", g_failures);
		return 1;
	}
	std::printf("v1-vendor-gameswf-gameswf_as_value-794e76-0 test: ok\n");
	return 0;
}
