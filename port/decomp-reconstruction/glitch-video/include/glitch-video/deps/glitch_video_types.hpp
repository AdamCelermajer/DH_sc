// Stand-ins for glitch::video enums and interface types that the null driver
// signatures mention. Enum values are not reconstructed, so each enum is an
// int32_t alias; only the values returned by the stub bodies are used.
#pragma once

#include <cstdint>

#include "glitch-video/deps/glitch_core.hpp"
#include "glitch-video/deps/intrusive_ptr.hpp"

namespace glitch::video {

using E_BLEND_FACTOR = int32_t;
using E_BLEND_EQUATION = int32_t;
using E_FACE_SIDE = int32_t;
using E_FACE_WINDING = int32_t;
using E_COMPARE_FUNC = int32_t;
using E_POLYGON_MODE = int32_t;
using E_STENCIL_OP = int32_t;
using E_TRANSFORMATION_STATE = int32_t;
using E_TEXTURE_CUBE_MAP_FACE = int32_t;
using E_PIXEL_FORMAT = int32_t;
using E_DRIVER_TYPE = int32_t;
using E_BUFFER_TYPE = int32_t;
using E_BUFFER_USAGE = int32_t;
using E_PRIMITIVE_TYPE = int32_t;

class ITexture;
class IImage;
class IRenderBuffer;
class IRenderTarget;
class CMaterial;
class CMaterialRenderer;
class CVertexStreams;
class CPrimitiveStream;
class CDriverBinding;

// Texture load descriptor. Only the fields written by the ATC header reader (0x602e34) are named;
// the DDS reader (0x604398) uses the same +0x00, +0x04, +0x10, +0x14, +0x18 and +0x1c fields.
struct STextureDesc {
    uint32_t type = 0;          // +0x00, zeroed by the ATC reader
    uint32_t format = 0;        // +0x04, 21 or 22 for ATC; left unchanged for other magic values
    uint32_t unknown08 = 0;     // +0x08, zeroed by the ATC reader; meaning not identified
    uint32_t unknown0c = 0;     // +0x0c, not written by the ATC reader
    uint32_t width = 0;         // +0x10
    uint32_t height = 0;        // +0x14
    uint32_t depth = 0;         // +0x18
    uint8_t flag = 0;           // +0x1c, set to 1 by the ATC reader
};

namespace detail::driver {
// Placeholder for the driver render-state block. Layout not reconstructed.
struct SRenderState {};
}  // namespace detail::driver

}  // namespace glitch::video
