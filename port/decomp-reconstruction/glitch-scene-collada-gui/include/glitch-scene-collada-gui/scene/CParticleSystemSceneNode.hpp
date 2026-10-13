// glitch::scene::CParticleSystemSceneNode: the particle system node (tag 'ptcl'). Canonical header of the scene-owner wave. It replaces the
// stand-in class of scene_simple_accessors.hpp (a plain struct with no base) and derives from scene::IParticleSystemSceneNode, as the
// original does. The glitch::collada::CParticleSystemSceneNode is a different class (collada_scene_accessors.hpp) and is left alone.
//
// Evidence (libDungeonHunter2.so, ARM, 32-bit layout; constructor 0x6c2478 (272 bytes), tools/trackb/field_sweep.py with 39 queue
// functions):
//   +0x000 scene::IParticleSystemSceneNode (ISceneNode, ends at +0x130)
//   +0x130 affector list: a self-linked sentinel (the constructor points both words at +0x130). addAffector and the destructor use it.
//   +0x138 emitter (IParticleEmitter*), getEmitter 0x6c0df8
//   +0x13c particle records: begin, end and capacity (vector of 0x44-byte records); getNumberOfParticles 0x6c0dd0 reads begin/end
//   +0x148 particle size (width, height); the constructor sets 5 x 5 through setParticleSize (0x6c0ef4)
//   +0x154 unknown word, 0xffff at construction
//   +0x158 bounding box (6 floats): (-1,-1,-1)..(1,1,1) at construction
//   +0x170 render-from-identity flag (0 at construction); +0x174 unknown word (0)
//   +0x17c billboard type (0 at construction; setBillboardType 0x6c0f0c)
//   +0x180 particles-are-global flag (1 at construction; setParticlesAreGlobal 0x6c0eec)
//   +0x184 transparency (1.0 at construction; setTransparency 0x6c0ed8)
//   +0x188 glitch::IReferenceCounted virtual base (vptr); +0x18c the reference count, 1 at construction. Not modelled.
// Bodies that need the emitter or affector code (serialize, deserialize, clone, onRegisterSceneNode, the buffer reallocation) are
// PARTIAL or declared only; see src/scene_simple_accessors.cpp and the ledger.
#pragma once

#include <cstdint>
#include <vector>

#include "glitch-scene-collada-gui/scene/IParticleSystemSceneNode.hpp"

namespace glitch::scene {

class IParticleEmitter;

class CParticleSystemSceneNode : public IParticleSystemSceneNode {
public:
    // One particle record, 0x44 bytes in the original. The fields are not mapped.
    struct SParticleRecord {
        std::uint8_t bytes[0x44] = {};
    };

    // Default node (identity transform, unit scale): the constructor 0x6c2478 with id -1 and zero position and rotation. The original's
    // constructor takes a bool and the transform; the port's test users build the node from its id only.
    explicit CParticleSystemSceneNode(int id = -1);
    // 0x6c2478 family, the D1/D2/D0 destructors are PARTIAL (the affector list and the emitter release are not reconstructed).
    ~CParticleSystemSceneNode() override;

    // vptr+0xbc, 0x6c0dc4: returns 'ptcl'.
    E_SCENE_NODE_TYPE getType() const override;
    // 0x6c0ee4: the box at +0x158.
    const core::aabbox3d<float>& getBoundingBox() const override;
    // 0x6c0e58: returns 1.
    std::uint32_t getMaterialCount() const override;
    // 0x6c0ee0: empty (the draw path is not reconstructed).
    void render(void* renderContext) override;

    // 0x6c0df8: the emitter at +0x138.
    IParticleEmitter* getEmitter();
    // 0x6c0ed8: stores the transparency at +0x184.
    void setTransparency(float alpha);
    // 0x6c0eec: stores the particles-are-global flag at +0x180.
    void setParticlesAreGlobal(bool global);
    // 0x6c0f08: empty (the buffer reallocation is not reconstructed).
    void reallocateBuffers();
    // 0x6c0dd0: the particle count (the record vector size).
    std::int32_t getNumberOfParticles() const;
    // 0x6c0ef4: stores the particle size at +0x148.
    void setParticleSize(const core::dimension2d<float>& size);
    // 0x6c0f0c: stores the billboard type at +0x17c.
    void setBillboardType(std::uint32_t type);

    IParticleEmitter* emitter = nullptr;                    // +0x138
    std::vector<SParticleRecord> particles;                 // +0x13c (begin, end, capacity)
    core::dimension2d<float> particleSize{5.0f, 5.0f};      // +0x148
    std::uint32_t unk_154 = 0xFFFFu;                        // +0x154
    core::aabbox3d<float> boundingBox{{-1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}};  // +0x158
    bool renderFromIdentity = false;                        // +0x170
    std::uint32_t unk_174 = 0;                              // +0x174
    std::uint32_t billboardType = 0;                        // +0x17c
    bool particlesAreGlobal = true;                         // +0x180
    float transparency = 1.0f;                              // +0x184
};

}  // namespace glitch::scene
