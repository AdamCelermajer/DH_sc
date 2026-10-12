// glitch::gui accessors and interface destructors whose bodies are constant
// returns, one-member reads or one-member writes. Each class maps only the
// members its bodies touch; offsets are kept as comments and the code uses typed
// members only. Virtual dispatch is not modelled (no vtables in the stand-ins).
#pragma once

#include <cstdint>
#include <vector>

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"

namespace glitch::gui {

class IGUIFont;
class IGUISpriteBank;
class IGUIButton;
class CGUITab;
class CGUIContextMenu;

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

// One default-text entry of the skin. Only the text pointer (first word) is
// mapped; the entries are 0x48 bytes apart in the original.
struct SSkinDefaultText {
    const wchar_t* text = nullptr;  // +0x00 of each entry (stride 0x48)
};

class CGUISkin {
public:
    // 0x54ab38 _ZNK6glitch3gui8CGUISkin13getSpriteBankEv
    IGUISpriteBank* getSpriteBank() const;
    // 0x54ac2c _ZNK6glitch3gui8CGUISkin7getTypeEv
    std::int32_t getType() const;
    // 0x54aa24 _ZNK6glitch3gui8CGUISkin8getColorENS0_18EGUI_DEFAULT_COLORE
    // Out-of-range indices (>= 21) return the colour the original leaves in its
    // registers; that value is not recoverable, so zero is returned (partial).
    video::SColor getColor(std::uint32_t which) const;
    // 0x54aaa0 _ZNK6glitch3gui8CGUISkin7getSizeENS0_17EGUI_DEFAULT_SIZEE (0 when out of range)
    std::int32_t getSize(std::uint32_t which) const;
    // 0x54aab4 _ZN6glitch3gui8CGUISkin7setSizeENS0_17EGUI_DEFAULT_SIZEEi
    void setSize(std::uint32_t which, std::int32_t size);
    // 0x54ab74 _ZNK6glitch3gui8CGUISkin7getIconENS0_17EGUI_DEFAULT_ICONE (0 when out of range)
    std::uint32_t getIcon(std::uint32_t which) const;
    // 0x54ab88 _ZN6glitch3gui8CGUISkin7setIconENS0_17EGUI_DEFAULT_ICONEj
    void setIcon(std::uint32_t which, std::uint32_t icon);
    // 0x54ab98 _ZNK6glitch3gui8CGUISkin14getDefaultTextENS0_17EGUI_DEFAULT_TEXTE
    // Out-of-range indices fall back to entry 0.
    const wchar_t* getDefaultText(std::uint32_t which) const;

    video::SColor colors[0x15];          // +0x04..+0x57, 21 entries
    std::int32_t sizes[10] = {};         // +0x58..+0x7f
    std::uint32_t icons[0x17] = {};      // +0x80..+0xdb, 23 entries
    SSkinDefaultText defaultTexts[8];    // +0x138..+0x1b7, stride 0x48
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
    // 0x551838 _ZN6glitch3gui14CGUIStaticText11setWordWrapEb
    // Partial: the original then re-breaks the text; breakText is not ported, so only the flag is set.
    void setWordWrap(bool enable);
    // 0x54ffc4 _ZNK6glitch3gui14CGUIStaticText17isWordWrapEnabledEv
    bool isWordWrapEnabled() const;
    // 0x54ff1c _ZN6glitch3gui14CGUIStaticText16setOverrideColorENS_5video6SColorE
    // Also sets the override-colour enable flag, as the original does.
    void setOverrideColor(video::SColor color);
    // 0x54ff54 _ZN6glitch3gui14CGUIStaticText18setBackgroundColorENS_5video6SColorE
    // Also sets the draw-background flag.
    void setBackgroundColor(video::SColor color);
    // 0x54ff9c _ZN6glitch3gui14CGUIStaticText16setTextAlignmentENS0_14EGUI_ALIGNMENTES2_
    void setTextAlignment(std::uint32_t horizontal, std::uint32_t vertical);
    // 0x54ffa8 _ZNK6glitch3gui14CGUIStaticText16getOverrideColorEv
    const video::SColor& getOverrideColor() const;

