#include "glitch-scene-collada-gui/gui_simple_accessors.hpp"

namespace glitch::gui {

// 0x53dbcc _ZNK6glitch3gui8IGUIFont7getTypeEv
std::int32_t IGUIFont::getType() const { return 3; }

// 0x53dbd4 _ZNK6glitch3gui14IGUIFontBitmap7getTypeEv
std::int32_t IGUIFontBitmap::getType() const { return 0; }

// 0x54a9f8 _ZNK6glitch3gui8IGUISkin7getTypeEv
std::int32_t IGUISkin::getType() const { return 3; }

// 0x54ab38 _ZNK6glitch3gui8CGUISkin13getSpriteBankEv
IGUISpriteBank* CGUISkin::getSpriteBank() const { return spriteBank; }

// 0x54ac2c _ZNK6glitch3gui8CGUISkin7getTypeEv
std::int32_t CGUISkin::getType() const { return skinType; }

// 0x54ff14 _ZNK6glitch3gui14CGUIStaticText15getOverrideFontEv
IGUIFont* CGUIStaticText::getOverrideFont() const { return overrideFont; }

// 0x54ff8c _ZN6glitch3gui14CGUIStaticText17setDrawBackgroundEb
void CGUIStaticText::setDrawBackground(bool draw) { drawBackground = draw; }

// 0x54ff94 _ZN6glitch3gui14CGUIStaticText13setDrawBorderEb
void CGUIStaticText::setDrawBorder(bool draw) { drawBorder = draw; }

// 0x54ffb4 _ZN6glitch3gui14CGUIStaticText19enableOverrideColorEb
void CGUIStaticText::enableOverrideColor(bool enable) { overrideColorEnabled = enable; }

// 0x54ffbc _ZNK6glitch3gui14CGUIStaticText22isOverrideColorEnabledEv
bool CGUIStaticText::isOverrideColorEnabled() const { return overrideColorEnabled; }

// 0x54ffc4 _ZNK6glitch3gui14CGUIStaticText17isWordWrapEnabledEv
// 0x551838 _ZN6glitch3gui14CGUIStaticText11setWordWrapEb
void CGUIStaticText::setWordWrap(bool enable) { wordWrap = enable; }

bool CGUIStaticText::isWordWrapEnabled() const { return wordWrap; }

// 0x55250c _ZNK6glitch3gui14CGUITabControl12getTabHeightEv
std::int32_t CGUITabControl::getTabHeight() const { return tabHeight; }

// 0x552514 _ZNK6glitch3gui14CGUITabControl16getTabExtraWidthEv
std::int32_t CGUITabControl::getTabExtraWidth() const { return tabExtraWidth; }

// 0x552800 _ZNK6glitch3gui14CGUITabControl23getTabVerticalAlignmentEv
std::int32_t CGUITabControl::getTabVerticalAlignment() const { return tabVerticalAlignment; }

// 0x552808 _ZNK6glitch3gui14CGUITabControl12getActiveTabEv
std::int32_t CGUITabControl::getActiveTab() const { return activeTab; }

// 0x555100 _ZNK6glitch3gui9CGUITable15getActiveColumnEv
std::int32_t CGUITable::getActiveColumn() const { return activeColumn; }

// 0x555108 _ZNK6glitch3gui9CGUITable23getActiveColumnOrderingEv
std::int32_t CGUITable::getActiveColumnOrdering() const { return activeColumnOrdering; }

// 0x555110 _ZN6glitch3gui9CGUITable19setResizableColumnsEb
void CGUITable::setResizableColumns(bool resizable) { resizableColumns = resizable; }

// 0x555118 _ZNK6glitch3gui9CGUITable19hasResizableColumnsEv
bool CGUITable::hasResizableColumns() const { return resizableColumns; }

// 0x555328 _ZNK6glitch3gui9CGUITable11getSelectedEv
std::int32_t CGUITable::getSelected() const { return selected; }

// 0x555330 _ZNK6glitch3gui9CGUITable17getSelectedColumnEv
std::int32_t CGUITable::getSelectedColumn() const { return selectedColumn; }

// 0x555338 _ZN6glitch3gui9CGUITable19setSelectableColumnEb
void CGUITable::setSelectableColumn(bool selectable) { selectableColumn = selectable; }

// 0x555340 _ZN6glitch3gui9CGUITable19getSelectableColumnEv
bool CGUITable::getSelectableColumn() { return selectableColumn; }

// 0x555dd8 _ZN6glitch3gui9CGUITable12setDrawFlagsEi
void CGUITable::setDrawFlags(std::int32_t flags) { drawFlags = flags; }

// 0x555de0 _ZNK6glitch3gui9CGUITable12getDrawFlagsEv
std::int32_t CGUITable::getDrawFlags() const { return drawFlags; }

// 0x55ba30 _ZN6glitch3gui10IGUITTFont11clearGlyphsEv
void IGUITTFont::clearGlyphs() {}

// 0x55bcd4 _ZN6glitch3gui10CGUITTFont15setKerningWidthEi
void CGUITTFont::setKerningWidth(std::int32_t /*width*/) {}

// 0x55bcd8 _ZN6glitch3gui10CGUITTFont16setKerningHeightEi
void CGUITTFont::setKerningHeight(std::int32_t /*height*/) {}

// 0x55bcdc _ZNK6glitch3gui10CGUITTFont15getKerningWidthEPKwS3_
std::int32_t CGUITTFont::getKerningWidth(const wchar_t* /*first*/, const wchar_t* /*second*/) const {
    return 0;
}

// 0x55bce4 _ZNK6glitch3gui10CGUITTFont16getKerningHeightEv
std::int32_t CGUITTFont::getKerningHeight() const { return 0; }

// 0x55bcec _ZN6glitch3gui10CGUITTFont11setTrackingEi
void CGUITTFont::setTracking(std::int32_t value) { tracking = value < 0 ? 0 : value; }

// 0x55bd00 _ZNK6glitch3gui10CGUITTFont11getTrackingEv
std::int32_t CGUITTFont::getTracking() const { return tracking; }

// 0x55bd08 _ZN6glitch3gui10CGUITTFont12setAntiAliasEb
void CGUITTFont::setAntiAlias(bool enable) { antiAlias = enable; }

// 0x55bd10 _ZNK6glitch3gui10CGUITTFont12getAntiAliasEv
bool CGUITTFont::getAntiAlias() const { return antiAlias; }

// 0x55bd18 _ZN6glitch3gui10CGUITTFont15setTransparencyEb
void CGUITTFont::setTransparency(bool enable) { transparency = enable; }

// 0x55bd20 _ZNK6glitch3gui10CGUITTFont15getTransparencyEv
bool CGUITTFont::getTransparency() const { return transparency; }

// 0x55bd28 _ZN6glitch3gui10CGUITTFont13setSpaceWidthEi
void CGUITTFont::setSpaceWidth(std::int32_t width) { spaceWidth = width; }

// 0x55bd30 _ZN6glitch3gui10CGUITTFont13getSpaceWidthEv
std::int32_t CGUITTFont::getSpaceWidth() const { return spaceWidth; }

// 0x55eb44 _ZNK6glitch3gui10CGUIWindow14getCloseButtonEv
IGUIButton* CGUIWindow::getCloseButton() const { return closeButton; }

// 0x55eb4c _ZNK6glitch3gui10CGUIWindow17getMinimizeButtonEv
IGUIButton* CGUIWindow::getMinimizeButton() const { return minimizeButton; }

// 0x55eb54 _ZNK6glitch3gui10CGUIWindow17getMaximizeButtonEv
IGUIButton* CGUIWindow::getMaximizeButton() const { return maximizeButton; }

// 0x6aab14 _ZNK6glitch3gui12CGUIComboBox11getSelectedEv
std::int32_t CGUIComboBox::getSelected() const { return selected; }

// 0x6ad4d4 _ZNK6glitch3gui15CGUIContextMenu15getSelectedItemEv
std::int32_t CGUIContextMenu::getSelectedItem() const { return selectedItem; }

// 0x6a76ac _ZN6glitch3gui12CGUICheckBox10setCheckedEb
void CGUICheckBox::setChecked(bool value) { checked = value; }

// 0x6a76b4 _ZNK6glitch3gui12CGUICheckBox9isCheckedEv
bool CGUICheckBox::isChecked() const { return checked; }

// 0x6afaa8 _ZN6glitch3gui11CGUIEditBox13setDrawBorderEb
void CGUIEditBox::setDrawBorder(bool draw) { drawBorder = draw; }

// 0x6afab0 _ZN6glitch3gui11CGUIEditBox19enableOverrideColorEb
void CGUIEditBox::enableOverrideColor(bool enable) { overrideColorEnabled = enable; }

// 0x6afab8 _ZNK6glitch3gui11CGUIEditBox17isWordWrapEnabledEv
// 0x6b1624 _ZN6glitch3gui11CGUIEditBox11setWordWrapEb
void CGUIEditBox::setWordWrap(bool enable) { wordWrap = enable; }

bool CGUIEditBox::isWordWrapEnabled() const { return wordWrap; }

// 0x6afac0 _ZN6glitch3gui11CGUIEditBox12setMultiLineEb
void CGUIEditBox::setMultiLine(bool value) { multiLine = value; }

// 0x6afac8 _ZNK6glitch3gui11CGUIEditBox18isMultiLineEnabledEv
bool CGUIEditBox::isMultiLineEnabled() const { return multiLine; }

// 0x6afad0 _ZNK6glitch3gui11CGUIEditBox13isPasswordBoxEv
bool CGUIEditBox::isPasswordBox() const { return passwordBox; }

// 0x6afae4 _ZN6glitch3gui11CGUIEditBox13setAutoScrollEb
void CGUIEditBox::setAutoScroll(bool enable) { autoScroll = enable; }

// 0x6afaec _ZNK6glitch3gui11CGUIEditBox19isAutoScrollEnabledEv
bool CGUIEditBox::isAutoScrollEnabled() const { return autoScroll; }

// 0x6afaf4 _ZNK6glitch3gui11CGUIEditBox6getMaxEv
std::int32_t CGUIEditBox::getMax() const { return maxChars; }

// 0x5353c4 _ZN6glitch3gui15IGUIEnvironmentD1Ev
// (the header defines the destructor as defaulted)

// 0x6a5cc8 _ZNK6glitch3gui25CDefaultGUIElementFactory31getCreatableGUIElementTypeCountEv
std::uint32_t CDefaultGUIElementFactory::getCreatableGUIElementTypeCount() const { return 0x16; }

// 0x6a56ec _ZN6glitch3gui25CDefaultGUIElementFactoryC2EPNS0_15IGUIEnvironmentE
// 0x6a571c _ZN6glitch3gui25CDefaultGUIElementFactoryC1EPNS0_15IGUIEnvironmentE
CDefaultGUIElementFactory::CDefaultGUIElementFactory(IGUIEnvironment* env) : environment(env) {}

// Name table of glitch::gui::GUIElementTypeNames (0x956b40 in the binary, 23 words, the
// last one null). Entries 9, 17 and 19 are tail-merged literals: "image" and "table" were
// located inside longer strings; "tab" is inferred from the enum order (its literal is not
// in strings.csv).
const char* const kGUIElementTypeNames[0x17] = {
    "button",   "checkBox",     "comboBox",   "contextMenu", "menu",         "editBox",
    "fileOpenDialog", "colorSelectDialog", "inOutFader", "image", "listBox", "meshViewer",
    "messageBox", "modalScreen", "scrollBar", "spinBox",     "staticText",  "tab",
    "tabControl", "table",      "toolBar",    "window",      nullptr,
};

// 0x6a5cd0 _ZNK6glitch3gui25CDefaultGUIElementFactory27getCreateableGUIElementTypeEi
std::uint32_t CDefaultGUIElementFactory::getCreateableGUIElementType(std::int32_t index) const {
    std::uint32_t type = static_cast<std::uint32_t>(index);
    if (type > 0x15) {
        type = 0x17;
    }
    return type;
}

// 0x6a5ce0 _ZNK6glitch3gui25CDefaultGUIElementFactory31getCreateableGUIElementTypeNameEi
const char* CDefaultGUIElementFactory::getCreateableGUIElementTypeName(std::int32_t index) const {
    const std::uint32_t type = static_cast<std::uint32_t>(index);
    return type < 0x16 ? kGUIElementTypeNames[type] : nullptr;
}

// 0x6a5d00 _ZNK6glitch3gui25CDefaultGUIElementFactory31getCreateableGUIElementTypeNameENS0_17EGUI_ELEMENT_TYPEE
const char* CDefaultGUIElementFactory::getCreateableGUIElementTypeName(EGUI_ELEMENT_TYPE type) const {
    return static_cast<std::uint32_t>(type) < 0x16 ? kGUIElementTypeNames[type] : nullptr;
}

// 0x54aa24 _ZNK6glitch3gui8CGUISkin8getColorENS0_18EGUI_DEFAULT_COLORE
// Out of range (>= 0x15) the original returns whatever the caller left in its
// registers; zero is returned here (partial).
video::SColor CGUISkin::getColor(std::uint32_t which) const {
    return which < 0x15 ? colors[which] : video::SColor{};
}

// 0x54aaa0 _ZNK6glitch3gui8CGUISkin7getSizeENS0_17EGUI_DEFAULT_SIZEE
std::int32_t CGUISkin::getSize(std::uint32_t which) const {
    return which < 10 ? sizes[which] : 0;
}

// 0x54aab4 _ZN6glitch3gui8CGUISkin7setSizeENS0_17EGUI_DEFAULT_SIZEEi
void CGUISkin::setSize(std::uint32_t which, std::int32_t size) {
    if (which < 10) {
        sizes[which] = size;
    }
}

// 0x54ab74 _ZNK6glitch3gui8CGUISkin7getIconENS0_17EGUI_DEFAULT_ICONE
std::uint32_t CGUISkin::getIcon(std::uint32_t which) const {
    return which < 0x17 ? icons[which] : 0;
}

// 0x54ab88 _ZN6glitch3gui8CGUISkin7setIconENS0_17EGUI_DEFAULT_ICONEj
void CGUISkin::setIcon(std::uint32_t which, std::uint32_t icon) {
    if (which < 0x17) {
        icons[which] = icon;
    }
}

// 0x54ab98 _ZNK6glitch3gui8CGUISkin14getDefaultTextENS0_17EGUI_DEFAULT_TEXTE
const wchar_t* CGUISkin::getDefaultText(std::uint32_t which) const {
    return which < 8 ? defaultTexts[which].text : defaultTexts[0].text;
}

// 0x54ff1c _ZN6glitch3gui14CGUIStaticText16setOverrideColorENS_5video6SColorE
void CGUIStaticText::setOverrideColor(video::SColor color) {
    overrideColorEnabled = true;
    overrideColor = color;
}

// 0x54ff54 _ZN6glitch3gui14CGUIStaticText18setBackgroundColorENS_5video6SColorE
void CGUIStaticText::setBackgroundColor(video::SColor color) {
    drawBackground = true;
    backgroundColor = color;
}

// 0x54ff9c _ZN6glitch3gui14CGUIStaticText16setTextAlignmentENS0_14EGUI_ALIGNMENTES2_
void CGUIStaticText::setTextAlignment(std::uint32_t horizontal, std::uint32_t vertical) {
    horizontalAlign = horizontal;
    verticalAlign = vertical;
}

// 0x54ffa8 _ZNK6glitch3gui14CGUIStaticText16getOverrideColorEv
const video::SColor& CGUIStaticText::getOverrideColor() const {
    return overrideColor;
}

// 0x552214 _ZNK6glitch3gui14CGUITabControl11getTabCountEv
std::int32_t CGUITabControl::getTabCount() const {
    return static_cast<std::int32_t>(tabs.size());
}

// 0x552228 _ZNK6glitch3gui14CGUITabControl6getTabEi
CGUITab* CGUITabControl::getTab(std::uint32_t index) const {
    return index < tabs.size() ? tabs[index] : nullptr;
}

// 0x554f60 _ZNK6glitch3gui9CGUITable14getColumnCountEv
std::int32_t CGUITable::getColumnCount() const {
    return static_cast<std::int32_t>(columns.size());
}

// 0x554f80 _ZNK6glitch3gui9CGUITable11getRowCountEv
std::int32_t CGUITable::getRowCount() const {
    return static_cast<std::int32_t>(rows.size());
}

// 0x555120 _ZN6glitch3gui9CGUITable12setCellColorEjjNS_5video6SColorE
void CGUITable::setCellColor(std::uint32_t row, std::uint32_t column, video::SColor color) {
    if (row < rows.size() && column < columns.size()) {
        rows[row].cells[column].color = color;
    }
}

// 0x5551bc _ZN6glitch3gui9CGUITable11setCellDataEjjPv
void CGUITable::setCellData(std::uint32_t row, std::uint32_t column, void* data) {
    if (row < rows.size() && column < columns.size()) {
        rows[row].cells[column].data = data;
    }
}

// 0x555230 _ZNK6glitch3gui9CGUITable11getCellTextEjj
const wchar_t* CGUITable::getCellText(std::uint32_t row, std::uint32_t column) const {
    if (row < rows.size() && column < columns.size()) {
        return rows[row].cells[column].text;
    }
    return nullptr;
}

// 0x5552ac _ZNK6glitch3gui9CGUITable11getCellDataEjj
void* CGUITable::getCellData(std::uint32_t row, std::uint32_t column) const {
    if (row < rows.size() && column < columns.size()) {
        return rows[row].cells[column].data;
    }
    return nullptr;
}

// 0x555914 _ZN6glitch3gui9CGUITable17setColumnOrderingEjNS0_20EGUI_COLUMN_ORDERINGE
void CGUITable::setColumnOrdering(std::uint32_t column, std::int32_t ordering) {
    if (column < columns.size()) {
        columns[column].ordering = ordering;
    }
}

// 0x6aaa88 _ZNK6glitch3gui12CGUIComboBox12getItemCountEv
std::int32_t CGUIComboBox::getItemCount() const {
    return static_cast<std::int32_t>(items.size());
}

// 0x6aaab8 _ZNK6glitch3gui12CGUIComboBox7getItemEj
const wchar_t* CGUIComboBox::getItem(std::uint32_t index) const {
    return index < items.size() ? items[index].text : nullptr;
}

// 0x6acdac _ZNK6glitch3gui15CGUIContextMenu12getItemCountEv
std::int32_t CGUIContextMenu::getItemCount() const {
    return static_cast<std::int32_t>(items.size());
}

// 0x6acf08 _ZNK6glitch3gui15CGUIContextMenu11getItemTextEj
const wchar_t* CGUIContextMenu::getItemText(std::uint32_t index) const {
    return index < items.size() ? items[index].text : nullptr;
}

// 0x6acf44 _ZNK6glitch3gui15CGUIContextMenu13isItemEnabledEj
bool CGUIContextMenu::isItemEnabled(std::uint32_t index) const {
    return index < items.size() && items[index].enabled;
}

// 0x6acf80 _ZNK6glitch3gui15CGUIContextMenu13isItemCheckedEj
bool CGUIContextMenu::isItemChecked(std::uint32_t index) const {
    return index < items.size() && items[index].checked;
}

// 0x6acfbc _ZN6glitch3gui15CGUIContextMenu14setItemEnabledEjb
void CGUIContextMenu::setItemEnabled(std::uint32_t index, bool enabled) {
    if (index < items.size()) {
        items[index].enabled = enabled;
    }
}

// 0x6acff4 _ZN6glitch3gui15CGUIContextMenu14setItemCheckedEjb
void CGUIContextMenu::setItemChecked(std::uint32_t index, bool checked) {
    if (index < items.size()) {
        items[index].checked = checked;
    }
}

// 0x6ad1d0 _ZNK6glitch3gui15CGUIContextMenu8getHRectERKNS1_5SItemERKNS_4core4rectIiEE
// Keeps the rectangle's left and right edges; the top edge moves by the item
// offset and the bottom edge is the top plus the item height.
core::rect<std::int32_t> CGUIContextMenu::getHRect(const SItem& item,
                                                   const core::rect<std::int32_t>& rect) const {
    core::rect<std::int32_t> out = rect;
    out.UpperLeftCorner.Y = rect.UpperLeftCorner.Y + item.offset;
    out.LowerRightCorner.Y = out.UpperLeftCorner.Y + item.height;
    return out;
}

// 0x6ad214 _ZNK6glitch3gui15CGUIContextMenu7getRectERKNS1_5SItemERKNS_4core4rectIiEE
// Like getHRect, with the left edge indented by 20 units and the right edge kept.
core::rect<std::int32_t> CGUIContextMenu::getRect(const SItem& item,
                                                  const core::rect<std::int32_t>& rect) const {
    core::rect<std::int32_t> out;
    out.UpperLeftCorner.X = rect.UpperLeftCorner.X + 0x14;
    out.UpperLeftCorner.Y = rect.UpperLeftCorner.Y + item.offset;
    out.LowerRightCorner.X = rect.LowerRightCorner.X;
    out.LowerRightCorner.Y = out.UpperLeftCorner.Y + item.height;
    return out;
}

// 0x6ad4dc _ZNK6glitch3gui15CGUIContextMenu10getSubMenuEj
CGUIContextMenu* CGUIContextMenu::getSubMenu(std::uint32_t index) const {
    return index < items.size() ? items[index].subMenu : nullptr;
}

// 0x6ad518 _ZNK6glitch3gui15CGUIContextMenu16getItemCommandIdEj
std::int32_t CGUIContextMenu::getItemCommandId(std::uint32_t index) const {
    return index < items.size() ? items[index].commandId : -1;
}

// 0x6ad554 _ZN6glitch3gui15CGUIContextMenu16setItemCommandIdEji
void CGUIContextMenu::setItemCommandId(std::uint32_t index, std::int32_t commandId) {
    if (index < items.size()) {
        items[index].commandId = commandId;
    }
}

// 0x545818 _ZNK6glitch3gui8CGUIMenu8getHRectERKNS0_15CGUIContextMenu5SItemERKNS_4core4rectIiEE
// Keeps the top and bottom edges; the left edge moves by the item offset and the
// right edge is the left plus the item width.
core::rect<std::int32_t> CGUIMenu::getHRect(const CGUIContextMenu::SItem& item,
                                            const core::rect<std::int32_t>& rect) const {
    core::rect<std::int32_t> out = rect;
    out.UpperLeftCorner.X = rect.UpperLeftCorner.X + item.offset;
    out.LowerRightCorner.X = out.UpperLeftCorner.X + item.width;
    return out;
}

// 0x6afa70 _ZN6glitch3gui11CGUIEditBox16setOverrideColorENS_5video6SColorE
void CGUIEditBox::setOverrideColor(video::SColor color) {
    overrideColorEnabled = true;
    overrideColor = color;
}

// 0x6afad8 _ZN6glitch3gui11CGUIEditBox16setTextAlignmentENS0_14EGUI_ALIGNMENTES2_
void CGUIEditBox::setTextAlignment(std::uint32_t horizontal, std::uint32_t vertical) {
    horizontalAlign = horizontal;
    verticalAlign = vertical;
}

// 0x54f8c0 _ZNK6glitch3gui14CGUISpriteBank15getTextureCountEv
std::int32_t CGUISpriteBank::getTextureCount() const {
    return static_cast<std::int32_t>(textures.size());
}

// 0x54f8b0 _ZN6glitch3gui14CGUISpriteBank12getPositionsEv
std::vector<void*>* CGUISpriteBank::getPositions() {
    return &positions;
}

// 0x54f8b8 _ZN6glitch3gui14CGUISpriteBank10getSpritesEv
std::vector<void*>* CGUISpriteBank::getSprites() {
    return &sprites;
}

}  // namespace glitch::gui
