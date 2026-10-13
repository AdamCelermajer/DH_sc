// Slice 31 reconstruction stand-ins (namespace glitch::collada::recon_31).
// Covers the glitch::scene node entries, constructors and attribute bodies,
// the glitch::gui element, widget and skin attribute bodies, and the camera
// and light node attribute bodies.
//
// Attribute calls go through the glitch::io::CAttributes vtable slots (see
// the decomp_vtable.py listing for CAttributes); each declaration gives the
// slot offset the original calls through. Object-level virtuals have no names
// in the category yet. The names used here are inferred from what each call
// passes and returns; the bodies that depend on them are marked partial in
// the ledger.
//
// Layout: member offsets from the original are kept as comments (byte offsets;
// the pseudocode indexes 32-bit words). The types are stand-ins, not ABI
// compatible. Nothing here is attached to the existing headers; the bodies are
// free functions because no class in the category declares these methods.
#pragma once

#include <string>

#include "glitch-core-io/core/reference_counted.hpp"
#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"
#include "glitch-scene-collada-gui/deps/intrusive_ptr.hpp"
#include "glitch-scene-collada-gui/deps/video_stand_ins.hpp"

namespace glitch::video {
class ITexture;
class CLight;
class CMaterial;
}  // namespace glitch::video

namespace glitch::io {
class IReadFile;
}  // namespace glitch::io

namespace glitch::collada::recon_31 {

struct SAttributeReadWriteOptions;
class ISceneNode;
class CGUISkin;

// ---------------------------------------------------------------------------
// glitch::io::IAttributes (stand-in). Slot offsets are the CAttributes vtable.
// ---------------------------------------------------------------------------
class IAttributes {
public:
    virtual ~IAttributes() = default;

    virtual void addInt(const char* name, int value, bool flag) = 0;                       // +0x4c
    virtual void addFloat(const char* name, float value, bool flag) = 0;                   // +0x64
    virtual void addBool(const char* name, bool value, bool flag) = 0;                     // +0xd8
    virtual void addString(const char* name, const char* value, bool flag) = 0;            // +0x7c
    virtual void addStringW(const char* name, const wchar_t* value, bool flag) = 0;        // +0x94
    virtual void addEnum(const char* name, int value, const char* const* literals, bool flag) = 0;  // +0xf4
    virtual void addColor(const char* name, glitch::video::SColor value, bool flag) = 0;   // +0x118
    virtual void addPosition2d(const char* name, glitch::core::position2d<int> value, bool flag) = 0;  // +0x1d8
    virtual void addRect(const char* name, glitch::core::rect<int> value, bool flag) = 0;  // +0x1f0
    virtual void addVector3d(const char* name, const glitch::core::vector3d<float>& value, bool flag) = 0;  // +0x1a8
    virtual void addTexture(const char* name, boost::intrusive_ptr<glitch::video::ITexture> value, bool flag) = 0;  // +0x2b0

    virtual bool existsAttribute(const char* name) const = 0;                              // +0x24
    virtual int getInt(const char* name) const = 0;                                        // +0x58
    virtual float getFloat(const char* name) const = 0;                                    // +0x70
    virtual bool getBool(const char* name) const = 0;                                      // +0xe4
    virtual std::string getString(const char* name) const = 0;                             // +0x84
    virtual std::wstring getStringW(const char* name) const = 0;                           // +0x9c
    virtual int getEnum(const char* name, const char* const* literals) const = 0;          // +0x100
    virtual glitch::video::SColor getColor(const char* name) const = 0;                    // +0x124
    virtual glitch::core::position2d<int> getPosition2d(const char* name) const = 0;       // +0x1e4
    virtual glitch::core::rect<int> getRect(const char* name) const = 0;                   // +0x1fc
    virtual glitch::core::vector3d<float> getVector3d(const char* name) const = 0;         // +0x1b4
    virtual boost::intrusive_ptr<glitch::video::ITexture> getTexture(const char* name) const = 0;  // +0x2bc

