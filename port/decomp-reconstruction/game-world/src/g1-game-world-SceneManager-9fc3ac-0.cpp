// Batch g1-game-world-SceneManager-9fc3ac-0: twenty SceneManager functions (part 1 of the class). Declarations come from
// dh2/SceneManager.hpp.
//
// Reconstructed (eight): both SearchByType and both SearchByName overloads, DisableFog, DBG_PrintNode, ForceRegister and
// setSceneNodeCustomRenderPass. They use only the scene-tree API (CSceneManager, ISceneNode) and the new fog flag.
//
// Partial (twelve). The blockers, in the order they appear in this file:
//   - Engine classes this translation unit can see. The scene headers that SceneManager.hpp includes declare stand-ins for
//     glitch::video::IVideoDriver (empty), CMaterial and CMaterialRenderer (reference count only): glitch-core-io/deps/
//     glitch_core_objects.hpp and glitch-scene-collada-gui/deps/video_stand_ins.hpp. The canonical headers in glitch-video/
//     (material.hpp, material_renderer.hpp, video_driver.hpp) redefine the same classes, so they cannot be included here.
//     The members and methods the bodies need are listed as layout requests in the ledger notes.
//   - Parameter values. The typed IMaterialParameters setParameter<T> and getParameter<T> templates are declared in
//     glitch-video/material_parameters_typed.hpp but are not defined: the mixin integration is not done.
//   - AddNodeToMap calls OptimizeStatic(node), a free function with no declaration in this tree.
//   - The constructor builds m_lightSetManager (LightSetManager::LightSetManager, +0x294) as a member. The base call is reproduced from the
//     constructor's disassembly: the mesh cache argument is null.

#include "dh2/SceneManager.hpp"

#include <cmath>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include "glitch-scene-collada-gui/scene/CEmptySceneNode.hpp"
#include "glitch-scene-collada-gui/scene/ISceneNode.hpp"

// 0x350e5c _ZN12SceneManager12SearchByTypeEPN6glitch5scene10ISceneNodeERSt6vectorIS3_NS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEENS1_17E_SCENE_NODE_TYPEE
// Appends the nodes of the given type below root to out. The original makes a virtual call through vptr+0x20, which is
// CSceneManager::getSceneNodesFromType(type, out, root). Nothing happens without a root.
void SceneManager::SearchByType(glitch::scene::ISceneNode* root, std::vector<glitch::scene::ISceneNode*>& out,
                                std::uint32_t type) {
    if (root) {
        getSceneNodesFromType(static_cast<glitch::scene::E_SCENE_NODE_TYPE>(type), out, root);
    }
}

// 0x350e80 _ZN12SceneManager10DisableFogEPN6glitch5scene10ISceneNodeE
// Clears the fog flag at +0x432. The node argument is not read.
void SceneManager::DisableFog(glitch::scene::ISceneNode* node) {
    (void)node;
    m_lightSetManager.m_fogEnabled = false;
}

// 0x350e8c _ZN12SceneManager13DBG_PrintNodeEPN6glitch5scene10ISceneNodeEbj
// Walks the children of node, recursively, when verbose is set. The original has no print call in this build, so only the
// walk remains.
void SceneManager::DBG_PrintNode(glitch::scene::ISceneNode* node, bool verbose, std::uint32_t depth) {
    if (!node || !verbose) {
        return;
    }
    for (glitch::scene::ISceneNode* child : node->getChildren()) {
        DBG_PrintNode(child, true, depth + 1);
    }
}

// 0x350ee0 _ZN12SceneManager13ForceRegisterEv
// Sets m_forceRegister and the base class visibility-changed byte (CSceneManager +0x289).
void SceneManager::ForceRegister() {
    m_forceRegister = true;
    visibilityChanged = true;
}

