// Tests for the DH2 tu_string bodies in src/v1-vendor-gameswf-gameswf_tu_string-39e5bd-0.cpp.
// The expected hash values were computed with an independent script of the binary recurrence
// (tu_string::get_hashi, 0x7635c8), not with the code under test.

#include <cstdio>

#include "base/container.h"

namespace {

int g_failures = 0;

void check(bool ok, const char* what, int line) {
	if (!ok) {
		std::printf("FAIL line %d: %s\n", line, what);
		++g_failures;
	}
}

#define CHECK(expr) check((expr), #expr, __LINE__)

// operator+=(char const*), 0x7521cc
void test_append_c_string() {
	tu_string s("abc");
	s += "def";
	CHECK(s.length() == 6);
	CHECK(s == "abcdef");
	s += "";
	CHECK(s.length() == 6);
	CHECK(s == "abcdef");

	// Long text: the append grows the buffer past the inline size.
	tu_string long_text("0123456789");
	long_text += "abcdefghijklmnopqrstuvwxyz";
	CHECK(long_text.length() == 36);
	CHECK(long_text == "0123456789abcdefghijklmnopqrstuvwxyz");
}

// operator+=(char), 0x7a3cf4
void test_append_char() {
	tu_string s("ab");
	s += 'c';
	CHECK(s == "abc");
	s += static_cast<char>(0);  // a zero character is ignored by the binary
	CHECK(s.length() == 3);
	CHECK(s == "abc");
	s += 'd';
	CHECK(s == "abcd");
}

// operator+=(gameswf::tu_string const&), 0x753120
void test_append_string() {
	tu_string a("ab");
	tu_string b("cd");
	a += b;
	CHECK(a == "abcd");
	CHECK(b == "cd");

	tu_string empty;
	empty += a;
	CHECK(empty == "abcd");
	empty += tu_string();
	CHECK(empty == "abcd");
}

// operator==(gameswf::tu_string const&), 0x796f18
void test_equal_tu_string() {
	tu_string a("hello");
	tu_string b("hello");
	tu_string c("world");
	CHECK(a == b);
	CHECK(!(a == c));
	CHECK(a != c);
	CHECK(a == a);
	// The binary compares with strcmp, so text after an embedded NUL is not compared.
	tu_string x("a\0b", 3);
	tu_string y("a\0c", 3);
	CHECK(x == y);
}

// get_hashi() const, 0x7635c8
void test_hashi() {
	tu_string empty;
	CHECK(empty.get_hashi() == 5381);
	CHECK(tu_string("a").get_hashi() == 177604);
	CHECK(tu_string("abc").get_hashi() == -7910651);
	CHECK(tu_string("ABC").get_hashi() == -7910651);  // 'A'..'Z' are folded to lower case
	CHECK(tu_string("hello").get_hashi() == -3169369);
	CHECK(tu_string("world").get_hashi() == -3377881);
	CHECK(tu_string("aaa").get_hashi() == -7916892);  // bit 23 set: result is sign-extended
	CHECK(tu_string("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz").get_hashi() == -6051740);
	CHECK(tu_string("Dungeon Hunter 2 long heap string").get_hashi() == 5444248);

	// The hash is recomputed from the text, so an append changes the result.
	tu_string s("a");
	CHECK(s.get_hashi() == 177604);
	s += "b";
	CHECK(s.get_hashi() == 5861062);
	s += "cdef";
	CHECK(s.get_hashi() == -7992446);
}

}  // namespace

int main() {
	test_append_c_string();
	test_append_char();
	test_append_string();
	test_equal_tu_string();
	test_hashi();
	if (g_failures == 0) {
		std::printf("tu_string checks passed\n");
	}
	return g_failures == 0 ? 0 : 1;
}
