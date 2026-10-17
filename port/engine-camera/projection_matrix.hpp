#pragma once

#include "camera_math.hpp"

#include <cstdint>

namespace dh2::engine_camera::projection_matrix {

using Word = std::uint32_t;

enum class Status : std::int32_t { complete, invalid_argument };

// Adapted from Irrlicht SVN r6038's matrix4.h LH builders; upstream license
// text is retained in port/irrlicht-android/upstream/readme.txt. The DH2 engine
// uses the same z-in-[0,1] projection terms but appends a one-byte identity
// hint to its 16 binary32 matrix words.
Status perspective_fov(Word fov, Word aspect, Word near_plane, Word far_plane,
                       camera_math::Matrix4* output) noexcept;
Status perspective_fov_infinite(Word fov, Word aspect, Word near_plane,
                                camera_math::Matrix4* output) noexcept;
Status orthographic(Word width, Word height, Word near_plane, Word far_plane,
                    camera_math::Matrix4* output) noexcept;

// Source CCommonGLDriverBase::fixUpProjectionMatrix, limited to the original
// Android driver's default Orientation=0. `flip_y` is the source driver byte
// at +0x4a0; its constructor initializes it to zero.
Status fix_up_for_gles_default_orientation(camera_math::Matrix4* matrix,
                                           bool flip_y = false) noexcept;

} // namespace dh2::engine_camera::projection_matrix