    bool overrideColorEnabled = false;  // +0x170
    bool drawBorder = false;            // +0x164
    bool drawBackground = false;        // +0x172
    bool wordWrap = false;              // +0x171
    video::SColor overrideColor;        // +0x173 (unaligned in the original)
    video::SColor backgroundColor;      // +0x177 (unaligned in the original)
    std::uint32_t horizontalAlign = 0;  // +0x168
    std::uint32_t verticalAlign = 0;    // +0x16c
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
    // 0x552214 _ZNK6glitch3gui14CGUITabControl11getTabCountEv
    std::int32_t getTabCount() const;
    // 0x552228 _ZNK6glitch3gui14CGUITabControl6getTabEi (null when out of range)
    CGUITab* getTab(std::uint32_t index) const;

    std::vector<CGUITab*> tabs;               // +0x158 (begin) .. +0x15c (end)
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
    // 0x554f60 _ZNK6glitch3gui9CGUITable14getColumnCountEv
    std::int32_t getColumnCount() const;
    // 0x554f80 _ZNK6glitch3gui9CGUITable11getRowCountEv
    std::int32_t getRowCount() const;
    // 0x555120 _ZN6glitch3gui9CGUITable12setCellColorEjjNS_5video6SColorE (ignored when out of range)
    void setCellColor(std::uint32_t row, std::uint32_t column, video::SColor color);
    // 0x5551bc _ZN6glitch3gui9CGUITable11setCellDataEjjPv (ignored when out of range)
    void setCellData(std::uint32_t row, std::uint32_t column, void* data);
    // 0x555230 _ZNK6glitch3gui9CGUITable11getCellTextEjj (null when out of range)
    const wchar_t* getCellText(std::uint32_t row, std::uint32_t column) const;
    // 0x5552ac _ZNK6glitch3gui9CGUITable11getCellDataEjj (null when out of range)
    void* getCellData(std::uint32_t row, std::uint32_t column) const;
    // 0x555914 _ZN6glitch3gui9CGUITable17setColumnOrderingEjNS0_20EGUI_COLUMN_ORDERINGE (ignored when out of range)
    void setColumnOrdering(std::uint32_t column, std::int32_t ordering);

    // One table cell. The original record is 0x98 bytes; the mapped fields are the
    // text pointer, the colour and the user data.
    struct SCell {
        const wchar_t* text = nullptr;  // +0x44
        video::SColor color;            // +0x90 (bytes +0x90..+0x93)
        void* data = nullptr;           // +0x94
    };
    // One row record: the cell array (stride 0x98) is its first word; the row
    // record itself is 0x0c bytes.
    struct SRow {
        std::vector<SCell> cells;  // +0x00
    };
    // One column record: stride 0x54. Only the ordering field (+0x50) is mapped.
    struct SColumn {
        std::int32_t ordering = 0;  // +0x50
    };

    std::vector<SColumn> columns;  // +0x158 (begin) .. +0x15c (end)
    std::vector<SRow> rows;        // +0x164 (begin) .. +0x168 (end)

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
    // 0x55bcec _ZN6glitch3gui10CGUITTFont11setTrackingEi
    // Negative values are stored as zero.
    void setTracking(std::int32_t value);
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
    // 0x6aaa88 _ZNK6glitch3gui12CGUIComboBox12getItemCountEv
    std::int32_t getItemCount() const;
    // 0x6aaab8 _ZNK6glitch3gui12CGUIComboBox7getItemEj (null when out of range)
    const wchar_t* getItem(std::uint32_t index) const;

    // One combo item record, stride 0x48; the text pointer is at +0x44.
    struct SItem {
        const wchar_t* text = nullptr;  // +0x44
    };

