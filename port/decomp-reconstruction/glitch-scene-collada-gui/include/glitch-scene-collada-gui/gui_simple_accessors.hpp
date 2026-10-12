// glitch::gui accessors and interface destructors whose bodies are constant
// returns, one-member reads or one-member writes. Each class maps only the
// members its bodies touch; offsets are kept as comments and the code uses typed
// members only. Virtual dispatch is not modelled (no vtables in the stand-ins).
#pragma once

#include <cstdint>

namespace glitch::gui {

class IGUIFont;
class IGUISpriteBank;
class IGUIButton;

// ---- Font, skin and sprite-bank interfaces ----

class IGUIFont {
public:
    // 0x53dbcc _ZNK6glitch3gui8IGUIFont7getTypeEv (returns 3)
    std::int32_t getType() const;
    // 0x53dc20 _ZN6glitch3gui8IGUIFontD1Ev (empty)
    ~IGUIFont() = default;
};

class IGUIFontBitmap {
public:
    // 0x53dbd4 _ZNK6glitch3gui14IGUIFontBitmap7getTypeEv (returns 0)
    std::int32_t getType() const;
    // 0x53dc24 _ZN6glitch3gui14IGUIFontBitmapD1Ev (empty)
    ~IGUIFontBitmap() = default;
};

class IGUISkin {
public:
    // 0x54a9f8 _ZNK6glitch3gui8IGUISkin7getTypeEv (returns 3)
    std::int32_t getType() const;
    // 0x54aa00 _ZN6glitch3gui8IGUISkinD1Ev (empty)
    ~IGUISkin() = default;
};

class CGUISkin {
public:
    // 0x54ab38 _ZNK6glitch3gui8CGUISkin13getSpriteBankEv
    IGUISpriteBank* getSpriteBank() const;
    // 0x54ac2c _ZNK6glitch3gui8CGUISkin7getTypeEv
    std::int32_t getType() const;

    IGUISpriteBank* spriteBank = nullptr;  // +0xf0
    std::int32_t skinType = 0;             // +0x33c
};

class IGUISpriteBank {
public:
    // 0x54f78c _ZN6glitch3gui14IGUISpriteBankD1Ev (empty)
    ~IGUISpriteBank() = default;
};

// ---- Static text ----

class CGUIStaticText {
public:
    // 0x54ff14 _ZNK6glitch3gui14CGUIStaticText15getOverrideFontEv
    IGUIFont* getOverrideFont() const;
    // 0x54ff8c _ZN6glitch3gui14CGUIStaticText17setDrawBackgroundEb
    void setDrawBackground(bool draw);
    // 0x54ff94 _ZN6glitch3gui14CGUIStaticText13setDrawBorderEb
    void setDrawBorder(bool draw);
    // 0x54ffb4 _ZN6glitch3gui14CGUIStaticText19enableOverrideColorEb
    void enableOverrideColor(bool enable);
    // 0x54ffbc _ZNK6glitch3gui14CGUIStaticText22isOverrideColorEnabledEv
    bool isOverrideColorEnabled() const;
    // 0x54ffc4 _ZNK6glitch3gui14CGUIStaticText17isWordWrapEnabledEv
    bool isWordWrapEnabled() const;

    bool overrideColorEnabled = false;  // +0x170
    bool drawBorder = false;            // +0x164
    bool drawBackground = false;        // +0x172
    bool wordWrap = false;              // +0x171
    IGUIFont* overrideFont = nullptr;   // +0x17c
};

// ---- Tab control ----

class CGUITabControl {
public:
    // 0x55250c _ZNK6glitch3gui14CGUITabControl12getTabHeightEv
    std::int32_t getTabHeight() const;
    // 0x552514 _ZNK6glitch3gui14CGUITabControl16getTabExtraWidthEv
    std::int32_t getTabExtraWidth() const;
    // 0x552800 _ZNK6glitch3gui14CGUITabControl23getTabVerticalAlignmentEv
    std::int32_t getTabVerticalAlignment() const;
    // 0x552808 _ZNK6glitch3gui14CGUITabControl12getActiveTabEv
    std::int32_t getActiveTab() const;

