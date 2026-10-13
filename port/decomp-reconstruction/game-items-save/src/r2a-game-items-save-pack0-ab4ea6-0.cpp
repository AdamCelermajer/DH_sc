// Batch r2a-game-items-save-pack0-ab4ea6-0 (redo): PropertyMap::LoadOverridesFromXML (0x513a00). Declaration and layout
// come from dh2/PropertyMap.hpp. The rest of PropertyMap is in the g1-game-items-save-PropertyMap-ab4ea6-0 and -1 files.
//
// Checked against the asm (0x513a00); the body is the original loop.

#include "dh2/PropertyMap.hpp"

#include "tinyxml-and-c-libs/tixml_dh.hpp"

// 0x513a00 _ZN11PropertyMap20LoadOverridesFromXMLEP12TiXmlElement
// A null element returns at once. Otherwise the loop walks GetPropertyMap() in key order, passes each key's data pointer (node +0x24,
// the c_str of the key) to TiXmlElement::Attribute(const char*) const (0x514c70, unmodified upstream TinyXML) and then passes the
// name and the attribute value to PropertyMap::SetProperty (0x51387c). A null value there resets the property to its default.
void PropertyMap::LoadOverridesFromXML(TiXmlElement* element) {
    if (element == nullptr) {
        return;
    }
    for (const auto& entry : GetPropertyMap()) {
        const char* name = entry.first.c_str();  // the key's data pointer, node +0x24 in the original
        const char* value = element->Attribute(name);
        SetProperty(name, value);
    }
}
