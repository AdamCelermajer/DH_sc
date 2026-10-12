// glitch::gui element and widget accessors. Each class maps only the members its
// accessors touch; the rest of the layout (base widgets, string objects, lists)
// is not reconstructed yet. Offsets are kept as comments and the code uses
// typed members only.
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"
#include "glitch-scene-collada-gui/deps/intrusive_ptr.hpp"
#include "glitch-scene-collada-gui/deps/video_stand_ins.hpp"

namespace glitch {
class IEventReceiver;
}
namespace glitch::io {
class IFileSystem;
}
namespace glitch::os {
class IOSOperator;
}
namespace glitch::video {
class ITexture;
}

namespace glitch::gui {

class IGUIElement;
class CGUIFont;
class IGUIEditBox;
class IGUISkin;
class IGUISpriteBank;

// Flags and fields of IGUIElement. The visible, enabled and sub-element flags are
// single bytes; the ID is an int.
class IGUIElement {
public:
    // 0x534f90 _ZNK6glitch3gui11IGUIElement9isVisibleEv
    bool isVisible() const;
    // 0x534f98 _ZN6glitch3gui11IGUIElement10setVisibleEb
    void setVisible(bool visible);
    // 0x534fa0 _ZNK6glitch3gui11IGUIElement12isSubElementEv
    bool isSubElement() const;
    // 0x534fa8 _ZN6glitch3gui11IGUIElement13setSubElementEb
    void setSubElement(bool subElement);
    // 0x534fb0 _ZNK6glitch3gui11IGUIElement9isEnabledEv
    bool isEnabled() const;
    // 0x534fb8 _ZN6glitch3gui11IGUIElement10setEnabledEb
    void setEnabled(bool enabled);
    // 0x534fc0 _ZNK6glitch3gui11IGUIElement7getTextEv
    const wchar_t* getText() const;
    // 0x534fc8 _ZNK6glitch3gui11IGUIElement14getToolTipTextEv
    const std::wstring& getToolTipText() const;
    // 0x534fd0 _ZNK6glitch3gui11IGUIElement5getIDEv
    int getID() const;
    // 0x534fd8 _ZN6glitch3gui11IGUIElement5setIDEi
    void setID(int id);
    // 0x534fe0 _ZNK6glitch3gui11IGUIElement7getNameEv
    const char* getName() const;
    // 0x535014 _ZNK6glitch3gui11IGUIElement11getChildrenEv
    const std::vector<IGUIElement*>& getChildren() const;
    // 0x5350a4 _ZNK6glitch3gui11IGUIElement11getTypeNameEv (partial: the name table is not mapped)
    const char* getTypeName() const;

    std::vector<IGUIElement*> children;  // +0x04, the child list (core::list in the original)
    const char* name = nullptr;          // +0x20, data pointer returned by getName
    std::uint8_t subElement = 0;         // +0x9a
    std::uint8_t visible = 0;            // +0x98
    std::uint8_t enabled = 0;            // +0x99
    const wchar_t* text = nullptr;       // +0xe4
    std::wstring toolTipText;            // +0xe8
    int id = 0;                          // +0x130
    int guiType = 0;                     // +0x154, index into the type-name table
};

// Environment fields read by the accessors. The focus element is compared by
// identity; the factory list is a vector of element-factory pointers.
// One node of the built-in font list. Only the font pointer at +0x18 is mapped;
// the links before it are not reconstructed.
struct SBuiltInFontNode {
    std::uint8_t links[0x18] = {};  // +0x00..+0x17
    CGUIFont* font = nullptr;       // +0x18
};

class CGUIEnvironment {
public:
    // 0x535598 _ZNK6glitch3gui15CGUIEnvironment8getFocusEv
    IGUIElement* getFocus() const;
    // 0x53562c _ZNK6glitch3gui15CGUIEnvironment8hasFocusEPNS0_11IGUIElementE
    bool hasFocus(IGUIElement* element) const;
    // 0x535bf8 _ZNK6glitch3gui15CGUIEnvironment14getBuiltInFontEv (partial: list links are not mapped)
    CGUIFont* getBuiltInFont() const;
    // 0x535640 _ZNK6glitch3gui15CGUIEnvironment14getVideoDriverEv
    video::IVideoDriver* getVideoDriver() const;
    // 0x535648 _ZNK6glitch3gui15CGUIEnvironment13getFileSystemEv
    const boost::intrusive_ptr<io::IFileSystem>& getFileSystem() const;
    // 0x535650 _ZNK6glitch3gui15CGUIEnvironment13getOSOperatorEv
    os::IOSOperator* getOSOperator() const;
    // 0x535768 _ZN6glitch3gui15CGUIEnvironment20setUserEventReceiverEPNS_14IEventReceiverE
    void setUserEventReceiver(IEventReceiver* receiver);
    // 0x535770 _ZNK6glitch3gui15CGUIEnvironment7getSkinEv
    IGUISkin* getSkin() const;
    // 0x5357e8 _ZNK6glitch3gui15CGUIEnvironment35getRegisteredGUIElementFactoryCountEv
    std::uint32_t getRegisteredGUIElementFactoryCount() const;

