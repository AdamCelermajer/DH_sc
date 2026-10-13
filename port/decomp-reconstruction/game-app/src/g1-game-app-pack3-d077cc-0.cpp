// Batch g1-game-app-pack3-d077cc-0 (POProjectile, POZone; POVillager and PlayerLightTweaker are deferred, see the end).
// Rebuilt from the Hex-Rays pseudocode and checked against the assembly (tools/decomp_asm.py).
// The collision callbacks reach the owning GameObject through its handle, as the originals do: ObjectBase::GetHandle
// on the owner, then ObjectHandle::GetObject or operator GameObject*. The Hex-Rays signatures drop some arguments
// (the `this` of GetHandle, and the other body of POZone::onCollisionBegins), so the argument use below follows the asm.
#include "dh2/POProjectile.hpp"
#include "dh2/POZone.hpp"

#include "core-platform/deps/game_types.hpp"  // Point2D, Point3D
#include "dh2/GameObject.hpp"
#include "dh2/ObjectHandle.hpp"
#include "dh2/Projectile.hpp"
#include "dh2/PlayerLightTweaker.hpp"
#include "dh2/Zone.hpp"

namespace {

// The GameObject that owns body, resolved through the owner's handle (ObjectBase::GetHandle, then operator
// GameObject*). The original does not test body or its owner for null first, and neither does this.
GameObject* ResolveOwner(PhysicalBaseObject* body) {
    ObjectHandle handle = body->m_owner->GetHandle();
    return handle;
}

}  // namespace

// 0x47046c POProjectile::onCollisionPersists(PhysicalBaseObject*, Point2D<float> const&, bool)
// The asm loads slot +0x0c of the vtable (onCollisionBegins) and calls it with r1..r3 unchanged, so the
// arguments pass through as they are. Hex-Rays shows only a1 because the call is a plain pass-through.
void POProjectile::onCollisionPersists(PhysicalBaseObject* other, const Point2D<float>& point, bool flag) {
    onCollisionBegins(other, point, flag);
}

// 0x470480 POProjectile::~POProjectile() (D1, complete-object destructor)
// The original stores the POProjectile vtable and calls PhysicalObject's base destructor (asm: BL
// PhysicalObject::~PhysicalObject, the D2 form). The empty body gives the same two steps.
POProjectile::~POProjectile() {}

// 0x4704b4 POProjectile::onCollisionBegins(PhysicalBaseObject*, Point2D<float> const&, bool)
// The owner of this body (m_owner, +0x08) is looked up through its handle. When its GO id (+0xf4, m_goId) is 9,
// which is Projectile (GO_IDS 9 in global/object_factory.hpp), and the other body is set, the owner's slot +0xd0
// is called: Projectile::OnCollision(the other body's owner, point). The flag is not read (r3 is unused in the asm).
void POProjectile::onCollisionBegins(PhysicalBaseObject* other, const Point2D<float>& point, bool /*flag*/) {
    ObjectHandle handle = m_owner->GetHandle();
    ObjectBase* object = handle.GetObject(false);
    if (object != nullptr && static_cast<int>(object->m_goId) == 9) {
        if (other != nullptr) {
            static_cast<Projectile*>(object)->OnCollision(other->m_owner, point);
        }
    }
}

// 0x470520 POProjectile::onCollisionTest(PhysicalBaseObject*, short, ushort, ushort, short, ushort, ushort)
// Runs PhysicalObject::onCollisionTest with the same arguments. When it passes, the owner of this body is
// looked up through its handle and accepted when its GO id is 9 (Projectile) or 10 (LaserTypeProjectile, GO_IDS 10).
// For an accepted owner and a set other body, the owner's slot +0xd0 is called with the other body's owner and the
// owner's position (m_position X and Y, +0x160 and +0x164). Always returns false (0).
// LaserTypeProjectile.hpp now declares it as a Projectile subclass, so the id 10 call goes through the Projectile type:
// slot +0xd0 is the virtual OnCollision, which the asm calls with the same signature for both ids.
bool POProjectile::onCollisionTest(PhysicalBaseObject* other, short otherGroup, unsigned short otherCategory,
                                   unsigned short otherMask, short group, unsigned short category,
                                   unsigned short mask) {
    if (PhysicalObject::onCollisionTest(other, otherGroup, otherCategory, otherMask, group, category, mask)) {
        ObjectHandle handle = m_owner->GetHandle();
        ObjectBase* target = handle.GetObject(false);
        bool accepted = target != nullptr && static_cast<int>(target->m_goId) == 9;
        if (!accepted) {
            ObjectHandle handle10 = m_owner->GetHandle();
            target = handle10.GetObject(false);
            accepted = target != nullptr && static_cast<int>(target->m_goId) == 10;
        }
        if (accepted && other != nullptr) {
            const GameObject* game = static_cast<GameObject*>(target);
            const Point2D<float> position{game->m_position.X, game->m_position.Y};
            static_cast<Projectile*>(target)->OnCollision(other->m_owner, position);
        }
    }
    return false;
}

