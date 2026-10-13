// Test for batch v1-vendor-gameswf-pack11-4edfaf-0.
//   bash tools/trackb/check_tu.sh --test <this file> port/decomp-reconstruction/vendor-gameswf/src/v1-vendor-gameswf-pack11-4edfaf-0.cpp
//
// Covers:
//   gameswf::dh2::as_array_is_index_text (the text test under as_array::is_index, 0x79806c).
//   gameswf::standard_array_sorter::operator() (0x7986e4) with each option bit.
//
// The sorter reads as_value::to_number, as_value::to_tu_string and as_value's destructor (drop_refs), and it calls
// tu_string::stricmp. Those belong to other batches, so this file supplies stand-ins: numbers and strings come from a
// table keyed by the as_value's address. tu_string's own members used here are inline in base/container.h.

#include <cctype>
#include <cstdio>
#include <cstring>
#include <map>
#include <string>

#include "base/container.h"
#include "vendor-gameswf/as_value.hpp"
#include "vendor-gameswf/as_array_sorter.hpp"
#include "vendor-gameswf/as_array_dh2_helpers.hpp"

namespace {

int g_failures = 0;

void check(bool ok, const char* what) {
	if (!ok) {
		++g_failures;
		std::fprintf(stderr, "FAIL: %s\n", what);
	}
}

struct Entry {
	double number = 0.0;
	::tu_string* text = nullptr;  // the object to_tu_string returns; created on first use, never freed
};

std::map<const gameswf::as_value*, Entry> g_values;

Entry& entry_of(const gameswf::as_value* v) {
	return g_values[v];
}

::tu_string& text_of(const gameswf::as_value* v) {
	Entry& e = entry_of(v);
	if (e.text == nullptr) {
		e.text = new ::tu_string();
	}
	return *e.text;
}

void set_number(const gameswf::as_value& v, double n) {
	entry_of(&v).number = n;
}

// tu_string(const char*) is inline in base/container.h. It calls the exported tu_string::resize, which the test stubs
// as a no-op, and then copies the text into the inline local buffer (the test strings are short).
void set_text(const gameswf::as_value& v, const char* s) {
	entry_of(&v).text = new ::tu_string(s);
}

}  // namespace

// Stand-ins for callees outside this batch.
namespace gameswf {

double as_value::to_number() const {
	return entry_of(this).number;
}

const ::tu_string& as_value::to_tu_string() const {
	return text_of(this);
}

void as_value::drop_refs() {
}

}  // namespace gameswf

// Exported members of tu_string that inline code in base/container.h reaches. None of them is called by the sorter or
// by the test's own strings; they are stubbed only so the test links.
void tu_string::resize(int) {
}

void tu_string::operator+=(const char*) {
}

void tu_string::operator+=(const tu_string&) {
}

bool tu_string::operator==(const tu_string&) const {
	return false;
}

int tu_string::utf8_char_count(const char*, int) {
	return 0;
}

int tu_string::stricmp(const char* a, const char* b) {
	for (;; ++a, ++b) {
		const int ca = std::tolower(static_cast<unsigned char>(*a));
		const int cb = std::tolower(static_cast<unsigned char>(*b));
		if (ca != cb) {
			return ca - cb;
		}
		if (ca == 0) {
			return 0;
		}
	}
}

namespace {

void expect_index(const char* text, bool want) {
	const bool got = gameswf::dh2::as_array_is_index_text(text);
	if (got != want) {
		++g_failures;
		std::fprintf(stderr, "FAIL: is_index('%s') = %d, want %d\n", text, got ? 1 : 0, want ? 1 : 0);
	}
}

void test_is_index() {
	expect_index("", true);
	expect_index("0", true);
	expect_index("7", true);
	expect_index("123", true);
	expect_index("0042", true);
	expect_index("12a", false);
	expect_index("a1", false);
	expect_index("-1", false);
	expect_index(" 1", false);
	expect_index("1 ", false);
	expect_index("1.5", false);
	expect_index("length", false);
}

// Sorter result for one option set. A fresh standard_array_sorter has its pad bytes zeroed.
bool sort_less(int options, const gameswf::as_value& a, const gameswf::as_value& b) {
	gameswf::standard_array_sorter s{};
	s.m_options = options;
	return s(a, b);
}

void test_sorter() {
	gameswf::as_value apple;
	gameswf::as_value banana;
	gameswf::as_value upper_banana;
	set_text(apple, "apple");
	set_text(banana, "banana");
	set_text(upper_banana, "Banana");

	// Default: case-sensitive strcmp. "apple" > "Banana" because 'a' (97) > 'B' (66).
	check(sort_less(0, apple, banana), "default: apple < banana");
	check(!sort_less(0, banana, apple), "default: not banana < apple");
	check(!sort_less(0, apple, upper_banana), "default: apple is not before Banana (case-sensitive)");

	// CASEINSENSITIVE (1): compares "apple" with "banana" ignoring case.
	check(sort_less(1, apple, upper_banana), "CASEINSENSITIVE: apple < Banana");
	check(!sort_less(1, upper_banana, apple), "CASEINSENSITIVE: not Banana < apple");

	// DESCENDING (2): negates the order.
	check(!sort_less(2, apple, banana), "DESCENDING: not apple < banana");
	check(sort_less(2, banana, apple), "DESCENDING: banana < apple");

	// CASEINSENSITIVE | DESCENDING (3).
	check(!sort_less(3, apple, upper_banana), "CI|DESC: not apple < Banana");
	check(sort_less(3, upper_banana, apple), "CI|DESC: Banana < apple");

	// NUMERIC (16): numbers compare by value, so 2 < 10 even though "2" > "10" as text.
	gameswf::as_value two;
	gameswf::as_value ten;
	set_number(two, 2.0);
	set_number(ten, 10.0);
	set_text(two, "2");
	set_text(ten, "10");
	check(sort_less(16, two, ten), "NUMERIC: 2 < 10");
	check(!sort_less(16, ten, two), "NUMERIC: not 10 < 2");
	check(!sort_less(0, two, ten), "default text order: \"2\" is not before \"10\"");
	check(sort_less(16 | 2, ten, two), "NUMERIC|DESC: 10 before 2");

	// Equal text in different objects: the strcmp branch finds nothing, so the address order decides, and exactly
	// one of the two orders is true.
	gameswf::as_value same_a;
	gameswf::as_value same_b;
	set_text(same_a, "same");
	set_text(same_b, "same");
	check(sort_less(0, same_a, same_b) != sort_less(0, same_b, same_a), "equal text: one order wins");
	check(sort_less(2, same_a, same_b) != sort_less(2, same_b, same_a), "equal text, DESC: one order wins");
	check(sort_less(0, same_a, same_b) == (&same_a < &same_b), "equal text: address order");

	// The same object twice: the address order is false both ways.
	check(!sort_less(0, apple, apple), "same object: not before itself");
	check(!sort_less(1, apple, apple), "same object, CI: not before itself");
}

}  // namespace

int main() {
	test_is_index();
	test_sorter();
	if (g_failures != 0) {
		std::fprintf(stderr, "%d failure(s)\n", g_failures);
		return 1;
	}
	std::printf("ok\n");
	return 0;
}
