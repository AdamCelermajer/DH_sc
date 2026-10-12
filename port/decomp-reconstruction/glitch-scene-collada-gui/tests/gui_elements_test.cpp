// Accessor checks for the glitch::gui element and widget fields.
#include "glitch-scene-collada-gui/gui_elements.hpp"

#include <cstdio>

using namespace glitch::gui;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

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

    std::printf("%s\n", failures ? "gui_elements: FAILED" : "gui_elements: ok");
    return failures != 0;
}
