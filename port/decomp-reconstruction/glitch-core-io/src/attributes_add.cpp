// Typed attribute creation (addX) and the by-name setters that fall back to it.
// Each addX builds the typed attribute, stores name / flag / value, and appends
// it to the current group. The by-name setters call the matching virtual when
// the name exists, and otherwise call addX with flag false.
#include "glitch-core-io/io/attributes.hpp"
#include "glitch-core-io/io/scalar_attributes.hpp"
#include "glitch-core-io/io/typed_attributes.hpp"

namespace glitch::io {

// 0x568b1c _ZN6glitch2io11CAttributes6addIntEPKcib
void CAttributes::addInt(const char* name, int value, bool flag) {
    currentAttributes().push_back(new CIntAttribute(name, value, flag));
}

// 0x56891c _ZN6glitch2io11CAttributes8addFloatEPKcfb
void CAttributes::addFloat(const char* name, float value, bool flag) {
    currentAttributes().push_back(new CFloatAttribute(name, value, flag));
}

// 0x56871c _ZN6glitch2io11CAttributes7addBoolEPKcbb
void CAttributes::addBool(const char* name, bool value, bool flag) {
    currentAttributes().push_back(new CBoolAttribute(name, value, flag));
}

// 0x568038 _ZN6glitch2io11CAttributes14addUserPointerEPKcPvb
void CAttributes::addUserPointer(const char* name, void* value, bool flag) {
    currentAttributes().push_back(new CUserPointerAttribute(name, value, flag));
}

// 0x568ce0 _ZN6glitch2io11CAttributes7addRectEPKcNS_4core4rectIiEEb
void CAttributes::addRect(const char* name, glitch::core::rect<std::int32_t> value, bool flag) {
    currentAttributes().push_back(new CRectAttribute(name, value, flag));
}

// 0x56b11c _ZN6glitch2io11CAttributes12setAttributeEPKci
void CAttributes::setAttribute(const char* name, int value) {
    if (IAttribute* a = getAttributeP(name)) {
        a->setInt(value);
    } else {
        addInt(name, value, false);
    }
}

// 0x56b07c _ZN6glitch2io11CAttributes12setAttributeEPKcf
void CAttributes::setAttribute(const char* name, float value) {
    if (IAttribute* a = getAttributeP(name)) {
        a->setFloat(value);
    } else {
        addFloat(name, value, false);
    }
}

// 0x56b1bc _ZN6glitch2io11CAttributes12setAttributeEPKcb
void CAttributes::setAttribute(const char* name, bool value) {
    if (IAttribute* a = getAttributeP(name)) {
        a->setBool(value);
    } else {
        addBool(name, value, false);
    }
}

// 0x56ad30 _ZN6glitch2io11CAttributes12setAttributeEPKcPv
void CAttributes::setAttribute(const char* name, void* value) {
    if (IAttribute* a = getAttributeP(name)) {
        a->setUserPointer(value);
    } else {
        addUserPointer(name, value, false);
    }
}

// 0x56b878 _ZN6glitch2io11CAttributes12setAttributeEPKcNS_4core4rectIiEE
void CAttributes::setAttribute(const char* name, glitch::core::rect<std::int32_t> value) {
    if (IAttribute* a = getAttributeP(name)) {
        a->setRect(value);
    } else {
        addRect(name, value, false);
    }
}

// Each typed add creates the attribute with the matching constructor and appends it.
// 0x56a1a0
void CAttributes::addVector2d(const char* name, const glitch::core::vector2d<float>& value, bool flag) {
    currentAttributes().push_back(new CVector2DAttribute(name, value, flag));
}

// 0x56a030
void CAttributes::addVector3d(const char* name, const glitch::core::vector3d<float>& value, bool flag) {
    currentAttributes().push_back(new CVector3DAttribute(name, value, flag));
}

// 0x569ebc
void CAttributes::addVector4d(const char* name, const glitch::core::vector4d<float>& value, bool flag) {
    currentAttributes().push_back(new CVector4DAttribute(name, value, flag));
}

// 0x5692c8
void CAttributes::addVector2di(const char* name, const glitch::core::vector2d<std::int32_t>& value, bool flag) {
    currentAttributes().push_back(new CVector2DIAttribute(name, value, flag));
}

// 0x569158
void CAttributes::addVector3di(const char* name, const glitch::core::vector3d<std::int32_t>& value, bool flag) {
    currentAttributes().push_back(new CVector3DIAttribute(name, value, flag));
}

// 0x568fe4
void CAttributes::addVector4di(const char* name, const glitch::core::vector4d<std::int32_t>& value, bool flag) {
    currentAttributes().push_back(new CVector4DIAttribute(name, value, flag));
}

// 0x568e64
void CAttributes::addPosition2d(const char* name, glitch::core::position2d<std::int32_t> value, bool flag) {
    currentAttributes().push_back(new CPosition2DAttribute(name, value, flag));
}

// 0x569604
void CAttributes::addLine2d(const char* name, glitch::core::line2d<float> value, bool flag) {
    currentAttributes().push_back(new CLine2dAttribute(name, value, flag));
}

// 0x56944c
void CAttributes::addLine3d(const char* name, glitch::core::line3d<float> value, bool flag) {
    currentAttributes().push_back(new CLine3dAttribute(name, value, flag));
}

// 0x5697ac
void CAttributes::addTriangle3d(const char* name, glitch::core::triangle3d<float> value, bool flag) {
    currentAttributes().push_back(new CTriangleAttribute(name, value, flag));
}

// 0x569b3c
void CAttributes::addBox3d(const char* name, glitch::core::aabbox3d<float> value, bool flag) {
    currentAttributes().push_back(new CBBoxAttribute(name, value, flag));
}

// 0x569998
void CAttributes::addPlane3d(const char* name, glitch::core::plane3d<float> value, bool flag) {
    currentAttributes().push_back(new CPlaneAttribute(name, value, flag));
}

// 0x569d00
void CAttributes::addQuaternion(const char* name, glitch::core::quaternion value, bool flag) {
    currentAttributes().push_back(new CQuaternionAttribute(name, value, flag));
}

// 0x56a31c
void CAttributes::addColorf(const char* name, glitch::video::SColorf value, bool flag) {
    currentAttributes().push_back(new CColorfAttribute(name, value, flag));
}

// 0x56a70c
void CAttributes::addMatrix(const char* name, const glitch::core::CMatrix4<float>& value, bool flag) {
    currentAttributes().push_back(new CMatrixAttribute(name, value, flag));
}

// By-name setters: set the existing attribute through its virtual, else add with flag false.
// 0x5659b4
void CAttributes::setAttribute(const char* name, const glitch::core::vector2d<float>& value) {
    if (IAttribute* a = getAttributeP(name)) a->setVector2d(value); else addVector2d(name, value, false);
}

// 0x567a9c
void CAttributes::setAttribute(const char* name, const glitch::core::vector2d<std::int32_t>& value) {
    if (IAttribute* a = getAttributeP(name)) a->setVector2d(value); else addVector2di(name, value, false);
}

// 0x56b7b8
void CAttributes::setAttribute(const char* name, const glitch::core::vector3d<float>& value) {
    if (IAttribute* a = getAttributeP(name)) a->setVector3d(value); else addVector3d(name, value, false);
}

// 0x56b6f8
void CAttributes::setAttribute(const char* name, const glitch::core::vector4d<float>& value) {
    if (IAttribute* a = getAttributeP(name)) a->setVector4d(value); else addVector4d(name, value, false);
}

// 0x56b638
void CAttributes::setAttribute(const char* name, const glitch::core::vector3d<std::int32_t>& value) {
    if (IAttribute* a = getAttributeP(name)) a->setVector3di(value); else addVector3di(name, value, false);
}

// 0x56b578
void CAttributes::setAttribute(const char* name, const glitch::core::vector4d<std::int32_t>& value) {
    if (IAttribute* a = getAttributeP(name)) a->setVector4di(value); else addVector4di(name, value, false);
}

// 0x56b978
void CAttributes::setAttribute(const char* name, glitch::core::position2d<std::int32_t> value) {
    if (IAttribute* a = getAttributeP(name)) a->setPosition(value); else addPosition2d(name, value, false);
}

// 0x56bcc4
void CAttributes::setAttribute(const char* name, glitch::core::line2d<float> value) {
    if (IAttribute* a = getAttributeP(name)) a->setLine2d(value); else addLine2d(name, value, false);
}

// 0x56bf4c
void CAttributes::setAttribute(const char* name, const glitch::core::plane3d<float>& value) {
    if (IAttribute* a = getAttributeP(name)) a->setPlane(value); else addPlane3d(name, value, false);
}

// 0x56add0
void CAttributes::setAttribute(const char* name, glitch::core::quaternion value) {
    if (IAttribute* a = getAttributeP(name)) a->setQuaternion(value); else addQuaternion(name, value, false);
}

// 0x56aee4
void CAttributes::setAttribute(const char* name, const glitch::core::CMatrix4<float>& value) {
    if (IAttribute* a = getAttributeP(name)) a->setMatrix(value); else addMatrix(name, value, false);
}

}  // namespace glitch::io
