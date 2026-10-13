// SceneManager, batch g1-game-world-SceneManager-9fc3ac-2 (part 3 of 3). Functions in address order:
//   0x356c9c ~SceneManager (D0)            skipped: compiler-generated deleting destructor
//   0x357780 renderCustomList<SDefaultNodeEntry>   partial
//   0x357dc0 clearRenderLists              partial
//   0x357ea4 _registerSceneNodes           partial
//   0x358008 renderCustomLists             partial
//   0x3586ac SearchByName(vector&, string) reconstructed
//   0x3587a0 _renderLists                  partial
//   0x3592b0 _drawAll                      reconstructed
//   0x359338 drawAll                       partial
//   0x3594a4 LoadFXLib                     partial
//   0x3596f8 LoadScene                     partial
//   0x359a38 AddSkyBoxSceneNode            partial
//   0x359b68 PreloadScene                  reconstructed
//   0x359c60 GetMaterial(node, char*)      reconstructed
//   0x359d08 SetMaterialRimLightEffects(material) partial
//   0x359fac SearchByName(vector&, char*)  reconstructed
//   0x35a044 SearchByName(visit, char*)    reconstructed
//   0x35a0e4 SearchByName(char*) -> node   reconstructed
//   0x35a188 SetMaterialRimLightEffects(node) reconstructed
//   0x35a3d8 registerNodeForCustomRendering partial
//
// The render lists of the engine base (CSceneManager +0x30 .. +0x8f) are not declared in the canonical header
// (glitch-scene-collada-gui/scene/CSceneManager.hpp keeps them as unk_30[0x60]), so every function that walks or
// fills them is partial. See the layout requests in the ledger notes.

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include "dh2/Application.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/Level.hpp"
#include "dh2/SceneManager.hpp"
#include "dh2/batch/BatchNodeCompiler.hpp"
#include "dh2/batch/GOBatchSceneNode.hpp"

// 0x357780 void SceneManager::renderCustomList<glitch::scene::CSceneManager::SDefaultNodeEntry>(
//              E_SCENE_NODE_CUSTOM_RENDER_PASS, std::vector<SDefaultNodeEntry, SAllocator>&)
// PARTIAL. Rebuilt: the pass bookkeeping (renderPass at +0x174 is the pass when it is 8 or less, otherwise 9;
// m_customRenderPass at +0x488 is the pass) and the final resize of the list to zero. Not rebuilt:
//  - the walk over the entries. The original appends a zeroed sentinel entry, moves each entry into the current and
//    next rendered-node state (CSceneManager +0x9c .. +0xb0) and calls the entry node's vtable slot +0x1c
//    (ISceneNode::render). The pseudocode shows that call with no argument, so the render context is not recovered
//    and the call is not made.
//  - the state words. CSceneManager +0x9c and +0xa8 are typed std::uint32_t in the canonical header, but the first
//    word of an entry is the ISceneNode pointer, which does not fit a 32-bit word on the host. Layout request.
template <>
void SceneManager::renderCustomList<glitch::scene::CSceneManager::SDefaultNodeEntry>(
        std::uint32_t pass, std::vector<glitch::scene::CSceneManager::SDefaultNodeEntry>& list) {
    renderPass = pass <= 8 ? pass : 9u;     // +0x174
    m_customRenderPass = pass;              // +0x488
    list.clear();                           // the original resizes the list to zero at the end
}

// 0x357dc0 SceneManager::clearRenderLists()
// The original resizes the seven render-list vectors of the engine base to zero, in this order: +0x3c (unsorted entries),
// +0x48 (distance entries), +0x54 and +0x60 (render-data sort entries), +0x6c (second unsorted list), +0x78 (default
// entries) and +0x84 (transparent entries). The canonical CSceneManager header declares each of them.
void SceneManager::clearRenderLists() {
    cameraNodes.clear();          // +0x3c
    lightNodes.clear();           // +0x48
    shadowVolumeNodes.clear();    // +0x54
    shadowMeshNodes.clear();      // +0x60
    preSolidNodes.clear();        // +0x6c
    defaultNodes.clear();         // +0x78
    transparentNodes.clear();     // +0x84
}

