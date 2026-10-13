// Batch g1-game-world-XrayModularSkinnedMeshSceneNode-7b0f5c-0: the thirteen XrayModularSkinnedMeshSceneNode functions
// (two bounding-box forwarders, UpdateXray, updateXrayMaterial, renderMesh, render, onRegisterSceneNode,
// regenerateXrayMaterial, the two constructors and the three destructors). Declarations come from
// dh2/XrayModularSkinnedMeshSceneNode.hpp.
//
// Reconstructed: getBoundingBox, getTransformedBoundingBox.
// Skipped: the deleting destructor (D0 0x36393c), which is the complete destructor followed by CustomFree.
// Partial: UpdateXray, updateXrayMaterial, renderMesh, render, onRegisterSceneNode, regenerateXrayMaterial, the complete
// constructor (C1 0x364174) and the complete destructor (D1 0x363848). Each comment names what is missing. The VTT-taking
// base-object forms (C2 0x364254 and D2 0x362e1c) have no body of their own: the compiler emits them from the complete forms.
//
// Engine declarations that these bodies need are not in this tree (layout requests in the ledger notes):
//   - glitch::IDevice (glitch-core-io/os/device.hpp) declares nothing at +0x10 (the video driver) or +0x1c (the scene
//     manager). The pseudocode reads both through the global dword_99F73C, which is Singleton<Application>::s_inst.m_device.
//   - glitch::collada::CModularSkinnedMeshSceneNode is not a scene::ISceneNode in this tree. The object therefore has no
//     scene manager pointer at +0x110 and no virtual setAbsoluteTransformation (vptr+0x3c) to call on itself.
//   - glitch::collada::CModularSkinnedMesh (the mesh at +0x134) declares none of the virtuals the pass calls with the binary's
//     signatures: getMeshBufferCount (vptr+0x10), getMeshBuffer (+0x14), getMaterial (+0x18), getMaterialVertexAttributeMap
//     (+0x1c), setMaterial (+0x20), vptr+0x24, onPrepareBufferForRendering (+0x38) and releaseProcessBuffer (+0x3c). The
//     mesh word at +0x14 (the buffer bit mask read by renderMesh) is not declared either.
//   - The module type of CModularSkinnedMesh::currentModules (ISkinnedMesh) declares no virtuals (setMaterial is vptr+0x20).
//   - glitch::video::IVideoDriver: the pseudocode calls its setTransform (vptr+0x6c), a state call (vptr+0xd4) and clears a
//     word at +0xe8. None of these are bound here.
//   - SceneManager::ChangeCustomXrayTechnique is declared with three bools after the material. The pseudocode calls pass the
//     material and two flags only, so the declaration needs checking by the integrator.

#include "dh2/XrayModularSkinnedMeshSceneNode.hpp"

#include "dh2/DebugSwitches.hpp"
#include "glitch-scene-collada-gui/collada/IMesh.hpp"
#include "glitch-scene-collada-gui/collada_scene_accessors.hpp"
#include "glitch-scene-collada-gui/modular_skinned_mesh.hpp"
#include "glitch-scene-collada-gui/scene/ISceneNode.hpp"

// The base's constructor and destructor are explicit specializations defined in
// g1-game-world-BaseMeshSceneNode-fb9ec3-0.cpp. They are declared here so this file refers to them as specializations.
template <>
BaseMeshSceneNode<glitch::collada::CModularSkinnedMeshSceneNode>::BaseMeshSceneNode(
        const boost::intrusive_ptr<glitch::collada::IMesh>& mesh);
template <>
BaseMeshSceneNode<glitch::collada::CModularSkinnedMeshSceneNode>::~BaseMeshSceneNode();

// 0x362b28 _ZNK31XrayModularSkinnedMeshSceneNode14getBoundingBoxEv
// Reconstructed. Forwards to m_sourceNode (+0x1c8): the original calls the ISceneNode virtual at vptr+0x30
// (getBoundingBox, K 0x30 in scene/ISceneNode.hpp) on it and returns the reference it gets back.
const glitch::core::aabbox3d<float>& XrayModularSkinnedMeshSceneNode::getBoundingBox() const {
    return m_sourceNode->getBoundingBox();
}

