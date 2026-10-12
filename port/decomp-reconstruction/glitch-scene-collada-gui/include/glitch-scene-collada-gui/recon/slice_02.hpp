// Slice 02 of the decompiled glitch::gui reconstruction (CGUIListBox, CGUIMenu,
// CGUIMeshViewer, CGUIMessageBox, CGUIModalScreen, CGUIScrollBar, CGUISkin, CGUISpinBox).
//
// The types here are stand-ins in glitch::collada::recon_02. They carry only the
// members the slice touches. Offsets from the 32-bit original are kept as comments
// (`// +0x40`); the code uses named members and never does raw offset arithmetic.
// Base-class layout (IGUIElement, IGUISkin, fonts, sprite banks) is not mapped in
// the original tree, so the virtuals below are the minimum needed to compile.
#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace glitch::collada::recon_02 {

class IGUIElement;
class IGUISkin;
class IGUIFont;
class IGUISpriteBank;
class CGUIScrollBar;
class IGUIEditBox;

// core::rect<int> (16 bytes: upper-left x, y then lower-right x, y).
struct SRect {
    int x0 = 0;  // +0x00
    int y0 = 0;  // +0x04
    int x1 = 0;  // +0x08
    int y1 = 0;  // +0x0c

    int width() const { return x1 - x0; }
    int height() const { return y1 - y0; }
};

// Event record as read by the onEvent bodies. The field meanings follow the
// pseudocode indices: category at word 0, element pointer at word 2, a second
// pointer or key code at word 3, an event code at word 4 (GUI and mouse) or
// word 5 (GUI sub-events), and the key-down flag as a byte at offset 16.
struct SEvent {
    int category = 0;                     // word 0: 0 mouse, 1 GUI, 2 keyboard
    IGUIElement* element = nullptr;       // word 2
    int keyOrRelated = 0;                 // word 3: key code, or related element / value
    int eventCode = 0;                    // word 4
    int guiCode = 0;                      // word 5 (GUI category)
    bool keyDown = false;                 // byte 16 (keyboard category)
    float amount = 0.0f;                  // word 1 as float (wheel amount)
};

// Keyboard and GUI codes used by the bodies below.
enum : int {
    kKeyReturn = 13,
    kKeySpace = 32,
    kKeyEscape = 27,
    kKeyUp = 38,
    kKeyDown = 40,
    kKeyPageUp = 33,
    kKeyPageDown = 34,
    kKeyHome = 36,
    kKeyEnd = 35,
    kKeyY = 89,
    kKeyN = 78,
};

// Platform clock in milliseconds (stand-in for glitch::os::Timer::getTime).
std::uint32_t getTimeMs();

// Stand-in for glitch::gui::IGUIEnvironment: the fields the slice reads.
struct IGUIEnvironment {
    IGUISkin* skin = nullptr;             // environment skin
    IGUIElement* focus = nullptr;         // focused element
};

// Stand-in for the skin interface. Colour, size and font lookups go through
// virtuals; the default implementations return zero.
class IGUISkin {
public:
    virtual ~IGUISkin() = default;
    virtual std::uint32_t getColor(int which) const { (void)which; return 0; }
    virtual int getSize(int which) const { (void)which; return 0; }
    virtual IGUIFont* getFont(int which) const { (void)which; return nullptr; }
    virtual void drawIcon(IGUIElement* element, int icon, int x, int y, std::uint32_t time, bool center,
                          const SRect* clip) {
        (void)element; (void)icon; (void)x; (void)y; (void)time; (void)center; (void)clip;
    }
};

// Stand-in for glitch::gui::IGUIFont. Only the dimension and height queries are used.
class IGUIFont {
public:
    virtual ~IGUIFont() = default;
    virtual int getHeight() const { return 0; }
    virtual int getDimensionWidth(const std::wstring& text) const { (void)text; return 0; }
};

// Stand-in for glitch::gui::IGUIElement. Child order is the original list order.
class IGUIElement {
public:
    IGUIElement() = default;
    IGUIElement(const IGUIElement&) = delete;
    IGUIElement& operator=(const IGUIElement&) = delete;
    virtual ~IGUIElement() = default;

    virtual bool onEvent(const SEvent& event);   // forwards to the parent in the original
    virtual void draw() {}
    virtual void updateAbsolutePosition() {}
    virtual bool isEnabled() const { return enabled; }
    virtual void setRelativePosition(const SRect& rect) { relativeRect = rect; }
    virtual void addChild(IGUIElement* child);
    virtual void removeChild(IGUIElement* child);

    void setTabOrder(int order);
    IGUIElement* getNextElement(int direction, bool forward, bool skipDisabled, IGUIElement*& best,
                                IGUIElement*& candidate, bool recursive) const;

