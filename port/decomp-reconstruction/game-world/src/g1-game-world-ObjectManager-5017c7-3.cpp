// ObjectManager, part 4 of 4 (batch g1-game-world-ObjectManager-5017c7-3).
// Reconstructed from the Hex-Rays pseudocode and the disassembly of libDungeonHunter2.so. Offsets in comments are the
// original 32-bit ones.
//
// Spawn is complete. The 5-argument LoadFromXML is partial: everything after its null-element ASSERT needs
// TiXmlElement::Attribute, which tinyxml-and-c-libs/include/tinyxml-and-c-libs/tixml_dh.hpp does not declare. The
// 2-argument LoadFromXML forwards exactly as the original does, but it is partial as well, because the int that the
// original returns cannot be carried (the header declares void; see the ledger rows).

#include <cstdio>
#include <cstdlib>
#include <string>

#include "dh2/ObjectBase.hpp"
#include "dh2/ObjectHandle.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/Point3D.hpp"

// The global assert level (gAssertLevel at 0x99f914). It is declared here as in the other ObjectManager parts.
extern int gAssertLevel;

namespace {

// The source path the original's ASSERT messages print (ObjectManager.cpp in the Visual Studio project).
const char kObjectManagerSource[] =
    "..\\..\\project_vs2005\\Game/..\\..\\sources\\Core\\ObjectManager\\ObjectManager.cpp";

// The original ASSERT(expression) at line. Level 2 stops the program (the original stores to address 0 to crash);
// level 1 prints the message and the function goes on.
void ReportAssert(const char* expression, int line) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", expression, kObjectManagerSource, line);
    }
}

}  // namespace

// 0x34b724 ObjectManager::Spawn(char const*,char const*,bool,bool)
// The original returns its own return slot, the ObjectHandle, which this version returns by value.
// Disassembly: GetNewObject(type, name, -1, silent) is called with flags -1 and the fourth parameter as its active flag.
// Then, when the handle resolves:
//   InitProperties and LoadDefaultProperties on the object's PropertyMap (+0x04), SetName(name),
//   m_gameType (+0x48) = type,
//   when the third parameter is false: InitPost() (vtable +0x1c), then TestEnableCondition(true),
//   when IsUpdatable() (vtable +0x38) is true: the object is appended to m_spawnList (+0x34).
// The parameter names follow the header. The disassembly confirms only the positions: the third parameter gates the
// init pair and the fourth feeds GetNewObject's active flag. Whether those names are right is not confirmed.
// Objects are created through GetNewObject (0x34b520), which is partial, so a null handle comes back until it is written.
ObjectHandle ObjectManager::Spawn(char const* type, char const* name, bool active, bool silent) {
    ObjectHandle handle = GetNewObject(type, name, -1, silent);
    if (handle.GetObject(false) != nullptr) {
        handle.GetObject(true)->PropertyMap::InitProperties();
        handle.GetObject(true)->PropertyMap::LoadDefaultProperties();
        handle.GetObject(true)->SetName(name);
        handle.GetObject(true)->m_gameType.assign(type);
        if (!active) {
            handle.GetObject(true)->InitPost();
            handle.GetObject(true)->TestEnableCondition(true);
        }
        if (handle.GetObject(true)->IsUpdatable()) {
            m_spawnList.push_back(handle.GetObject(false));
        }
    }
    return handle;
}

// 0x34b868 ObjectManager::LoadFromXML(TiXmlElement*,char const*,Point3D<float> const&,int)
// PARTIAL. Written: the null-element ASSERT (line 524), which returns after the report.
// Not written: the rest of the body. It reads the attributes "gametype", "name" and "template" through
// TiXmlElement::Attribute, which tixml_dh.hpp does not declare (it has Value() only). The disassembly gives the rest:
//   1. gametype and name must both be non-null, and a non-null type filter must be equal to gametype (strcmp); otherwise return.
//   2. name "_prim_PlayerLight" becomes "<name>_S" with flags -1. The other sprintf results in the original are dead.
//   3. handle = GetNewObject(gametype, name, flags, true). The active flag is the constant 1 in the disassembly, and
//      Hex-Rays drops it from the call.
//   4. A null object returns. Otherwise: InitProperties; SetTemplate(template) when template is set; LoadDefaultProperties;
//      LoadOverridesFromXML(element); when gametype is "LevelConfig", InitPost().
//   5. When IsGameObject() (vtable +0x20) is true: the new position is the object's m_position (+0x160) plus pos,
//      component by component, and SetPosition(position, true).
// The original returns an int: strcmp's result on a type mismatch, the name pointer when gametype is missing, 0 on the
// other early exits, and what SetPosition leaves in r0 on success. This header declares void, so the value is not
// reproduced. Level::_LoadFromXML reads it (see the Level-7c7f5d-1 note).
void ObjectManager::LoadFromXML(TiXmlElement* element, char const* type, Point3D<float> const& pos, int flags) {
    if (element == nullptr) {
        ReportAssert("element", 524);
        return;
    }
    // NOT WRITTEN: see the row note.
    (void)type;
    (void)pos;
    (void)flags;
}

// 0x34bbe4 ObjectManager::LoadFromXML(TiXmlElement*,char const*)
// PARTIAL. The original zeroes a Point3D (memset of 12 bytes) and calls the 5-argument overload with flags -1, and this
// version does the same. The int return of the 5-argument overload is not carried (see 0x34b868).
void ObjectManager::LoadFromXML(TiXmlElement* element, char const* type) {
    Point3D<float> origin{0.0f, 0.0f, 0.0f};  // memset(v4, 0, 12)
    LoadFromXML(element, type, origin, -1);
}
