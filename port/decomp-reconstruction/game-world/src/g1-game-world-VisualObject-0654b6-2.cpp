// Batch g1-game-world-VisualObject-0654b6-2 (part 3 of 4): VisualObject mesh visibility and box, the scale and
// rotation setters and their owner forwarders, parenting, the constructor and destructor, shadow and x-ray mesh
// creation, weapon skins, the x-ray and base material passes, the mesh flush and the modular module id.
// Declarations come from dh2/VisualObject.hpp; no header change is needed.
//
// Most bodies are partial. They reach a scene node through m_node (RootSceneNode*) or through the device's scene
// manager, and neither can be bound yet:
//   - RootSceneNode derives from glitch::collada::CRootSceneNode, which has no base in root_scene_node.hpp, so the
//     ISceneNode calls the original makes on m_node (getScale, setPosition, setVisible, remove, ...) are not declared
//     on it. The ISceneNode vtable slots used here follow the original's vptr+N numbering (N is the pseudocode offset).
//   - DeviceSceneManager() is the one point for the scene manager (IDevice +0x1c); it returns null without a device, so the scene-manager calls are skipped then.
// The bodies that do not need those reads are written in full: the constructor's field setup, the owner forwarders,
// the modular module name and the box normalise-and-centre step (CenterMeshBox, called once the box reads are bound).

#include <cfloat>
#include <cstdint>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include "dh2/Application.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/Level.hpp"
#include "dh2/RootSceneNode.hpp"
#include "dh2/SceneManager.hpp"
#include "dh2/VisualObject.hpp"
#include "dh2/ShadowModularSkinnedMeshSceneNode.hpp"
#include "dh2/XrayModularSkinnedMeshSceneNode.hpp"
#include "dh2/global/Vec3f_Origin.hpp"
#include "dh2/global/DeviceSceneManager.hpp"
#include "dh2/global/ctrl_camera.hpp"
#include "glitch-core-io/os/device.hpp"
#include "glitch-scene-collada-gui/collada_scene_accessors.hpp"
#include "glitch-scene-collada-gui/scene/ISceneNode.hpp"

namespace {

// Names of VisualObject::SetWeaponSkin(char const*,int,int)::s_slotMapping, a three-entry table in .data.rel.ro at
// 0x956ad8. Its entries are relocated to the strings at 0x8cd710, 0x8cd730 and 0x8cd750 in libDungeonHunter2.so.
// Slot 0..2 picks the mount node. The original indexes it without a range check.
const char* const kSlotMapping[3] = {
    "anchor_shield_left_offset",
    "anchor_weapon_right_offset",
    "anchor_weapon_left_offset",
};

// Orders one axis so that the lower value is in lo: the original's min/max pair per axis, 0x47211c tail.
void OrderAxis(float& lo, float& hi) {
    if (hi < lo) {
        std::swap(lo, hi);
    }
}

// 0x47211c, tail: each axis is ordered (min <= max), and the box is then centred on Vec3f_Origin with its half extents
// (min = origin - half, max = origin + half). Not called from CalcMeshBox yet, because the node reads that fill the box
// are not bound (see CalcMeshBox).
[[maybe_unused]] void CenterMeshBox(glitch::core::aabbox3d<float>& box) {
    OrderAxis(box.MinEdge.X, box.MaxEdge.X);
    OrderAxis(box.MinEdge.Y, box.MaxEdge.Y);
    OrderAxis(box.MinEdge.Z, box.MaxEdge.Z);

    const float halfX = (box.MaxEdge.X - box.MinEdge.X) * 0.5f;
    const float halfY = (box.MaxEdge.Y - box.MinEdge.Y) * 0.5f;
    const float halfZ = (box.MaxEdge.Z - box.MinEdge.Z) * 0.5f;

    box.MinEdge.X = Vec3f_Origin.X - halfX;
    box.MaxEdge.X = halfX + Vec3f_Origin.X;
    box.MinEdge.Y = Vec3f_Origin.Y - halfY;
    box.MaxEdge.Y = halfY + Vec3f_Origin.Y;
    box.MinEdge.Z = Vec3f_Origin.Z - halfZ;
    box.MaxEdge.Z = halfZ + Vec3f_Origin.Z;
}

// The release the original destructor (0x473abc) gives a node that it owns: removeAnimators (vptr+0x74), remove
// (vptr+0x68), then the reference drop, and the slot is cleared. Used for the weapon nodes at +0x30 and +0x34.
void ReleaseSceneNode(glitch::scene::ISceneNode*& node) {
    if (node == nullptr) {
        return;
    }
    node->removeAnimators();
    node->remove();
    node->drop();
    node = nullptr;
}

}  // namespace

