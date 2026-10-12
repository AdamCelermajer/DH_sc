#include "net-transport/buffer.hpp"

#include <cstdio>
#include <cstring>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    {
        Buffer b;
        b.setSize(16);
        CHECK(b.size() == 16);
        const char msg[] = "hello";
        b.write(3, msg, sizeof(msg));
        char out[sizeof(msg)] = {};
        b.read(3, out, sizeof(msg));
        CHECK(std::strcmp(out, "hello") == 0);
        b.clear();
        CHECK(b.size() == 0);
    }
    {
        // Writes that cross block boundaries are clipped to the block (as the original does).
        Buffer b;
        b.setSize(8);
        std::uint8_t data[8] = {1, 2, 3, 4, 5, 6, 7, 8};
        b.write(4, data, 8);
        CHECK(b.size() == 12);
        std::uint8_t back[4] = {};
        b.read(4, back, 4);
        CHECK(back[0] == 1 && back[3] == 4);
    }
    std::printf("%s\n", failures ? "buffer: FAILED" : "buffer: ok");
    return failures != 0;
}
