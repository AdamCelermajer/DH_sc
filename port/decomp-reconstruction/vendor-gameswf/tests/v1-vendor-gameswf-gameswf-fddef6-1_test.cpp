// Test for batch v1-vendor-gameswf-gameswf-fddef6-1. Returns non-zero when a check fails.
//
// Run: bash tools/trackb/check_tu.sh --test <this file> <src/v1-vendor-gameswf-gameswf-fddef6-1.cpp>
//
// The batch .cpp links on its own with stubs for the pieces this test does not build: the allocator
// (fddef6-0 batch) and the stream reads (gameswf_stream.cpp). hash::clear reaches free through tu_free
// (base/utility.h defines dlfree as free), so the C library free is used and nothing is stubbed for it.
// The image types are destroyed only through a null pointer here, so no image_base body is needed.

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "base/jpeg.h"
#include "gameswf/gameswf_stream.h"
#include "vendor-gameswf/gameswf_dh2_runtime.hpp"
#include "vendor-gameswf/image_rgb.hpp"
#include "vendor-gameswf/image_rgba.hpp"

namespace {

int g_failures = 0;

#define CHECK(cond)                                                              \
	do {                                                                         \
		if (!(cond)) {                                                           \
			std::fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
			++g_failures;                                                        \
		}                                                                        \
	} while (0)

int g_malloc_calls = 0;
unsigned int g_last_malloc_size = 0;
int g_free_calls = 0;
unsigned int g_last_free_size = 0xffffffffu;
int g_read_u16_calls = 0;
int g_read_u8_calls = 0;
int g_read_u32_calls = 0;

void dummy_loader(gameswf::stream*, int, gameswf::movie_definition_sub*) {}
void other_loader(gameswf::stream*, int, gameswf::movie_definition_sub*) {}

}  // namespace

// ---- link stubs ----

namespace gameswf {

void* malloc_internal(unsigned int size, swf::MemHint) {
	++g_malloc_calls;
	g_last_malloc_size = size;
	return std::malloc(size);
}

void free_internal(void* p, unsigned int size) {
	++g_free_calls;
	g_last_free_size = size;
	std::free(p);
}

Uint16 stream::read_u16() {
	++g_read_u16_calls;
	return 0;
}

Uint8 stream::read_u8() {
	++g_read_u8_calls;
	return 0;
}

Uint32 stream::read_u32() {
	++g_read_u32_calls;
	return 0;
}

}  // namespace gameswf

// tu_string members that container.h declares out of line. The inline members of the hash and of the
// stream helpers reach them from this translation unit; the test never calls them, so they abort.
void tu_string::resize(int) { std::abort(); }
void tu_string::operator+=(const char*) { std::abort(); }
void tu_string::operator+=(char) { std::abort(); }
void tu_string::operator+=(const tu_string&) { std::abort(); }
bool tu_string::operator==(const tu_string&) const { std::abort(); }
int tu_string::utf8_char_count(const char*, int) { std::abort(); }

// ---- access to the private hash::set_raw_capacity (test only) ----

namespace {

typedef hash<int, gameswf::loader_function, fixed_size_hash<int> > loader_hash;
typedef loader_hash::entry loader_entry;
// hash::table is private: two ints (entry count, size mask), no padding.
typedef int loader_table_header[2];

// An explicit instantiation may name private members, so the address of set_raw_capacity is captured by
// the explicit instantiation below and read back through a friend function found by argument-dependent lookup.
struct loader_set_raw_capacity_tag {
	typedef void (loader_hash::*type)(int);
	friend type get_loader_set_raw_capacity(loader_set_raw_capacity_tag);
};

template <loader_set_raw_capacity_tag::type M>
struct loader_set_raw_capacity_access {
	friend loader_set_raw_capacity_tag::type get_loader_set_raw_capacity(loader_set_raw_capacity_tag) {
		return M;
	}
};

template struct loader_set_raw_capacity_access<&loader_hash::set_raw_capacity>;

void loader_set_raw(loader_hash& h, int raw_size) {
	(h.*get_loader_set_raw_capacity(loader_set_raw_capacity_tag()))(raw_size);
}

}  // namespace

// ---- checks ----

static void test_verbose_flag() {
	gameswf::set_verbose_dlist(false);
	CHECK(gameswf::get_verbose_dlist() == false);
	gameswf::set_verbose_dlist(true);
	CHECK(gameswf::get_verbose_dlist() == true);
	gameswf::set_verbose_dlist(false);
	CHECK(gameswf::get_verbose_dlist() == false);
}

static void test_texture_loader_callback() {
	gameswf::register_texture_loader_callback(nullptr);
	CHECK(gameswf::s_texture_loader_callback == nullptr);
	// Any non-null function pointer is stored as given. It is not called here.
	gameswf::texture_loader_callback fn = nullptr;
	gameswf::register_texture_loader_callback(fn);
	CHECK(gameswf::s_texture_loader_callback == fn);
}

static void test_destruct_null_is_noop() {
	int before = g_free_calls;
	gameswf::destructp<unsigned char>(nullptr, 16);
	gameswf::destruct<gameswf::image_rgb>(nullptr);
	gameswf::destruct<gameswf::image_rgba>(nullptr);
	gameswf::destruct<gameswf::ref_counted>(nullptr);
	gameswf::destruct<jpeg::input>(nullptr);
	CHECK(g_free_calls == before);
}

static void test_destructp_frees_with_size_zero() {
	unsigned char* block = static_cast<unsigned char*>(std::malloc(32));
	if (block == nullptr) {
		++g_failures;
		return;
	}
	std::memset(block, 0, 32);
	int before = g_free_calls;
	gameswf::destructp<unsigned char>(block, 32);
	CHECK(g_free_calls == before + 1);
	CHECK(g_last_free_size == 0u);  // binary passes 0 as the size
}

