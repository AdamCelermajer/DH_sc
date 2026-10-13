// Test for batch v1-vendor-gameswf-bitmap_info_ogl-434136-0 (part 1 of gameswf::bitmap_info_ogl).
//   bash tools/trackb/check_tu.sh --test <this file> <part-1 .cpp> <part-2 .cpp> <gameswf_bitmap_info .cpp> <gameswf_ref_counted .cpp>   (all under vendor-gameswf/src/)
//
// Checks: p2 (0x7d3fcc), the filter-bit steps of set_min_filter (0x7d3ccc) and set_mag_filter (0x7d3d34) on
// the texture words, the s_filter_map identity table (0x90c044), and the constructed state of a bitmap that has no
// texture. Part 2 supplies the destructor and layout(); its body runs with no texture and no image here.
// The gameswf.h include chain needs the tu_string and free_internal stubs below (the same ones the bitmap_info
// test uses). The texture itself is not built: the stand-in ITexture has no width or flag words (M1).

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <new>

#include "core-platform/deps/swf_types.hpp"
#include "vendor-gameswf/bitmap_info_ogl.hpp"
#include "vendor-gameswf/bitmap_info_ogl_filter.hpp"

namespace {

int g_failures = 0;

#define CHECK(cond)                                                              \
	do {                                                                         \
		if (!(cond)) {                                                           \
			std::printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond);          \
			++g_failures;                                                        \
		}                                                                        \
	} while (0)

}  // namespace

namespace gameswf {

void free_internal(void* p, unsigned int size) {
	(void)size;
	std::free(p);
}

template <class T>
void destruct(const T* object) {
	delete const_cast<T*>(object);
}

template void destruct<ref_counted>(const ref_counted* object);

}  // namespace gameswf

void* operator new(std::size_t size, swf::MemHint) {
	return std::malloc(size);
}

// membuf::~membuf is defined in no batch linked here. The test never holds a membuf (m_membuf stays null), so
// release() never reaches it and the stub aborts if it is.
membuf::~membuf() { std::abort(); }

// The gameswf.h include chain emits inline tu_string members; this test never runs them.
bool tu_string::operator==(const tu_string&) const { std::abort(); }
void tu_string::operator+=(const char*) { std::abort(); }
void tu_string::operator+=(const tu_string&) { std::abort(); }
void tu_string::resize(int) { std::abort(); }
int tu_string::utf8_char_count(const char*, int) { std::abort(); }

