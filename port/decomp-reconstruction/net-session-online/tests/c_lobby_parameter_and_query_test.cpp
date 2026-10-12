#include "net-session-online/c_lobby_parameter_and_query.hpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    // Empty object: nothing to pack.
    {
        CLobbyParameterAndQuery q;
        std::int32_t len = -1;
        CHECK(q.PackParameter(len) == nullptr);
        CHECK(len == 0);
    }

    // Three parameters, the middle one empty: count byte is 2, payload is the
    // two non-empty values in order.
    {
        CLobbyParameterAndQuery q;
        q.Reserve(3);
        q.StoreParameter(0, "ab", 2);
        q.StoreParameter(1, nullptr, 0);
        q.StoreParameter(2, "xyz", 3);
        std::int32_t len = 0;
        char* out = q.PackParameter(len);
        CHECK(out != nullptr);
        CHECK(len == 6);
        if (out) {
            CHECK(out[0] == 2);
            CHECK(std::memcmp(out + 1, "abxyz", 5) == 0);
            CHECK(out[6] == 0);
            delete[] out;
        }
    }

    // All parameters empty: nullptr, length 0.
    {
        CLobbyParameterAndQuery q;
        q.Reserve(2);
        std::int32_t len = 9;
        CHECK(q.PackParameter(len) == nullptr);
        CHECK(len == 0);
    }

    // Out-of-range index is ignored.
    {
        CLobbyParameterAndQuery q;
        q.Reserve(1);
        q.StoreParameter(5, "zz", 2);
        std::int32_t len = 0;
        CHECK(q.PackParameter(len) == nullptr);
    }

    if (failures == 0) std::printf("all checks passed\n");
    return failures == 0 ? 0 : 1;
}