    std::vector<IGUIElement*> children;  // +0x04 child list
    IGUIElement* parent = nullptr;       // +0x24
    IGUIEnvironment* environment = nullptr;  // +0x2c
    SRect absoluteRect;                  // +0x40..+0x4c
    SRect relativeRect;                  // +0x38..
    bool visible = true;                 // +0x98
    bool enabled = true;                 // +0x99
    bool tabStop = true;                 // +0x9a
    bool tabGroup = false;               // +0x9b
    std::wstring text;                   // +0xe4
    int id = -1;                         // +0x130
    int tabOrder = -1;                   // +0x138 (value stored by setTabOrder)
    std::uint32_t tabMaxOrder = 0;       // +0x134
};

// Stand-in for glitch::gui::CGUIScrollBar. The bar keeps the position and the
// maximum; the visual parts are child buttons.
class CGUIScrollBar : public IGUIElement {
public:
    CGUIScrollBar() = default;
    ~CGUIScrollBar() override = default;

    int getPosFromMousePos(int mouseX, int mouseY) const;   // 0x548c34
    void setPos(int pos);                                    // 0x548d90
    void setMax(int max);                                    // 0x548f6c
    void refreshControls();                                  // 0x5490c0 (partial)
    void updateAbsolutePosition() override;                  // 0x5496b4 (partial)
    bool onEvent(const SEvent& event) override;              // 0x54a228 (partial)
    void draw() override;                                    // 0x54a758 (partial)

    int pos = 0;                 // +0x174 current position (index 93)
    int max = 0;                 // +0x180 maximum (index 96)
    int trackStart = 0;          // +0x2c (index 11)
    int trackEnd = 0;            // +0x34 (index 13)
    int trackOrigin = 0;         // +0x28 (index 10)
    int trackExtent = 0;         // +0x30 (index 12)
    int thumbPos = 0;            // +0x178 (index 94)
    int thumbSize = 0;           // +0x17c (index 95)
    bool horizontal = false;     // +0x171 (byte, index 369)
    IGUIElement* upButton = nullptr;    // +0x158 (index 86)
    IGUIElement* downButton = nullptr;  // +0x15c (index 87)
};

// Item record of CGUIListBox. The original stride is 96 bytes; the per-colour
// override flag and colour are stored per colour slot.
struct SListItem {
    std::wstring text;                         // +0x00 (string object)
    int data = 0;                              // +0x48 value passed to setItem
    int widthMax = 0;                          // +0x4c
    bool overrideSet[4] = {};                  // flags per colour slot
    std::uint32_t overrideColor[4] = {};       // colour per slot
    int top = 0;                               // +0x40 (list-space y)
    int bottom = 0;                            // +0x44
};

// Stand-in for glitch::gui::CGUIListBox.
class CGUIListBox : public IGUIElement {
public:
    CGUIListBox() = default;
    ~CGUIListBox() override = default;

    void recalculateScrollPos();                       // 0x541e18
    void recalculateItemWidth(std::uint32_t width);    // 0x541ef4 (partial)
    void setItemOverrideColor(std::uint32_t index, int colorIndex, std::uint32_t color); // 0x542050
    std::uint32_t getItemDefaultColor(int colorIndex) const;                             // 0x542258
    void selectNew(int index, bool setBySecond);       // 0x542400
    void setSelected(std::uint32_t index);             // 0x542518
    bool onEvent(const SEvent& event) override;        // 0x542f54 (partial)
    void draw() override;                              // 0x5439b4 (partial)
    void clear();                                      // 0x544280
    void removeItem(std::uint32_t index);              // 0x544390
    void swapItems(std::uint32_t a, std::uint32_t b);  // 0x5446dc
    void setItem(std::uint32_t index, const wchar_t* text, int data); // 0x544828
    bool getSerializationLabels(int colorIndex, std::string& useLabel, std::string& colorLabel) const; // 0x5448ac
    std::uint32_t insertItem(std::uint32_t index, const wchar_t* text, int data); // 0x545304
    std::uint32_t addItem(const wchar_t* text, int data);                         // 0x545480
    void recalculateItemHeight();                      // used by clear, insertItem, addItem (not in slice)

    std::vector<SListItem> items;      // +0x158 begin, +0x15c end
    int selected = -1;                 // +0x164 (index 89)
    int itemHeight = 0;                // +0x168 (index 90)
    int defaultItemHeight = 0;         // +0x16c (index 91)
    int maxItemWidth = 0;              // +0x170 (index 92)
    IGUISpriteBank* iconBank = nullptr;  // +0x174 (index 93)
    IGUISpriteBank* selectIcons = nullptr; // +0x178 (index 94)
    CGUIScrollBar* scrollBar = nullptr;  // +0x17c (index 95)
    std::uint32_t lastSelectTime = 0;  // +0x184 (index 97)
    bool mouseSelectArmed = false;     // +0x180
    bool drawBackground = false;       // +0x181
    bool allowKeySelect = false;       // +0x182
    std::wstring typeAhead;            // +0x18c (type-ahead buffer)
    std::uint32_t typeAheadTime = 0;   // +0x1d4 (index 117)
};

