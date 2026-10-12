// Track B batch e1-glitch-scene-collada-gui-glitch_scene-0: helpers of glitch::scene (anonymous namespace).
// createTriangles reads a vertex stream and map/unmap a buffer. This header holds the layouts it needs
// from glitch::video, named as in glitch-video/vertex_streams.hpp (SVertexStream, 16 bytes) and
// glitch-video/buffer.hpp (IBuffer). They are stand-ins for this category only: glitch-video is not on
// this category's include path. Remove them when the category includes the glitch-video headers.
#pragma once

#include <cstdint>

namespace glitch::video {

// Stand-in for glitch::video::IBuffer. Only the two members createTriangles calls are declared; their
// bodies are in glitch-video (0x5a1adc map, 0x637a68 unmap).
class IBuffer {
public:
    // map(E_BUFFER_READ_MAP_ACCESS): the batch passes access value 1 (MOV R1, #1 before the BL).
    // Returns the mapped base pointer, or nullptr.
    std::uint8_t* map(std::int32_t access);

    // Releases one map reference. The batch inlines this release in createTriangles; the body with the
    // unmap callback (vtable +0x18) is in glitch-video.
    void unmap();
};

// Stand-in for glitch::video::SVertexStream: 16 bytes in the original.
struct SVertexStream {
    IBuffer* buffer = nullptr;         // +0x00 (boost::intrusive_ptr<IBuffer> in the original)
    std::uint32_t offset = 0;          // +0x04: added to the mapped base pointer
    std::uint16_t attribute = 0;       // +0x08
    std::uint16_t valueType = 0;       // +0x0a
    std::uint16_t componentCount = 0;  // +0x0c: 2, 3 or 4 select the createTriangles layouts
    std::uint16_t stride = 0;          // +0x0e: bytes per vertex
};

}  // namespace glitch::video
