// glitch::scene::CSceneManager: the scene manager (there is no ISceneManager class in the binary: no symbol, no vtable,
// and CSceneManager's own vtable carries every slot). Canonical header of the engine unification wave (u1-scene). It
// replaces scene_nodes.hpp (CSceneManager with a handful of members), layouts/scene.hpp (a flags-only struct),
// recon/slice_06.hpp (recon_06::SceneManager) and recon/slice_31.hpp (recon_31::CSceneManager, nested node entries).
// The game class SceneManager (game/include/dh2/SceneManager.hpp) derives from this one and fills the pure slot below.
//
// Layout (original 32-bit offsets; constructor 0x58d908 / 0x58ddd0 and the bodies that touch each word):
//   +0x00 vptr (glitch::io::IAttributeExchangingObject, the primary base)
//   +0x04 the root node (CSceneManagerRootNode, created by the constructor)
//   +0x08 child count, +0x0c / +0x10 child list sentinel: the root node is linked here through its own sibling link
//   +0x14 video driver (a reference is held), +0x18 the same pointer again, +0x1c CBatchDriver (new 0x264 bytes)
//   +0x20 file system (intrusive pointer), +0x24 GUI environment (held), +0x28 cursor control (held)
//   +0x2c CSceneCollisionManager (new 0x1c bytes)
//   +0x30 .. +0x8f render lists: unsorted entries (+0x30, +0x3c), distance entries (+0x48), render-data sort (+0x54, +0x64),
//         default entries (+0x78), transparent entries (+0x84); registerNodeForRendering fills them
//   +0x90 shadow receiver targets (vector of intrusive pointers)
//   +0x9c, +0xa0, +0xa4 and +0xa8, +0xac, +0xb0: the current and the next rendered node (word, node, second word)
//   +0xb4 external mesh loaders, +0xc0 deletion queue, +0xcc scene node factories, +0xd8 animator factories (vectors)
//   +0xe4 active camera (a reference is held), +0xe8 camera position (setupCamera)
//   +0xf4 shadow colours (4 words), +0x104 ambient light (4 floats), +0x114 CAttributes (embedded, 92 bytes)
//   +0x170 mesh cache, +0x174 render pass (starts at 9), +0x178 / +0x1c0 / +0x208 three wide strings "i", "n", "t"
//   +0x250 culling enabled byte, +0x254 time, +0x270 / +0x27c the node lists collectAllNodes fills (render, animated)
//   +0x288 hierarchy-changed byte, +0x289 visibility-changed byte, +0x28a byte
//   +0x28c glitch::IReferenceCounted virtual base (vptr, then the count at +0x290)
//
// Slots (vptr+K; tools/decomp_vtable.py prints K + 0x0c for this class because a virtual-base offset heads the vtable):
// the order of the virtual declarations below is the slot order.
//
// Port notes: pointers are host width (TRACK-B decision 2). The glitch std::vector (SAllocator) is std::vector.
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "glitch-core-io/boost/intrusive_ptr.hpp"
#include "glitch-core-io/core/reference_counted.hpp"
#include "glitch-core-io/io/interfaces.hpp"
#include "glitch-scene-collada-gui/deps/core_stand_ins.hpp"
#include "glitch-scene-collada-gui/scene/ISceneNode.hpp"
#include "glitch-scene-collada-gui/scene/scene_types.hpp"

namespace glitch::video {
class CBatchDriver;
}

namespace glitch::scene {

class CSceneCollisionManager;

class CSceneManager : public io::IAttributeExchangingObject, public virtual glitch::IReferenceCounted {
public:
    // Entries of the render lists (registerNodeForRendering). Sizes in the original: 0x10, 0x10 and 0x14 bytes.
    struct SDefaultNodeEntry {
        // 0x350c98
        SDefaultNodeEntry(ISceneNode* node, boost::intrusive_ptr<video::CMaterial> material, void* userData,
                          int sortOrder);
        // 0x3539e8: larger sort order first, then the material hash, then the vertex count of the node.
        bool operator<(const SDefaultNodeEntry& other) const;

        ISceneNode* node = nullptr;                               // +0x00
        void* userData = nullptr;                                 // +0x04
        boost::intrusive_ptr<video::CMaterial> material;          // +0x08
        int sortOrder = 0;                                        // +0x0c
    };
    struct SDistanceNodeEntry {
        // 0x350cf4
        SDistanceNodeEntry(ISceneNode* node, const core::vector3d<float>& position, void* userData);

