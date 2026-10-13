// Batch g1-game-world-Projectile-6685f5-0: the Projectile class (flight, impacts and spawn setup).
// Reconstructed from the Hex-Rays pseudocode of libDungeonHunter2.so. Where the pseudocode drops a register
// argument (Update, OnExpire, OnCollision, HandleImpactFX), the value comes from the assembly. Offsets in
// comments are the original 32-bit ones. Partial functions name the blocked call in their comment; the ledger
// notes repeat the reason.

#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "dh2/AnimController.hpp"
#include "dh2/Application.hpp"
#include "dh2/CharAI.hpp"
#include "dh2/Character.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/ObjectHandle.hpp"
#include "dh2/PFFloor.hpp"
#include "dh2/PFObject.hpp"
#include "dh2/Projectile.hpp"
#include "dh2/ProjectileManager.hpp"
#include "dh2/VisualObject.hpp"
#include "dh2/global/ctrl_camera.hpp"
#include "glitch-scene-collada-gui/scene_node.hpp"
#include "pydata-accessors/structs_records.hpp"
#include "pydata-accessors/w7_arrays_00.hpp"
#include "pydata-accessors/w7_structs_5.hpp"

// Engine-wide assert level (core-platform/memory.hpp).
extern int gAssertLevel;

namespace {

const char kProjectileSource[] =
    "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Projectiles\\Projectile.cpp";

// The original ASSERT(expression). Level 2 stops the process (the original writes to address 0); level 1
// prints the message and carries on.
void ReportAssert(const char* expression, int line) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", expression, kProjectileSource, line);
    }
}

// Arrays::ProjectileTable row. The original rows are 72 bytes; the host elements are Structs::Projectile,
// reached through a typed pointer (pydata-accessors/element_table.hpp), never through byte arithmetic.
const Structs::Projectile& ProjectileRow(int tableIndex) {
    return reinterpret_cast<const Structs::Projectile*>(Arrays::ProjectileTable::members)[tableIndex];
}

// Arrays::ProjectileDict row (12 bytes in the original, Structs::ColladaFile here). +0x08 is the dae name.
const char* ProjectileDaeName(int dictIndex) {
    return reinterpret_cast<const Structs::ColladaFile*>(Arrays::ProjectileDict::members)[dictIndex].m_w2;
}

// The table columns one impact plays. -1 means the column is empty.
struct ImpactColumns {
    int fx;     // effect set id, played through VisualFXManager::PlayAnimFXSet
    int sound;  // 3D sound id, played through VoxSoundManager::Play3D
};

// Column choice of Projectile::HandleImpactFX(ImpactType, Point3D) (asm 0x3e4db0). A missing sound column (+0x30)
// falls back to the character columns (+0x14 effect, +0x18 sound) for both; a missing effect column (+0x2c)
// falls back to +0x14 only.
ImpactColumns SelectImpactColumns(const Structs::Projectile& row, Projectile::ImpactType impact) {
    switch (impact) {
        case Projectile::IMPACT_NONE:
            return ImpactColumns{-1, -1};
        case Projectile::IMPACT_EXPIRE:
            return ImpactColumns{row.m_w2, row.m_w3};  // +0x08 effect, +0x0c sound
        case Projectile::IMPACT_GROUND:
        case Projectile::IMPACT_OBJECT: {
            ImpactColumns columns{row.m_w11, row.m_w12};  // +0x2c effect, +0x30 sound
            if (columns.sound == -1) {
                columns = ImpactColumns{row.m_w5, row.m_w6};  // +0x14 effect, +0x18 sound
            } else if (columns.fx == -1) {
                columns.fx = row.m_w5;  // +0x14
            }
            return columns;
        }
        default:  // IMPACT_CHARACTER and any other value
            return ImpactColumns{row.m_w5, row.m_w6};  // +0x14 effect, +0x18 sound
    }
}

// The original's LABEL_8 in Update: the projectile is dead, no longer expiring, and goes back to its pool.
void DespawnProjectile(Projectile& projectile) {
    projectile.m_dead = true;                              // +0x3d0
    projectile.m_expiring = false;                         // +0x3d1
    projectile.m_manager->DeSpawn(&projectile, false);     // ProjectileManager::DeSpawn(this, false)
}

}  // namespace