// 0x362b44 _ZNK31XrayModularSkinnedMeshSceneNode25getTransformedBoundingBoxEv
// Reconstructed as found. The original forwards to m_sourceNode the same way as 0x362b28: it calls vptr+0x30
// (getBoundingBox), not vptr+0x34 (getTransformedBoundingBox). This body does the same.
const glitch::core::aabbox3d<float>& XrayModularSkinnedMeshSceneNode::getTransformedBoundingBox() const {
    return m_sourceNode->getBoundingBox();
}

// 0x362bc8 _ZN31XrayModularSkinnedMeshSceneNode10UpdateXrayEv
// partial, empty body. The original builds an identity matrix (16 floats, 1.0 at words 0, 5, 10 and 15) in a 0x41-byte
// argument whose byte at +0x40 is 1, and calls this->setAbsoluteTransformation(matrix) (vptr+0x3c, the ISceneNode
// virtual). This class is not an ISceneNode in this tree, so the call is not bound.
void XrayModularSkinnedMeshSceneNode::UpdateXray() {}

// 0x362c38 _ZN31XrayModularSkinnedMeshSceneNode18updateXrayMaterialEv
// partial, empty body. For each entry i of m_meshMaterials (+0x198; the count is its size), the original calls
// SceneManager::SetMaterialTexture(sm, m_xrayMaterials[i], m_meshMaterials[i]), so the x-ray material is the target and the
// mesh material the texture. sm is the scene manager at dword_99F73C +0x1c, and that IDevice member is not declared (layout
// request), so the loop is not bound.
void XrayModularSkinnedMeshSceneNode::updateXrayMaterial() {}

// 0x362f08 _ZN31XrayModularSkinnedMeshSceneNode10renderMeshEPvb
// partial, empty body. The pseudocode's second argument (char *a2) is a 1-based mesh buffer index: it draws nothing when the
// index is null, and the buffer is a2 - 1. The third argument (bool) is never read. The header's parameter names are kept.
// For index = a2 - 1, when the mesh and the driver are set and getMeshBuffer(index) (mesh vptr+0x14) returns a buffer:
//   - bit (index & 31) of the mesh word at +0x14 is tested. When it is clear, onPrepareBufferForRendering(1, driver, index)
//     (mesh vptr+0x38) runs and its result & 4 is kept. Otherwise the kept flag is 0;
//   - driver setTransform(1, absolute matrix at +0x24) (driver vptr+0x6c);
//   - driver setMaterial(m_xrayMaterials[m_bufferMaterialIndex[index]], m_xrayVertexAttributeMaps[same index]);
//   - when the scene manager's custom render pass (SceneManager::m_customRenderPass, +0x488) is 10, driver vptr+0xd4 (0, 0);
//     the driver word at +0xe8 is cleared;
//   - driver drawMeshBuffer(buffer);
//   - releaseProcessBuffer (mesh vptr+0x3c, driver, index) when the flag was set, then driver vptr+0xd4 (1, 1);
//   - it drops its handles.
// Missing: the driver (dword_99F73C +0x10), the scene manager (dword_99F73C +0x1c), the scene node's +0x110 scene manager,
// and the mesh and driver virtuals listed in the file header.
void XrayModularSkinnedMeshSceneNode::renderMesh(void* renderContext, bool xray) {
    (void)renderContext;
    (void)xray;
}

