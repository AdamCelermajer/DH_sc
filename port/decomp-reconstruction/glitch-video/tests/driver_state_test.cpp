#include "glitch-video/material.hpp"
#include "glitch-video/texture_manager.hpp"
#include "glitch-video/video_driver.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

using namespace glitch::video;

class TestDriver : public IVideoDriver {};

int main() {
    // IVideoDriver options: set and clear bits.
    TestDriver driver;
    driver.setOption(0x3, true);
    CHECK(driver.getOptionFlags() == 0x3);
    driver.setOption(0x1, false);
    CHECK(driver.getOptionFlags() == 0x2);

    // Scene: beginScene returns true; endScene registers a frame (no crash, returns true).
    CHECK(driver.beginScene());
    CHECK(driver.endScene(1000));
    CHECK(driver.getPrimitiveCountDrawn(0) == 0);

    // Material destroyed: the cached material pointers are cleared.
    CMaterial* m = reinterpret_cast<CMaterial*>(0x1000);
    CMaterial* other = reinterpret_cast<CMaterial*>(0x2000);
    driver.onMaterialDestroyed(other);   // no match: nothing changes
    driver.onMaterialDestroyed(m);       // no match either

    // Render buffer list: unregister is a no-op when absent.
    driver.unregisterRenderBuffer(nullptr);

    // Texture manager: placeholder clears by type, and clearPlaceHolders needs refcount 1.
    CTextureManager manager;
    ITexture a, b;
    a.m_refCount = 1;
    b.m_refCount = 2;
    manager.m_placeHolders[1] = &a;
    manager.m_placeHolders[5] = &b;
    manager.clearPlaceHolder(1, &a);
    CHECK(manager.m_placeHolders[1] == nullptr);
    manager.clearPlaceHolders();
    CHECK(manager.m_placeHolders[5] == &b);   // refcount 2: kept
    manager.m_placeHolders[0] = &a;
    manager.clearPlaceHolders();
    CHECK(manager.m_placeHolders[0] == nullptr);

    // Writer registration bumps the reference count.
    IImageWriter writer;
    manager.addExternalImageWriter(&writer);
    CHECK(writer.m_refCount == 1 && manager.m_externalImageWriters.size() == 1);
    manager.addExternalImageWriter(nullptr);
    CHECK(manager.m_externalImageWriters.size() == 1);

    // CMaterial render-state helpers.
    CMaterial material;
    // The renderer is not used by the original body; any reference will do.
    char renderer = 0;
    CHECK(material.getRenderStateBlockSize(*reinterpret_cast<const CMaterialRenderer*>(&renderer)) == 0);

    std::printf("%s\n", failures ? "driver_state: FAILED" : "driver_state: ok");
    return failures != 0;
}
