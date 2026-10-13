// Batch v1-vendor-gameswf-bitmap_info_ogl-434136-1 (part 2 of 2): gameswf::bitmap_info_ogl, the glitch texture
// backend. Class and layout: vendor-gameswf/include/vendor-gameswf/bitmap_info_ogl.hpp.
//
// Status of the 12 rows (ledger: ledger.d/v1-vendor-gameswf-bitmap_info_ogl-434136-1.csv):
//   reconstructed  0x7d5b80 unlayout (forwards to release)
//   skipped        0x7d5564 and 0x7d57e0 (C2 constructors, same code as C1), 0x7d5c04 D0 (compiler-generated deleting
//                  destructor)
//   partial        0x7d5294, 0x7d5404, 0x7d567c, 0x7d5914 (constructors), 0x7d5a9c release, 0x7d5b2c set_texture,
//                  0x7d5b84 D1, 0x7d5c20 layout. Each body keeps the binary's order and writes every field it can name.
//                  The missing parts are marked MISSING at the place they belong, with the Hex-Rays call and its arguments.
//
// What is missing, in short (layout requests in the batch report):
//   L1  glitch::video::IVideoDriver member +0xe0: the CTextureManager pointer. Not declared on IVideoDriver in
//       the tree (glitch-video/video_driver.hpp has no such member; the glitch-core-io stand-in is empty).
//   L2  IVideoDriver vtable +0x84: the texture view (m_texture_view). Slot not named.
//   L3  IVideoDriver vtable +0xa0: setOption(16, flag) virtual call. The stand-in setOption is not virtual.
//   L4  IVideoDriver option word +0x88 (bit 0x10) and driver flag word +0x9c (bits 0x6000). Not declared.
//   L5  CTextureManager::createImage(out, mgr, 12, {w, h}) and CTextureManager::addTexture(...) for the
//       constructors, and addTexture(out, mgr, name, const intrusive_ptr<CImage>&, bool, E_TEXTURE_LAYOUT) and
//       getTexture(out, IReadFile*, const char*, bool) in layout(). None are in the tree.
//   L6  CTextureManager word +0x74 (bit 0x10, the writable flag). Not declared (texture_manager.hpp has no +0x74).
//   L7  CImage data pointer (+0x08, the pixels). glitch-core-io's CImage stand-in has no members.
//   L8  ITexture +0x20 (width), +0x24 (height), +0x38 (format word), +0x3e (mip count), +0x40 (dirty word).
//       The glitch-core-io ITexture stand-in declares none of them (glitch-video/texture.hpp has +0x20, +0x38,
//       +0x3e, +0x40 under a different class).
//   L9  PFDTable[format * 40] flags (glitch::video::pixel_format::detail::PFDTable) and the CImage format and
//       flag bytes (+0x20, +0x28). Not declared.
//   L10 glitch::io::createMemoryReadFile (glitch-core-io/io/memory_read_file.hpp): the header does not compile on its
//       own (CMemoryReadFile::getFileName and getFullPath return std::string& against IReadFile's const char*).
//       It is outside this batch, so the membuf branch of layout() is not written.
//
// Constants read from the binary: s_filter_map (0x90c044) = {0, 1, 2, 3, 4, 5}, see bitmap_ogl_detail.

#include "vendor-gameswf/bitmap_info_ogl.hpp"

#include <cstdint>
#include <cstdio>

#include "glitch-core-io/deps/glitch_core_objects.hpp"  // IVideoDriver, ITexture, CImage (stand-ins, counted)

