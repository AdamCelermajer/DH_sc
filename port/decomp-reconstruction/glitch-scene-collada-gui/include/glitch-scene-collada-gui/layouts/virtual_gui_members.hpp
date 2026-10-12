// Member layouts for classes whose ledger rows needed member declarations
// (layouts/gui.hpp, gui_simple_accessors.hpp, scene_simple_accessors.hpp), taken
// from the Hex-Rays pseudocode of the glitch-scene-collada-gui category. 32-bit ARM
// offsets. Declarations only, no function bodies.
//
// Pointers are stored as 32-bit guest words (std::uint32_t) so the offsets hold on
// the 64-bit host. Each named member carries its offset; offsets are checked with
// static_assert. Gaps are pad arrays, and gap contents that are not read by the
// cited bodies are marked "not confirmed".
//
// Base layers: the GUI classes sit on glitch::gui::IGUIElement, which occupies
// +0x00..+0x157 and is declared in layouts/gui.hpp (not repeated here). The scene
// classes sit on ISceneNode / ISceneNodeAnimator, whose layouts are not declared, so
// their leading bytes are pads.
//
// Name clashes: CGUIButton, CGUICheckBox, CGUIEditBox, CSceneNodeAnimatorCollisionResponse
// and CAnimatedMeshSceneNode are also declared with partial members in gui_elements.hpp,
// gui_simple_accessors.hpp and scene_simple_accessors.hpp. Do not include this header in
// the same translation unit as those.
//
// Not declared here (already declared with matching offsets in layouts/scene_members.hpp):
// CSceneNodeAnimatorFlyCircle, CSceneNodeAnimatorFlyStraight, CSceneNodeAnimatorRotation,
// CSceneNodeAnimatorTexture, CDefaultSceneNodeAnimatorFactory. CTerrainSceneNode is also
// declared there, but its +0x17c and +0x178..0x17c entries conflict with the pseudocode
// (see the report). The animation_track rows (CVirtualEx, CApplyValueEx, CInterpreter)
// touch no members of their own class; see layouts/animation_track.hpp.
#ifndef GLITCH_SCENE_COLLADA_GUI_LAYOUTS_VIRTUAL_GUI_MEMBERS_HPP
#define GLITCH_SCENE_COLLADA_GUI_LAYOUTS_VIRTUAL_GUI_MEMBERS_HPP

#include <cstddef>
#include <cstdint>

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"

namespace glitch::gui {

// glitch::gui::CGUIEditBox (ctor 0x6b2608). The IGUIEditBox layer between IGUIElement
// and this class is not declared separately; its fields, if any, appear below as
// members. Its base ctor is not in these rows.
class CGUIEditBox {
public:
    std::uint8_t pad_0x000[0x158];          // +0x000..+0x157 IGUIElement layer, see layouts/gui.hpp

