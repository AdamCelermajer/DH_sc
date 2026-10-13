// Batch g1-game-world-LiftableObject-ad7bbe-0 (part 1 of 2): the methods of LiftableObject from IsUpdatable through the
// destructors. Reconstructed from libDungeonHunter2.so pseudocode (IDA Hex-Rays and the assembly for the call shapes).
//
// Layout: the members are those of dh2/LiftableObject.hpp. m_dataId is at +0x374, which GameObject.hpp also declares as
// m_entryPointId; the bodies use the named member, so only the offset comment is in doubt (see the layout request).
//
// Three functions are partial: Release (the node re-attach to the scene root), PickUp (the QE_PickedUpLiftable event)
// and InitPost (the data-row lookup). Drop is partial because it calls Release. The rest are reconstructed.
// The destructor is defined once in the tree, here (0x3eebcc, see the note there), because this file emits the vtable.

#include <cstdio>
#include <cstdlib>

#include "dh2/AnimController.hpp"
#include "dh2/Application.hpp"
#include "dh2/LiftableObject.hpp"
#include "dh2/PFWorld.hpp"
#include "dh2/PhysicalObject.hpp"
#include "dh2/RootSceneNode.hpp"
#include "dh2/Singleton.hpp"
#include "dh2/VisualObject.hpp"
#include "dh2/global/Vec3f_Origin.hpp"
#include "glitch-scene-collada-gui/scene/ISceneNode.hpp"

extern int gAssertLevel;  // engine assert level (core-platform/memory.hpp, not included here, as in DestructibleContainer)

namespace {

const char kLiftableObjectSource[] =
    "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Complex\\LiftableObject.cpp";

// The original ASSERT: level 2 writes to address 0 (a crash here, kept as abort); level 1 prints and execution goes on.
void ReportAssert(const char* expression, int line) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", expression, kLiftableObjectSource, line);
    }
}

// VisualObject::m_node (a RootSceneNode*) as the glitch::scene::ISceneNode* the original passes to ISceneNode methods
// with no adjustment (Release 0x3ee468 calls getAbsolutePosition and addChild on it). RootSceneNode does not derive from
// glitch::scene::ISceneNode in this tree, so this is a reinterpret_cast, correct while the ISceneNode base is at offset 0
// (the same view as GameObject::IsBatchVisible). Waits on a RootSceneNode declaration that derives from ISceneNode.
glitch::scene::ISceneNode* AsSceneNode(RootSceneNode* node) {
    return reinterpret_cast<glitch::scene::ISceneNode*>(node);
}

}  // namespace

// 0x3ee3b0 _ZNK14LiftableObject11IsUpdatableEv
bool LiftableObject::IsUpdatable() const {
    return true;
}

// 0x3ee3b8 _ZNK14LiftableObject10IsAnimatedEv
bool LiftableObject::IsAnimated() const {
    return true;
}

// 0x3ee3c0 _ZNK14LiftableObject28IsUpdatingPositionFromVisualEv
// True while an owner holds the object (+0x390 m_owner is set), so the visual drives the position.
bool LiftableObject::IsUpdatingPositionFromVisual() const {
    return m_owner != nullptr;
}

// 0x3ee3d0 _ZNK14LiftableObject29IsUpdatingPositionFromPhysicsEv
bool LiftableObject::IsUpdatingPositionFromPhysics() const {
    return m_owner == nullptr;
}

// 0x3ee3e0 _ZNK14LiftableObject13IsInteractiveEP10GameObject
// Not interactive while marked for deletion (ObjectBase +0x81), held (+0x390), or dropping (+0x394).
bool LiftableObject::IsInteractive(GameObject* other) const {
    (void)other;
    if (m_markedForDeletion != 0 || m_owner != nullptr) {
        return false;
    }
    return !m_dropping;
}

// 0x3ee40c _ZNK14LiftableObject10IsObstacleEv
bool LiftableObject::IsObstacle() const {
    return true;
}

// 0x3ee414 _ZNK14LiftableObject17GetObstacleRadiusEv
// The original returns the bits 0x43160000 (1125515264), which is the float 150.0f (GameObject declares float).
float LiftableObject::GetObstacleRadius() const {
    return 150.0f;
}

// 0x3ee420 _ZNK14LiftableObject19GetObstacleStrengthEv
// The original returns the bits 0x41200000 (1092616192), which is the float 10.0f.
float LiftableObject::GetObstacleStrength() const {
    return 10.0f;
}

