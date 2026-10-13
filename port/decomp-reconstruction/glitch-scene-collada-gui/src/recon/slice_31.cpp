// Slice 31 reconstruction (see include/glitch-scene-collada-gui/recon/slice_31.hpp).
// Each function keeps the original address and mangled symbol in its comment.
// Attribute calls go through the IAttributes stand-in; the slot offsets are
// listed in the header.

#include "glitch-scene-collada-gui/recon/slice_31.hpp"

#include "glitch-video/material.hpp"
#include "glitch-video/texture.hpp"

namespace glitch::collada::recon_31 {

// 0x35a9d8 _ZNK6glitch7collada21CSkinnedMeshSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Thunk: forwards to the ISceneNode body.
void CSkinnedMeshSceneNode_serializeAttributes(const CSkinnedMeshSceneNode& self, IAttributes* attributes,
                                               SAttributeReadWriteOptions* options) {
    ISceneNode_serializeAttributes(&self, attributes, options);
}

// 0x35a9ec _ZN6glitch7collada21CSkinnedMeshSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Thunk: forwards to the ISceneNode body.
void CSkinnedMeshSceneNode_deserializeAttributes(CSkinnedMeshSceneNode& self, IAttributes* attributes,
                                                 SAttributeReadWriteOptions* options) {
    ISceneNode_deserializeAttributes(&self, attributes, options);
}

// ---------------------------------------------------------------------------
// glitch::gui::IGUIElement
// ---------------------------------------------------------------------------

// 0x5350bc _ZNK6glitch3gui11IGUIElement19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void IGUIElement_serializeAttributes(const IGUIElement& self, IAttributes* attributes,
                                     SAttributeReadWriteOptions* options) {
    (void)options;
    attributes->addInt("Id", self.id, false);
    attributes->addString("Name", self.getName(), false);
    attributes->addString("Caption", self.getCaption(), false);
    attributes->addRect("Rect", self.absoluteRect, false);
    attributes->addPosition2d("MinSize", self.minSize, false);
    attributes->addPosition2d("MaxSize", self.maxSize, false);
    attributes->addBool("NoClip", self.noClip, false);
    attributes->addBool("IgnoreAllClipping", self.ignoreAllClipping, false);
    attributes->addEnum("LeftAlign", self.alignLeft, GUIAlignmentNames, false);
    attributes->addEnum("RightAlign", self.alignRight, GUIAlignmentNames, false);
    attributes->addEnum("TopAlign", self.alignTop, GUIAlignmentNames, false);
    attributes->addEnum("BottomAlign", self.alignBottom, GUIAlignmentNames, false);
    attributes->addBool("Visible", self.visible, false);
    attributes->addBool("Enabled", self.enabled, false);
    attributes->addBool("TabStop", self.tabStop, false);
    attributes->addBool("TabGroup", self.tabGroup, false);
    attributes->addInt("TabOrder", self.tabOrder, false);
}

// 0x539838 _ZN6glitch3gui11IGUIElement21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void IGUIElement_deserializeAttributes(IGUIElement& self, IAttributes* attributes,
                                       SAttributeReadWriteOptions* options) {
    (void)options;
    self.setId(attributes->getInt("Id"));
    self.setName(attributes->getString("Name").c_str());
    self.setText(attributes->getStringW("Caption").c_str());
    self.setVisible(attributes->getBool("Visible"));
    self.setEnabled(attributes->getBool("Enabled"));
    self.tabStop = attributes->getBool("TabStop");
    self.tabGroup = attributes->getBool("TabGroup");
    self.tabOrder = attributes->getInt("TabOrder");

    self.maxSize = attributes->getPosition2d("MaxSize");
    self.updateAbsolutePosition();

    const glitch::core::position2d<int> minSize = attributes->getPosition2d("MinSize");
    self.minSize = minSize;
    if (minSize.X <= 0) {
        self.minSize.X = 1;
    }
    if (minSize.Y <= 0) {
        self.minSize.Y = 1;
    }
    self.updateAbsolutePosition();

    self.noClip = attributes->getBool("NoClip");
    self.ignoreAllClipping = attributes->getBool("IgnoreAllClipping");
    const int left = attributes->getEnum("LeftAlign", GUIAlignmentNames);
    const int right = attributes->getEnum("RightAlign", GUIAlignmentNames);
    const int top = attributes->getEnum("TopAlign", GUIAlignmentNames);
    const int bottom = attributes->getEnum("BottomAlign", GUIAlignmentNames);
    self.setAlignment(left, right, top, bottom);
    self.setRelativePosition(attributes->getRect("Rect"));
}