    std::uint8_t mouseDown;                 // +0x158 set on press and cleared on release (processMouse, onEvent)
    std::uint8_t border;                    // +0x159 ctor argument (bool border), read by draw
    std::uint8_t overrideColorEnabled;      // +0x15a serialize attribute "OverrideColorEnabled"
    std::uint8_t pad_0x15b[0x01];           // +0x15b not confirmed
    std::uint32_t selectionStart;           // +0x15c cursor index stored on press; role inferred (compared with selectionEnd)
    std::uint32_t selectionEnd;             // +0x160 cursor index stored on press and drag; role inferred
    std::uint32_t overrideColor;            // +0x164 serialize attribute "OverrideColor" (dword at +0x164, bytes +0x164..+0x167)
    std::uint32_t overrideFont;             // +0x168 guest pointer to a refcounted IGUIFont (setOverrideFont, dtor drops it)
    std::uint32_t field_0x16c;              // +0x16c not confirmed (read by draw)
    std::uint32_t skin;                     // +0x170 guest pointer to a refcounted skin (ctor takes it from the environment, dtor drops it)
    std::uint32_t cursorBlinkStart;         // +0x174 Timer::getTime at press; draw computes (now - this) % 700 for the cursor blink
    std::int32_t cursorPos;                 // +0x178 cursor position (processMouse, ctor)
    std::int32_t field_0x17c;               // +0x17c not confirmed (zeroed by setTextRect)
    std::int32_t scrollOffset;              // +0x180 scroll offset written by calculateScrollPos and subtracted from the text top in setTextRect
    std::int32_t maxChars;                  // +0x184 serialize attribute "MaxChars" (written by setMax)
    std::uint8_t wordWrap;                  // +0x188 serialize attribute "WordWrap"
    std::uint8_t multiLine;                 // +0x189 serialize attribute "MultiLine"
    std::uint8_t autoScroll;                // +0x18a serialize attribute "AutoScroll"
    std::uint8_t passwordBox;               // +0x18b serialize attribute "PasswordBox" (written by setPasswordBox)
    std::uint32_t passwordChar;             // +0x18c serialize attribute "PasswordChar" (wide char; ctor sets 42)
    std::int32_t horizontalAlign;           // +0x190 serialize attribute "HTextAlign"
    std::int32_t verticalAlign;             // +0x194 serialize attribute "VTextAlign" (ctor sets 2)
    std::uint32_t linesBegin;               // +0x198 vector begin, 72-byte entries (wide-string lines); breakText writes it
    std::uint32_t linesEnd;                 // +0x19c vector end
    std::uint32_t linesCapacity;            // +0x1a0 vector capacity (zeroed by ctor)
    std::uint32_t breakOffsetsBegin;        // +0x1a4 int array begin (freed by dtor); indexed as 4 * i
    std::uint32_t breakOffsetsEnd;          // +0x1a8 int array end (breakText pushes 4 bytes)
    std::uint32_t breakOffsetsCapacity;     // +0x1ac int array capacity (zeroed by ctor)
    core::rect<std::int32_t> textRect;      // +0x1b0 written by setTextRect as {left, top, right, bottom}
    core::rect<std::int32_t> clientRect;    // +0x1c0 ctor copies the rect argument and adjusts it by skin values
    std::uint32_t secondaryVptr0;           // +0x1d0 secondary base vtable pointer (ctor and dtor)
    std::uint32_t secondaryVptr1;           // +0x1d4 secondary base vtable pointer (ctor and dtor)
    std::int32_t field_0x1d8;               // +0x1d8 set to 1 by the ctor, role not confirmed
};

static_assert(offsetof(CGUIEditBox, mouseDown) == 0x158, "CGUIEditBox::mouseDown");
static_assert(offsetof(CGUIEditBox, border) == 0x159, "CGUIEditBox::border");
static_assert(offsetof(CGUIEditBox, overrideColorEnabled) == 0x15a, "CGUIEditBox::overrideColorEnabled");
static_assert(offsetof(CGUIEditBox, selectionStart) == 0x15c, "CGUIEditBox::selectionStart");
static_assert(offsetof(CGUIEditBox, selectionEnd) == 0x160, "CGUIEditBox::selectionEnd");
static_assert(offsetof(CGUIEditBox, overrideColor) == 0x164, "CGUIEditBox::overrideColor");
static_assert(offsetof(CGUIEditBox, overrideFont) == 0x168, "CGUIEditBox::overrideFont");
static_assert(offsetof(CGUIEditBox, field_0x16c) == 0x16c, "CGUIEditBox::field_0x16c");
static_assert(offsetof(CGUIEditBox, skin) == 0x170, "CGUIEditBox::skin");
static_assert(offsetof(CGUIEditBox, cursorBlinkStart) == 0x174, "CGUIEditBox::cursorBlinkStart");
static_assert(offsetof(CGUIEditBox, cursorPos) == 0x178, "CGUIEditBox::cursorPos");
static_assert(offsetof(CGUIEditBox, field_0x17c) == 0x17c, "CGUIEditBox::field_0x17c");
static_assert(offsetof(CGUIEditBox, scrollOffset) == 0x180, "CGUIEditBox::scrollOffset");
static_assert(offsetof(CGUIEditBox, maxChars) == 0x184, "CGUIEditBox::maxChars");
static_assert(offsetof(CGUIEditBox, wordWrap) == 0x188, "CGUIEditBox::wordWrap");
static_assert(offsetof(CGUIEditBox, multiLine) == 0x189, "CGUIEditBox::multiLine");
static_assert(offsetof(CGUIEditBox, autoScroll) == 0x18a, "CGUIEditBox::autoScroll");
static_assert(offsetof(CGUIEditBox, passwordBox) == 0x18b, "CGUIEditBox::passwordBox");
static_assert(offsetof(CGUIEditBox, passwordChar) == 0x18c, "CGUIEditBox::passwordChar");
static_assert(offsetof(CGUIEditBox, horizontalAlign) == 0x190, "CGUIEditBox::horizontalAlign");
static_assert(offsetof(CGUIEditBox, verticalAlign) == 0x194, "CGUIEditBox::verticalAlign");
static_assert(offsetof(CGUIEditBox, linesBegin) == 0x198, "CGUIEditBox::linesBegin");
static_assert(offsetof(CGUIEditBox, linesEnd) == 0x19c, "CGUIEditBox::linesEnd");
static_assert(offsetof(CGUIEditBox, linesCapacity) == 0x1a0, "CGUIEditBox::linesCapacity");
static_assert(offsetof(CGUIEditBox, breakOffsetsBegin) == 0x1a4, "CGUIEditBox::breakOffsetsBegin");
static_assert(offsetof(CGUIEditBox, breakOffsetsEnd) == 0x1a8, "CGUIEditBox::breakOffsetsEnd");
static_assert(offsetof(CGUIEditBox, breakOffsetsCapacity) == 0x1ac, "CGUIEditBox::breakOffsetsCapacity");
static_assert(offsetof(CGUIEditBox, textRect) == 0x1b0, "CGUIEditBox::textRect");
static_assert(offsetof(CGUIEditBox, clientRect) == 0x1c0, "CGUIEditBox::clientRect");
static_assert(offsetof(CGUIEditBox, secondaryVptr0) == 0x1d0, "CGUIEditBox::secondaryVptr0");
static_assert(offsetof(CGUIEditBox, secondaryVptr1) == 0x1d4, "CGUIEditBox::secondaryVptr1");
static_assert(offsetof(CGUIEditBox, field_0x1d8) == 0x1d8, "CGUIEditBox::field_0x1d8");

// glitch::gui::CGUIButton. Flags and image fields as read by serializeAttributes
// (0x6a6630) and deserializeAttributes (0x6a6bb0). The ImageRect and PressedImageRect
// fields are rect<int> values read by serialize and written by deserialize.
class CGUIButton {
public:
    std::uint8_t pad_0x000[0x158];          // +0x000..+0x157 IGUIElement layer, see layouts/gui.hpp

