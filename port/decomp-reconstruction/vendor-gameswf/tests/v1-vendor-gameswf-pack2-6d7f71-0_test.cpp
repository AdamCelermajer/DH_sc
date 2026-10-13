// Test for batch v1-vendor-gameswf-pack2-6d7f71-0. Returns non-zero when a check fails.
//
// Run: bash tools/trackb/check_tu.sh --test <this file> port/decomp-reconstruction/vendor-gameswf/src/v1-vendor-gameswf-pack2-6d7f71-0.cpp
//
// The batch .cpp defines the D1 destructors and the virtual methods of the tag classes, so its vtables refer to the
// virtuals that other batches own. This test defines those as empty stubs. It also stubs the allocator
// (free_internal and malloc_internal) and the membuf base (base/membuf.cpp is not in this batch). free_internal only
// records the call, so the test frees its own allocations. hash::clear reaches free through tu_free (base/utility.h).
//
// Covered: the execute_tag and place_object_2 return values (through virtual dispatch), counted_buffer's reference
// count and its destructor (zeroing below unk_14, ownership flag, word count for the free size), and the bucket
// count and rehash of the cxform and matrix tables (DH2 minimum 4, values kept across growth and clear).

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <new>

#include "base/container.h"
#include "base/membuf.h"
#include "core-platform/deps/swf_types.hpp"
#include "vendor-gameswf/counted_buffer.hpp"
#include "vendor-gameswf/execute_tags.hpp"
#include "vendor-gameswf/gameswf_alloc.hpp"
#include "vendor-gameswf/gameswf_hash_dh2.hpp"

namespace {

int g_failures = 0;
int g_free_calls = 0;
unsigned int g_last_free_size = 0xffffffffu;
void* g_last_free_ptr = nullptr;
int g_membuf_dtor_calls = 0;

void reset_counters() {
	g_free_calls = 0;
	g_last_free_size = 0xffffffffu;
	g_last_free_ptr = nullptr;
	g_membuf_dtor_calls = 0;
}

#define CHECK(cond)                                                              \
	do {                                                                         \
		if (!(cond)) {                                                           \
			std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
			++g_failures;                                                        \
		}                                                                        \
	} while (0)

// hash's only data member is m_table (a table* whose first two ints are the entry count and the size mask).
// Returns the size mask, or -1 when there is no table.
int table_mask(const void* hash_object) {
	void* table = *static_cast<void* const*>(hash_object);
	return table != nullptr ? static_cast<const int*>(table)[1] : -1;
}

int g_values[100];

}  // namespace

// ---- membuf base (upstream base/membuf.cpp, not in this batch) ----

membuf::membuf() : m_size(0), m_capacity(0), m_data(nullptr), m_read_only(false) {}

membuf::~membuf() {
	++g_membuf_dtor_calls;
	m_data = nullptr;
}

// ---- link stubs ----

namespace gameswf {

void* malloc_internal(unsigned int size, swf::MemHint) {
	return std::malloc(size ? size : 1);
}

void free_internal(void* p, unsigned int size) {
	++g_free_calls;
	g_last_free_ptr = p;
	g_last_free_size = size;
}

// Tag classes: the virtuals that the batch's vtables refer to and that other batches define.
void execute_tag::execute_state_reverse(character*, int) {}
bool execute_tag::is_remove_tag() const { return false; }
bool execute_tag::is_action_tag() const { return false; }
std::uint32_t execute_tag::get_depth_id_of_replace_or_add_tag() const { return 0; }

place_object_2::~place_object_2() {}
void place_object_2::execute(character*) {}
void place_object_2::execute_state(character*) {}
void place_object_2::execute_state_reverse(character*, int) {}
std::uint32_t place_object_2::get_depth_id_of_replace_or_add_tag() const { return 0; }

void remove_object_2::execute(character*) {}
void remove_object_2::execute_state(character*) {}
void remove_object_2::execute_state_reverse(character*, int) {}
bool remove_object_2::is_remove_tag() const { return true; }

void set_background_color::execute(character*) {}
void set_background_color::execute_state(character*) {}

}  // namespace gameswf

