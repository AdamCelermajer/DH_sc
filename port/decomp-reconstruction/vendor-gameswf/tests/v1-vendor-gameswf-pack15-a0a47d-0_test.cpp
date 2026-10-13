// Test for batch v1-vendor-gameswf-pack15-a0a47d-0 (image_base, image_rgba, image_rgb, tu_file::read_fully,
// tu_loadlib).
//   bash tools/trackb/check_tu.sh --test <this file> port/decomp-reconstruction/vendor-gameswf/src/v1-vendor-gameswf-pack15-a0a47d-0.cpp
//
// The test supplies the callees that belong to other batches or to upstream files this batch does not link,
// so the constructor arithmetic, pixel store, read loop and stream stubs can run on their own:
//   gameswf::image_base::~image_base (another batch): frees m_data and clears it, as the listing does
//   gameswf::scanline, both overloads (0x7b526c, 0x7b527c): m_data + m_pitch * y
//   operator new[](size, swf::MemHint) (0x752ba4): malloc(), with the last size recorded
//   tu_file constructor (upstream base/tu_file.cpp): stores the callbacks. The initial error field comes from the
//       stream, so a test can start with an error set
//   tu_file destructor (upstream base/tu_file.cpp): nothing
//   membuf constructor and destructor (upstream base/membuf.cpp): a malloc'd buffer of the given size
//   tu_loadlib constructor (upstream base/tu_loadlib.cpp): no library loaded
//   tu_string::resize and tu_string::utf8_char_count (upstream base/container.cpp): the MSVC/MinGW dllexport on
//       exported_module makes every tu_string member inline-emitted in each TU that includes container.h, so these
//       two are referenced even though this test never calls a string member. They abort if reached.
// Link line: the batch source and the tu_string batch source (src/v1-vendor-gameswf-gameswf_tu_string-39e5bd-0.cpp).

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "base/membuf.h"
#include "base/tu_file.h"
#include "base/tu_loadlib.h"
#include "core-platform/deps/swf_types.hpp"
#include "vendor-gameswf/image_rgb.hpp"
#include "vendor-gameswf/image_rgba.hpp"