    virtual void push(const char* name) = 0;                                               // +0x30
    virtual void pop() = 0;                                                                // +0x3c
};

// Enumeration name tables passed to addEnum/getEnum and the per-slot name
// lists. The tables are data in the category and are declared here only.
extern const char* const GUIAlignmentNames[];
extern const char* const GUISkinTypeNames[];
extern const char* const GUISkinColorNames[21];
extern const char* const GUISkinSizeNames[10];
extern const char* const GUISkinTextNames[8];
extern const char* const GUISkinIconNames[23];
extern const char* const AutomaticCullingNames[];

// ---------------------------------------------------------------------------
// glitch::scene::ISceneNode (stand-in). Only the virtuals the bodies call.
// ---------------------------------------------------------------------------
class ISceneNode {
public:
    virtual ~ISceneNode() = default;

    // Object-level virtuals; names inferred from the call sites.
    virtual const glitch::core::vector3d<float>* getAbsoluteTranslation() const = 0;  // +0x38
    virtual const glitch::core::aabbox3d<float>* getBoundingBox() const = 0;          // +0x30
    virtual float getDistanceBias() const = 0;                                        // +0xd0
    virtual int getDefaultSortOrder() const = 0;                                      // +0xd8
};

// The base attribute bodies belong to ISceneNode in the category; the derived
// bodies forward to them.
void ISceneNode_serializeAttributes(const ISceneNode* self, IAttributes* attributes,
                                    SAttributeReadWriteOptions* options);
void ISceneNode_deserializeAttributes(ISceneNode* self, IAttributes* attributes,
                                      SAttributeReadWriteOptions* options);

// glitch::scene::CSkinnedMeshSceneNode: its serialise and deserialise are
// thunks to the ISceneNode bodies.
class CSkinnedMeshSceneNode : public ISceneNode {};
void CSkinnedMeshSceneNode_serializeAttributes(const CSkinnedMeshSceneNode& self, IAttributes* attributes,
                                               SAttributeReadWriteOptions* options);
void CSkinnedMeshSceneNode_deserializeAttributes(CSkinnedMeshSceneNode& self, IAttributes* attributes,
                                                 SAttributeReadWriteOptions* options);

// ---------------------------------------------------------------------------
// glitch::scene billboard and light nodes.
// ---------------------------------------------------------------------------
class IBillboardSceneNode : public ISceneNode {};

class CBillboardSceneNode : public IBillboardSceneNode {
public:
    glitch::video::SColor colorBottom;  // +0x178 (word 94), "Shade_Down"
    glitch::video::SColor colorTop;     // +0x19c (word 103), "Shade_Top"
    float width = 0.0f;                 // +0x134 (word 77)
    float height = 0.0f;                // +0x138 (word 78)

    // Object-level virtual that sets both sizes; name inferred from the call.
    virtual void setSize(const glitch::core::dimension2d<float>& size) = 0;  // vtable +0x08
};

void CBillboardSceneNode_getColor(const CBillboardSceneNode& self, glitch::video::SColor& top,
                                  glitch::video::SColor& bottom);
void CBillboardSceneNode_serializeAttributes(const CBillboardSceneNode& self, IAttributes* attributes,
                                             SAttributeReadWriteOptions* options);
void CBillboardSceneNode_deserializeAttributes(CBillboardSceneNode& self, IAttributes* attributes,
                                               SAttributeReadWriteOptions* options);

class CLightSceneNode : public ISceneNode {
public:
    glitch::video::CLight* light = nullptr;  // +0x134 (word 77)
};

// CLight::deserializeAttributes is defined with the video light code.
void CLight_deserializeAttributes(glitch::video::CLight* light, IAttributes* attributes,
                                  SAttributeReadWriteOptions* options);
void CLightSceneNode_deserializeAttributes(CLightSceneNode& self, IAttributes* attributes,
                                           SAttributeReadWriteOptions* options);

// ---------------------------------------------------------------------------
// glitch::gui::IGUIElement and its widgets.
// ---------------------------------------------------------------------------
class IGUIElement {
public:
    virtual ~IGUIElement() = default;

