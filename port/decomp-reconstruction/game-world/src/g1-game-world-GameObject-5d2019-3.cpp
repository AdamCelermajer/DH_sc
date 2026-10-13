// Batch g1-game-world-GameObject-5d2019-3 (GameObject, part 4 of 8).
// Reconstructed from the Hex-Rays pseudocode of libDungeonHunter2.so. Layout and declarations: dh2/GameObject.hpp.
// Offsets in comments are the original 32-bit ones.
//
// The Lua statics take (const Arguments&, ReturnValues&, void* self). The Hex-Rays listings name the self pointer
// a3 and the return values a2 (the argument types are shifted by one). The assembly passes the object in the third
// register, so the bodies use that. sfc::script::lua::ReturnValues and sfc::script::lua::Binder have no declarations
// in the tree yet: the push calls and the Lua registrations are not made, and those functions are partial.

#include <cstddef>
#include <cstdint>
#include <string>

#include "dh2/GameObject.hpp"
#include "dh2/AnchorBase.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/LuaScript.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/PhysicalObject.hpp"
#include "dh2/SimpleTypeProperty.hpp"
#include "dh2/VisualObject.hpp"
#include "dh2/global/ObjectManagerInstance.hpp"
#include "dh2/global/Vec3f_Origin.hpp"

// The profiling markers are declared in dh2/global/runtime_support.hpp (0x3136b4, 0x3136b8). That header is not
// included here: it pulls in core-platform/memory.hpp, whose deps/glitch_core.hpp conflicts with the
// glitch-shared/glitch_core.hpp that AnchorBase.hpp and VisualObject.hpp need in the same translation unit.
void PushProfilingContext(const char* name);  // 0x3136b4
void PopProfilingContext(const char* name);   // 0x3136b8

// 0x38cbe8 GameObject::Update()
void GameObject::Update() {
    PushProfilingContext("GameObject::Update");
    // The original calls load() and GetSwitch() on DebugSwitches::s_inst, which is private here. GetInstance()
    // returns the same object (it loads the file on first use, and load() repeats the read as in the original).
    DebugSwitches* switches = DebugSwitches::GetInstance();
    switches->load();
    (void)switches->GetSwitch("TraceUpdateGameObjectOnce");  // the original discards the result
    ++dword_99F764->unk_58;                                    // ObjectManager +0x58 counter (assembly: ldr/add/str)
    if (m_pendingInteractor != nullptr) {                      // +0x2e4
        Interact(m_pendingInteractor);                         // vtable +0x98 with the interactor in r1
        m_pendingInteractor = nullptr;
    }
    m_prevPosition = m_position;                               // +0x160..+0x168 copied to +0x190..+0x198
    m_prevRotation = m_rotation;                               // +0x16c..+0x174 copied to +0x19c..+0x1a4
    UpdatePath();
    UpdateRotation();
    UpdateSubObjects();
    UpdateTargetPosition();
    RequireOnlineUpdate();
    if (m_idleSoundId >= 0) {                                  // +0x370 (int16); negative means no idle sound
        UpdateIdleSound();
    }
    PopProfilingContext("GameObject::Update");
}

