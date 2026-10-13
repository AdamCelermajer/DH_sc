// glitch::scene::CTerrainSceneNode: the terrain node. Canonical header (scene3 wave). It replaces the stand-in class that
// scene_simple_accessors.hpp carried, and derives from the canonical scene::ISceneNode.
//
// Layout (original 32-bit offsets). The terrain members start at +0x130, the end of the ISceneNode part. The
// glitch::IReferenceCounted virtual base sits after the terrain members and is not modelled here.
// Members the stand-in named keep those names; every other swept offset is unk_<hex offset>. Types come from the field
// sweep (tools/trackb/field_sweep.py): _DWORD -> uint32_t, _BYTE -> uint8_t, float -> float. Offsets are comments; the
// std::vector and vector3d members are typed, not byte-exact, as in the other stand-in layouts.
// Non-virtual return types the ledger does not recover are declared with the best available type; those functions have
// no body yet.
#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "glitch-scene-collada-gui/deps/intrusive_ptr.hpp"
#include "glitch-scene-collada-gui/scene/ISceneNode.hpp"

namespace glitch::io {
class IFileSystem;
class IReadFile;
}

namespace glitch::video {
struct SColor;
class CMaterial;
}

namespace glitch::scene {

class CMeshBuffer;

// One terrain patch record, stride 0x38. The LOD word is the first field and the bounding box follows it; the rest of the
// record is not mapped.
struct STerrainPatch {
    std::int32_t lod = 0;                    // +0x00
    core::aabbox3d<float> boundingBox;       // +0x04
};

class CTerrainSceneNode : public ISceneNode {
public:
    // 0x6d18bc (C2) and 0x6d1b5c (C1): full constructor. PARTIAL: the ISceneNode base construction and the STerrainData
    // build are not reconstructed, so there is no body.
    CTerrainSceneNode(const boost::intrusive_ptr<io::IFileSystem>& fileSystem, std::int32_t, std::uint32_t, std::int32_t,
                      std::int32_t patchSize, const core::vector3d<float>& position, const core::quaternion& rotation,
                      const core::vector3d<float>& scale);
    // Default node (identity transform, unit scale). Not an original constructor; it exists so the stand-in users still
    // build an empty terrain node.
    explicit CTerrainSceneNode(int id = -1);
    // 0x6d127c (D2) and 0x6d132c (D1): PARTIAL, empty. 0x6d13e8 (D0) is skipped.
    ~CTerrainSceneNode() override;

    // vptr+0x1c, 0x6d176c: PARTIAL, the draw path is not reconstructed.
    void render(void* renderContext) override;
    // vptr+0x30, 0x6d01e8: the box at +0x184.
    const core::aabbox3d<float>& getBoundingBox() const override;
    // vptr+0xbc, 0x6d00cc: 'terr'.
    E_SCENE_NODE_TYPE getType() const override;
    // vptr+0xc0, 0x6d5518: PARTIAL, returns nullptr (the node copy is not reconstructed).
    ISceneNode* clone() override;
    // 0x6d0074: PARTIAL, forwards to the material holder at +0x1ac (not declared), returns an empty pointer.
    boost::intrusive_ptr<video::CMaterial> getMaterial(std::uint32_t index) const override;
    // 0x6d0098: PARTIAL, forwards to the material holder at +0x1ac, returns 0.
    std::uint32_t getMaterialCount() const override;
    // 0x6d00b4: the scale at +0x15c.
    const core::vector3d<float>& getScale() const override;
    // 0x6d2f28: stores the scale at +0x15c and sets the dirty byte +0x1bf. PARTIAL: applyTransformation is not called.
    void setScale(const core::vector3d<float>& scale) override;
    // 0x6d00bc: the rotation quaternion at +0x140.
    const core::quaternion& getRotation() const override;
    // 0x6d2ef0: stores the quaternion at +0x140 and sets the dirty byte +0x1bf. PARTIAL: applyTransformation is not called.
    void setRotation(const core::quaternion& rotation) override;
    // 0x6d00c4: the position at +0x134.
    const core::vector3d<float>& getPosition() const override;
    // 0x6d2ec0: stores the position at +0x134 and sets the dirty byte +0x1bf. PARTIAL: applyTransformation is not called.
    void setPosition(const core::vector3d<float>& position) override;
    // 0x6d083c: PARTIAL, the ISceneNode base serialization is not reconstructed.
    void serializeAttributes(io::IAttributes* attributes, io::SAttributeReadWriteOptions* options) const override;
    // 0x6d5268: PARTIAL, the ISceneNode base read and the terrain attributes are not reconstructed.
    void deserializeAttributes(io::IAttributes* attributes, io::SAttributeReadWriteOptions* options) override;
    // vptr+0x10, 0x6d3544: PARTIAL, returns the ISceneNode base registration result.
    int onRegisterSceneNode() override;

