// Batch g1-game-world-VisualObject-0654b6-1: the empty fade and update hooks, the modular skin queries,
// the shadow and material hooks, light set and position application, visibility sync and the
// modular node search. Declarations come from dh2/VisualObject.hpp; no member was added.
//
// The scene manager is the word at device +0x1c (dword_99F73C is Singleton<Application>::s_inst.m_device,
// so the manager is IDevice +0x1c, not Application +0x1c). No header declares it yet, so every original call
// made through it is left out and named in a comment. Bodies with such a call are PARTIAL.
// dh2/ShadowSkinnedMeshSceneNode.hpp and dh2/ShadowModularSkinnedMeshSceneNode.hpp do not compile yet
// (BaseMeshSceneNode<CSkinnedMeshSceneNode> is incomplete), so the shadow loops only walk the node pointers.

#include "dh2/GameObject.hpp"
#include "dh2/RootSceneNode.hpp"
#include "dh2/VisualObject.hpp"
#include "glitch-scene-collada-gui/collada_scene_accessors.hpp"

// 0x470ce8 _ZN12VisualObject10StopFadeInEv
// Empty in the original (bare return).
void VisualObject::StopFadeIn() {}

// 0x470cec _ZN12VisualObject12UpdateFadeInEv
// Empty in the original (bare return).
void VisualObject::UpdateFadeIn() {}

// 0x470cf0 _ZN12VisualObject6UpdateEv
// Virtual slot 2. Empty in the original (bare return).
void VisualObject::Update() {}

// 0x470cf4 _ZNK12VisualObject4DrawEv
// Virtual slot 3. Empty in the original (bare return).
void VisualObject::Draw() const {}

// 0x470e18 _ZN12VisualObject14SetModularSkinEii
// PARTIAL: the original does nothing when there is no modular node or categoryId is -1. Otherwise it calls
// m_modularNode->setCategoryModule(categoryId, moduleId), which is CModularSkinnedMeshSceneNode::setCategoryModule(int, int)
// (_ZN6glitch7collada28CModularSkinnedMeshSceneNode17setCategoryModuleEii, 0x649430). That overload is not
// declared on the engine class. It then calls notifyVisibilityChanged on the scene manager (device +0x1c).
// Both calls are left out; the guard is kept.
void VisualObject::SetModularSkin(int categoryId, int moduleId) {
    if (m_modularNode == nullptr || categoryId == -1) {
        return;
    }
    (void)moduleId;
    // Not made: m_modularNode->setCategoryModule(categoryId, moduleId), see above.
    // Not made: scene manager->notifyVisibilityChanged(), scene manager at device +0x1c.
}

// 0x470e5c _ZNK12VisualObject20GetModularCategoryIdEPKc
// Category id from the modular node, or -1 when there is no modular node.
int VisualObject::GetModularCategoryId(const char* category) const {
    if (m_modularNode != nullptr) {
        return m_modularNode->getCategoryId(category);
    }
    return -1;
}

// 0x470e74 _ZNK12VisualObject20GetModularModuleNameEii
// Module name from the modular node, or "???" when there is no modular node.
const char* VisualObject::GetModularModuleName(int categoryId, int moduleId) const {
    if (m_modularNode != nullptr) {
        return m_modularNode->getModuleName(categoryId, moduleId);
    }
    return "???";
}

// 0x470e94 _ZNK12VisualObject21GetModularModuleCountEi
// Hex-Rays types the result as a node pointer. The null path returns the null word, which is 0 as an int.
int VisualObject::GetModularModuleCount(int categoryId) const {
    if (m_modularNode != nullptr) {
        return static_cast<int>(m_modularNode->getCategoryModuleCount(categoryId));
    }
    return 0;
}

// 0x470ea4 _ZNK12VisualObject23GetModularCategoryCountEv
// Same shape as GetModularModuleCount: the category count of the modular node, 0 without one.
int VisualObject::GetModularCategoryCount() const {
    if (m_modularNode != nullptr) {
        return static_cast<int>(m_modularNode->getCategoryCount());
    }
    return 0;
}

// 0x470eb4 _ZN12VisualObject35ApplyShadowProjectionMaterialEffectEN6glitch4core8vector3dIfEEff
// PARTIAL: the gate (color.X, a or b above -0.5) and the m_shadowCreated test are made. The original then calls
// SceneManager::SetShadowProjectionMaterialEffects(sm, material, color, a, b) for the material at +0x198 of every
// ShadowSkinnedMeshSceneNode in m_shadowSkinned, and for every material in the +0x1b0 vector of every
// ShadowModularSkinnedMeshSceneNode in m_shadowModular. The scene manager is not reachable and the two shadow node
// classes do not compile yet, so the loops only walk the pointers.
void VisualObject::ApplyShadowProjectionMaterialEffect(glitch::core::vector3d<float> color, float a, float b) {
    if (!(color.X > -0.5f || a > -0.5f || b > -0.5f)) {
        return;
    }
    if (!m_shadowCreated) {
        return;
    }
    for (ShadowSkinnedMeshSceneNode* node : m_shadowSkinned) {
        (void)node;  // Not made: SetShadowProjectionMaterialEffects(sm, node->m_shadowMaterial (+0x198), color, a, b).
    }
    for (ShadowModularSkinnedMeshSceneNode* node : m_shadowModular) {
        (void)node;  // Not made: for each material in node->unk_1b0 (+0x1b0), SetShadowProjectionMaterialEffects(sm, material, color, a, b).
    }
}

