// Batch g1-game-items-save-PropertyMap-ab4ea6-1: CloneProperties, AddProperty(const char*, Property*),
// InitProperties, SetTemplate and LoadTemplate of PropertyMap. Declarations and layout come from
// dh2/PropertyMap.hpp (AddProperty's return type is the map slot, as the original returns it).
//
// Not in this file yet: LoadOverridesFromXML (0x513a00). It reads each attribute with
// TiXmlElement::Attribute, but the global TiXmlElement in tinyxml-and-c-libs/include/tinyxml-and-c-libs/tixml_dh.hpp
// declares only Value(). The upstream TinyXML API is not vendored here, and the Attribute declaration in
// vox-audio/vendor/tixml_dh.hpp belongs to vox::tixml, a different class.
//
// The bodies use GetThisClassName (0x510b4c), GetPropertyMap (0x5134c0), SetProperty (0x51387c) and the
// s_classMap table. These belong to the other PropertyMap batch (ab4ea6-0), which is not in this file.

#include "dh2/PropertyMap.hpp"

#include <cstdio>
#include <cstdlib>
#include <map>
#include <new>
#include <string>

#include "core-platform/memory.hpp"
#include "dh2/Property.hpp"
#include "dh2/SimpleTypeProperty.hpp"

namespace {

// The original's ASSERT(expr) in PropertyMap.cpp and PropertyMapTemplates.cpp. gAssertLevel 2 stops the
// program (the original writes to address 0 to force a crash), 1 prints the message and carries on, 0 stays silent.
void ReportPropertyMapAssert(const char* expr, const char* file, int line) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", expr, file, line);
    }
}

}  // namespace

// 0x5138fc _ZN11PropertyMap15ClonePropertiesERS_
void PropertyMap::CloneProperties(PropertyMap& other) {
    // Walks the other object's current template table in key order. Each value is read from the other
    // object (the owner passed to ToString), then written into this object under the same name.
    for (const auto& entry : other.GetPropertyMap()) {
        std::string value = entry.second->ToString(&other);
        SetProperty(entry.first.c_str(), value.c_str());
    }
}

// 0x513ce4 _ZN11PropertyMap11AddPropertyEPKcP8Property
Property*& PropertyMap::AddProperty(const char* name, Property* property) {
    // Properties always go into the class's default table (template ""), whatever m_template is.
    std::map<std::string, Property*>& defaults = s_classMap[GetThisClassName()][""];
    auto found = defaults.find(name);
    if (found != defaults.end()) {
        Property* replaced = found->second;
        if (replaced != nullptr) {
            // The original sets the vptr to Property's table, destroys the name and frees the block with
            // CustomFree. Property has no destructor slot, so the port calls the base destructor directly.
            // The derived part (SimpleTypeProperty's value) is not destroyed, as in the original.
            replaced->~Property();
            CustomFree(replaced);
        }
    }
    Property*& slot = defaults[name];
    slot = property;
    return slot;
}

// 0x513d78 _ZN11PropertyMap14InitPropertiesEv
void PropertyMap::InitProperties() {
    // First use of this class: it has no table yet. Register the "template" property (the m_template
    // string at +0x04, default "") and then let the class declare its own properties.
    if (s_classMap.find(GetThisClassName()) != s_classMap.end()) {
        return;
    }
    std::string defaultTemplate;
    Property* templateProperty = new (CustomAlloc(static_cast<unsigned>(sizeof(SimpleTypeProperty<std::string>))))
        SimpleTypeProperty<std::string>(this, "template", &m_template, defaultTemplate);
    AddProperty("template", templateProperty);
    DeclareProperties();
}

// 0x513fec _ZN11PropertyMap11SetTemplateERKSs
void PropertyMap::SetTemplate(const std::string& templateName) {
    if (templateName.empty()) {
        return;
    }
    if (&templateName != &m_template) {
        m_template.assign(templateName);
    }
    if (GetPropertyMap().empty()) {
        std::map<std::string, Property*>& defaults = s_classMap[GetThisClassName()][""];
        std::map<std::string, Property*>& fresh = GetPropertyMap();
        if (!fresh.empty()) {
            // Unreachable from here: the table was empty just above. Kept as in the original.
            ReportPropertyMapAssert("newTemplate.size() == 0",
                                    "..\\..\\project_vs2005\\Game/..\\..\\sources\\Data\\PropertyMap\\PropertyMap.cpp",
                                    112);
        }
        // Each default property is cloned into the new template's table under the same name.
        for (const auto& entry : defaults) {
            Property*& slot = fresh[entry.first];
            slot = entry.second->Clone();
        }
    }
    LoadTemplate();
}

// 0x51419c _ZN11PropertyMap12LoadTemplateEv
void PropertyMap::LoadTemplate() {
    // The base class is never asked to load a template: the original asserts false here.
    ReportPropertyMapAssert("false",
                            "..\\..\\project_vs2005\\Game/..\\..\\sources\\Data\\PropertyMap\\PropertyMapTemplates.cpp",
                            9);
}
