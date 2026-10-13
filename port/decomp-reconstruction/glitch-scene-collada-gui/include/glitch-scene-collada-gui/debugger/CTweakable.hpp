// glitch::debugger::CTweakable: the debug tweaker base class. Owned by the glitch-scene-collada-gui queue (the
// engine-glitch functions at 0x31f410 .. 0x32cfc8 and 0x404ab0 in its queue.csv). This is the one canonical
// declaration; the game classes (dh2/GamepadTweaker.hpp, dh2/PlayerLightTweaker.hpp) include it by this path.
//
// A tweaker holds named variables (int, float, SColor) registered by its derived class, and a root group name
// and xml file name loaded from a tweaker XML. Original size: the base occupies +0x00 .. +0x7b (the game classes
// place their first member at +0x7c).
//
// Vtable (tools/decomp_vtable.py, _ZTVN6glitch8debugger10CTweakableE): +0x00/+0x04 ~CTweakable, +0x08
// getInstanceName, +0x0c getXML, +0x10 writeValues, +0x14 onSetValue.
//
// Partial: loadXML and writeValues need the XML reader and writer (CAttributes, CXMLWriter, CReadFile), which are not
// mapped, so their bodies are empty stubs and their notes say so. setValue, loadGroup and writeGroup are not declared
// because their argument types are not recovered.
#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "glitch-shared/glitch_core.hpp"  // glitch::video::SColor

namespace glitch {
class IDevice;  // glitch-core-io/include/glitch-core-io/os/device.hpp
namespace io {
class IWriteFile;  // glitch-core-io/include/glitch-core-io/io/interfaces.hpp
}  // namespace io
}  // namespace glitch

namespace glitch::debugger {

class CTweakable {
public:
    // A registered variable: the value kind (E_ATTRIBUTE_TYPE in the original: 0 int, 1 float, 5 SColor), the address of
    // the variable, and the name and text strings.
    class SMapping {
    public:
        // 0x32d088 _ZN6glitch8debugger10CTweakable8SMappingC1ENS_2io16E_ATTRIBUTE_TYPEEPv
        SMapping() = default;
        SMapping(int t, void* v) : type(t), value(v) {}
        // 0x320294 _ZN6glitch8debugger10CTweakable8SMappingaSERKS2_: member-wise copy, the defaulted assignment.

        int type = 0;            // +0x00
        void* value = nullptr;   // +0x04
        std::string name;        // +0x08
        std::string text;        // +0x20
    };

    // 0x32cfc8 _ZN6glitch8debugger10CTweakableC2EPNS_7IDeviceE: members start empty, the device pointer is stored.
    explicit CTweakable(glitch::IDevice* dev) : device(dev) {}

    // 0x32a088 (D1) and 0x404ab0 (D2) share one body; 0x32a124 (D0) is the deleting thunk. The original clears the map
    // and frees the group names and the xml string.
    virtual ~CTweakable() {
        mappings.clear();
        names.clear();
    }

    // +0x08 _ZNK6glitch8debugger10CTweakable15getInstanceNameEv: returns the instance name id at +0x68.
    virtual int getInstanceName() const { return instanceName; }

    // +0x0c _ZNK6glitch8debugger10CTweakable6getXMLEv. Partial: the original returns the address of the xml-name member
    // (+0x70) reinterpreted as char*; this returns the string itself.
    virtual const std::string& getXML() const { return xmlName; }

    // +0x10 _ZN6glitch8debugger10CTweakable11writeValuesEPNS_2io10IWriteFileE. Partial: the XML writer (writeElement,
    // writeClosingTag and the group walk) is not mapped, so nothing is written yet.
    virtual void writeValues(glitch::io::IWriteFile* writeFile) { (void)writeFile; }

    // +0x14 _ZN6glitch8debugger10CTweakable10onSetValueERKSs: empty in the original; derived classes override it.
    virtual void onSetValue(const std::string& name) { (void)name; }

    // 0x32bb98 _ZN6glitch8debugger10CTweakable20registerVariableNameEPKc: adds the name to the group list unless it is
    // already mapped.
    void registerVariableName(const char* name) {
        if (mappings.find(name) == mappings.end()) {
            names.emplace_back(name);
        }
    }

    // 0x32eeb8 _ZN6glitch8debugger10CTweakable16registerVariableEPKcRNS_5video6SColorE: maps the name to an SColor (kind 5).
    void registerVariable(const char* name, glitch::video::SColor& value) {
        registerVariableName(name);
        mappings[name] = SMapping(5, &value);
    }

    // 0x32ef4c _ZN6glitch8debugger10CTweakable16registerVariableEPKcRf: maps the name to a float (kind 1).
    void registerVariable(const char* name, float& value) {
        registerVariableName(name);
        mappings[name] = SMapping(1, &value);
    }

    // 0x32efe0 _ZN6glitch8debugger10CTweakable16registerVariableEPKcRi: maps the name to an int (kind 0).
    void registerVariable(const char* name, int& value) {
        registerVariableName(name);
        mappings[name] = SMapping(0, &value);
    }

    // 0x32b6b8 _ZN6glitch8debugger10CTweakable7loadXMLEPKcb. Partial: the file read and the XML attribute reader are not
    // mapped; the flow is open the file, read the attributes, store the root group name, load the group, drop the reader.
    void loadXML(const char* file, bool flag) {
        (void)file;
        (void)flag;
    }

    // +0x04 original std::map<std::string, SMapping>, keyed by the registered name.
    std::map<std::string, SMapping> mappings;  // +0x04
    // +0x1c original group name list (the push_back target of registerVariableName).
    std::vector<std::string> names;            // +0x1c
    uint8_t pad_28[0x2c];                      // +0x28 .. +0x53, not mapped
    // +0x54 root group name, assigned from the loaded XML.
    std::string rootGroup;                     // +0x54
    uint8_t pad_58[0x10];                      // +0x58 .. +0x67, not mapped
    // +0x68 instance name id, returned by getInstanceName.
    int instanceName = 0;                      // +0x68
    // +0x6c device pointer (glitch::IDevice* in the original).
    glitch::IDevice* device = nullptr;         // +0x6c
    // +0x70 xml file name, returned by getXML.
    std::string xmlName;                       // +0x70
    uint8_t pad_74[8];                         // +0x74 .. +0x7b, not mapped
};

}  // namespace glitch::debugger