// 0x38cd48 GameObject::InitFinal()
// PARTIAL. Reconstructed: the spawn gate, the always-visible switch, the radius, the debug name copy, the visual
// Sync and the final UpdatePFObject call. Not reconstructed:
//  (1) PFWorld::InitObject(Singleton<PFWorld>::s_inst, m_pfObject, m_static, m_position, radius, this). The PFWorld
//      instance has no accessor in the tree, so the object is not registered with the world.
//  (2) The light set id: LightSetManager::GetLightSetIdFromName(lightSetManager, m_lightSet). The manager is the
//      SceneManager's m_lightSetManager (SceneManager +0x294), reached through the device at Application +0x10 and
//      the device pointer at +0x1c. Neither the device nor the SceneManager accessor is declared. The id is stored
//      in the visual at +0x40 (VisualObject::unk_40, no name yet).
//  (3) m_targetNode = the "target_node" child of the visual's root node: ISceneNode::getSceneNodeFromName is not
//      declared, and VisualObject::m_node (RootSceneNode) is only forward-declared.
void GameObject::InitFinal() {
    // Original gate: CheckSpawnProbability() < m_spawnProb, and the object is not marked for deletion (+0x81).
    if (!(CheckSpawnProbability() < m_spawnProb && m_markedForDeletion == 0)) {
        return;
    }
    if (m_alwaysVisible) {
        SetVisible(true);
    }

    // The radius is the larger of the X and Y extents of the local box: relAabb max minus min (+0x150 - +0x144,
    // +0x154 - +0x148).
    float radius = m_relAabb[3] - m_relAabb[0];
    const float extentY = m_relAabb[4] - m_relAabb[1];
    if (radius < extentY) {
        radius = extentY;
    }
    (void)radius;  // the PFWorld::InitObject argument, see note (1)

    // Not reconstructed: PFWorld::InitObject(...), see note (1).

    // The original reads the character pointer of the object name (+0x44, inside m_name at +0x30) and assigns it to
    // PFObject::m_debugName (+0x8c). That is m_name.c_str().
    m_pfObject.m_debugName.assign(m_name.c_str());

    if (m_visualObject != nullptr) {                         // +0x2d8
        m_visualObject->Sync();
        // Not reconstructed: the light set id (2) and the target node (3).
    }
    UpdatePFObject();
}

// 0x38cee8 GameObject::DeclareProperties()
// PARTIAL: "facultative" (ObjectBase +0x60, default 1) is not registered. ObjectBase::m_facultative is uint8_t and
// AddProperty<bool> takes a bool&. Waits on a layout request that makes ObjectBase::m_facultative a bool, as m_visible
// is. The other registrations are made in the original's order.
void GameObject::DeclareProperties() {
    ObjectBase::DeclareProperties();
    AddProperty<Point3D<float>>("position", m_position, Vec3f_Origin);                // +0x160
    AddProperty<Point3D<float>>("rotation", m_rotation, Vec3f_Origin);                // +0x16c
    AddProperty<std::string>("dae", m_dae);                                           // +0x290
    AddProperty<std::string>("xrefobject", m_xrefObject);                             // +0x2a8
    AddProperty<std::string>("xrefmax", m_xrefMax);                                   // +0x2c0
    AddProperty<Point3D<float>>("scale", m_scale, Point3D<float>{1.0f, 1.0f, 1.0f});  // +0x120 (1.0f words)
    AddProperty<bool>("alwaysVisible", m_alwaysVisible, false);                       // +0x2ed
    AddProperty<bool>("skip_checkpoint", m_skipCheckpoint, false);                    // +0x15c
    // "spawn_prob" (+0x274, default 100) is the SimpleTypeProperty<int> the original builds by hand (36 bytes).
    // Its offset is the member address minus the PropertyMap sub-object at +0x04, which is what the constructor takes.
    AddProperty("spawn_prob",
                new SimpleTypeProperty<int>(static_cast<PropertyMap*>(this), "spawn_prob", &m_spawnProb, 100));
    AddProperty<std::string>("lightset", m_lightSet, std::string("PlayerLight"));     // +0x278
    AddProperty<std::string>("idle_sound", m_idleSound);                              // +0x358
    // Not reconstructed: "facultative" (see above).
}

// 0x38d228 GameObject::~GameObject()   (D1, complete object destructor)
// 0x38d378 GameObject::~GameObject()   (D2, base object destructor; the same body as D1)
// The vtable pointers are written by the compiler. The members (m_idleSound, m_targetList, m_xrefMax, m_xrefObject,
// m_dae, m_lightSet, m_pfObject) and then ObjectBase are destroyed after this body, in the original's order.
// PARTIAL: the sound stop is not reconstructed: VoxSoundManager::Stop(VoxSoundManager::s_instance, m_idleSoundId, 0).
// vox-audio/include/vox-audio/vox/sound_manager.hpp declares neither VoxSoundManager::s_instance nor Stop.
GameObject::~GameObject() {
    if (m_visualObject != nullptr) {                         // +0x2d8, deleted through its virtual destructor
        delete m_visualObject;
        m_visualObject = nullptr;
    }
    if (m_physicalObject != nullptr) {                       // +0x2dc
        delete m_physicalObject;
        m_physicalObject = nullptr;
    }
    if (m_cameraAnchor != nullptr) {                         // +0x2e0
        delete m_cameraAnchor;
        m_cameraAnchor = nullptr;
    }
    if (m_script != nullptr) {                               // +0x300
        delete m_script;
        m_script = nullptr;
    }
    // Not reconstructed: VoxSoundManager::Stop(VoxSoundManager::s_instance, m_idleSoundId, 0), see above.
}