// The device's scene manager, the object at +0x1c of the device pointer that the original reads as dword_99F73C (Application::m_device). Null without a device.
SceneManager* DeviceSceneManager() {
    glitch::IDevice* device = ApplicationGetInstance().m_device;
    return device != nullptr ? reinterpret_cast<SceneManager*>(device->sceneManager) : nullptr;
}

// 0x471f1c _ZN12VisualObject14SetMeshVisibleEb
// PARTIAL: the three typed node lists under m_node (tags 1835360612, 1936023908, 1298489700) get setVisible. The
// shadow-modular and x-ray loops are made. The shadow-skinned loop is not made: ShadowSkinnedMeshSceneNode does not
// compile in this tree (BaseMeshSceneNode<CSkinnedMeshSceneNode>), so its setVisible is not reachable.
void VisualObject::SetMeshVisible(bool visible) {
    if (m_node == nullptr) {
        return;
    }
    const std::uint32_t tags[3] = {1835360612u, 1936023908u, 1298489700u};
    for (std::uint32_t tag : tags) {
        std::vector<glitch::scene::ISceneNode*> nodes;
        m_node->getSceneNodesFromType(static_cast<glitch::scene::E_SCENE_NODE_TYPE>(tag), nodes);
        for (glitch::scene::ISceneNode* node : nodes) {
            node->setVisible(visible);
        }
    }
    if (m_shadowCreated) {
        for (ShadowSkinnedMeshSceneNode* node : m_shadowSkinned) {
            (void)node;  // node->setVisible(visible): class does not compile yet
        }
        for (ShadowModularSkinnedMeshSceneNode* node : m_shadowModular) {
            node->setVisible(visible);
        }
    }
    if (m_xrayCreated) {
        for (XrayModularSkinnedMeshSceneNode* node : m_xrayModular) {
            node->setVisible(visible);
        }
    }
}

// 0x47211c _ZN12VisualObject11CalcMeshBoxEv
// partial: the box is filled from node reads that are not bound. With a collision box node (+0x0c), the original takes
// the node's bounds (getBoundingBox, vptr+0x30, not declared on ISceneNode), scales its min and max corners by the
// scale of the node's parent (getParent, then getScale, vptr+0x90), applies the node's relative transform (vptr+0x40, a
// 4x4 matrix whose translation is cleared) with Point3D<float>::transform, and then normalises and centres the box.
// Without one, it grows the box over the mesh nodes found under m_node (SearchByType tags 1936023908 then 1835360612),
// and zeroes the box and returns when no node is found. This body keeps the empty-box start (min +FLT_MAX, max
// -FLT_MAX) and leaves the node loops out. CenterMeshBox is the tail and is not called until those reads are bound.
void VisualObject::CalcMeshBox() {
    if (m_colBoxNode != nullptr) {
        // Collision box path: not bound (see the comment above).
    } else {
        m_meshBox.MinEdge.X = FLT_MAX;
        m_meshBox.MinEdge.Y = FLT_MAX;
        m_meshBox.MinEdge.Z = FLT_MAX;
        m_meshBox.MaxEdge.X = -FLT_MAX;
        m_meshBox.MaxEdge.Y = -FLT_MAX;
        m_meshBox.MaxEdge.Z = -FLT_MAX;
        // Mesh node loop: not bound (see the comment above).
    }
}

// 0x472708 _ZN12VisualObject10SetScalingEf
// Compares m_node's scale with (uniform, uniform, uniform); when any axis differs, sets it, then CalcMeshBox and ApplyMeshBox.
void VisualObject::SetScaling(float uniform) {
    if (m_node == nullptr) {
        return;
    }
    const glitch::core::vector3d<float>& current = m_node->getScale();
    if (current.X != uniform || current.Y != uniform || current.Z != uniform) {
        m_node->setScale(glitch::core::vector3d<float>{uniform, uniform, uniform});
        CalcMeshBox();
        ApplyMeshBox();
    }
}

// 0x4727ac _ZN12VisualObject10SetScalingERK7Point3DIfE
// Same as the uniform overload, with the vector (scale.X, scale.Y, scale.Z).
void VisualObject::SetScaling(const Point3D<float>& scale) {
    if (m_node == nullptr) {
        return;
    }
    const glitch::core::vector3d<float>& current = m_node->getScale();
    if (current.X != scale.X || current.Y != scale.Y || current.Z != scale.Z) {
        m_node->setScale(glitch::core::vector3d<float>{scale.X, scale.Y, scale.Z});
        CalcMeshBox();
        ApplyMeshBox();
    }
}

