// Checks the glitch::gui accessors in gui_simple_accessors.hpp: type tags and
// constant returns, one-member reads and writes, and the factory type count.
#include "glitch-scene-collada-gui/gui_simple_accessors.hpp"

#include <cstdio>

using namespace glitch::gui;

static int failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } } while (0)

int main() {
    CHECK(IGUIFont().getType() == 3);
    CHECK(IGUIFontBitmap().getType() == 0);
    CHECK(IGUISkin().getType() == 3);

    CGUISkin skin;
    CHECK(skin.getSpriteBank() == nullptr);
    CHECK(skin.getType() == 0);
    skin.skinType = 2;
    CHECK(skin.getType() == 2);

    CGUIStaticText text;
    CHECK(text.getOverrideFont() == nullptr);
    text.setDrawBackground(true);
    CHECK(text.drawBackground);
    text.setDrawBorder(true);
    CHECK(text.drawBorder);
    text.enableOverrideColor(true);
    CHECK(text.isOverrideColorEnabled());
    CHECK(!text.isWordWrapEnabled());
    text.wordWrap = true;
    CHECK(text.isWordWrapEnabled());

    CGUITabControl tabs;
    CHECK(tabs.getActiveTab() == -1);
    tabs.tabHeight = 24;
    tabs.tabExtraWidth = 6;
    tabs.tabVerticalAlignment = 1;
    CHECK(tabs.getTabHeight() == 24);
    CHECK(tabs.getTabExtraWidth() == 6);
    CHECK(tabs.getTabVerticalAlignment() == 1);
    tabs.activeTab = 2;
    CHECK(tabs.getActiveTab() == 2);

    CGUITable table;
    CHECK(table.getActiveColumn() == -1);
    CHECK(table.getSelected() == -1);
    CHECK(table.getSelectedColumn() == -1);
    CHECK(table.getDrawFlags() == 0);
    table.activeColumn = 3;
    table.activeColumnOrdering = -1;
    table.selected = 5;
    table.selectedColumn = 2;
    CHECK(table.getActiveColumn() == 3);
    CHECK(table.getActiveColumnOrdering() == -1);
    CHECK(table.getSelected() == 5);
    CHECK(table.getSelectedColumn() == 2);
    table.setResizableColumns(true);
    CHECK(table.hasResizableColumns());
    table.setSelectableColumn(true);
    CHECK(table.getSelectableColumn());
    table.setDrawFlags(0x7);
    CHECK(table.getDrawFlags() == 0x7);

    IGUITTFont ttfBase;
    ttfBase.clearGlyphs();

    CGUITTFont tt;
    tt.setKerningWidth(4);
    tt.setKerningHeight(4);
    CHECK(tt.getKerningWidth(L"a", L"b") == 0);
    CHECK(tt.getKerningHeight() == 0);
    tt.tracking = 1;
    CHECK(tt.getTracking() == 1);
    tt.setAntiAlias(true);
    CHECK(tt.getAntiAlias());
    tt.setTransparency(true);
    CHECK(tt.getTransparency());
    tt.setSpaceWidth(9);
    CHECK(tt.getSpaceWidth() == 9);

    CGUIWindow window;
    CHECK(window.getCloseButton() == nullptr);
    CHECK(window.getMinimizeButton() == nullptr);
    CHECK(window.getMaximizeButton() == nullptr);

    CGUIComboBox combo;
    CHECK(combo.getSelected() == -1);
    combo.selected = 4;
    CHECK(combo.getSelected() == 4);

    CGUIContextMenu menu;
    CHECK(menu.getSelectedItem() == -1);
    menu.selectedItem = 1;
    CHECK(menu.getSelectedItem() == 1);

    CGUICheckBox check;
    CHECK(!check.isChecked());
    check.setChecked(true);
    CHECK(check.isChecked());

    CGUIEditBox edit;
    edit.setDrawBorder(true);
    CHECK(edit.drawBorder);
    edit.enableOverrideColor(true);
    CHECK(edit.overrideColorEnabled);
    edit.wordWrap = true;
    CHECK(edit.isWordWrapEnabled());
    edit.setMultiLine(true);
    CHECK(edit.isMultiLineEnabled());
    edit.passwordBox = true;
    CHECK(edit.isPasswordBox());
    edit.setAutoScroll(true);
    CHECK(edit.isAutoScrollEnabled());
    edit.maxChars = 32;
    CHECK(edit.getMax() == 32);

    CDefaultGUIElementFactory factory;
    CHECK(factory.getCreatableGUIElementTypeCount() == 0x16);

    std::printf("%s\n", failures ? "gui_simple_accessors: FAILED" : "gui_simple_accessors: ok");
    return failures != 0;
}