// 0x38d35c GameObject::~GameObject()   (D0, deleting destructor)
// Skipped: the compiler generates the deleting destructor for `delete`. The original calls the D1 body and then
// CustomFree(this), which is operator delete in this tree.

// 0x38d5f8 GameObject::_Lock(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// The Lua LOCK method increments the ObjectBase lock counter (+0x29).
void GameObject::_Lock(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                       void* self) {
    ++static_cast<GameObject*>(self)->m_lockCount;
}

// 0x38d7a8 GameObject::_Unlock(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// The Lua UNLOCK method decrements the lock counter (+0x29) when it is not zero.
void GameObject::_Unlock(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                         void* self) {
    GameObject* object = static_cast<GameObject*>(self);
    if (object->m_lockCount != 0) {
        --object->m_lockCount;
    }
}

// 0x38d7ec GameObject::createBindings(sfc::script::lua::Binder&)
// PARTIAL: the Lua registrations are not made, because sfc::script::lua::Binder has no declaration in the tree (only a
// forward declaration), so bindFunction and bindMethod cannot be called. The original registers, in this order
// (F = bindFunction with this as the data, F0 = bindFunction with nullptr, M = bindMethod, the function casts to
// void (*)(const Arguments*, ReturnValues*, void*)):
//   LOCK M _Lock; UNLOCK M _Unlock; GetID F M; GetName F M; GetSelf F; GetObjectByName F; IsDead F M;
//   IsCharacter F M; IsPlayer F M; IsDoor F M; IsDecor F M; GetPosition F M; SetPosition F M; GetDistanceFrom F M;
//   GetDistanceBetween F0; IsInRange F M; IsOverAHole F M; IsInWater F M; IsFlying F M; MarkAsFlying F M;
//   IsSwimming F M; MarkAsSwimming F M; HasPath F M; SetTargetListCharacterFilter F M; SetTargetListObjectFilter F M;
//   SetTargetListSorting F M; TargetListSearch F M; TargetListSearchRect F M; TargetListResort F M;
//   TargetListBackup F M; IsTargetListEmpty F M; GetTargetListSize F M; GetTargetListTop F M; PopTargetList F M;
//   PlayFX F M; GrabFX F M; DropFX F M; SetFXEndPoint F0; PlaySound3D F; Summon F M; RegisterSummon F0;
//   SummonTriggerTrap F M; SummonTimerTrap F M; GetRand F M (_Rand); EnableCollisions F M; DealDamages F M;
//   SetMaxPath F, then M (the return value is the bindMethod result).
void GameObject::createBindings(sfc::script::lua::Binder& binder) {
    (void)binder;  // Not reconstructed: the registrations listed above.
}

// 0x38e700 GameObject::_GetPosition(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the three pushNumber calls are not made. The values are read in order: position X, Y, Z (+0x160..+0x168).
void GameObject::_GetPosition(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                              void* self) {
    const Point3D<float>& position = static_cast<GameObject*>(self)->m_position;
    (void)position;  // Not reconstructed: ret.pushNumber(X), pushNumber(Y), pushNumber(Z)
}

// 0x38e930 GameObject::_GetTargetListSize(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the pushInteger call is not made. The original takes the deque distance from the begin iterator (+0x304)
// to the end iterator (+0x314), which is the size of m_targetList.m_results.
void GameObject::_GetTargetListSize(const sfc::script::lua::Arguments& /*args*/,
                                    sfc::script::lua::ReturnValues& /*ret*/, void* self) {
    const std::size_t count = static_cast<GameObject*>(self)->m_targetList.m_results.size();
    (void)count;  // Not reconstructed: ret.pushInteger(count)
}

