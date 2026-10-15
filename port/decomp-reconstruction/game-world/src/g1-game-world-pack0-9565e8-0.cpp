// Batch g1-game-world-pack0-9565e8-0: CheckpointZone (constructor, destructors, OnCollisionBegins), ColBox (destructors,
// IsUpdatable, InitPost, Draw, DeclareProperties) and Decor (destructors, IsUpdatable, LoadFloorMap, InitPost,
// DeclareProperties). Declarations come from dh2/CheckpointZone.hpp, dh2/ColBox.hpp and dh2/Decor.hpp. Each PARTIAL
// body names its seam in a comment. The compiler-generated deleting destructors (D0) are not written here.

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <set>

#include "dh2/Application.hpp"
#include "dh2/Character.hpp"
#include "dh2/CheckpointZone.hpp"
#include "dh2/ColBox.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/Decor.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/Level.hpp"
#include "dh2/ObjectHandle.hpp"
#include "dh2/PODecor.hpp"
#include "dh2/PhysicalObject.hpp"
#include "dh2/PlayerInfo.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/Point3D.hpp"
#include "dh2/PropertyMap.hpp"
#include "dh2/VisualObject.hpp"
#include "dh2/global/ctrl_camera.hpp"  // Application& ApplicationGetInstance()

// Global assert level (libDungeonHunter2.so gAssertLevel). Declared the same way as the other game-world sources.
extern int gAssertLevel;

namespace {

// The source path that the original's ASSERT messages print (CheckpointZone.cpp in the Visual Studio project).
const char kCheckpointZoneSource[] =
    "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Zones\\CheckpointZone.cpp";

// The original ASSERT. gAssertLevel 2 stops the program (the original stores to address 0 to crash), level 1 prints
// the message and the function goes on.
void ReportAssert(const char* expression, int line) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", expression, kCheckpointZoneSource, line);
    }
}

// ColBox::Draw. The debug box is lifted by 50 units on the Z axis (both Z corners) before it is drawn.
constexpr float kColBoxDrawLiftZ = 50.0f;  // 0x42480000

// ColBox::DeclareProperties. The default of each component of the "dimensions" box.
constexpr float kColBoxDefaultDimension = 200.0f;  // 0x43480000

}  // namespace

// ============================================================================================================
// CheckpointZone
// ============================================================================================================

// 0x39594c _ZN14CheckpointZoneC1EN10ObjectBase6GO_IDSE
// 0x3959b4 _ZN14CheckpointZoneC2EN10ObjectBase6GO_IDSE (same body)
// The original runs Zone(id, true, true) and then writes the vptrs (done by the compiler here), +0x388 .. +0x39f as an
// empty std::set (colour 0, root null, leftmost and rightmost pointing at the header, count 0), and the sensor flag
// byte. The std::set member's default constructor leaves the same empty set.
CheckpointZone::CheckpointZone(ObjectBase::GO_IDS id) : Zone(id, true, true) {}

// 0x395b38 _ZN14CheckpointZoneD1Ev
// 0x395bd0 _ZN14CheckpointZoneD2Ev (same body)
// The original resets the vptrs, erases the set's tree when the count is not zero, resets the header and then runs
// Zone::~Zone. The clear is written out. The member destructor would empty the same set after this body.
CheckpointZone::~CheckpointZone() {
    unk_388.clear();
}

// 0x395bb4 _ZN14CheckpointZoneD0Ev: skipped. This is the compiler-generated deleting destructor (D1, then
// CustomFree); a delete expression produces it.

// 0x395f04 _ZN14CheckpointZone17OnCollisionBeginsEP10GameObject
// PARTIAL. Rebuilt: the IsCharacter test, the handle conversion, the local player's character (PlayerInfo +0x660),
// the IsPlayer test, the m_position == character->m_savedPoint test (the other operand is Character +0x1468, the
// Point3D the header names m_savedPoint), the CheckpointSave call and the set insertion.
// The insertion is one std::set::insert. The original's inlined lower_bound and insert_unique pair does the same
// work: the key is inserted only when no equal key is in the tree, and the tree compares pointers with <.
// Missing (seams):
//   1. A null other. The original reports the assert, then loads other's vptr from address 0 and crashes (level 2
//      stores to address 0 first). The port reports the assert and aborts, which ends the process in the same way.
//   2. A null level (assert "l"). The original calls Level::CheckpointSave on a null this when the local player has
//      moved from the stored point. The port skips that call, and it still inserts the character as the original does.
void CheckpointZone::OnCollisionBegins(GameObject* other) {
    if (other == nullptr) {
        ReportAssert("go", 38);
        std::abort();  // the original crashes here: it reads other's vptr at address 0
    }
    if (!other->IsCharacter()) {
        return;
    }

    ObjectHandle handle = other->GetHandle();
    Character* character = handle;  // ObjectHandle::operator Character*
    Character* localCharacter = ApplicationGetInstance().m_playerManager->GetLocalPlayer(0, true)->unk_660;  // +0x660
    Level* level = ApplicationGetInstance().GetCurrentLevel();
    if (level == nullptr) {
        ReportAssert("l", 46);
    }

    if (character->IsPlayer() && character == localCharacter) {
        // The original calls the save when the zone's position is not the character's m_savedPoint (+0x1468).
        // The save is made at m_position (+0x160) with usePosition false.
        if (!(m_position == character->m_savedPoint)) {
            level->CheckpointSave(m_position, false);
        }
    }

    unk_388.insert(character);
}