// ---------------------------------------------------------------------------
// glitch::gui::CGUIImage
// ---------------------------------------------------------------------------

// 0x540534 _ZNK6glitch3gui9CGUIImage19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CGUIImage_serializeAttributes(const CGUIImage& self, IAttributes* attributes,
                                   SAttributeReadWriteOptions* options) {
    IGUIElement_serializeAttributes(self, attributes, options);
    attributes->addTexture("Texture", self.texture, false);
    attributes->addBool("UseAlphaChannel", self.useAlphaChannel, false);
    attributes->addColor("Color", self.color, false);
    attributes->addBool("ScaleImage", self.scaleImage, false);
}

// 0x540c0c _ZN6glitch3gui9CGUIImage21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Partial: the original sets the texture, alpha, colour and scale through
// virtual setters that are not named yet; the members are assigned directly.
void CGUIImage_deserializeAttributes(CGUIImage& self, IAttributes* attributes,
                                     SAttributeReadWriteOptions* options) {
    IGUIElement_deserializeAttributes(self, attributes, options);
    self.texture = attributes->getTexture("Texture");
    self.useAlphaChannel = attributes->getBool("UseAlphaChannel");
    self.color = attributes->getColor("Color");
    self.scaleImage = attributes->getBool("ScaleImage");
}

// ---------------------------------------------------------------------------
// glitch::gui::CGUIInOutFader
// ---------------------------------------------------------------------------

// 0x54124c _ZN6glitch3gui14CGUIInOutFader8setColorENS_5video6SColorES3_
void CGUIInOutFader_setColor(CGUIInOutFader& self, glitch::video::SColor first, glitch::video::SColor second) {
    self.firstColor = first;
    self.secondColor = second;
    if (self.fadeMode == 2) {
        self.fullColor = second;
        self.transColor = first;
    } else if (self.fadeMode == 1) {
        self.fullColor = first;
        self.transColor = second;
    }
}

// 0x5412cc _ZNK6glitch3gui14CGUIInOutFader19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CGUIInOutFader_serializeAttributes(const CGUIInOutFader& self, IAttributes* attributes,
                                        SAttributeReadWriteOptions* options) {
    IGUIElement_serializeAttributes(self, attributes, options);
    attributes->addColor("FullColor", self.fullColor, false);
    attributes->addColor("TransColor", self.transColor, false);
}

// 0x541984 _ZN6glitch3gui14CGUIInOutFader21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CGUIInOutFader_deserializeAttributes(CGUIInOutFader& self, IAttributes* attributes,
                                          SAttributeReadWriteOptions* options) {
    IGUIElement_deserializeAttributes(self, attributes, options);
    self.fullColor = attributes->getColor("FullColor");
    self.transColor = attributes->getColor("TransColor");
}

// ---------------------------------------------------------------------------
// glitch::gui::CGUIMessageBox
// ---------------------------------------------------------------------------

// 0x546f1c _ZNK6glitch3gui14CGUIMessageBox19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CGUIMessageBox_serializeAttributes(const CGUIMessageBox& self, IAttributes* attributes,
                                        SAttributeReadWriteOptions* options) {
    IGUIElement_serializeAttributes(self, attributes, options);
    attributes->addBool("OkayButton", (self.buttons & 1) != 0, false);
    attributes->addBool("CancelButton", ((self.buttons >> 1) & 1) != 0, false);
    attributes->addBool("YesButton", ((self.buttons >> 2) & 1) != 0, false);
    attributes->addBool("NoButton", ((self.buttons >> 3) & 1) != 0, false);
    attributes->addString("MessageText", self.messageText.c_str(), false);
}