// 0x4706ec POProjectile::~POProjectile() (D0, deleting destructor)
// Skipped: compiler-generated. It runs the D1 body and then CustomFree(this) (asm 0x4706ec). A C++ delete of a
// POProjectile does the same, so no body is written.

// 0x470728 POZone::~POZone() (D1, complete-object destructor)
// Stores the POZone vtable and calls PhysicalObject's base destructor (asm: BL PhysicalObject D2). The empty body
// gives the same two steps.
POZone::~POZone() {}

// 0x47075c POZone::onCollisionEnds(PhysicalBaseObject*, Point2D<float> const&, bool)
// The other body's owner is resolved to a GameObject (asm: LDR R1,[R1,#8], then GetHandle and operator GameObject*).
// When the zone (this->m_owner, +0x08) and that object are both set, the zone's slot +0xd4 is called:
// Zone::OnCollisionEnds(object). The point and flag are not read.
void POZone::onCollisionEnds(PhysicalBaseObject* other, const Point2D<float>& /*point*/, bool /*flag*/) {
    Zone* zone = static_cast<Zone*>(m_owner);
    GameObject* object = ResolveOwner(other);
    if (object != nullptr && zone != nullptr) {
        zone->OnCollisionEnds(object);
    }
}

// 0x4707a8 POZone::onCollisionPersists(PhysicalBaseObject*, Point2D<float> const&, bool)
// Same lookup as onCollisionEnds. Calls the zone's slot +0xd0: Zone::OnCollisionPersists(object).
void POZone::onCollisionPersists(PhysicalBaseObject* other, const Point2D<float>& /*point*/, bool /*flag*/) {
    Zone* zone = static_cast<Zone*>(m_owner);
    GameObject* object = ResolveOwner(other);
    if (object != nullptr && zone != nullptr) {
        zone->OnCollisionPersists(object);
    }
}

// 0x4707f4 POZone::onCollisionBegins(PhysicalBaseObject*, Point2D<float> const&, bool)
// Same lookup as onCollisionEnds (the other body's owner, asm: LDR R1,[R1,#8]). Calls the zone's slot +0xcc:
// Zone::OnCollisionBegins(object).
void POZone::onCollisionBegins(PhysicalBaseObject* other, const Point2D<float>& /*point*/, bool /*flag*/) {
    Zone* zone = static_cast<Zone*>(m_owner);
    GameObject* object = ResolveOwner(other);
    if (object != nullptr && zone != nullptr) {
        zone->OnCollisionBegins(object);
    }
}

// 0x470840 POZone::onCollisionTest(PhysicalBaseObject*, short, ushort, ushort, short, ushort, ushort)
// Returns the PhysicalObject::onCollisionTest result. When it is true, the same lookup runs and the zone's slot +0xc8
// is called: Zone::OnCollision(object) when both the zone and the object are set.
bool POZone::onCollisionTest(PhysicalBaseObject* other, short otherGroup, unsigned short otherCategory,
                             unsigned short otherMask, short group, unsigned short category, unsigned short mask) {
    const bool passed =
        PhysicalObject::onCollisionTest(other, otherGroup, otherCategory, otherMask, group, category, mask);
    if (passed) {
        Zone* zone = static_cast<Zone*>(m_owner);
        GameObject* object = ResolveOwner(other);
        if (object != nullptr && zone != nullptr) {
            zone->OnCollision(object);
        }
    }
    return passed;
}

// 0x4709a0 POZone::~POZone() (D0, deleting destructor)
// Skipped: compiler-generated. It runs the D1 body and then CustomFree(this) (asm). A C++ delete of a POZone does
// the same, so no body is written.

// Deferred to the redo pass (batch r2a-game-app-pack1-e4281c-0, game-app/src/r2a-game-app-pack1-e4281c-0.cpp):
//   0x3b3824 POVillager::~POVillager() (D1) is written there now that dh2/POCharacter.hpp exists.
//   0x40dd7c PlayerLightTweaker::GetIdsFromName is written there now that the CTweakable base header exists.
// Still deferred here, and not compiled in this file:
//   0x3b43e0 POVillager::~POVillager() (D0): compiler-generated; its D1 is in the pack1 file.
//   0x32f2b4 PlayerLightTweaker::PlayerLightTweaker() and 0x40de48 onSetValue: not in this file.
//   0x40e518 ~PlayerLightTweaker() (D0): compiler-generated.

// 0x40e4c4 PlayerLightTweaker::~PlayerLightTweaker() (D1). The original resets the vptr, frees the heap block of the
// m_lightType string (+0xf8) when it is not the inline buffer, then runs ~CTweakable. The std::string member and the base
// destructor do the same here.
PlayerLightTweaker::~PlayerLightTweaker() {}