// 0x4710f0 _ZN12VisualObject19ApplyShadowMaterialEv
// PARTIAL: the m_shadowCreated test and the walk over m_shadowModular are made. The original calls
// regenerateShadowMaterial() on each node, then discards the result of the getType() slot (vptr+0xbc; the
// modular override returns a constant, so that call is left out). For each material in the +0x1b0 vector it
// calls SceneManager::ChangeCustomShadowProjectionTechnique(sm, material, true). Neither call is made: the node
// class does not compile yet and the scene manager is not reachable.
void VisualObject::ApplyShadowMaterial() {
    if (!m_shadowCreated) {
        return;
    }
    for (ShadowModularSkinnedMeshSceneNode* node : m_shadowModular) {
        (void)node;  // Not made: node->regenerateShadowMaterial(); then the sm call per +0x1b0 material.
    }
}

// 0x471214 _ZN12VisualObject13ApplyLightSetEv
// PARTIAL: the original tail-calls SceneManager::UpdateLightSet(sm, index, filter, node) with index = unk_40
// (+0x40), filter = the std::vector<bool>& at this+0x44 and node = m_node (+0x08). The scene manager is not
// reachable, and +0x44 is declared as raw words in VisualObject.hpp (layout request), so the call is left out.
void VisualObject::ApplyLightSet() {
    // Not made: SceneManager::UpdateLightSet(sm, unk_40, <std::vector<bool> at +0x44>, m_node).
}

// 0x47124c _ZN12VisualObject13ApplyPositionEv
// PARTIAL: the original reads the node position into a local with GetPosition (0x470be4, partial: it leaves its
// output unset) and then calls GameObject::SetPosition(pos, false) on m_owner. Until GetPosition fills its output,
// the owner would be set to (0, 0, 0), so the SetPosition call is left out. The m_owner guard is kept.
void VisualObject::ApplyPosition() {
    if (m_owner == nullptr) {
        return;
    }
    // Not made: Point3D<float> pos; GetPosition(pos); m_owner->SetPosition(pos, false).
}

// 0x471298 _ZN12VisualObject16ExternalNodeSyncEP13RootSceneNode
// PARTIAL: when node and m_owner are set, the original copies the owner's transform into the scene node:
//   node->setPosition(m_owner->m_position)                                       vptr+0xa4
//   node->updateAbsolutePosition(false)                                          vptr+0xb8
//   quaternion::set(q, m_rotation.Y, -m_rotation.X, -m_rotation.Z); node->setRotation(q)   vptr+0x9c
//   node->setScale(m_owner->m_scale)                                             vptr+0x94
// (m_position at GameObject +0x160, m_rotation at +0x16c, m_scale at +0x120.) RootSceneNode does not reach these
// ISceneNode methods: glitch::collada::CRootSceneNode (root_scene_node.hpp) has no ISceneNode base in the tree.
// The four calls are left out; the guard is kept.
void VisualObject::ExternalNodeSync(RootSceneNode* node) {
    if (node == nullptr || m_owner == nullptr) {
        return;
    }
    // Not made: the four node calls above.
}

// 0x471368 _ZN12VisualObject10SetVisibleEb
// PARTIAL: when the node is set, the original calls SceneManager::ForceRegister() if the requested flag differs
// from bit 0 of the node's flags word (CRootSceneNode +0x11c, unk_011c), then the node's setVisible(bool)
// (vptr+0x48). The ForceRegister call is left out (scene manager not reachable); setVisible is made.
void VisualObject::SetVisible(bool visible) {
    if (m_node == nullptr) {
        return;
    }
    if (visible != ((m_node->unk_011c & 1u) != 0)) {
        // Not made: SceneManager::ForceRegister() on the scene manager at device +0x1c.
    }
    m_node->setVisible(visible);
}

// 0x4713d0 _ZN12VisualObject14SyncVisibilityEv
// Visible when the owner's visible flag (+0x80, m_visible) is set and either the owner is not zonable
// (IsZonable, vptr+0xc4), zoning is off (m_zoningEnabled at +0x2ee is 0) or the owner is in a zone (m_inZone at
// +0x2f0 is set). The result goes to SetVisible. This body is complete; SetVisible (0x471368) is partial only in
// its ForceRegister call.
void VisualObject::SyncVisibility() {
    if (m_owner == nullptr) {
        return;
    }
    const bool visible = m_owner->m_visible &&
                         (!m_owner->IsZonable() || m_owner->m_zoningEnabled == 0 || m_owner->m_inZone != 0);
    SetVisible(visible);
}