    // 0x6d01c4: setRotationPivot.
    void setRotationPivot(const core::vector3d<float>& pivot);
    // 0x6d0200: the bounding box of the patch at (row, column).
    const core::aabbox3d<float>& getBoundingBox(std::int32_t row, std::int32_t column) const;
    // 0x6d021c: sets the LOD word of the patch at (row, column).
    void setLODOfPatch(std::int32_t row, std::int32_t column, std::int32_t lod);
    // 0x6d03f0: sets every patch to one LOD.
    void setCurrentLODOfPatches(std::int32_t lod);
    // 0x6d042c: sets each patch LOD from the vector.
    void setCurrentLODOfPatches(const std::vector<std::int32_t>& lods);
    // 0x6d21ac: fills the vector with each patch LOD.
    void getCurrentLODOfPatches(std::vector<std::int32_t>& lods) const;
    // 0x6d0238: returns 1 when the index is inside the LOD table (the squared distance is stored), else 0.
    std::int32_t overrideLODDistance(std::int32_t index, double distance);

    // 0x6d00f4 (C1): STerrainData constructor. Not reconstructed.
    struct STerrainData {
        STerrainData(std::int32_t, std::int32_t, const core::vector3d<float>& position, const core::quaternion& rotation,
                     const core::vector3d<float>& scale);
    };

    // 0x6d00d8: the render buffer (counted pointer at +0x1b0). The element type is not declared.
    void* getRenderBuffer() const;
    // 0x6d01f0: PARTIAL, body not reconstructed.
    bool loadHeightMapRAW(io::IReadFile* file, std::int32_t, video::SColor color, std::int32_t);
    // 0x6d1e34: height at (x, z).
    float getHeight(float x, float z) const;
    // 0x6d028c: index into the patch grid.
    std::uint32_t getIndex(std::int32_t, std::int32_t, std::int32_t, std::uint32_t, std::uint32_t) const;
    // 0x6d22f0: fills the index buffer for one patch.
    void getIndicesForPatch(std::vector<std::uint32_t>& indices, std::int32_t, std::int32_t, std::int32_t);
    // 0x6d0cfc: builds the patch table.
    void createPatches();
    // 0x6d2534: computes the patch data from the vertex grid.
    void calculatePatchData();
    // 0x6d2b08: maps the render vertex buffer through the transform.
    void applyTransformation();
    // 0x6d0470 / 0x6d0660: index generation over the patch tables.
    template <typename T>
    void preRenderIndicesCalculationsDirect(T* indices);
    // 0x6d3440: calls the templated index passes.
    void preRenderIndicesCalculations();
    // 0x6d0dbc: LOD selection over the patch table.
    void preRenderLODCalculations();
    // 0x6d1650: LOD distance thresholds.
    void calculateDistanceThresholds(bool enabled);
    // 0x6d4454: heightmap load.
    void loadHeightMap(io::IReadFile* file, video::SColor color, std::int32_t);
    // 0x6d2ff0: vertex smoothing over the mesh buffer.
    void smoothTerrain(const boost::intrusive_ptr<CMeshBuffer>& buffer, std::int32_t);
    // 0x6d315c: texture coordinate pass.
    void scaleTexture(float, float);
    // 0x6d3578: normal accumulation over the mesh buffer.
    void calculateNormals(const boost::intrusive_ptr<CMeshBuffer>& buffer);

