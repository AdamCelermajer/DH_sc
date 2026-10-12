// glitch::gui element and widget accessors (see gui_elements.hpp for the layout).
#include "glitch-scene-collada-gui/gui_elements.hpp"

namespace glitch::gui {

// 0x534f90 _ZNK6glitch3gui11IGUIElement9isVisibleEv
bool IGUIElement::isVisible() const {
    return visible != 0;
}

// 0x534f98 _ZN6glitch3gui11IGUIElement10setVisibleEb
void IGUIElement::setVisible(bool value) {
    visible = value ? 1 : 0;
}

// 0x534fa0 _ZNK6glitch3gui11IGUIElement12isSubElementEv
bool IGUIElement::isSubElement() const {
    return subElement != 0;
}

// 0x534fa8 _ZN6glitch3gui11IGUIElement13setSubElementEb
void IGUIElement::setSubElement(bool value) {
    subElement = value ? 1 : 0;
}

// 0x534fb0 _ZNK6glitch3gui11IGUIElement9isEnabledEv
bool IGUIElement::isEnabled() const {
    return enabled != 0;
}

// 0x534fb8 _ZN6glitch3gui11IGUIElement10setEnabledEb
void IGUIElement::setEnabled(bool value) {
    enabled = value ? 1 : 0;
}

// 0x534fc0 _ZNK6glitch3gui11IGUIElement7getTextEv
const wchar_t* IGUIElement::getText() const {
    return text;
}

// 0x534fc8 _ZNK6glitch3gui11IGUIElement14getToolTipTextEv
const std::wstring& IGUIElement::getToolTipText() const {
    return toolTipText;
}

// 0x534fd0 _ZNK6glitch3gui11IGUIElement5getIDEv
int IGUIElement::getID() const {
    return id;
}

// 0x534fd8 _ZN6glitch3gui11IGUIElement5setIDEi
void IGUIElement::setID(int newId) {
    id = newId;
}

// 0x534fe0 _ZNK6glitch3gui11IGUIElement7getNameEv
const char* IGUIElement::getName() const {
    return name;
}

// 0x535014 _ZNK6glitch3gui11IGUIElement11getChildrenEv
// Name table indexed by guiType. The original reads it from a static table
// (glitch::gui::GUIElementTypeNames) that is not mapped yet, so it stays null
// and getTypeName then returns null.
namespace {
const char* const* guiElementTypeNames = nullptr;
}  // namespace

// 0x5350a4 _ZNK6glitch3gui11IGUIElement11getTypeNameEv
const char* IGUIElement::getTypeName() const {
    return guiElementTypeNames ? guiElementTypeNames[guiType] : nullptr;
}

const std::vector<IGUIElement*>& IGUIElement::getChildren() const {
    return children;
}

// 0x535598 _ZNK6glitch3gui15CGUIEnvironment8getFocusEv
IGUIElement* CGUIEnvironment::getFocus() const {
    return focus;
}

// 0x53562c _ZNK6glitch3gui15CGUIEnvironment8hasFocusEPNS0_11IGUIElementE
bool CGUIEnvironment::hasFocus(IGUIElement* element) const {
    return focus == element;
}

// 0x535640 _ZNK6glitch3gui15CGUIEnvironment14getVideoDriverEv
video::IVideoDriver* CGUIEnvironment::getVideoDriver() const {
    return videoDriver;
}

// 0x535648 _ZNK6glitch3gui15CGUIEnvironment13getFileSystemEv
const boost::intrusive_ptr<io::IFileSystem>& CGUIEnvironment::getFileSystem() const {
    return fileSystem;
}

// 0x535650 _ZNK6glitch3gui15CGUIEnvironment13getOSOperatorEv
os::IOSOperator* CGUIEnvironment::getOSOperator() const {
    return osOperator;
}

// 0x535768 _ZN6glitch3gui15CGUIEnvironment20setUserEventReceiverEPNS_14IEventReceiverE
void CGUIEnvironment::setUserEventReceiver(IEventReceiver* receiver) {
    userEventReceiver = receiver;
}

// 0x535770 _ZNK6glitch3gui15CGUIEnvironment7getSkinEv
IGUISkin* CGUIEnvironment::getSkin() const {
    return skin;
}

// 0x5357e8 _ZNK6glitch3gui15CGUIEnvironment35getRegisteredGUIElementFactoryCountEv
// The factory list is a pointer range; its element count is (end - begin) / 4.
std::uint32_t CGUIEnvironment::getRegisteredGUIElementFactoryCount() const {
    return static_cast<std::uint32_t>(guiElementFactories.size());
}

// 0x53e26c _ZNK6glitch3gui8CGUIFont13getSpriteBankEv
IGUISpriteBank* CGUIFont::getSpriteBank() const {
    return spriteBank;
}

// 0x53de4c _ZN6glitch3gui8CGUIFont15setKerningWidthEi
void CGUIFont::setKerningWidth(int width) {
    kerningWidth = width;
}

// 0x53de54 _ZN6glitch3gui8CGUIFont16setKerningHeightEi
void CGUIFont::setKerningHeight(int height) {
    kerningHeight = height;
}

// 0x53de5c _ZNK6glitch3gui8CGUIFont16getKerningHeightEv
int CGUIFont::getKerningHeight() const {
    return kerningHeight;
}

// 0x53dc14 _ZN6glitch3gui8CGUIFont11setTrackingEi
// Empty in the original.
void CGUIFont::setTracking(int) {}

// 0x53dc18 _ZNK6glitch3gui8CGUIFont11getTrackingEv
int CGUIFont::getTracking() const {
    return 0;
}

// 0x53dc0c _ZNK6glitch3gui8CGUIFont7getTypeEv
std::uint32_t CGUIFont::getType() const {
    return 0;
}

// 0x53dbfc _ZNK6glitch3gui8CGUIFont12getDimensionEPKc
// The original writes a zero dimension through the return pointer.
core::dimension2d<std::uint32_t> CGUIFont::getDimension(const char*) const {
    return core::dimension2d<std::uint32_t>{};
}

// 0x53dbdc _ZN6glitch3gui8CGUIFont4drawEPKcRKNS_4core4rectIiEENS_5video6SColorEbbPS7_
// Empty in the original.
void CGUIFont::draw(const char*, const core::rect<int>&, video::SColor, bool, bool, const core::rect<int>*) {}

// 0x53dbe8 _ZN6glitch3gui8CGUIFont13drawInTextureEPKwRKN5boost13intrusive_ptrINS_5video8ITextureEEENS_4core4rectIiEENS6_6SColorEbb
// Empty in the original.
void CGUIFont::drawInTexture(const wchar_t*, const boost::intrusive_ptr<video::ITexture>&, core::rect<int>,
                             video::SColor, bool, bool) {}

// 0x53dbec _ZN6glitch3gui8CGUIFont13drawInTextureEPKcRKN5boost13intrusive_ptrINS_5video8ITextureEEENS_4core4rectIiEENS6_6SColorEbb
// Empty in the original.
void CGUIFont::drawInTexture(const char*, const boost::intrusive_ptr<video::ITexture>&, core::rect<int>,
                             video::SColor, bool, bool) {}

// 0x53dbf0 _ZN6glitch3gui8CGUIFont9setBorderEjNS_5video6SColorE
// Empty in the original.
void CGUIFont::setBorder(std::uint32_t, video::SColor) {}

// 0x540514 _ZN6glitch3gui9CGUIImage18setUseAlphaChannelEb
void CGUIImage::setUseAlphaChannel(bool use) {
    useAlphaChannel = use;
}

// 0x54052c _ZNK6glitch3gui9CGUIImage18isAlphaChannelUsedEv
bool CGUIImage::isAlphaChannelUsed() const {
    return useAlphaChannel;
}

// 0x54051c _ZN6glitch3gui9CGUIImage13setScaleImageEb
void CGUIImage::setScaleImage(bool scale) {
    scaleImage = scale;
}

// 0x540524 _ZNK6glitch3gui9CGUIImage13isImageScaledEv
bool CGUIImage::isImageScaled() const {
    return scaleImage;
}

// 0x541dc4 _ZNK6glitch3gui11CGUIListBox11getSelectedEv
int CGUIListBox::getSelected() const {
    return selected;
}

// 0x541ee4 _ZN6glitch3gui11CGUIListBox20setAutoScrollEnabledEb
void CGUIListBox::setAutoScrollEnabled(bool enabled) {
    autoScroll = enabled;
}

// 0x541eec _ZNK6glitch3gui11CGUIListBox19isAutoScrollEnabledEv
bool CGUIListBox::isAutoScrollEnabled() const {
    return autoScroll;
}

// 0x541c6c _ZN6glitch3gui11CGUIListBox7addItemEPKw
std::uint32_t CGUIListBox::addItem(const wchar_t* text) {
    return addItem(text, -1);
}

// 0x548f2c _ZNK6glitch3gui13CGUIScrollBar12getSmallStepEv
int CGUIScrollBar::getSmallStep() const {
    return smallStep;
}

// 0x548f34 _ZN6glitch3gui13CGUIScrollBar12setSmallStepEi
void CGUIScrollBar::setSmallStep(int step) {
    smallStep = step > 0 ? step : 10;
}

// 0x548f48 _ZNK6glitch3gui13CGUIScrollBar12getLargeStepEv
int CGUIScrollBar::getLargeStep() const {
    return largeStep;
}

// 0x548f50 _ZN6glitch3gui13CGUIScrollBar12setLargeStepEi
void CGUIScrollBar::setLargeStep(int step) {
    largeStep = step > 0 ? step : 50;
}

// 0x548f64 _ZNK6glitch3gui13CGUIScrollBar6getMaxEv
int CGUIScrollBar::getMax() const {
    return max;
}

// 0x548fd0 _ZNK6glitch3gui13CGUIScrollBar6getPosEv
int CGUIScrollBar::getPos() const {
    return pos;
}

// 0x54df38 _ZNK6glitch3gui11CGUISpinBox10getEditBoxEv
IGUIEditBox* CGUISpinBox::getEditBox() const {
    return editBox;
}

// 0x54df5c _ZNK6glitch3gui11CGUISpinBox6getMinEv
float CGUISpinBox::getMin() const {
    return minValue;
}

// 0x54df64 _ZNK6glitch3gui11CGUISpinBox6getMaxEv
float CGUISpinBox::getMax() const {
    return maxValue;
}

// 0x54df6c _ZNK6glitch3gui11CGUISpinBox11getStepSizeEv
float CGUISpinBox::getStepSize() const {
    return stepSize;
}

// 0x54df74 _ZN6glitch3gui11CGUISpinBox11setStepSizeEf
void CGUISpinBox::setStepSize(float step) {
    stepSize = step;
}

// 0x552040 _ZNK6glitch3gui7CGUITab9getNumberEv
int CGUITab::getNumber() const {
    return number;
}

// 0x552048 _ZN6glitch3gui7CGUITab9setNumberEi
void CGUITab::setNumber(int value) {
    number = value;
}

// 0x552050 _ZN6glitch3gui7CGUITab17setDrawBackgroundEb
void CGUITab::setDrawBackground(bool draw) {
    drawBackground = draw;
}

// 0x5520f0 _ZNK6glitch3gui7CGUITab19isDrawingBackgroundEv
bool CGUITab::isDrawingBackground() const {
    return drawBackground;
}

// 0x6a5df0 _ZN6glitch3gui10CGUIButton13setDrawBorderEb
void CGUIButton::setDrawBorder(bool draw) {
    drawBorder = draw ? 1 : 0;
}

// 0x6a5eec _ZNK6glitch3gui10CGUIButton15isDrawingBorderEv
bool CGUIButton::isDrawingBorder() const {
    return drawBorder != 0;
}

// 0x6a5ec4 _ZN6glitch3gui10CGUIButton15setIsPushButtonEb
void CGUIButton::setIsPushButton(bool push) {
    pushButton = push ? 1 : 0;
}

// 0x6a5ed4 _ZNK6glitch3gui10CGUIButton12isPushButtonEv
bool CGUIButton::isPushButton() const {
    return pushButton != 0;
}

// 0x6a5ecc _ZNK6glitch3gui10CGUIButton9isPressedEv
bool CGUIButton::isPressed() const {
    return pressed != 0;
}

// 0x6a5edc _ZN6glitch3gui10CGUIButton18setUseAlphaChannelEb
void CGUIButton::setUseAlphaChannel(bool use) {
    useAlphaChannel = use ? 1 : 0;
}

// 0x6a5ee4 _ZNK6glitch3gui10CGUIButton18isAlphaChannelUsedEv
bool CGUIButton::isAlphaChannelUsed() const {
    return useAlphaChannel != 0;
}

// 0x535bf8 _ZNK6glitch3gui15CGUIEnvironment14getBuiltInFontEv
CGUIFont* CGUIEnvironment::getBuiltInFont() const {
    if (builtInFontBegin == builtInFontEnd) {
        return nullptr;
    }
    return builtInFontBegin->font;
}

}  // namespace glitch::gui