// 0x3e3df4 Projectile::IsDead() const
bool Projectile::IsDead() const {
    return m_dead;
}

// 0x3e3dfc Projectile::IsValidatingFloorPosition() const
int Projectile::IsValidatingFloorPosition() const {
    return 2;
}

// 0x3e3e04 Projectile::IsZonable() const
bool Projectile::IsZonable() const {
    return false;
}

// 0x3e3e0c Projectile::IsUpdatable() const
bool Projectile::IsUpdatable() const {
    return true;
}

// 0x3e3e14 Projectile::IsAnimated() const
bool Projectile::IsAnimated() const {
    return true;
}

// 0x3e3e1c Projectile::IsInteractive(GameObject*) const
bool Projectile::IsInteractive(GameObject* /*other*/) const {
    return false;
}

// 0x3e4da8 Projectile::GetSpeed() const
// Returns the current speed (+0x3ac), decayed in Update.
float Projectile::GetSpeed() const {
    return m_speed;
}

// 0x3e4db0 Projectile::HandleImpactFX(Projectile::ImpactType, Point3D<float> const&)
// PARTIAL: the effect and the 3D sound are not played. The column choice is reconstructed. The effect call
// VisualFXManager::PlayAnimFXSet(id, position, nullptr, nullptr) needs a VisualFXManager instance accessor
// (Singleton<VisualFXManager>::s_inst), which dh2/VisualFXManager.hpp does not declare. The sound call
// VoxSoundManager::Play3D(id, position, ...) needs VoxSoundManager::s_instance and Play3D, which
// vox-audio/vox/sound_manager.hpp does not declare. See the layout requests.
void Projectile::HandleImpactFX(ImpactType impact, const Point3D<float>& position) {
    if (impact == IMPACT_NONE) {
        return;  // case 0 returns before the table is read
    }
    const ImpactColumns columns = SelectImpactColumns(ProjectileRow(m_projectileId), impact);
    if (columns.fx != -1) {
        // NOT MADE: VisualFXManager::PlayAnimFXSet(columns.fx, position, nullptr, nullptr)
    }
    if (columns.sound != -1) {
        // NOT MADE: VoxSoundManager::s_instance->Play3D(columns.sound, position, ...)
    }
    (void)position;
}

// 0x3e4eec Projectile::HandleImpactFX(GameObject*, Point2D<float> const&)
// A hit on a character plays the character impact at the object's target position. Any other object plays
// the object impact at the hit point's x and y with the object's target z.
void Projectile::HandleImpactFX(GameObject* hit, const Point2D<float>& position) {
    if (hit == nullptr) {
        ReportAssert("go", 338);
        // The original dereferences the null object right after the assert, so it always ends in a crash.
        std::abort();
    }
    if (hit->IsCharacter()) {
        HandleImpactFX(IMPACT_CHARACTER, hit->GetTargetPosition());
        return;
    }
    const Point3D<float> target = hit->GetTargetPosition();
    HandleImpactFX(IMPACT_OBJECT, Point3D<float>{position.X, position.Y, target.Z});
}

// 0x3e4fe4 Projectile::SetManager(ProjectileManager*)
void Projectile::SetManager(ProjectileManager* manager) {
    if (manager == nullptr) {
        ReportAssert("mgr", 68);
    }
    m_manager = manager;  // +0x378
}

// 0x3e5078 Projectile::OnExpire(Projectile::ImpactType)
// PARTIAL: the floor snap runs only when the object holds a PFFloor (PFObject +0x10). The room and world
// queries are not made: the room pointer the original reads at GameObject +0x1d4 is PFObject +0x0c, which
// PFObject.hpp declares as the float m_floorHeight, and Singleton<PFWorld>::s_inst has no accessor. Without a
// floor, the z of the position is left as it is. See the layout requests.
void Projectile::OnExpire(ImpactType impact) {
    if (ProjectileRow(m_projectileId).m_w13) {  // table +0x34: snap to the floor on expiry
        PFFloor* floor = m_pfObject.m_floor;     // PFObject +0x10 (GameObject +0x1d8)
        if (floor != nullptr) {
            float height = 0.0f;
            Point3D<float> normal{0.0f, 0.0f, 0.0f};
            floor->GetFloorHeightAt(m_position, &height, &normal);
            SetPosition(Point3D<float>{m_position.X, m_position.Y, height}, true);
        }
    }
    m_hitObject = nullptr;  // +0x3cc
    m_expiring = true;      // +0x3d1
    HandleImpactFX(impact, GetTargetPosition());
}