// 0x472860 _ZN12VisualObject11SyncScalingEv
// Forwards the owner's scale (GameObject::m_scale, +0x120) to SetScaling. The target is partial (0x4727ac).
void VisualObject::SyncScaling() {
    if (m_owner != nullptr) {
        SetScaling(m_owner->m_scale);
    }
}

// 0x472874 _ZN12VisualObject11SetRotationERK7Point3DIfE
// Builds the quaternion from the angles as (Y, -X, -Z). When it differs from m_node's rotation in any component, sets it,
// then CalcMeshBox and ApplyMeshBox.
void VisualObject::SetRotation(const Point3D<float>& rotation) {
    if (m_node == nullptr) {
        return;
    }
    glitch::core::quaternion q;
    q.set(rotation.Y, -rotation.X, -rotation.Z);
    const glitch::core::quaternion& current = m_node->getRotation();
    if (current.X != q.X || current.Y != q.Y || current.Z != q.Z || current.W != q.W) {
        m_node->setRotation(q);
        CalcMeshBox();
        ApplyMeshBox();
    }
}

// 0x472948 _ZN12VisualObject12SyncRotationEv
// Forwards the owner's rotation (GameObject::m_rotation, +0x16c) to SetRotation. The target is partial (0x472874).
void VisualObject::SyncRotation() {
    if (m_owner != nullptr) {
        SetRotation(m_owner->m_rotation);
    }
}

// 0x47295c _ZN12VisualObject9SetParentEP10GameObject
// partial: stores the owner. When the owner is not static, or it is static and animated (IsAnimated, vptr+0x80), the
// original runs Sync and then RecursiveSetBoolOnNode(m_node, true, setOnAnimateEnabled). Otherwise it runs Sync and
// copies the node position onto itself (getPosition, vptr+0xa0, then setPosition, vptr+0xa4) and returns
// OptimizeStatic(m_node). The node calls are not bound: m_node is a RootSceneNode*, and the free functions take an
// ISceneNode*. OptimizeStatic is not declared in any header.
void VisualObject::SetParent(GameObject* owner) {
    m_owner = owner;
    if (owner == nullptr) {
        return;
    }
    if (!owner->m_static || owner->IsAnimated()) {
        Sync();
        // RecursiveSetBoolOnNode(m_node, true, setOnAnimateEnabled): not bound.
        return;
    }
    Sync();
    // m_node->setPosition(m_node->getPosition()) and OptimizeStatic(m_node): not bound.
}

// 0x472c5c _ZN12VisualObjectC2EP10GameObjectRKSsS3_
// partial. 0x472a0c (_ZN12VisualObjectC1EP10GameObjectRKSsS3_) is the complete-object copy of the same constructor.
// This body sets every field that the original sets. The model load is not bound: AssetManager::loadSceneNode is
// declared to return int, and the original stores the returned node pointer in m_node. While m_node stays null the rest
// of the original constructor does not run: SetParent(owner), RootSceneNode::RefreshBoundingBox, _FindModularSkinnedMeshNode,
// the add to the scene root and ForceRegister, the "_colbox_" lookup (m_colBoxNode, m_hasColBox), CalcMeshBox,
// ApplyMeshBox, and the AnimController(m_node, false) that SetAnimController takes.
// The original constructor does not set m_shadowCreated (+0x98) or m_xrayCreated (+0xa8). They are set false here.
VisualObject::VisualObject(GameObject* owner, const std::string& model, const std::string& variant) {
    m_owner = owner;
    m_node = nullptr;
    m_colBoxNode = nullptr;
    m_hasColBox = false;
    m_meshBox.MinEdge.X = 0.0f;
    m_meshBox.MinEdge.Y = 0.0f;
    m_meshBox.MinEdge.Z = 0.0f;
    m_meshBox.MaxEdge.X = 0.0f;
    m_meshBox.MaxEdge.Y = 0.0f;
    m_meshBox.MaxEdge.Z = 0.0f;
    m_modularNode = nullptr;
    m_weaponNodeMain = nullptr;
    m_weaponNodeOff = nullptr;
    m_animController = nullptr;
    unk_3c = false;
    unk_40 = 0;
    m_effectColor[0] = -1.0f;
    m_effectColor[1] = -1.0f;
    m_effectColor[2] = -1.0f;
    m_effectParamB = -1.0f;
    m_effectParamA = -1.0f;
    unk_6c = false;
    unk_74 = -1.0f;
    unk_7c = false;
    unk_7d = false;
    m_rimLight = false;
    m_hasModularNode = false;
    m_shadowCreated = false;
    m_xrayCreated = false;
    m_xrayMask = false;

    // Model load: not bound. The original passes model.c_str() and, when variant is empty, a null name. Its flags are
    // false and 255. See the comment above.
    const char* variantName = variant.empty() ? nullptr : variant.c_str();
    (void)model;
    (void)variantName;
}

