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

struct STextureDesc {};  // placeholder for the texture load descriptor

namespace detail::driver {
// Placeholder for the driver render-state block. Layout not reconstructed.
struct SRenderState {};
}  // namespace detail::driver

}  // namespace glitch::video
