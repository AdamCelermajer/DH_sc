// Batch g1-game-world-Zone-03efcc-0: the Zone bodies of Zone.cpp in the VS2005 project. Declarations come from
// dh2/Zone.hpp. A PARTIAL body names what it waits on.
//
// The physics body of a Zone (a POZone) is a PhysicalObject with the POZone vtable stored in it. dh2/POZone.hpp
// declares no constructor, so the physics branches of InitWithBoundingBox and InitPost are not written. The colzone
// branches (the "_colzone" mesh node) need glitch::scene::ISceneNode as a complete type and the scene manager, which
// are not in the tree yet.

#include <cstdio>
#include <cstdlib>

#include "dh2/VisualObject.hpp"
#include "dh2/Zone.hpp"

// Engine-wide assert level (core-platform/memory.hpp). That header is not included here: its glitch_core.hpp
// clashes with the one Zone.hpp uses, and the GameObject batches declare the variable the same way.
extern int gAssertLevel;

namespace {

// Source file named in the original's assert messages (Zone.cpp of the VS2005 project).
const char kZoneSourceFile[] = "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Zones\\Zone.cpp";

// The assert of the original Zone bodies. Level 2 stops the process: the original stores 0 to address 0 there, which
// faults, and std::abort() ends the process the same way. Level 1 prints the message to stderr (the original's
// &__sF + 2, the third standard stream) and returns, so the caller goes on.
void ZoneAssertFailed(const char* expr, int line) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", expr, kZoneSourceFile, line);
    }
}

}  // namespace

// 0x395940 Zone::OnCollision(GameObject*)
// Empty in the original (a single bx lr). Zone's collision virtuals are hooks for the subclasses.
void Zone::OnCollision(GameObject* other) {
    (void)other;
}

// 0x395944 Zone::OnCollisionPersists(GameObject*)
// Empty in the original (a single bx lr).
void Zone::OnCollisionPersists(GameObject* other) {
    (void)other;
}

// 0x395948 Zone::OnCollisionEnds(GameObject*)
// Empty in the original (a single bx lr).
void Zone::OnCollisionEnds(GameObject* other) {
    (void)other;
}

// 0x39649c Zone::OnCollisionBegins(GameObject*)
// Empty in the original (a single bx lr).
void Zone::OnCollisionBegins(GameObject* other) {
    (void)other;
}

// 0x3970c8 Zone::IsInside(GameObject*)
// PARTIAL: the box test (no colzone node) is rebuilt. The colzone branch is not. The original sweeps a segment from the
// other object's position through the colzone's triangle selector: it calls the node's virtual at vtable +0xb0 for the
// selector, then the scene collision manager stored at SceneManager +0x2c (its vtable slot +0x0c) for the hit test. It
// then runs the DisplayColZoneWhenColliding debug draw. None of those interfaces is declared in the tree, and the
// segment needs the Vec3f_K global (0x99f878). The colzone branch returns false here, so a colzone zone never reports
// an object inside.
bool Zone::IsInside(GameObject* other) {
    if (!other) {
        ZoneAssertFailed("go", 152);
        // The original then reads other->m_physicalObject through the null pointer and faults.
        std::abort();
    }
    if (!other->m_physicalObject) {  // +0x2dc
        return false;
    }
    if (m_colZoneNode) {  // +0x384
        return false;     // PARTIAL: see above.
    }
    // The original also builds a direction from this to other, scales it by PhysicalObject::getRadius() and never reads
    // the result. That work has no effect, so it is left out.
    const float* box = m_absAabb;          // +0x12c: the world AABB, min xyz then max xyz
    const float x = other->m_position.X;   // +0x160
    const float y = other->m_position.Y;   // +0x164
    const float z = other->m_position.Z;   // +0x168
    return box[0] <= x && x <= box[3] && box[1] <= y && y <= box[4] && box[2] <= z && z <= box[5];
}

// 0x397594 Zone::InitWithBoundingBox(glitch::core::aabbox3d<float> const&)
// PARTIAL: the box size, the local AABB and the centre are rebuilt. The physics body is not. For a Zone with physics
// (m_hasPhysics, +0x380) the original builds a POZone: a PhysicalObject of 40 bytes on Application::m_physicalWorld
// (+0x44) with owner this, static, sensor, not a circle, not a bullet, group -5, category 0x800 and mask 4 for a
// playerOnly zone or 1310 otherwise. Its vptr is then set to the POZone vtable (0x969018) and the body is passed to
// GameObject::SetPhysicalObject(body, false). POZone.hpp declares no constructor, so the body cannot be built.
// LAYOUT REQUEST (dh2/POZone.hpp): constructor POZone(PhysicalWorld*, GameObject*, bool, bool, bool, bool, short,
// unsigned short, unsigned short, int) that runs the PhysicalObject constructor and leaves the POZone vptr.
void Zone::InitWithBoundingBox(const glitch::core::aabbox3d<float>& box) {
    // +0x374 .. +0x37c: the size of the box.
    m_dimensions.X = box.MaxEdge.X - box.MinEdge.X;
    m_dimensions.Y = box.MaxEdge.Y - box.MinEdge.Y;
    m_dimensions.Z = box.MaxEdge.Z - box.MinEdge.Z;
    // +0x144 .. +0x158: the local AABB, min xyz then max xyz.
    m_relAabb[0] = box.MinEdge.X;
    m_relAabb[1] = box.MinEdge.Y;
    m_relAabb[2] = box.MinEdge.Z;
    m_relAabb[3] = box.MaxEdge.X;
    m_relAabb[4] = box.MaxEdge.Y;
    m_relAabb[5] = box.MaxEdge.Z;
    // The object is placed at the centre of the box, and that position is set as the destination too.
    const Point3D<float> centre{(box.MinEdge.X + box.MaxEdge.X) * 0.5f,
                                (box.MinEdge.Y + box.MaxEdge.Y) * 0.5f,
                                (box.MinEdge.Z + box.MaxEdge.Z) * 0.5f};
    GameObject::SetPosition(centre, true);
    if (m_hasPhysics) {
        // PARTIAL: the POZone body, see above.
    }
}

