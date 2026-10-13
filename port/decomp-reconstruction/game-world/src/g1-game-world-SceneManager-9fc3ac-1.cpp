// Batch g1-game-world-SceneManager-9fc3ac-1 (part 2 of 3): SceneManager skinned-mesh registration, the material walkers
// (wire frame, mask x-ray, light set, normal map, material effects, material lookup), the technique remapping, the custom
// render list, clear and the destructor. Declarations come from dh2/SceneManager.hpp.
//
// Blocker for the material bodies. The scene-node chain that dh2/SceneManager.hpp includes (layouts/scene.hpp, through
// glitch-core-io's io/interfaces.hpp) pulls in the stand-in glitch::video::CMaterial and CMaterialRenderer of
// glitch-core-io/deps/glitch_core_objects.hpp, which have no members. The canonical glitch-video/material.hpp and
// material_renderer.hpp cannot be included in the same translation unit (they redefine the classes). Every body that
// reads a material or renderer field (m_renderer, m_technique, m_name, m_techniques, getTechnique, setRenderState) is
// therefore partial until the stand-ins are removed. The pure logic of those bodies is in
// dh2/global/scene_technique_names.hpp and is tested.
//
// Storage. The header keeps the light set manager (+0x294) as byte storage, so the calls on the LightSetManager object
// (ApplySettings, copy-assign, destructor) are unbound.
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <string>
#include <vector>

#include "dh2/LightBase.hpp"
#include "dh2/SceneManager.hpp"
#include "glitch-core-io/core/memory_hint.hpp"
#include "glitch-scene-collada-gui/scene/CLightSceneNode.hpp"

// Engine allocator, declared by hand: core-platform/memory.hpp redefines the glitch_core types that
// glitch-shared/glitch_core.hpp already defines (the game-world sources declare these the same way).
extern int gAssertLevel;
void* GlitchAlloc(unsigned size, glitch::memory::E_MEMORY_HINT hint);
void GlitchFree(void* p);

using glitch::scene::ISceneNode;

namespace {

// SearchByType words of the three node kinds the material walkers visit, searched in this order. They are the fourcc
// words "daem", "daes" and "daeM"; scene_types.hpp does not name them.
constexpr std::uint32_t kMaterialNodeTypes[3] = {0x6d656164u, 0x73656164u, 0x4d656164u};

// Most entries the batch list can hold: the original's 0x3FFFFFFF element limit, restated for 8-byte host pointers
// (GlitchAlloc takes an unsigned byte count).
constexpr std::size_t kMaxBatchEntries = std::numeric_limits<unsigned>::max() / sizeof(void*);

// Every node the material walkers visit: the three searches of the original, appended to one list in order.
std::vector<ISceneNode*> SearchMaterialNodes(SceneManager& manager, ISceneNode* root) {
    std::vector<ISceneNode*> nodes;
    for (std::uint32_t type : kMaterialNodeTypes) {
        manager.SearchByType(root, nodes, type);
    }
    return nodes;
}

}  // namespace

// 0x353afc SceneManager::registerSkinnedMesh(SkinnedMeshSceneNodeEx*)
// The batch list grows by doubling (1 when empty), as the original does. The count is taken in elements, not bytes.
void SceneManager::registerSkinnedMesh(SkinnedMeshSceneNodeEx* mesh) {
    if (m_batchEnd == m_batchCap) {
        const std::size_t count = static_cast<std::size_t>(m_batchEnd - m_batchBegin);
        const std::size_t capacity = count != 0 ? 2 * count : 1;
        // Past the element limit the original asks for -4 bytes (0xFFFFFFFC), which its allocator refuses.
        const unsigned bytes = capacity > kMaxBatchEntries ? 0xFFFFFFFCu
                                                           : static_cast<unsigned>(capacity * sizeof(void*));
        void** block = static_cast<void**>(GlitchAlloc(bytes, static_cast<glitch::memory::E_MEMORY_HINT>(0)));
        if (count != 0) {
            std::memmove(block, m_batchBegin, count * sizeof(void*));
        }
        block[count] = mesh;
        GlitchFree(m_batchBegin);
        m_batchBegin = block;
        m_batchEnd = block + count + 1;
        m_batchCap = block + capacity;
    } else {
        *m_batchEnd = mesh;
        ++m_batchEnd;
    }
}