    // Object-level virtuals used by the attribute bodies; names inferred.
    virtual void setId(int id) = 0;                                          // +0x58
    virtual void setName(const char* name) = 0;                              // +0x60
    virtual void setText(const wchar_t* text) = 0;                           // +0x44
    virtual void setVisible(bool visible) = 0;                               // +0x30
    virtual void setEnabled(bool enabled) = 0;                               // +0x40
    virtual void updateAbsolutePosition() = 0;                               // +0x0c
    virtual void setRelativePosition(const glitch::core::rect<int>& rect) = 0;  // direct call in the original
    void setAlignment(int left, int right, int top, int bottom);               // direct call in the original
    virtual const char* getName() const = 0;                                 // +0x5c
    virtual const wchar_t* getCaption() const = 0;                           // +0x48

    int id = 0;                                              // +0x130 (word 76)
    glitch::core::rect<int> absoluteRect;                    // +0x58..+0x64 (words 22..25)
    glitch::core::position2d<int> maxSize;                   // +0x88..+0x8c (words 34..35)
    glitch::core::position2d<int> minSize;                   // +0x90..+0x94 (words 36..37)
    int tabOrder = 0;                                        // +0x138 (word 78)
    int alignLeft = 0;                                       // +0x140 (word 80)
    int alignRight = 0;                                      // +0x144 (word 81)
    int alignTop = 0;                                        // +0x148 (word 82)
    int alignBottom = 0;                                     // +0x14c (word 83)
    bool visible = true;                                     // +0x98
    bool enabled = true;                                     // +0x99
    bool noClip = false;                                     // +0x9b
    bool ignoreAllClipping = false;                          // +0x9c
    bool tabStop = false;                                    // +0x134
    bool tabGroup = false;                                   // +0x13c
};

void IGUIElement_serializeAttributes(const IGUIElement& self, IAttributes* attributes,
                                     SAttributeReadWriteOptions* options);
void IGUIElement_deserializeAttributes(IGUIElement& self, IAttributes* attributes,
                                       SAttributeReadWriteOptions* options);

class CGUIImage : public IGUIElement {
public:
    boost::intrusive_ptr<glitch::video::ITexture> texture;  // +0x15c (word 87)
    glitch::video::SColor color;                              // +0x158 (word 86)
    bool useAlphaChannel = false;                             // +0x160
    bool scaleImage = false;                                  // +0x161
};

void CGUIImage_serializeAttributes(const CGUIImage& self, IAttributes* attributes,
                                   SAttributeReadWriteOptions* options);
void CGUIImage_deserializeAttributes(CGUIImage& self, IAttributes* attributes,
                                     SAttributeReadWriteOptions* options);

class CGUIInOutFader : public IGUIElement {
public:
    int fadeMode = 0;                  // +0x160 (word 88): 1 full = first, 2 full = second
    glitch::video::SColor firstColor;  // +0x164 (word 89), set by setColor
    glitch::video::SColor secondColor; // +0x168 (word 90), set by setColor
    glitch::video::SColor fullColor;   // +0x16c (word 91), "FullColor"
    glitch::video::SColor transColor;  // +0x170 (word 92), "TransColor"
};

void CGUIInOutFader_setColor(CGUIInOutFader& self, glitch::video::SColor first, glitch::video::SColor second);
void CGUIInOutFader_serializeAttributes(const CGUIInOutFader& self, IAttributes* attributes,
                                        SAttributeReadWriteOptions* options);
void CGUIInOutFader_deserializeAttributes(CGUIInOutFader& self, IAttributes* attributes,
                                          SAttributeReadWriteOptions* options);

class CGUIMessageBox : public IGUIElement {
public:
    int buttons = 0;             // +0x184 (word 97): bit 0 okay, 1 cancel, 2 yes, 3 no
    std::wstring messageText;    // +0x188 (word 98)
    void refreshControls();      // defined with the message box code
};

void CGUIMessageBox_serializeAttributes(const CGUIMessageBox& self, IAttributes* attributes,
                                        SAttributeReadWriteOptions* options);
void CGUIMessageBox_deserializeAttributes(CGUIMessageBox& self, IAttributes* attributes,
                                          SAttributeReadWriteOptions* options);

class CGUIModalScreen : public IGUIElement {};
void CGUIModalScreen_serializeAttributes(const CGUIModalScreen& self, IAttributes* attributes,
                                         SAttributeReadWriteOptions* options);
void CGUIModalScreen_deserializeAttributes(CGUIModalScreen& self, IAttributes* attributes,
                                           SAttributeReadWriteOptions* options);

class CGUIScrollBar : public IGUIElement {
public:
    bool horizontal = false;   // +0x171 (byte 369)
    int value = 0;             // +0x174 (word 93)
    int maxValue = 0;          // +0x180 (word 96)
    int smallStep = 0;         // +0x184 (word 97)
    int largeStep = 0;         // +0x188 (word 98)