// 0x39771c Zone::InitPost()
// PARTIAL: the scaling, the local AABB and the SetRelativeAABB call are rebuilt, and the colzone node is looked up. Not
// rebuilt: (1) the POZone body of m_hasPhysics, see InitWithBoundingBox; (2) the rest of the colzone setup. The
// original raises the node's reference count (glitch::IReferenceCounted, not complete in the tree), searches the scene
// manager for the one mesh of the node (the scene manager is read from IDevice +0x1c, which is not declared), builds a
// CTriangleSelector from that mesh's triangle selector, and sets it on the node. It asserts the mesh count is 1 (Zone.cpp
// line 96) before that search. Without the search the node has no triangle selector.
void Zone::InitPost() {
    if (GameObject::CheckSpawnProbability() < m_spawnProb) {  // +0x274
        GameObject::InitPost();
        // +0x374 .. +0x37c: the dimensions take the object scale (+0x120 .. +0x128).
        m_dimensions.X *= m_scale.X;
        m_dimensions.Y *= m_scale.Y;
        m_dimensions.Z *= m_scale.Z;
        // The local AABB is centred on the origin: min is -half the size, max is +half the size.
        const float halfX = m_dimensions.X * 0.5f;
        const float halfY = m_dimensions.Y * 0.5f;
        const float halfZ = m_dimensions.Z * 0.5f;
        m_relAabb[0] = -halfX;
        m_relAabb[1] = -halfY;
        m_relAabb[2] = -halfZ;
        m_relAabb[3] = halfX;
        m_relAabb[4] = halfY;
        m_relAabb[5] = halfZ;
        // Virtual SetRelativeAABB(box, false) at vtable +0x9c. aabb<T> is not defined in the tree, so the box is passed
        // as the six floats in memory. GameObject::SetRelativeAABB reads them that way.
        SetRelativeAABB(*reinterpret_cast<const aabb<float>*>(m_relAabb), false);
        if (m_hasPhysics) {
            // PARTIAL: the POZone body, see InitWithBoundingBox.
        }
        if (!m_colZoneNode && m_visualObject) {  // +0x384, +0x2d8
            m_colZoneNode = m_visualObject->GetSpecificNode("_colzone");
            if (m_colZoneNode) {
                // PARTIAL: the reference count, the mesh search and the triangle selector, see above.
            }
        }
    }
}

// 0x397b38 Zone::~Zone()   (complete object destructor, D1)
// 0x397bc4 Zone::~Zone()   (base object destructor, D2; the same code)
// PARTIAL: the original drops m_colZoneNode when it is set, through glitch::IReferenceCounted (the node's base, at the
// offset stored at vtable -0x0c), then clears the pointer. glitch::scene::ISceneNode is not a complete type in the
// tree, so the drop is not made. The compiler runs the vptr resets and the GameObject base destructor, as the original
// does through GameObject::~GameObject.
Zone::~Zone() {}

// 0x397ba8 Zone::~Zone()   (deleting destructor, D0)
// skipped: the compiler generates the deleting destructor (~Zone() followed by CustomFree(this)). A delete expression
// on a Zone does the same, so there is no separate body.

// 0x397c2c Zone::Zone(ObjectBase::GO_IDS, bool, bool)   (complete object constructor, C1)
// 0x397ca0 Zone::Zone(ObjectBase::GO_IDS, bool, bool)   (base object constructor, C2; the same code)
// The GameObject base takes the id (the original passes it on in r1). Then the size is zeroed, the two flags are
// stored, the colzone node pointer is null, and ObjectBase::m_static (+0x84) is set to 1.
Zone::Zone(ObjectBase::GO_IDS id, bool hasPhysics, bool playerOnly)
    : GameObject(id),
      m_dimensions{},
      m_hasPhysics(hasPhysics),
      m_playerOnly(playerOnly),
      m_colZoneNode(nullptr) {
    m_static = true;  // ObjectBase +0x84
}

// 0x397df8 Zone::DeclareProperties()
// GameObject's properties first, then "dimensions": a Point3D<float> with default (200, 200, 200) at the member
// m_dimensions (the original's property offset 880, relative to the PropertyMap base at +4, is +0x374).
// The default is the single word 0x43480000 (200.0f), written to the property's three default slots at +0x20, +0x24
// and +0x28 (MOV R3, #0x43480000 then three STRs). 100.0f would be 0x42C80000.
void Zone::DeclareProperties() {
    GameObject::DeclareProperties();
    AddProperty<Point3D<float>>("dimensions", m_dimensions, Point3D<float>{200.0f, 200.0f, 200.0f});
}