// The base declares the same slot as the pure registerSkinnedMesh(void*). The typed overload above is what the original
// stores there, so this forwards to it. Not a ledger row.
void SceneManager::registerSkinnedMesh(void* skinnedMeshNode) {
    registerSkinnedMesh(static_cast<SkinnedMeshSceneNodeEx*>(skinnedMeshNode));
}

// 0x353bac SceneManager::_RegisterAutomacticLights(void)
// The original clears the distance list (CSceneManager +0x48, lightNodes) and appends {light->m_irrLight (LightBase +0x120),
// null user data, index as a double} for each LightBase* in LightSetManager::unk_190 (+0x424 .. +0x428 of this object).
void SceneManager::_RegisterAutomacticLights() {
    lightNodes.clear();                                                     // +0x48: the original sets end to begin
    for (std::size_t index = 0; index < m_lightSetManager.unk_190.size(); ++index) {
        LightBase* light = m_lightSetManager.unk_190[index];
        SDistanceNodeEntry entry;
        entry.node = light->m_irrLight;                                     // LightBase +0x120
        entry.userData = nullptr;
        entry.distance = static_cast<double>(index);                        // +0x08 stored as a double
        lightNodes.push_back(entry);
    }
}

// 0x353dec SceneManager::renderCustomList<glitch::scene::CSceneManager::SUnsortedNodeEntry>(
//          E_SCENE_NODE_CUSTOM_RENDER_PASS, std::vector<SUnsortedNodeEntry>&)
// PARTIAL. Written: renderPass (+0x174) takes the pass clamped to 9, and m_customRenderPass (+0x488) keeps the pass. The
// walk is unbound. The original appends a zeroed sentinel, calls render() (vtable slot 0x1c) on each entry's node while it
// updates the current and next rendered-node words (+0x9c..+0xb0), then resizes the list to zero. CSceneManager::
// SUnsortedNodeEntry is not declared in the engine layout, and the rendered-node word at +0xa8 is a 32-bit field that
// cannot hold a host pointer. The list is emptied here, which is the original's end state.
template <typename Entry>
void SceneManager::renderCustomList(std::uint32_t pass, std::vector<Entry>& list) {
    renderPass = static_cast<glitch::scene::E_SCENE_NODE_RENDER_PASS>(pass <= 8 ? pass : 9);
    m_customRenderPass = pass;
    list.clear();
}

// 0x354468 SceneManager::clear(void)
// PARTIAL. Written, in the original's order: the loaded scene roots are dropped and the list emptied, the base clear runs,
// unk_438 is dropped and cleared, and m_nodeMapRoot is cleared (the original does not drop it here). Unbound: the
// LightSetManager reset (byte storage), glitch::collada::CResFileManager::Inst->unloadAll(), the driver's
// CMaterialRendererManager (+0xdc: clearUnusedInstances, removeAll), the CTextureManager (+0xe0: clearPlaceHolders,
// removeAll) and the IShaderManager (+0xd8) vtable slot 12. None of those is declared in a reachable header yet.
void SceneManager::clear() {
    for (ISceneNode* root : m_loadedScenes) {
        root->drop();
    }
    m_loadedScenes.clear();
    glitch::scene::CSceneManager::clear();
    if (unk_438 != nullptr) {
        unk_438->drop();
        unk_438 = nullptr;
    }
    m_nodeMapRoot = nullptr;
}

// 0x354588 SceneManager::DBG_WireFrame(bool,glitch::scene::ISceneNode*)
// PARTIAL, waits on the CMaterial layout. Per material of each node: the renderer (CMaterial +0x04), the technique
// (getTechnique), the first pass of the renderer's technique table (+0x18, +0x08), its state block with word 1 edited by
// dh2::scene_technique::WireFrameWord1, then setRenderState(technique, 0, state).
void SceneManager::DBG_WireFrame(bool enable, ISceneNode* node) {
    (void)enable;
    (void)node;
}

