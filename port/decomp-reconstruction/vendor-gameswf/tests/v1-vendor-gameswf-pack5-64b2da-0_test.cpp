// Test for batch v1-vendor-gameswf-pack5-64b2da-0 (hash<> set_raw_capacity explicit specializations).
//   bash tools/trackb/check_tu.sh --test <this file> \
//     port/decomp-reconstruction/vendor-gameswf/src/v1-vendor-gameswf-pack5-64b2da-0.cpp \
//     port/decomp-reconstruction/vendor-gameswf/upstream/gameswf-r1714/base/container.cpp \
//     port/decomp-reconstruction/vendor-gameswf/upstream/gameswf-r1714/base/utf8.cpp \
//     port/decomp-reconstruction/vendor-gameswf/src/v1-vendor-gameswf-gameswf_tu_string-39e5bd-0.cpp
// The last three give tu_string its out-of-line members (tu_stringi and the hash keys need them).
//
// Covers the state changes that run without the engine: growing and shrinking the tables with set_capacity, which goes
// through the DH2 set_raw_capacity bodies, keeps every entry and its value, and clear() empties the table.
// The slot count of each table is checked against the binary's sizing rule (ln 2 = the float 0x3F317218), at the
// power-of-two boundaries 2^k + 1, where the wrong divisor 0.69315f gives half the slots.
// The allocator is the test's own (std::malloc / std::free). The game's SwfAlloc is not linked here. as_value's
// out-of-line members are stubbed because the as_value instantiation is compiled into the batch source.

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "vendor-gameswf/gameswf_alloc.hpp"
#include "vendor-gameswf/gameswf_dh2_hash.hpp"

namespace {

int g_failures = 0;

#define CHECK(cond)                                                   \
	do {                                                              \
		if (!(cond)) {                                                \
			std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
			++g_failures;                                             \
		}                                                             \
	} while (0)

}  // namespace

// Test-only stand-ins for the game's allocator and for as_value members that live in another batch.
namespace gameswf {

void* malloc_internal(unsigned int size, swf::MemHint /*hint*/) { return std::malloc(size); }
void free_internal(void* p, unsigned int /*size*/) { std::free(p); }

void as_value::operator=(const as_value& /*v*/) {}
void as_value::drop_refs() {}

}  // namespace gameswf

// hash<int, tu_string>: values are short strings, so they stay in the value's local buffer.
static void test_int_tu_string() {
	hash<int, tu_string, fixed_size_hash<int> > h;
	CHECK(h.size() == 0);
	CHECK(h.is_empty());

	h.set_capacity(0);  // raw capacity 0: clear, nothing to do
	CHECK(h.size() == 0);

	h.set_capacity(3);  // raw capacity 4: the DH2 minimum
	for (int i = 0; i < 3; i++) {
		h.add(i, tu_string("v"));
	}
	CHECK(h.size() == 3);

	// Grow across several powers of two; every entry must survive each rehash.
	for (int i = 3; i < 200; i++) {
		h.add(i, tu_string("w"));
	}
	CHECK(h.size() == 200);

	h.set_capacity(1000);  // larger raw capacity, rehash keeps the contents
	CHECK(h.size() == 200);
	for (int i = 0; i < 200; i++) {
		tu_string value;
		CHECK(h.get(i, &value));
		CHECK(value.length() == 1);
	}
	CHECK(!h.get(500, nullptr));

	// Asking for a size the table already has is a no-op.
	h.set_capacity(1000);
	CHECK(h.size() == 200);

	h.clear();
	CHECK(h.is_empty());
	CHECK(h.size() == 0);
}

// hash<tu_stringi, bool>: string keys; the values alternate so a wrong rehash shows up in the lookups.
static void test_tu_stringi_bool() {
	hash<tu_stringi, bool, stringi_hash_functor<tu_stringi> > h;
	char key[16];
	for (int i = 0; i < 120; i++) {
		std::snprintf(key, sizeof(key), "key%d", i);
		h.add(tu_stringi(key), (i % 2) == 0);
	}
	CHECK(h.size() == 120);

	for (int i = 0; i < 120; i++) {
		std::snprintf(key, sizeof(key), "key%d", i);
		bool value = false;
		CHECK(h.get(tu_stringi(key), &value));
		CHECK(value == ((i % 2) == 0));
	}

	h.set_capacity(2);  // shrinks the raw capacity request, but never below the stored count
	CHECK(h.size() == 120);
}

// hash<gameswf::as_object*, bool>: the keys are never dereferenced, so fake, distinct addresses are enough.
static void test_as_object_pointer_bool() {
	hash<gameswf::as_object*, bool, fixed_size_hash<gameswf::as_object*> > h;
	for (int i = 0; i < 64; i++) {
		gameswf::as_object* key = reinterpret_cast<gameswf::as_object*>(static_cast<std::uintptr_t>(0x1000u + 16u * i));
		h.add(key, i < 32);
	}
	CHECK(h.size() == 64);
	for (int i = 0; i < 64; i++) {
		gameswf::as_object* key = reinterpret_cast<gameswf::as_object*>(static_cast<std::uintptr_t>(0x1000u + 16u * i));
		bool value = false;
		CHECK(h.get(key, &value));
		CHECK(value == (i < 32));
	}
}

// Slot count of a fresh table after set_raw_capacity(raw), through the shared body.
static int slots_for_raw_capacity(int raw) {
	hash<int, tu_string, fixed_size_hash<int> > h;
	dh2_hash_access::set_raw_capacity(h, raw);
	return dh2_hash_access::table_size(h);
}

// The binary's sizing rule: 1 << (int)(float)(logf(raw - 1) / ln2 + 1.0), minimum 4. Each request 2^k + 1 gives 2^(k + 1)
// slots (the 0.69315f divisor gives 2^k). The expected values below are the binary's.
static void test_table_sizes() {
	std::uint32_t ln2_bits = 0;
	std::memcpy(&ln2_bits, &dh2_hash_access::kLn2, sizeof(ln2_bits));
	CHECK(ln2_bits == 0x3F317218u);

	struct Case {
		int raw;
		int slots;
	};
	static const Case cases[] = {
		{1, 4},     {2, 4},     {3, 4},     {4, 4},       {5, 8},       {8, 8},     {9, 16},     {16, 16},   {17, 32},
		{33, 64},   {65, 128},  {129, 256}, {257, 512},   {513, 1024},  {1025, 2048}, {200, 256}, {1000, 1024},
	};
	for (const Case& c : cases) {
		CHECK(slots_for_raw_capacity(c.raw) == c.slots);
	}

	// set_capacity(22) requests raw capacity 3 * 22 / 2 = 33, which the binary gives 64 slots.
	hash<int, tu_string, fixed_size_hash<int> > sized;
	sized.set_capacity(22);
	CHECK(dh2_hash_access::table_size(sized) == 64);

	// The same body for a bool-valued instantiation: raw capacity 5 gives 8 slots.
	hash<tu_stringi, bool, stringi_hash_functor<tu_stringi> > flags;
	dh2_hash_access::set_raw_capacity(flags, 5);
	CHECK(dh2_hash_access::table_size(flags) == 8);
}

int main() {
	test_int_tu_string();
	test_tu_stringi_bool();
	test_as_object_pointer_bool();
	test_table_sizes();

	if (g_failures != 0) {
		std::fprintf(stderr, "%d failure(s)\n", g_failures);
		return 1;
	}
	std::printf("v1-vendor-gameswf-pack5-64b2da-0 test: all checks passed\n");
	return 0;
}
