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

}  // namespace glitch::io