    IGUIElement* focus = nullptr;                                 // +0x1b0
    video::IVideoDriver* videoDriver = nullptr;                   // +0x1a8
    boost::intrusive_ptr<io::IFileSystem> fileSystem;             // +0x1c0
    os::IOSOperator* osOperator = nullptr;                        // +0x1c8
    IGUISkin* skin = nullptr;                                     // +0x1bc
    IEventReceiver* userEventReceiver = nullptr;                  // +0x1c4
    std::vector<void*> guiElementFactories;                       // +0x16c..+0x170
    SBuiltInFontNode* builtInFontBegin = nullptr;                 // +0x178
    SBuiltInFontNode* builtInFontEnd = nullptr;                   // +0x17c, sentinel
};

// Font accessors. The kerning values and sprite bank are the only mapped members.
class CGUIFont {
public:
    // 0x53e26c _ZNK6glitch3gui8CGUIFont13getSpriteBankEv
    IGUISpriteBank* getSpriteBank() const;
    // 0x53de4c _ZN6glitch3gui8CGUIFont15setKerningWidthEi
    void setKerningWidth(int width);
    // 0x53de54 _ZN6glitch3gui8CGUIFont16setKerningHeightEi
    void setKerningHeight(int height);
    // 0x53de5c _ZNK6glitch3gui8CGUIFont16getKerningHeightEv
    int getKerningHeight() const;
    // 0x53dc14 _ZN6glitch3gui8CGUIFont11setTrackingEi (empty in the original)
    void setTracking(int tracking);
    // 0x53dc18 _ZNK6glitch3gui8CGUIFont11getTrackingEv (returns 0 in the original)
    int getTracking() const;
    // 0x53dc0c _ZNK6glitch3gui8CGUIFont7getTypeEv (returns 0 in the original)
    std::uint32_t getType() const;
    // 0x53dbfc _ZNK6glitch3gui8CGUIFont12getDimensionEPKc
    // Returns an empty dimension in the original.
    core::dimension2d<std::uint32_t> getDimension(const char* text) const;
    // 0x53dbdc _ZN6glitch3gui8CGUIFont4drawEPKcRKNS_4core4rectIiEENS_5video6SColorEbbPS7_ (empty)
    void draw(const char* text, const core::rect<int>& area, video::SColor color, bool hcenter, bool vcenter,
              const core::rect<int>* clip);
    // 0x53dbe8 _ZN6glitch3gui8CGUIFont13drawInTextureEPKwRKN5boost13intrusive_ptrINS_5video8ITextureEEENS_4core4rectIiEENS6_6SColorEbb (empty)
    void drawInTexture(const wchar_t* text, const boost::intrusive_ptr<video::ITexture>& texture,
                       core::rect<int> area, video::SColor color, bool hcenter, bool vcenter);
    // 0x53dbec _ZN6glitch3gui8CGUIFont13drawInTextureEPKcRKN5boost13intrusive_ptrINS_5video8ITextureEEENS_4core4rectIiEENS6_6SColorEbb (empty)
    void drawInTexture(const char* text, const boost::intrusive_ptr<video::ITexture>& texture,
                       core::rect<int> area, video::SColor color, bool hcenter, bool vcenter);
    // 0x53dbf0 _ZN6glitch3gui8CGUIFont9setBorderEjNS_5video6SColorE (empty)
    void setBorder(std::uint32_t width, video::SColor color);

    IGUISpriteBank* spriteBank = nullptr;  // +0x30
    int kerningWidth = 0;                  // +0x40
    int kerningHeight = 0;                 // +0x44
};

class CGUIImage {
public:
    // 0x540514 _ZN6glitch3gui9CGUIImage18setUseAlphaChannelEb
    void setUseAlphaChannel(bool use);
    // 0x54052c _ZNK6glitch3gui9CGUIImage18isAlphaChannelUsedEv
    bool isAlphaChannelUsed() const;
    // 0x54051c _ZN6glitch3gui9CGUIImage13setScaleImageEb
    void setScaleImage(bool scale);
    // 0x540524 _ZNK6glitch3gui9CGUIImage13isImageScaledEv
    bool isImageScaled() const;