        ISceneNode* node = nullptr;   // +0x00
        void* userData = nullptr;     // +0x04
        double distance = 0.0;        // +0x08 squared distance minus half the squared box extent (stored as a double)
    };
    struct STransparentNodeEntry {
        // 0x354e8c
        STransparentNodeEntry(ISceneNode* node, const core::vector3d<float>& position,
                              boost::intrusive_ptr<video::CMaterial> material, void* userData,
                              const core::vector3d<float>* transformed, int sortOrder);
        // 0x58d710
        ~STransparentNodeEntry();
        // 0x3538ac: larger sort order first, then the farthest first, then the material, then the vertex count.
        bool operator<(const STransparentNodeEntry& other) const;

        ISceneNode* node = nullptr;                               // +0x00
        void* userData = nullptr;                                 // +0x04
        boost::intrusive_ptr<video::CMaterial> material;          // +0x08
        int sortOrder = 0;                                        // +0x0c
        float distance = 0.0f;                                    // +0x10
    };

    // 0x58d908 (C1, complete object) and 0x58ddd0 (C2, base object): one C++ constructor. PARTIAL: the batch driver,
    // collision manager, attribute set, default factories and the Collada binary loader it creates are not ported.
    CSceneManager(video::IVideoDriver* driver, const boost::intrusive_ptr<io::IFileSystem>& fileSystem,
                  gui::ICursorControl* cursorControl, IMeshCache* meshCache, gui::IGUIEnvironment* guiEnvironment);

    // K 0x00 / 0x04: 0x588fe8 and 0x58bbc4. PARTIAL (the attribute writers are not ported).
    void serializeAttributes(io::IAttributes* attributes, io::SAttributeReadWriteOptions* options) const override;
    void deserializeAttributes(io::IAttributes* attributes, io::SAttributeReadWriteOptions* options) override;

    // K 0x08 / 0x0c: 0x58eb70 (D1), 0x58ef1c (D2), 0x58ef00 (D0, skipped).
    ~CSceneManager() override;

    // K 0x10 .. 0x20: searches of the node tree below `start` (the root when null).
    virtual ISceneNode* getSceneNodeFromId(int id, ISceneNode* start);                                  // 0x589208
    virtual ISceneNode* getSceneNodeFromUID(const char* uid, ISceneNode* start);                        // 0x589d48
    virtual ISceneNode* getSceneNodeFromName(const char* name, ISceneNode* start);                      // 0x589d58
    virtual ISceneNode* getSceneNodeFromType(E_SCENE_NODE_TYPE type, ISceneNode* start);               // 0x589298
    virtual void getSceneNodesFromType(E_SCENE_NODE_TYPE type, std::vector<ISceneNode*>& out,
                                       ISceneNode* start);                                              // 0x58c3cc
    // K 0x24, 0x58f348: files the node into the list for `pass`; false when it is not registered.
    virtual bool registerNodeForRendering(ISceneNode* node, const boost::intrusive_ptr<video::CMaterial>& material,
                                          void* userData, E_SCENE_NODE_RENDER_PASS pass,
                                          const core::vector3d<float>* position, int sortOrder);
    // K 0x28 .. 0x34: registration of a node tree and of a node vector, for rendering and for compiling.
    virtual void registerSceneNodes(ISceneNode* node);                                                  // 0x58b88c
    virtual void registerSceneNodes(const std::vector<ISceneNode*>& nodes);                             // 0x589128
    virtual void registerSceneNodesCompile(ISceneNode* node);                                           // 0x589fa4
    virtual void registerSceneNodesCompile(const std::vector<ISceneNode*>& nodes);                      // 0x589168
    // K 0x38 / 0x3c, 0x58b728 and 0x58b7f4: the frame (drawInit, collect, setupCamera, register, shadows, renderLists).
    virtual void drawAll(const std::vector<ISceneNode*>& nodes);
    virtual void drawAll(ISceneNode* node);
    // K 0x40 .. 0x4c.
    virtual void drawInit(video::IVideoDriver* driver);                                                 // 0x589e7c
    virtual void setupCamera();                                                                         // 0x589e18
    virtual void renderLists(video::IVideoDriver* driver);                                              // 0x590660
    virtual void drawShadowReceivers();                                                                 // 0x58b0a8
    // K 0x50 .. 0x5c: the four compile overloads (node vector or node tree; segment size as a vector3d or an unsigned).
    virtual CBatchSceneNode* compile(const std::vector<ISceneNode*>& nodes, CBatchSceneNode* batch, bool flag,
                                     ISegmentCompileCallback* compileCallback,
                                     ISplitSegmentChoiceCallback* choiceCallback,
                                     core::vector3d<float> segmentSize);                                // 0x58ff10
    virtual CBatchSceneNode* compile(const std::vector<ISceneNode*>& nodes, CBatchSceneNode* batch, bool flag,
                                     ISegmentCompileCallback* compileCallback,
                                     ISplitSegmentChoiceCallback* choiceCallback,
                                     std::uint32_t maxIndexCount);                                      // 0x58fee0
    virtual CBatchSceneNode* compile(ISceneNode* node, CBatchSceneNode* batch, bool flag,
                                     ISegmentCompileCallback* compileCallback,
                                     ISplitSegmentChoiceCallback* choiceCallback,
                                     core::vector3d<float> segmentSize);                                // 0x58fccc
    virtual CBatchSceneNode* compile(ISceneNode* node, CBatchSceneNode* batch, bool flag,
                                     ISegmentCompileCallback* compileCallback,
                                     ISplitSegmentChoiceCallback* choiceCallback,
                                     std::uint32_t maxIndexCount);                                      // 0x58fc9c
    // K 0x60 .. 0x84.
    virtual void update(float elapsed, bool updateAnimators);                                           // 0x58b9f0
    virtual bool postEventFromUser(const glitch::SEvent& event);                                        // 0x589328
    virtual void clear();                                                                               // 0x589888
    virtual ISceneNode* addSceneNode(const char* typeName, ISceneNode* parent);                         // 0x58942c
    virtual CSceneManager* createNewSceneManager(bool copyRoot);                                        // 0x58dd50
    virtual bool saveScene(const char* fileName, ISceneUserDataSerializer* userData);                   // 0x589364
    virtual bool saveScene(io::IWriteFile* file, ISceneUserDataSerializer* userData);                   // 0x58d36c
    virtual bool loadScene(const char* fileName, ISceneUserDataSerializer* userData);                   // 0x589b84
    virtual bool loadScene(io::IReadFile* file, ISceneUserDataSerializer* userData);                    // 0x58e768
    virtual IMeshWriter* createMeshWriter(EMESH_WRITER_TYPE type);                                      // 0x589558 (returns null)
    // K 0x88: pure in the engine class (a null vtable entry); the game's SceneManager implements it (listing +0x94). The
    // parameter type is not recoverable from CSceneManager itself; the game class passes its SkinnedMeshSceneNodeEx.
    virtual void registerSkinnedMesh(void* skinnedMeshNode) = 0;

