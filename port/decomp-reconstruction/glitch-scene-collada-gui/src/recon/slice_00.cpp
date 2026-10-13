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

// glitch::scene::SViewFrustum::intersects and intersectsWithoutBoxTest3 are the canonical bodies
// (tb/glitch_scene_SViewFrustum.hpp, src/tb_scene-collada-gui-glitch_scene_SViewFrustum-0.cpp).

// ---------------------------------------------------------------------------
// Free-standing bodies
// ---------------------------------------------------------------------------

// 0x50cf04 _ZNK6glitch5scene13CBatchMeshExtIP10GameObjectE20saveSegmentExtraDataEPKvPNS_2io10IWriteFileENS_2os8E_ENDIANE
// Empty in the original.
void BatchMeshExtExtraData::saveSegmentExtraData(const void*, void*, int) const {}

}  // namespace glitch::collada::recon_00