// 0x4718f0 _ZN12VisualObject27_FindModularSkinnedMeshNodeEv
// PARTIAL: the original asserts m_node (VisualObject.cpp line 1023; gAssertLevel 2 writes to address 0, level 1
// prints the message). It then asks the scene manager for every node of type 0x4D656164 ("Mead", the modular
// skinned mesh tag) through vptr+0x20 (CSceneManager::getSceneNodesFromType, listing 0x2c). That call passes
// two arguments, while the listed overload takes a root as well. For each node found it stores m_modularNode
// (+0x2c) and sets m_hasModularNode (+0x7f). The assert, the lookup and the stores are left out.
void VisualObject::_FindModularSkinnedMeshNode() {
    // Not made: ASSERT(m_node) at VisualObject.cpp:1023.
    // Not made: scene manager->getSceneNodesFromType(0x4D656164, nodes) through vptr+0x20; for each node,
    //           m_modularNode = node and m_hasModularNode = true.
}

// 0x471a0c _ZN12VisualObject13ResetSkinningEv
// PARTIAL: the original asserts m_node (VisualObject.cpp line 997) and gets the 0x4D656164 nodes from the scene
// manager (vptr+0x20, as in _FindModularSkinnedMeshNode). For each node and each category i of this object's
// modular node, it reads node->getCurrentModuleId(i), calls SetModularSkin(i, id == 0) and then
// SetModularSkin(i, id). The node list is not reachable and SetModularSkin is partial, so the loop is not made.
void VisualObject::ResetSkinning() {
    // Not made: ASSERT(m_node) at VisualObject.cpp:997.
    // Not made: for each node from scene manager->getSceneNodesFromType(0x4D656164, ...) through vptr+0x20:
    //           for i in [0, GetModularCategoryCount()): id = node->getCurrentModuleId(i);
    //           SetModularSkin(i, id == 0); SetModularSkin(i, id).
}

// 0x471b7c _ZN12VisualObject19ApplyMaterialEffectEN6glitch4core8vector3dIfEEff
// PARTIAL: the effect state is updated exactly as in the original: the colour (+0x58..+0x60), param a (+0x68)
// and param b (+0x64), each compared with its previous value before it is stored. When one of the three changed,
// the original searches the scene manager for nodes of types 0x6D656164, 0x73656164 and 0x4D656164 under m_node
// and calls SetMaterialEffects(node, color, a, b) on each. Those searches and per-node calls are left out (scene
// manager not reachable). The final ApplyShadowProjectionMaterialEffect((-1, -1, -1), b, -1) call is made.
void VisualObject::ApplyMaterialEffect(glitch::core::vector3d<float> color, float a, float b) {
    const bool colorChanged = (color.X != m_effectColor[0] || color.Y != m_effectColor[1] ||
                               color.Z != m_effectColor[2]) && color.X > -0.5f;
    const bool paramAChanged = m_effectParamA != a && a > -0.5f;
    const bool paramBChanged = m_effectParamB != b && b > -0.5f;
    m_effectColor[0] = color.X;
    m_effectColor[1] = color.Y;
    m_effectColor[2] = color.Z;
    m_effectParamA = a;
    m_effectParamB = b;
    if (!(colorChanged || paramAChanged || paramBChanged)) {
        return;
    }
    // Not made: SearchByType(sm, m_node, nodes, t) for t in {0x6D656164, 0x73656164, 0x4D656164}, then
    //           SetMaterialEffects(sm, node, color, a, b) for each node.
    ApplyShadowProjectionMaterialEffect(glitch::core::vector3d<float>{-1.0f, -1.0f, -1.0f}, b, -1.0f);
}

// 0x471dc4 _ZN12VisualObject23ApplyUnlitColorMaterialEv
// PARTIAL: the original collects the nodes of type 0x73656164 ("sead") under m_node with SceneManager::SearchByType.
// For each node it reads the parent's name (ISceneNode::getName, vptr+0x24). When the name is not "SP_shadow"
// (LC_API_STRNCMP(name, "SP_shadow", 9) != 0) it discards the node's getType() result (vptr+0xbc, the constant
// getter, so that call is left out) and calls SceneManager::ResetCustomTechniqueToDefault(sm, m_node, false, false,
// 0, true, true). The search, the name test and the reset are left out: the scene manager and the ISceneNode
// methods are not reachable from RootSceneNode.
void VisualObject::ApplyUnlitColorMaterial() {
    // Not made: nodes = SearchByType(sm, m_node, 0x73656164); for each node whose parent name is not "SP_shadow",
    //           ResetCustomTechniqueToDefault(sm, m_node, false, false, 0, true, true).
}