// 0x4732ac _ZN12VisualObject16CreateShadowMeshEv
// partial: the shadow copies are built per skinned node (ShadowSkinnedMeshSceneNode, 428 bytes) and per modular node
// (ShadowModularSkinnedMeshSceneNode, 472 bytes). Each copy takes its mesh from the node's getter (vptr+0xf8, not
// named in the headers) and is added to the scene root (addChild on the root, vptr+0x5c). The node lists come from
// getSceneNodesFromType on m_node, which is not bound. The flag and the final ForceRegister are bound.
void VisualObject::CreateShadowMesh() {
    if (m_node == nullptr) {
        return;
    }
    m_shadowCreated = true;
    // Shadow node loops: not bound (see the comment above).
    if (SceneManager* sm = DeviceSceneManager()) {
        sm->ForceRegister();
    }
}

// 0x4736e4 _ZN12VisualObject14CreateXrayMeshEv
// partial: the same shape as CreateShadowMesh, with XrayModularSkinnedMeshSceneNode (468 bytes) per modular node, and
// the flag at +0xa8. The node list and the mesh getter are not bound (see CreateShadowMesh).
void VisualObject::CreateXrayMesh() {
    if (m_node == nullptr) {
        return;
    }
    m_xrayCreated = true;
    // X-ray node loop: not bound (see CreateShadowMesh).
    if (SceneManager* sm = DeviceSceneManager()) {
        sm->ForceRegister();
    }
}

// 0x473abc _ZN12VisualObjectD2Ev
// 0x473884 (_ZN12VisualObjectD1Ev) is the complete-object copy of the same destructor.
// SetAnimController(nullptr); the collision box node (+0x0c) gets a plain drop; the model root m_node (+0x08) and each weapon
// node (+0x30 main, +0x34 off) get removeAnimators (vptr+0x74), remove (vptr+0x68) and drop; then ForceRegister on the device
// scene manager. The light filter (+0x44) and the node vectors are std::vector members, released with the object.
VisualObject::~VisualObject() {
    SetAnimController(nullptr);
    if (m_colBoxNode != nullptr) {
        m_colBoxNode->drop();
        m_colBoxNode = nullptr;
    }
    if (m_node != nullptr) {
        m_node->removeAnimators();
        m_node->remove();
        m_node->drop();
        m_node = nullptr;
    }
    ReleaseSceneNode(m_weaponNodeMain);
    ReleaseSceneNode(m_weaponNodeOff);
    if (SceneManager* sm = DeviceSceneManager()) {
        sm->ForceRegister();
    }
}

// 0x473cd8 _ZN12VisualObject13SetWeaponSkinEPKcii
// partial: the weapon scene is loaded from data/3d/characters/prince/weapons/<weapon>.bdae. The load is not bound: the
// original calls CColladaDatabase::constructScene with the path, flag 1 and ColladaFactory::s_factory, and the engine
// declares only constructScene(IVideoDriver*). The slot then takes the loaded node: kind 1 is the main hand
// (m_weaponNodeMain) and any other kind is the off hand (m_weaponNodeOff). The previous node is removed and dropped
// first (not bound). A null weapon clears the slot, which is bound here. The node named by kSlotMapping[slot] is found
// under m_node (SearchByName), and the new node is added to it (addChild, vptr+0x5c). Neither the search nor the add
// is bound, because m_node is a RootSceneNode* and the search takes an ISceneNode*.
glitch::scene::ISceneNode* VisualObject::SetWeaponSkin(const char* weapon, int kind, int slot) {
    if (weapon != nullptr) {
        std::string path = "data/3d/characters/prince/weapons/";
        path += weapon;
        path += ".bdae";
        (void)path;
        // Load of path, then the slot store of the loaded node: not bound.
    } else {
        if (kind == 1) {
            m_weaponNodeMain = nullptr;
        } else {
            m_weaponNodeOff = nullptr;
        }
    }
    const char* mountName = (slot >= 0 && slot < 3) ? kSlotMapping[slot] : nullptr;
    (void)mountName;
    // SearchByName(m_node, mountName, false) and the addChild of the new node: not bound.
    return nullptr;
}

