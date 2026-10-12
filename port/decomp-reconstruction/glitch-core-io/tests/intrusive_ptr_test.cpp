// boost::intrusive_ptr over the glitch reference-counted types: copies add a use,
// assignment moves the use, and the last drop deletes the object.
#include "glitch-core-io/boost/intrusive_ptr.hpp"
#include "glitch-core-io/io/attributes.hpp"
#include "glitch-core-io/io/typed_attributes.hpp"

#include <cstdio>

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

namespace {

struct Counted : glitch::video::ITexture {
    static int alive;
    Counted() { ++alive; }
    ~Counted() { --alive; }
};
int Counted::alive = 0;

}  // namespace

int main() {
    {
        boost::intrusive_ptr<glitch::video::ITexture> a(new Counted());
        CHECK(Counted::alive == 1 && a->referenceCounter == 1);
        {
            boost::intrusive_ptr<glitch::video::ITexture> b(a);
            CHECK(a.get() == b.get() && a->referenceCounter == 2);
        }
        CHECK(a->referenceCounter == 1);
        boost::intrusive_ptr<glitch::video::ITexture> c;
        c = a;
        CHECK(a->referenceCounter == 2);
        c = boost::intrusive_ptr<glitch::video::ITexture>();
        CHECK(a->referenceCounter == 1);
    }
    CHECK(Counted::alive == 0);

    // The IAttribute base returns null textures and lights.
    glitch::io::CTextureAttribute texture;
    CHECK(!texture.getTexture() && !texture.getLight());
    CHECK(!texture.getBool());
    glitch::io::CLightAttribute light;
    CHECK(!light.getBool());

    // CMaterial and CMaterialRenderer count at +0x00. Stack objects start with two uses
    // so that releasing one does not delete them.
    glitch::video::CMaterial material;
    material.referenceCounter = 2;
    {
        boost::intrusive_ptr<glitch::video::CMaterial> held(&material);
        CHECK(material.referenceCounter == 2);
    }
    CHECK(material.referenceCounter == 1);
    glitch::video::CMaterialRenderer renderer;
    renderer.referenceCounter = 2;
    {
        boost::intrusive_ptr<glitch::video::CMaterialRenderer> held(&renderer);
    }
    CHECK(renderer.referenceCounter == 1);
    // The last release of a heap material deletes it (no count is read afterwards).
    {
        boost::intrusive_ptr<const glitch::video::CMaterial> last(new glitch::video::CMaterial);
    }

    std::printf("%s\n", failures ? "intrusive_ptr: FAILED" : "intrusive_ptr: ok");
    return failures != 0;
}