// ============================================================================================================
// ColBox
// ============================================================================================================

// 0x340fe0 _ZN6ColBoxC1EN10ObjectBase6GO_IDSE
// Inlined into GetNewInstance<ColBox> (0x340fe0), so the binary has no standalone ColBox constructor symbol. The asm
// runs GameObject::GameObject(GO_IDS 20) (MOV R1,#0x14 before the BL), stores the primary vptr at +0x00, the third
// vptr at +0x04 and the second at +0x24, sets m_static (+0x84) to 1 and zeroes the three words at +0x374, +0x378 and
// +0x37c, which is the Point3D<float> at +0x374. The vptr stores and the base call are what the C++ constructor emits.
ColBox::ColBox(GO_IDS id) : GameObject(id), m_dimensions{} {}

// 0x3883f4 _ZNK6ColBox11IsUpdatableEv
bool ColBox::IsUpdatable() const {
    return false;
}

// 0x3884b8 _ZN6ColBoxD1Ev
// The original resets the vptrs and runs GameObject::~GameObject. The base destructor runs after this empty body.
ColBox::~ColBox() {}

// 0x388e60 _ZN6ColBoxD0Ev: skipped. This is the compiler-generated deleting destructor (D1, then CustomFree).

// 0x3884fc _ZN6ColBox8InitPostEv
// PARTIAL. Rebuilt: GameObject::InitPost, the scaling of the dimensions by the scale (+0x374 by +0x120, +0x378 by
// +0x124, +0x37c by +0x128), the relative box (+0x144 .. +0x158 from the half dimensions, minimum negated), the
// absolute AABB update, the PhysicalObject with the Application's PhysicalWorld (Application +0x44) and the
// SetPhysicalObject(body, false) hand-off.
// Missing: the original stores _ZTV8POColmap+8 (0x964008) into the new object's vptr, so the body is a POColmap. The
// POColmap constructor is not recovered (it is not in the queue) and POColmap adds no members or virtuals beyond its
// destructors. The port keeps PhysicalObject's vptr. Pending the POColmap constructor, the body is a PhysicalObject.
void ColBox::InitPost() {
    GameObject::InitPost();

    m_dimensions.X = m_dimensions.X * m_scale.X;  // +0x374 *= +0x120
    m_dimensions.Y = m_dimensions.Y * m_scale.Y;  // +0x378 *= +0x124
    m_dimensions.Z = m_dimensions.Z * m_scale.Z;  // +0x37c *= +0x128

    const float halfX = m_dimensions.X * 0.5f;
    const float halfY = m_dimensions.Y * 0.5f;
    const float halfZ = m_dimensions.Z * 0.5f;
    m_relAabb[0] = -halfX;  // +0x144 minimum X
    m_relAabb[1] = -halfY;  // +0x148 minimum Y
    m_relAabb[2] = -halfZ;  // +0x14c minimum Z
    m_relAabb[3] = halfX;   // +0x150 maximum X
    m_relAabb[4] = halfY;   // +0x154 maximum Y
    m_relAabb[5] = halfZ;   // +0x158 maximum Z

    GameObject::UpdateAbsoluteAABB();

    PhysicalWorld* world = ApplicationGetInstance().m_physicalWorld;  // Application +0x44
    // Original: operator new(0x28), then PhysicalObject(world, this, isStatic true, isCircle false, isSensor false,
    // isBullet false, group 0, category 1, mask 0xffff, unused 1), then the POColmap vptr (see the note above).
    PhysicalObject* body = new PhysicalObject(world, this, true, false, false, false, 0, 1, 0xFFFF, 1);
    GameObject::SetPhysicalObject(body, false);
}