// Stand-in for glitch::gui::CGUIMenu (context menu).
class CGUIMenu : public IGUIElement {
public:
    CGUIMenu() = default;
    ~CGUIMenu() override = default;
    void recalculateSize();                             // 0x5455b8 (partial)
    SRect getRect(const SRect& base) const;             // 0x54585c
    void updateAbsolutePosition() override;             // 0x545878
    bool onEvent(const SEvent& event) override;         // 0x545c18 (partial)
    void draw() override;                               // 0x545fc4 (partial)

    int parentWidth = 0;               // +0x60 (index 24)
    std::uint32_t hoverItem = 0;       // +0x158 (index 86) -1 when none
};

// Stand-in for glitch::gui::CGUIMeshViewer.
class CGUIMeshViewer : public IGUIElement {
public:
    CGUIMeshViewer() = default;
    ~CGUIMeshViewer() override = default;
    void setMesh(void* mesh);                          // 0x5462c4 (ref held by the viewer)
    void* getMesh() const;                             // 0x5462f0
    void* getMaterial() const;                         // 0x54630c
    void setMaterial(void* material);                  // 0x546348
    bool onEvent(const SEvent& event) override;        // 0x5467ac
    void draw() override;                              // 0x5469fc (partial)

    void* mesh = nullptr;              // +0x158 (index 86 area; held reference)
    void* material = nullptr;          // +0x158 (index 86)
};

// Stand-in for glitch::gui::CGUIModalScreen.
class CGUIModalScreen : public IGUIElement {
public:
    CGUIModalScreen() = default;
    ~CGUIModalScreen() override = default;
    void updateAbsolutePosition() override;            // 0x548260
    void removeChild(IGUIElement* child) override;     // 0x5482c8
    void addChild(IGUIElement* child) override;        // 0x5482fc
    void draw() override;                              // 0x5488bc (partial)
    bool onEvent(const SEvent& event) override;        // 0x548a50 (partial)

    std::uint32_t lastClickTime = 0;   // +0x158 (index 86)
};

// Stand-in for glitch::gui::CGUIMessageBox.
class CGUIMessageBox : public IGUIElement {
public:
    CGUIMessageBox() = default;
    ~CGUIMessageBox() override = default;
    bool onEvent(const SEvent& event) override;        // 0x547014 (partial)
    void refreshControls();                            // 0x5472e0 (partial)

    IGUIElement* okButton = nullptr;     // +0x170 (index 92 area)
    IGUIElement* cancelButton = nullptr; // +0x174
    IGUIElement* yesButton = nullptr;    // +0x178
    IGUIElement* noButton = nullptr;     // +0x17c
    IGUIElement* closeButton = nullptr;  // +0x184? (index 95)
    bool modalActive = false;            // +0x1d0 (byte 464)
};

// Stand-in for glitch::gui::CGUISkin.
class CGUISkin : public IGUISkin {
public:
    IGUIFont* getFont(int which) const override;       // 0x54aac4
    void setFont(IGUIFont* font, int which);           // 0x54aaf0
    void setSpriteBank(IGUISpriteBank* bank);          // 0x54ab40
    void drawIcon(IGUIElement* element, int icon, int x, int y, std::uint32_t time, bool center,
                  const SRect* clip) override;         // 0x54abb0
    void setDefaultText(int which, const wchar_t* text); // 0x54d0bc

    IGUIFont* fonts[5] = {};             // +0xdc..+0xec (index 55..59)
    IGUISpriteBank* spriteBank = nullptr; // +0xf0 (index 60)
    std::wstring defaultText[8];          // +0xf4 (72-byte stride in the original)
};

// Stand-in for the edit box child of a spin box. The text accessors are the only
// members the spin box touches.
class IGUIEditBox : public IGUIElement {
public:
    virtual void setText(const wchar_t* value) { text = value ? value : L""; }
    virtual const wchar_t* getText() const { return text.c_str(); }
};

// Stand-in for glitch::gui::CGUISpinBox.
class CGUISpinBox : public IGUIElement {
public:
    void setRange(float low, float high);              // 0x54df40
    void verifyValueRange();                           // 0x54df7c
    void setText(const wchar_t* value);                // 0x54dfd8
    const wchar_t* getText() const;                    // 0x54e028

    float rangeLow = 0.0f;               // +0x168 (index 90)
    float rangeHigh = 0.0f;              // +0x16c (index 91)
    IGUIEditBox* editBox = nullptr;     // +0x158 (index 86)

    virtual float getValue() const { return 0.0f; }      // slot +132
    virtual void setValue(float value) { (void)value; }  // slot +128
    virtual void refresh() {}                            // slot +160
};

}  // namespace glitch::collada::recon_02