// 0x473ea0 _ZN12VisualObject17ApplyXrayMaterialEv
// For each x-ray node: regenerateXrayMaterial(). Then, for each material in the node's m_xrayMaterials (+0x1b0):
// ChangeCustomXrayTechnique(sm, material, 1, 1), updateXrayMaterial() on the node, SetMaterialRimLightEffects(sm, material,
// 4.0, "char_rim_light_xray.tga") and SetMaterialEffects(sm, material, (0, 0, 1), 0.98, -1.0). The engine's routine reads
// only the second and third flags of ChangeCustomXrayTechnique, so the third header flag is passed as false. The getType()
// result at vptr+0xbc is discarded and left out.
void VisualObject::ApplyXrayMaterial() {
    if (!m_xrayCreated) {
        return;
    }
    SceneManager* sm = DeviceSceneManager();
    if (sm == nullptr) {
        return;
    }
    for (XrayModularSkinnedMeshSceneNode* node : m_xrayModular) {
        node->regenerateXrayMaterial();
        for (auto& material : node->m_xrayMaterials) {
            sm->ChangeCustomXrayTechnique(material, true, true, false);
            node->updateXrayMaterial();
            sm->SetMaterialRimLightEffects(material, 4.0f, std::string("char_rim_light_xray.tga"));
            sm->SetMaterialEffects(material, glitch::core::vector3d<float>{0.0f, 0.0f, 1.0f}, 0.98f, -1.0f);
        }
    }
}

// 0x474140 _ZN12VisualObject11FlushMeshesEv
// partial. The original runs SearchByType on the device scene manager for the three mesh tags under m_node, flushes each
// found node's mesh (the mesh comes from the node's getMesh, vptr+0xf8, and goes to FlushMeshBuffers(mesh, 1, 1, 0)),
// and returns the model name from m_node's UID holder (uidOwner at RootSceneNode +0x14c, then the word at +0x20). Missing:
//   - the search root. SearchByType takes a glitch::scene::ISceneNode*, and m_node is a RootSceneNode*. RootSceneNode is not
//     an ISceneNode in the tree (layout request: CRootSceneNode derives from ISceneNode at offset 0).
//   - FlushMeshBuffers(glitch::scene::IMesh*, bool, bool, bool), symbol _Z16FlushMeshBuffersPN6glitch5scene5IMeshEbbb, is
//     declared in no header.
//   - the device scene manager (IDevice +0x1c, see the destructor): DeviceSceneManager() returns null.
//   - the model name. The original reads the word at +0x20 of CColladaUIDHolder (uidOwner) as a char*, but
//     glitch::collada::CColladaUIDHolder declares it as a 32-bit uid (layout request: pointer-width const char* at +0x20).
// Returns an empty string until those are bound.
std::string VisualObject::FlushMeshes() {
    return std::string();
}

// 0x474318 _ZN12VisualObject22ApplyMaterialBaseParamEv
// For each node of the three typed lists under m_node (SearchByType tags 1835360612, 1936023908, 1298489700): with m_rimLight
// set, SetMaterialRimLightEffects(node, 1.9, the current level's rim light file); with m_xrayMask set, reload the debug switches
// and, when "RENDERING_EnableXray" is on, SetMaskXray(node, true).
void VisualObject::ApplyMaterialBaseParam() {
    SceneManager* sm = DeviceSceneManager();
    if (sm == nullptr || m_node == nullptr) {
        return;
    }
    const std::uint32_t tags[3] = {1835360612u, 1936023908u, 1298489700u};
    for (std::uint32_t tag : tags) {
        std::vector<glitch::scene::ISceneNode*> nodes;
        sm->SearchByType(m_node, nodes, tag);
        for (glitch::scene::ISceneNode* node : nodes) {
            if (m_rimLight) {
                Level* level = ApplicationGetInstance().GetCurrentLevel();
                if (level != nullptr) {
                    sm->SetMaterialRimLightEffects(node, 1.9f, level->GetRimLightFile());
                }
            }
            if (m_xrayMask) {
                DebugSwitches* switches = DebugSwitches::GetInstance();
                switches->load();
                if (switches->GetSwitch("RENDERING_EnableXray")) {
                    sm->SetMaskXray(node, true);
                }
            }
        }
    }
}

// 0x474568 _ZNK12VisualObject18GetModularModuleIdEiPKc
// Builds "#<module>-mesh-skin" and asks the modular skinned node for its module id. The category argument is unused in
// the original too. Returns -1 without a modular node.
int VisualObject::GetModularModuleId(int categoryId, const char* module) const {
    (void)categoryId;
    std::string name = "#";
    name += module;
    name += "-mesh-skin";
    if (m_modularNode == nullptr) {
        return -1;
    }
    return m_modularNode->getModuleId(name.c_str());
}
