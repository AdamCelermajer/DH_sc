#include "core-platform/accelerometer.hpp"
#include "core-platform/filesystem_win32.hpp"
#include "core-platform/slim_text.hpp"
#include "core-platform/touch_screen_base.hpp"
#include "core-platform/touch_screen_win32.hpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

// Test stubs for the engine-owned tick source that AccelerometerBase does not use.
int ApplicationGetDt() { return 0; }

int main() {
    // slim: utf8 -> code points, detect, back to utf8 (ASCII and a 3-byte sequence).
    const char text[] = "A\xE2\x82\xAC";  // 'A' and U+20AC
    std::uint32_t cps[4] = {};
    int n = slim::utf8toutf16(text, 4, cps, 4);
    CHECK(n == 2);
    CHECK(cps[0] == 'A');
    CHECK(slim::detectUtf8(text, 4));
    CHECK(!slim::detectUtf8("\xC3", 1));
    char back[8] = {};
    slim::utf16toutf8(cps, 2, back, sizeof back);
    CHECK(std::strcmp(back, text) == 0);
    CHECK(!slim::XmlDocument().loadFromStream());
    CHECK(slim::XmlDocument().save("x"));

    // TouchScreenBase slot allocation and queries.
    struct Touch : TouchScreenBase {
        void setSlot(int i, bool touched, bool valid) {
            slots_[i].touched_ = touched;
            slots_[i].valid_ = valid;
        }
    };
    Touch t;
    CHECK(!t.hasTouch());
    CHECK(t._IsQueueEmpty());
    CHECK(t.getID(7) == 0);
    CHECK(t.getID(9) == 0);  // getID does not mark slots touched, so slot 0 stays first free
    CHECK(t.getID(7) == 0);
    t.setSlot(1, true, true);
    CHECK(t.hasTouch());
    CHECK(t.getTouchPoint(1) != nullptr);
    CHECK(t.getTouchPoint(2) == nullptr);
    t.clear();
    CHECK(!t.hasTouch());
    CHECK(TouchScreenWin32().getLeftBound() == 0);

    // Accelerometer: iPhone negates each axis before storing.
    AccelerometerIPhone acc;
    acc.moved(1.f, -2.f, 3.f);
    acc.update(0.0);
    acc.clear();

    // FileSystemWin32 handle: size, seek, tell.
    std::FILE* f = std::tmpfile();
    CHECK(f != nullptr);
    if (f) {
        std::fputs("0123456789", f);
        FileSystemWin32::_FileHandle h;
        h.file_ = f;
        h.canRead_ = true;
        CHECK(h.canRead() && !h.canWrite());
        CHECK(h.size() == 10);
        h.seek(4);
        CHECK(h.tell() == 4);
        CHECK(h.size() == 10);
        std::fclose(f);
    }
    std::printf("%s\n", failures ? "win32_slim: FAILED" : "win32_slim: ok");
    return failures != 0;
}
