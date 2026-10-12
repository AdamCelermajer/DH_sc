// Slice 00 reconstruction (see include/glitch-scene-collada-gui/recon/slice_00.hpp).
// Each function keeps the original address and mangled symbol in its comment.

#include "glitch-scene-collada-gui/recon/slice_00.hpp"

#include <cstring>
#include <cwchar>

namespace glitch::collada::recon_00 {

// ---------------------------------------------------------------------------
// glitch::debugger::CTweakable
// ---------------------------------------------------------------------------

// 0x32d088 _ZN6glitch8debugger10CTweakable8SMappingC1ENS_2io16E_ATTRIBUTE_TYPEEPv
TweakableMapping::TweakableMapping(int t, void* v) : type(t), value(v) {}

// 0x320294 _ZN6glitch8debugger10CTweakable8SMappingaSERKS2_
TweakableMapping& TweakableMapping::operator=(const TweakableMapping& other) {
    type = other.type;    // +0x00
    value = other.value;  // +0x04
    name = other.name;    // +0x08
    text = other.text;    // +0x20
    return *this;
}

// 0x32cfc8 _ZN6glitch8debugger10CTweakableC2EPNS_7IDeviceE
// Members start empty, as the original sets its two empty string blocks and
// its empty map and list.
Tweakable::Tweakable(void* dev) : device(dev) {}

// 0x32a088 _ZN6glitch8debugger10CTweakableD1Ev, D2 at 0x404ab0 shares this body.
// Original clears the map, frees the group names and the xml string. Those are
// member destructors here; the map is cleared explicitly, as the original does.
Tweakable::~Tweakable() {
    mappings.clear();
    names.clear();
}

// 0x31f420 _ZN6glitch8debugger10CTweakable10onSetValueERKSs (empty body)
void Tweakable::onSetValue(const std::string&) {}

// 0x31f410 _ZNK6glitch8debugger10CTweakable15getInstanceNameEv
int Tweakable::getInstanceName() const {
    return instanceName;
}

// 0x31f418 _ZNK6glitch8debugger10CTweakable6getXMLEv
// Partial: the original returns the member address reinterpreted as char*.
const std::string& Tweakable::getXML() const {
    return xmlName;
}

// 0x32bb98 _ZN6glitch8debugger10CTweakable20registerVariableNameEPKc
void Tweakable::registerVariableName(const char* name) {
    if (mappings.find(name) == mappings.end()) {
        names.emplace_back(name);
    }
}

// 0x32eeb8 _ZN6glitch8debugger10CTweakable16registerVariableEPKcRNS_5video6SColorE
void Tweakable::registerVariable(const char* name, glitch::video::SColor& value) {
    registerVariableName(name);
    mappings[name] = TweakableMapping(5, &value);
}

// 0x32ef4c _ZN6glitch8debugger10CTweakable16registerVariableEPKcRf
void Tweakable::registerVariable(const char* name, float& value) {
    registerVariableName(name);
    mappings[name] = TweakableMapping(1, &value);
}

// 0x32efe0 _ZN6glitch8debugger10CTweakable16registerVariableEPKcRi
void Tweakable::registerVariable(const char* name, int& value) {
    registerVariableName(name);
    mappings[name] = TweakableMapping(0, &value);
}

// 0x328994 _ZN6glitch8debugger10CTweakable11writeValuesEPNS_2io10IWriteFileE
// Partial: the XML writer (writeElement / writeClosingTag) is not mapped; the
// group walk writeGroup(this, group, writer, 0) is not reproduced.
void Tweakable::writeValues(void* writeFile) {
    (void)writeFile;
}

// 0x32b3b4 _ZN6glitch8debugger10CTweakable8setValueERNS_2io11CAttributesEi
// Partial: the 10-way type switch reads typed values through CAttributes
// virtuals (slots +16, +92, +116, +140, +232, +296, +320, +416, +440, +464),
// none of which are mapped. Returns null for now.
void* Tweakable::setValue(void* attributes, int index) {
    (void)attributes;
    (void)index;
    return nullptr;
}

// 0x32b650 _ZN6glitch8debugger10CTweakable9loadGroupERNS_2io11CAttributesEb.clone.25
// Partial: recursion over the attribute group children; the attribute walker
// (slots +60, +64, +68) is not mapped.
int Tweakable::loadGroup(void* attributes, bool flag) {
    (void)attributes;
    (void)flag;
    return 0;
}

// 0x32b6b8 _ZN6glitch8debugger10CTweakable7loadXMLEPKcb
// Partial: CReadFile / CXMLAttributesReader are not mapped; the flow is:
// open file, read attributes, store the root group name, loadGroup, drop reader.
void Tweakable::loadXML(const char* file, bool flag) {
    (void)file;
    (void)flag;
}

// ---------------------------------------------------------------------------
// glitch::scene::SViewFrustum and culling helpers
// ---------------------------------------------------------------------------

// 0x35bec8 _ZNK6glitch5scene12SViewFrustum10intersectsERKNS_4core8aabbox3dIfEE
bool ViewFrustum::intersects(const glitch::core::aabbox3d<float>& b) const {
    // Bounding-box overlap against the frustum box (a1[27..32]).
    if (!(box.MinEdge.X <= b.MaxEdge.X && box.MinEdge.Y <= b.MaxEdge.Y &&
          box.MinEdge.Z <= b.MaxEdge.Z && box.MaxEdge.X >= b.MinEdge.X &&
          box.MaxEdge.Y >= b.MinEdge.Y && box.MaxEdge.Z >= b.MinEdge.Z)) {
        return false;
    }
    for (const FrustumPlane& p : planes) {
        const float x = p.normal[0] < 0.0f ? b.MaxEdge.X : b.MinEdge.X;
        const float y = p.normal[1] >= 0.0f ? b.MinEdge.Y : b.MaxEdge.Y;
        const float z = p.normal[2] >= 0.0f ? b.MinEdge.Z : b.MaxEdge.Z;
        const float dist = p.normal[0] * x + p.normal[1] * y + p.normal[2] * z + p.d;
        if (dist > 0.0f) {
            return false;
        }
    }
    return true;
}

// 0x50cacc _ZNK6glitch5scene12SViewFrustum25intersectsWithoutBoxTest3ERKNS_4core8aabbox3dIfEE
// Tests planes 2, 3 and 0 only, as the original does (no bounding-box test).
bool ViewFrustum::intersectsWithoutBoxTest3(const glitch::core::aabbox3d<float>& b) const {
    auto outside = [&b](const FrustumPlane& p) {
        const float x = p.normal[0] < 0.0f ? b.MaxEdge.X : b.MinEdge.X;
        const float y = p.normal[1] < 0.0f ? b.MaxEdge.Y : b.MinEdge.Y;
        const float z = p.normal[2] < 0.0f ? b.MaxEdge.Z : b.MinEdge.Z;
        return p.normal[0] * x + p.normal[1] * y + p.normal[2] * z + p.d > 0.0f;
    };
    if (outside(planes[2])) {
        return false;
    }
    if (outside(planes[3])) {
        return false;
    }
    return !outside(planes[0]);
}

// ---------------------------------------------------------------------------
// glitch::collada synchronized animator blender and set
// ---------------------------------------------------------------------------

// 0x367870 _ZNK6glitch7collada37CSceneNodeAnimatorSynchronizedBlender14getNumSegmentsEv
int SynchronizedBlender::getNumSegments() const {
    // The original asks the first segment (vtable slot +8). Guarded for an empty list.
    if (segments.empty()) {
        return 0;
    }
    return segments.front()->getNumSegments();
}

// 0x367898 _ZNK6glitch7collada37CSceneNodeAnimatorSynchronizedBlender24getSynchronizationLengthEij
int SynchronizedBlender::getSynchronizationLength(int index, unsigned int at) const {
    return segments[index]->getSynchronizationLength(at);
}

// 0x3678c4 _ZNK6glitch7collada37CSceneNodeAnimatorSynchronizedBlender24getSynchronizationLengthEj
// Weighted sum over all segments.
int SynchronizedBlender::getSynchronizationLength(unsigned int at) const {
    float sum = 0.0f;
    for (std::size_t i = 0; i < segments.size(); ++i) {
        sum += static_cast<float>(getSynchronizationLength(static_cast<int>(i), at)) * weights[i];
    }
    return static_cast<int>(sum);
}

// 0x367948 _ZNK6glitch7collada37CSceneNodeAnimatorSynchronizedBlender27getSynchronizationTimestampEij
int SynchronizedBlender::getSynchronizationTimestamp(int index, unsigned int at) const {
    return segments[index]->getSynchronizationTimestamp(at);
}

// 0x367974 _ZNK6glitch7collada37CSceneNodeAnimatorSynchronizedBlender27getSynchronizationTimestampEj
int SynchronizedBlender::getSynchronizationTimestamp(unsigned int at) const {
    float sum = 0.0f;
    for (std::size_t i = 0; i < segments.size(); ++i) {
        sum += static_cast<float>(getSynchronizationTimestamp(static_cast<int>(i), at)) * weights[i];
    }
    return static_cast<int>(sum);
}

// 0x3679f8 _ZNK6glitch7collada37CSceneNodeAnimatorSynchronizedBlender25getSynchronizationSegmentEii
int SynchronizedBlender::getSynchronizationSegment(int index, int at) const {
    return segments[index]->getSynchronizationSegment(static_cast<unsigned int>(at));
}

// 0x367a24 _ZNK6glitch7collada37CSceneNodeAnimatorSynchronizedBlender25getSynchronizationSegmentEi
// Partial: the original calls vtable slot +156 without an argument in the
// pseudocode; this uses the timestamp of each segment as the comparison value.
int SynchronizedBlender::getSynchronizationSegment(int at) const {
    const int count = getNumSegments();
    int result = count - 1;
    for (int i = 0; i < count; ++i) {
        if (getSynchronizationTimestamp(i, 0u) <= at) {
            result = i;
        }
    }
    return result;
}

// 0x367a88 _ZN6glitch7collada37CSceneNodeAnimatorSynchronizedBlender16normalizeWeightsEv
// The pseudocode returns the incoming float register; the caller ignores it.
void SynchronizedBlender::normalizeWeights() {
    if (weightsNormalized) {
        return;
    }
    if (!weights.empty()) {
        float sum = 0.0f;
        for (float w : weights) {
            sum += w;
        }
        if (sum != 0.0f) {
            for (float& w : weights) {
                w = w / sum;
            }
        } else {
            weights.front() = 1.0f;
        }
    }
    weightsNormalized = true;
}

// 0x367b44 _ZNK6glitch7collada37CSceneNodeAnimatorSynchronizedBlender28getSynchronizationPercentageEv
int SynchronizedBlender::getSynchronizationPercentage() const {
    if (segments.empty()) {
        return 0;
    }
    return segments.front()->getSynchronizationPercentage();
}

// 0x368e30 _ZNK6glitch7collada33CSceneNodeAnimatorSynchronizedSet25getSynchronizationSegmentEi
// Partial: uses getSegmentTimestamp (vtable slot +156) and the segment count at +108.
unsigned int SynchronizedSet::getSynchronizationSegment(int at) const {
    unsigned int result = segmentCount - 1;
    if (segmentCount != 0) {
        for (unsigned int i = 0; i < segmentCount; ++i) {
            if (getSegmentTimestamp(i) <= static_cast<unsigned int>(at)) {
                result = i;
            }
        }
    }
    return result;
}

// 0x368ed4 _ZNK6glitch7collada33CSceneNodeAnimatorSynchronizedSet28getSynchronizationPercentageEv
// Partial: total length comes from virtual slot +72, not mapped.
float SynchronizedSet::getSynchronizationPercentage() const {
    return weight / getTotalLength();
}

float SynchronizedSet::getTotalLength() const {
    return 0.0f;  // stand-in for vtable slot +72
}

int SynchronizedSet::getSegmentTimestamp(unsigned int) const {
    return 0;  // stand-in for vtable slot +156
}

// ---------------------------------------------------------------------------
// glitch::gui::IGUIElement
// ---------------------------------------------------------------------------

bool GuiElement::isPointInside(const glitch::core::position2d<int>&) const {
    return false;  // stand-in for vtable slot +16
}

// 0x534eb8 _ZN6glitch3gui11IGUIElement4drawEv
void GuiElement::draw() {
    drawChildren();
}

// 0x534ef8 _ZN6glitch3gui11IGUIElement12OnPostRenderEj
void GuiElement::OnPostRender(unsigned int time) {
    postRenderChildren(time);
}

int GuiElement::getId() const {
    return 0;  // stand-in for vtable slot +84
}

const char* GuiElement::getName() const {
    return "";  // stand-in for vtable slot +92
}

int GuiElement::forwardEvent() {
    return 0;  // stand-in for vtable slot +8
}

int GuiElement::updateAbsolutePosition() {
    return 0;  // stand-in for vtable slot +12 (0x534920 not rebuilt in this pass)
}

// 0x534740 _ZN6glitch3gui11IGUIElement19setRelativePositionERKNS_4core4rectIiEE
// Partial: the scaled alignment fractions are computed for every mode-3 side
// from the parent's absolute rectangle; updateAbsolutePosition is a stand-in.
int GuiElement::setRelativePosition(const glitch::core::rect<int>& r) {
    if (parent) {
        const float parentWidth = static_cast<float>(parent->absolute.LowerRightCorner.X -
                                                     parent->absolute.UpperLeftCorner.X);
        const float parentHeight = static_cast<float>(parent->absolute.LowerRightCorner.Y -
                                                      parent->absolute.UpperLeftCorner.Y);
        if (alignMode[0] == 3) {
            alignScale[0] = static_cast<float>(r.UpperLeftCorner.X) / parentWidth;
        }
        if (alignMode[1] == 3) {
            alignScale[2] = static_cast<float>(r.LowerRightCorner.X) / parentWidth;
        }
        if (alignMode[2] == 3) {
            alignScale[1] = static_cast<float>(r.UpperLeftCorner.Y) / parentHeight;
        }
        if (alignMode[3] == 3) {
            alignScale[3] = static_cast<float>(r.LowerRightCorner.Y) / parentHeight;
        }
    }
    relative = r;
    return updateAbsolutePosition();
}

// 0x534840 _ZN6glitch3gui11IGUIElement12setAlignmentENS0_14EGUI_ALIGNMENTES2_S2_S2_
// Partial: only the left-edge scale branch is reconstructed; the other three
// branches (right, top, bottom) are not read in this pass.
void GuiElement::setAlignment(int a0, int a1, int a2, int a3) {
    alignMode[0] = a0;
    alignMode[1] = a1;
    alignMode[2] = a2;
    alignMode[3] = a3;
    if (parent) {
        const float parentWidth = static_cast<float>(parent->absolute.LowerRightCorner.X -
                                                     parent->absolute.UpperLeftCorner.X);
        if (a0 == 3) {
            alignScale[0] = static_cast<float>(relative.UpperLeftCorner.X) / parentWidth;
        }
    }
}

// 0x534e90 _ZN6glitch3gui11IGUIElement6removeEv
// Partial: the original returns the element pointer when there is no parent; 1 is returned here.
int GuiElement::remove() {
    if (parent) {
        return parent->removeChild(this);
    }
    return 1;
}

// 0x534f40 _ZN6glitch3gui11IGUIElement4moveENS_4core10position2dIiEE
int GuiElement::move(glitch::core::position2d<int> delta) {
    glitch::core::rect<int> r = relative;
    r.UpperLeftCorner.X += delta.X;
    r.UpperLeftCorner.Y += delta.Y;
    r.LowerRightCorner.X += delta.X;
    r.LowerRightCorner.Y += delta.Y;
    return setRelativePosition(r);
}

// 0x534fe8 _ZN6glitch3gui11IGUIElement7onEventERKNS_6SEventE
// Partial: the parent forwarding call has no event argument in the pseudocode.
int GuiElement::onEvent() {
    if (parent) {
        return parent->forwardEvent();
    }
    return 0;
}

// 0x534dd0 _ZN6glitch3gui11IGUIElement19getElementFromPointERKNS_4core10position2dIiEE
GuiElement* GuiElement::getElementFromPoint(const glitch::core::position2d<int>& point) {
    if (!visible) {
        return nullptr;
    }
    for (GuiElement* child : children) {
        if (GuiElement* found = child->getElementFromPoint(point)) {
            return found;
        }
    }
    if (visible && isPointInside(point)) {
        return this;
    }
    return nullptr;
}

// 0x534ef8 helper (children walk)
void GuiElement::drawChildren() {
    if (!visible) {
        return;
    }
    for (GuiElement* child : children) {
        child->draw();
    }
}

void GuiElement::postRenderChildren(unsigned int time) {
    if (!visible) {
        return;
    }
    for (GuiElement* child : children) {
        child->OnPostRender(time);
    }
}

// 0x53501c _ZNK6glitch3gui11IGUIElement16getElementFromIdEib
// Partial: the original recurses through slot +108; the child call is a direct call here.
GuiElement* GuiElement::getElementFromId(int id, bool recursive) const {
    for (GuiElement* child : children) {
        if (child->getId() == id) {
            return child;
        }
        if (recursive) {
            if (GuiElement* found = child->getElementFromId(id, true)) {
                return found;
            }
        }
    }
    return nullptr;
}

// 0x535d10 _ZN6glitch3gui11IGUIElement11removeChildEPS1_
// Partial: reference drop on the removed child is a stand-in.
int GuiElement::removeChild(GuiElement* child) {
    for (auto it = children.begin(); it != children.end(); ++it) {
        if (*it == child) {
            child->parent = nullptr;
            children.erase(it);
            return 1;
        }
    }
    return 0;
}

// 0x535d70 _ZN6glitch3gui11IGUIElement12bringToFrontEPS1_
// Partial: the child is moved to the end of the list (front of draw order).
int GuiElement::bringToFront(GuiElement* child) {
    for (auto it = children.begin(); it != children.end(); ++it) {
        if (*it == child) {
            children.erase(it);
            children.push_back(child);
            return 1;
        }
    }
    return 0;
}

// 0x535dec _ZN6glitch3gui11IGUIElement8addChildEPS1_
// Partial: reference add and the rect propagation (+0x68..) are stand-ins.
GuiElement* GuiElement::addChild(GuiElement* child) {
    if (child) {
        child->parent = this;
        children.push_back(child);
    }
    return this;
}

// 0x5362c8 _ZN6glitch3gui11IGUIElement7setTextEPKw
int GuiElement::setText(const wchar_t* s) {
    text.assign(s, std::wcslen(s));
    return 0;
}

// ---------------------------------------------------------------------------
// glitch::gui::CGUIEnvironment
// ---------------------------------------------------------------------------

// 0x5353c8 _ZN6glitch3gui15CGUIEnvironment8setFocusEPNS0_11IGUIElementE
// Partial: focus-reference counting is a stand-in.
int GuiEnvironment::setFocus(GuiElement* element) {
    if (focus == element) {
        return 0;
    }
    focus = element;
    return 1;
}

// 0x5355a0 _ZN6glitch3gui15CGUIEnvironment11removeFocusEPNS0_11IGUIElementE
// Partial: the element's own handler (slot +8) decides whether the drop happens.
int GuiEnvironment::removeFocus(GuiElement* element) {
    if (!focus) {
        return 0;
    }
    if (focus != element) {
        focus = nullptr;
        return 1;
    }
    return 0;
}

// 0x535658 _ZN6glitch3gui15CGUIEnvironment5clearEv
// Partial: clears focus; the child walk on the root (slot +104) is not mapped.
void GuiEnvironment::clear() {
    focus = nullptr;
    focusReference = nullptr;
}

// 0x535778 _ZN6glitch3gui15CGUIEnvironment7setSkinEPNS0_8IGUISkinE
// Partial: skin reference counting is a stand-in; only the pointer swap is kept.
int GuiEnvironment::setSkin(void* newSkin) {
    if (skin != newSkin) {
        skin = newSkin;
    }
    return 0;
}

// 0x5357d0 _ZNK6glitch3gui15CGUIEnvironment27getDefaultGUIElementFactoryEv
// Partial: vtable slot +224 (factory lookup) is not mapped.
void* GuiEnvironment::getDefaultGUIElementFactory() const {
    return nullptr;
}

// 0x535818 _ZN6glitch3gui15CGUIEnvironment13addGUIElementEPKcPNS0_11IGUIElementE
// Partial: the factory fan-out (vtable slot +16 on each registered factory) is not mapped.
int GuiEnvironment::addGUIElement(const char* name, GuiElement* parentElement) {
    (void)name;
    (void)parentElement;
    return 0;
}

// 0x535884 _ZN6glitch3gui15CGUIEnvironment7saveGUIEPKcPNS0_11IGUIElementE
// Partial: writer creation (vtable slot +20) and the save call are not mapped.
int GuiEnvironment::saveGUI(const char* file, GuiElement* element) {
    (void)file;
    (void)element;
    return 0;
}

// 0x5358e0 _ZN6glitch3gui15CGUIEnvironment7saveGUIEPNS_2io10IWriteFileEPNS0_11IGUIElementE
// Partial: writer creation (vtable slot +96) is not mapped.
int GuiEnvironment::saveGUI(void* writeFile, GuiElement* element) {
    (void)writeFile;
    (void)element;
    return 0;
}

// 0x535b58 _ZN6glitch3gui15CGUIEnvironment14getElementByIdEiPNS0_11IGUIElementE
// Partial: the recursive search uses vtable slot +68 on the environment.
GuiElement* GuiEnvironment::getElementById(int id, GuiElement* start) {
    GuiElement* from = start ? start : &root;
    if (from->getId() == id) {
        return from;
    }
    for (GuiElement* child : from->children) {
        if (GuiElement* found = getElementById(id, child)) {
            return found;
        }
    }
    return nullptr;
}

// 0x535c10 _ZN6glitch3gui15CGUIEnvironment17getRootGUIElementEv
GuiElement* GuiEnvironment::getRootGUIElement() {
    return &root;
}

// 0x5364f4 _ZN6glitch3gui15CGUIEnvironment11addComboBoxERKNS_4core4rectIiEEPNS0_11IGUIElementEi
// Partial: concrete element constructor not mapped.
GuiElement* GuiEnvironment::addComboBox(const glitch::core::rect<int>&, GuiElement*, int) {
    return nullptr;
}

// 0x536574 _ZN6glitch3gui15CGUIEnvironment13addInOutFaderEPKNS_4core4rectIiEEPNS0_11IGUIElementEi
// Partial: concrete element constructor not mapped.
GuiElement* GuiEnvironment::addInOutFader(const glitch::core::rect<int>*, GuiElement*, int) {
    return nullptr;
}

// 0x536628 _ZN6glitch3gui15CGUIEnvironment10addToolBarEPNS0_11IGUIElementEi
// Partial: concrete element constructor not mapped.
GuiElement* GuiEnvironment::addToolBar(GuiElement*, int) {
    return nullptr;
}

// 0x53669c _ZN6glitch3gui15CGUIEnvironment7addMenuEPNS0_11IGUIElementEi
// Partial: concrete element constructor not mapped.
GuiElement* GuiEnvironment::addMenu(GuiElement*, int) {
    return nullptr;
}

// 0x536724 _ZN6glitch3gui15CGUIEnvironment14addContextMenuERKNS_4core4rectIiEEPNS0_11IGUIElementEi
// Partial: concrete element constructor not mapped.
GuiElement* GuiEnvironment::addContextMenu(const glitch::core::rect<int>&, GuiElement*, int) {
    return nullptr;
}

// 0x5367ac _ZN6glitch3gui15CGUIEnvironment6addTabERKNS_4core4rectIiEEPNS0_11IGUIElementEi
// Partial: concrete element constructor not mapped.
GuiElement* GuiEnvironment::addTab(const glitch::core::rect<int>&, GuiElement*, int) {
    return nullptr;
}

// 0x536810 _ZN6glitch3gui15CGUIEnvironment13addTabControlERKNS_4core4rectIiEEPNS0_11IGUIElementEbbi
// Partial: concrete element constructor not mapped.
GuiElement* GuiEnvironment::addTabControl(const glitch::core::rect<int>&, GuiElement*, bool, bool, int) {
    return nullptr;
}

// 0x536880 _ZN6glitch3gui15CGUIEnvironment10addSpinBoxEPKwRKNS_4core4rectIiEEPNS0_11IGUIElementEi
// Partial: concrete element constructor not mapped.
GuiElement* GuiEnvironment::addSpinBox(const wchar_t*, const glitch::core::rect<int>&, GuiElement*, int) {
    return nullptr;
}

// 0x5368e8 _ZN6glitch3gui15CGUIEnvironment10addEditBoxEPKwRKNS_4core4rectIiEEbPNS0_11IGUIElementEi
// Partial: concrete element constructor not mapped.
GuiElement* GuiEnvironment::addEditBox(const wchar_t*, const glitch::core::rect<int>&, bool, GuiElement*, int) {
    return nullptr;
}

// 0x536388 _ZN6glitch3gui15CGUIEnvironment7loadGUIEPKcPNS0_11IGUIElementE
// Partial: read-file factory (vtable slot +12) and the load call are not mapped.
int GuiEnvironment::loadGUI(const char* file, GuiElement* element) {
    (void)file;
    (void)element;
    return 0;
}

// 0x53644c _ZN6glitch3gui15CGUIEnvironment16getElementByNameEPKcPNS0_11IGUIElementE
// Partial: the name compare (slot +92) and the recursive child walk are reconstructed.
GuiElement* GuiEnvironment::getElementByName(const char* name, GuiElement* start) {
    GuiElement* from = start ? start : &root;
    if (std::strcmp(from->getName(), name) == 0) {
        return from;
    }
    for (GuiElement* child : from->children) {
        if (GuiElement* found = getElementByName(name, child)) {
            return found;
        }
    }
    return nullptr;
}

// ---------------------------------------------------------------------------
// Free-standing bodies
// ---------------------------------------------------------------------------

// 0x50cf04 _ZNK6glitch5scene13CBatchMeshExtIP10GameObjectE20saveSegmentExtraDataEPKvPNS_2io10IWriteFileENS_2os8E_ENDIANE
// Empty in the original.
void BatchMeshExtExtraData::saveSegmentExtraData(const void*, void*, int) const {}

// 0x533fb0 _ZN6glitch11CIrrFactory16createFileSystemEv
// Partial: the CFileSystem constructor is not mapped; the pointer is left null here.
IrrFactory* IrrFactory::createFileSystem() {
    return this;
}

// 0x533f28 _ZN6glitch11CIrrFactory20createGUIEnvironment...
// Partial: the CGUIEnvironment constructor is not mapped.
GuiEnvironment* IrrFactory::createGUIEnvironment(void*, void*, void*) {
    return new GuiEnvironment();
}

}  // namespace glitch::collada::recon_00