// 0x350ef0 _ZN12SceneManager28setSceneNodeCustomRenderPassE31E_SCENE_NODE_CUSTOM_RENDER_PASS
// Stores the new custom render pass and returns the previous one.
std::uint32_t SceneManager::setSceneNodeCustomRenderPass(std::uint32_t pass) {
    const std::uint32_t previous = m_customRenderPass;
    m_customRenderPass = pass;
    return previous;
}

// 0x352154 _ZN12SceneManager14DBG_DrawCircleERK7Point2DIfEffRKN6glitch5video6SColorE
// partial. Sixteen segments of 22.5 degrees at height z, radius in the XY plane. The original keeps (cos, sin) of the
// previous and the current point. The first previous point is angle 0, and the first current point is 22.5 degrees,
// stored as the constants 0.92388 and 0.38268. After each segment the next current point is cosf and sinf of the
// accumulated angle plus one step. The original draws each segment with a virtual draw3DLine call on the driver
// (IVideoDriver vptr+0x1c, with the start point, the end point and the colour). That call is missing: the stand-in
// IVideoDriver declares no members.
void SceneManager::DBG_DrawCircle(const Point2D<float>& center, float z, float radius,
                                  const glitch::video::SColor& color) {
    float prevCos = 1.0f;
    float prevSin = 0.0f;
    float curCos = 0.92388f;
    float curSin = 0.38268f;
    float angle = 0.0f;
    for (int i = 0; i < 16; ++i) {
        const float fromX = center.X + radius * prevCos;
        const float fromY = center.Y + radius * prevSin;
        angle = angle + 0.3927f;
        const float toX = center.X + radius * curCos;
        const float toY = center.Y + radius * curSin;
        (void)fromX;
        (void)fromY;
        (void)toX;
        (void)toY;
        (void)z;
        (void)color;
        if (i == 15) {
            break;
        }
        const float nextCos = std::cos(angle + 0.3927f);
        const float nextSin = std::sin(angle + 0.3927f);
        prevCos = curCos;
        prevSin = curSin;
        curCos = nextCos;
        curSin = nextSin;
    }
}

// 0x3522f8 _ZN12SceneManager9UpdateFogEffRK7Point3DIfE
// partial. When the fog flag is set, the original stores {start, end} as a vector2d<float> with the typed setter on the
// driver's global parameter manager (IVideoDriver +0xe4), under the parameter ID at IVideoDriver +0xfa plus two. The
// store is missing: the typed setter is not defined, and the driver members are not visible here. The position argument
// is not read.
void SceneManager::UpdateFog(float start, float end, const Point3D<float>& position) {
    (void)position;
    if (!m_lightSetManager.m_fogEnabled) {
        return;
    }
    (void)start;
    (void)end;
}

// 0x35233c _ZN12SceneManager9EnableFogEffRK7Point3DIfEPN6glitch5scene10ISceneNodeE
// partial. Sets the fog flag. The original then stores {start, end} as a vector2d<float> at the parameter ID
// IVideoDriver +0xfa + 2, and the colour as an SColor at the parameter ID IVideoDriver +0xfa, both through the typed global
// setter (not defined here). The colour bytes are (uint8)(uint32) of color.X, color.Y, color.Z, and 0. The node argument is
// not read.
void SceneManager::EnableFog(float start, float end, const Point3D<float>& color,
                             glitch::scene::ISceneNode* node) {
    (void)node;
    m_lightSetManager.m_fogEnabled = true;
    const std::uint8_t r = static_cast<std::uint8_t>(static_cast<std::uint32_t>(color.X));
    const std::uint8_t g = static_cast<std::uint8_t>(static_cast<std::uint32_t>(color.Y));
    const std::uint8_t b = static_cast<std::uint8_t>(static_cast<std::uint32_t>(color.Z));
    (void)start;
    (void)end;
    (void)r;
    (void)g;
    (void)b;
}