// 0x35477c SceneManager::SetMaskXray(glitch::scene::ISceneNode*,bool)
// PARTIAL, waits on the CMaterial layout. Per material: the raw technique byte (CMaterial +0x08), the first pass of the
// renderer's technique table, its state block edited by dh2::scene_technique::MaskXrayWords, then setRenderState. The
// enable argument is never read by the original.
void SceneManager::SetMaskXray(ISceneNode* node, bool enable) {
    (void)node;
    (void)enable;
}

// 0x3549a0 SceneManager::UpdateLightSet(int,std::vector<bool>&,glitch::scene::ISceneNode*)
// PARTIAL. The original calls LightSetManager::ApplySettings(lightSetId, material, filter) on the light set manager at
// +0x294 for each material. That manager is byte storage in the header, so the call is unbound.
void SceneManager::UpdateLightSet(int lightSetId, std::vector<bool>& filter, ISceneNode* node) {
    (void)lightSetId;
    (void)filter;
    (void)node;
}

// 0x354b2c SceneManager::AutoLoadNormalMap(glitch::scene::ISceneNode*)
// Each material of the node's subtree goes through the material overload (0x352fa8).
void SceneManager::AutoLoadNormalMap(ISceneNode* node) {
    for (ISceneNode* found : SearchMaterialNodes(*this, node != nullptr ? node : rootNode)) {
        const std::uint32_t count = found->getMaterialCount();
        for (std::uint32_t i = 0; i != count; ++i) {
            AutoLoadNormalMap(found->getMaterial(i));
        }
    }
}

// 0x354cb8 SceneManager::SetMaterialEffects(glitch::scene::ISceneNode*,glitch::core::vector3d<float>,float,float)
// Each material of the node's subtree goes through the material overload (0x3527fc).
void SceneManager::SetMaterialEffects(ISceneNode* node, glitch::core::vector3d<float> color, float a, float b) {
    for (ISceneNode* found : SearchMaterialNodes(*this, node != nullptr ? node : rootNode)) {
        const std::uint32_t count = found->getMaterialCount();
        for (std::uint32_t i = 0; i != count; ++i) {
            SetMaterialEffects(found->getMaterial(i), color, a, b);
        }
    }
}

// 0x355000 SceneManager::GetMaterial(glitch::scene::ISceneNode*,std::string const&,bool)
// PARTIAL, waits on the CMaterial layout. The original returns the first material of the node's subtree whose name equals
// the argument (flag clear) or whose first name.size() characters match it (flag set), and null when none does. The
// name comparison needs the material's name (CMaterial +0x1c, a shared string with the text at +4). Until then the
// result is null.
boost::intrusive_ptr<glitch::video::CMaterial> SceneManager::GetMaterial(ISceneNode* node, const std::string& name,
                                                                         bool flag) {
    (void)node;
    (void)name;
    (void)flag;
    return boost::intrusive_ptr<glitch::video::CMaterial>();
}

// 0x3551e4 SceneManager::ChangeCommonTechnique(boost::intrusive_ptr<glitch::video::CMaterial>,bool,bool,bool)
// PARTIAL, waits on the CMaterial layout. The technique name of the material's current technique selects the remap in
// dh2::scene_technique::CommonTechniqueName, using the second flag and SceneManager +0x432 (fog) as the gate.
void SceneManager::ChangeCommonTechnique(boost::intrusive_ptr<glitch::video::CMaterial> material, bool a, bool b,
                                         bool c) {
    (void)material;
    (void)a;
    (void)b;
    (void)c;
}

// 0x355484 SceneManager::ResetCommonTechniqueToDefault(glitch::scene::ISceneNode*,bool,bool,bool)
// PARTIAL, waits on the CMaterialRenderer name (+0x08). Materials whose renderer name does not start with GL_Diffuse,
// GL_Blend3Map, GL_ColorEffects or GL_Decal get ChangeCommonTechnique.
void SceneManager::ResetCommonTechniqueToDefault(ISceneNode* node, bool a, bool b, bool c) {
    (void)node;
    (void)a;
    (void)b;
    (void)c;
}

// 0x355704 SceneManager::ChangeCustomXrayTechnique(boost::intrusive_ptr<glitch::video::CMaterial>,bool,bool,bool)
// PARTIAL, waits on the CMaterial name. The name logic is dh2::scene_technique::CustomXrayTechniqueName. The third flag
// is never read.
void SceneManager::ChangeCustomXrayTechnique(boost::intrusive_ptr<glitch::video::CMaterial> material, bool a, bool b,
                                             bool c) {
    (void)material;
    (void)a;
    (void)b;
    (void)c;
}

