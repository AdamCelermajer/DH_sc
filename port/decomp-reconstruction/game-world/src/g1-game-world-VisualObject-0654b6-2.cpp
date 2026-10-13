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
//   - DeviceSceneManager() is the one point for the scene manager (IDevice +0x1c). It returns null until the device
//     layout is declared, so every scene-manager call is skipped.
// The bodies that do not need those reads are written in full: the constructor's field setup, the owner forwarders,
// the modular module name and the box normalise-and-centre step (CenterMeshBox, called once the box reads are bound).

#include <cfloat>
#include <cstddef>
#include <string>
#include <utility>
#include <vector>

#include "dh2/Application.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/Level.hpp"
#include "dh2/SceneManager.hpp"
#include "dh2/VisualObject.hpp"
#include "dh2/global/Vec3f_Origin.hpp"
#include "dh2/global/ctrl_camera.hpp"
#include "glitch-scene-collada-gui/collada_scene_accessors.hpp"

namespace {

// Names of VisualObject::SetWeaponSkin(char const*,int,int)::s_slotMapping, a three-entry table in .data.rel.ro at
// 0x956ad8. Its entries are relocated to the strings at 0x8cd710, 0x8cd730 and 0x8cd750 in libDungeonHunter2.so.
// Slot 0..2 picks the mount node. The original indexes it without a range check.
const char* const kSlotMapping[3] = {
    "anchor_shield_left_offset",
    "anchor_weapon_right_offset",
    "anchor_weapon_left_offset",
};

// The device's scene manager, the object at +0x1c of the device pointer that the original reads as dword_99F73C
// (Application::m_device). No header declares the device layout or an accessor for it yet, so this returns null
// and each scene-manager call that depends on it is skipped.
SceneManager* DeviceSceneManager() {
    return nullptr;
}

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

}  // namespace