// 0x3e51b0 Projectile::Update()
// PARTIAL: the floor and room checks that decide whether a projectile still inside its range expires on terrain
// need the PFWorld instance (Singleton<PFWorld>::s_inst, no accessor in the tree). Until that exists, a
// projectile inside its range keeps flying here. Point3D::normalize is also not reachable from GameObject.hpp,
// so the destination direction is not normalized. The finish callback, the target expiry, the lifetime and
// speed decay, and the range test are reconstructed.
void Projectile::Update() {
    if (m_expiring) {
        const int finishResult = m_onFinish != nullptr ? m_onFinish(this, m_userData) : 0;
        if (m_hitObject == nullptr) {
            DespawnProjectile(*this);
            return;
        }
        if (finishResult == 0) {
            HandleImpactFX(m_hitObject, m_impactPosition);
            DespawnProjectile(*this);
            return;
        }
        if (finishResult == 1) {
            m_dead = false;      // +0x3d0
            m_expiring = false;  // +0x3d1
        }
    }

    if (m_target != nullptr && m_target->IsDead()) {  // target +0x34 IsDead()
        m_target = nullptr;                           // +0x384
    }
    if (m_dead) {
        return;
    }

    const Point3D<float> targetPosition = GetTargetPosition();
    m_position = targetPosition;  // +0x394..+0x3a0 copy of GetTargetPosition()

    const Structs::Projectile& row = ProjectileRow(m_projectileId);
    if (row.m_off53 && m_target != nullptr) {  // table +0x35: home on the target
        SetDestination(m_target->GetTargetPosition());
    } else {
        // PARTIAL: the original normalizes (destination - position) in the XY plane and places the
        // destination 1000 units along it. The direction is not normalized here (see the header comment).
        const Point3D<float> direction{m_destination.X - targetPosition.X, m_destination.Y - targetPosition.Y,
                                       0.0f};
        SetDestination(Point3D<float>{direction.X * 1000.0f + targetPosition.X,
                                      direction.Y * 1000.0f + targetPosition.Y,
                                      direction.Z * 1000.0f + targetPosition.Z});
    }

    GameObject::Update();

    const int dt = ApplicationGetInstance().GetDt();
    m_speed = m_speed + (m_deceleration * static_cast<float>(static_cast<unsigned int>(dt))) / -1000.0f;
    m_lifetimeMs = static_cast<int>(static_cast<unsigned int>(m_lifetimeMs) - static_cast<unsigned int>(dt));

    if (m_lifetimeMs <= 0 || m_speed <= 0.0f) {
        OnExpire(IMPACT_EXPIRE);
        return;
    }
    if (m_rangeSquared >= 0.0f) {
        const Point3D<float>& position = GetTargetPosition();
        const float dx = position.X - m_startPosition.X;
        const float dy = position.Y - m_startPosition.Y;
        const float dz = position.Z - m_startPosition.Z;
        const float distanceSquared = dx * dx + dy * dy + dz * dz;
        if (m_rangeSquared <= distanceSquared) {
            OnExpire(IMPACT_EXPIRE);
            return;
        }
    }
    // PARTIAL: the floor query (PFWorld::GetFloorHeightAt on the object position, then PFObject::CanPathOn,
    // the m_projectileNode and launch height test, or the room query when there is no floor) is not made.
    // The projectile keeps flying.
}