// 0x38e970 GameObject::_IsTargetListEmpty(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the pushBoolean call is not made. The original compares the begin and end iterators of the target deque.
void GameObject::_IsTargetListEmpty(const sfc::script::lua::Arguments& /*args*/,
                                    sfc::script::lua::ReturnValues& /*ret*/, void* self) {
    const bool empty = static_cast<GameObject*>(self)->m_targetList.m_results.empty();
    (void)empty;  // Not reconstructed: ret.pushBoolean(empty)
}

// 0x38e98c GameObject::_HasPath(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the pushBoolean call is not made. The path is the PFObject list m_pfObject.m_path (object +0x200). The
// original walks it and reports true when it has any node, which is !empty().
void GameObject::_HasPath(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                          void* self) {
    const bool hasPath = !static_cast<GameObject*>(self)->m_pfObject.m_path.empty();
    (void)hasPath;  // Not reconstructed: ret.pushBoolean(hasPath)
}

// 0x38e9c0 GameObject::_IsDecor(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the pushBoolean call is not made. The test is m_goId == 21 (the GO_IDS value the binary compares against;
// the enum has no named values yet).
void GameObject::_IsDecor(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                          void* self) {
    const bool isDecor = static_cast<std::int32_t>(static_cast<GameObject*>(self)->m_goId) == 21;
    (void)isDecor;  // Not reconstructed: ret.pushBoolean(isDecor)
}

// 0x38e9d8 GameObject::_IsDoor(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the pushBoolean call is not made. The test is m_goId == 2 (see _IsDecor on the GO_IDS values).
void GameObject::_IsDoor(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                         void* self) {
    const bool isDoor = static_cast<std::int32_t>(static_cast<GameObject*>(self)->m_goId) == 2;
    (void)isDoor;  // Not reconstructed: ret.pushBoolean(isDoor)
}

// 0x38e9f0 GameObject::_IsPlayer(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the pushBoolean call is not made. The value is the virtual IsPlayer() (vtable +0x28).
void GameObject::_IsPlayer(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                           void* self) {
    const bool isPlayer = static_cast<GameObject*>(self)->IsPlayer();
    (void)isPlayer;  // Not reconstructed: ret.pushBoolean(isPlayer)
}

// 0x38ea1c GameObject::_IsCharacter(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the pushBoolean call is not made. The value is the virtual IsCharacter() (vtable +0x24).
void GameObject::_IsCharacter(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                              void* self) {
    const bool isCharacter = static_cast<GameObject*>(self)->IsCharacter();
    (void)isCharacter;  // Not reconstructed: ret.pushBoolean(isCharacter)
}

// 0x38ea48 GameObject::_IsDead(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the pushBoolean call is not made. The value is the virtual IsDead() (vtable +0x34).
void GameObject::_IsDead(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                         void* self) {
    const bool isDead = static_cast<GameObject*>(self)->IsDead();
    (void)isDead;  // Not reconstructed: ret.pushBoolean(isDead)
}

// 0x38ea74 GameObject::_IsSwimming(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the pushBoolean call is not made. The value is PFObject::IsSwimming() on the embedded PFObject (+0x1c8).
void GameObject::_IsSwimming(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                             void* self) {
    const bool isSwimming = static_cast<GameObject*>(self)->m_pfObject.IsSwimming();
    (void)isSwimming;  // Not reconstructed: ret.pushBoolean(isSwimming)
}

// 0x38ea94 GameObject::_IsFlying(sfc::script::lua::Arguments const&, sfc::script::lua::ReturnValues&, void*)
// PARTIAL: the pushBoolean call is not made. The value is PFObject::IsFlying() on the embedded PFObject (+0x1c8).
void GameObject::_IsFlying(const sfc::script::lua::Arguments& /*args*/, sfc::script::lua::ReturnValues& /*ret*/,
                           void* self) {
    const bool isFlying = static_cast<GameObject*>(self)->m_pfObject.IsFlying();
    (void)isFlying;  // Not reconstructed: ret.pushBoolean(isFlying)
}