// 0x363138 _ZN31XrayModularSkinnedMeshSceneNode6renderEPv
// partial. The part reproduced here is the gate: DebugSwitches "RENDERING_EnableXray" is loaded and tested, and the original
// does nothing unless it is set. When the gate is open the original calls UpdateXray, then:
//   - sets the x-ray technique on each m_xrayMaterials entry through SceneManager::ChangeCustomXrayTechnique(sm, material, 1, 1)
//     when the scene manager's custom render pass (+0x488) is 10, and (material, 0, 1) otherwise;
//   - for each mesh buffer i, mesh vptr+0x20 (setMaterial) with (i, m_xrayMaterials[m_bufferMaterialIndex[i]],
//     m_xrayVertexAttributeMaps[...], 1);
//   - for each current module j, module vptr+0x20 with (0, x-ray material and map at m_nodeMaterialIndex[j]);
//   - renderMesh(this, a2, false), where a2 is render's own second argument, passed through as found;
//   - restores the mesh and module materials from m_meshMaterials and m_meshVertexAttributeMaps the same way.
// The two local vectors the original destroys at the end are always empty. Missing: the scene manager (see the file header),
// the mesh and module virtuals, and the scene manager's custom render pass read.
void XrayModularSkinnedMeshSceneNode::render(void* renderContext) {
    DebugSwitches::s_inst.load();
    if (!DebugSwitches::s_inst.GetSwitch("RENDERING_EnableXray")) {
        return;
    }
    UpdateXray();
    (void)renderContext;
}

// 0x363574 _ZN31XrayModularSkinnedMeshSceneNode19onRegisterSceneNodeEv
// partial. Returns 1 on every path, as the original does. When DebugSwitches "RENDERING_EnableXray" is set, it calls
// UpdateXray. Then, when the mesh at +0x134 is set and the same switch is set again, the original walks the mesh buffers i.
// For each i whose getMeshBuffer(i) (mesh vptr+0x14) is non-null, it takes m_xrayMaterials[m_bufferMaterialIndex[i]] (one
// extra use) and calls onPrepareBufferForRendering(0, driver, i) (mesh vptr+0x38), where driver is the +0x110 scene manager's
// +0x14 word. A result of 4 or 16 calls the scene manager's vptr+0x8c (SceneManager virtual, slot 140) twice, with pass 9 and
// then 10. A result of 5 calls mesh vptr+0x24 with no arguments. The walk is not bound: the scene node has no +0x110 scene
// manager in this tree, and the mesh and scene manager virtuals are not declared.
int XrayModularSkinnedMeshSceneNode::onRegisterSceneNode() {
    DebugSwitches::s_inst.load();
    if (DebugSwitches::s_inst.GetSwitch("RENDERING_EnableXray")) {
        UpdateXray();
        if (mesh) {
            DebugSwitches::s_inst.load();
            if (DebugSwitches::s_inst.GetSwitch("RENDERING_EnableXray")) {
                // partial: the mesh buffer walk described above is not reproduced.
            }
        }
    }
    return 1;
}

// 0x363848 _ZN31XrayModularSkinnedMeshSceneNodeD1Ev
// partial, empty body. The members are destroyed in reverse declaration order (+0x1bc, +0x1b0, +0x1a4, +0x198, +0x18c, +0x180),
// the same order the original uses for its six vectors. Not reproduced: the stores of this class's vptr and of the IRC
// virtual-base vptr at +0x1cc (unk_1cc), which have no host counterpart, and the explicit call of the
// CModularSkinnedMeshSceneNode destructor. The raw source node at +0x1c8 is not released, as in the original.
XrayModularSkinnedMeshSceneNode::~XrayModularSkinnedMeshSceneNode() {}

// 0x36393c _ZN31XrayModularSkinnedMeshSceneNodeD0Ev
// skipped: compiler-generated deleting destructor. It is the complete destructor (0x363848) followed by CustomFree(this).
// A delete expression does the same.

// 0x362e1c _ZN31XrayModularSkinnedMeshSceneNodeD2Ev
// partial, no body of its own. This is the base-object destructor that takes the VTT (a2). The compiler emits it from the
// complete destructor above. Its VTT-indexed vptr stores and the call to the CModularSkinnedMeshSceneNode base-object destructor
// with a2+1 are not checked on the host.