    std::vector<SItem> items;  // +0x164 (begin) .. +0x168 (end)
    std::int32_t selected = -1;  // +0x170
};

class CGUIContextMenu {
public:
    // One menu item record, stride 0x60. Field names follow the uses in the
    // accessors; the layout of the other fields is not mapped.
    struct SItem {
        const wchar_t* text = nullptr;   // +0x44
        bool enabled = false;            // +0x49
        bool checked = false;            // +0x4a
        std::int32_t width = 0;          // +0x4c (menu bar: horizontal extent)
        std::int32_t height = 0;         // +0x50 (context menu: vertical extent)
        std::int32_t offset = 0;         // +0x54 (menu bar: x offset, context menu: y offset)
        CGUIContextMenu* subMenu = nullptr;  // +0x58
        std::int32_t commandId = 0;      // +0x5c
    };

    // 0x6ad4d4 _ZNK6glitch3gui15CGUIContextMenu15getSelectedItemEv
    std::int32_t getSelectedItem() const;
    // 0x6acdac _ZNK6glitch3gui15CGUIContextMenu12getItemCountEv
    std::int32_t getItemCount() const;
    // 0x6acf08 _ZNK6glitch3gui15CGUIContextMenu11getItemTextEj (null when out of range)
    const wchar_t* getItemText(std::uint32_t index) const;
    // 0x6acf44 _ZNK6glitch3gui15CGUIContextMenu13isItemEnabledEj (false when out of range)
    bool isItemEnabled(std::uint32_t index) const;
    // 0x6acf80 _ZNK6glitch3gui15CGUIContextMenu13isItemCheckedEj (false when out of range)
    bool isItemChecked(std::uint32_t index) const;
    // 0x6acfbc _ZN6glitch3gui15CGUIContextMenu14setItemEnabledEjb (ignored when out of range)
    void setItemEnabled(std::uint32_t index, bool enabled);
    // 0x6acff4 _ZN6glitch3gui15CGUIContextMenu14setItemCheckedEjb (ignored when out of range)
    void setItemChecked(std::uint32_t index, bool checked);
    // 0x6ad1d0 _ZNK6glitch3gui15CGUIContextMenu8getHRectERKNS1_5SItemERKNS_4core4rectIiEE
    core::rect<std::int32_t> getHRect(const SItem& item, const core::rect<std::int32_t>& rect) const;
    // 0x6ad214 _ZNK6glitch3gui15CGUIContextMenu7getRectERKNS1_5SItemERKNS_4core4rectIiEE
    core::rect<std::int32_t> getRect(const SItem& item, const core::rect<std::int32_t>& rect) const;
    // 0x6ad4dc _ZNK6glitch3gui15CGUIContextMenu10getSubMenuEj (null when out of range)
    CGUIContextMenu* getSubMenu(std::uint32_t index) const;
    // 0x6ad518 _ZNK6glitch3gui15CGUIContextMenu16getItemCommandIdEj (-1 when out of range)
    std::int32_t getItemCommandId(std::uint32_t index) const;
    // 0x6ad554 _ZN6glitch3gui15CGUIContextMenu16setItemCommandIdEji (ignored when out of range)
    void setItemCommandId(std::uint32_t index, std::int32_t commandId);