    // Non-virtual members (direct calls in the original).
    void notifyVisibilityChanged();                                                  // 0x5890a8
    void notifyHierarchyChanged();                                                   // 0x5890b4 (also raises the visibility flag)
    void setActiveCamera(ICameraSceneNode* camera);                                  // 0x5890c0
    void setShadowColor(video::SColor color);                                        // 0x5891ac (all four slots)
    E_SCENE_NODE_RENDER_PASS setSceneNodeRenderPass(E_SCENE_NODE_RENDER_PASS pass);  // 0x589354 (returns the previous pass)
    const char* getSceneNodeTypeName(E_SCENE_NODE_TYPE type) const;                  // 0x5893c0
    const char* getAnimatorTypeName(E_SCENE_NODE_ANIMATOR_TYPE type) const;         // 0x5894a4
    void setAmbientLight(const video::SColorf& color);                               // 0x589508
    std::uint32_t getCurrentRenderedNode(void** node, std::int32_t* second) const;   // 0x589518
    std::uint32_t getNextRenderedNode(void** node, std::int32_t* second) const;      // 0x589538
    void removeShadowReceiverTarget(boost::intrusive_ptr<video::ITexture> texture);  // 0x589754
    void removeShadowReceiverTarget(boost::intrusive_ptr<IShadowReceiverTarget> target);  // 0x5897d4
    void addShadowReceiverTarget(boost::intrusive_ptr<IShadowReceiverTarget> target);     // 0x58a7f0
    bool isCulled(const ISceneNode* node) const;                                     // 0x58ab28
    bool isCulled(const core::aabbox3d<float>& box, E_CULLING_TYPE culling) const;   // 0x58ca64
    void registerSceneNodeFactory(ISceneNodeFactory* factory);                       // 0x58ae20
    void registerSceneNodeAnimatorFactory(ISceneNodeAnimatorFactory* factory);       // 0x58d758
    void addExternalMeshLoader(IMeshLoader* loader);                                 // 0x58d8ac
    void clearDeletionList();                                                        // 0x58b350
    void addToDeletionQueue(ISceneNode* node);                                       // 0x58b4e0
    void collectAllNodes();                                                          // 0x58b7ac
    void collectAllNodes(ISceneNode* node);                                          // 0x58b558
    void collectAllNodes(const std::vector<ISceneNode*>& nodes);                     // 0x58b6c8
    boost::intrusive_ptr<IMesh> getMesh(io::IReadFile* file);                        // 0x58bdb0
    boost::intrusive_ptr<IMesh> getMesh(const char* fileName);                       // 0x58e824
    void readUserData(io::IXMLReader* reader, ISceneNode* node, ISceneUserDataSerializer* userData);  // 0x58c4a4
    void readAnimators(io::IXMLReader* reader, ISceneNode* node);                    // 0x58c644
    void readMaterials(io::IXMLReader* reader, ISceneNode* node);                    // 0x58c8dc
    ISceneNode* readSceneNode(io::IXMLReader* reader, ISceneNode* parent, ISceneUserDataSerializer* userData);  // 0x58e1e0
    void writeSceneNode(io::IXMLWriter* writer, const ISceneNode* node, ISceneUserDataSerializer* userData);    // 0x58cdd4
    CBatchSceneNode* compileInternal(const std::vector<ISceneNode*>& nodes, CBatchSceneNode* batch, bool flag,
                                     ISegmentCompileCallback* compileCallback,
                                     ISplitSegmentChoiceCallback* choiceCallback);                       // 0x58fd10
    CBatchSceneNode* compileInternal(ISceneNode* node, CBatchSceneNode* batch, bool flag,
                                     ISegmentCompileCallback* compileCallback,
                                     ISplitSegmentChoiceCallback* choiceCallback);                       // 0x58facc