// 0x363958 _ZN31XrayModularSkinnedMeshSceneNode22regenerateXrayMaterialEv
// partial, empty body. The original does this:
//   1. clears m_meshMaterials, m_meshVertexAttributeMaps, m_bufferMaterialIndex, m_xrayMaterials and m_xrayVertexAttributeMaps,
//      releasing the handles they hold;
//   2. opens gl_xray.bdae (CColladaDatabase with DefaultFactory) and constructs the effect GL_Xray-fx on the driver (IDevice
//      +0x10, root 0), keeping its CMaterialRenderer;
//   3. resizes m_bufferMaterialIndex to the mesh buffer count (mesh vptr+0x10), m_meshMaterials and m_meshVertexAttributeMaps
//      likewise. For each buffer i it scans the entries k of m_meshMaterials: an empty entry takes getMaterial(i) (mesh vptr+0x18)
//      and its getMaterialVertexAttributeMap(i) (vptr+0x1c) goes to m_meshVertexAttributeMaps[k]; the first entry equal to
//      getMaterial(i) gives m_bufferMaterialIndex[i] = k;
//   4. resizes m_xrayMaterials and m_xrayVertexAttributeMaps to the size of m_meshMaterials, and m_nodeMaterialIndex to the
//      current module count (CModularSkinnedMesh::currentModules at +0x24, 8-byte records). For each module j it stores in
//      m_nodeMaterialIndex[j] the entry of m_meshMaterials that equals the module's getMaterial(0) (module vptr+0x18);
//   5. for each entry i of m_meshMaterials that is set, allocates a CMaterial (CMaterial::allocate with the renderer and the
//      name XR_XRAY_SKINNED_MODULAR, count 0) into m_xrayMaterials[i], calls SceneManager::ChangeCustomXrayTechnique(sm,
//      material, 1, 1), and when m_meshVertexAttributeMaps[i] is set allocates a CMaterialVertexAttributeMap into
//      m_xrayVertexAttributeMaps[i] from the x-ray material's +4 word.
// Missing: the driver (IDevice +0x10) and scene manager (IDevice +0x1c) layouts; the mesh and module virtuals (CModularSkinnedMesh
// and ISkinnedMesh declare none with these signatures); CColladaDatabase::constructEffect, which returns a raw pointer in the
// stand-in where the original writes an intrusive_ptr through its first argument; and the meaning of the +4 argument of
// CMaterialVertexAttributeMap::allocate, which is not identified.
void XrayModularSkinnedMeshSceneNode::regenerateXrayMaterial() {}

// 0x364174 _ZN31XrayModularSkinnedMeshSceneNodeC1EN5boost13intrusive_ptrIN6glitch7collada5IMeshEEEPKNS2_5scene10ISceneNodeE
// partial. The original stores 1 at +0x1d0 (unk_1d0), stores the vptrs of the IRC virtual subobject (+0x1cc) and of this class,
// calls the BaseMeshSceneNode base-object constructor with the mesh, stores the source node at +0x1c8, zeroes the six vectors and
// index vectors (the members' own initialisers do that here), and calls regenerateXrayMaterial. Not reproduced: the vptr stores.
// The base constructor is partial because it drops the mesh (see g1-game-world-BaseMeshSceneNode-fb9ec3-0.cpp), and
// regenerateXrayMaterial is partial, so the object has no x-ray materials yet.
XrayModularSkinnedMeshSceneNode::XrayModularSkinnedMeshSceneNode(boost::intrusive_ptr<glitch::collada::IMesh> mesh,
                                                                 const glitch::scene::ISceneNode* sourceNode)
    : BaseMeshSceneNode<glitch::collada::CModularSkinnedMeshSceneNode>(mesh), m_sourceNode(sourceNode) {
    unk_1d0 = 1;
    regenerateXrayMaterial();
}

// 0x364254 _ZN31XrayModularSkinnedMeshSceneNodeC2EN5boost13intrusive_ptrIN6glitch7collada5IMeshEEEPKNS2_5scene10ISceneNodeE
// partial, no body of its own. This is the base-object constructor that takes the VTT (a2), the mesh and the source node. The
// compiler emits it from the complete constructor above. Its VTT-indexed vptr stores (the IRC virtual base) and the call to the
// BaseMeshSceneNode base-object constructor with a2+1 are not checked on the host.