// 0x3ee42c _ZNK14LiftableObject9GetDataIdEv
// +0x374: the row index in Arrays::LiftableObjects that InitPost stores (-1 when there is no row).
int LiftableObject::GetDataId() const {
    return m_dataId;
}

// 0x3ee434 _ZN14LiftableObject10__CallbackEPN6glitch5scene19ITimelineControllerEPv
// Timeline callback of the visual, registered in InitPost with the object as user data: the drop animation has ended.
void LiftableObject::__Callback(glitch::scene::ITimelineController* controller, void* user) {
    (void)controller;
    static_cast<LiftableObject*>(user)->m_dropping = false;
}

// 0x3ee440 _ZNK14LiftableObject18GetInteractionTypeEP10GameObject
// 4 when nobody holds the object, 5 when other holds it, -1 otherwise.
int LiftableObject::GetInteractionType(GameObject* other) const {
    if (m_owner == nullptr) {
        return 4;
    }
    if (m_owner == other) {
        return 5;
    }
    return -1;
}

// 0x3ee460 _ZN14LiftableObject8InteractEP10GameObject
// The original body is empty.
void LiftableObject::Interact(GameObject* other) {
    (void)other;
}

// 0x3ee464 _ZNK14LiftableObject9IsZonableEv
// Thunk: the original is a tail branch to GameObject::MeetCondition (0x38ab60), forwarded here.
bool LiftableObject::IsZonable() const {
    return GameObject::MeetCondition();
}

// 0x3ee468 _ZN14LiftableObject7ReleaseEv
// PARTIAL. The original ends by attaching the visual's node to the scene root: root->addChild(m_visualObject->m_node),
// vtable +0x5c (ISceneNode::addChild). The root is read from the device's scene manager, Singleton<Application>::s_inst
// .m_device (+0x10) at +0x1c, then +0x04 of that object. IDevice +0x1c is not declared in
// glitch-core-io/os/device.hpp, the same blocker as Billboard::InitPost, so that call is not made here. Everything else
// is reconstructed in the original order.
void LiftableObject::Release() {
    if (m_owner == nullptr) {
        ReportAssert("m_owner", 233);
    }
    VisualObject* visual = m_visualObject;
    const glitch::core::vector3d<float> absolute = AsSceneNode(visual->m_node)->getAbsolutePosition();
    GameObject::SetPosition(Point3D<float>{absolute.X, absolute.Y, absolute.Z}, true);
    GameObject::SetScale(m_baseScale);
    Singleton<PFWorld>::s_inst.ValidatePosition(m_position, &m_pfObject);
    visual->Sync();
    // PARTIAL: root->addChild(visual->m_node) is not made (see above).
    m_physicalObject->enableFilter();
    m_owner = nullptr;
    m_pinPending = true;
}

// 0x3ee5a8 _ZN14LiftableObject4DropEv
// PARTIAL, through Release. Releases the object, then plays the "drop" clip with PlayClip(name, loop = false, 0, 0). The
// original passes r3 = 0 and the stack slot 0 for the last two parameters (checked in the assembly). m_dropping is set
// when the clip starts, and the timeline callback clears it. The node re-attach missing from Release means the drop does
// not re-attach the node to the scene root.
bool LiftableObject::Drop() {
    Release();
    const bool playing = m_visualObject->m_animController->PlayClip("drop", false, 0, 0);
    if (playing) {
        m_dropping = true;
    }
    return playing;
}