// 0x547f28 _ZN6glitch3gui14CGUIMessageBox21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CGUIMessageBox_deserializeAttributes(CGUIMessageBox& self, IAttributes* attributes,
                                          SAttributeReadWriteOptions* options) {
    int buttons = attributes->getBool("OkayButton") ? 1 : 0;
    if (attributes->getBool("CancelButton")) {
        buttons |= 2;
    }
    if (attributes->getBool("YesButton")) {
        buttons |= 4;
    }
    if (attributes->getBool("NoButton")) {
        buttons |= 8;
    }
    self.buttons = buttons;
    self.messageText = attributes->getStringW("MessageText");
    IGUIElement_deserializeAttributes(self, attributes, options);
    self.refreshControls();
}

// ---------------------------------------------------------------------------
// glitch::gui::CGUIModalScreen (both bodies are empty in the original)
// ---------------------------------------------------------------------------

// 0x5482a0 _ZNK6glitch3gui15CGUIModalScreen19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CGUIModalScreen_serializeAttributes(const CGUIModalScreen& self, IAttributes* attributes,
                                         SAttributeReadWriteOptions* options) {
    (void)self;
    (void)attributes;
    (void)options;
}

// 0x5482a4 _ZN6glitch3gui15CGUIModalScreen21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CGUIModalScreen_deserializeAttributes(CGUIModalScreen& self, IAttributes* attributes,
                                           SAttributeReadWriteOptions* options) {
    (void)self;
    (void)attributes;
    (void)options;
}

// ---------------------------------------------------------------------------
// glitch::gui::CGUIScrollBar
// ---------------------------------------------------------------------------

// 0x548fd8 _ZNK6glitch3gui13CGUIScrollBar19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CGUIScrollBar_serializeAttributes(const CGUIScrollBar& self, IAttributes* attributes,
                                       SAttributeReadWriteOptions* options) {
    IGUIElement_serializeAttributes(self, attributes, options);
    attributes->addBool("Horizontal", self.horizontal, false);
    attributes->addInt("Value", self.value, false);
    attributes->addInt("Max", self.maxValue, false);
    attributes->addInt("SmallStep", self.smallStep, false);
    attributes->addInt("LargeStep", self.largeStep, false);
}

// 0x54a12c _ZN6glitch3gui13CGUIScrollBar21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Partial: the setters are virtual and their names are inferred from the call order.
void CGUIScrollBar_deserializeAttributes(CGUIScrollBar& self, IAttributes* attributes,
                                         SAttributeReadWriteOptions* options) {
    IGUIElement_deserializeAttributes(self, attributes, options);
    self.horizontal = attributes->getBool("Horizontal");
    self.setMax(attributes->getInt("Max"));
    self.setValue(attributes->getInt("Value"));
    self.setSmallStep(attributes->getInt("SmallStep"));
    self.setLargeStep(attributes->getInt("LargeStep"));
    self.refreshControls();
}

// ---------------------------------------------------------------------------
// glitch::gui::CGUISkin
// ---------------------------------------------------------------------------

// 0x54aa64 _ZN6glitch3gui8CGUISkin8setColorENS0_18EGUI_DEFAULT_COLORENS_5video6SColorE
void CGUISkin_setColor(CGUISkin& self, unsigned int which, glitch::video::SColor color) {
    if (which <= 20) {
        self.colors[which] = color;
    }
}

// 0x54ac34 _ZNK6glitch3gui8CGUISkin19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CGUISkin_serializeAttributes(const CGUISkin& self, IAttributes* attributes,
                                  SAttributeReadWriteOptions* options) {
    (void)options;
    for (int i = 0; i != 21; ++i) {
        attributes->addColor(GUISkinColorNames[i], self.colors[i], false);
    }
    for (int j = 0; j != 10; ++j) {
        attributes->addInt(GUISkinSizeNames[j], self.sizes[j], false);
    }
    for (int k = 0; k != 8; ++k) {
        attributes->addString(GUISkinTextNames[k], self.texts[k].c_str(), false);
    }
    for (int m = 0; m != 23; ++m) {
        attributes->addInt(GUISkinIconNames[m], self.icons[m], false);
    }
}