// 0x355890 SceneManager::ChangeCustomShadowProjectionTechnique(boost::intrusive_ptr<glitch::video::CMaterial>,bool,bool)
// PARTIAL, waits on the CMaterial name. The name logic is dh2::scene_technique::ShadowProjectionTechniqueName. The second
// flag is never read.
void SceneManager::ChangeCustomShadowProjectionTechnique(boost::intrusive_ptr<glitch::video::CMaterial> material, bool a,
                                                         bool b) {
    (void)material;
    (void)a;
    (void)b;
}

// 0x3559e8 SceneManager::ResetCustomShadowProjectionTechniqueToDefault(glitch::scene::ISceneNode*,bool,bool)
// PARTIAL, waits on the CMaterialRenderer name. Materials whose renderer name does not start with GL_ShadowProjection get
// ChangeCustomShadowProjectionTechnique with the first flag (the original does not pass the second).
void SceneManager::ResetCustomShadowProjectionTechniqueToDefault(ISceneNode* node, bool a, bool b) {
    (void)node;
    (void)a;
    (void)b;
}

// 0x355bf0 SceneManager::ChangeCustomTechnique(boost::intrusive_ptr<glitch::video::CMaterial>,bool,bool,
//                                             SceneManager::EForceBool,bool,bool,bool)
// PARTIAL, waits on the CMaterial and CMaterialRenderer names. The name logic is dh2::scene_technique::CustomTechniqueName,
// fed with the flags and SceneManager +0x430 (lighting), +0x431 (specular) and +0x432 (fog). The seventh flag is never read.
void SceneManager::ChangeCustomTechnique(boost::intrusive_ptr<glitch::video::CMaterial> material, bool a, bool b,
                                         std::int32_t forceBool, bool c, bool d, bool e) {
    (void)material;
    (void)a;
    (void)b;
    (void)forceBool;
    (void)c;
    (void)d;
    (void)e;
}

// 0x356434 SceneManager::ResetCustomTechniqueToDefault(glitch::scene::ISceneNode*,bool,bool,SceneManager::EForceBool,bool,bool,bool)
// PARTIAL, waits on the CMaterialRenderer name. Materials whose renderer name starts with GL_Diffuse, GL_Blend3Map,
// GL_ColorEffects or GL_Decal get ChangeCustomTechnique.
void SceneManager::ResetCustomTechniqueToDefault(ISceneNode* node, bool a, bool b, std::int32_t forceBool, bool c,
                                                 bool d, bool e) {
    (void)node;
    (void)a;
    (void)b;
    (void)forceBool;
    (void)c;
    (void)d;
    (void)e;
}

// 0x3566ec SceneManager::ChangeTechnique(glitch::scene::ISceneNode*,char const*,char const*,int)
// PARTIAL, waits on the CMaterial and CMaterialRenderer layouts. The original returns false when the node has no
// materials, reports a bad index through gAssertLevel (2 stores to address 0, 1 prints an assert line), and otherwise
// checks the current technique name against techniqueB, looks techniqueA up with getTechniqueID (255 when absent) and
// stores the id in the material's technique byte. Until the layouts are reachable the result is false.
bool SceneManager::ChangeTechnique(ISceneNode* node, const char* techniqueA, const char* techniqueB, int pass) {
    (void)node;
    (void)techniqueA;
    (void)techniqueB;
    (void)pass;
    return false;
}

// 0x356bec SceneManager::~SceneManager()  (complete-object destructor; D2 at 0x356878 is the base-object variant)
// PARTIAL. The batch list is released here. Unbound: the LightSetManager destructor on the byte storage at +0x294 (the
// object is never constructed in this layout), the two CSceneManager::SDefaultNodeEntry vectors at +0x464 and +0x470
// (raw bytes in the header, so their storage is not released), and the reset of the second subobject's vptr at +0x48c,
// which the compiler manages. The loaded-scene vector and the base destructor run as members and base.
SceneManager::~SceneManager() {
    if (m_batchBegin != nullptr) {
        GlitchFree(m_batchBegin);
    }
}