    std::uint8_t pressed;                   // +0x158 serialize attribute "Pressed" (read only when pushButton is set)
    std::uint8_t pushButton;                // +0x159 serialize attribute "PushButton"
    std::uint8_t useAlphaChannel;           // +0x15a serialize attribute "UseAlphaChannel"
    std::uint8_t border;                    // +0x15b serialize attribute "Border"
    std::uint8_t pad_0x15c[0x54];           // +0x15c..+0x1af not confirmed (not read by the cited bodies)
    std::uint32_t image;                    // +0x1b0 guest pointer to a refcounted texture, serialize attribute "Image"
    std::uint32_t pressedImage;             // +0x1b4 guest pointer to a refcounted texture, serialize attribute "PressedImage"
    core::rect<std::int32_t> imageRect;     // +0x1b8 serialize attribute "ImageRect"
    core::rect<std::int32_t> pressedImageRect; // +0x1c8 serialize attribute "PressedImageRect"
};

static_assert(offsetof(CGUIButton, pressed) == 0x158, "CGUIButton::pressed");
static_assert(offsetof(CGUIButton, pushButton) == 0x159, "CGUIButton::pushButton");
static_assert(offsetof(CGUIButton, useAlphaChannel) == 0x15a, "CGUIButton::useAlphaChannel");
static_assert(offsetof(CGUIButton, border) == 0x15b, "CGUIButton::border");
static_assert(offsetof(CGUIButton, image) == 0x1b0, "CGUIButton::image");
static_assert(offsetof(CGUIButton, pressedImage) == 0x1b4, "CGUIButton::pressedImage");
static_assert(offsetof(CGUIButton, imageRect) == 0x1b8, "CGUIButton::imageRect");
static_assert(offsetof(CGUIButton, pressedImageRect) == 0x1c8, "CGUIButton::pressedImageRect");

// glitch::gui::CGUICheckBox. Only the checked flag is read by the cited bodies
// (serializeAttributes 0x6a76bc, deserializeAttributes 0x6a7e94).
class CGUICheckBox {
public:
    std::uint8_t pad_0x000[0x158];          // +0x000..+0x157 IGUIElement layer, see layouts/gui.hpp
    std::uint8_t pad_0x158[0x01];           // +0x158 not confirmed
    std::uint8_t checked;                   // +0x159 serialize attribute "Checked"
};

static_assert(offsetof(CGUICheckBox, checked) == 0x159, "CGUICheckBox::checked");

}  // namespace glitch::gui

