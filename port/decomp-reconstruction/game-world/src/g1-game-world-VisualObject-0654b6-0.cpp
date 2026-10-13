// Batch g1-game-world-VisualObject-0654b6-0: VisualObject sync, animation-controller swap, rotation
// copy, position sync and the empty apply and fade hooks. Declarations come from dh2/VisualObject.hpp.
//
// Bodies that call into scene nodes are partial. RootSceneNode (the m_node target), the scene manager
// that Application keeps at +0x1c, and the shadow and x-ray skin node classes have no declarations in
// any header yet, so those calls cannot be bound. Each partial body names the call it is missing.

#include "dh2/AnimController.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/VisualObject.hpp"

// 0x38ba74 _ZN12VisualObject4SyncEv
void VisualObject::Sync() {
    SyncPosition();
    SyncRotation();  // 0x472948, another batch
    SyncScaling();   // 0x472860, another batch
}

// 0x4709dc _ZNK12VisualObject15GetSpecificNodeEi
// partial: the original calls getSceneNodeFromType(typeTag, m_node) (glitch::scene::CSceneManager, vptr+0x1c)
// on the object at Application +0x1c (dword_99F73C + 28). That field is void* (Application::unk_1c), and
// m_node is RootSceneNode, which is not declared. Returns null until both are declared.
glitch::scene::ISceneNode* VisualObject::GetSpecificNode(int typeTag) const {
    (void)typeTag;
    return nullptr;
}

// 0x470a18 _ZNK12VisualObject15GetSpecificNodeEPKc
// partial: same as the int overload, through getSceneNodeFromName(name, m_node) (vptr+0x18). Same gaps.
glitch::scene::ISceneNode* VisualObject::GetSpecificNode(const char* name) const {
    (void)name;
    return nullptr;
}

// 0x470a54 _ZN12VisualObject12ApplyMeshBoxEv
// partial: the original calls m_owner->SetRelativeAABB(m_meshBox, m_hasColBox) (GameObject vptr+0x9c)
// when the owner is set. GameObject::SetRelativeAABB takes aabb<float>, which no header defines (it is
// only forward declared), and m_meshBox is glitch::core::aabbox3d<float>. The call is not bound.
void VisualObject::ApplyMeshBox() {
    if (m_owner != nullptr) {
        // m_owner->SetRelativeAABB(m_meshBox, m_hasColBox): not bound, see above.
    }
}

// 0x470a84 _ZN12VisualObject17SetAnimControllerEP14AnimController
// Replaces m_animController. The previous controller is deleted through its virtual destructor
// (vptr+0x04 in the original, the deleting destructor).
void VisualObject::SetAnimController(AnimController* controller) {
    if (m_animController != controller) {
        if (m_animController != nullptr) {
            delete m_animController;
            m_animController = nullptr;
        }
        m_animController = controller;
    }
}

// 0x470ac4 _ZNK12VisualObject9IsVisibleEv
// partial: the original returns (m_node != 0) && (m_node->word at +0x11c & 1), bit 0 being the visible flag
// of the scene node. RootSceneNode is not declared, so the word is not read and false is returned.
bool VisualObject::IsVisible() const {
    return false;
}

// 0x470ad8 _ZN12VisualObject27SetShadowSkinnedMeshVisibleEb
// partial: the original passes visible to vptr+0x48 (setVisible(bool) on the node) of every node in
// m_shadowSkinned (+0x80) and then every node in m_shadowModular (+0x8c). The shadow node classes are not
// declared, so neither call is bound.
void VisualObject::SetShadowSkinnedMeshVisible(bool visible) {
    (void)visible;
    for (ShadowSkinnedMeshSceneNode* node : m_shadowSkinned) {
        (void)node;  // node->setVisible(visible): not bound
    }
    for (ShadowModularSkinnedMeshSceneNode* node : m_shadowModular) {
        (void)node;  // node->setVisible(visible): not bound
    }
}

// 0x470b78 _ZN12VisualObject25SetXraySkinnedMeshVisibleEb
// partial: the original passes visible to vptr+0x48 (setVisible(bool)) of every node in m_xrayModular
// (+0x9c). The x-ray node class is not declared, so the call is not bound.
void VisualObject::SetXraySkinnedMeshVisible(bool visible) {
    (void)visible;
    for (XrayModularSkinnedMeshSceneNode* node : m_xrayModular) {
        (void)node;  // node->setVisible(visible): not bound
    }
}

// 0x470bd0 _ZN12VisualObject19ForceUpdatePositionEv
// partial: the original stores 1 in the byte at m_node +0x208 when m_node is set. RootSceneNode is not
// declared, so the store is not bound.
void VisualObject::ForceUpdatePosition() {
    // if (m_node != nullptr) m_node->unk_208 = 1: not bound, see above.
}

// 0x470be4 _ZN12VisualObject11GetPositionER7Point3DIfE
// partial: the original copies the three floats returned by m_node's getPosition (vptr+0xa0, which returns a
// const vector3d<float>&) into out when m_node is set. RootSceneNode is not declared, so out is not written.
void VisualObject::GetPosition(Point3D<float>& out) {
    (void)out;
}

// 0x470c24 _ZN12VisualObject11SetPositionERK7Point3DIfE
// partial: when m_node is set, the original calls m_node->setPosition(position) (vptr+0xa4) with a copy of
// the argument, and then m_node->updateAbsolutePosition(false) (vptr+0xb8). Neither call is bound.
void VisualObject::SetPosition(const Point3D<float>& position) {
    (void)position;
}

// 0x470c84 _ZN12VisualObject11GetRotationER7Point3DIfE
// Copies the owner's rotation (GameObject::m_rotation at +0x16c) into out. The original reads it only when
// m_node is set, so the copy is gated the same way.
void VisualObject::GetRotation(Point3D<float>& out) {
    if (m_node == nullptr || m_owner == nullptr) {
        return;
    }
    out = m_owner->m_rotation;
}

// 0x470cb8 _ZN12VisualObject12SyncPositionEv
// Pushes the owner's position (GameObject::m_position at +0x160) into the node through SetPosition, which
// is partial (0x470c24).
void VisualObject::SyncPosition() {
    if (m_owner != nullptr) {
        SetPosition(m_owner->m_position);
    }
}

// 0x470ccc _ZN12VisualObject13ApplyRotationEv
// Empty in the original (a bare return).
void VisualObject::ApplyRotation() {}

// 0x470cd0 _ZN12VisualObject12ApplyScalingEv
// Empty in the original (a bare return).
void VisualObject::ApplyScaling() {}

// 0x470cd4 _ZN12VisualObject13ApplyMaterialEv
// Empty in the original (a bare return).
void VisualObject::ApplyMaterial() {}

// 0x470cd8 _ZN12VisualObject12StartFadeOutEf
// Empty in the original (a bare return).
void VisualObject::StartFadeOut(float seconds) {
    (void)seconds;
}

// 0x470cdc _ZN12VisualObject11StopFadeOutEv
// Empty in the original (a bare return).
void VisualObject::StopFadeOut() {}

// 0x470ce0 _ZN12VisualObject13UpdateFadeOutEv
// Empty in the original (a bare return).
void VisualObject::UpdateFadeOut() {}

// 0x470ce4 _ZN12VisualObject11StartFadeInEf
// Empty in the original (a bare return).
void VisualObject::StartFadeIn(float seconds) {
    (void)seconds;
}
