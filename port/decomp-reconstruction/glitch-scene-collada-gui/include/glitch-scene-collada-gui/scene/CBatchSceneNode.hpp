// glitch::scene::CBatchSceneNode: the scene node that renders a CBatchMesh (static level geometry in a few draw calls).
// Canonical header of the engine unification wave (u1-scene). It replaces the CBatchSceneNode in scene_nodes.hpp (four
// accessors, no base), layouts/scene.hpp (an empty struct), layouts/scene_evidence.hpp (evidence::CBatchSceneNode) and the
// recon_04 / recon_05 / recon_31 / recon_v2_02::CBatchSceneNodeView stand-ins.
//
// Evidence (libDungeonHunter2.so, ARM, 32-bit layout; constructors 0x57f9c0 / 0x57fc04 (id), 0x57f0d4 / 0x57faf8 (id, mesh),
// 0x57f874 / 0x57fd74 (id, mesh&, flag), 0x57fe84 / 0x57ffd4 (id, mesh, manager); tools/decomp_vtable.py
// glitch::scene::CBatchSceneNode, 328 bytes, virtual base IReferenceCounted at +0x160):
//   CBatchSceneNode derives from IMeshSceneNode (the vtable carries IMeshSceneNode::setNodeHierarchyAsShadowCaster at +0xe4 and
//   the pure setMesh / getMesh slots at +0xf4 / +0xf8, which it implements) and adds +0xfc setup, +0x100 postCompile,
//   +0x104 preRegisterSceneNode, +0x108 isSegmentVisible and +0x10c updateIndices.
//   Members (the base is 0x130 bytes):
//     +0x130 the CBatchMesh (a reference is held; the one-argument constructor creates a CBatchMesh)
//     +0x134 .. +0x13f three words (the constructors write 0 / 2, 2 / 2 and 0; roles not recovered)
//     +0x140 a word (0), +0x144 a word (-1), +0x148 a byte (1)
//     +0x14c a reference counted object (the destructor drops it)
//     +0x150 byte: the visible-index cache is on (1 by default; setVisibleIndexCache 0x57ed0c toggles it)
//     +0x154 the visible-index cache (delete[] when the cache is turned off)
//     +0x158 the visible entries, 20-byte records (count, match, tick, flags; updateInfo 0x57e650; delete[] in the destructor)
//     +0x15c a word (-1), +0x160 the virtual base
//   The constructors end with setAutomaticCulling(2).
#pragma once

#include <cstdint>

#include "glitch-scene-collada-gui/scene/CBatchMesh.hpp"
#include "glitch-scene-collada-gui/scene/CMeshSceneNode.hpp"

namespace glitch::scene {

class CBatchSceneNode : public IMeshSceneNode {
public:
    // 0x57f9c0 (C2) / 0x57fc04 (C1): a node with a new empty CBatchMesh.
    explicit CBatchSceneNode(int id);
    // 0x57f0d4 (C2) / 0x57faf8 (C1): a node over `mesh`.
    CBatchSceneNode(int id, const boost::intrusive_ptr<CBatchMesh>& mesh);
    // 0x57f874 (C2) / 0x57fd74 (C1).
    CBatchSceneNode(int id, boost::intrusive_ptr<CBatchMesh>& mesh, bool flag);
    // 0x57fe84 (C2) / 0x57ffd4 (C1): a node over `mesh` that registers with `manager`.
    CBatchSceneNode(int id, const boost::intrusive_ptr<CBatchMesh>& mesh, CSceneManager* manager);
    // 0x57f344, 0x57f3ec (D1), 0x57f49c (D0): skipped / partial.
    ~CBatchSceneNode() override;

    // vptr+0x00 / +0x04.
    void serializeAttributes(io::IAttributes* attributes, io::SAttributeReadWriteOptions* options) const override;  // 0x57e6f8
    void deserializeAttributes(io::IAttributes* attributes, io::SAttributeReadWriteOptions* options) override;      // 0x57ed98
    int onRegisterSceneNode() override;                                         // vptr+0x10, 0x57f4b8
    void render(void* renderContext) override;                                  // vptr+0x1c, 0x5808ac
    const core::aabbox3d<float>& getBoundingBox() const override;               // vptr+0x30, 0x57e538
    const core::aabbox3d<float>& getTransformedBoundingBox() const override;    // vptr+0x34, 0x57e524
    boost::intrusive_ptr<video::CMaterial> getMaterial(std::uint32_t index) const override;  // vptr+0x84, 0x57e570
    std::uint32_t getMaterialCount() const override;                            // vptr+0x88, 0x57e554
    E_SCENE_NODE_TYPE getType() const override;                                 // vptr+0xbc, 0x50cd2c ('basn')
    void setMesh(const boost::intrusive_ptr<IMesh>& mesh) override;             // vptr+0xf4, 0x57e5b0 (empty)
    boost::intrusive_ptr<IMesh> getMesh() const override;                       // vptr+0xf8, 0x57e594

    // vptr+0xfc .. +0x10c.
    virtual void setup(bool flag, video::IVideoDriver* driver);                 // 0x57f074
    virtual void postCompile();                                                 // 0x57ee34
    virtual void preRegisterSceneNode();                                        // 0x50cd38 (empty)
    virtual bool isSegmentVisible(const CBatchMesh::SSegment& segment);         // 0x50cd3c
    virtual void updateIndices(std::uint32_t batch, const boost::intrusive_ptr<video::IBuffer>& buffer,
                               const boost::intrusive_ptr<const video::IBuffer>& source);  // 0x57f704

    // Non-virtual members (direct calls; bodies are the recon_05 stand-ins or not reconstructed).
    std::int32_t computeMaxIndices(std::uint32_t batch);                        // 0x57e5b4 (int in the binary)
    std::int32_t clearVisibleSegments();                                        // 0x57e604 (int in the binary)
    std::int32_t updateInfo(std::uint32_t batch, std::uint32_t segment);        // 0x57e650 (int in the binary)
    void registerSolidBatches();                                                // 0x57e800
    void registerTransparentBatches();                                          // 0x57ea70
    void invalidateVisibleIndexCache(std::uint32_t batch);                      // 0x57ebec
    void setVisibleIndexCache(bool enabled);                                    // 0x57ed0c
    void renderBatchBBoxes(video::IVideoDriver* driver, std::uint32_t batch);   // 0x57f21c
    void renderSolidBatch(video::IVideoDriver* driver, std::uint32_t batch);    // 0x5800e8
    void flushTransparentBatch(video::IVideoDriver* driver);                    // 0x5804f0
    void renderTransparentSegment(video::IVideoDriver* driver, CBatchMesh::SSegment& segment);  // 0x5807a0

    boost::intrusive_ptr<CBatchMesh> mesh;                        // +0x130
    std::uint32_t unk_134 = 0;                                    // +0x134
    std::uint32_t unk_138 = 2;                                    // +0x138
    std::uint32_t unk_13c = 0;                                    // +0x13c
    std::uint32_t unk_140 = 0;                                    // +0x140
    std::int32_t unk_144 = -1;                                    // +0x144
    bool flag148 = true;                                          // +0x148
    boost::intrusive_ptr<glitch::IReferenceCounted> cacheRef;     // +0x14c
    bool visibleIndexCacheOn = true;                              // +0x150
    std::uint32_t* visibleIndexCache = nullptr;                   // +0x154
    std::uint32_t* visibleEntries = nullptr;                      // +0x158 20-byte records in the original
    std::int32_t unk_15c = -1;                                    // +0x15c
};

}  // namespace glitch::scene
