// Batch v1-vendor-gameswf-bitmap_info_ogl-434136-0 (part 1 of 2): gameswf::bitmap_info_ogl, the glitch texture
// backend. Class and layout: vendor-gameswf/include/vendor-gameswf/bitmap_info_ogl.hpp (shared with part 2, which is
// src/v1-vendor-gameswf-bitmap_info_ogl-434136-1.cpp). The filter steps are in bitmap_info_ogl_filter.hpp.
//
// Status (ledger.d/v1-vendor-gameswf-bitmap_info_ogl-434136-0.csv):
//   reconstructed  activate, set_writable, get_width, get_height, is_layout, p2, the (driver) and (driver, membuf)
//                  constructors (C1).
//   partial        set_min_filter, set_mag_filter, get_internal_width, get_internal_height, unlock, lock and the
//                  (driver, ITexture*) constructor (C1). Each MISSING site names its layout request below.
//   skipped        the C2 twins 0x7d4bfc, 0x7d4d30, 0x7d4e50 (same bodies as the C1 defined here), and 0x7d515c and
//                  0x7d5004 (the C2 of the (driver, int, int) and (driver, image::rgb) constructors; their C1 bodies,
//                  0x7d5294 and 0x7d5914, are in part 2).
//
// Layout requests (the stand-in types are in glitch-core-io/deps/glitch_core_objects.hpp):
//   M1  glitch::video::ITexture (stand-in) declares only slotType at +0x38, and it has no unlock() or lock(bool).
//       The binary reads the width at +0x20, the height at +0x24, the flags at +0x38 (format, wrap and filter bits),
//       the mip count at +0x3e (byte) and the dirty word at +0x40 (uint16). It calls unlock() (0x5fdc84) and
//       lock(bool) (0x5fe300). glitch-video/texture.hpp declares the words, but as a different class with the same name.
//       Same request as L8 in the part 2 batch.
//   M2  glitch::video::CImage (stand-in) declares no members. The binary reads the width at +0x10 (returned by
//       get_internal_width when m_image is set); glitch-video/image.hpp declares it but private.
//   M3  Namespace: the binary's symbols are _ZN15bitmap_info_ogl... (global scope), while the shared header puts
//       bitmap_info_ogl in namespace gameswf. This batch follows the header, so both parts compile together. The
//       integrator decides which is right.
//   M4  membuf: the binary's constructor takes gameswf::membuf const* (mangled N7gameswf6membufE). The shared header
//       stores ::membuf* (non-const, global scope), and release() destroys it. This batch takes const ::membuf* and
//       stores it with const_cast, the same pointer value the binary stores at +0x1c.
//
// Constant read from the binary: s_filter_map (0x90c044, int32[6]) = {0, 1, 2, 3, 4, 5}; see bitmap_ogl_detail.

#include "vendor-gameswf/bitmap_info_ogl.hpp"

#include <cstdint>

#include "glitch-core-io/deps/glitch_core_objects.hpp"  // IVideoDriver, ITexture, CImage (stand-ins, counted)
#include "vendor-gameswf/bitmap_info_ogl_filter.hpp"     // min_filter_step, mag_filter_step (texture words, M1)