// 0x357ea4 SceneManager::_registerSceneNodes(glitch::scene::ISceneNode*)
// PARTIAL. Rebuilt: the frame counter, the needRegister carry (a function-local static), the GetCurrentLevel query, the
// full-registration branch (clearRenderLists, the batch list reset, CSceneManager::registerSceneNodes and the flag
// resets) and the level's batch-visibility update. Not rebuilt:
//  - byte_9A600C: the original tests (needRegister || byte_9A600C) and stores byte_9A600C ? 1 : m_forceRegister in
//    needRegister. The global has no name in the tree, so only the needRegister term is modelled. The result is exact
//    when byte_9A600C is 0.
//  - the batch list walk: the original calls vtable slot +0x00 of every element from m_batchBegin to m_batchEnd with the
//    element as the only argument. The element type of the void** list is not declared, so the walk is omitted.
void SceneManager::_registerSceneNodes(glitch::scene::ISceneNode* node) {
    static bool needRegister = false;       // SceneManager::_registerSceneNodes::needRegister

    ++m_registerCounter;                    // +0x440
    if (needRegister) {
        m_registerCounter = 0;
    }
    needRegister = m_forceRegister;         // +0x448

    Level* currentLevel = ApplicationGetInstance().GetCurrentLevel();

    if (visibilityChanged || m_registerCounter % m_registerInterval == 0) {   // +0x289, +0x440 and +0x444
        clearRenderLists();
        m_batchEnd = m_batchBegin;          // +0x480 = +0x47c when they differ
        glitch::scene::CSceneManager::registerSceneNodes(node);
        m_forceRegister = false;            // +0x448
        m_registerCounter = 0;              // +0x440
        visibilityChanged = false;          // +0x289
    } else {
        if (currentLevel) {
            batch::BatchNodeCompiler* compiler = currentLevel->m_batchNodeCompiler;   // Level +0x158
            if (compiler) {
                batch::GOBatchSceneNode* batchNode = compiler->m_batchNode;            // BatchNodeCompiler +0x34
                if (batchNode) {
                    batchNode->updateSegmentVisibility();
                }
            }
        }
        // PARTIAL: the batch list walk (see the header comment) is not made.
    }
}

// 0x358008 SceneManager::renderCustomLists(glitch::video::IVideoDriver*)
// PARTIAL: stub. The original sorts and renders the unsorted, distance, default, render-data sort and transparent
// lists of the engine base (CSceneManager +0x3c .. +0x84, see clearRenderLists), calls the driver's deleteAllDynamicLights
// and the driver's vtable +0xa8 slot, and then clears the deletion list. None of those lists or the driver slot are
// declared, so the body is empty. Layout request.
void SceneManager::renderCustomLists(glitch::video::IVideoDriver* driver) {
    (void)driver;
}

// 0x3586ac SceneManager::SearchByName(glitch::scene::ISceneNode*, std::vector<ISceneNode*>&, std::string const&, bool)
// Preorder search of the subtree under root. A node whose name matches is appended to out before its children are
// searched. The flag selects a prefix match (strncmp over the length of name) or an exact match (strcmp).
void SceneManager::SearchByName(glitch::scene::ISceneNode* root, std::vector<glitch::scene::ISceneNode*>& out,
                                const std::string& name, bool flag) {
    if (!root) {
        return;
    }
    const char* nodeName = root->getName();
    const bool matched = flag ? std::strncmp(nodeName, name.c_str(), name.size()) == 0
                              : std::strcmp(nodeName, name.c_str()) == 0;
    if (matched) {
        out.push_back(root);
    }
    for (glitch::scene::ISceneNode* child : root->getChildren()) {
        SearchByName(child, out, name, flag);
    }
}