    bool useAlphaChannel = false;  // +0x160
    bool scaleImage = false;       // +0x161
};

class CGUIListBox {
public:
    // 0x541dc4 _ZNK6glitch3gui11CGUIListBox11getSelectedEv
    int getSelected() const;
    // 0x541ee4 _ZN6glitch3gui11CGUIListBox20setAutoScrollEnabledEb
    void setAutoScrollEnabled(bool enabled);
    // 0x541eec _ZNK6glitch3gui11CGUIListBox19isAutoScrollEnabledEv
    bool isAutoScrollEnabled() const;
    // 0x541c6c _ZN6glitch3gui11CGUIListBox7addItemEPKw
    // Adds the item with icon -1; the call goes through the virtual addItem(text, icon).
    std::uint32_t addItem(const wchar_t* text);
    // Vtable slot 0x88 in the original; implemented by the list box proper.
    virtual std::uint32_t addItem(const wchar_t* text, int icon) = 0;

    int selected = -1;         // +0x164
    bool autoScroll = false;   // +0x188
};

class CGUIScrollBar {
public:
    // 0x548f2c _ZNK6glitch3gui13CGUIScrollBar12getSmallStepEv
    int getSmallStep() const;
    // 0x548f34 _ZN6glitch3gui13CGUIScrollBar12setSmallStepEi
    // Steps of zero or less fall back to 10 in the original.
    void setSmallStep(int step);
    // 0x548f48 _ZNK6glitch3gui13CGUIScrollBar12getLargeStepEv
    int getLargeStep() const;
    // 0x548f50 _ZN6glitch3gui13CGUIScrollBar12setLargeStepEi
    // Steps of zero or less fall back to 50 in the original.
    void setLargeStep(int step);
    // 0x548f64 _ZNK6glitch3gui13CGUIScrollBar6getMaxEv
    int getMax() const;
    // 0x548fd0 _ZNK6glitch3gui13CGUIScrollBar6getPosEv
    int getPos() const;

    int pos = 0;        // +0x174
    int max = 0;        // +0x180
    int smallStep = 10; // +0x184
    int largeStep = 50; // +0x188
};

class CGUISpinBox {
public:
    // 0x54df38 _ZNK6glitch3gui11CGUISpinBox10getEditBoxEv
    IGUIEditBox* getEditBox() const;
    // 0x54df5c _ZNK6glitch3gui11CGUISpinBox6getMinEv
    float getMin() const;
    // 0x54df64 _ZNK6glitch3gui11CGUISpinBox6getMaxEv
    float getMax() const;
    // 0x54df6c _ZNK6glitch3gui11CGUISpinBox11getStepSizeEv
    float getStepSize() const;
    // 0x54df74 _ZN6glitch3gui11CGUISpinBox11setStepSizeEf
    void setStepSize(float step);

    IGUIEditBox* editBox = nullptr;  // +0x158
    float stepSize = 0.0f;           // +0x164
    float minValue = 0.0f;           // +0x168
    float maxValue = 0.0f;           // +0x16c
};

class CGUITab {
public:
    // 0x552040 _ZNK6glitch3gui7CGUITab9getNumberEv
    int getNumber() const;
    // 0x552048 _ZN6glitch3gui7CGUITab9setNumberEi
    void setNumber(int number);
    // 0x552050 _ZN6glitch3gui7CGUITab17setDrawBackgroundEb
    void setDrawBackground(bool draw);
    // 0x5520f0 _ZNK6glitch3gui7CGUITab19isDrawingBackgroundEv
    bool isDrawingBackground() const;

    int number = 0;               // +0x158
    bool drawBackground = false;  // +0x15c
};

class CGUIButton {
public:
    // 0x6a5df0 _ZN6glitch3gui10CGUIButton13setDrawBorderEb
    void setDrawBorder(bool draw);
    // 0x6a5eec _ZNK6glitch3gui10CGUIButton15isDrawingBorderEv
    bool isDrawingBorder() const;
    // 0x6a5ec4 _ZN6glitch3gui10CGUIButton15setIsPushButtonEb
    void setIsPushButton(bool push);
    // 0x6a5ed4 _ZNK6glitch3gui10CGUIButton12isPushButtonEv
    bool isPushButton() const;
    // 0x6a5ecc _ZNK6glitch3gui10CGUIButton9isPressedEv
    bool isPressed() const;
    // 0x6a5edc _ZN6glitch3gui10CGUIButton18setUseAlphaChannelEb
    void setUseAlphaChannel(bool use);
    // 0x6a5ee4 _ZNK6glitch3gui10CGUIButton18isAlphaChannelUsedEv
    bool isAlphaChannelUsed() const;

    std::uint8_t pressed = 0;       // +0x158
    std::uint8_t pushButton = 0;    // +0x159
    std::uint8_t useAlphaChannel = 0;  // +0x15a
    std::uint8_t drawBorder = 0;    // +0x15b
};

}  // namespace glitch::gui