namespace {

int g_failures = 0;
int g_image_base_dtor_calls = 0;
int g_new_calls = 0;
std::size_t g_last_new_size = 0;

#define CHECK(cond)                                                    \
    do {                                                               \
        if (!(cond)) {                                                 \
            std::printf("FAIL %s:%d: %s\n", __FILE__, __LINE__, #cond); \
            ++g_failures;                                              \
        }                                                              \
    } while (0)

// A byte stream for tu_file. read() returns at most `chunk` bytes per call. The error field starts at `error`.
struct fake_stream {
    const std::uint8_t* data;
    int size;
    int pos;
    int chunk;
    int error;
    int reads;
};

int fake_read(void* dst, int bytes, void* appdata) {
    fake_stream* s = static_cast<fake_stream*>(appdata);
    ++s->reads;
    int n = bytes;
    if (n > s->chunk) {
        n = s->chunk;
    }
    if (n > s->size - s->pos) {
        n = s->size - s->pos;
    }
    if (n < 0) {
        n = 0;
    }
    std::memcpy(dst, s->data + s->pos, static_cast<std::size_t>(n));
    s->pos += n;
    return n;
}

bool fake_eof(void* appdata) {
    fake_stream* s = static_cast<fake_stream*>(appdata);
    return s->pos >= s->size;
}

}  // namespace

namespace gameswf {

image_base::~image_base() {
    ++g_image_base_dtor_calls;
    if (m_data != nullptr) {
        std::free(m_data);
        m_data = nullptr;
    }
}

std::uint8_t* scanline(image_base* surf, int y) {
    return surf->m_data + surf->m_pitch * y;
}

const std::uint8_t* scanline(const image_base* surf, int y) {
    return surf->m_data + surf->m_pitch * y;
}

}  // namespace gameswf

void* operator new[](std::size_t size, swf::MemHint) {
    ++g_new_calls;
    g_last_new_size = size;
    return std::malloc(size);
}

tu_file::tu_file(void* appdata, read_func rf, write_func wf, seek_func sf, seek_to_end_func ef, tell_func tf,
                 get_eof_func gef, close_func cf)
    : m_data(appdata),
      m_read(rf),
      m_write(wf),
      m_seek(sf),
      m_seek_to_end(ef),
      m_tell(tf),
      m_get_eof(gef),
      m_close(cf),
      m_error(appdata != nullptr ? static_cast<fake_stream*>(appdata)->error : 0) {}

tu_file::~tu_file() {}

membuf::membuf(const void* data, int size)
    : m_size(size), m_capacity(size), m_data(std::malloc(static_cast<std::size_t>(size))), m_read_only(false) {
    if (data != nullptr && size > 0) {
        std::memcpy(m_data, data, static_cast<std::size_t>(size));
    }
}

membuf::~membuf() {
    std::free(m_data);
}

tu_loadlib::tu_loadlib(const char* /*library_name*/) : m_hlib(nullptr) {}

// Stand-ins for upstream base/container.cpp. Never reached by this test (see the header comment).
void tu_string::resize(int) {
    std::abort();
}

int tu_string::utf8_char_count(const char*, int) {
    std::abort();
}

int main() {
    // 1. image_base constructor: the fields are stored as the listing stores them.
    {
        gameswf::image_base b(nullptr, 3, 2, 12, gameswf::image_base::RGB);
        CHECK(b.m_data == nullptr);
        CHECK(b.m_width == 3);
        CHECK(b.m_height == 2);
        CHECK(b.m_pitch == 12);
        CHECK(b.m_type == gameswf::image_base::RGB);
    }

    // 2. image_rgba constructor: pitch 4 * width, buffer pitch * height, type RGBA.
    {
        const int new_before = g_new_calls;
        gameswf::image_rgba a(3, 2);
        CHECK(g_new_calls == new_before + 1);
        CHECK(g_last_new_size == 24);
        CHECK(a.m_width == 3);
        CHECK(a.m_height == 2);
        CHECK(a.m_pitch == 12);
        CHECK(a.m_type == gameswf::image_base::RGBA);
        CHECK(a.m_data != nullptr);
        gameswf::image_rgba one(1, 1);
        CHECK(one.m_pitch == 4);
        CHECK(g_last_new_size == 4);
    }

    // 3. image_rgb constructor: pitch (3 * (width + 1)) & ~3, buffer pitch * height, type RGB.
    {
        const int expect_pitch[] = {0, 4, 8, 12, 12, 16, 20};  // widths 0..6
        for (int w = 1; w <= 6; ++w) {
            gameswf::image_rgb c(w, 1);
            CHECK(c.m_pitch == expect_pitch[w]);
            CHECK(c.m_pitch % 4 == 0);
            CHECK(c.m_pitch >= 3 * w);
            CHECK(c.m_type == gameswf::image_base::RGB);
        }
        gameswf::image_rgb d(4, 2);
        CHECK(d.m_width == 4);
        CHECK(d.m_height == 2);
        CHECK(g_last_new_size == 24);  // 12 * 2
        CHECK(d.m_data != nullptr);
    }

    // 4. Destructors: the base destructor runs once per object (the derived bodies are empty). Heap objects go
    //    through the compiler's deleting destructor.
    {
        const int before = g_image_base_dtor_calls;
        {
            gameswf::image_rgba b(2, 2);
            gameswf::image_rgb c(2, 2);
            CHECK(g_image_base_dtor_calls == before);
        }
        CHECK(g_image_base_dtor_calls == before + 2);

        gameswf::image_rgba* p = new gameswf::image_rgba(1, 1);
        gameswf::image_rgb* q = new gameswf::image_rgb(1, 1);
        delete p;
        delete q;
        CHECK(g_image_base_dtor_calls == before + 4);
    }

    // 5. image_rgba::set_pixel: byte 0..3 of pixel x in row y at m_pitch * y + 4 * x get r, g, b, a.
    {
        gameswf::image_rgba img(2, 2);
        std::memset(img.m_data, 0, static_cast<std::size_t>(img.m_pitch) * static_cast<std::size_t>(img.m_height));
        img.set_pixel(1, 1, 1, 2, 3, 4);
        const std::uint8_t* px = img.m_data + 8 + 4;  // row 1 starts at pitch 8, pixel 1 at +4
        CHECK(px[0] == 1);
        CHECK(px[1] == 2);
        CHECK(px[2] == 3);
        CHECK(px[3] == 4);
        int nonzero = 0;
        for (int i = 0; i < 16; ++i) {
            if (img.m_data[i] != 0) {
                ++nonzero;
            }
        }
        CHECK(nonzero == 4);
        img.set_pixel(0, 0, 9, 8, 7, 6);
        CHECK(img.m_data[0] == 9);
        CHECK(img.m_data[1] == 8);
        CHECK(img.m_data[2] == 7);
        CHECK(img.m_data[3] == 6);
    }

    // 6. tu_file::read_fully. The stream holds "0123456789".
    static const std::uint8_t kDigits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    {
        // bytes == -1: the membuf size (10) is read in chunks of 3 until the stream ends. The last step returns
        // the eof flag, 1.
        fake_stream s{kDigits, 10, 0, 3, 0, 0};
        tu_file f(&s, fake_read, nullptr, nullptr, nullptr, nullptr, fake_eof, nullptr);
        membuf out(nullptr, 10);
        int result = f.read_fully(&out, -1);
        CHECK(result == 1);
        CHECK(s.pos == 10);
        CHECK(s.reads == 4);
        CHECK(std::memcmp(out.data(), "0123456789", 10) == 0);
    }
    {
        // bytes == 4 with a large chunk: one read of 4, then nothing is left and the eof flag is 0.
        fake_stream s{kDigits, 10, 0, 10, 0, 0};
        tu_file f(&s, fake_read, nullptr, nullptr, nullptr, nullptr, fake_eof, nullptr);
        membuf out(nullptr, 4);
        int result = f.read_fully(&out, 4);
        CHECK(result == 0);
        CHECK(s.pos == 4);
        CHECK(s.reads == 1);
        CHECK(std::memcmp(out.data(), "0123", 4) == 0);
    }
    {
        // The stream ends before the request is full: 5 bytes arrive, then the eof flag ends the loop with 1.
        fake_stream s{kDigits, 5, 0, 8, 0, 0};
        tu_file f(&s, fake_read, nullptr, nullptr, nullptr, nullptr, fake_eof, nullptr);
        membuf out(nullptr, 8);
        int result = f.read_fully(&out, 8);
        CHECK(result == 1);
        CHECK(s.reads == 1);
        CHECK(std::memcmp(out.data(), "01234", 5) == 0);
    }
    {
        // A read error set before the call: one read happens, then the loop stops with that read's count.
        fake_stream s{kDigits, 10, 0, 3, 1, 0};
        tu_file f(&s, fake_read, nullptr, nullptr, nullptr, nullptr, fake_eof, nullptr);
        membuf out(nullptr, 10);
        int result = f.read_fully(&out, 10);
        CHECK(result == 3);
        CHECK(s.reads == 1);
        CHECK(s.pos == 3);
        CHECK(std::memcmp(out.data(), "012", 3) == 0);
    }
    {
        // bytes == 0: no read at all. The eof flag is 0 for a stream that has data.
        fake_stream s{kDigits, 10, 0, 3, 0, 0};
        tu_file f(&s, fake_read, nullptr, nullptr, nullptr, nullptr, fake_eof, nullptr);
        membuf out(nullptr, 1);
        int result = f.read_fully(&out, 0);
        CHECK(result == 0);
        CHECK(s.reads == 0);
        CHECK(s.pos == 0);
    }

    // 7. tu_loadlib: the listing's destructor is empty and get_function returns 0.
    {
        tu_loadlib lib("unused");
        CHECK(lib.get_function("anything") == nullptr);
    }

    if (g_failures != 0) {
        std::printf("%d check(s) failed\n", g_failures);
        return 1;
    }
    std::printf("all checks passed\n");
    return 0;
}