    std::int32_t selectedItem = -1;  // +0x158
    std::vector<SItem> items;        // +0x15c (begin) .. +0x160 (end)
};

// Menu bar. Its highlight rectangle uses the same item record as the context menu.
class CGUIMenu {
public:
    // 0x545818 _ZNK6glitch3gui8CGUIMenu8getHRectERKNS0_15CGUIContextMenu5SItemERKNS_4core4rectIiEE
    core::rect<std::int32_t> getHRect(const CGUIContextMenu::SItem& item,
                                      const core::rect<std::int32_t>& rect) const;
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
    // 0x6b1624 _ZN6glitch3gui11CGUIEditBox11setWordWrapEb
    // Partial: the original then re-breaks the text; breakText is not ported, so only the flag is set.
    void setWordWrap(bool enable);
    // 0x6afab8 _ZNK6glitch3gui11CGUIEditBox17isWordWrapEnabledEv
    bool isWordWrapEnabled() const;
    // 0x6afac0 _ZN6glitch3gui11CGUIEditBox12setMultiLineEb
    void setMultiLine(bool multiLine);
    // 0x6afac8 _ZNK6glitch3gui11CGUIEditBox18isMultiLineEnabledEv
    bool isMultiLineEnabled() const;
    // 0x6afad0 _ZNK6glitch3gui11CGUIEditBox13isPasswordBoxEv
    bool isPasswordBox() const;
    // 0x6afa70 _ZN6glitch3gui11CGUIEditBox16setOverrideColorENS_5video6SColorE
    // Also sets the override-colour enable flag, as the original does.
    void setOverrideColor(video::SColor color);
    // 0x6afad8 _ZN6glitch3gui11CGUIEditBox16setTextAlignmentENS0_14EGUI_ALIGNMENTES2_
    void setTextAlignment(std::uint32_t horizontal, std::uint32_t vertical);
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
    video::SColor overrideColor;      // +0x164
    std::uint32_t horizontalAlign = 0;  // +0x190
    std::uint32_t verticalAlign = 0;    // +0x194
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

// Element type ids in the factory's name table (glitch's order; the upstream
// EGUIElementTypes.h list is the same up to EGUIET_WINDOW minus its tree-view entry).
enum EGUI_ELEMENT_TYPE : std::uint32_t {
    EGUIET_BUTTON = 0,
    EGUIET_CHECK_BOX,
    EGUIET_COMBO_BOX,
    EGUIET_CONTEXT_MENU,
    EGUIET_MENU,
    EGUIET_EDIT_BOX,
    EGUIET_FILE_OPEN_DIALOG,
    EGUIET_COLOR_SELECT_DIALOG,
    EGUIET_IN_OUT_FADER,
    EGUIET_IMAGE,
    EGUIET_LIST_BOX,
    EGUIET_MESH_VIEWER,
    EGUIET_MESSAGE_BOX,
    EGUIET_MODAL_SCREEN,
    EGUIET_SCROLL_BAR,
    EGUIET_SPIN_BOX,
    EGUIET_STATIC_TEXT,
    EGUIET_TAB,
    EGUIET_TAB_CONTROL,
    EGUIET_TABLE,
    EGUIET_TOOL_BAR,
    EGUIET_WINDOW,
};

class CDefaultGUIElementFactory {
public:
    // 0x6a56ec _ZN6glitch3gui25CDefaultGUIElementFactoryC2EPNS0_15IGUIEnvironmentE
    // 0x6a571c _ZN6glitch3gui25CDefaultGUIElementFactoryC1EPNS0_15IGUIEnvironmentE
    explicit CDefaultGUIElementFactory(IGUIEnvironment* env);
    // 0x6a5d20 _ZN6glitch3gui25CDefaultGUIElementFactoryD1Ev (empty)
    ~CDefaultGUIElementFactory() = default;
    // 0x6a5cc8 _ZNK6glitch3gui25CDefaultGUIElementFactory31getCreatableGUIElementTypeCountEv (returns 0x16)
    std::uint32_t getCreatableGUIElementTypeCount() const;
    // 0x6a5cd0 _ZNK6glitch3gui25CDefaultGUIElementFactory27getCreateableGUIElementTypeEi
    // Indices above 0x15 (including negative ones read as unsigned) become 0x17.
    std::uint32_t getCreateableGUIElementType(std::int32_t index) const;
    // 0x6a5ce0 _ZNK6glitch3gui25CDefaultGUIElementFactory31getCreateableGUIElementTypeNameEi (null when out of range)
    const char* getCreateableGUIElementTypeName(std::int32_t index) const;
    // 0x6a5d00 _ZNK6glitch3gui25CDefaultGUIElementFactory31getCreateableGUIElementTypeNameENS0_17EGUI_ELEMENT_TYPEE (null when out of range)
    const char* getCreateableGUIElementTypeName(EGUI_ELEMENT_TYPE type) const;