// 0x3896f0 _ZNK6ColBox4DrawEv
// PARTIAL. Rebuilt: the DebugSwitches "isDisplayingColBox" test and the box the original passes to the driver. The draw
// itself is a seam. The original takes the driver from the device (dword_99F73C, the Application +0x10 IDevice, then
// IDevice +0x10), takes the material renderer manager at driver +0xdc, gets a material instance (creating the renderer
// when the id at +0x2e is 0xffff), calls IVideoDriver::setMaterial (technique 255 when there is no material), and then
// calls the driver's vtable slot +0x28 (IVideoDriver::draw3DBox) with the box and colour 0xFF0000FF. IDevice declares no
// +0x10, IVideoDriver declares no +0xdc and no draw3DBox, so none of it can be called from here.
void ColBox::Draw() const {
    DebugSwitches::s_inst.load();
    if (!DebugSwitches::s_inst.GetSwitch("isDisplayingColBox")) {
        return;
    }

    // The box is the absolute AABB with the Z corners lifted: minimum (x, y, z + 50), maximum (x, y, z + 50).
    const float box[6] = {
        m_absAabb[0], m_absAabb[1], m_absAabb[2] + kColBoxDrawLiftZ,
        m_absAabb[3], m_absAabb[4], m_absAabb[5] + kColBoxDrawLiftZ,
    };
    // SEAM: the material setup and the driver draw3DBox(box, 0xFF0000FF), see the note above.
    (void)box;
}

// 0x389938 _ZN6ColBox17DeclarePropertiesEv
void ColBox::DeclareProperties() {
    GameObject::DeclareProperties();
    PropertyMap::AddProperty<Point3D<float>>(
        "dimensions", m_dimensions,
        Point3D<float>{kColBoxDefaultDimension, kColBoxDefaultDimension, kColBoxDefaultDimension});
}

// ============================================================================================================
// Decor
// ============================================================================================================

// 0x3410fc _ZN5DecorC1EN10ObjectBase6GO_IDSE
// Inlined into GetNewInstance<Decor> (0x3410fc), so the binary has no standalone Decor constructor symbol. The asm runs
// GameObject::GameObject(GO_IDS 20) (MOV R1,#0x14 before the BL), stores the primary vptr at +0x00, the third at +0x04
// and the second at +0x24, and sets m_static (+0x84), m_floorMapPending (+0x375) and m_isSolid (+0x376) to 1. The vptr
// stores and the base call are compiler-generated; the three flags are the whole body. Module (0x389fa8) inlines these
// same three flag stores, so its Decor(id) base call compiles to this definition.
Decor::Decor(GO_IDS id) : GameObject(id) {
    m_static = true;           // ObjectBase +0x84
    m_floorMapPending = true;  // +0x375
    m_isSolid = true;          // +0x376
}

// 0x3883d4 _ZNK5Decor11IsUpdatableEv
bool Decor::IsUpdatable() const {
    return false;
}

// 0x388428 _ZN5DecorD1Ev
// The original resets the vptrs and runs GameObject::~GameObject. The base destructor runs after this empty body.
Decor::~Decor() {}

// 0x388eb0 _ZN5DecorD0Ev: skipped. This is the compiler-generated deleting destructor (D1, then CustomFree).

// 0x388730 _ZN5Decor12LoadFloorMapEv
// PARTIAL. Rebuilt: the guard (a visual object and the pending flag at +0x375), and the clear of the pending flag once
// the guard passes. Missing (seams):
//   1. PFWorld::LoadRoom(node, unk_64, name). The node is the visual's root (VisualObject +0x08) and the flags are
//      unk_64 (-1 by default). The name is the word at ObjectBase +0x44 (asm LDR R3,[R4,#0x44]), which lies inside
//      m_name as the header declares it (+0x30 .. +0x47), so the layout does not say what pointer is there.
//   2. The room's flags (PFRoom +0x24, bit 0 set from Decor::m_isSolid) and PFRoom::ExtendBoundingBox(absolute AABB).
//      The bounding box argument is an aabb<float>, which no header defines (GameObject::m_absAabb is float[6]).
void Decor::LoadFloorMap() {
    if (m_visualObject == nullptr || !m_floorMapPending) {
        return;
    }
    // SEAM: the room load and the flag and bounding box updates, see the note above.
    m_floorMapPending = false;  // +0x375, cleared on every pass through the guard
}

// 0x388a98 _ZN5Decor8InitPostEv
// Rebuilt in full. The original returns without LoadFloorMap when the visual object is null.
void Decor::InitPost() {
    GameObject::InitPost();
    if (m_visualObject == nullptr) {
        return;
    }

    m_visualObject->ApplyMeshBox();
    if (m_visualObject->m_hasColBox) {
        PhysicalWorld* world = ApplicationGetInstance().m_physicalWorld;  // Application +0x44
        // The original passes a bool register that is left over from the operator new call. PODecor never reads it
        // (PODecor.hpp), so false is passed here.
        PODecor* body = new PODecor(world, this, false);
        GameObject::SetPhysicalObject(body, false);
    }

    LoadFloorMap();
}

// 0x3899d0 _ZN5Decor17DeclarePropertiesEv
void Decor::DeclareProperties() {
    GameObject::DeclareProperties();
    PropertyMap::AddProperty<bool>("is_solid", m_isSolid, true);
}
