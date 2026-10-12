// Wave 09 test: checks the three reconstructed identity bodies.
#include "glitch-scene-collada-gui/recon/v2/wave_09.hpp"

#include <cstdio>
#include <cstring>

using namespace glitch::collada::recon_v2_09;

static int failures = 0;

static void check(bool ok, const char* what) {
    if (!ok) {
        std::printf("FAIL: %s\n", what);
        ++failures;
    }
}

static void test_one(void* (*fn)(int, void*), const char* name) {
    unsigned char buf[32];
    std::memset(buf, 0xAB, sizeof buf);
    void* ret = fn(0, buf + 8);
    check(ret == buf + 8, name);
    for (int i = 0; i < 8; ++i) check(buf[i] == 0xAB, name);
    for (int i = 8; i < 24; ++i) check(buf[i] == 0, name);
    for (int i = 24; i < 32; ++i) check(buf[i] == 0xAB, name);
}

int main() {
    test_one(identity_618e88, "identity_618e88");
    test_one(identity_618e98, "identity_618e98");
    test_one(identity_618f18, "identity_618f18");
    if (failures == 0) std::printf("PASS: recon_v2_wave_09 (3 bodies)\n");
    return failures == 0 ? 0 : 1;
}