    std::int32_t activeTab = -1;              // +0x164
    std::int32_t tabHeight = 0;               // +0x16c
    std::int32_t tabVerticalAlignment = 0;    // +0x170
    std::int32_t tabExtraWidth = 0;           // +0x184
};

// ---- Table ----

class CGUITable {
public:
    // 0x555100 _ZNK6glitch3gui9CGUITable15getActiveColumnEv
    std::int32_t getActiveColumn() const;
    // 0x555108 _ZNK6glitch3gui9CGUITable23getActiveColumnOrderingEv
    std::int32_t getActiveColumnOrdering() const;
    // 0x555110 _ZN6glitch3gui9CGUITable19setResizableColumnsEb
    void setResizableColumns(bool resizable);
    // 0x555118 _ZNK6glitch3gui9CGUITable19hasResizableColumnsEv
    bool hasResizableColumns() const;
    // 0x555328 _ZNK6glitch3gui9CGUITable11getSelectedEv
    std::int32_t getSelected() const;
    // 0x555330 _ZNK6glitch3gui9CGUITable17getSelectedColumnEv
    std::int32_t getSelectedColumn() const;
    // 0x555338 _ZN6glitch3gui9CGUITable19setSelectableColumnEb
    void setSelectableColumn(bool selectable);
    // 0x555340 _ZN6glitch3gui9CGUITable19getSelectableColumnEv
    bool getSelectableColumn();
    // 0x555dd8 _ZN6glitch3gui9CGUITable12setDrawFlagsEi
    void setDrawFlags(std::int32_t flags);
    // 0x555de0 _ZNK6glitch3gui9CGUITable12getDrawFlagsEv
    std::int32_t getDrawFlags() const;

    bool resizableColumns = false;         // +0x188
    bool selectableColumn = false;         // +0x189
    std::int32_t activeColumn = -1;        // +0x1a8
    std::int32_t activeColumnOrdering = 0; // +0x1ac
    std::int32_t selected = -1;            // +0x198
    std::int32_t selectedColumn = -1;      // +0x19c
    std::int32_t drawFlags = 0;            // +0x1b0
};

// ---- Font (TrueType) ----

class IGUITTFont {
public:
    // 0x55ba30 _ZN6glitch3gui10IGUITTFont11clearGlyphsEv (empty)
    void clearGlyphs();
    // 0x55bb2c _ZN6glitch3gui10IGUITTFontD1Ev (empty)
    ~IGUITTFont() = default;
};

class CGUITTFont {
public:
    // 0x55bcd4 _ZN6glitch3gui10CGUITTFont15setKerningWidthEi (empty)
    void setKerningWidth(std::int32_t width);
    // 0x55bcd8 _ZN6glitch3gui10CGUITTFont16setKerningHeightEi (empty)
    void setKerningHeight(std::int32_t height);
    // 0x55bcdc _ZNK6glitch3gui10CGUITTFont15getKerningWidthEPKwS3_ (returns 0)
    std::int32_t getKerningWidth(const wchar_t* first, const wchar_t* second) const;
    // 0x55bce4 _ZNK6glitch3gui10CGUITTFont16getKerningHeightEv (returns 0)
    std::int32_t getKerningHeight() const;
    // 0x55bd00 _ZNK6glitch3gui10CGUITTFont11getTrackingEv
    std::int32_t getTracking() const;
    // 0x55bd08 _ZN6glitch3gui10CGUITTFont12setAntiAliasEb
    void setAntiAlias(bool enable);
    // 0x55bd10 _ZNK6glitch3gui10CGUITTFont12getAntiAliasEv
    bool getAntiAlias() const;
    // 0x55bd18 _ZN6glitch3gui10CGUITTFont15setTransparencyEb
    void setTransparency(bool enable);
    // 0x55bd20 _ZNK6glitch3gui10CGUITTFont15getTransparencyEv
    bool getTransparency() const;
    // 0x55bd28 _ZN6glitch3gui10CGUITTFont13setSpaceWidthEi
    void setSpaceWidth(std::int32_t width);
    // 0x55bd30 _ZN6glitch3gui10CGUITTFont13getSpaceWidthEv
    std::int32_t getSpaceWidth() const;

