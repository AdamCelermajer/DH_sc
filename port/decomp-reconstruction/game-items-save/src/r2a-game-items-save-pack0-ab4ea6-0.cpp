// Batch r2a-game-items-save-pack0-ab4ea6-0 (redo): PropertyMap::LoadOverridesFromXML (0x513a00). Declaration and layout
// come from dh2/PropertyMap.hpp. The rest of PropertyMap is in the g1-game-items-save-PropertyMap-ab4ea6-0 and -1 files.
//
// PARTIAL. Checked against the asm (0x513a00): a null element returns at once. Otherwise the loop walks GetPropertyMap()
// in key order, reads each key's data pointer (node +0x24, the c_str of the key string), passes it to
// TiXmlElement::Attribute(const char*) const (0x514c70) and then passes the name and the attribute value to
// PropertyMap::SetProperty (0x51387c). A null value there resets the property to its default.
// tixml_dh.hpp does not declare TiXmlElement::Attribute, so the attribute is not read yet and SetProperty is not called:
// this body changes no property. It becomes the original once that call is declared.

#include "dh2/PropertyMap.hpp"

// 0x513a00 _ZN11PropertyMap20LoadOverridesFromXMLEP12TiXmlElement
void PropertyMap::LoadOverridesFromXML(TiXmlElement* element) {
    if (element == nullptr) {
        return;
    }
    for (const auto& entry : GetPropertyMap()) {
        const char* name = entry.first.c_str();  // the key's data pointer, node +0x24 in the original
        // missing: const char* value = element->Attribute(name);  (TiXmlElement::Attribute(const char*) const, 0x514c70)
        // missing: SetProperty(name, value);  (PropertyMap::SetProperty, 0x51387c)
        static_cast<void>(name);
    }
}
