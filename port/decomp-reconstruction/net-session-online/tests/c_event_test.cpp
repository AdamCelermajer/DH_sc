#include "net-session-online/c_event.hpp"

#include <cstdio>
#include <cstring>

#include "net-session-online/deps/platform_stand_ins.hpp"

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    OnlineClockStorage() = 1000;
    const char payload[] = "abcd";

    CEvent e(7, payload, sizeof payload);
    CHECK(e.Type() == 7);
    char out[8] = {};
    CHECK(e.CopyData(out, 8) == -1);
    CHECK(std::strcmp(out, "abcd") == 0);

    // Too small a destination: nothing copied, return value unchanged.
    char small[2] = {'x', 'x'};
    CHECK(e.CopyData(small, 2) == -1);
    CHECK(small[0] == 'x');

    // Expiry: fresh event is not expired; after more than 99 ms it is.
    CHECK(!e.IsExpired(1));
    OnlineClockStorage() = 1099;
    CHECK(!e.IsExpired(1));
    OnlineClockStorage() = 1100;
    CHECK(e.IsExpired(1));
    CHECK(!e.IsExpired(0));

    // Copy construction and assignment duplicate the payload.
    CEvent copy(e);
    CHECK(copy.Type() == 7);
    char out2[8] = {};
    CHECK(copy.CopyData(out2, 8) == -1);
    CHECK(std::strcmp(out2, "abcd") == 0);

    CEvent other(3, nullptr, 0);
    other = e;
    CHECK(other.Type() == 7);
    char out3[8] = {};
    other.CopyData(out3, 8);
    CHECK(std::strcmp(out3, "abcd") == 0);

    // An event without a payload copies nothing.
    CEvent empty(9, nullptr, 0);
    char out4[4] = {'z', 0, 0, 0};
    empty.CopyData(out4, 4);
    CHECK(out4[0] == 'z');

    std::printf("%s\n", failures ? "c_event: FAILED" : "c_event: ok");
    return failures != 0;
}