    // +0x130: first terrain word (accessed by the constructor and the patch passes).
    std::uint32_t unk_130 = 0;                         // +0x130
    core::vector3d<float> position;                    // +0x134 (spans +0x138, +0x13c)
    core::quaternion rotation;                         // +0x140 (spans +0x144, +0x148, +0x14c)
    core::vector3d<float> rotationPivot;               // +0x150 (spans +0x154, +0x158)
    core::vector3d<float> scale;                       // +0x15c (spans +0x160, +0x164)
    float unk_168 = 0.0f;                              // +0x168
    float unk_16c = 0.0f;                              // +0x16c
    float unk_170 = 0.0f;                              // +0x170
    std::uint32_t unk_174 = 0;                         // +0x174
    std::uint32_t unk_178 = 0;                         // +0x178
    std::int32_t patchesPerSide = 0;                   // +0x17c
    std::int32_t lodDistanceCount = 0;                 // +0x180 (entries of the squared LOD distance table)
    core::aabbox3d<float> boundingBox;                 // +0x184 (spans +0x188 .. +0x198)
    std::vector<double> lodDistanceSquared;            // +0x19c (squared LOD distances, lodDistanceCount entries)
    std::uint32_t unk_1a0 = 0;                         // +0x1a0
    std::vector<STerrainPatch> patches;                // +0x1a8, patchesPerSide squared entries
    std::uint32_t unk_1ac = 0;                         // +0x1ac (material holder)
    std::uint32_t unk_1b0 = 0;                         // +0x1b0 (render buffer counted pointer)
    std::uint32_t unk_1b4 = 0;                         // +0x1b4
    std::uint32_t unk_1b8 = 0;                         // +0x1b8
    std::uint8_t unk_1bc = 0;                          // +0x1bc
    std::uint8_t lodDistanceOverridden = 0;            // +0x1bd (set to 1 by overrideLODDistance)
    std::uint8_t rotationPivotFlag = 0;                // +0x1be (cleared by setRotationPivot)
    std::uint8_t unk_1bf = 0;                          // +0x1bf (dirty byte set by setPosition, setRotation, setScale)
    float unk_1c0 = 0.0f;                              // +0x1c0
    float unk_1c4 = 0.0f;                              // +0x1c4
    float unk_1c8 = 0.0f;                              // +0x1c8
    float unk_1cc = 0.0f;                              // +0x1cc
    float unk_1d0 = 0.0f;                              // +0x1d0
    std::uint32_t unk_1d4 = 0;                         // +0x1d4
    float unk_1d8 = 0.0f;                              // +0x1d8
    float unk_1dc = 0.0f;                              // +0x1dc
    float unk_1e0 = 0.0f;                              // +0x1e0
    float unk_1e4 = 0.0f;                              // +0x1e4
    float unk_1e8 = 0.0f;                              // +0x1e8
    float unk_1ec = 0.0f;                              // +0x1ec
    std::uint32_t unk_1f0 = 0;                         // +0x1f0
    std::uint8_t pad_1f4[12] = {};                     // +0x1f4 .. +0x1ff (not modelled)
    std::uint32_t unk_200 = 0;                         // +0x200
    std::uint32_t unk_204 = 0;                         // +0x204
    std::uint32_t unk_208 = 0;                         // +0x208
    std::uint32_t unk_20c = 0;                         // +0x20c
    std::uint32_t unk_210 = 0;                         // +0x210
};

}  // namespace glitch::scene
