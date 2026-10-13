// Batch g1-game-world-LiftableObject-ad7bbe-1 (part 2 of 2): the two copies of the constructor, InitFinal and
// DeclareProperties of LiftableObject. The destructor is defined in part 1 only. Declarations come from
// dh2/LiftableObject.hpp. Original 32-bit size 0x3a8 (936).
//
// Partial functions and what is missing:
//   - InitFinal: the physics body is a PhysicalObject here. The original stores the PODecor vtable over it after the
//     constructor, which C++ cannot express (see the comment on InitFinal).
//
// No test: the constructors and DeclareProperties need GameObject, ObjectBase and PropertyMap, and
// InitFinal needs the Application singleton and the physics world. None of the logic runs without the engine.

#include "dh2/LiftableObject.hpp"

#include "dh2/Application.hpp"
#include "dh2/Character.hpp"
#include "dh2/PhysicalObject.hpp"
#include "dh2/PlayerInfo.hpp"
#include "dh2/PlayerManager.hpp"

// The base-object destructor (D2, 0x3eec3c) has no definition in this file. Its body is the destructor defined once in
// g1-game-world-LiftableObject-ad7bbe-0.cpp (D1, 0x3eebcc). That is the key function of the class, so the file emits the
// vtable there. A second definition here would be a duplicate symbol at link time.

// 0x3eec88 LiftableObject::LiftableObject(ObjectBase::GO_IDS)
// 0x3eed10 LiftableObject::LiftableObject(ObjectBase::GO_IDS)
// Two copies of the same constructor (the C1 and C2 forms). The copy at 0x3eed10 differs only in a cast of the string
// address, so one definition covers both. The compiler writes the vtable pointers (+0x00, +0x04, +0x24) and constructs
// m_dataDesc (+0x378) empty. The body clears the fields the original clears. It does not write m_dataId (+0x374).
LiftableObject::LiftableObject(GO_IDS id) : GameObject(id) {
    m_static = false;                                   // +0x84 ObjectBase::m_static (GameObject::IsValidatingFloorPosition reads it)
    m_owner = nullptr;                                  // +0x390
    m_dropping = false;                                 // +0x394
    m_baseScale = Point3D<float>{0.0f, 0.0f, 0.0f};     // +0x398 .. +0x3a3
    m_pinPending = false;                               // +0x3a4
}

// 0x3eeec8 LiftableObject::InitFinal()
// PARTIAL: the physics body is built as a PhysicalObject, not as a PODecor. The original calls
// PhysicalObject::PhysicalObject(world, this, isStatic 0, isCircle 0, isSensor 0, isBullet 0, group 0, category 2,
// mask 0xffff, unused 1) and then stores the PODecor vtable (off_968F88, ELF _ZTV7PODecor + 8) over the PhysicalObject
// one. C++ cannot retype an object after construction. new PODecor(world, this, false) would not match either:
// PODecor's constructor passes isStatic true, and the original passes 0. The only visible difference is the dynamic
// type. PODecor's deleting destructor (0x470188) also calls CustomFree, which this body does not.
// Layout request for dh2/PODecor.hpp: a constructor that takes isStatic (or a way to build a non-static PODecor body).
void LiftableObject::InitFinal() {
    if (CheckSpawnProbability() < m_spawnProb) {        // +0x274 (GameObject::m_spawnProb)
        GameObject::InitFinal();
        if (MeetCondition()) {
            // PlayerManager::GetLocalPlayer(0, true) (Application +0x40), then the character at PlayerInfo +0x660. The
            // header declares that member as unk_660; the pseudocode names it character_660.
            Character* character = ApplicationGetInstance().m_playerManager->GetLocalPlayer(0, true)->unk_660;
            m_baseScale = character->m_scale;           // +0x120 .. +0x128 of the character (GameObject::m_scale)
            SetScale(m_baseScale);                      // GameObject::SetScale(this, &m_baseScale)

            // The physics world is Application +0x44 (the original's dword_99F770).
            PhysicalObject* body = new PhysicalObject(ApplicationGetInstance().m_physicalWorld, this, false, false, false,
                                                      false, 0, 2, 0xFFFF, 1);
            body->pin();                                // PhysicalObject::pin, zero mass
            SetPhysicalObject(body, false);
            Update();                                   // the original tail-calls vtable slot +0x2c (LiftableObject::Update)
        }
    }
}

// 0x3eefec LiftableObject::DeclareProperties()
// Declares "data" (int, default -1, m_dataId) and "data_desc" (string, default "", m_dataDesc) after the GameObject
// properties, in the original order. The inline SimpleTypeProperty objects of the original match the typed AddProperty
// templates, as in Container::DeclareProperties. The byte offsets the original stores are 880 and 884 from the
// PropertyMap sub-object at +0x04, that is +0x374 and +0x378 from the start of the object.
// Layout conflict for the integrator: dh2/GameObject.hpp names m_entryPointId (int) at +0x374, which is where this
// offset puts m_dataId. dh2/Container.hpp puts the same data/data_desc pair at +0x374 as well. One of the two claims is
// wrong, and this body does not decide which.
void LiftableObject::DeclareProperties() {
    GameObject::DeclareProperties();
    AddProperty("data", m_dataId, -1);
    AddProperty("data_desc", m_dataDesc);
}
