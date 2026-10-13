// The CSceneManager virtuals that are not reconstructed in full, plus the constructor, destructor and nested node entries.
// This file defines the key function (serializeAttributes), so it carries the vtable; see src/scene/CSceneManager.cpp for the
// accessors and the faithful bodies. Every function here is PARTIAL unless its comment says otherwise: the note names what
// the original does that this body does not (the driver, GUI and batch-driver types are not ported).
#include "glitch-scene-collada-gui/scene/CSceneManager.hpp"

#include "glitch-core-io/io/iattributes.hpp"
#include "glitch-core-io/os/logging.hpp"
#include "glitch-scene-collada-gui/scene/CEmptySceneNode.hpp"
#include "glitch-scene-collada-gui/scene/ICameraSceneNode.hpp"
#include "glitch-scene-collada-gui/scene/IMesh.hpp"
#include "glitch-scene-collada-gui/scene/ISceneNode.hpp"
#include "glitch-scene-collada-gui/scene/SceneFactories.hpp"

namespace glitch::scene {

namespace {
// Sentinel passed to update() to reset the clock from the timer.
constexpr float kUpdateResetTime = -123460.0f;
}  // namespace

// 0x58d908 _ZN6glitch5scene13CSceneManagerC1EPNS_5video12IVideoDriverERKN5boost13intrusive_ptrINS_2io11IFileSystemEEEPNS_3gui14ICursorControlEPNS0_10IMeshCacheEPNS7_15IGUIEnvironmentE (complete object)
// 0x58ddd0 _ZN6glitch5scene13CSceneManagerC2EPNS_5video12IVideoDriverERKN5boost13intrusive_ptrINS_2io11IFileSystemEEEPNS_3gui14ICursorControlEPNS0_10IMeshCacheEPNS7_15IGUIEnvironmentE (base object)
// PARTIAL. Stores the driver, file system, GUI environment, cursor control and mesh cache arguments, creates the root node
// (CSceneManagerRootNode linked into the +0x08 child list), and sets the shadow colour -1778384896 (0x96000000, the word the
// constructor passes) and the render pass 9. Missing: the references the original adds to the driver, GUI environment, cursor control
// and mesh cache (their classes are not ported), the CBatchDriver (new 0x264 bytes), the default mesh cache, the
// CSceneCollisionManager, the CColladaBinaryFileLoader and the two default factories it registers, and the "i", "n", "t" strings.
CSceneManager::CSceneManager(video::IVideoDriver* videoDriver, const boost::intrusive_ptr<io::IFileSystem>& fileSystemArg,
                             gui::ICursorControl* cursorControlArg, IMeshCache* meshCacheArg,
                             gui::IGUIEnvironment* guiEnvironmentArg)
    : fileSystem(fileSystemArg) {
    driver = videoDriver;
    driverCopy = videoDriver;
    guiEnvironment = guiEnvironmentArg;
    cursorControl = cursorControlArg;
    meshCache = meshCacheArg;
    auto* root = new CSceneManagerRootNode(this);
    rootNode = root;
    SListLink* rootLink = static_cast<SListLink*>(static_cast<ISceneNode*>(root));
    SListLink* tail = rootChildren.sentinel.prev;
    rootLink->prev = tail;
    tail->next = rootLink;
    rootChildren.sentinel.prev = rootLink;
    rootLink->next = &rootChildren.sentinel;
    ++rootChildren.count;
    setShadowColor(video::SColor{0x96000000u});
}

// 0x58eb70 _ZN6glitch5scene13CSceneManagerD1Ev
// 0x58ef1c _ZN6glitch5scene13CSceneManagerD2Ev
// 0x58ef00 _ZN6glitch5scene13CSceneManagerD0Ev (skipped)
// PARTIAL. Clears the deletion list, drops the held factories, loaders and the active camera, unlinks the root child list.
// Missing: the drops of the driver, batch driver, GUI environment, cursor control, collision manager and mesh cache (their
// classes are not ported), the attribute set and the wide strings, and the render-node lists of the derived managers.
CSceneManager::~CSceneManager() {
    clearDeletionList();
    for (IMeshLoader* loader : externalMeshLoaders) {
        if (loader != nullptr) loader->drop();
    }
    externalMeshLoaders.clear();
    if (activeCamera != nullptr) {
        activeCamera->drop();
        activeCamera = nullptr;
    }
    for (ISceneNodeFactory* factory : sceneNodeFactories) {
        if (factory != nullptr) factory->drop();
    }
    sceneNodeFactories.clear();
    for (ISceneNodeAnimatorFactory* factory : sceneNodeAnimatorFactories) {
        if (factory != nullptr) factory->drop();
    }
    sceneNodeAnimatorFactories.clear();
    SListLink* link = rootChildren.sentinel.next;
    while (link != &rootChildren.sentinel) {
        SListLink* following = link->next;
        link->next = nullptr;
        link->prev = nullptr;
        link = following;
    }
    rootChildren.sentinel.next = &rootChildren.sentinel;
    rootChildren.sentinel.prev = &rootChildren.sentinel;
    rootChildren.count = 0;
}

// 0x588fe8 _ZNK6glitch5scene13CSceneManager19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Writes the root node's name ("Name"), its id ("Id") and the ambient light ("AmbientLight", four floats).
void CSceneManager::serializeAttributes(io::IAttributes* attributes, io::SAttributeReadWriteOptions* /*options*/) const {
    attributes->addString("Name", rootNode->getName(), false);
    attributes->addInt("Id", rootNode->getID(), false);
    attributes->addColorf("AmbientLight", ambientLight, false);
}

// 0x58bbc4 _ZN6glitch5scene13CSceneManager21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Reads "Name", "Id" and "AmbientLight" into the root node and the ambient light, then resets the root's transform (zero
// position, identity rotation, unit scale), makes it visible, clears culling and the debug flag, and updates its matrix.
void CSceneManager::deserializeAttributes(io::IAttributes* attributes, io::SAttributeReadWriteOptions* /*options*/) {
    rootNode->setName(attributes->getString("Name"));
    rootNode->setID(attributes->getInt("Id"));
    ambientLight = attributes->getColorf("AmbientLight");
    rootNode->setPosition(core::vector3d<float>{0.0f, 0.0f, 0.0f});
    core::quaternion identity;
    identity.X = 0.0f;
    identity.Y = 0.0f;
    identity.Z = 0.0f;
    identity.W = 1.0f;
    rootNode->setRotation(identity);
    rootNode->setScale(core::vector3d<float>{1.0f, 1.0f, 1.0f});
    rootNode->setVisible(true);
    rootNode->setAutomaticCulling(static_cast<E_CULLING_TYPE>(0));
    rootNode->setIsDebugObject(false);
    rootNode->updateAbsolutePosition(false);
}

// 0x58f348 _ZN6glitch5scene13CSceneManager24registerNodeForRenderingEPNS0_10ISceneNodeERKN5boost13intrusive_ptrINS_5video9CMaterialEEEPvNS0_24E_SCENE_NODE_RENDER_PASSEPKNS_4core8vector3dIfEEi
// PARTIAL. The original is a switch on the render pass that appends SDefaultNodeEntry, SDistanceNodeEntry and
// STransparentNodeEntry records to the vectors at +0x30 .. +0x84. Missing: those vectors (still a pad), the material sort
// fallback through CMaterial vtable +0xd8 and the Stats counters. This body registers nothing and returns false.
bool CSceneManager::registerNodeForRendering(ISceneNode* /*node*/, const boost::intrusive_ptr<video::CMaterial>& /*material*/,
                                             void* /*userData*/, E_SCENE_NODE_RENDER_PASS /*pass*/,
                                             const core::vector3d<float>* /*position*/, int /*sortOrder*/) {
    return false;
}

// 0x58b88c _ZN6glitch5scene13CSceneManager18registerSceneNodesEPNS0_10ISceneNodeE (skipped in the ledger: 356 bytes of render-list code)
// 0x589128 _ZN6glitch5scene13CSceneManager18registerSceneNodesERKSt6vectorIPNS0_10ISceneNodeENS_4core10SAllocatorIS4_LNS_6memory13E_MEMORY_HINTE0EEEE
// PARTIAL: registers nothing (needs the render lists and onRegisterSceneNode through the visibility and culling tests).
void CSceneManager::registerSceneNodes(ISceneNode* /*node*/) {}

// 0x589128: registers each node of the vector through the virtual single-node overload (vptr+0x28).
void CSceneManager::registerSceneNodes(const std::vector<ISceneNode*>& nodes) {
    for (ISceneNode* node : nodes) {
        registerSceneNodes(node);
    }
}

namespace {

// The walk of 0x589fa4: the children of a node are compiled when the node reports itself registered. The original walks
// the tree with parent links and explicit state; this recursion visits the same nodes in the same order.
void registerCompileChildren(ISceneNode* owner) {
    for (ISceneNode* child : owner->getChildren()) {
        if ((child->flags & ESNF_VISIBLE) != 0 && child->onRegisterSceneNode() != 0) {
            registerCompileChildren(child);
        }
    }
}

}  // namespace

// 0x589fa4 _ZN6glitch5scene13CSceneManager24registerSceneNodesCompileEPNS0_10ISceneNodeE
// A null node means the root. The node itself is registered when it is visible and onRegisterSceneNode returns non-zero;
// then its visible children are walked the same way.
void CSceneManager::registerSceneNodesCompile(ISceneNode* node) {
    ISceneNode* start = node != nullptr ? node : rootNode;
    if ((start->flags & ESNF_VISIBLE) != 0 && start->onRegisterSceneNode() != 0) {
        registerCompileChildren(start);
    }
}

// 0x589168 _ZN6glitch5scene13CSceneManager24registerSceneNodesCompileERKSt6vectorIPNS0_10ISceneNodeENS_4core10SAllocatorIS4_LNS_6memory13E_MEMORY_HINTE0EEEE
void CSceneManager::registerSceneNodesCompile(const std::vector<ISceneNode*>& nodes) {
    for (ISceneNode* node : nodes) {
        registerSceneNodesCompile(node);
    }
}

// 0x58b728 _ZN6glitch5scene13CSceneManager7drawAllERKSt6vectorIPNS0_10ISceneNodeENS_4core10SAllocatorIS4_LNS_6memory13E_MEMORY_HINTE0EEEE
// 0x58b7f4 _ZN6glitch5scene13CSceneManager7drawAllEPNS0_10ISceneNodeE
// PARTIAL. The original calls, in order: drawInit(driver), collectAllNodes, setupCamera, registerSceneNodes, drawShadowReceivers,
// renderLists(driver), then sets the render pass to 9 and resets the frame statistics. This body runs the calls it can (the
// virtual hooks) and sets the render pass; the driver and the statistics are not ported.
void CSceneManager::drawAll(const std::vector<ISceneNode*>& nodes) {
    drawInit(driver);
    collectAllNodes(nodes);
    setupCamera();
    registerSceneNodes(nodes);
    drawShadowReceivers();
    renderLists(driver);
    renderPass = 9;
}
void CSceneManager::drawAll(ISceneNode* node) {
    drawInit(driver);
    if (node == nullptr && hierarchyChanged) {
        collectAllNodes();
    }
    setupCamera();
    registerSceneNodes(node);
    drawShadowReceivers();
    renderLists(driver);
    renderPass = 9;
}

// 0x589e7c _ZN6glitch5scene13CSceneManager8drawInitEPNS_5video12IVideoDriverE
// PARTIAL: resets the per-frame render state on the driver (not ported); empty here.
void CSceneManager::drawInit(video::IVideoDriver* /*videoDriver*/) {}

// 0x590660 _ZN6glitch5scene13CSceneManager11renderListsEPNS_5video12IVideoDriverE (skipped: 2128 bytes)
void CSceneManager::renderLists(video::IVideoDriver* /*videoDriver*/) {}

// 0x58b0a8 _ZN6glitch5scene13CSceneManager19drawShadowReceiversEv (skipped: 472 bytes)
void CSceneManager::drawShadowReceivers() {}

// 0x58ff10, 0x58fee0, 0x58fccc and 0x58fc9c: the four compile overloads; 0x58fd10 and 0x58facc the internal ones.
// PARTIAL: building a CBatchSceneNode needs CBatchMesh construction and the segment callbacks; every overload returns null.
CBatchSceneNode* CSceneManager::compile(const std::vector<ISceneNode*>& /*nodes*/, CBatchSceneNode* /*batch*/, bool /*flag*/,
                                        ISegmentCompileCallback* /*compileCallback*/,
                                        ISplitSegmentChoiceCallback* /*choiceCallback*/,
                                        core::vector3d<float> /*segmentSize*/) {
    return nullptr;
}
CBatchSceneNode* CSceneManager::compile(const std::vector<ISceneNode*>& /*nodes*/, CBatchSceneNode* /*batch*/, bool /*flag*/,
                                        ISegmentCompileCallback* /*compileCallback*/,
                                        ISplitSegmentChoiceCallback* /*choiceCallback*/, std::uint32_t /*maxIndexCount*/) {
    return nullptr;
}
CBatchSceneNode* CSceneManager::compile(ISceneNode* /*node*/, CBatchSceneNode* /*batch*/, bool /*flag*/,
                                        ISegmentCompileCallback* /*compileCallback*/,
                                        ISplitSegmentChoiceCallback* /*choiceCallback*/,
                                        core::vector3d<float> /*segmentSize*/) {
    return nullptr;
}
CBatchSceneNode* CSceneManager::compile(ISceneNode* /*node*/, CBatchSceneNode* /*batch*/, bool /*flag*/,
                                        ISegmentCompileCallback* /*compileCallback*/,
                                        ISplitSegmentChoiceCallback* /*choiceCallback*/, std::uint32_t /*maxIndexCount*/) {
    return nullptr;
}
CBatchSceneNode* CSceneManager::compileInternal(const std::vector<ISceneNode*>& /*nodes*/, CBatchSceneNode* /*batch*/,
                                                bool /*flag*/, ISegmentCompileCallback* /*compileCallback*/,
                                                ISplitSegmentChoiceCallback* /*choiceCallback*/) {
    return nullptr;
}
CBatchSceneNode* CSceneManager::compileInternal(ISceneNode* /*node*/, CBatchSceneNode* /*batch*/, bool /*flag*/,
                                                ISegmentCompileCallback* /*compileCallback*/,
                                                ISplitSegmentChoiceCallback* /*choiceCallback*/) {
    return nullptr;
}

// 0x58b9f0 _ZN6glitch5scene13CSceneManager6updateEfb
// The reset sentinel (-123460) re-reads the clock from the timer; otherwise `elapsed` is added. The time is then truncated
// to milliseconds. The node lists are rebuilt when the hierarchy changed. With updateAnimators the animated nodes take
// onUpdateTime; without it the root node takes onAnimate.
void CSceneManager::update(float elapsed, bool updateAnimators) {
    if (elapsed == kUpdateResetTime) {
        time = static_cast<float>(os::Timer::getTime());
    } else {
        time += elapsed;
    }
    const auto timeMs = static_cast<std::uint32_t>(time);
    if (hierarchyChanged) {
        collectAllNodes();
    }
    if (updateAnimators) {
        for (std::size_t i = 0; i < animatedNodes.size(); ++i) {
            animatedNodes[i]->onUpdateTime(timeMs);
        }
    } else {
        rootNode->onAnimate(timeMs);
    }
}

// 0x589888 _ZN6glitch5scene13CSceneManager5clearEv
// Removes every child of the root node (vptr+0x64) and drops the shadow receiver targets.
void CSceneManager::clear() {
    if (rootNode != nullptr) {
        rootNode->removeAll();
    }
    for (IShadowReceiverTarget* target : shadowReceiverTargets) {
        if (target != nullptr) target->drop();
    }
    shadowReceiverTargets.clear();
}

// 0x58dd50 _ZN6glitch5scene13CSceneManager21createNewSceneManagerEb
// PARTIAL: the original constructs a derived manager of the same kind; the manager class is abstract here, so null.
CSceneManager* CSceneManager::createNewSceneManager(bool /*copyRoot*/) {
    return nullptr;
}

// 0x589364 _ZN6glitch5scene13CSceneManager8saveSceneEPKcPNS0_24ISceneUserDataSerializerE
// Creates the file through the file system (not appending), saves into it, and drops the file.
bool CSceneManager::saveScene(const char* fileName, ISceneUserDataSerializer* userData) {
    io::IWriteFile* file = fileSystem->createAndWriteFile(fileName, false);
    if (file == nullptr) {
        return false;
    }
    const bool result = saveScene(file, userData);
    file->drop();
    return result;
}

// 0x58d36c _ZN6glitch5scene13CSceneManager8saveSceneEPNS_2io10IWriteFileEPNS0_24ISceneUserDataSerializerE
// Creates the XML writer over the file, writes the header and the root node tree, and drops the writer.
bool CSceneManager::saveScene(io::IWriteFile* file, ISceneUserDataSerializer* userData) {
    if (file == nullptr) {
        return false;
    }
    io::IXMLWriter* writer = fileSystem->createXMLWriter(file);
    if (writer == nullptr) {
        return false;
    }
    writer->writeXMLHeader();
    writeSceneNode(writer, rootNode, userData);
    writer->drop();
    return true;
}

// 0x589b84 _ZN6glitch5scene13CSceneManager8loadSceneEPKcPNS0_24ISceneUserDataSerializerE
// Opens the file through the file system; on failure logs "Unable to open scene file" with the name and returns false.
bool CSceneManager::loadScene(const char* fileName, ISceneUserDataSerializer* userData) {
    io::IReadFile* file = fileSystem->createAndOpenFile(fileName);
    if (file == nullptr) {
        os::Printer::log("Unable to open scene file", fileName, ELL_ERROR);
        return false;
    }
    const bool result = loadScene(file, userData);
    file->drop();
    return result;
}

// 0x58e768 _ZN6glitch5scene13CSceneManager8loadSceneEPNS_2io9IReadFileEPNS0_24ISceneUserDataSerializerE
// PARTIAL: the scene reader (readSceneNode) is not reconstructed; reports failure.
bool CSceneManager::loadScene(io::IReadFile* /*file*/, ISceneUserDataSerializer* /*userData*/) {
    return false;
}

// 0x58bdb0 and 0x58e824: getMesh by file and by name. PARTIAL: the mesh cache and loader path is not reconstructed; null.
boost::intrusive_ptr<IMesh> CSceneManager::getMesh(io::IReadFile* /*file*/) {
    return boost::intrusive_ptr<IMesh>();
}
boost::intrusive_ptr<IMesh> CSceneManager::getMesh(const char* /*fileName*/) {
    return boost::intrusive_ptr<IMesh>();
}

// 0x58c4a4, 0x58c644, 0x58c8dc, 0x58e1e0 (the XML readers) and 0x58cdd4 (the XML writer). PARTIAL: empty / null.
void CSceneManager::readUserData(io::IXMLReader* /*reader*/, ISceneNode* /*node*/, ISceneUserDataSerializer* /*userData*/) {}
void CSceneManager::readAnimators(io::IXMLReader* /*reader*/, ISceneNode* /*node*/) {}
void CSceneManager::readMaterials(io::IXMLReader* /*reader*/, ISceneNode* /*node*/) {}
ISceneNode* CSceneManager::readSceneNode(io::IXMLReader* /*reader*/, ISceneNode* /*parent*/,
                                         ISceneUserDataSerializer* /*userData*/) {
    return nullptr;
}
void CSceneManager::writeSceneNode(io::IXMLWriter* /*writer*/, const ISceneNode* /*node*/, ISceneUserDataSerializer* /*userData*/) {}

// 0x589754 _ZN6glitch5scene13CSceneManager26removeShadowReceiverTargetEN5boost13intrusive_ptrINS_5video8ITextureEEE
// Removes the first registered target whose texture (IShadowReceiverTarget +0x0c) is the given texture, null included, and
// drops the list's use of it.
void CSceneManager::removeShadowReceiverTarget(boost::intrusive_ptr<video::ITexture> texture) {
    video::ITexture* raw = texture.get();
    for (auto it = shadowReceiverTargets.begin(); it != shadowReceiverTargets.end(); ++it) {
        if ((*it)->texture == raw) {
            IShadowReceiverTarget* removed = *it;
            shadowReceiverTargets.erase(it);
            removed->drop();
            return;
        }
    }
}

// 0x58ca64 _ZNK6glitch5scene13CSceneManager8isCulledERKNS_4core8aabbox3dIfEENS0_14E_CULLING_TYPEE (skipped, 280 bytes)
// PARTIAL: the box-only form of the test (no node). Returns false (not culled).
bool CSceneManager::isCulled(const core::aabbox3d<float>& /*box*/, E_CULLING_TYPE /*culling*/) const {
    return false;
}

// Nested node entries ------------------------------------------------------------------------------------------------

// 0x350c98 _ZN6glitch5scene13CSceneManager17SDefaultNodeEntryC1EPNS0_10ISceneNodeEN5boost13intrusive_ptrINS_5video9CMaterialEEEPvi
// The sort order 0x7fffffff means "ask the node": it is replaced by the node's rendering layer (vptr+0xd8).
CSceneManager::SDefaultNodeEntry::SDefaultNodeEntry(ISceneNode* n, boost::intrusive_ptr<video::CMaterial> m, void* data,
                                                    int order)
    : node(n), userData(data), material(m), sortOrder(order == 0x7fffffff ? n->getRenderingLayer() : order) {}

// 0x3539e8 _ZNK6glitch5scene13CSceneManager17SDefaultNodeEntryltERKS1_
// PARTIAL. Larger sort order first. With equal sort orders and both materials set, the original compares the material hash
// codes, then (equal) the nodes' vertex counts (getRenderVertexCount at vptr+0x20), else CMaterial::operator<; the CMaterial
// calls are not ported, so materials are compared by pointer and the vertex count is used on a tie.
bool CSceneManager::SDefaultNodeEntry::operator<(const SDefaultNodeEntry& other) const {
    if (sortOrder > other.sortOrder) return true;
    if (sortOrder != other.sortOrder) return false;
    const video::CMaterial* a = material.get();
    const video::CMaterial* b = other.material.get();
    if (a == nullptr && b == nullptr) return node < other.node;
    if (a == nullptr || b == nullptr || a != b) return a < b;
    return node->getRenderVertexCount(userData) < other.node->getRenderVertexCount(other.userData);
}

// 0x350cf4 _ZN6glitch5scene13CSceneManager18SDistanceNodeEntryC1EPNS0_10ISceneNodeERKNS_4core8vector3dIfEEPv
// distance = |node translation - position|^2 + (squared extent of the node's box) * -0.5 (the box is vptr+0x30).
CSceneManager::SDistanceNodeEntry::SDistanceNodeEntry(ISceneNode* n, const core::vector3d<float>& position, void* data)
    : node(n), userData(data) {
    const core::CMatrix4<float>& m = n->getAbsoluteTransformation();
    const float dx = m.M[12] - position.X;
    const float dy = m.M[13] - position.Y;
    const float dz = m.M[14] - position.Z;
    const float distanceSquared = (dx * dx + dy * dy) + dz * dz;
    const core::aabbox3d<float>& box = n->getBoundingBox();
    const float ex = box.MaxEdge.X - box.MinEdge.X;
    const float ey = box.MaxEdge.Y - box.MinEdge.Y;
    const float ez = box.MaxEdge.Z - box.MinEdge.Z;
    const float extentSquared = (ex * ex + ey * ey) + ez * ez;
    distance = static_cast<double>(distanceSquared) + static_cast<double>(extentSquared) * -0.5;
}

// 0x354e8c _ZN6glitch5scene13CSceneManager21STransparentNodeEntryC1EPNS0_10ISceneNodeERKNS_4core8vector3dIfEEN5boost13intrusive_ptrINS_5video9CMaterialEEEPvPS8_i
// sort order as in SDefaultNodeEntry; distance = |point - position|^2 + the node's camera offset (vptr+0xd0), where the point
// is `transformed` when given and the node's translation otherwise.
CSceneManager::STransparentNodeEntry::STransparentNodeEntry(ISceneNode* n, const core::vector3d<float>& position,
                                                            boost::intrusive_ptr<video::CMaterial> m, void* data,
                                                            const core::vector3d<float>* transformed, int order)
    : node(n), userData(data), material(m), sortOrder(order == 0x7fffffff ? n->getRenderingLayer() : order) {
    float px, py, pz;
    if (transformed != nullptr) {
        px = transformed->X;
        py = transformed->Y;
        pz = transformed->Z;
    } else {
        const core::CMatrix4<float>& matrix = n->getAbsoluteTransformation();
        px = matrix.M[12];
        py = matrix.M[13];
        pz = matrix.M[14];
    }
    const float dx = px - position.X;
    const float dy = py - position.Y;
    const float dz = pz - position.Z;
    distance = ((dx * dx) + (dy * dy)) + (dz * dz) + n->getCameraOffset();
}

// 0x58d710 _ZN6glitch5scene13CSceneManager21STransparentNodeEntryD1Ev
CSceneManager::STransparentNodeEntry::~STransparentNodeEntry() = default;

// 0x3538ac _ZNK6glitch5scene13CSceneManager21STransparentNodeEntryltERKS1_
// PARTIAL. Larger sort order first, then the larger distance first; the material tie-break (CMaterial::operator== and <) is
// replaced by a pointer comparison, then the nodes' vertex counts decide.
bool CSceneManager::STransparentNodeEntry::operator<(const STransparentNodeEntry& other) const {
    if (sortOrder > other.sortOrder) return true;
    if (sortOrder != other.sortOrder) return false;
    if (distance > other.distance) return true;
    if (distance != other.distance) return false;
    const video::CMaterial* a = material.get();
    const video::CMaterial* b = other.material.get();
    if (a == nullptr && b == nullptr) return node < other.node;
    if (a == nullptr || b == nullptr || a != b) return a < b;
    return node->getRenderVertexCount(userData) < other.node->getRenderVertexCount(other.userData);
}

}  // namespace glitch::scene