// 0x3587a0 SceneManager::_renderLists(glitch::video::IVideoDriver*)
// PARTIAL: stub. The original heap-sorts the render lists, renders the unsorted, distance, default, render-data sort
// and transparent entries with their current and next rendered nodes (CSceneManager +0x9c .. +0xb0), switches the
// driver's render states (driver vtable +0xa0 and +0xd4) around each material and node, and clears the deletion list.
// The lists, the state words and the driver slots are not declared (see clearRenderLists and renderCustomList). Empty body.
void SceneManager::_renderLists(glitch::video::IVideoDriver* driver) {
    (void)driver;
}

// 0x3592b0 SceneManager::_drawAll(glitch::scene::ISceneNode*)
// Calls the virtual drawInit on the driver (vptr+0x40), collects every node when no node was given and the hierarchy
// changed, runs setupCamera (vptr+0x44), registers the scene nodes, registers the automatic lights, runs
// drawShadowReceivers (vptr+0x4c), renders the lists and leaves the render pass at 9.
void SceneManager::_drawAll(glitch::scene::ISceneNode* node) {
    drawInit(driverCopy);                                                  // +0x18 driver, vptr+0x40
    if (!node && hierarchyChanged) {                                       // +0x288
        glitch::scene::CSceneManager::collectAllNodes();
    }
    setupCamera();                                                         // vptr+0x44
    _registerSceneNodes(node);
    _RegisterAutomacticLights();
    drawShadowReceivers();                                                 // vptr+0x4c
    _renderLists(driverCopy);
    renderPass = 9u;                                                       // +0x174
}

// 0x359338 SceneManager::drawAll(glitch::scene::ISceneNode*)
// PARTIAL. Rebuilt: the light set update (LightSetManager::Update on m_lightSetManager, +0x294), the frame draw (_drawAll)
// and the screenshot request (the flag at +0x290 is cleared, the ToggleGamepadScreenShot debug switch is read after
// DebugSwitches::load). Not rebuilt:
//  - the screenshot file. The original names it screenshot_<time>.tga, takes the frame from the driver's createScreenShot
//    (vptr+0x98) and writes it through CImageWriterTGA. The original passes three arguments to glitch::io::createWriteFile;
//    the canonical declaration takes two (name, append), and the third argument is not recovered. CImageWriterTGA's write
//    slot (vptr+0x10) is not confirmed against the canonical header.
void SceneManager::drawAll(glitch::scene::ISceneNode* node) {
    m_lightSetManager.Update();                                            // +0x294 light set update, before the draw
    _drawAll(node);
    if (m_screenshotRequested) {                                           // +0x290
        m_screenshotRequested = false;
        DebugSwitches::s_inst.load();
        const std::string switchName("ToggleGamepadScreenShot");
        if (DebugSwitches::s_inst.GetSwitch(switchName)) {
            // PARTIAL: the screenshot is not written (see the header comment).
        }
    }
}

// 0x3594a4 SceneManager::LoadFXLib(char const*, char const*)
// PARTIAL: stub. The original builds a glitch::collada::CColladaDatabase on the file name (second argument) with
// ColladaFactory::s_factory, appends "-fx" to the effect name when it is missing, constructs the effect through the
// database (constructEffect) and returns it through the hidden result slot (an intrusive pointer to a material
// renderer). The header declares void, and the assert branch (gAssertLevel, fprintf) has no declared counterpart.
// Empty body.
void SceneManager::LoadFXLib(const char* fileA, const char* fileB) {
    (void)fileA;
    (void)fileB;
}