// 0x54adc0 _ZN6glitch3gui8CGUISkin21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CGUISkin_deserializeAttributes(CGUISkin& self, IAttributes* attributes,
                                    SAttributeReadWriteOptions* options) {
    (void)options;
    for (int i = 0; i != 21; ++i) {
        self.colors[i] = attributes->getColor(GUISkinColorNames[i]);
    }
    for (int j = 0; j != 10; ++j) {
        self.sizes[j] = attributes->getInt(GUISkinSizeNames[j]);
    }
    for (int k = 0; k != 8; ++k) {
        self.texts[k] = attributes->getStringW(GUISkinTextNames[k]);
    }
    for (int m = 0; m != 23; ++m) {
        self.icons[m] = attributes->getInt(GUISkinIconNames[m]);
    }
}

// ---------------------------------------------------------------------------
// glitch::gui::CGUISpinBox
// ---------------------------------------------------------------------------

// 0x54e044 _ZNK6glitch3gui11CGUISpinBox19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CGUISpinBox_serializeAttributes(const CGUISpinBox& self, IAttributes* attributes,
                                     SAttributeReadWriteOptions* options) {
    IGUIElement_serializeAttributes(self, attributes, options);
    attributes->addFloat("Min", self.getMin(), false);
    attributes->addFloat("Max", self.getMax(), false);
    attributes->addFloat("Step", self.getStep(), false);
    attributes->addInt("DecimalPlaces", self.decimalPlaces, false);
}

// 0x54f0c0 _ZN6glitch3gui11CGUISpinBox21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Partial: the range, step and decimal setters are virtual; names inferred.
void CGUISpinBox_deserializeAttributes(CGUISpinBox& self, IAttributes* attributes,
                                       SAttributeReadWriteOptions* options) {
    IGUIElement_deserializeAttributes(self, attributes, options);
    self.setRange(attributes->getFloat("Min"), attributes->getFloat("Max"));
    self.setStep(attributes->getFloat("Step"));
    self.setDecimalPlaces(attributes->getInt("DecimalPlaces"));
}

// ---------------------------------------------------------------------------
// glitch::gui::CGUIStaticText
// ---------------------------------------------------------------------------

// 0x5507e4 _ZNK6glitch3gui14CGUIStaticText19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CGUIStaticText_serializeAttributes(const CGUIStaticText& self, IAttributes* attributes,
                                        SAttributeReadWriteOptions* options) {
    IGUIElement_serializeAttributes(self, attributes, options);
    attributes->addBool("Border", self.border, false);
    attributes->addBool("OverrideColorEnabled", self.overrideColorEnabled, false);
    attributes->addBool("WordWrap", self.wordWrap, false);
    attributes->addBool("Background", self.background, false);
    attributes->addColor("OverrideColor", self.overrideColor, false);
    attributes->addEnum("HTextAlign", self.hTextAlign, GUIAlignmentNames, false);
    attributes->addEnum("VTextAlign", self.vTextAlign, GUIAlignmentNames, false);
}

// 0x550dcc _ZN6glitch3gui14CGUIStaticText21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Partial: the override flag, word wrap and alignment go through virtual setters
// whose names are inferred; border, background and colour are assigned directly.
void CGUIStaticText_deserializeAttributes(CGUIStaticText& self, IAttributes* attributes,
                                          SAttributeReadWriteOptions* options) {
    IGUIElement_deserializeAttributes(self, attributes, options);
    self.border = attributes->getBool("Border");
    self.overrideColor = attributes->getColor("OverrideColor");
    self.setOverrideColorEnabled(attributes->getBool("OverrideColorEnabled"));
    self.setWordWrap(attributes->getBool("WordWrap"));
    self.background = attributes->getBool("Background");
    const int h = attributes->getEnum("HTextAlign", GUIAlignmentNames);
    const int v = attributes->getEnum("VTextAlign", GUIAlignmentNames);
    self.setTextAlignment(h, v);
}