namespace glitch::scene {

// glitch::scene::CSceneNodeAnimatorCollisionResponse (serialize 0x6ca764, deserialize 0x6ca804).
class CSceneNodeAnimatorCollisionResponse {
public:
    std::uint8_t pad_0x000[0x18];           // +0x000..+0x017 ISceneNodeAnimator base region, not confirmed
    core::vector3d<float> radius;           // +0x18 serialize attribute "Radius" (the first vector, name from the serializer order)
    core::vector3d<float> gravity;          // +0x24 serialize attribute "Gravity"
    core::vector3d<float> translation;      // +0x30 serialize attribute "Translation"
    std::uint8_t pad_0x3c[0x1a];            // +0x3c..+0x55 not confirmed
    std::uint8_t animateCameraTarget;       // +0x56 serialize attribute "AnimateCameraTarget"
};

static_assert(offsetof(CSceneNodeAnimatorCollisionResponse, radius) == 0x18, "CollisionResponse::radius");
static_assert(offsetof(CSceneNodeAnimatorCollisionResponse, gravity) == 0x24, "CollisionResponse::gravity");
static_assert(offsetof(CSceneNodeAnimatorCollisionResponse, translation) == 0x30, "CollisionResponse::translation");
static_assert(offsetof(CSceneNodeAnimatorCollisionResponse, animateCameraTarget) == 0x56, "CollisionResponse::animateCameraTarget");

// glitch::scene::CAnimatedMeshSceneNode (serialize 0x6f6478, deserialize 0x6f6d98).
// The ISceneNode base is not declared, so its bytes are pads.
class CAnimatedMeshSceneNode {
public:
    std::uint8_t pad_0x000[0x110];          // +0x000..+0x10f ISceneNode base region, not confirmed
    std::uint32_t field_0x110;              // +0x110 guest pointer, role not confirmed (serialize and deserialize call through its +368 object)
    std::uint8_t pad_0x114[0x1c];           // +0x114..+0x12f not confirmed
    std::uint32_t meshHolder;               // +0x130 refcounted object (serialize and deserialize take a ref around the mesh name query)
    std::uint8_t pad_0x134[0x0c];           // +0x134..+0x13f not confirmed
    std::uint32_t framesPerSecond;          // +0x140 serialize attribute "FramesPerSecond" (attribute word; type not confirmed)
    std::uint8_t pad_0x144[0x10];           // +0x144..+0x153 not confirmed
    std::uint8_t looping;                   // +0x154 serialize attribute "Looping"
};

static_assert(offsetof(CAnimatedMeshSceneNode, field_0x110) == 0x110, "CAnimatedMeshSceneNode::field_0x110");
static_assert(offsetof(CAnimatedMeshSceneNode, meshHolder) == 0x130, "CAnimatedMeshSceneNode::meshHolder");
static_assert(offsetof(CAnimatedMeshSceneNode, framesPerSecond) == 0x140, "CAnimatedMeshSceneNode::framesPerSecond");
static_assert(offsetof(CAnimatedMeshSceneNode, looping) == 0x154, "CAnimatedMeshSceneNode::looping");

}  // namespace glitch::scene

#endif  // GLITCH_SCENE_COLLADA_GUI_LAYOUTS_VIRTUAL_GUI_MEMBERS_HPP