namespace gameswf {

// ---------------------------------------------------------------------------------------------------------------
// 0x7d5294 _ZN15bitmap_info_oglC1EPN6glitch5video12IVideoDriverEii
// PARTIAL: the field setup is written in binary order. Not written: CTextureManager::addTexture(out, mgr, {w, h},
// "gameswf_target", 14, 1), with mgr = driver +0xe0 (L1, L5). Its result is stored in m_texture (+0x10, counted), and
// then the driver view call (vtable +0x84, L2) stores its result in m_texture_view (+0x14).
bitmap_info_ogl::bitmap_info_ogl(glitch::video::IVideoDriver* driver, int width, int height)
    : m_texture_registered(true), m_width(width), m_height(height), m_driver(driver) {
    // binary: m_membuf = 0, m_writable = 0, m_texture = m_texture_view = m_image = 0, m_texture_registered = 1,
    // m_min_filter = 1 (+0x2c), m_mag_filter = 1 (+0x30). The defaults in the header give the same values.
    // MISSING: addTexture(&tmp, driver+0xe0, {width, height}, "gameswf_target", 14, 1); m_texture = tmp (L1, L5).
    // MISSING: m_texture_view = driver vtable +0x84 (m_texture, 0) (L2).
}

// ---------------------------------------------------------------------------------------------------------------
// 0x7d5404 _ZN15bitmap_info_oglC1EPN6glitch5video12IVideoDriverEiiPh
// PARTIAL: the field setup is written. CTextureManager::createImage(out, mgr, 12, {w, h}) (L5) gives m_image, and the
// alpha copy then writes pixel i as dst[4i] = alpha[i], dst[4i+1..3] = 0xff for i < w*h (L7).
bitmap_info_ogl::bitmap_info_ogl(glitch::video::IVideoDriver* driver, int width, int height, unsigned char* alpha)
    : m_width(width), m_height(height), m_driver(driver) {
    // binary: m_membuf = 0, m_writable = 0, m_texture_registered = 0, texture, view and image = 0, m_min_filter = m_mag_filter = 1.
    // MISSING: m_image = createImage(12, {width, height}) (L1, L5).
    // MISSING, when alpha is set and width * height > 0: bitmap_ogl_detail::expand_alpha8_to_argb(pixels, alpha, width * height),
    // with pixels = m_image's data (L7).
    (void)alpha;
}

// ---------------------------------------------------------------------------------------------------------------
// 0x7d5564 _ZN15bitmap_info_oglC2EPN6glitch5video12IVideoDriverEiiPh
// SKIPPED: the base-object constructor the compiler emits for the same source constructor as 0x7d5404. The class
// has no virtual base, so C2 runs the same code as C1 and the C1 body above is the behaviour.

// ---------------------------------------------------------------------------------------------------------------
// 0x7d567c _ZN15bitmap_info_oglC1EPN6glitch5video12IVideoDriverEPN7gameswf10image_rgbaE
// PARTIAL: the field setup is written. The width and height come from the image (+0x0c, +0x10). The RGBA copy is
// bitmap_ogl_detail::expand_rgba_to_argb(pixels, image->m_data, width * height) (L7), pixels from m_image.
bitmap_info_ogl::bitmap_info_ogl(glitch::video::IVideoDriver* driver, image::rgba* image)
    : m_width(image->m_width), m_height(image->m_height), m_driver(driver) {
    // binary: m_membuf = 0, m_writable = 0, m_texture_registered = 0, texture, view and image = 0, min/mag filter = 1.
    // MISSING: m_image = createImage(12, {m_width, m_height}) (L1, L5).
    // MISSING: expand_rgba_to_argb(pixels, image->m_data, m_width * m_height) when the count is positive (L7).
}

// ---------------------------------------------------------------------------------------------------------------
// 0x7d57e0 _ZN15bitmap_info_oglC2EPN6glitch5video12IVideoDriverEPN7gameswf10image_rgbaE
// SKIPPED: the base-object constructor the compiler emits for the same source constructor as 0x7d567c (no virtual
// base, so C2 runs the same code as C1).

// ---------------------------------------------------------------------------------------------------------------
// 0x7d5914 _ZN15bitmap_info_oglC1EPN6glitch5video12IVideoDriverEPN7gameswf9image_rgbE
// PARTIAL: the field setup is written. The copy is per row: row y comes from image::scanline(image, y) (upstream
// image_base row accessor, pitch-based); each row is bitmap_ogl_detail::expand_rgb_to_argb of width pixels, and the
// destination advances contiguously, width * 4 bytes per row (L7).
bitmap_info_ogl::bitmap_info_ogl(glitch::video::IVideoDriver* driver, image::rgb* image)
    : m_width(image->m_width), m_height(image->m_height), m_driver(driver) {
    // binary: as the 0x7d567c constructor.
    // MISSING: m_image = createImage(12, {m_width, m_height}) (L1, L5).
    // MISSING: for y in [0, height): expand_rgb_to_argb(pixels + y * width * 4, image::scanline(image, y), width) (L7).
}

// ---------------------------------------------------------------------------------------------------------------
// 0x7d5a9c _ZN15bitmap_info_ogl7releaseEv
// PARTIAL: the texture is released in the binary through the texture manager when m_texture_registered is set
// (removeTexture(mgr, &m_texture), which also clears the pointer; L1, L5). That call is MISSING. The rest is written.
void bitmap_info_ogl::release() {
    if (m_texture) {
        if (m_texture_registered) {
            // MISSING: CTextureManager::removeTexture(boost::intrusive_ptr<ITexture>&) on driver +0xe0 (L1).
        }
        m_texture = nullptr;  // binary: the slot is cleared, then the old reference is dropped
    }
    if (m_image) {
        m_image = nullptr;  // binary: the slot is cleared, then the old reference is dropped
    }
    if (m_membuf) {
        m_membuf->~membuf();
        free_internal(m_membuf, 0);
        m_membuf = nullptr;
    }
}

// ---------------------------------------------------------------------------------------------------------------
// 0x7d5b2c _ZN15bitmap_info_ogl11set_textureEPN6glitch5video8ITextureE
// PARTIAL: release(), then the texture pointer is replaced with a counted reference (old reference dropped). The copy of
// the texture size is MISSING: the binary reads texture +0x20 (width) and +0x24 (height) and stores them in m_width and
// m_height, with no null test (L8). The texture's height field is not declared in the tree.
void bitmap_info_ogl::set_texture(glitch::video::ITexture* texture) {
    release();
    m_texture = texture;
    // MISSING: m_width = texture->+0x20; m_height = texture->+0x24 (L8). The binary dereferences the texture here.
}

// ---------------------------------------------------------------------------------------------------------------
// 0x7d5b80 _ZN15bitmap_info_ogl8unlayoutEv
// Thunk: a tail jump to release(). Forwarded, not reimplemented.
void bitmap_info_ogl::unlayout() {
    release();
}

// ---------------------------------------------------------------------------------------------------------------
// 0x7d5b84 _ZN15bitmap_info_oglD1Ev
// PARTIAL: the binary calls release(), drops m_image, m_texture_view and m_texture, then sets the vptr to the
// bitmap_info vtable and runs the ref_counted destructor. The member destructors run in the same order here, after
// release(). release() is partial (L1), so the texture removal is missing.
bitmap_info_ogl::~bitmap_info_ogl() {
    release();
}

// ---------------------------------------------------------------------------------------------------------------
// 0x7d5c04 _ZN15bitmap_info_oglD0Ev
// SKIPPED: the compiler-generated deleting destructor. It runs D1 and then frees the object.

// ---------------------------------------------------------------------------------------------------------------
// 0x7d5c20 _ZN15bitmap_info_ogl6layoutEv
// PARTIAL. The binary's order and branches are written; the parts that need the texture manager or the driver's
// words are MISSING (L1 to L6, L8, L9). Steps:
//   1. return when m_texture is set.
//   2. name = sprintf("bitmap_info_ogl_0x%x", this). The pointer is printed at host width (unsigned long long).
//   3. driver option bit 0x10: when the driver's option word has it, the binary clears it through the driver's
//      vtable +0xa0 (setOption(16, false)) and saves the old bit. MISSING (L3, L4).
//   4. texture manager word +0x74 bit 0x10 is set from m_writable, and the old bit is saved. MISSING (L1, L6).
//   5. when m_image is set: addTexture(out, mgr, name, m_image, bool, layout) (MISSING, L5), m_texture = out.
//      The bool is the image's flag byte at +0x28, and the layout is 1 on the writable path (driver flags 0x9c and
//      PFDTable checks, MISSING L4, L9) or 0 otherwise.
//   6. otherwise, when m_membuf is set: File = createMemoryReadFile(membuf data, membuf size, name, false), then
//      m_texture = mgr->getTexture(File, nullptr, false), then the filter update, then m_texture_registered = true,
//      File dropped, membuf destroyed and freed. MISSING as a whole (L1, L5, L10): the branch is not written.
//   7. filter update on the texture (both paths, min then mag): bitmap_ogl_detail::apply_filter_bits(flags, dirty,
//      mip_count, m_min_filter, m_mag_filter) on the texture words (MISSING, L8).
//   8. the image is released at the end of the image path.
//   9. restore the texture manager bit and the driver's bit saved in steps 3 and 4 (MISSING, L3, L4, L6).
void bitmap_info_ogl::layout() {
    if (m_texture) {
        return;
    }
    char name[64];
    std::snprintf(name, sizeof name, "bitmap_info_ogl_0x%llx",
                  static_cast<unsigned long long>(reinterpret_cast<std::uintptr_t>(this)));

    // MISSING: steps 3 and 4 (L1, L3, L4, L6).

    if (m_image) {
        // MISSING: m_texture = mgr->addTexture(name, m_image, flag, layout) (L5). The writable branch needs L4 and L9.
        m_texture_registered = true;
        // MISSING: step 7 on m_texture (L8).
        m_image = nullptr;
    } else if (m_membuf) {
        // MISSING: File = glitch::io::createMemoryReadFile(m_membuf->data(), m_membuf->size(), name, false) (L10). The
        // header glitch-core-io/io/memory_read_file.hpp does not compile on its own (getFileName and getFullPath return
        // std::string& against IReadFile's const char*). Then m_texture = mgr->getTexture(File, nullptr, false) with
        // mgr = driver +0xe0 (L1, L5), m_texture_registered = true, step 7 (L8), File dropped, and the membuf destroyed
        // and freed (release() does that part, written in release()). The branch is left out here, so the membuf is
        // kept for a later layout() call.
    }

    // MISSING: step 9 (L1, L3, L4, L6).
}

}  // namespace gameswf