// tu_string members that container.h declares out of line. Inline code in container.h (the tu_string copy
// constructor, which the hash and membuf headers reach) refers to them, so this translation unit needs them to
// link. The test never calls them, so they abort (as in the fddef6-1 test).
void tu_string::resize(int) { std::abort(); }
void tu_string::operator+=(const char*) { std::abort(); }
void tu_string::operator+=(char) { std::abort(); }
void tu_string::operator+=(const tu_string&) { std::abort(); }
bool tu_string::operator==(const tu_string&) const { std::abort(); }
int tu_string::utf8_char_count(const char*, int) { std::abort(); }

// ---- tests ----

static void test_execute_tags() {
	gameswf::execute_tag tag;
	CHECK(tag.get_depth() == -1);
	CHECK(tag.get_action_offset() == -1);
	CHECK(!tag.is_place_tag());

	gameswf::place_object_2 place;
	place.m_depth = 7;
	CHECK(place.get_depth() == 7);
	CHECK(place.is_place_tag());

	place.m_depth = 0xFFFF;  // u16 at +0x0c: zero-extended, not sign-extended
	gameswf::execute_tag* base = &place;
	CHECK(base->get_depth() == 65535);
	CHECK(base->is_place_tag());
	CHECK(base->get_action_offset() == -1);  // inherited from execute_tag

	gameswf::remove_object_2 removed;
	gameswf::execute_tag* removed_base = &removed;
	CHECK(removed_base->is_remove_tag());
	CHECK(!removed_base->is_place_tag());
}

static void test_counted_buffer() {
	// drop_ref: count 2 -> 1 (no destruction); 1 -> 0 runs the destructor and then free_internal(this, 0).
	void* mem = std::malloc(sizeof(gameswf::counted_buffer));
	gameswf::counted_buffer* cb = new (mem) gameswf::counted_buffer();
	cb->m_ref_count = 2;
	reset_counters();
	cb->drop_ref();
	CHECK(cb->m_ref_count == 1);
	CHECK(g_free_calls == 0);
	CHECK(g_membuf_dtor_calls == 0);
	cb->drop_ref();
	CHECK(g_membuf_dtor_calls == 1);
	CHECK(g_free_calls == 1);
	CHECK(g_last_free_ptr == mem);
	CHECK(g_last_free_size == 0u);
	std::free(mem);  // free_internal only records

	// Destructor, not owned (unk_20 != 0) with unk_18 = -2: zeroes the two words before unk_14 and nothing else.
	std::uint32_t* base_words = static_cast<std::uint32_t*>(std::malloc(8 * sizeof(std::uint32_t)));
	for (int i = 0; i < 8; ++i) {
		base_words[i] = 0xAAAAAAAAu;
	}
	void* mem2 = std::malloc(sizeof(gameswf::counted_buffer));
	gameswf::counted_buffer* cb2 = new (mem2) gameswf::counted_buffer();
	cb2->unk_14 = base_words + 2;
	cb2->unk_18 = -2;
	cb2->unk_20 = 1;
	cb2->unk_1c = 6;
	reset_counters();
	cb2->~counted_buffer();
	CHECK(base_words[0] == 0u && base_words[1] == 0u);
	CHECK(base_words[2] == 0xAAAAAAAAu && base_words[7] == 0xAAAAAAAAu);
	CHECK(cb2->unk_18 == 0);
	CHECK(cb2->unk_14 == base_words + 2);  // not owned: pointer kept
	CHECK(cb2->unk_1c == 6);               // not owned: word count kept
	CHECK(g_free_calls == 0);
	CHECK(g_membuf_dtor_calls == 1);
	std::free(mem2);
	std::free(base_words);

	// Destructor, owned (unk_20 == 0), unk_18 = 0: frees unk_14 with size 4 * unk_1c and clears the fields.
	std::uint32_t* owned_words = static_cast<std::uint32_t*>(std::malloc(4 * sizeof(std::uint32_t)));
	void* mem3 = std::malloc(sizeof(gameswf::counted_buffer));
	gameswf::counted_buffer* cb3 = new (mem3) gameswf::counted_buffer();
	cb3->unk_14 = owned_words;
	cb3->unk_18 = 0;
	cb3->unk_20 = 0;
	cb3->unk_1c = 4;
	reset_counters();
	cb3->~counted_buffer();
	CHECK(g_free_calls == 1);
	CHECK(g_last_free_ptr == static_cast<void*>(owned_words));
	CHECK(g_last_free_size == 16u);
	CHECK(cb3->unk_14 == nullptr);
	CHECK(cb3->unk_1c == 0);
	CHECK(cb3->unk_18 == 0);
	std::free(owned_words);
	std::free(mem3);

	// Owned with a null pointer: no free call; the word count is still cleared.
	void* mem4 = std::malloc(sizeof(gameswf::counted_buffer));
	gameswf::counted_buffer* cb4 = new (mem4) gameswf::counted_buffer();
	cb4->unk_14 = nullptr;
	cb4->unk_20 = 0;
	cb4->unk_1c = 3;
	reset_counters();
	cb4->~counted_buffer();
	CHECK(g_free_calls == 0);
	CHECK(cb4->unk_1c == 0);
	std::free(mem4);
}

