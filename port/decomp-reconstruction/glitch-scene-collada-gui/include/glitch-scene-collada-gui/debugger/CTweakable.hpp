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
// State of the rows that touch this class (rechecked against the listing):
//  * setValue (0x32b3b4) and loadGroup (0x32b650): the reason recorded earlier ("CAttributes not mapped") was
//    STALE - glitch-core-io's CAttributes (io/attributes.hpp, vtable slots verified against IAttributes) has every
//    getter and group call the listings use, so both bodies are written below and are `reconstructed`.
//  * writeValues (0x328994): its own body is now fully readable (CXMLWriter ctor, writeXMLHeader, an open
//    L"attributes" element with no attribute pairs, a line break, the group walk, the closing tag, a line break),
//    but the group walk it calls - writeGroup (0x327be0) - is still unwritten, so the row stays `partial` and the
//    stub below also stays (writing the call would only reach a link stub).
//  * writeGroup (0x327be0): stays `skipped`. Its argument types are recovered (the mangled symbol) and its shape is
//    known (per variable one empty element <typeString n="name" value="value" range=".." desc=".."/>, built from
//    nine inlined CStringAttribute/CNumbersAttribute temporaries), but the listing does not expose the attribute
//    temporaries' construction arguments, so the body is not written. See the ledger note.
//  * loadXML (0x32b6b8): the recorded blocker was also STALE (CReadFile, CXMLAttributesReader and CAttributes all
//    exist). Its real blocker is in glitch-core-io/os/device.hpp: IDevice's file system member (original +0x34) is
//    declared `FileSystemWin32*`, which has no vtable, while the listing calls its IFileSystem slot +0x50
//    (createXMLReader(IReadFile*)). The member needs to be the glitch::io::IFileSystem the device constructor gets
//    from CIrrFactory::createFileSystem (0x533fb0). Everything else the body needs is present.
//  * SGroup::~SGroup (0x329d64): the group record's members are recovered from the listing (see the class below);
//    the defaulted member-wise destructor is defined out of line in src/recon/slice_00.cpp so that the original
//    symbol exists, and it makes exactly the three calls the listing shows.
#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "glitch-shared/glitch_core.hpp"  // glitch::video::SColor

namespace glitch {
class IDevice;  // glitch-core-io/include/glitch-core-io/os/device.hpp
namespace io {
class CAttributes;  // glitch-core-io/include/glitch-core-io/io/attributes.hpp
class CXMLWriter;   // glitch-core-io/include/glitch-core-io/io/xml_writer.hpp
class IWriteFile;   // glitch-core-io/include/glitch-core-io/io/interfaces.hpp
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

    // glitch::debugger::CTweakable::SGroup: the group record (0x34 bytes in the original). Layout from the
    // destructor 0x329d64, which runs the three member destructors in reverse declaration order:
    //   _ZNSt6vectorIN6glitch8debugger10CTweakable6SGroupESaIS3_EED1Ev at this+9 dwords = +0x24
    //   _ZNSt6vectorISsSaISsEED1Ev at this+6 dwords = +0x18
    //   _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv at this+0x00 (the string member)
    // so the declaration order is string, vector<std::string>, vector<SGroup>. writeGroup's child loop advances
    // 52 (0x34) bytes per element, which is the evidence for the trailing 4-byte member that the destructor never
    // touches; the CTweakable constructor (0x32cfc8) zeroes it among the eight dwords at +0x34 .. +0x53.
    // The two vectors are the plain-allocator STLport vectors (SaISs_/SaIS3_), unlike the SAllocator ones elsewhere.
    class SGroup {
    public:
        // 0x329d64 _ZN6glitch8debugger10CTweakable6SGroupD1Ev. Defaulted and defined out of line in
        // src/recon/slice_00.cpp: the compiler's member-wise destructor is exactly the listing's three calls.
        ~SGroup();

        std::string name;                        // +0x00, the group name (writeGroup writes it as the element name)
        std::vector<std::string> variableNames;  // +0x18, the group's registered variable names
        std::vector<SGroup> groups;              // +0x24, the child groups writeGroup recurses into
        std::uint32_t unk_30 = 0;                // +0x30, zeroed by the constructor and not destroyed
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

    // +0x0c _ZNK6glitch8debugger10CTweakable6getXMLEv. Verified against Hex-Rays 0x31f418: the original body is one
    // instruction, `return this + 112`, i.e. the ADDRESS of the xml-name member at +0x70 as a char*, which is the
    // address of the std::string object rather than its text. The mangled symbol carries no return type, so this
    // signature is symbol-compatible; returning the string itself is a deliberate port deviation (a pointer into a
    // std::string is not a usable interface) and is the only deviation on this row.
    virtual const std::string& getXML() const { return xmlName; }
    // Not an original virtual: the C-string form of the same member, for the callers that compare it with
    // LC_API_STRCMP (the material-renderer path). Non-virtual, so no symbol is involved.
    const char* getXMLString() const { return xmlName.c_str(); }

