#include "glitch-core-io/io/attributes.hpp"

namespace glitch::io {

namespace {
// Original bounds check: unsigned index strictly below the element count.
// Negative indices therefore fail the check as well.
inline IAttribute* at(const std::vector<IAttribute*>& list, int index) {
    if (index < 0 || static_cast<std::size_t>(index) >= list.size()) return nullptr;
    return list[static_cast<std::size_t>(index)];
}
// Getters return a default when the index is out of range (count <= index).
}  // namespace

// 0x5607e4 _ZNK6glitch2io11CAttributes17getAttributeCountEv
int CAttributes::getAttributeCount() const {
    return static_cast<int>(attributes.size());
}

// 0x5607fc _ZN6glitch2io11CAttributes16getAttributeNameEi
const char* CAttributes::getAttributeName(int index) const {
    IAttribute* a = at(attributes, index);
    return a ? a->name : nullptr;
}

// 0x560820 _ZN6glitch2io11CAttributes16getAttributeTypeEi
int CAttributes::getAttributeType(int index) const {
    IAttribute* a = at(attributes, index);
    return a ? a->getType() : 0x1e;
}

// 0x5608a0 _ZN6glitch2io11CAttributes7getBoolEi
bool CAttributes::getBool(int index) const {
    IAttribute* a = at(attributes, index);
    return a ? a->getBool() : false;
}

// 0x5608dc _ZN6glitch2io11CAttributes6getIntEi
int CAttributes::getInt(int index) const {
    IAttribute* a = at(attributes, index);
    return a ? a->getInt() : 0;
}

// 0x560918 _ZN6glitch2io11CAttributes8getFloatEi
float CAttributes::getFloat(int index) const {
    IAttribute* a = at(attributes, index);
    return a ? a->getFloat() : 0.f;
}

// 0x560d1c _ZN6glitch2io11CAttributes7getEnumEi
int CAttributes::getEnum(int index) const {
    IAttribute* a = at(attributes, index);
    return a ? a->getEnum() : 0;
}

// 0x560ce4 _ZN6glitch2io11CAttributes13getBinaryDataEiPvi
void CAttributes::getBinaryData(int index, void* data, int size) const {
    if (IAttribute* a = at(attributes, index)) a->getBinary(data, size);
}

// 0x560db0 _ZN6glitch2io11CAttributes12setAttributeEiPKc
void CAttributes::setAttribute(int index, const char* value) {
    if (IAttribute* a = at(attributes, index)) a->setString(value);
}

// 0x560e20 _ZN6glitch2io11CAttributes12setAttributeEib
void CAttributes::setAttribute(int index, bool value) {
    if (IAttribute* a = at(attributes, index)) a->setBool(value);
}

// 0x560e58 _ZN6glitch2io11CAttributes12setAttributeEii
void CAttributes::setAttribute(int index, int value) {
    if (IAttribute* a = at(attributes, index)) a->setInt(value);
}

// 0x560e90 _ZN6glitch2io11CAttributes12setAttributeEif
void CAttributes::setAttribute(int index, float value) {
    if (IAttribute* a = at(attributes, index)) a->setFloat(value);
}

// 0x560ec8 _ZN6glitch2io11CAttributes12setAttributeEiNS_5video6SColorE
void CAttributes::setAttribute(int index, glitch::video::SColor value) {
    if (IAttribute* a = at(attributes, index)) a->setColor(value);
}

// 0x560f6c _ZN6glitch2io11CAttributes12setAttributeEiRKNS_4core8vector2dIfEE
void CAttributes::setAttribute(int index, const glitch::core::vector2d<float>& value) {
    if (IAttribute* a = at(attributes, index)) a->setVector2d(value);
}

// 0x560fb8 _ZN6glitch2io11CAttributes12setAttributeEiRKNS_4core8vector3dIfEE
void CAttributes::setAttribute(int index, const glitch::core::vector3d<float>& value) {
    if (IAttribute* a = at(attributes, index)) a->setVector3d(value);
}

// 0x560ff0 _ZN6glitch2io11CAttributes12setAttributeEiRKNS_4core8vector4dIfEE
void CAttributes::setAttribute(int index, const glitch::core::vector4d<float>& value) {
    if (IAttribute* a = at(attributes, index)) a->setVector4d(value);
}

// 0x561028 _ZN6glitch2io11CAttributes12setAttributeEiRKNS_4core8vector2dIiEE
void CAttributes::setAttribute(int index, const glitch::core::vector2d<std::int32_t>& value) {
    if (IAttribute* a = at(attributes, index)) a->setVector2di(value);
}

// 0x56106c _ZN6glitch2io11CAttributes12setAttributeEiRKNS_4core8vector3dIiEE
void CAttributes::setAttribute(int index, const glitch::core::vector3d<std::int32_t>& value) {
    if (IAttribute* a = at(attributes, index)) a->setVector3di(value);
}

// 0x5610a4 _ZN6glitch2io11CAttributes12setAttributeEiRKNS_4core8vector4dIiEE
void CAttributes::setAttribute(int index, const glitch::core::vector4d<std::int32_t>& value) {
    if (IAttribute* a = at(attributes, index)) a->setVector4di(value);
}

// 0x5610dc _ZN6glitch2io11CAttributes12setAttributeEiNS_4core10position2dIiEE
void CAttributes::setAttribute(int index, glitch::core::position2d<std::int32_t> value) {
    if (IAttribute* a = at(attributes, index)) a->setPosition(value);
}

// 0x56117c _ZN6glitch2io11CAttributes12setAttributeEiPvi
void CAttributes::setAttribute(int index, void* data, int size) {
    if (IAttribute* a = at(attributes, index)) a->setBinary(data, size);
}

// 0x5611b4 _ZN6glitch2io11CAttributes12setAttributeEiPKcPKS3_
void CAttributes::setAttribute(int index, const char* value, const char* const* choices) {
    if (IAttribute* a = at(attributes, index)) a->setEnum(value, choices);
}

// 0x56189c _ZN6glitch2io11CAttributes12setAttributeEiPv
void CAttributes::setAttribute(int index, void* userPointer) {
    if (IAttribute* a = at(attributes, index)) a->setUserPointer(userPointer);
}

// 0x5609e8 _ZN6glitch2io11CAttributes9getColorfEi
// Out of range gives opaque black (0, 0, 0, 1).
glitch::video::SColorf CAttributes::getColorf(int index) const {
    IAttribute* a = at(attributes, index);
    return a ? a->getColorf() : glitch::video::SColorf{0.f, 0.f, 0.f, 1.f};
}

// 0x560a44 _ZN6glitch2io11CAttributes11getVector2dEi
glitch::core::vector2d<float> CAttributes::getVector2d(int index) const {
    IAttribute* a = at(attributes, index);
    return a ? a->getVector2d() : glitch::core::vector2d<float>{0.f, 0.f};
}

// 0x560a94 _ZN6glitch2io11CAttributes11getVector3dEi
glitch::core::vector3d<float> CAttributes::getVector3d(int index) const {
    IAttribute* a = at(attributes, index);
    return a ? a->getVector3d() : glitch::core::vector3d<float>{0.f, 0.f, 0.f};
}

// 0x560ae8 _ZN6glitch2io11CAttributes11getVector4dEi
glitch::core::vector4d<float> CAttributes::getVector4d(int index) const {
    IAttribute* a = at(attributes, index);
    return a ? a->getVector4d() : glitch::core::vector4d<float>{0.f, 0.f, 0.f, 0.f};
}

// 0x560b40 _ZN6glitch2io11CAttributes12getVector2diEi
glitch::core::vector2d<std::int32_t> CAttributes::getVector2di(int index) const {
    IAttribute* a = at(attributes, index);
    return a ? a->getVector2di() : glitch::core::vector2d<std::int32_t>{0, 0};
}

// 0x560b90 _ZN6glitch2io11CAttributes12getVector3diEi
glitch::core::vector3d<std::int32_t> CAttributes::getVector3di(int index) const {
    IAttribute* a = at(attributes, index);
    return a ? a->getVector3di() : glitch::core::vector3d<std::int32_t>{0, 0, 0};
}

// 0x560be4 _ZN6glitch2io11CAttributes12getVector4diEi
glitch::core::vector4d<std::int32_t> CAttributes::getVector4di(int index) const {
    IAttribute* a = at(attributes, index);
    return a ? a->getVector4di() : glitch::core::vector4d<std::int32_t>{0, 0, 0, 0};
}

// 0x560c3c _ZN6glitch2io11CAttributes13getPosition2dEi
glitch::core::position2d<std::int32_t> CAttributes::getPosition2d(int index) const {
    IAttribute* a = at(attributes, index);
    return a ? a->getPosition() : glitch::core::position2d<std::int32_t>{0, 0};
}

// 0x560c8c _ZN6glitch2io11CAttributes7getRectEi
glitch::core::rect<std::int32_t> CAttributes::getRect(int index) const {
    IAttribute* a = at(attributes, index);
    return a ? a->getRect() : glitch::core::rect<std::int32_t>{0, 0, 0, 0};
}

// 0x560f0c _ZN6glitch2io11CAttributes12setAttributeEiNS_5video7SColorfE
void CAttributes::setAttribute(int index, glitch::video::SColorf value) {
    if (IAttribute* a = at(attributes, index)) a->setColor(value);
}

}  // namespace glitch::io
