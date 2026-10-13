// glitch::collada::CCoronasSceneNode: a billboard quad (a corona) with a material, a size and a vertex colour. Canonical header of the
// scene-owner wave. It replaces the CCoronasSceneNode stand-in of collada_scene_accessors.hpp and the address-named stub of
// generated_stubs.hpp (getType 0x6e5a2c). The class derives from the canonical scene::ISceneNode, as the other scene nodes do.
//
// Evidence (libDungeonHunter2.so, ARM, 32-bit layout; tools/trackb/field_sweep.py glitch::collada::CCoronasSceneNode, 16 queue rows):
//   +0x000 scene::ISceneNode (ends at +0x130)
//   +0x130 blocking callback: isBlocked calls it with the segment (camera -> this) as six floats and returns its int result.
//   +0x148 width, +0x14c height (setSize, 0x6e5a40, stores both and maps a zero to 1)
//   +0x150 bounding box (6 floats; the constructor sets it to (-1,-1,-1)..(1,1,1))
//   +0x168 material (counted intrusive pointer to video::CMaterial)
//   +0x16c four corner records, stride 36 (0x24): position (3 floats), then the fields the render path fills (+0x0c, +0x10, +0x14,
//          +0x18, +0x1c, not mapped), then the vertex colour word at +0x20. The vertex stream is 4 * 36 = 144 bytes (setupStreams stride 144).
//   +0x1fc vertex streams (counted pointer to video::CVertexStreams); the constructor allocates it with CVertexStreams::allocate.
//   +0x204, +0x208: not mapped; the constructor stores 1 at +0x208.
// Constructors: 0x6e7164 (C1) and 0x6e7488 (C2) build the node from a driver; 0x6e7778 (C1) and 0x6e7a80 (C2) from a database
// record. Destructors 0x6e703c (D1), 0x6e70e4 (D2); D0 0x6e70c8 is skipped. Bodies that need the scene manager (+0x110), the camera
// record or the vertex stream code are PARTIAL and are listed in the ledger.
#pragma once

#include <cstdint>

#include "glitch-scene-collada-gui/scene/ISceneNode.hpp"

namespace glitch {
namespace core {
template <typename T>
class dimension2d;
}
namespace video {
class CMaterial;
class CVertexStreams;
class IVideoDriver;
struct SColor;
}  // namespace video
}  // namespace glitch

namespace glitch::collada {

class CColladaDatabase;
class CRootSceneNode;
struct SCoronas;

// One corner of the quad, 36 bytes (9 words). The colour is the vertex colour word at +0x20.
struct SCoronaCorner {
    float position[3] = {0.0f, 0.0f, 0.0f};  // +0x00
    float unk_0c = 0.0f;                     // +0x0c
    float unk_10 = 0.0f;                     // +0x10
    float unk_14 = 0.0f;                     // +0x14
    float unk_18 = 0.0f;                     // +0x18
    float unk_1c = 0.0f;                     // +0x1c
    std::uint32_t color = 0;                 // +0x20
};
static_assert(sizeof(SCoronaCorner) == 0x24, "corona corner is 36 bytes");

class CCoronasSceneNode : public scene::ISceneNode {
public:
    // 0x6e7164 (C1) / 0x6e7488 (C2): the driver constructor. PARTIAL: the vertex stream allocation and setupStreams are not
    // reconstructed here; the base, the size, the material and the corner colours are set.
    CCoronasSceneNode(video::IVideoDriver* driver, int id, const core::vector3d<float>& position,
                      boost::intrusive_ptr<video::CMaterial> material, const core::dimension2d<float>& size,
                      video::SColor color);
    // 0x6e7778 (C1) / 0x6e7a80 (C2): the database constructor. PARTIAL: declared only, no body.
    CCoronasSceneNode(const CColladaDatabase& db, video::IVideoDriver* driver, SCoronas& coronas, CRootSceneNode* root);
    // 0x6e703c (D1) / 0x6e70e4 (D2): PARTIAL, empty; the vertex stream count is not dropped here.
    ~CCoronasSceneNode() override;

    // vptr+0xbc, 0x6e5a2c: returns the constant 1936618083 (0x736e7263).
    scene::E_SCENE_NODE_TYPE getType() const override;
    // 0x6e5a38: the box at +0x150.
    const core::aabbox3d<float>& getBoundingBox() const override;
    // 0x6e5a40: stores the width and height, mapping a zero to 1.
    void setSize(const core::dimension2d<float>& size);
    // 0x6e5bc8: PARTIAL. Calls the blocking callback at +0x130 with the segment from the absolute position of this node to the
    // camera's; the camera lookup (scene manager +0x110) is not reconstructed, so this returns 0.
    std::int32_t isBlocked();
    // 0x6e5c40: registers the node with the scene manager unless it is blocked. PARTIAL: the registration call is not
    // reconstructed; returns 1 as the original does.
    int onRegisterSceneNode() override;
    // 0x6e5d14: PARTIAL, declared only. The draw path (camera basis, corner expansion, vertex stream fill, material setup) is
    // not reconstructed.
    void render(void* renderContext) override;
    // 0x6e5a90 / 0x6e5b0c: PARTIAL, declared only; the attribute slots are written by the layout evidence in recon/slice_33.
    void serializeAttributes(io::IAttributes* attributes, io::SAttributeReadWriteOptions* options) const override;
    void deserializeAttributes(io::IAttributes* attributes, io::SAttributeReadWriteOptions* options) override;

    // +0x130: blocking callback (see isBlocked). Null means never blocked.
    std::int32_t (*blockCallback)(const float* segment) = nullptr;  // +0x130
    float width = 0.0f;                                             // +0x148
    float height = 0.0f;                                            // +0x14c
    core::aabbox3d<float> boundingBox;                              // +0x150
    boost::intrusive_ptr<video::CMaterial> material;                // +0x168
    SCoronaCorner corners[4];                                       // +0x16c
    video::CVertexStreams* vertexStreams = nullptr;                 // +0x1fc
    std::uint32_t unk_208 = 1;                                      // +0x208 (set to 1 by the constructor)
};

}  // namespace glitch::collada