namespace gameswf {

// 0x7d3cbc _ZN15bitmap_info_ogl8activateEv
// Empty override (bx lr). Vtable slot +0x10.
void bitmap_info_ogl::activate() {
}

// 0x7d3cc0 _ZN15bitmap_info_ogl12set_writableEv
// m_writable = 1 (byte +0x0c). Vtable slot +0x14.
void bitmap_info_ogl::set_writable() {
    m_writable = true;
}

// 0x7d3ccc _ZN15bitmap_info_ogl14set_min_filterEN7gameswf11bitmap_info11filter_modeE
// PARTIAL. The mode is stored at +0x2c first, as the binary does. When m_texture is set, the binary applies the min step
// to the texture words (flags +0x38, mip count +0x3e, dirty +0x40, M1). That step is bitmap_ogl_filter::min_filter_step.
// Its r0 result is not part of the void signature and is not reproduced.
void bitmap_info_ogl::set_min_filter(filter_mode mode) {
    m_min_filter = static_cast<int>(mode);  // +0x2c
    if (!m_texture) {
        return;
    }
    // MISSING (M1): bitmap_ogl_filter::min_filter_step(texture->+0x38, texture->+0x40, texture->+0x3e, m_min_filter).
}

// 0x7d3d34 _ZN15bitmap_info_ogl14set_mag_filterEN7gameswf11bitmap_info11filter_modeE
// PARTIAL. Same as set_min_filter, with the mag step (bits 15..17, no mip-count test) and the mag index at +0x30.
void bitmap_info_ogl::set_mag_filter(filter_mode mode) {
    m_mag_filter = static_cast<int>(mode);  // +0x30
    if (!m_texture) {
        return;
    }
    // MISSING (M1): bitmap_ogl_filter::mag_filter_step(texture->+0x38, texture->+0x40, m_mag_filter).
}

// 0x7d3d84 _ZNK15bitmap_info_ogl9get_widthEv
// Returns m_width (+0x20). Vtable slot +0x24.
int bitmap_info_ogl::get_width() const {
    return m_width;
}

// 0x7d3d8c _ZNK15bitmap_info_ogl10get_heightEv
// Returns m_height (+0x24). Vtable slot +0x28.
int bitmap_info_ogl::get_height() const {
    return m_height;
}

// 0x7d3d94 _ZNK15bitmap_info_ogl9is_layoutEv
// Returns whether m_texture (+0x10) is set. The binary returns a bool in r0, and the base declares int, so the
// override returns int 0 or 1. Vtable slot +0x34.
int bitmap_info_ogl::is_layout() const {
    return m_texture.get() != nullptr ? 1 : 0;
}

// 0x7d3da4 _ZNK15bitmap_info_ogl18get_internal_widthEv
// PARTIAL. Binary: the texture width (texture +0x20, M1) when m_texture is set; else the CImage width (+0x10, M2) when
// m_image is set; else m_width. Both field reads are MISSING. The return is m_width, which is also what the binary
// returns when neither is set. Vtable slot +0x38.
int bitmap_info_ogl::get_internal_width() const {
    // MISSING (M1, M2): return texture->+0x20 when m_texture is set, else CImage->+0x10 when m_image is set.
    return m_width;
}

// 0x7d3dcc _ZNK15bitmap_info_ogl19get_internal_heightEv
// PARTIAL. Same as get_internal_width with the heights: texture +0x24 (M1), CImage +0x14 (M2), else m_height.
// Vtable slot +0x3c.
int bitmap_info_ogl::get_internal_height() const {
    // MISSING (M1, M2): return texture->+0x24 when m_texture is set, else CImage->+0x14 when m_image is set.
    return m_height;
}

// 0x7d3fcc _ZN15bitmap_info_ogl2p2Ei
// The smallest power of two that holds value (1 for value <= 1). It is halved when value / power is under 0.6, so the
// result is the power of two nearest below value when value fills less than 60% of the next one. The binary reads
// float, then compares with the double constant 0.6, which C++ does by promoting the float.
int bitmap_info_ogl::p2(int value) {
    int i;
    if (value > 1) {
        for (i = 1; i < value; i *= 2) {
        }
    } else {
        i = 1;
    }
    if (static_cast<float>(value) / static_cast<float>(i) < 0.6) {
        i >>= 1;
    }
    return i;
}

// 0x7d4048 _ZN15bitmap_info_ogl6unlockEv
// PARTIAL. Binary: a tail call to glitch::video::ITexture::unlock(m_texture) (0x5fdc84), with no null test. The call
// is MISSING (M1): the stand-in ITexture declares no unlock(). Vtable slot +0x1c.
void bitmap_info_ogl::unlock() {
    // MISSING (M1): m_texture->unlock(), with the binary's unchecked pointer.
}

// 0x7d4050 _ZN15bitmap_info_ogl4lockEv
// PARTIAL. Binary: calls the virtual layout() through vtable slot +0x08 on this, then tail-calls
// glitch::video::ITexture::lock(m_texture, 0) (0x5fe300) and returns its result. The layout() call is written. The
// lock call is MISSING (M1): the stand-in ITexture declares no lock(bool). The stub return is 0. Vtable slot +0x18.
int bitmap_info_ogl::lock() {
    layout();
    // MISSING (M1): return glitch::video::ITexture::lock(m_texture.get(), false) (0x5fe300).
    return 0;
}

// 0x7d4b3c _ZN15bitmap_info_oglC1EPN6glitch5video12IVideoDriverEiiPKN7gameswf6membufE
// 0x7d4bfc (C2): skipped, same body.
// Stores the membuf (+0x1c), the size (+0x20, +0x24) and the driver (+0x28). The other fields are the header defaults:
// m_writable 0, m_texture_registered 0, texture, view and image null, filters 1 (+0x2c, +0x30). M4 for the membuf.
bitmap_info_ogl::bitmap_info_ogl(glitch::video::IVideoDriver* driver, int width, int height, const ::membuf* buf)
    : m_membuf(const_cast<::membuf*>(buf)), m_width(width), m_height(height), m_driver(driver) {
}

// 0x7d4c74 _ZN15bitmap_info_oglC1EPN6glitch5video12IVideoDriverEPNS1_8ITextureE
// 0x7d4d30 (C2): skipped, same body.
// PARTIAL. Binary: m_texture is the texture, counted (its reference count goes up when it is not null). The size
// copy is MISSING (M1): m_width = texture->+0x20 and m_height = texture->+0x24, copied with no null test. The header
// defaults give the other fields: m_writable 0, m_texture_registered 0, membuf 0, image null, filters 1.
bitmap_info_ogl::bitmap_info_ogl(glitch::video::IVideoDriver* driver, glitch::video::ITexture* texture)
    : m_driver(driver) {
    m_texture = texture;  // +0x10, counted
    // MISSING (M1): m_width = texture->+0x20; m_height = texture->+0x24.
}

// 0x7d4dbc _ZN15bitmap_info_oglC1EPN6glitch5video12IVideoDriverE
// 0x7d4e50 (C2): skipped, same body.
// Stores the driver (+0x28). The header defaults give the rest: size 0 (+0x20, +0x24), membuf 0 (+0x1c), texture,
// view and image null, filters 1 (+0x2c, +0x30), writable 0 (+0x0c), texture_registered 0 (+0x0d).
bitmap_info_ogl::bitmap_info_ogl(glitch::video::IVideoDriver* driver) : m_driver(driver) {
}

}  // namespace gameswf