    bool antiAlias = false;        // +0x34
    bool transparency = false;     // +0x35
    std::int32_t tracking = 0;     // +0x38
    std::int32_t spaceWidth = 0;   // +0x3c
};

// ---- Window ----

class CGUIWindow {
public:
    // 0x55eb44 _ZNK6glitch3gui10CGUIWindow14getCloseButtonEv
    IGUIButton* getCloseButton() const;
    // 0x55eb4c _ZNK6glitch3gui10CGUIWindow17getMinimizeButtonEv
    IGUIButton* getMinimizeButton() const;
    // 0x55eb54 _ZNK6glitch3gui10CGUIWindow17getMaximizeButtonEv
    IGUIButton* getMaximizeButton() const;

    IGUIButton* closeButton = nullptr;     // +0x164
    IGUIButton* minimizeButton = nullptr;  // +0x168
    IGUIButton* maximizeButton = nullptr;  // +0x16c
};

// ---- Combo box, context menu, check box ----

class CGUIComboBox {
public:
    // 0x6aab14 _ZNK6glitch3gui12CGUIComboBox11getSelectedEv
    std::int32_t getSelected() const;

    std::int32_t selected = -1;  // +0x170
};

class CGUIContextMenu {
public:
    // 0x6ad4d4 _ZNK6glitch3gui15CGUIContextMenu15getSelectedItemEv
    std::int32_t getSelectedItem() const;

    std::int32_t selectedItem = -1;  // +0x158
};

class CGUICheckBox {
public:
    // 0x6a76ac _ZN6glitch3gui12CGUICheckBox10setCheckedEb
    void setChecked(bool checked);
    // 0x6a76b4 _ZNK6glitch3gui12CGUICheckBox9isCheckedEv
    bool isChecked() const;

    bool checked = false;  // +0x159
};

// ---- Edit box ----

class CGUIEditBox {
public:
    // 0x6afaa8 _ZN6glitch3gui11CGUIEditBox13setDrawBorderEb
    void setDrawBorder(bool draw);
    // 0x6afab0 _ZN6glitch3gui11CGUIEditBox19enableOverrideColorEb
    void enableOverrideColor(bool enable);
    // 0x6afab8 _ZNK6glitch3gui11CGUIEditBox17isWordWrapEnabledEv
    bool isWordWrapEnabled() const;
    // 0x6afac0 _ZN6glitch3gui11CGUIEditBox12setMultiLineEb
    void setMultiLine(bool multiLine);
    // 0x6afac8 _ZNK6glitch3gui11CGUIEditBox18isMultiLineEnabledEv
    bool isMultiLineEnabled() const;
    // 0x6afad0 _ZNK6glitch3gui11CGUIEditBox13isPasswordBoxEv
    bool isPasswordBox() const;
    // 0x6afae4 _ZN6glitch3gui11CGUIEditBox13setAutoScrollEb
    void setAutoScroll(bool enable);
    // 0x6afaec _ZNK6glitch3gui11CGUIEditBox19isAutoScrollEnabledEv
    bool isAutoScrollEnabled() const;
    // 0x6afaf4 _ZNK6glitch3gui11CGUIEditBox6getMaxEv
    std::int32_t getMax() const;

    bool drawBorder = false;          // +0x159
    bool overrideColorEnabled = false;  // +0x15a
    bool autoScroll = false;          // +0x18a
    bool multiLine = false;           // +0x189
    bool passwordBox = false;         // +0x18b
    bool wordWrap = false;            // +0x188
    std::int32_t maxChars = 0;        // +0x184
};

// ---- Factory and environment interfaces (empty destructors) ----

class IGUIEnvironment {
public:
    // 0x5353c4 _ZN6glitch3gui15IGUIEnvironmentD1Ev (empty)
    ~IGUIEnvironment() = default;
};

class IGUIElementFactory {
public:
    // 0x6a56e8 _ZN6glitch3gui18IGUIElementFactoryD1Ev (empty)
    ~IGUIElementFactory() = default;
};

class CDefaultGUIElementFactory {
public:
    // 0x6a5d20 _ZN6glitch3gui25CDefaultGUIElementFactoryD1Ev (empty)
    ~CDefaultGUIElementFactory() = default;
    // 0x6a5cc8 _ZNK6glitch3gui25CDefaultGUIElementFactory31getCreatableGUIElementTypeCountEv (returns 0x16)
    std::uint32_t getCreatableGUIElementTypeCount() const;
};

}  // namespace glitch::gui
