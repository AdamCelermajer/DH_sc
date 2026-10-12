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

}  // namespace glitch::gui
