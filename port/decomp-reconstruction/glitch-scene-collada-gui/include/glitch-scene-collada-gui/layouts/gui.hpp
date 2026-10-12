// glitch::gui member layouts reconstructed from the Hex-Rays pseudocode of the
// glitch-scene-collada-gui category (recovered/native/hexrays-all). Offsets are the
// 32-bit ARM ones. Only classes whose pseudocode in this category touches a
// member are declared: IGUIElement, CGUIComboBox and CGUIContextMenu. Declarations
// only; no function bodies. Not compatible with gui_elements.hpp in the same
// translation unit (both define glitch::gui::IGUIElement).
//
// Sources, per member:
//   IGUIElement::setTabOrder, IGUIElement::getNextElement (both copies),
//   IGUIComboBox ctor 0x6aae64, CGUIComboBox ctors 0x6ab38c / 0x6ab740,
//   CGUIComboBox accessors, IGUIContextMenu ctor 0x6adcf0, CGUIContextMenu ctors
//   0x6adfb8 / 0x6ae100, and the CGUIContextMenu accessors.
#ifndef GLITCH_SCENE_COLLADA_GUI_LAYOUTS_GUI_HPP
#define GLITCH_SCENE_COLLADA_GUI_LAYOUTS_GUI_HPP

#include <cstdint>

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"