    // Members, in offset order. The base class occupies +0x00 (its vptr).
    ISceneNode* rootNode = nullptr;                          // +0x04
    SChildList rootChildren;                                 // +0x08 count, +0x0c sentinel
    video::IVideoDriver* driver = nullptr;                   // +0x14 (a reference is held)
    video::IVideoDriver* driverCopy = nullptr;               // +0x18
    video::CBatchDriver* batchDriver = nullptr;              // +0x1c
    boost::intrusive_ptr<io::IFileSystem> fileSystem;        // +0x20
    gui::IGUIEnvironment* guiEnvironment = nullptr;          // +0x24 (held)
    gui::ICursorControl* cursorControl = nullptr;            // +0x28 (held)
    CSceneCollisionManager* collisionManager = nullptr;      // +0x2c
    std::uint8_t unk_30[0x60] = {};                          // +0x30 .. +0x8f render lists (see the layout notes above)
    std::vector<IShadowReceiverTarget*> shadowReceiverTargets;  // +0x90 begin, +0x94 end, +0x98 capacity (references held)
    std::uint32_t currentRenderedWord = 0;                   // +0x9c (returned by getCurrentRenderedNode)
    void* currentRenderedNode = nullptr;                     // +0xa0
    std::int32_t currentRenderedSecond = 0;                  // +0xa4
    std::uint32_t nextRenderedWord = 0;                      // +0xa8 (returned by getNextRenderedNode)
    void* nextRenderedNode = nullptr;                        // +0xac
    std::int32_t nextRenderedSecond = 0;                     // +0xb0
    std::vector<IMeshLoader*> externalMeshLoaders;           // +0xb4
    std::vector<ISceneNode*> deletionQueue;                  // +0xc0
    std::vector<ISceneNodeFactory*> sceneNodeFactories;      // +0xcc
    std::vector<ISceneNodeAnimatorFactory*> sceneNodeAnimatorFactories;  // +0xd8
    ICameraSceneNode* activeCamera = nullptr;                // +0xe4 (a reference is held)
    core::vector3d<float> cameraPosition;                    // +0xe8 (setupCamera)
    video::SColor shadowColors[4];                           // +0xf4 .. +0x103
    video::SColorf ambientLight;                             // +0x104 .. +0x113
    std::uint8_t attributes[0x5c] = {};                      // +0x114 embedded glitch::io::CAttributes (92 bytes)
    IMeshCache* meshCache = nullptr;                         // +0x170
    E_SCENE_NODE_RENDER_PASS renderPass = 9;                 // +0x174 (the constructor value)
    std::uint8_t unk_178[0xd8] = {};                         // +0x178 three wide strings (72 bytes each): "i", "n", "t"
    bool cullingEnabled = true;                              // +0x250 (isCulled returns false when clear)
    float time = 0.0f;                                       // +0x254
    std::uint8_t unk_258[0x18] = {};                         // +0x258 .. +0x26f
    std::vector<ISceneNode*> renderNodes;                    // +0x270 begin, +0x274 end, +0x278 capacity (collectAllNodes)
    std::vector<ISceneNode*> animatedNodes;                  // +0x27c begin, +0x280 end, +0x284 capacity (nodes with animators)
    bool hierarchyChanged = true;                            // +0x288
    bool visibilityChanged = true;                           // +0x289
    std::uint8_t unk_28a = 0;                                // +0x28a
};

}  // namespace glitch::scene
