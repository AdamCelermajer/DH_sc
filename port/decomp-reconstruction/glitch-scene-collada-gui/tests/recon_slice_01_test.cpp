#include "glitch-scene-collada-gui/recon/slice_01.hpp"

#include <cstdio>

using namespace glitch::gui;
using namespace glitch::gui::recon_slice_01;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

namespace {
int destroyed = 0;
struct Counted : Recon01RefCounted {
    ~Counted() override { ++destroyed; }
};
}  // namespace

int main() {
    // STTFont ordering: name first, then key on equal names.
    STTFontKey a{"Arial", 1};
    STTFontKey b{"Arial", 2};
    STTFontKey c{"Verdana", 0};
    STTFontKey shorter{"Ari", 9};
    CHECK(a < b);
    CHECK(!(b < a));
    CHECK(a < c);
    CHECK(shorter < a);
    CHECK(!(a < shorter));

    // setToolTipText writes the wide string.
    IGUIElement element;
    setToolTipText(element, L"tip");
    CHECK(element.toolTipText == L"tip");

    // registerGUIElementFactory grabs and records the factory; null is ignored.
    std::vector<void*> factories;
    Counted* factory = new Counted();
    registerGUIElementFactory(factories, factory);
    registerGUIElementFactory(factories, nullptr);
    CHECK(factories.size() == 1);
    CHECK(factory->referenceCount == 2);
    recon01Drop(factory);  // release the grab; the list still holds one
    CHECK(destroyed == 0);

    // setSpriteBank drops the old bank and grabs the new one.
    Counted* oldBank = new Counted();
    Counted* newBank = new Counted();
    Recon01RefCounted* slot = oldBank;
    setSpriteBank(slot, newBank);
    CHECK(slot == newBank);
    CHECK(newBank->referenceCount == 2);
    CHECK(destroyed == 1);  // old bank had one reference and was dropped
    recon01Drop(newBank);

    if (failures == 0) {
        std::printf("recon_slice_01_test: OK\n");
    }
    return failures == 0 ? 0 : 1;
}