int main() {
	using gameswf::bitmap_info_ogl;
	namespace filt = gameswf::bitmap_ogl_filter;

	// 1. p2: the smallest power of two that holds the value, halved when the value fills under 0.6 of it.
	CHECK(bitmap_info_ogl::p2(0) == 0);
	CHECK(bitmap_info_ogl::p2(1) == 1);
	CHECK(bitmap_info_ogl::p2(2) == 2);
	CHECK(bitmap_info_ogl::p2(3) == 4);   // 3/4 = 0.75
	CHECK(bitmap_info_ogl::p2(4) == 4);
	CHECK(bitmap_info_ogl::p2(5) == 8);   // 5/8 = 0.625
	CHECK(bitmap_info_ogl::p2(6) == 8);
	CHECK(bitmap_info_ogl::p2(8) == 8);
	CHECK(bitmap_info_ogl::p2(9) == 8);   // 9/16 = 0.5625, halved
	CHECK(bitmap_info_ogl::p2(16) == 16);
	CHECK(bitmap_info_ogl::p2(17) == 16); // 17/32 = 0.53, halved
	CHECK(bitmap_info_ogl::p2(-5) == 0);  // value at most 1: i = 1, then halved because -5 < 0.6

	// 2. The filter table is the identity over its six entries (s_filter_map, binary 0x90c044).
	for (int i = 0; i < 6; ++i) {
		CHECK(gameswf::bitmap_ogl_detail::kFilterMap[i] == i);
	}

	// 3. min step: writes bits 12..14 when the value differs and (mip > 1 or value <= 1); sets dirty bit 0x4.
	{
		std::uint32_t flags = 0;
		std::uint16_t dirty = 0;
		filt::min_filter_step(flags, dirty, 2, 3);
		CHECK(flags == 0x3000u);
		CHECK(dirty == 0x4u);
	}
	{
		std::uint32_t flags = 0x3000u;  // already value 3
		std::uint16_t dirty = 0;
		filt::min_filter_step(flags, dirty, 2, 3);
		CHECK(flags == 0x3000u);
		CHECK(dirty == 0u);
	}
	{
		std::uint32_t flags = 0;  // one mip and value 3: the mip test fails
		std::uint16_t dirty = 0;
		filt::min_filter_step(flags, dirty, 1, 3);
		CHECK(flags == 0u);
		CHECK(dirty == 0u);
	}
	{
		std::uint32_t flags = 0;  // one mip and value 1: the value test passes
		std::uint16_t dirty = 0;
		filt::min_filter_step(flags, dirty, 1, 1);
		CHECK(flags == 0x1000u);
		CHECK(dirty == 0x4u);
	}
	{
		std::uint32_t flags = 0x10000u;  // mag bits set; the min step must keep them
		std::uint16_t dirty = 0x8u;
		filt::min_filter_step(flags, dirty, 2, 3);
		CHECK(flags == 0x13000u);
		CHECK(dirty == 0xCu);
	}

	// 4. mag step: writes bits 15..17 when the value differs; no mip test; sets dirty bit 0x8.
	{
		std::uint32_t flags = 0;
		std::uint16_t dirty = 0;
		filt::mag_filter_step(flags, dirty, 2);
		CHECK(flags == 0x10000u);
		CHECK(dirty == 0x8u);
	}
	{
		std::uint32_t flags = 0x38000u;  // mag = 7
		std::uint16_t dirty = 0;
		filt::mag_filter_step(flags, dirty, 4);
		CHECK(flags == 0x20000u);
		CHECK(dirty == 0x8u);
	}
	{
		std::uint32_t flags = 0x20000u;  // mag already 4
		std::uint16_t dirty = 0;
		filt::mag_filter_step(flags, dirty, 4);
		CHECK(flags == 0x20000u);
		CHECK(dirty == 0u);
	}
	{
		std::uint32_t flags = 0x1000u;  // min = 1; mag value 0 matches the empty mag field, nothing changes
		std::uint16_t dirty = 0;
		filt::mag_filter_step(flags, dirty, 0);
		CHECK(flags == 0x1000u);
		CHECK(dirty == 0u);
	}

	// 5. A bitmap without a texture: the constructor state, and the calls that need no texture.
	{
		bitmap_info_ogl* b = new bitmap_info_ogl(nullptr, 64, 32, static_cast<const ::membuf*>(nullptr));
		CHECK(b->get_width() == 64);
		CHECK(b->get_height() == 32);
		CHECK(b->is_layout() == 0);
		CHECK(b->m_writable == false);
		CHECK(b->m_min_filter == 1);
		CHECK(b->m_mag_filter == 1);
		b->set_writable();
		CHECK(b->m_writable == true);
		b->set_min_filter(static_cast<gameswf::bitmap_info::filter_mode>(3));
		CHECK(b->m_min_filter == 3);
		b->set_mag_filter(static_cast<gameswf::bitmap_info::filter_mode>(4));
		CHECK(b->m_mag_filter == 4);
		b->activate();
		delete b;
	}
	{
		bitmap_info_ogl* d = new bitmap_info_ogl(nullptr);
		CHECK(d->get_width() == 0);
		CHECK(d->get_height() == 0);
		CHECK(d->m_driver == nullptr);
		delete d;
	}

	if (g_failures != 0) {
		std::printf("%d failure(s)\n", g_failures);
		return 1;
	}
	std::printf("bitmap_info_ogl part 1: all checks passed\n");
	return 0;
}
