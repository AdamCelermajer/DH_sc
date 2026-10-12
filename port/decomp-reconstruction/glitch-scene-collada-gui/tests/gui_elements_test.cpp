// Accessor checks for the glitch::gui element and widget fields.
#include "glitch-scene-collada-gui/gui_elements.hpp"
#include "glitch-scene-collada-gui/gui_simple_accessors.hpp"

#include <cstdio>

using namespace glitch::gui;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

// CGUIListBox has a pure virtual addItem (the list box proper implements it);
// this stand-in supplies it so the accessors can be tested.
struct ListBoxForTest : CGUIListBox {
    std::uint32_t addItem(const wchar_t*, int) override { return 0; }
};

int main() {
    IGUIElement element;
    CHECK(!element.isVisible() && !element.isEnabled() && !element.isSubElement());
    element.setVisible(true);
    element.setEnabled(true);
    element.setSubElement(true);
    CHECK(element.isVisible() && element.isEnabled() && element.isSubElement());
    element.setVisible(false);
    CHECK(!element.isVisible() && element.isEnabled());
    element.setID(77);
    CHECK(element.getID() == 77);
    CHECK(element.getText() == nullptr);
    CHECK(element.getName() == nullptr);
    CHECK(element.getChildren().empty());
    CHECK(element.getToolTipText().empty());

    CGUIEnvironment env;
    IGUIElement other;
    CHECK(env.getFocus() == nullptr);
    CHECK(!env.hasFocus(&other));
    env.focus = &other;
    CHECK(env.getFocus() == &other);
    CHECK(env.hasFocus(&other));
    CHECK(env.getRegisteredGUIElementFactoryCount() == 0);
    env.guiElementFactories = {nullptr, nullptr};
    CHECK(env.getRegisteredGUIElementFactoryCount() == 2);
    env.setUserEventReceiver(nullptr);
    CHECK(env.userEventReceiver == nullptr);

    CGUIFont font;
    font.setKerningWidth(3);
    font.setKerningHeight(-4);
    CHECK(font.getKerningHeight() == -4);
    font.setTracking(9);
    CHECK(font.getTracking() == 0);
    CHECK(font.getType() == 0);
    auto dim = font.getDimension("abc");
    CHECK(dim.Width == 0 && dim.Height == 0);
    CHECK(font.getSpriteBank() == nullptr);

    CGUIImage image;
    image.setUseAlphaChannel(true);
    image.setScaleImage(true);
    CHECK(image.isAlphaChannelUsed() && image.isImageScaled());
    image.setScaleImage(false);
    CHECK(image.isAlphaChannelUsed() && !image.isImageScaled());

    CGUIScrollBar bar;
    CHECK(bar.getSmallStep() == 10 && bar.getLargeStep() == 50);
    bar.setSmallStep(4);
    CHECK(bar.getSmallStep() == 4);
    bar.setSmallStep(0);
    CHECK(bar.getSmallStep() == 10);
    bar.setLargeStep(-1);
    CHECK(bar.getLargeStep() == 50);
    bar.setLargeStep(120);
    CHECK(bar.getLargeStep() == 120);
    CHECK(bar.getMax() == 0 && bar.getPos() == 0);

    CGUISpinBox spin;
    spin.setStepSize(0.5f);
    CHECK(spin.getStepSize() == 0.5f);
    CHECK(spin.getEditBox() == nullptr);
    CHECK(spin.getMin() == 0.0f && spin.getMax() == 0.0f);

    CGUITab tab;
    tab.setNumber(5);
    tab.setDrawBackground(true);
    CHECK(tab.getNumber() == 5 && tab.isDrawingBackground());

    CGUIButton button;
    CHECK(!button.isPressed() && !button.isPushButton());
    button.setIsPushButton(true);
    button.setDrawBorder(true);
    button.setUseAlphaChannel(true);
    CHECK(button.isPushButton() && button.isDrawingBorder() && button.isAlphaChannelUsed());
    CHECK(!button.isPressed());

    element.guiType = 3;
    CHECK(element.getTypeName() == nullptr);

    SBuiltInFontNode fontNode;
    fontNode.font = &font;
    SBuiltInFontNode fontSentinel;
    CGUIEnvironment fonts;
    CHECK(fonts.getBuiltInFont() == nullptr);
    fonts.builtInFontBegin = &fontNode;
    fonts.builtInFontEnd = &fontSentinel;
    CHECK(fonts.getBuiltInFont() == &font);
    fonts.builtInFontBegin = &fontSentinel;
    CHECK(fonts.getBuiltInFont() == nullptr);

    // List box: item text, icon, and the four override-colour records.
    ListBoxForTest list;
    CHECK(list.getItemCount() == 0);
    CHECK(list.getIcon(0) == -1);
    static const wchar_t kItem[] = L"item";
    SListItem item;
    item.text = kItem;
    item.icon = 3;
    item.overrides[2].enabled = true;
    item.overrides[2].color = glitch::video::SColor{0x90807060u};
    list.items.push_back(item);
    CHECK(list.getItemCount() == 1);
    CHECK(list.getListItem(0) == kItem);
    CHECK(list.getListItem(1) == nullptr);
    CHECK(list.getIcon(0) == 3);
    CHECK(list.getIcon(1) == -1);
    CHECK(list.hasItemOverrideColor(0, 2));
    CHECK(!list.hasItemOverrideColor(0, 1));
    CHECK(!list.hasItemOverrideColor(0, 4));
    CHECK(list.getItemOverrideColor(0, 2).color == 0x90807060u);
    CHECK(list.getItemOverrideColor(0, 4).color == 0);
    list.clearItemOverrideColor(0, 2);
    CHECK(!list.hasItemOverrideColor(0, 2));
    list.items[0].overrides[0].enabled = true;
    list.clearItemOverrideColor(0);
    CHECK(!list.hasItemOverrideColor(0, 0));
    list.clearItemOverrideColor(5);

    // Image and fader colour, tab colours.
    CGUIImage imageEl;
    imageEl.setColor(glitch::video::SColor{0xCAFEBABEu});
    CHECK(imageEl.color.color == 0xCAFEBABEu);
    CGUIInOutFader fader;
    fader.color = glitch::video::SColor{0x13579BDFu};
    CHECK(fader.getColor().color == 0x13579BDFu);
    CGUITab tabEl;
    tabEl.setBackgroundColor(glitch::video::SColor{0x0A0B0C0Du});
    tabEl.setTextColor(glitch::video::SColor{0x1A1B1C1Du});
    CHECK(tabEl.backgroundColor.color == 0x0A0B0C0Du);
    CHECK(tabEl.textColor.color == 0x1A1B1C1Du);

    // Tab control: count and bounds-checked lookup.
    CGUITabControl tabStrip;
    CHECK(tabStrip.getTabCount() == 0);
    CHECK(tabStrip.getTab(0) == nullptr);
    CGUITab firstTab;
    tabStrip.tabs.push_back(&firstTab);
    CHECK(tabStrip.getTabCount() == 1);
    CHECK(tabStrip.getTab(0) == &firstTab);
    CHECK(tabStrip.getTab(1) == nullptr);

    // Tab colours, and the list box single-colour setter (first override record).
    CGUITab colourTab;
    colourTab.setTextColor(glitch::video::SColor{0x11223344u});
    colourTab.setBackgroundColor(glitch::video::SColor{0x55667788u});
    CHECK(colourTab.getTextColor().color == 0x11223344u);
    CHECK(colourTab.getBackgroundColor().color == 0x55667788u);
    ListBoxForTest colourList;
    SListItem colourItem;
    colourList.items.push_back(colourItem);
    colourList.setItemOverrideColor(0, glitch::video::SColor{0xAABBCCDDu});
    CHECK(colourList.hasItemOverrideColor(0, 0));
    CHECK(colourList.getItemOverrideColor(0, 0).color == 0xAABBCCDDu);
    colourList.setItemOverrideColor(5, glitch::video::SColor{0x1u});
    CHECK(colourList.getItemCount() == 1);

    // Factory list lookup by index.
    CGUIEnvironment factoryEnv;
    int factoryTag = 0;
    factoryEnv.guiElementFactories.push_back(&factoryTag);
    CHECK(factoryEnv.getGUIElementFactory(0) == &factoryTag);
    CHECK(factoryEnv.getGUIElementFactory(1) == nullptr);

    std::printf("%s\n", failures ? "gui_elements: FAILED" : "gui_elements: ok");
    return failures != 0;
}