// 0x3596f8 SceneManager::LoadScene(char const*, char const*, bool, bool)
// PARTIAL: stub. The original constructs a node (constructNode with "<name>-node" when a node name is given, otherwise
// constructScene) through the collada database held at dword_99F73C + 0x10. The canonical
// CColladaDatabase::constructNode takes (driver, SNode*, root), which does not match the (database, file, name, factory)
// call in the pseudocode, and the owner of that database is not declared. The original then stores the file name and the
// node name on the new root (RootSceneNode m_fileName and m_name), runs ResetPositionFromFile when flagB is set, attaches
// an animator (constructAnimator), turns automatic culling on, and for a "_module" node removes every child whose name
// contains none of "_minimap", "_mesh", "_floor" and "_exit". It adds the node to the scene root when flagA is set.
// Returns null here.
glitch::scene::ISceneNode* SceneManager::LoadScene(const char* file, const char* nodeName, bool flagA, bool flagB) {
    (void)file;
    (void)nodeName;
    (void)flagA;
    (void)flagB;
    return nullptr;
}

// 0x359a38 SceneManager::AddSkyBoxSceneNode(char const*, char const*)
// PARTIAL. Rebuilt: the early exit on an empty file name and the load of the sky scene (LoadScene with flagA clear and
// flagB set). Not rebuilt: the sky node lookup result is not used, so the sky box is not created. The original then
// (1) removes the previous sky box held at +0x438 (unk_438, which holds an ISceneNode and is removed through vptr+0x68),
// (2) takes the sky node's mesh through IMeshSceneNode::getMesh (vptr+0xf8), (3) builds a SkyBoxMeshSceneNode on it,
// (4) adds it to the root (vptr+0x5c), stores it in unk_438 and drops the loaded scene. unk_438 is declared as
// glitch::IReferenceCounted and SkyBoxMeshSceneNode.hpp derives from the legacy accessor stub, so the steps do not compile
// against the current declarations.
void SceneManager::AddSkyBoxSceneNode(const char* fileA, const char* fileB) {
    if (*fileA == '\0') {
        return;
    }
    glitch::scene::ISceneNode* loaded = LoadScene(fileA, fileB, false, true);
    if (!loaded) {
        return;
    }
    // PARTIAL: the sky lookup and the sky box construction are not made (see the header comment).
    (void)getSceneNodeFromType(static_cast<glitch::scene::E_SCENE_NODE_TYPE>(0x6D656164u), loaded);   // 'daem'
}

// 0x359b68 SceneManager::PreloadScene(char const*, char const*)
// Loads a scene without attaching it to the root and remembers its root node in m_loadedScenes.
void SceneManager::PreloadScene(const char* file, const char* nodeName) {
    glitch::scene::ISceneNode* scene = LoadScene(file, nodeName, false, false);
    if (scene) {
        m_loadedScenes.push_back(scene);
    }
}

// 0x359c60 SceneManager::GetMaterial(glitch::scene::ISceneNode*, char const*, bool)
// A null node or a null name gives a null material (the original writes 0 through its hidden result slot); otherwise the result of the std::string overload (0x355000).
boost::intrusive_ptr<glitch::video::CMaterial> SceneManager::GetMaterial(glitch::scene::ISceneNode* node, const char* name, bool flag) {
    if (!node || !name) {
        return boost::intrusive_ptr<glitch::video::CMaterial>();
    }
    return GetMaterial(node, std::string(name), flag);
}

// 0x359d08 SceneManager::SetMaterialRimLightEffects(boost::intrusive_ptr<glitch::video::CMaterial>, float, std::string)
// PARTIAL: stub. The original applies the rim light to a material whose renderer technique name starts with GL_Diffuse
// or GL_Xray and whose material name starts with GL_ or XR_ (after a 4-byte prefix skip). It sets the RimLight_Level
// parameter to the power when the power is above -0.5, and loads the texture "data/3d/textures/<tag>" (through
// CTextureManager::getTexture on the texture manager held at +0xe0 of the object at dword_99F73C + 0x10) into the
// RimLight parameter unless the current texture is char_rim_light_off.tga. The parameter access and the reference
// handling of the texture slot are not confirmed against the canonical material headers. Empty body.
void SceneManager::SetMaterialRimLightEffects(boost::intrusive_ptr<glitch::video::CMaterial> material, float power,
                                              std::string tag) {
    (void)material;
    (void)power;
    (void)tag;
}