// 0x3e55b4 Projectile::OnCollision(GameObject*, Point2D<float> const&)
// Returns true when the hit was consumed (the projectile is now expiring).
bool Projectile::OnCollision(GameObject* other, const Point2D<float>& position) {
    if (m_dead || m_expiring) {
        return false;
    }
    if (other == nullptr || m_owner == other) {
        return false;
    }

    ObjectHandle otherHandle = other->GetHandle();
    Character* otherCharacter = otherHandle.operator Character*();
    if (otherCharacter == nullptr) {
        // Not a character: only a projectile without a homing target hits it.
        if (m_target != nullptr) {
            return false;
        }
    } else {
        const Structs::Projectile& row = ProjectileRow(m_projectileId);
        if (row.m_w1 && m_target != other) {  // table +0x04: a homing shot hits its target only
            return false;
        }
        if (!row.m_w4) {  // table +0x10: a character hit must be an enemy of the owner
            ObjectHandle ownerHandle = m_owner->GetHandle();
            Character* ownerCharacter = ownerHandle.operator Character*();
            if (ownerCharacter != nullptr && !ownerCharacter->AI_IsEnemy(other)) {
                return false;
            }
        }
    }

    m_hitObject = other;                                   // +0x3cc
    m_impactPosition = Point2D<float>{position.X, position.Y};  // +0x3c4

    if (m_onHit != nullptr) {
        const int result = m_onHit(this, m_userData);
        if (result == 2) {
            // Finish callback and impact effect, and the projectile keeps flying.
            if (m_onFinish != nullptr) {
                m_onFinish(this, m_userData);
            }
            HandleImpactFX(m_hitObject, m_impactPosition);
            return false;
        }
        if (result == 3) {
            OnExpire(IMPACT_EXPIRE);
        } else if (result == 1) {
            return false;
        }
    }
    m_expiring = true;  // +0x3d1
    return true;
}

// 0x3e57cc Projectile::SetInfo(int, GameObject*, GameObject*, Callback, Callback, void*, float)
// PARTIAL: the aim is not rotated by the angle. The original calls Point3D::rotateXY (0x3e5740) on the look vector,
// and that method is not reachable from GameObject.hpp (see the Point3D layout request). The destination is built
// from the unrotated look vector.
void Projectile::SetInfo(int projectileId, GameObject* owner, GameObject* target, Callback onHit,
                         Callback onFinish, void* userData, float angleDegrees) {
    SetInfo(projectileId, owner, target, onHit, onFinish, userData, false);  // virtual slot +0xc8

    Point3D<float> aim{0.0f, 0.0f, 0.0f};
    owner->GetLookAtVec(aim);
    // NOT MADE: aim.rotateXY(static_cast<float>(angleDegrees * 0.017453)), see the header comment.
    (void)angleDegrees;

    const Point3D<float> base = owner->GetTargetPosition();
    SetDestination(Point3D<float>{aim.X * 1000.0f + base.X, aim.Y * 1000.0f + base.Y, aim.Z * 1000.0f + base.Z});
    if (m_visualObject != nullptr) {  // +0x2d8
        m_visualObject->SyncRotation();
    }
}