namespace glitch::gui {

class IGUIEnvironment;
class CGUIListBox;

// glitch::gui::IGUIElement, the base of the GUI element family.
// Mapped so far: the child list, parent, rectangles, flag bytes, tab fields, id and
// guiType. Gaps are pads. The string storage at +0x0c..+0x23 and +0xa0..+0xdf is
// not decoded here.
class IGUIElement {
public:
    std::uint8_t pad_0x00[0x04];             // +0x00 vtable pointer (implicit in the original)
    void* childrenNext = nullptr;            // +0x04 child list sentinel, next (self-linked when empty)
    void* childrenPrev = nullptr;            // +0x08 child list sentinel, prev
    std::uint8_t pad_0x0c[0x18];             // +0x0c..+0x23 string header, not mapped
    IGUIElement* parent = nullptr;           // +0x24 parent element (walked by setTabOrder)
    core::rect<std::int32_t> relativeRect;   // +0x28 relative rectangle (ctor copies the rect argument)
    core::rect<std::int32_t> absoluteRect;   // +0x38 absolute rectangle (ctor derives it from the parent's +0x38)
    core::rect<std::int32_t> absoluteClippingRect; // +0x48 clipping rectangle (ctor clips it to the parent's +0x48)
    core::rect<std::int32_t> desiredRect;    // +0x58 desired rectangle (ctor copies the rect argument)
    core::rect<std::int32_t> lastParentRect; // +0x68 parent absolute rectangle as last seen
    std::uint32_t field_0x78[3];             // +0x78..+0x83 zeroed by the constructors, role not confirmed
    std::uint32_t field_0x84;                // +0x84 zeroed by the constructors, role not confirmed
    std::uint32_t field_0x88[2];             // +0x88..+0x8f zeroed by the constructors, role not confirmed
    std::int32_t field_0x90 = 1;             // +0x90 set to 1 by the constructors, role not confirmed
    std::int32_t field_0x94 = 1;             // +0x94 set to 1 by the constructors, role not confirmed
    std::uint8_t visible = 1;                // +0x98 set to 1 by the constructors
    std::uint8_t enabled = 1;                // +0x99 set to 1 by the constructors
    std::uint8_t subElement = 0;             // +0x9a
    std::uint8_t field_0x9b = 0;             // +0x9b, type not confirmed; set to 1 at the end of the combo and context menu ctors
    std::uint8_t field_0x9c = 0;             // +0x9c zeroed by the constructors, role not confirmed
    std::uint8_t pad_0x9d[0x03];             // +0x9d..+0x9f
    std::uint8_t pad_0xa0[0x40];             // +0xa0..+0xdf string storage, not decoded here
    void* field_0xe0 = nullptr;              // +0xe0 set to the address of the +0xa0 storage by the constructors
    void* field_0xe4 = nullptr;              // +0xe4 set to the address of the +0xa0 storage by the constructors (gui_elements.hpp reads this word as the text pointer)
    std::uint8_t pad_0xe8[0x40];             // +0xe8..+0x127 tool-tip string storage, not decoded here
    void* field_0x128 = nullptr;             // +0x128 set to the address of the +0xe8 storage by the constructors
    void* field_0x12c = nullptr;             // +0x12c set to the address of the +0xe8 storage by the constructors
    std::int32_t id = 0;                     // +0x130 element id (constructor argument)
    std::uint8_t tabStop = 0;                // +0x134 tab-stop flag (getNextElement tests it)
    std::uint8_t pad_0x135[0x03];            // +0x135..+0x137
    std::int32_t tabOrder = -1;              // +0x138 tab order, -1 by default (setTabOrder writes it)
    std::uint8_t tabGroup = 0;               // +0x13c tab-group flag (setTabOrder and getNextElement test it)
    std::uint8_t pad_0x13d[0x03];            // +0x13d..+0x13f
    std::uint8_t pad_0x140[0x10];            // +0x140..+0x14f zeroed by the constructors, not named
    void* environment = nullptr;             // +0x150 third constructor argument; the combo and menu code calls through it, type not confirmed
    std::int32_t guiType = 0;                // +0x154 type index, set by each derived constructor (2 for the combo box, 3 for the context menu)
};

// glitch::gui::CGUIComboBox, derived from IGUIElement (the IGUIComboBox base adds no
// members of its own below +0x158).
class CGUIComboBox : public IGUIElement {
public:
    IGUIElement* dropButton = nullptr;       // +0x158 button element (vtable +56 and tab-stop flag set on it)
    IGUIElement* textElement = nullptr;      // +0x15c element showing the selected text (setText through vtable slot +68)
    CGUIListBox* listBox = nullptr;          // +0x160 drop-down list box, created in openCloseMenu
    void* itemsBegin = nullptr;              // +0x164 item vector begin (72-byte entries: text pointer at entry +0x44)
    void* itemsEnd = nullptr;                // +0x168 item vector end
    void* itemsCapacity = nullptr;           // +0x16c item vector capacity
    std::int32_t selected = -1;              // +0x170 selected item index, -1 when none
    std::uint8_t field_0x174 = 0;            // +0x174, type not confirmed, zeroed by the constructor
    std::int32_t field_0x178 = 0;            // +0x178, zeroed by the constructor, role not confirmed
    std::int32_t horizontalAlignment = 0;    // +0x17c set by setTextAlignment (first argument)
    std::int32_t verticalAlignment = 2;      // +0x180 set by setTextAlignment (second argument); the constructor stores 2
    std::uint8_t pad_0x184[0x08];            // +0x184..+0x18b: secondary vtable words written by the constructors and destructor
    std::int32_t field_0x18c = 1;            // +0x18c set to 1 by the derived constructor, role not confirmed
};

// glitch::gui::CGUIContextMenu, derived from IGUIElement.
class CGUIContextMenu : public IGUIElement {
public:
    std::int32_t highlightedItem = -1;       // +0x158 index used by highlight and setVisible, -1 by default
    void* itemsBegin = nullptr;              // +0x15c item vector begin (addItem, removeItem, sendClick, closeAllSubMenus)
    void* itemsEnd = nullptr;                // +0x160 item vector end
    void* itemsCapacity = nullptr;           // +0x164 item vector capacity, zeroed by the constructor
    std::int32_t extent[2] = {0, 0};         // +0x168..+0x16f pair copied from *a5 by the constructor (position or size)
    std::uint32_t field_0x170 = 0;           // +0x170 zeroed by the constructor, role not confirmed
    std::uint32_t field_0x174 = 0;           // +0x174 zeroed by the constructor, role not confirmed
    std::uint8_t flag_0x178 = 0;             // +0x178, type not confirmed; set from the constructor's last argument, tested by onEvent
    std::uint8_t pad_0x179[0x03];            // +0x179..+0x17b
    std::uint32_t field_0x17c = 0;           // +0x17c zeroed by the constructor, role not confirmed
    std::uint8_t pad_0x180[0x08];            // +0x180..+0x187: secondary vtable words written by the constructors and destructor
    std::int32_t field_0x188 = 1;            // +0x188 set to 1 by the constructor, role not confirmed
};

}  // namespace glitch::gui

#endif  // GLITCH_SCENE_COLLADA_GUI_LAYOUTS_GUI_HPP