// 0x359fac SceneManager::SearchByName(glitch::scene::ISceneNode*, std::vector<ISceneNode*>&, char const*, bool)
void SceneManager::SearchByName(glitch::scene::ISceneNode* root, std::vector<glitch::scene::ISceneNode*>& out,
                                const char* name, bool flag) {
    if (!name || !root) {
        return;
    }
    SearchByName(root, out, std::string(name), flag);
}

// 0x35a044 SceneManager::SearchByName(glitch::scene::ISceneNode*, void (*)(glitch::scene::ISceneNode*), char const*, bool)
void SceneManager::SearchByName(glitch::scene::ISceneNode* root, void (*visit)(glitch::scene::ISceneNode*),
                                const char* name, bool flag) {
    if (!visit || !root) {
        return;
    }
    if (name) {
        SearchByName(root, visit, std::string(name), flag);
    }
}

// 0x35a0e4 SceneManager::SearchByName(glitch::scene::ISceneNode*, char const*, bool)
// Returns the first node found by the string overload, or null when the name or the root is null.
glitch::scene::ISceneNode* SceneManager::SearchByName(glitch::scene::ISceneNode* root, const char* name, bool flag) {
    if (!name || !root) {
        return nullptr;
    }
    return SearchByName(root, std::string(name), flag);
}

// 0x35a188 SceneManager::SetMaterialRimLightEffects(glitch::scene::ISceneNode*, float, std::string)
// Applies the rim light to every material of the nodes of the three character types under node (the root when node is
// null). The type tags are 'daem' (0x6d656164), 'daes' (0x73656164) and 'daeM' (0x4d656164), packed as fourcc values.
void SceneManager::SetMaterialRimLightEffects(glitch::scene::ISceneNode* node, float power, std::string tag) {
    glitch::scene::ISceneNode* root = node ? node : rootNode;             // +0x04 when node is null
    std::vector<glitch::scene::ISceneNode*> found;
    SearchByType(root, found, 0x6D656164u);
    SearchByType(root, found, 0x73656164u);
    SearchByType(root, found, 0x4D656164u);
    for (glitch::scene::ISceneNode* current : found) {
        const int count = static_cast<int>(current->getMaterialCount());
        for (int i = 0; i < count; ++i) {
            boost::intrusive_ptr<glitch::video::CMaterial> material =
                    current->getMaterial(static_cast<std::uint32_t>(i));
            SetMaterialRimLightEffects(material, power, tag);
        }
    }
}

// 0x35a3d8 SceneManager::registerNodeForCustomRendering(glitch::scene::ISceneNode*, boost::intrusive_ptr<CMaterial> const&,
//              void*, E_SCENE_NODE_CUSTOM_RENDER_PASS, glitch::core::vector3d<float> const*, int)
// PARTIAL: stub. The original returns 1 when the node was filed into a list for the pass and 0 otherwise. Passes 0, 1,
// 2, 5 and 6 file into the engine-base lists (pass 0 first rejects a node already in its unsorted list). Pass 4 files into
// the default list, passes 3 and 7 into the transparent or the default list (chosen by the material technique and the
// byte at +0x28a), and passes 9 and 10 into the game lists at +0x464 and +0x470. The render lists and their entry types
// are not declared, the game's +0x464 and +0x470 lists are byte arrays in SceneManager.hpp, and the header declares void.
// Empty body.
void SceneManager::registerNodeForCustomRendering(glitch::scene::ISceneNode* node,
                                                  const boost::intrusive_ptr<glitch::video::CMaterial>& material,
                                                  void* userData, std::uint32_t pass,
                                                  const glitch::core::vector3d<float>* position, int sortOrder) {
    (void)node;
    (void)material;
    (void)userData;
    (void)pass;
    (void)position;
    (void)sortOrder;
}
