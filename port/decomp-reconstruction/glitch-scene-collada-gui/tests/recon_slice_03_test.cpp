// Slice 03 tests: sprite bank texture list and spin box format/parse.
#include <cstdio>

#include "glitch-scene-collada-gui/recon/slice_03.hpp"

using namespace glitch::collada::recon_03;

namespace {

class FakeEnvironment : public IGUIEnvironment {
public:
    Texture* getDefaultTexture() override { return &texture; }
    Texture texture;
};

class FakeEditBox : public IGUIEditBox {
public:
    const wchar_t* getText() const override { return text; }
    const wchar_t* text = L"";
};

int check(bool ok, const char* what) {
    if (!ok) {
        std::printf("FAIL: %s\n", what);
        return 1;
    }
    return 0;
}

}  // namespace

int main() {
    int failures = 0;

    FakeEnvironment env;
    {
        CGUISpriteBank bank(&env);
        failures += check(bank.driverTexture == &env.texture, "driver texture kept");
        failures += check(env.texture.refCount == 2, "ctor grabs driver texture");

        Texture* a = new Texture();
        bank.addTexture(a);
        failures += check(a->refCount == 2, "addTexture grabs");
        Texture* got = bank.getTexture(0);
        failures += check(got == a && a->refCount == 3, "getTexture index 0 returns a new reference");
        drop(got);
        failures += check(bank.getTexture(5) == nullptr, "getTexture out of range is null");

        Texture* b = new Texture();
        bank.setTexture(3, b);
        failures += check(bank.textures.size() == 4, "setTexture grows with nulls");
        failures += check(bank.textures[3] == b && b->refCount == 2, "setTexture stores and grabs");
        failures += check(bank.textures[1] == nullptr, "grown slot is null");
    }
    failures += check(env.texture.refCount == 1, "dtor drops driver texture");

    FakeEditBox box;
    CGUISpinBox spin;
    spin.editBox = &box;
    box.text = L"12.5";
    failures += check(spin.getValue() == 12.5f, "getValue parses text");
    box.text = nullptr;
    failures += check(spin.getValue() == 0.0f, "getValue with no text is zero");

    spin.setDecimalPlaces(2);
    failures += check(spin.format == L"%.2f", "setDecimalPlaces(2) format");
    spin.setDecimalPlaces(-1);
    failures += check(spin.format == L"%f", "setDecimalPlaces(-1) format");

    if (failures == 0) {
        std::printf("slice_03 tests passed\n");
    }
    return failures == 0 ? 0 : 1;
}
