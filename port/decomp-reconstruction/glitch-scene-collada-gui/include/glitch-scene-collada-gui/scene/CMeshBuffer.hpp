// glitch::scene::CMeshBuffer: one vertex/index buffer pair of a mesh. Canonical header of the engine unification wave
// (u1-scene). It replaces the CMeshBuffer layout struct in layouts/scene.hpp and the empty stub that glitch-core-io's
// deps/glitch_core_objects.hpp carries (a class with only the IReferenceCounted base).
//
// Layout (original 32-bit offsets; 56 bytes, operator new(0x38) in clone 0x6bcb90; constructors 0x6b88a8 and 0x6bcf80):
//   +0x00 vptr, +0x04 reference count (glitch::IReferenceCounted; the vtable is 20 bytes: D1 0x6bc8c4, D0 0x6bca38,
//         onDelete 0x6bc6d8, which forwards to the owner's onDelete)
//   +0x08, +0x0c, +0x10 three shared blocks (count at block + 0x00, element storage at block + 0x0c; types not confirmed)
//   +0x14 video::CVertexStreams* the vertex stream table (a counted reference; the count is at CVertexStreams + 0x00)
//   +0x18 .. +0x2f a video::CPrimitiveStream embedded by value (the constructor copies the 24-byte argument: index buffer
//         +0x18 with a reference, byte offset +0x1c, element count +0x20, first vertex +0x24, last vertex +0x28, index type
//         +0x2c, primitive type +0x2e)
//   +0x30 owner (an IReferenceCounted-derived object: onDelete forwards to its onDelete slot and the destructor runs its
//         deleting destructor), +0x34 byte set to 1 by the constructors
//
// This header includes glitch-video's CVertexStreams and CPrimitiveStream. Because glitch-core-io/io/interfaces.hpp still
// declares stand-ins for the video classes and a stub scene::CMeshBuffer, a translation unit must not include both.
#pragma once

#include <cstdint>

#include "glitch-core-io/core/reference_counted.hpp"
#include "glitch-scene-collada-gui/scene/scene_types.hpp"
#include "glitch-video/primitive_stream.hpp"
#include "glitch-video/vertex_streams.hpp"

namespace glitch::collada {
struct SMesh;
struct SBufferConfig;
}  // namespace glitch::collada

namespace glitch::scene {

class CMeshBuffer : public glitch::IReferenceCounted {
public:
    // Not part of the original: an empty buffer with count 0 and no vertex streams, for tests and derived stand-ins.
    CMeshBuffer();
    // 0x6b88a8, 0x6d11d4 and 0x6d5ed4 (three listings of one constructor): a buffer for the vertex attributes in
    // `attributeMask` (CVertexStreams::allocate(mask)) that renders `stream`. The count starts at 0, so the first holder
    // adds the use (clone() and the append buffer do). The last argument is not read.
    CMeshBuffer(std::uint32_t attributeMask, const video::CPrimitiveStream& stream, bool flag);
    // 0x6bcf80 (C2) and 0x6bd9f8 (C1, skipped): from a Collada mesh record. PARTIAL, see src.
    CMeshBuffer(video::IVideoDriver* driver, collada::SMesh& mesh, std::uint32_t primitiveIndex,
                const collada::SBufferConfig& vertexConfig, const collada::SBufferConfig& indexConfig, bool onDemand);
    // 0x6bc8c4 (D1), 0x6bca38 (D2), 0x6bca1c (D0, skipped): deletes the owner, then the streams and the blocks.
    ~CMeshBuffer() override;

    // vptr+0x08, 0x6bc6d8: forwards to the owner's onDelete slot.
    void onDelete() override;
    // 0x6bcb90: a copy with a cloned vertex stream table and the same primitive stream; the blocks and the owner are not
    // shared. The copy is returned with one use.
    boost::intrusive_ptr<CMeshBuffer> clone() const;

    void* block_08 = nullptr;                                    // +0x08
    void* block_0c = nullptr;                                    // +0x0c
    void* block_10 = nullptr;                                    // +0x10
    boost::intrusive_ptr<video::CVertexStreams> vertexStreams;   // +0x14
    video::CPrimitiveStream primitiveStream;                     // +0x18 .. +0x2f
    glitch::IReferenceCounted* owner = nullptr;                  // +0x30
    std::uint8_t flag_34 = 1;                                    // +0x34 set to 1 by the constructors; role not confirmed
};

}  // namespace glitch::scene
