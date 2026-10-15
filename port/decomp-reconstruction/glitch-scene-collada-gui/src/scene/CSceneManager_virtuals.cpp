// The CSceneManager virtuals that are not part of the frame path (src/scene/CSceneManager_render.cpp), the scene file I/O
// (src/scene/CSceneManager_scene_io.cpp) or the plain accessors (src/scene/CSceneManager.cpp): the constructor, the destructor, the
// attribute exchange, update, clear, createNewSceneManager and the compile entry points. This file defines the key function
// (serializeAttributes), so it carries the vtable; see src/scene/CSceneManager.cpp for the accessors and the faithful bodies.
#include "glitch-scene-collada-gui/scene/CSceneManager.hpp"

#include <cstddef>
#include <exception>

#include "glitch-core-io/io/iattributes.hpp"
#include "glitch-core-io/os/device.hpp"
#include "glitch-core-io/os/logging.hpp"
#include "glitch-scene-collada-gui/gui/ICursorControl.hpp"
#include "glitch-scene-collada-gui/gui/IGUIEnvironment.hpp"
#include "glitch-scene-collada-gui/scene/CColladaBinaryFileLoader.hpp"
#include "glitch-scene-collada-gui/scene/CDefaultSceneNodeAnimatorFactory.hpp"
#include "glitch-scene-collada-gui/scene/CDefaultSceneNodeFactory.hpp"
#include "glitch-scene-collada-gui/scene/CEmptySceneNode.hpp"
#include "glitch-scene-collada-gui/scene/CMeshCache.hpp"
#include "glitch-scene-collada-gui/scene/CSceneCollisionManager.hpp"
#include "glitch-scene-collada-gui/scene/ICameraSceneNode.hpp"
#include "glitch-scene-collada-gui/scene/IMesh.hpp"
#include "glitch-scene-collada-gui/scene/ISceneNode.hpp"
#include "glitch-video/batch_driver.hpp"
#include "glitch-video/video_driver.hpp"