// Bucket count and rehash for the cxform and matrix tables. set_capacity(n) asks for raw capacity 3n/2, which
// reaches set_raw_capacity. The DH2 minimum is 4 buckets (upstream uses 16), so set_capacity(4) (raw 6, log2(5)+1 = 3.3)
// gives 8 buckets, mask 7.
static void test_cxform_hash() {
	typedef hash<int, gameswf::cxform*, fixed_size_hash<int> > cxform_hash;
	cxform_hash h;
	h.set_capacity(4);
	CHECK(table_mask(&h) == 7);
	h.set_capacity(4);  // same bucket count: no rebuild
	CHECK(table_mask(&h) == 7);

	cxform_hash small;
	small.set_capacity(1);  // raw 1: minimum 4 buckets
	CHECK(table_mask(&small) == 3);
	small.set_capacity(0);  // raw 0: clear
	CHECK(table_mask(&small) == -1);
	CHECK(small.is_empty());

	// Growth keeps every value. Keys are 1 + 7*i.
	cxform_hash grown;
	for (int i = 0; i < 100; ++i) {
		grown.add(1 + 7 * i, reinterpret_cast<gameswf::cxform*>(&g_values[i]));
	}
	CHECK(grown.size() == 100);
	for (int i = 0; i < 100; ++i) {
		gameswf::cxform* out = nullptr;
		CHECK(grown.get(1 + 7 * i, &out));
		CHECK(out == reinterpret_cast<gameswf::cxform*>(&g_values[i]));
	}
	CHECK(!grown.get(3, nullptr));  // 3 is not a key
}

static void test_matrix_hash() {
	typedef hash<int, gameswf::matrix*, fixed_size_hash<int> > matrix_hash;
	matrix_hash h;
	h.set_capacity(4);
	CHECK(table_mask(&h) == 7);

	matrix_hash small;
	small.set_capacity(2);  // raw 3: minimum 4 buckets
	CHECK(table_mask(&small) == 3);

	matrix_hash grown;
	for (int i = 0; i < 100; ++i) {
		grown.add(1 + 7 * i, reinterpret_cast<gameswf::matrix*>(&g_values[i]));
	}
	CHECK(grown.size() == 100);
	for (int i = 0; i < 100; ++i) {
		gameswf::matrix* out = nullptr;
		CHECK(grown.get(1 + 7 * i, &out));
		CHECK(out == reinterpret_cast<gameswf::matrix*>(&g_values[i]));
	}
}

int main() {
	test_execute_tags();
	test_counted_buffer();
	test_cxform_hash();
	test_matrix_hash();
	if (g_failures != 0) {
		std::fprintf(stderr, "%d check(s) failed\n", g_failures);
		return 1;
	}
	std::printf("pack2-6d7f71-0: all checks passed\n");
	return 0;
}
