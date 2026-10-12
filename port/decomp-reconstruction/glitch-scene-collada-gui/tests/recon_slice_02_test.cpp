// Tests for slice 02 reconstructions. Returns non-zero on the first failure.
#include <cstdio>
#include <string>

#include "glitch-scene-collada-gui/recon/slice_02.hpp"

using namespace glitch::collada::recon_02;

namespace {

int failures = 0;

#define CHECK(cond)                                                     \
    do {                                                                \
        if (!(cond)) {                                                  \
            std::fprintf(stderr, "%s:%d: CHECK failed: %s\n", __FILE__, \
                         __LINE__, #cond);                              \
            ++failures;                                                 \
        }                                                               \
    } while (0)

struct FakeSkin : IGUISkin {
    std::uint32_t getColor(int which) const override { return 0x100u + static_cast<std::uint32_t>(which); }
};

struct FakeBar : CGUIScrollBar {};

void testListBoxItems() {
    CGUIListBox box;
    CHECK(box.addItem(L"alpha", 3) == 0);
    CHECK(box.addItem(L"beta", 4) == 1);
    CHECK(box.insertItem(0, L"gamma", 5) == 0);
    CHECK(box.items.size() == 3);
    CHECK(box.items[0].text == L"gamma");
    CHECK(box.items[2].data == 4);

    box.setItem(1, L"delta", 9);
    CHECK(box.items[1].text == L"delta");
    CHECK(box.items[1].data == 9);

    box.swapItems(0, 2);
    CHECK(box.items[0].text == L"beta");

    box.removeItem(0);
    CHECK(box.items.size() == 2);
    CHECK(box.items[0].text == L"delta");

    box.clear();
    CHECK(box.items.empty());
    CHECK(box.selected == -1);
}

void testSelection() {
    CGUIListBox box;
    box.addItem(L"a", 0);
    box.addItem(L"b", 0);
    box.setSelected(1);
    CHECK(box.selected == 1);
    box.setSelected(7);
    CHECK(box.selected == -1);

    box.addItem(L"c", 0);
    box.setSelected(2);
    box.removeItem(0);
    CHECK(box.selected == 1);
    box.removeItem(1);
    CHECK(box.selected == -1);
}

void testOverrideColorAndSerialization() {
    CGUIListBox box;
    box.addItem(L"x", 0);
    box.setItemOverrideColor(0, 2, 0xff00ff00u);
    CHECK(box.items[0].overrideSet[2]);
    CHECK(box.items[0].overrideColor[2] == 0xff00ff00u);
    box.setItemOverrideColor(0, 4, 0x1u);
    CHECK(!box.items[0].overrideSet[0]);

    std::string use;
    std::string col;
    CHECK(box.getSerializationLabels(1, use, col));
    CHECK(use == "UseColTextHl");
    CHECK(col == "ColTextHl");
    CHECK(!box.getSerializationLabels(9, use, col));
}

void testDefaultColor() {
    CGUIListBox box;
    FakeSkin skin;
    IGUIEnvironment env;
    env.skin = &skin;
    box.environment = &env;
    CHECK(box.getItemDefaultColor(2) == 0x102u);
    CHECK(box.getItemDefaultColor(7) == 0u);
}

void testScrollBarMath() {
    FakeBar bar;
    bar.horizontal = false;
    bar.relativeRect = SRect{0, 0, 16, 160};
    bar.setMax(40);
    CHECK(bar.max == 40);
    bar.setPos(15);
    CHECK(bar.getPos() == 15);
    bar.setPos(90);
    CHECK(bar.getPos() == 40);
    bar.setPos(-3);
    CHECK(bar.getPos() == 0);
    bar.setMax(-1);
    CHECK(bar.max == 0);
}

void testTabOrder() {
    IGUIElement root;
    IGUIElement first;
    IGUIElement second;
    root.addChild(&first);
    root.addChild(&second);
    first.tabOrder = -5;
    second.tabOrder = -5;
    // No tab group above `second`: the order stays at 0.
    second.setTabOrder(0);
    CHECK(second.tabOrder == 0);
    // With a group root, the search runs and assigns the next order after the best candidate.
    root.tabGroup = true;
    second.setTabOrder(0);
    CHECK(second.tabOrder >= 0);
}

void testSpinBoxRange() {
    struct Spin : CGUISpinBox {
        float value = 0.0f;
        float getValue() const override { return value; }
        void setValue(float v) override { value = v; }
    } spin;
    spin.setRange(1.0f, 4.0f);
    CHECK(spin.rangeLow == 1.0f && spin.rangeHigh == 4.0f);
    spin.value = 9.0f;
    spin.verifyValueRange();
    CHECK(spin.value == 4.0f);
    spin.value = -2.0f;
    spin.verifyValueRange();
    CHECK(spin.value == 1.0f);
}

void testSkinFonts() {
    CGUISkin skin;
    CHECK(skin.getFont(3) == nullptr);
    CHECK(skin.getFont(12) == nullptr);
    skin.setDefaultText(2, L"hello");
    CHECK(skin.defaultText[2] == L"hello");
    skin.setDefaultText(9, L"ignored");
    CHECK(skin.defaultText[7].empty());
}

}  // namespace

int main() {
    testListBoxItems();
    testSelection();
    testOverrideColorAndSerialization();
    testDefaultColor();
    testScrollBarMath();
    testTabOrder();
    testSpinBoxRange();
    testSkinFonts();
    if (failures != 0) {
        std::fprintf(stderr, "recon_slice_02_test: %d failure(s)\n", failures);
        return 1;
    }
    std::printf("recon_slice_02_test: ok\n");
    return 0;
}