// 0x3523c4 _ZN12SceneManager12SetCustomFogERN5boost13intrusive_ptrIN6glitch5video9CMaterialEEENS2_4core8vector3dIfEES9_S9_
// partial. The original looks up the parameters fogDirectionMask, fogCameraMask and fogCameraOffset on the material's
// renderer (CMaterialRenderer::getParameterID) and stores a, b and c into them with setParameter<vector3d<float>>, each when
// its ID is not 0xFFFF. The lookups and the stores are missing: CMaterial and CMaterialRenderer are stand-ins here.
void SceneManager::SetCustomFog(boost::intrusive_ptr<glitch::video::CMaterial>& material,
                                glitch::core::vector3d<float> a, glitch::core::vector3d<float> b,
                                glitch::core::vector3d<float> c) {
    (void)material;
    (void)a;
    (void)b;
    (void)c;
}

// 0x3524a0 _ZN12SceneManager12AddNodeToMapEPN6glitch5scene10ISceneNodeE
// partial. On first use it creates the node-map root (CEmptySceneNode(-1)), adds it to the root node, drops the creator's
// reference, and hides it (setVisible(false), vptr+0x48). The node gains a reference. A node that has a parent is moved to
// its absolute position (setPosition, vptr+0xa4) and removed from its parent (remove, vptr+0x68). The node is then added to
// the map root (addChild, vptr+0x5c), and the reference taken above is dropped. Missing: the original calls
// OptimizeStatic(node) before the add, and OptimizeStatic has no declaration in this tree.
void SceneManager::AddNodeToMap(glitch::scene::ISceneNode* node) {
    if (!m_nodeMapRoot) {
        m_nodeMapRoot = new glitch::scene::CEmptySceneNode(-1);
        rootNode->addChild(m_nodeMapRoot);
        m_nodeMapRoot->drop();
        m_nodeMapRoot->setVisible(false);
    }
    ++node->referenceCounter;
    if (node->getParent()) {
        node->setPosition(node->getAbsolutePosition());
        node->remove();
    }
    m_nodeMapRoot->addChild(node);
    node->drop();
}

// 0x3525b4 _ZN12SceneManager18SetMaterialTextureEN5boost13intrusive_ptrIN6glitch5video9CMaterialEEES5_
// partial. The original looks up the Diffuse parameter of the target and of the texture material. When both exist (neither
// is 0xFFFF; the assembly tests both against MOVW #0xFFFF), it reads the texture material's Diffuse texture
// (getParameter<intrusive_ptr<ITexture>>) and, when it is non-null, stores it into the target's Diffuse
// (setParameter<intrusive_ptr<ITexture>>). Missing: the lookups and the typed getter and setter. CMaterial and
// CMaterialRenderer are stand-ins here.
void SceneManager::SetMaterialTexture(boost::intrusive_ptr<glitch::video::CMaterial> target,
                                      boost::intrusive_ptr<glitch::video::CMaterial> texture) {
    (void)target;
    (void)texture;
}

// 0x352668 _ZN12SceneManager34SetShadowProjectionMaterialEffectsEN5boost13intrusive_ptrIN6glitch5video9CMaterialEEENS2_4core8vector3dIfEEff
// partial. Applies only to a material whose renderer name starts with GL_ShadowProjection (renderer +0x08, a plain C string)
// and whose material name starts with SH_ (the shared-string text, count word first). The original looks up Object_Color,
// Object_Alpha and Shadow_Alpha, and stores the colour (when its X is above -0.5) into Object_Color, a into Object_Alpha
// (when above -0.5) and b into Shadow_Alpha (when above -0.5), each only when its ID is not 0xFFFF. Missing: the name tests,
// the lookups and the typed stores (CMaterial and CMaterialRenderer are stand-ins here).
void SceneManager::SetShadowProjectionMaterialEffects(boost::intrusive_ptr<glitch::video::CMaterial> material,
                                                      glitch::core::vector3d<float> color, float a, float b) {
    (void)material;
    (void)color;
    (void)a;
    (void)b;
}