namespace glitch::scene {

namespace {

// Sentinel passed to update() to reset the clock from the timer.
constexpr float kUpdateResetTime = -123460.0f;

// Adds / drops a use of a reference-counted object (the original: ++*(object + 4); IReferenceCounted::drop).
template <typename T>
void grab(T* object) {
    if (object != nullptr) ++object->referenceCounter;
}
template <typename T>
void release(T* object) {
    if (object != nullptr) object->drop();
}

// The batch driver (glitch::video::CBatchDriver, 0x264 bytes) now exists (glitch-video/include/glitch-video/
// batch_driver.hpp, included above), so the sizeof SFINAE below selects the allocating overload and the `long` fallback
// is dead code kept only so this TU still compiles if the include is ever dropped. The second helper drops the driver
// through IReferenceCounted::drop, as the original's destructor does (0x58eb70).
template <typename T, std::size_t = sizeof(T)>
T* makeBatchDriver(video::IVideoDriver* driver, int) {
    const auto* device = driver->m_device;
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    std::memcpy(&width, device->creationParameters.bytes + 0x40, sizeof(width));
    std::memcpy(&height, device->creationParameters.bytes + 0x44, sizeof(height));
    return new T(width, height, driver);
}
template <typename T>
T* makeBatchDriver(video::IVideoDriver*, long) {
    return nullptr;
}
template <typename T, std::size_t = sizeof(T)>
void dropBatchDriver(T* batchDriver, int) {
    if (batchDriver != nullptr) batchDriver->drop();
}
template <typename T>
void dropBatchDriver(T*, long) {}

}  // namespace

// 0x58d908 _ZN6glitch5scene13CSceneManagerC1EPNS_5video12IVideoDriverERKN5boost13intrusive_ptrINS_2io11IFileSystemEEEPNS_3gui14ICursorControlEPNS0_10IMeshCacheEPNS7_15IGUIEnvironmentE (complete object)
// 0x58ddd0 _ZN6glitch5scene13CSceneManagerC2EPNS_5video12IVideoDriverERKN5boost13intrusive_ptrINS_2io11IFileSystemEEEPNS_3gui14ICursorControlEPNS0_10IMeshCacheEPNS7_15IGUIEnvironmentE (base object)
// Reconstructed (the two symbols are one C++ constructor here, as in the rest of the port), with one host deviation.
// The order of the original: the driver (also kept at +0x18) and the batch driver built from the device's screen size
// (operator new(0x264) then CBatchDriver(width, height, driver) at 0x58d994-0x58d9bc, stored at this +0x1c), the file
// system, the stencil clear value 127 set on the driver, the root node (CSceneManagerRootNode, linked into the +0x08
// child list), then a use of the driver, the cursor control and the GUI environment, a use of the mesh cache or a new
// CMeshCache over the driver, the CSceneCollisionManager, the CColladaBinaryFileLoader (the first external mesh loader,
// its creation use is the list's), the CDefaultSceneNodeFactory and CDefaultSceneNodeAnimatorFactory (registered, then
// the creator's use is dropped) and the shadow colour 0x96000000.
// Deviation, named: the original dereferences the video driver unconditionally, and the port still cannot - 0x58d994
// reads the device out of the driver (`LDR R3,[R6,#0xD4]`) and makeBatchDriver dereferences the device in turn, while
// the scene tests build a CSceneManager with a null driver (seven test subclasses do). The batch driver is therefore
// created only when the driver is present; CNullDriver's constructor would fault on a null driver anyway. With a real
// driver the allocation now happens: glitch-video landed glitch::video::CBatchDriver (batch_driver.hpp, 0x264 bytes)
// for this batch, so the sizeof SFINAE in makeBatchDriver above selects the allocating overload.
CSceneManager::CSceneManager(video::IVideoDriver* videoDriver, const boost::intrusive_ptr<io::IFileSystem>& fileSystemArg,
                             gui::ICursorControl* cursorControlArg, IMeshCache* meshCacheArg,
                             gui::IGUIEnvironment* guiEnvironmentArg)
    : fileSystem(fileSystemArg) {
    driver = videoDriver;
    driverCopy = videoDriver;
    if (videoDriver != nullptr) {
        batchDriver = makeBatchDriver<video::CBatchDriver>(videoDriver, 0);
    }
    cursorControl = cursorControlArg;
    meshCache = meshCacheArg;
    guiEnvironment = guiEnvironmentArg;
    if (videoDriver != nullptr) {
        videoDriver->setClearStencil(127);
    }
    auto* root = new CSceneManagerRootNode(this);
    rootNode = root;
    SListLink* rootLink = static_cast<SListLink*>(static_cast<ISceneNode*>(root));
    SListLink* tail = rootChildren.sentinel.prev;
    rootLink->prev = tail;
    tail->next = rootLink;
    rootChildren.sentinel.prev = rootLink;
    rootLink->next = &rootChildren.sentinel;
    ++rootChildren.count;
    grab(videoDriver);
    grab(cursorControlArg);
    grab(guiEnvironmentArg);
    if (meshCacheArg != nullptr) {
        grab(meshCacheArg);
    } else {
        meshCache = new CMeshCache(videoDriver);
    }
    collisionManager = new CSceneCollisionManager(this, driver);
    externalMeshLoaders.push_back(new CColladaBinaryFileLoader(this, fileSystem));
    auto* nodeFactory = new CDefaultSceneNodeFactory(this, cursorControl, fileSystem);
    registerSceneNodeFactory(nodeFactory);
    nodeFactory->drop();
    auto* animatorFactory = new CDefaultSceneNodeAnimatorFactory(this, cursorControl);
    registerSceneNodeAnimatorFactory(animatorFactory);
    animatorFactory->drop();
    setShadowColor(video::SColor{0x96000000u});
}

// 0x58eb70 _ZN6glitch5scene13CSceneManagerD1Ev
// 0x58ef1c _ZN6glitch5scene13CSceneManagerD2Ev
// 0x58ef00 _ZN6glitch5scene13CSceneManagerD0Ev (skipped)
// PARTIAL. The order of the original: the deletion queue, the uses of the cursor control, the collision manager and the GUI
// environment, the external mesh loaders, the active camera, the mesh cache, the node and animator factories, the root node
// (after unlinking the root child list), the render node list, the batch driver and the driver; the vectors, the strings, the
// attribute set and the file system follow as members. Not done: the two static particle buffers the original releases here
// (CParticleSystemSceneNode::deleteSharedBuffer and CGlitchNewParticleSystemSceneNode::deleteSharedBuffer), which wait for the
// particle node classes.
CSceneManager::~CSceneManager() {
    clearDeletionList();
    release(cursorControl);
    release(collisionManager);
    release(guiEnvironment);
    for (IMeshLoader* loader : externalMeshLoaders) {
        release(loader);
    }
    if (activeCamera != nullptr) {
        ICameraSceneNode* camera = activeCamera;
        activeCamera = nullptr;
        camera->drop();
    }
    release(meshCache);
    for (ISceneNodeFactory* factory : sceneNodeFactories) {
        release(factory);
    }
    for (ISceneNodeAnimatorFactory* factory : sceneNodeAnimatorFactories) {
        release(factory);
    }
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
    release(rootNode);
    renderNodes.clear();
    dropBatchDriver(batchDriver, 0);
    release(driverCopy);
}

// 0x588fe8 _ZNK6glitch5scene13CSceneManager19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Writes the root node's name ("Name"), its id ("Id") and the ambient light ("AmbientLight", four floats).
void CSceneManager::serializeAttributes(io::IAttributes* attributesOut, io::SAttributeReadWriteOptions* /*options*/) const {
    attributesOut->addString("Name", rootNode->getName(), false);
    attributesOut->addInt("Id", rootNode->getID(), false);
    attributesOut->addColorf("AmbientLight", ambientLight, false);
}

// 0x58bbc4 _ZN6glitch5scene13CSceneManager21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
// Reads "Name", "Id" and "AmbientLight" into the root node and the ambient light, then resets the root's transform (zero
// position, identity rotation, unit scale), makes it visible, clears culling and the debug flag, and updates its matrix.
void CSceneManager::deserializeAttributes(io::IAttributes* attributesIn, io::SAttributeReadWriteOptions* /*options*/) {
    rootNode->setName(attributesIn->getString("Name"));
    rootNode->setID(attributesIn->getInt("Id"));
    ambientLight = attributesIn->getColorf("AmbientLight");
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

// 0x58ff10, 0x58fee0, 0x58fccc and 0x58fc9c: the four compile overloads; 0x58fd10 and 0x58facc the internal ones.
// PARTIAL. The original stores the segment size (a vector3d, switching the batch driver to size mode) or the maximal index count
// in the batch driver (CBatchDriver +0x250 .. +0x260, offsets 592 .. 608) and calls compileInternal, which creates the
// CBatchSceneNode (operator new(0x168), id -1) when none is given, installs the compile callbacks in the batch driver, makes the
// driver the manager's (drawInit with it), collects and registers the nodes with the batch flag raised (transparent passes use the
// default list), renders the lists into the batch driver, flushes it (vptr+0x1fc), finalizes the end-of-batch callback, calls the
// batch node's vptr+0xfc with the segment callback and the original driver, and restores the driver. CBatchDriver is not
// reconstructed in glitch-video, so none of that can run; every overload returns null.
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
    shadowReceiverTargets.clear();
}

// 0x58dd50 _ZN6glitch5scene13CSceneManager21createNewSceneManagerEb
// A new manager over the same driver, file system, cursor control, mesh cache and GUI environment (operator new(0x294): a plain
// CSceneManager); with `copyRoot` its own root node is dropped and replaced by a clone of this manager's root (vptr+0xc0). The
// original leaves the dropped root's link in the child list at +0x08 (a dangling entry the destructor then walks); the port unlinks
// it first, and the clone is not linked, as in the original.
CSceneManager* CSceneManager::createNewSceneManager(bool copyRoot) {
    auto* created = new CSceneManager(driver, fileSystem, cursorControl, meshCache, guiEnvironment);
    if (copyRoot) {
        SListLink* link = static_cast<SListLink*>(created->rootNode);
        link->prev->next = link->next;
        link->next->prev = link->prev;
        link->next = nullptr;
        link->prev = nullptr;
        --created->rootChildren.count;
        created->rootNode->drop();
        created->rootNode = rootNode->clone();
    }
    return created;
}

// K 0x88: see the declaration. Ends the program like a call through the imported __cxa_pure_virtual does.
void CSceneManager::registerSkinnedMesh(void* /*skinnedMeshNode*/) {
    std::terminate();
}

}  // namespace glitch::scene