// 0x3e58cc Projectile::SetInfo(int, GameObject*, GameObject*, Callback, Callback, void*, bool)
// PARTIAL in two places:
//   - The physics body is not created. The original builds a PhysicalObject on the PhysicalWorld (the global
//     dword_99F770, with no accessor in the tree) and gives it the POProjectile vtable (0x968fe8). POProjectile.hpp
//     declares no constructor for that, so SetPhysicalObject is not called and the projectile has no collision body.
//   - With a projectile node and useProjectileNode set, the aim direction is not normalized (Point3D::normalize is
//     not reachable from GameObject.hpp).
void Projectile::SetInfo(int projectileId, GameObject* owner, GameObject* target, Callback onHit,
                         Callback onFinish, void* userData, bool useProjectileNode) {
    if (projectileId < 0 || static_cast<std::uint32_t>(projectileId) >= Arrays::ProjectileTable::size) {
        ReportAssert("projectileId >= 0 && projectileId < (int)Arrays::ProjectileTable::size", 78);
    }
    if (owner == nullptr) {
        ReportAssert("owner", 79);
    }

    const Structs::Projectile& row = ProjectileRow(projectileId);
    m_projectileId = projectileId;             // +0x374
    unk_37c = row.m_off29;                     // +0x37c table +0x1d
    m_target = target;                         // +0x384
    m_onHit = onHit;                           // +0x3b8
    m_onFinish = onFinish;                     // +0x3bc
    m_owner = owner;                           // +0x380
    m_dead = false;                            // +0x3d0
    m_userData = userData;                     // +0x3c0
    m_expiring = false;                        // +0x3d1

    Point3D<float> aim{0.0f, 0.0f, 0.0f};
    owner->GetLookAtVec(aim);

    const Point3D<float> ownerPosition = owner->GetTargetPosition();
    m_startPosition = ownerPosition;                     // +0x388 owner's target position
    m_launchHeight = ownerPosition.Z;                    // +0x3a0
    m_projectileNode = nullptr;                          // +0x3a4
    m_rangeSquared = row.m_w8 >= 0.0f ? row.m_w8 * row.m_w8 : -1.0f;  // +0x3a8 table +0x20, squared

    VisualObject* ownerVisual = owner->m_visualObject;   // +0x2d8 of the owner
    if (ownerVisual != nullptr) {
        glitch::scene::ISceneNode* node = ownerVisual->GetSpecificNode("projectile_node");
        m_projectileNode = node;                         // +0x3a4
        if (node != nullptr) {
            const glitch::core::vector3d<float> absolute = node->getAbsolutePosition();
            m_startPosition = Point3D<float>{absolute.X, absolute.Y, absolute.Z};
            m_launchHeight = absolute.Z;
        }
    }

    m_speed = row.m_w16;         // +0x3ac table +0x40
    m_deceleration = row.m_w17;  // +0x3b0 table +0x44
    m_lifetimeMs = row.m_w15;    // +0x3b4 table +0x3c

    if (row.m_w10 < 0) {  // table +0x28: visual dict index
        SetVisualObject(nullptr);
    } else {
        SetVisualObject(ProjectileDaeName(row.m_w10), nullptr, false);
    }

    // NOT MADE: the PhysicalObject body with the POProjectile vtable and SetPhysicalObject(body, false). See the header
    // comment above.

    SetPosition(m_startPosition, true);
    SetRotation(owner->m_rotation);  // owner +0x16c

    if (m_target != nullptr) {
        SetDestination(m_target->GetTargetPosition());
    } else {
        Point3D<float> direction = aim;
        if (m_projectileNode != nullptr && useProjectileNode) {
            const glitch::core::vector3d<float> nodePosition = m_projectileNode->getAbsolutePosition();
            const Point3D<float> ownerTarget = owner->GetTargetPosition();
            direction = Point3D<float>{nodePosition.X - ownerTarget.X, nodePosition.Y - ownerTarget.Y, 0.0f};
            // NOT MADE: direction.normalize() (see the header comment above).
        }
        const Point3D<float> base = owner->GetTargetPosition();
        SetDestination(Point3D<float>{direction.X * 1000.0f + base.X, direction.Y * 1000.0f + base.Y,
                                      direction.Z * 1000.0f + base.Z});
    }

    if (m_visualObject != nullptr) {  // +0x2d8
        AnimController* animator = m_visualObject->m_animController;  // VisualObject +0x38
        animator->PlayClip(0u, true, 0, 0u);                          // vtable +0x1c (clip 0, loop, animation 0)
        m_visualObject->Sync();
        if (m_lifetimeMs == -666) {
            m_lifetimeMs = animator->GetClipDuration(0u);  // vtable +0x18: the animation length
        }
    }
    m_pfObject.SetFlying(true);
    m_pfObject.SetSwimming(true);
}

// 0x3e5d9c (D1) and 0x3e5e00 (D2): the same body. The vptr writes and the ~GameObject call are emitted by the compiler.
Projectile::~Projectile() {
}

// 0x3e5e40 (C1) and 0x3e5edc (C2): the same body. GameObject(id) is the base constructor, the vptr writes are the compiler's.
// The original zeroes only the members listed here; the others (the table columns and the callbacks) are set by SetInfo.
Projectile::Projectile(GO_IDS id) : GameObject(id) {
    m_impactPosition = Point2D<float>{0.0f, 0.0f};  // +0x3c4, +0x3c8
    m_projectileId = -1;                             // +0x374
    m_expiring = false;                              // +0x3d1
    unk_85 = 1;                                      // ObjectBase +0x85: the updating flag
    m_manager = nullptr;                             // +0x378
    m_owner = nullptr;                               // +0x380
    m_target = nullptr;                              // +0x384
    m_startPosition = Point3D<float>{0.0f, 0.0f, 0.0f};  // +0x388..+0x393
    m_position = Point3D<float>{0.0f, 0.0f, 0.0f};       // +0x394..+0x39f
    m_projectileNode = nullptr;                      // +0x3a4
    m_onHit = nullptr;                               // +0x3b8
    m_hitObject = nullptr;                           // +0x3cc
    m_dead = false;                                  // +0x3d0
}