// 0x3527fc _ZN12SceneManager18SetMaterialEffectsEN5boost13intrusive_ptrIN6glitch5video9CMaterialEEENS2_4core8vector3dIfEEff
// partial. Applies only when the renderer name starts with GL_Diffuse, GL_ColorEffects or GL_Xray, and the material name is
// longer than two characters and starts with GL_, CE_ or XR_. The original looks up Object_Color, Object_Saturation and
// Object_Alpha, and stores the colour (when its X is above -0.5) into Object_Color, a into Object_Saturation (when above
// -0.5) and b into Object_Alpha (when above -0.5), each only when its ID is not 0xFFFF. Missing: the name tests, the lookups
// and the typed stores (CMaterial and CMaterialRenderer are stand-ins here).
void SceneManager::SetMaterialEffects(boost::intrusive_ptr<glitch::video::CMaterial> material,
                                      glitch::core::vector3d<float> color, float a, float b) {
    (void)material;
    (void)color;
    (void)a;
    (void)b;
}

// 0x352a2c _ZN12SceneManager15ChangeTechniqueEN5boost13intrusive_ptrIN6glitch5video9CMaterialEEEPKc
// partial. The original takes the material's renderer, returns false for a null name or an unknown technique (getTechniqueID
// returns 255), and returns false when the technique is already the selected one (CMaterial +0x08). Otherwise it stores the
// new technique ID at CMaterial +0x08 and returns true. Missing: the renderer and technique members of CMaterial and the
// getTechniqueID call, since CMaterial and CMaterialRenderer are stand-ins here. The body returns false as a stub.
bool SceneManager::ChangeTechnique(boost::intrusive_ptr<glitch::video::CMaterial> material, const char* technique) {
    (void)material;
    (void)technique;
    return false;
}

// 0x352aa4 _ZN12SceneManager12SearchByNameEPN6glitch5scene10ISceneNodeEPFvS3_ERKSsb
// Calls visit on every node below root whose name matches name: a prefix match of name's length when flag is set, and an
// exact match otherwise. Matching nodes are visited before their children. Does nothing when visit or root is null.
void SceneManager::SearchByName(glitch::scene::ISceneNode* root, void (*visit)(glitch::scene::ISceneNode*),
                                const std::string& name, bool flag) {
    if (!visit || !root) {
        return;
    }
    const bool matched = flag ? std::strncmp(root->getName(), name.data(), name.size()) == 0
                              : std::strcmp(root->getName(), name.c_str()) == 0;
    if (matched) {
        visit(root);
    }
    for (glitch::scene::ISceneNode* child : root->getChildren()) {
        SearchByName(child, visit, name, flag);
    }
}

// 0x352b74 _ZN12SceneManager12SearchByNameEPN6glitch5scene10ISceneNodeERKSsb
// Returns the first node below root, in depth-first order with each node before its children, whose name matches name (a
// prefix match of name's length when flag is set, an exact match otherwise). Returns null when nothing matches or root is
// null.
glitch::scene::ISceneNode* SceneManager::SearchByName(glitch::scene::ISceneNode* root, const std::string& name,
                                                      bool flag) {
    if (!root) {
        return nullptr;
    }
    const bool matched = flag ? std::strncmp(root->getName(), name.data(), name.size()) == 0
                              : std::strcmp(root->getName(), name.c_str()) == 0;
    if (matched) {
        return root;
    }
    for (glitch::scene::ISceneNode* child : root->getChildren()) {
        if (glitch::scene::ISceneNode* found = SearchByName(child, name, flag)) {
            return found;
        }
    }
    return nullptr;
}

