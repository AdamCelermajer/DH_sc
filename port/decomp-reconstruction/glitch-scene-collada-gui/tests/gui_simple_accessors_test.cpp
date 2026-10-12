// Checks the glitch::gui accessors in gui_simple_accessors.hpp: type tags and
// constant returns, one-member reads and writes, and the factory type count.
#include "glitch-scene-collada-gui/gui_simple_accessors.hpp"

#include <cstdio>
#include <cstring>

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

    // Skin: colour, size, icon and default-text tables.
    CGUISkin skinTables;
    skinTables.colors[3] = glitch::video::SColor{0x11223344};
    CHECK(skinTables.getColor(3).color == 0x11223344u);
    CHECK(skinTables.getColor(0x15).color == 0);
    skinTables.setSize(9, 12);
    CHECK(skinTables.getSize(9) == 12);
    CHECK(skinTables.getSize(10) == 0);
    skinTables.setSize(10, 5);
    CHECK(skinTables.getSize(10) == 0);
    skinTables.setIcon(22, 7);
    CHECK(skinTables.getIcon(22) == 7);
    CHECK(skinTables.getIcon(23) == 0);
    static const wchar_t kDefaultText[] = L"text";
    skinTables.defaultTexts[0].text = kDefaultText;
    CHECK(skinTables.getDefaultText(7) == nullptr);
    CHECK(skinTables.getDefaultText(9) == kDefaultText);
    CHECK(skinTables.getDefaultText(0) == kDefaultText);

    // Static text: setters also set their enable flags.
    CGUIStaticText staticText;
    staticText.setOverrideColor(glitch::video::SColor{0xAABBCCDDu});
    CHECK(staticText.overrideColorEnabled);
    CHECK(staticText.getOverrideColor().color == 0xAABBCCDDu);
    staticText.setBackgroundColor(glitch::video::SColor{0x01020304u});
    CHECK(staticText.drawBackground);
    CHECK(staticText.backgroundColor.color == 0x01020304u);
    staticText.setTextAlignment(2, 3);
    CHECK(staticText.horizontalAlign == 2 && staticText.verticalAlign == 3);

    // Table: rows, columns, cell colour, data, text and column ordering.
    CGUITable grid;
    grid.columns.resize(2);
    grid.rows.resize(3);
    for (auto& row : grid.rows) {
        row.cells.resize(2);
    }
    CHECK(grid.getColumnCount() == 2);
    CHECK(grid.getRowCount() == 3);
    grid.setCellColor(1, 1, glitch::video::SColor{0x55667788u});
    CHECK(grid.rows[1].cells[1].color.color == 0x55667788u);
    grid.setCellColor(3, 0, glitch::video::SColor{0x1u});
    grid.setCellColor(0, 2, glitch::video::SColor{0x1u});
    CHECK(grid.rows[0].cells[0].color.color == 0);
    int payload = 42;
    grid.setCellData(2, 0, &payload);
    CHECK(grid.getCellData(2, 0) == &payload);
    CHECK(grid.getCellData(5, 0) == nullptr);
    static const wchar_t kCell[] = L"cell";
    grid.rows[0].cells[1].text = kCell;
    CHECK(grid.getCellText(0, 1) == kCell);
    CHECK(grid.getCellText(0, 9) == nullptr);
    grid.setColumnOrdering(1, -1);
    CHECK(grid.columns[1].ordering == -1);
    grid.setColumnOrdering(2, 1);
    CHECK(grid.getColumnCount() == 2);

    // Combo box.
    CGUIComboBox comboNew;
    static const wchar_t kEntry[] = L"entry";
    comboNew.items.push_back(CGUIComboBox::SItem{kEntry});
    CHECK(comboNew.getItemCount() == 1);
    CHECK(comboNew.getItem(0) == kEntry);
    CHECK(comboNew.getItem(1) == nullptr);

    // Context menu: flags, command ids, and the highlight and item rectangles.
    CGUIContextMenu ctxMenu;
    CHECK(ctxMenu.getItemCount() == 0);
    CHECK(ctxMenu.getItemCommandId(0) == -1);
    CGUIContextMenu::SItem entry;
    entry.text = kEntry;
    entry.enabled = true;
    entry.offset = 4;
    entry.height = 20;
    entry.width = 50;
    entry.commandId = 9;
    ctxMenu.items.push_back(entry);
    CHECK(ctxMenu.getItemCount() == 1);
    CHECK(ctxMenu.getItemText(0) == kEntry);
    CHECK(ctxMenu.isItemEnabled(0));
    CHECK(!ctxMenu.isItemChecked(0));
    ctxMenu.setItemChecked(0, true);
    CHECK(ctxMenu.isItemChecked(0));
    ctxMenu.setItemEnabled(0, false);
    CHECK(!ctxMenu.isItemEnabled(0));
    ctxMenu.setItemEnabled(4, true);
    CHECK(!ctxMenu.isItemEnabled(4));
    CHECK(ctxMenu.getItemCommandId(0) == 9);
    ctxMenu.setItemCommandId(0, 11);
    CHECK(ctxMenu.getItemCommandId(0) == 11);
    CHECK(ctxMenu.getSubMenu(0) == nullptr);
    CHECK(ctxMenu.getItemText(1) == nullptr);
    glitch::core::rect<std::int32_t> area{{10, 100}, {200, 300}};
    glitch::core::rect<std::int32_t> hl = ctxMenu.getHRect(entry, area);
    CHECK(hl.UpperLeftCorner.X == 10 && hl.LowerRightCorner.X == 200);
    CHECK(hl.UpperLeftCorner.Y == 104 && hl.LowerRightCorner.Y == 124);
    glitch::core::rect<std::int32_t> rr = ctxMenu.getRect(entry, area);
    CHECK(rr.UpperLeftCorner.X == 30 && rr.LowerRightCorner.X == 200);
    CHECK(rr.UpperLeftCorner.Y == 104 && rr.LowerRightCorner.Y == 124);

    // Menu bar: the highlight moves horizontally.
    CGUIMenu barMenu;
    glitch::core::rect<std::int32_t> barRect = barMenu.getHRect(entry, area);
    CHECK(barRect.UpperLeftCorner.X == 14 && barRect.LowerRightCorner.X == 64);
    CHECK(barRect.UpperLeftCorner.Y == 100 && barRect.LowerRightCorner.Y == 300);

    // Edit box: override colour and alignment.
    CGUIEditBox editBoxNew;
    editBoxNew.setOverrideColor(glitch::video::SColor{0x0F0E0D0Cu});
    CHECK(editBoxNew.overrideColorEnabled && editBoxNew.overrideColor.color == 0x0F0E0D0Cu);
    editBoxNew.setTextAlignment(1, 2);
    CHECK(editBoxNew.horizontalAlign == 1 && editBoxNew.verticalAlign == 2);

    int envTag = 0;
    CDefaultGUIElementFactory factory(reinterpret_cast<IGUIEnvironment*>(&envTag));
    CHECK(factory.referenceCount == 1 && factory.environment == reinterpret_cast<IGUIEnvironment*>(&envTag));

    CGUITTFont ttf;
    ttf.setTracking(-3);
    CHECK(ttf.getTracking() == 0);
    ttf.setTracking(4);
    CHECK(ttf.getTracking() == 4);

    CGUISpriteBank bank;
    CHECK(bank.getTextureCount() == 0);
    bank.textures.resize(2);
    CHECK(bank.getTextureCount() == 2);

    CHECK(factory.getCreatableGUIElementTypeCount() == 0x16);
    CHECK(factory.getCreateableGUIElementType(3) == 3);
    CHECK(factory.getCreateableGUIElementType(0x16) == 0x17);
    CHECK(factory.getCreateableGUIElementType(-1) == 0x17);
    CHECK(std::strcmp(factory.getCreateableGUIElementTypeName(0), "button") == 0);
    CHECK(std::strcmp(factory.getCreateableGUIElementTypeName(EGUIET_IMAGE), "image") == 0);
    CHECK(std::strcmp(factory.getCreateableGUIElementTypeName(EGUIET_TAB), "tab") == 0);
    CHECK(std::strcmp(factory.getCreateableGUIElementTypeName(EGUIET_WINDOW), "window") == 0);
    CHECK(factory.getCreateableGUIElementTypeName(0x16) == nullptr);
    CHECK(factory.getCreateableGUIElementTypeName(-1) == nullptr);

    // Interface destructors are defaulted; each class must construct and destroy cleanly.
    {
        IGUIFileOpenDialog a;
        IGUIImage b;
        IGUIInOutFader c;
        IGUIListBox d;
        IGUIContextMenu e;
        IGUIMeshViewer f;
        IGUIWindow g;
        IGUIScrollBar h;
        IGUISpinBox i;
        IGUIStaticText j;
        IGUITab k;
        IGUITabControl l;
        IGUITable m;
        IGUIToolBar n;
        IGUIButton o;
        IGUICheckBox p;
        IGUIColorSelectDialog q;
        IGUIComboBox r;
        IGUIEditBox s;
    }

    std::printf("%s\n", failures ? "gui_simple_accessors: FAILED" : "gui_simple_accessors: ok");
    return failures != 0;
}