    std::uint32_t referenceCount = 1;        // +0x04 (IReferenceCounted base, starts at 1)
    IGUIEnvironment* environment = nullptr;  // +0x08
};

// ---- Interface destructors (D1 bodies) ----
// Each interface destructor stores the vtable pointers, runs the IGUIElement base
// destructor and returns. The stand-ins have no vtables and no base members, so the
// destructors are defaulted here. The deleting variants (D0) are not ledgered.

class IGUIFileOpenDialog {
public:
    // 0x53c510 _ZN6glitch3gui18IGUIFileOpenDialogD1Ev (empty D1)
    ~IGUIFileOpenDialog() = default;
};

class IGUIImage {
public:
    // 0x540a74 _ZN6glitch3gui9IGUIImageD1Ev (empty D1)
    ~IGUIImage() = default;
};

class IGUIInOutFader {
public:
    // 0x541844 _ZN6glitch3gui14IGUIInOutFaderD1Ev (empty D1)
    ~IGUIInOutFader() = default;
};

class IGUIListBox {
public:
    // 0x544428 _ZN6glitch3gui11IGUIListBoxD1Ev (empty D1)
    ~IGUIListBox() = default;
};

class IGUIContextMenu {
public:
    // 0x545bb4 _ZN6glitch3gui15IGUIContextMenuD1Ev (empty D1)
    ~IGUIContextMenu() = default;
};

class IGUIMeshViewer {
public:
    // 0x5467d8 _ZN6glitch3gui14IGUIMeshViewerD1Ev (empty D1)
    ~IGUIMeshViewer() = default;
};

class IGUIWindow {
public:
    // 0x547c50 _ZN6glitch3gui10IGUIWindowD1Ev (empty D1)
    ~IGUIWindow() = default;
};

class IGUIScrollBar {
public:
    // 0x549f44 _ZN6glitch3gui13IGUIScrollBarD1Ev (empty D1)
    ~IGUIScrollBar() = default;
};

class IGUISpinBox {
public:
    // 0x54f364 _ZN6glitch3gui11IGUISpinBoxD1Ev (empty D1)
    ~IGUISpinBox() = default;
};

class IGUIStaticText {
public:
    // 0x550f24 _ZN6glitch3gui14IGUIStaticTextD1Ev (empty D1)
    ~IGUIStaticText() = default;
};

class IGUITab {
public:
    // 0x553f30 _ZN6glitch3gui7IGUITabD1Ev (empty D1)
    ~IGUITab() = default;
};

class IGUITabControl {
public:
    // 0x554070 _ZN6glitch3gui14IGUITabControlD1Ev (empty D1)
    ~IGUITabControl() = default;
};

class IGUITable {
public:
    // 0x557aa0 _ZN6glitch3gui9IGUITableD1Ev (empty D1)
    ~IGUITable() = default;
};

class IGUIToolBar {
public:
    // 0x55b7c8 _ZN6glitch3gui11IGUIToolBarD1Ev (empty D1)
    ~IGUIToolBar() = default;
};

class IGUIButton {
public:
    // 0x6a69b8 _ZN6glitch3gui10IGUIButtonD1Ev (empty D1)
    ~IGUIButton() = default;
};

class IGUICheckBox {
public:
    // 0x6a7d54 _ZN6glitch3gui12IGUICheckBoxD1Ev (empty D1)
    ~IGUICheckBox() = default;
};

class IGUIColorSelectDialog {
public:
    // 0x6a9b90 _ZN6glitch3gui21IGUIColorSelectDialogD1Ev (empty D1)
    ~IGUIColorSelectDialog() = default;
};

class IGUIComboBox {
public:
    // 0x6acbbc _ZN6glitch3gui12IGUIComboBoxD1Ev (empty D1)
    ~IGUIComboBox() = default;
};

class IGUIEditBox {
public:
    // 0x6b0b38 _ZN6glitch3gui11IGUIEditBoxD1Ev (empty D1)
    ~IGUIEditBox() = default;
};

// Sprite bank. Only the texture list is mapped; the sprite and position lists
// that come before it are not modelled yet.
class CGUISpriteBank {
public:
    // 0x54f8c0 _ZNK6glitch3gui14CGUISpriteBank15getTextureCountEv
    std::int32_t getTextureCount() const;

    std::uint8_t pad_0000[0x20] = {};  // +0x00..+0x1f (sprite and position lists, not mapped)
    std::vector<void*> textures;       // +0x20 (begin) .. +0x24 (end), one entry per texture
};

}  // namespace glitch::gui