// 0x352c3c _ZN12SceneManagerC1EPN6glitch5video12IVideoDriverEN5boost13intrusive_ptrINS0_2io11IFileSystemEEEPNS0_3gui14ICursorControlEPNS9_15IGUIEnvironmentE
// partial. The base call is reproduced from the disassembly: driver, the file system by reference, the cursor control, a
// null IMeshCache (the constant 0 is stored as the fourth argument) and the GUI environment. The base class is partial (see
// CSceneManager.hpp). LightSetManager::LightSetManager (+0x294) builds m_lightSetManager. The vptr stores are emitted by the compiler.
SceneManager::SceneManager(glitch::video::IVideoDriver* driver, boost::intrusive_ptr<glitch::io::IFileSystem> fileSystem,
                           glitch::gui::ICursorControl* cursor, glitch::gui::IGUIEnvironment* gui)
    : glitch::scene::CSceneManager(driver, fileSystem, cursor, nullptr, gui) {
    m_nodeMapRoot = nullptr;                              // +0x28c
    m_screenshotRequested = false;                        // +0x290
    m_registerInterval = 4;                               // +0x444
    unk_45c = 0;                                          // +0x45c
    unk_460 = 1.0f;                                       // +0x460
    unk_438 = nullptr;                                    // +0x438
    unk_43c = false;                                      // +0x43c
    m_registerCounter = 0;                                // +0x440
    m_forceRegister = false;                              // +0x448
    m_loadedScenes.clear();                               // +0x44c .. +0x457
    unk_458 = 0;                                          // +0x458
    std::memset(pad_464, 0, sizeof pad_464);              // +0x464 .. +0x46f
    std::memset(pad_470, 0, sizeof pad_470);              // +0x470 .. +0x47b
    m_batchBegin = nullptr;                               // +0x47c
    m_batchEnd = nullptr;                                 // +0x480
    m_batchCap = nullptr;                                 // +0x484
    m_secondaryRefCount = 1;                              // +0x490
}

// 0x352d4c _ZN12SceneManagerC2EPN6glitch5video12IVideoDriverEN5boost13intrusive_ptrINS0_2io11IFileSystemEEEPNS0_3gui14ICursorControlEPNS9_15IGUIEnvironmentE
// partial, no body of its own. This is the base-object form that takes the VTT. The compiler emits it from the complete
// constructor above (0x352c3c). Its VTT-indexed stores are not checked on the host.

// 0x352fa8 _ZN12SceneManager17AutoLoadNormalMapEN5boost13intrusive_ptrIN6glitch5video9CMaterialEEE
// partial. Only a material whose name starts with GL_, or which is longer than five characters and starts with DV, is handled.
// It looks up Diffuse and Normal. When both exist, the original returns if the material already has a Normal texture, and
// returns if it has no Diffuse texture. It then takes the Diffuse texture's name and replaces its suffix. The first of
// _diffuse.tga, _dif.tga and .tga found becomes _autonm.tga, and an unmatched name throws out_of_range. It looks the new
// name up in the texture manager (reached through the global at dword_99F73C, which is undeclared). When the texture is
// found, it writes N into the material name (byte +7 for GL_, byte +4 for DV) and stores the texture at Normal. Missing:
// every step, since the lookups and the typed texture getter and setter need CMaterial and CMaterialRenderer members that are
// stand-ins here. The body is empty.
void SceneManager::AutoLoadNormalMap(boost::intrusive_ptr<glitch::video::CMaterial> material) {
    (void)material;
}

// 0x3535d0 _ZN12SceneManager12SearchByTypeEPN6glitch5scene10ISceneNodeEPFvS3_ENS1_17E_SCENE_NODE_TYPEE
// Calls visit on every node of the given type below root. The original collects the nodes into a temporary vector with
// getSceneNodesFromType (vptr+0x20) and visits them in order. Does nothing when visit or root is null.
void SceneManager::SearchByType(glitch::scene::ISceneNode* root, void (*visit)(glitch::scene::ISceneNode*),
                                std::uint32_t type) {
    if (!visit || !root) {
        return;
    }
    std::vector<glitch::scene::ISceneNode*> nodes;
    getSceneNodesFromType(static_cast<glitch::scene::E_SCENE_NODE_TYPE>(type), nodes, root);
    for (glitch::scene::ISceneNode* node : nodes) {
        visit(node);
    }
}