static void test_video_loader_reads_one_u16() {
	g_read_u16_calls = 0;
	unsigned char dummy = 0;
	gameswf::stream* in = reinterpret_cast<gameswf::stream*>(&dummy);  // never dereferenced by the stubs
	gameswf::define_video_loader(in, 60, nullptr);
	CHECK(g_read_u16_calls == 1);
	CHECK(g_read_u8_calls == 0);
}

static void test_native_image_loader_header_reads() {
	g_read_u16_calls = 0;
	g_read_u8_calls = 0;
	g_read_u32_calls = 0;
	unsigned char dummy = 0;
	gameswf::stream* in = reinterpret_cast<gameswf::stream*>(&dummy);
	gameswf::define_native_image_loader(in, 36, nullptr);
	// Partial body: the header is read in the binary's order (u16, u16, u16, u8, u32); the image data is not.
	CHECK(g_read_u16_calls == 3);
	CHECK(g_read_u8_calls == 1);
	CHECK(g_read_u32_calls == 1);
}

static void test_loader_hash_raw_capacity_table_sizes() {
	// Raw sizes 2^k + 1 are where the divisor matters. The binary's logf(2^k) / ln2 is exactly k, so the table
	// has 2^(k+1) entries. With a divisor 47 ulp too large (0.69315f) the quotient is k - 4e-6, and (int)(q + 1)
	// is k, not k + 1, so that build gives 2^k entries (5 -> 4, 9 -> 8, ...). Sizes 5, 17, 65 and 257 are not
	// reachable through set_capacity ((n * 3) / 2), so the private set_raw_capacity is called directly.
	struct raw_case {
		int raw;
		int entries;
	};
	const raw_case cases[] = {
		{1, 4}, {2, 4}, {3, 4}, {4, 4},    // 1, 2 and 3 clamp to the DH2 minimum of 4; 4 is 2^2
		{5, 8}, {8, 8}, {9, 16}, {16, 16}, // 5 = 2^2 + 1 -> 2^3
		{17, 32}, {33, 64}, {65, 128}, {129, 256}, {150, 256},
		{257, 512},
	};
	for (const raw_case& c : cases) {
		int failures_before = g_failures;
		loader_hash h;
		int mallocs = g_malloc_calls;
		loader_set_raw(h, c.raw);
		CHECK(g_malloc_calls == mallocs + 1);
		CHECK(g_last_malloc_size == sizeof(loader_table_header) + sizeof(loader_entry) * c.entries);
		if (g_failures != failures_before) {
			std::fprintf(stderr, "  (raw size %d, expected %d entries)\n", c.raw, c.entries);
		}
	}
}

static void test_loader_hash_capacity() {
	// set_capacity(n) reaches set_raw_capacity with raw size (n * 3) / 2 (public API).
	loader_hash h;
	int mallocs = g_malloc_calls;
	int frees = g_free_calls;

	// set_capacity(2) -> raw 3 -> DH2 minimum of 4 entries (upstream would give 16).
	h.set_capacity(2);
	CHECK(g_malloc_calls == mallocs + 1);
	CHECK(g_last_malloc_size == sizeof(loader_table_header) + sizeof(loader_entry) * 4);

	// Raw size 4 again: no reallocation.
	mallocs = g_malloc_calls;
	frees = g_free_calls;
	h.set_capacity(2);
	CHECK(g_malloc_calls == mallocs);
	CHECK(g_free_calls == frees);

	// Fill some entries, then grow: every entry must move.
	h.add(1, dummy_loader);
	h.add(2, other_loader);
	h.add(3, dummy_loader);
	CHECK(h.size() == 3);
	mallocs = g_malloc_calls;
	frees = g_free_calls;
	h.set_capacity(100);  // raw 150 -> 2^8 = 256 entries
	CHECK(g_malloc_calls == mallocs + 1);
	CHECK(g_free_calls == frees + 1);
	CHECK(g_last_malloc_size == sizeof(loader_table_header) + sizeof(loader_entry) * 256);
	// The old table was freed with sizeof(table) + sizeof(entry) * 4 (its mask plus one).
	CHECK(g_last_free_size == sizeof(loader_table_header) + sizeof(loader_entry) * 4);

	gameswf::loader_function out = nullptr;
	CHECK(h.get(1, &out) && out == dummy_loader);
	CHECK(h.get(2, &out) && out == other_loader);
	CHECK(h.get(3, &out) && out == dummy_loader);
	CHECK(!h.get(4, &out));
	CHECK(h.size() == 3);
}

static void test_loader_hash_set_capacity_raw_9() {
	// set_capacity(6) -> raw (6 * 3) / 2 = 9 -> 2^4 = 16 entries. The public path agrees with the direct call above.
	loader_hash h;
	int mallocs = g_malloc_calls;
	h.set_capacity(6);
	CHECK(g_malloc_calls == mallocs + 1);
	CHECK(g_last_malloc_size == sizeof(loader_table_header) + sizeof(loader_entry) * 16);
}

int main() {
	test_verbose_flag();
	test_texture_loader_callback();
	test_destruct_null_is_noop();
	test_destructp_frees_with_size_zero();
	test_video_loader_reads_one_u16();
	test_native_image_loader_header_reads();
	test_loader_hash_raw_capacity_table_sizes();
	test_loader_hash_capacity();
	test_loader_hash_set_capacity_raw_9();

	if (g_failures != 0) {
		std::fprintf(stderr, "%d check(s) failed\n", g_failures);
		return 1;
	}
	std::printf("all checks passed\n");
	return 0;
}