// 0x3ee5fc _ZN14LiftableObject6PickUpEP10GameObject
// PARTIAL, in two places. (1) The holder and the attached node are m_node viewed as ISceneNode by reinterpret_cast
// (see AsSceneNode). (2) The QE_PickedUpLiftable event is not raised (see the note before the return). The original
// builds it on the stack (vtable 0x967530, the QE_InteractWith layout, no members of its own) and calls
// EventManager::RaiseAsync on the current level: event id = PyDataConstants "v2QuestObjectiveType" /
// "PickedUpLiftable", +0x08 character 0, +0x0c this->unk_64, +0x10 0, +0x11 0, +0x14 -1, +0x18 GetDataId(). Returns true.
bool LiftableObject::PickUp(GameObject* owner) {
    if (m_owner != nullptr) {
        ReportAssert("m_owner == 0", 186);
    }
    if (owner == nullptr) {
        ReportAssert("owner != 0", 187);
    }
    if (m_owner != nullptr || owner == nullptr) {
        return false;
    }
    VisualObject* ownerVisual = owner->m_visualObject;
    VisualObject* myVisual = m_visualObject;
    if (ownerVisual == nullptr) {
        ReportAssert("ownerVO", 199);
    }
    if (myVisual == nullptr) {
        ReportAssert("myVO", 200);
    }
    if (ownerVisual == nullptr || myVisual == nullptr) {
        return false;
    }

    // The holder is the owner's "projectile_node", or the owner's root node when there is none.
    glitch::scene::ISceneNode* holder = ownerVisual->GetSpecificNode("projectile_node");
    if (holder == nullptr) {
        holder = AsSceneNode(ownerVisual->m_node);
    }
    myVisual->SetPosition(Vec3f_Origin);
    myVisual->SetScaling(Point3D<float>{1.0f, 1.0f, 1.0f});
    holder->addChild(AsSceneNode(myVisual->m_node));
    m_physicalObject->disableFilter();
    m_physicalObject->unpin();
    m_owner = owner;

    if (Singleton<Application>::s_inst.GetCurrentLevel() == nullptr) {
        ReportAssert("level", 223);
    }
    // PARTIAL: the QE_PickedUpLiftable event is not raised. Its id (IEvent +0x04) is eventId_, which
    // net-session-online/i_event.hpp (included through QuestEvent.hpp) declares protected, so game code cannot set it
    // (waits on the IEvent unification: dh2/IEvent.hpp has it public). Raising it with id 0 would be wrong.
    return true;
}

// 0x3ee9a4 _ZN14LiftableObject6UpdateEv
// Pins the physics body once after a release (m_pinPending, +0x3a4), then follows the visual's position.
void LiftableObject::Update() {
    if (m_pinPending) {
        m_physicalObject->pin();
        m_pinPending = false;
    }
    VisualObject* visual = m_visualObject;
    if (visual != nullptr) {
        Point3D<float> position{0.0f, 0.0f, 0.0f};
        visual->GetPosition(position);
        GameObject::SetPosition(position, true);
    }
}

// 0x3eea10 _ZN14LiftableObject8InitPostEv
// PARTIAL. The data row is not looked up. The original searches Arrays::LiftableObjects::m_memberNames for m_dataDesc
// (strcmp, the first match) and sets m_dataId to the index, or -1 when there is no match or the table is empty. When the
// element's GameObjectDict index (+0x04 of its 8-byte record) is not -1, it copies that dict name (+0x08 of a 12-byte
// record) into m_dae (+0x290). The lookup needs pydata-accessors/member_names.hpp, which cannot share a translation unit
// with dh2/GameObject.hpp (CustomFree clash, as in Door::InitPost), and the element records are not typed. So m_dataId
// and m_dae are left as they are. The spawn probability, the GameObject::InitPost call, the MeetCondition branch, the
// visual callback and idle clip, and the SetVisible(false) branch are reconstructed in the original order.
void LiftableObject::InitPost() {
    if (GameObject::CheckSpawnProbability() < m_spawnProb) {
        if (!m_dataDesc.empty()) {
            // PARTIAL: the row lookup described above.
        }
        GameObject::InitPost();
        if (GameObject::MeetCondition()) {
            VisualObject* visual = m_visualObject;
            if (visual != nullptr) {
                visual->m_animController->SetCallbacks(&LiftableObject::__Callback, this, nullptr, nullptr);
                visual->m_animController->PlayClip("idle", true, 0, 0);
                visual->ApplyMeshBox();
            }
        } else {
            SetVisible(false);
        }
    }
}

// 0x3eebcc _ZN14LiftableObjectD1Ev
// The original stores the three vtable pointers, destroys m_dataDesc (+0x378) and calls GameObject::~GameObject. The
// implicit member and base destruction does the same.
// This is the only definition of the destructor in the tree. The destructor is the key function of LiftableObject (the
// constructor is not virtual, and this is the first virtual function declared that is not inline), so this translation
// unit emits the vtable and the typeinfo. The base-object form D2 at 0x3eec3c is generated from this definition, and the
// part-2 file (ad7bbe-1.cpp) does not define it again.
LiftableObject::~LiftableObject() {}

// 0x3eec20 _ZN14LiftableObjectD0Ev
// Compiler-generated deleting destructor: ~LiftableObject() followed by CustomFree(this). A delete expression produces
// it, so no source is written.
