// Dirty-bit bitmap of ITexture::setDataDirty, ITexture::unmap lock counting, and
// CTextureManager::removeTexture list handling.
#include "glitch-video/texture.hpp"
#include "glitch-video/texture_manager.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    using namespace glitch::video;

    // Two mip levels, one face: bitmap words start after mip index 2 (levels + 1 == index 3).
    int32_t levels[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    ITexture tex;
    tex.m_levelSizes = levels;
    tex.m_mipCount3e = 2;
    tex.m_flags38 = 0;  // not cube
    tex.m_flags3f = 0;  // per-mip sizes, no range
    tex.setDataDirty(false);
    CHECK(tex.m_dirtyFlags40 == 0);
    CHECK(levels[3] == 0);

    tex.setDataDirty(true);
    CHECK((tex.m_dirtyFlags40 & 1) != 0);
    CHECK(static_cast<uint32_t>(levels[3]) == 0xffffffffu);

    // Range form: bits at 0 and mipCount for each face (one face here: bit 0 only).
    int32_t range[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    ITexture ranged;
    ranged.m_levelSizes = range;
    ranged.m_mipCount3e = 3;
    ranged.m_flags3f = 2;
    ranged.setDataDirty(true);
    CHECK(range[4] == 1);

    // Cube form: six faces, bits at 0, 2, 4, 6, 8, 10 for mipCount 2.
    int32_t cube[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    ITexture cubeTex;
    cubeTex.m_levelSizes = cube;
    cubeTex.m_mipCount3e = 2;
    cubeTex.m_flags38 = 2;
    cubeTex.m_flags3f = 2;
    cubeTex.setDataDirty(true);
    CHECK(static_cast<uint32_t>(cube[3]) == ((1u << 0) | (1u << 2) | (1u << 4) | (1u << 6) | (1u << 8) | (1u << 10)));

    // unmap: a count above one decrements, otherwise the state clears.
    ITexture locked;
    locked.m_lockCount42 = 2;
    locked.m_flags3f = 0x40;
    locked.unmap();
    CHECK((locked.m_lockCount42 & 0x1f) == 1);
    CHECK((locked.m_flags3f & 0x40) != 0);
    locked.unmap();
    CHECK(locked.m_lockCount42 == 0);
    CHECK((locked.m_flags3f & 0x40) == 0);

    // removeTexture: present textures are removed, absent or null ones report zero.
    CTextureManager manager;
    ITexture a;
    ITexture b;
    a.m_flags38 = 1;
    manager.m_textures = {&a, &b};
    manager.m_placeHolders[1] = &a;
    CHECK(manager.removeTexture(&a) == 1);
    CHECK(manager.m_textures.size() == 1 && manager.m_textures[0] == &b);
    CHECK(manager.m_placeHolders[1] == nullptr);
    CHECK(manager.removeTexture(&a) == 0);
    CHECK(manager.removeTexture(nullptr) == 0);

    std::printf("%s\n", failures ? "texture_dirty: FAILED" : "texture_dirty: ok");
    return failures != 0;
}