// ---------------------------------------------------------------------------
// glitch::gui::CGUITab and CGUITabControl
// ---------------------------------------------------------------------------

// 0x552140 _ZNK6glitch3gui7CGUITab19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CGUITab_serializeAttributes(const CGUITab& self, IAttributes* attributes,
                                 SAttributeReadWriteOptions* options) {
    IGUIElement_serializeAttributes(self, attributes, options);
    attributes->addInt("TabNumber", self.tabNumber, false);
    attributes->addBool("DrawBackground", self.drawBackground, false);
    attributes->addColor("BackColor", self.backColor, false);
    attributes->addColor("TextColor", self.textColor, false);
}

// 0x553db0 _ZN6glitch3gui7CGUITab21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Partial: the text colour value is read and dropped in the original, and the
// parent tab-control hook (parent type 18) is not reconstructed.
void CGUITab_deserializeAttributes(CGUITab& self, IAttributes* attributes,
                                   SAttributeReadWriteOptions* options) {
    IGUIElement_deserializeAttributes(self, attributes, options);
    self.setTabNumber(attributes->getInt("TabNumber"));
    self.setDrawBackground(attributes->getBool("DrawBackground"));
    self.setBackColor(attributes->getColor("BackColor"));
    (void)attributes->getColor("TextColor");
}

// 0x55295c _ZNK6glitch3gui14CGUITabControl19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CGUITabControl_serializeAttributes(const CGUITabControl& self, IAttributes* attributes,
                                        SAttributeReadWriteOptions* options) {
    IGUIElement_serializeAttributes(self, attributes, options);
    attributes->addInt("ActiveTab", self.activeTab, false);
    attributes->addBool("Border", self.border, false);
    attributes->addBool("FillBackground", self.fillBackground, false);
    attributes->addInt("TabHeight", self.tabHeight, false);
    attributes->addEnum("TabVerticalAlignment", self.tabVerticalAlignment, GUIAlignmentNames, false);
}

// 0x553ca4 _ZN6glitch3gui14CGUITabControl21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Partial: setter names are inferred from the call order.
void CGUITabControl_deserializeAttributes(CGUITabControl& self, IAttributes* attributes,
                                          SAttributeReadWriteOptions* options) {
    self.border = attributes->getBool("Border");
    self.fillBackground = attributes->getBool("FillBackground");
    self.activeTab = -1;
    self.setTabHeight(attributes->getInt("TabHeight"));
    IGUIElement_deserializeAttributes(self, attributes, options);
    self.setActiveTab(attributes->getInt("ActiveTab"));
    self.setTabVerticalAlignment(attributes->getEnum("TabVerticalAlignment", GUIAlignmentNames));
}

// ---------------------------------------------------------------------------
// glitch::gui::CGUIListBox and CGUITable
// ---------------------------------------------------------------------------

// 0x5449b8 _ZNK6glitch3gui11CGUIListBox19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Partial: the per-item loop (item label, four override colours) is not reconstructed.
void CGUIListBox_serializeAttributes(const CGUIListBox& self, IAttributes* attributes,
                                     SAttributeReadWriteOptions* options) {
    IGUIElement_serializeAttributes(self, attributes, options);
    attributes->addBool("DrawBack", self.drawBack, false);
    attributes->addBool("MoveOverSelect", self.moveOverSelect, false);
    attributes->addBool("AutoScroll", self.autoScroll, false);
    attributes->addInt("ItemCount", self.itemCount, false);
}

// 0x544d7c _ZN6glitch3gui11CGUIListBox21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Partial: the per-item loop is not reconstructed; the list is cleared first.
void CGUIListBox_deserializeAttributes(CGUIListBox& self, IAttributes* attributes,
                                       SAttributeReadWriteOptions* options) {
    self.clearItems();
    self.drawBack = attributes->getBool("DrawBack");
    self.moveOverSelect = attributes->getBool("MoveOverSelect");
    self.autoScroll = attributes->getBool("AutoScroll");
    IGUIElement_deserializeAttributes(self, attributes, options);
    self.itemCount = attributes->getInt("ItemCount");
}

