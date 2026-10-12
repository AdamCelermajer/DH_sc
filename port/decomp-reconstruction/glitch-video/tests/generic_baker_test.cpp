// CGenericBaker constructor: shader stored, attribute map unset and referenced once.
#include <cstdio>

#include "glitch-video/generic_baker.hpp"
#include "glitch-video/vertex_attribute_map.hpp"

using namespace glitch::video;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    CGenericBaker baker(nullptr);
    CHECK(baker.m_shader == nullptr);
    CHECK(baker.m_unknown10 == 0xffff);
    CHECK(baker.m_unknown14 == 0);
    CHECK(baker.m_refCount == 0);
    CHECK(baker.m_attributeMap != nullptr);
    CHECK(baker.m_attributeMap->m_unknown00 == 1);
    for (uint32_t i = 0; i < CVertexAttributeMap::kMapSize; ++i) {
        CHECK(baker.m_attributeMap->m_map[i] == 0xff);
    }
    delete baker.m_attributeMap;
    if (failures == 0) std::printf("generic_baker_test: ok\n");
    return failures ? 1 : 0;
}