    // Object-level virtuals; names inferred from the setter order.
    virtual void setMax(int max) = 0;            // +0x80
    virtual void setValue(int value) = 0;        // +0x98
    virtual void setSmallStep(int step) = 0;     // +0x88
    virtual void setLargeStep(int step) = 0;     // +0x90
    void refreshControls();                      // defined with the scroll bar code
};

void CGUIScrollBar_serializeAttributes(const CGUIScrollBar& self, IAttributes* attributes,
                                       SAttributeReadWriteOptions* options);
void CGUIScrollBar_deserializeAttributes(CGUIScrollBar& self, IAttributes* attributes,
                                         SAttributeReadWriteOptions* options);

class CGUISpinBox : public IGUIElement {
public:
    int decimalPlaces = 0;  // +0x1b8 (word 110)

    // Object-level virtuals; names inferred from the call sites.
    virtual float getMin() const = 0;                   // +0x8c
    virtual float getMax() const = 0;                   // +0x90
    virtual float getStep() const = 0;                  // +0x9c
    virtual void setRange(float min, float max) = 0;    // +0x88
    virtual void setStep(float step) = 0;               // +0x94
    virtual void setDecimalPlaces(int places) = 0;      // +0x98
};

void CGUISpinBox_serializeAttributes(const CGUISpinBox& self, IAttributes* attributes,
                                     SAttributeReadWriteOptions* options);
void CGUISpinBox_deserializeAttributes(CGUISpinBox& self, IAttributes* attributes,
                                       SAttributeReadWriteOptions* options);

class CGUIStaticText : public IGUIElement {
public:
    bool border = false;                  // +0x164 (byte 356)
    int hTextAlign = 0;                   // +0x168 (word 360)
    int vTextAlign = 0;                   // +0x16c (word 364)
    bool overrideColorEnabled = false;    // +0x170 (byte 368)
    bool wordWrap = false;                // +0x171 (byte 369)
    bool background = false;              // +0x172 (byte 370)
    glitch::video::SColor overrideColor;  // +0x173 (bytes 371..374)

    // Object-level virtuals; names inferred from the call sites.
    virtual void setOverrideColorEnabled(bool enabled) = 0;  // +0x8c
    virtual void setWordWrap(bool wrap) = 0;                 // +0xa4
    virtual void setTextAlignment(int h, int v) = 0;         // +0xa0
};

void CGUIStaticText_serializeAttributes(const CGUIStaticText& self, IAttributes* attributes,
                                        SAttributeReadWriteOptions* options);
void CGUIStaticText_deserializeAttributes(CGUIStaticText& self, IAttributes* attributes,
                                          SAttributeReadWriteOptions* options);

class CGUITab : public IGUIElement {
public:
    int tabNumber = -1;                 // +0x158 (word 86)
    bool drawBackground = false;        // +0x15c (byte 348)
    glitch::video::SColor backColor;    // +0x15d (bytes 349..352)
    glitch::video::SColor textColor;    // +0x161 (bytes 353..356)