    // +0x10 _ZN6glitch8debugger10CTweakable11writeValuesEPNS_2io10IWriteFileE.
    // Verified against Hex-Rays 0x328994: construct CXMLWriter over the write file, writeXMLHeader(), an OPEN
    // (empty = false) L"attributes" element with no attribute pairs, writeLineBreak(), the group walk
    // writeGroup(this, group, writer, false), writeClosingTag(L"attributes"), writeLineBreak(), then the writer's
    // destructor. Partial: the group walk it calls, writeGroup (0x327be0), has no body in the tree yet, so the
    // call is not emitted here (it would only reach a link stub). Nothing else on this row is missing.
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

    // 0x327be0 _ZN6glitch8debugger10CTweakable10writeGroupERNS1_6SGroupERNS_2io10CXMLWriterEb. The signature is
    // recovered from the mangled symbol (SGroup&, glitch::io::CXMLWriter&, bool); the row is still `skipped` and
    // no body is written, so no symbol is emitted. What the listing does show: an optional leading group element
    // (flag set: the element name L"group" and a line break), then for every name of the group's list one empty
    // element built from an attribute temporary - the 13-argument writeElement(typeString, true, L"n", <name>,
    // L"value", <value>, L"range", <range>, L"desc", <desc>, nullptr, nullptr) whose two trailing pairs are null -
    // then the recursion writeGroup(this, child, writer, true) over the child groups, then the closing tag and a
    // line break when the flag is set. The nine attribute temporaries (int, float, wstring, bool, SColor, SColorf,
    // vector2d/3d/4d) are constructed inline with their default values and the switch writes the mapped value into
    // the chosen one; the listing does not expose those constructor arguments, which is why the body is not written.
    void writeGroup(SGroup& group, glitch::io::CXMLWriter& writer, bool flag);

    // 0x32b3b4 _ZN6glitch8debugger10CTweakable8setValueERNS_2io11CAttributesEi. Verified against Hex-Rays
    // 0x32b3b4: getAttributeName(index) (slot +0x10) names the attribute, the mapping with that name is looked up
    // in the map, and the ten-way switch on the mapping's type writes the attribute's typed value through the
    // mapped pointer - getInt(+0x5c), getFloat(+0x74), getString(+0x8c), getBool(+0xe8), getColor(+0x128),
    // getColorf(+0x140), getVector2d(+0x1a0), getVector3d(+0x1b8), getVector4d(+0x1d0). The onSetValue virtual
    // (slot +0x14) is then called with the mapping's key name, also when the type matched no case; an unmapped name
    // returns without a call. Deviations: a null name returns early (the original feeds it to the STLport map
    // lookup, which dereferences it), and the original's return value - the value the find or onSetValue left in
    // R0, read by no caller - is not modelled. Both are host-necessitated and observable-equivalent.
    // Defined out of line in src/recon/slice_00.cpp so the mangled symbol exists even when nothing calls it.
    void setValue(glitch::io::CAttributes& attributes, int index);

    // 0x32b650 _ZN6glitch8debugger10CTweakable9loadGroupERNS_2io11CAttributesEb.clone.25. Verified against
    // Hex-Rays 0x32b650: the group count (slot +0x40, read once) drives a do/while over the group indices; each
    // index is entered with setGroup(i) (slot +0x44), the recursion runs on the same container, and pop() (slot
    // +0x3c) leaves it. The bool is unused in this clone and this pointer is never dereferenced. Deviation: the
    // listing returns the value pop() left in R0, but the port's IAttributes::pop() returns void and every caller
    // of loadGroup discards the result, so this returns 0; the source cannot spell the .clone.25 suffix either.
    // Defined out of line in src/recon/slice_00.cpp so the mangled symbol exists even when nothing calls it.
    int loadGroup(glitch::io::CAttributes& attributes, bool flag);

    // 0x32b6b8 _ZN6glitch8debugger10CTweakable7loadXMLEPKcb. The flow is recovered from the listing (verified
    // against the assembly of 0x32b6b8): CReadFile(file, false); only when the file opened (its handle at +0x0c is
    // non-null), take the wide XML reader from the device's file system (device +0x34, vtable slot +0x50 =
    // createXMLReader(IReadFile*)), build a CXMLAttributesReader over it, build a CAttributes on the device's video
    // driver (device +0x10), read the document into it, pop the group stack until getGroupName() is empty, read the
    // string attribute L"Tweakable" and assign it to rootGroup (+0x54) when it is not empty, run loadGroup, drop the
    // reader. Partial, and the only missing piece is the type of that file-system member: glitch-core-io's
    // os/device.hpp declares it `FileSystemWin32*`, which has no vtable and no createXMLReader slot, while the
    // listing proves the original stores the glitch::io::IFileSystem returned by CIrrFactory::createFileSystem
    // (0x533fb0). Retyping IDevice's +0x34 member reconstructs this body; every other type it needs exists.
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
