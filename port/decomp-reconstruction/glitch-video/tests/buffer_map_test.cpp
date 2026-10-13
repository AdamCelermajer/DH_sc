// IBuffer::map (access overload), CBufferBase construction, the shadow-state defaults and
// CRenderTargetBase::getTargetCount.
#include "glitch-video/buffer.hpp"
#include "glitch-video/ccommon_gl_driver.hpp"
#include "video_test_types.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)



int main() {
    using namespace glitch::video;

    uint8_t payload[16] = {};

    // Data present, usage 0: maps to the data pointer and counts the map.
    test::TestBuffer plain(0, 0, 16, payload, 0);
    CHECK(plain.m_flags12 == 2);
    CHECK(plain.map(1) == payload);
    CHECK(plain.m_mapCount13 == 0x21);   // access 1 in bits 5..6, count 1
    CHECK(plain.map(1) == payload);
    CHECK(plain.m_mapCount13 == 0x22);   // count 2, access bits kept
    plain.unmap();
    CHECK((plain.m_mapCount13 & 0x1f) == 1);

    // No data and no flag 8: nothing is mapped and the count stays zero.
    test::TestBuffer noData(0, 0, 16, nullptr, 0);
    CHECK(noData.map(1) == nullptr);
    CHECK(noData.m_mapCount13 == 0);

    // Flag 8 with access up to 3 goes to the driver hook with access (a & 1) | 2.
    test::TestBuffer hooked(0, 0, 16, nullptr, 8);
    hooked.sentinel = payload;
    CHECK(hooked.map(1) == payload);
    CHECK(hooked.lastAccess == 3);
    CHECK(hooked.mapImplCalls == 1);
    CHECK(hooked.m_mapCount13 == 0);

    // Flag 8 with access above 3 and no data: still goes to the hook, with (a & 1) | 2.
    CHECK(hooked.map(5) == payload);
    CHECK(hooked.lastAccess == 3);
    CHECK(hooked.mapImplCalls == 2);

    // Flag 8 with data present and access above 3: maps the data and counts it.
    test::TestBuffer withData(0, 0, 16, payload, 8);
    CHECK(withData.map(5) == payload);
    CHECK(withData.mapImplCalls == 0);
    CHECK(withData.m_mapCount13 == 0xa1);  // access 5 << 5, count 1

    // A counted map with flag 0x20 returns through the getMappedPtr hook.
    test::TestBuffer counted(0, 0, 16, payload, 0x20);
    counted.sentinel = payload + 1;
    CHECK(counted.map(1) == payload);      // first map: plain data pointer
    CHECK(counted.map(1) == payload + 1);  // second map: counted path, hook
    CHECK(counted.mappedPtrCalls == 1);

    // Usage 4 keeps the flags unchanged after a map.
    test::TestBuffer usageFour(0, 4, 16, payload, 0);
    CHECK(usageFour.m_flags12 == 0);
    CHECK(usageFour.map(1) == payload);
    CHECK(usageFour.m_flags12 == 0);

    // CBufferBase: the IBuffer base is set up from the constructor arguments.
    CCommonGLDriver::CBuffer base(nullptr, 0, 0, 16, payload, false);
    CHECK(base.getMappedPtr() == nullptr);
    CHECK(base.map(1) == payload);

    // SCommonShadowState defaults: word +0x18 is 4, the others are zero.
    CCommonGLDriverBase::SCommonShadowState shadow;
    CHECK(shadow.m_words[6] == 4);
    bool othersZero = true;
    for (int i = 0; i < 12; ++i) {
        if (i != 6 && shadow.m_words[i] != 0) {
            othersZero = false;
        }
    }
    CHECK(othersZero);

    // getTargetCount: type 0 returns the count byte, 1 and 2 test the handles,
    // 3 needs both handles equal, anything else is zero.
    CCommonGLDriverBase::CRenderTargetBase target(nullptr);
    int handleA = 0;
    int handleB = 0;
    target.setTargetStateForTest(nullptr, nullptr, 3);
    CHECK(target.getTargetCount(0) == 3);
    CHECK(target.getTargetCount(1) == 0);
    CHECK(target.getTargetCount(2) == 0);
    CHECK(target.getTargetCount(3) == 0);
    target.setTargetStateForTest(&handleA, nullptr, 0);
    CHECK(target.getTargetCount(1) == 1);
    CHECK(target.getTargetCount(2) == 0);
    CHECK(target.getTargetCount(3) == 0);
    target.setTargetStateForTest(&handleA, &handleA, 0);
    CHECK(target.getTargetCount(3) == 1);
    target.setTargetStateForTest(&handleA, &handleB, 0);
    CHECK(target.getTargetCount(2) == 1);
    CHECK(target.getTargetCount(3) == 0);
    CHECK(target.getTargetCount(9) == 0);

    std::printf("%s\n", failures ? "buffer_map: FAILED" : "buffer_map: ok");
    return failures != 0;
}