    // Object-level virtuals; names inferred from the call sites.
    virtual void setTabNumber(int number) = 0;                  // +0x98
    virtual void setDrawBackground(bool draw) = 0;              // +0x80
    virtual void setBackColor(glitch::video::SColor color) = 0; // +0x84
};

void CGUITab_serializeAttributes(const CGUITab& self, IAttributes* attributes,
                                 SAttributeReadWriteOptions* options);
void CGUITab_deserializeAttributes(CGUITab& self, IAttributes* attributes,
                                   SAttributeReadWriteOptions* options);

class CGUITabControl : public IGUIElement {
public:
    int activeTab = -1;              // +0x164 (word 89)
    bool border = false;             // +0x168 (byte 360)
    bool fillBackground = false;     // +0x169 (byte 361)
    int tabHeight = 0;               // +0x16c (word 91)
    int tabVerticalAlignment = 0;    // +0x170 (word 92)

    // Object-level virtuals; names inferred from the call sites.
    virtual void setActiveTab(int tab) = 0;               // +0x88
    virtual void setTabHeight(int height) = 0;            // +0x94
    virtual void setTabVerticalAlignment(int align) = 0;  // +0x9c
};

void CGUITabControl_serializeAttributes(const CGUITabControl& self, IAttributes* attributes,
                                        SAttributeReadWriteOptions* options);
void CGUITabControl_deserializeAttributes(CGUITabControl& self, IAttributes* attributes,
                                          SAttributeReadWriteOptions* options);

class CGUIListBox : public IGUIElement {
public:
    bool drawBack = false;        // +0x181 (byte 385)
    bool moveOverSelect = false;  // +0x182 (byte 386)
    bool autoScroll = false;      // +0x188 (byte 392)
    int itemCount = 0;            // derived from the item vector (+0x158 - +0x15c) / 96

    virtual void clearItems() = 0;  // +0x98
};

void CGUIListBox_serializeAttributes(const CGUIListBox& self, IAttributes* attributes,
                                     SAttributeReadWriteOptions* options);
void CGUIListBox_deserializeAttributes(CGUIListBox& self, IAttributes* attributes,
                                       SAttributeReadWriteOptions* options);

class CGUITable : public IGUIElement {};
void CGUITable_serializeAttributes(const CGUITable& self, IAttributes* attributes,
                                   SAttributeReadWriteOptions* options);
void CGUITable_deserializeAttributes(CGUITable& self, IAttributes* attributes,
                                     SAttributeReadWriteOptions* options);

// glitch::gui::CGUISkin. The colour, size, text and icon tables are flat
// arrays starting at +0x04, +0x58, +0xf4 (72-byte entries) and +0x80.
class CGUISkin {
public:
    glitch::video::SColor colors[21];  // +0x04 (21 words)
    int sizes[10];                     // +0x58 (10 words)
    std::wstring texts[8];             // +0xf4, 72-byte entries (string at entry start)
    int icons[23];                     // +0x80 (23 words)
    glitch::video::IVideoDriver* driver = nullptr;  // +0x334
    int skinType = 0;                  // +0x338
};

void CGUISkin_setColor(CGUISkin& self, unsigned int which, glitch::video::SColor color);
void CGUISkin_serializeAttributes(const CGUISkin& self, IAttributes* attributes,
                                  SAttributeReadWriteOptions* options);
void CGUISkin_deserializeAttributes(CGUISkin& self, IAttributes* attributes,
                                    SAttributeReadWriteOptions* options);

// glitch::gui::CGUIEnvironment (stand-in). The skin is reached through virtuals.
class CGUIEnvironment {
public:
    virtual ~CGUIEnvironment() = default;
    virtual CGUISkin* getSkin() const = 0;   // +0x38
    virtual void setSkin(CGUISkin* skin) = 0;  // +0x3c
    virtual CGUISkin* createSkin(int type) = 0;  // +0x40
};

void CGUIEnvironment_serializeAttributes(const CGUIEnvironment& self, IAttributes* attributes,
                                         SAttributeReadWriteOptions* options);
void CGUIEnvironment_deserializeAttributes(CGUIEnvironment& self, IAttributes* attributes,
                                           SAttributeReadWriteOptions* options);

}  // namespace glitch::collada::recon_31