// 0x471f1c _ZN12VisualObject14SetMeshVisibleEb
// partial: the three typed node lists under m_node (getSceneNodesFromType for tags 1835360612, 1936023908 and
// 1298489700) are not bound, and each of their nodes needs setVisible(visible) (vptr+0x48). setVisible is not declared
// on glitch::scene::ISceneNode, and m_node is a RootSceneNode*. The shadow and x-ray lists below are walked as in the
// original. Their node calls are not bound either, because setVisible is not declared on those classes.
void VisualObject::SetMeshVisible(bool visible) {
    (void)visible;
    if (m_node == nullptr) {
        return;
    }
    if (m_shadowCreated) {
        for (ShadowSkinnedMeshSceneNode* node : m_shadowSkinned) {
            (void)node;  // node->setVisible(visible): not bound
        }
        for (ShadowModularSkinnedMeshSceneNode* node : m_shadowModular) {
            (void)node;  // node->setVisible(visible): not bound
        }
    }
    if (m_xrayCreated) {
        for (XrayModularSkinnedMeshSceneNode* node : m_xrayModular) {
            (void)node;  // node->setVisible(visible): not bound
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
// partial: the original compares m_node's scale (getScale, vptr+0x90) with (uniform, uniform, uniform). When any axis
// differs, it sets that vector (setScale, vptr+0x94), then runs CalcMeshBox and ApplyMeshBox. The comparison and the
// set are not bound because RootSceneNode is not an ISceneNode in the headers. The CalcMeshBox and ApplyMeshBox calls
// are written where they belong once the comparison is bound.
void VisualObject::SetScaling(float uniform) {
    (void)uniform;
    if (m_node == nullptr) {
        return;
    }
    // Compare and set: not bound (see the comment above).
}

// 0x4727ac _ZN12VisualObject10SetScalingERK7Point3DIfE
// partial: same as the uniform overload, with the vector (scale.X, scale.Y, scale.Z) and the per-axis comparison.
void VisualObject::SetScaling(const Point3D<float>& scale) {
    (void)scale;
    if (m_node == nullptr) {
        return;
    }
    // Compare and set: not bound (see the comment above).
}

// 0x472860 _ZN12VisualObject11SyncScalingEv
// Forwards the owner's scale (GameObject::m_scale, +0x120) to SetScaling. The target is partial (0x4727ac).
void VisualObject::SyncScaling() {
    if (m_owner != nullptr) {
        SetScaling(m_owner->m_scale);
    }
}

// 0x472874 _ZN12VisualObject11SetRotationERK7Point3DIfE
// partial: builds the quaternion from the Euler angles in radians as (Y, -X, -Z), as the original does. The comparison
// with m_node's rotation (getRotation, vptr+0x98) and the set (setRotation, vptr+0x9c) are not bound, because
// RootSceneNode is not an ISceneNode in the headers. CalcMeshBox and ApplyMeshBox follow the set in the original.
void VisualObject::SetRotation(const Point3D<float>& rotation) {
    if (m_node == nullptr) {
        return;
    }
    glitch::core::quaternion q;
    q.set(rotation.Y, -rotation.X, -rotation.Z);
    // Compare with getRotation and, when different, setRotation(q): not bound. Then CalcMeshBox and ApplyMeshBox.
    (void)q;
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
    unk_44 = nullptr;
    unk_48 = 0;
    unk_4c = 0;
    unk_50 = 0;
    unk_54 = nullptr;
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
// partial. 0x473884 (_ZN12VisualObjectD1Ev) is the complete-object copy of the same destructor.
// The original releases the AnimController first, then the collision box node and the model root (both are IReferenceCounted
// releases: removeAnimators (vptr+0x74) and remove (vptr+0x68) on the root, then drop). Neither is bound. It then runs
// ForceRegister on the scene manager, and frees the storage of the three node vectors and of the unk_44 block.
// The vector storage is released when this object's members are destroyed. The nodes themselves are owned by the
// scene, not here. The unk_44 block is not freed: the original frees it with CustomFree above 0x80 bytes and with the
// node allocator below that, and the node allocator is not declared.
VisualObject::~VisualObject() {
    SetAnimController(nullptr);
    // m_colBoxNode release and m_node release (removeAnimators, remove, drop): not bound.
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
// partial: for each x-ray node, the original regenerates its x-ray materials, then for each material it runs
// ChangeCustomXrayTechnique(material, 1, 1), updateXrayMaterial, SetMaterialRimLightEffects(material, 4.0,
// "char_rim_light_xray.tga") and SetMaterialEffects(material, (0, 0, 1), 0.98, -1.0). None of these node calls is
// compiled here: dh2/XrayModularSkinnedMeshSceneNode.hpp does not compile in this tree (its BaseMeshSceneNode
// <CModularSkinnedMeshSceneNode> overrides name slots the engine class does not declare), so the node members are not
// reachable from this file. The scene-manager calls are also not bound: the original passes two flags to
// ChangeCustomXrayTechnique, and SceneManager.hpp declares three (a layout request). The original also reads the node's
// vptr+0xbc (getType) and drops the result, which has no effect, so it is omitted.
void VisualObject::ApplyXrayMaterial() {
    if (!m_xrayCreated) {
        return;
    }
    for (XrayModularSkinnedMeshSceneNode* node : m_xrayModular) {
        (void)node;  // node->regenerateXrayMaterial(), the per-material passes, node->updateXrayMaterial(): not bound
    }
}

// 0x474140 _ZN12VisualObject11FlushMeshesEv
// partial: the original runs SearchByType on the device scene manager for the three mesh tags under m_node, flushes each
// found node's mesh (FlushMeshBuffers(mesh, 1, 1, 0), a free function that is not declared), and returns the model name
// from m_node's UID holder (uidOwner at RootSceneNode +0x14c, then +0x20). That +0x20 word is read as a char* in the
// original, but CColladaUIDHolder declares it as a 32-bit uid, so no name is returned here. The node work is not bound.
std::string VisualObject::FlushMeshes() {
    return std::string();
}

// 0x474318 _ZN12VisualObject22ApplyMaterialBaseParamEv
// partial: for each node under m_node (SearchByType on tags 1835360612, 1936023908 and 1298489700, not bound), the
// original does two things. With m_rimLight set, it sets the rim light of the node to 1.9, using the current level's
// rim light file (Application::GetCurrentLevel, then Level::GetRimLightFile). With m_xrayMask set, it reloads the debug
// switches and, when "RENDERING_EnableXray" is on, sets the x-ray mask on the node. The per-node loop below runs over an
// empty list, because the node search is not bound. The calls inside it are written as the original makes them.
void VisualObject::ApplyMaterialBaseParam() {
    SceneManager* sm = DeviceSceneManager();
    std::vector<glitch::scene::ISceneNode*> nodes;  // the three typed searches under m_node: not bound, stays empty
    for (glitch::scene::ISceneNode* node : nodes) {
        if (m_rimLight && sm != nullptr) {
            Level* level = ApplicationGetInstance().GetCurrentLevel();
            sm->SetMaterialRimLightEffects(node, 1.9f, level->GetRimLightFile());
        }
        if (m_xrayMask) {
            DebugSwitches* switches = DebugSwitches::GetInstance();
            switches->load();
            if (switches->GetSwitch("RENDERING_EnableXray") && sm != nullptr) {
                sm->SetMaskXray(node, true);
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
