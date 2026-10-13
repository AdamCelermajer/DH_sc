// glitch::scene free functions on meshes (IMesh / IAnimatedMesh / CMeshBuffer). Signatures follow the demangled
// symbols of the original (TRACK-B decision 10), not the earlier recon_NN stubs. Each declaration carries its
// original address. Functions that are still partial are declared in the recon slice headers only.
#pragma once

#include <cstdint>

#include "glitch-core-io/boost/intrusive_ptr.hpp"
#include "glitch-scene-collada-gui/scene/IMesh.hpp"

namespace glitch::video {
class CMaterialVertexAttributeMap;
}  // namespace glitch::video

namespace glitch::scene {

// 0x578d9c: sum over the techniques of the material renderer of the pass count (uint16 result, the sum truncated
// to 16 bits, 0 without a renderer technique).
std::uint32_t getNbVertexAttributeMapsDataSize(const boost::intrusive_ptr<video::CMaterialVertexAttributeMap>& map);

// 0x59af20: number of primitives of all mesh buffers of the mesh (0 for a null handle).
std::int32_t getPolyCount(const boost::intrusive_ptr<IMesh>& mesh);

// 0x59afb8: primitive count of the frame-0 mesh of an animated mesh (0 for a null handle or no frames).
std::int32_t getPolyCount(const boost::intrusive_ptr<IAnimatedMesh>& mesh);

// 0x59c7a4: scales the texture coordinates (float2 stream of the attribute `channel`, bit in the stream mask) of one buffer.
void scaleTCoords(const boost::intrusive_ptr<CMeshBuffer>& buffer, const core::vector2d<float>& scale,
                  std::uint32_t channel);

// 0x59c8e0: scaleTCoords over every buffer of the mesh.
void scaleTCoords(const boost::intrusive_ptr<IMesh>& mesh, const core::vector2d<float>& scale, std::uint32_t channel);

}  // namespace glitch::scene