// ---------------------------------------------------------------------------
// glitch::gui::CGUIEnvironment
// ---------------------------------------------------------------------------

// 0x53595c _ZNK6glitch3gui15CGUIEnvironment19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Partial: the original hands the skin to its virtual serialise through a slot
// that is not named; CGUISkin_serializeAttributes is used directly.
void CGUIEnvironment_serializeAttributes(const CGUIEnvironment& self, IAttributes* attributes,
                                         SAttributeReadWriteOptions* options) {
    CGUISkin* skin = self.getSkin();
    if (skin == nullptr) {
        return;
    }
    attributes->addEnum("Skin", skin->skinType, GUISkinTypeNames, false);
    CGUISkin_serializeAttributes(*skin, attributes, options);
}

// 0x535a08 _ZN6glitch3gui15CGUIEnvironment21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Partial: the skin is replaced when its type differs and then read. The
// original also resets the environment's root, focus and hover state
// afterwards; that part is not reconstructed.
void CGUIEnvironment_deserializeAttributes(CGUIEnvironment& self, IAttributes* attributes,
                                           SAttributeReadWriteOptions* options) {
    if (attributes->existsAttribute("Skin")) {
        CGUISkin* current = self.getSkin();
        const int type = attributes->getEnum("Skin", GUISkinTypeNames);
        if (current == nullptr || current->skinType != type) {
            self.setSkin(self.createSkin(type));
        }
        if (CGUISkin* skin = self.getSkin()) {
            CGUISkin_deserializeAttributes(*skin, attributes, options);
        }
    }
}

// ---------------------------------------------------------------------------
// glitch::scene::CBillboardSceneNode
// ---------------------------------------------------------------------------

// 0x580bec _ZNK6glitch5scene19CBillboardSceneNode8getColorERNS_5video6SColorES4_
void CBillboardSceneNode_getColor(const CBillboardSceneNode& self, glitch::video::SColor& top,
                                  glitch::video::SColor& bottom) {
    bottom = self.colorBottom;
    top = self.colorTop;
}

// 0x580c48 _ZNK6glitch5scene19CBillboardSceneNode19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CBillboardSceneNode_serializeAttributes(const CBillboardSceneNode& self, IAttributes* attributes,
                                             SAttributeReadWriteOptions* options) {
    ISceneNode_serializeAttributes(&self, attributes, options);
    attributes->addFloat("Width", self.width, false);
    attributes->addFloat("Height", self.height, false);
    attributes->addColor("Shade_Top", self.colorTop, false);
    attributes->addColor("Shade_Down", self.colorBottom, false);
}

// 0x580cf8 _ZN6glitch5scene19CBillboardSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CBillboardSceneNode_deserializeAttributes(CBillboardSceneNode& self, IAttributes* attributes,
                                               SAttributeReadWriteOptions* options) {
    ISceneNode_deserializeAttributes(&self, attributes, options);
    self.width = attributes->getFloat("Width");
    self.height = attributes->getFloat("Height");
    self.colorTop = attributes->getColor("Shade_Top");
    self.colorBottom = attributes->getColor("Shade_Down");
    self.setSize(glitch::core::dimension2d<float>{self.width, self.height});
}

// ---------------------------------------------------------------------------
// glitch::scene::CLightSceneNode
// ---------------------------------------------------------------------------

// 0x583d88 _ZN6glitch5scene15CLightSceneNode21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
void CLightSceneNode_deserializeAttributes(CLightSceneNode& self, IAttributes* attributes,
                                           SAttributeReadWriteOptions* options) {
    ISceneNode_deserializeAttributes(&self, attributes, options);
    attributes->push("Light");
    CLight_deserializeAttributes(self.light, attributes, options);
    attributes->pop();
}

}  // namespace glitch::collada::recon_31
