// glitch::video::CImage: a software image (pixel data with an optional mip chain). Canonical definition (engine
// unification wave u1-video); the stand-in in glitch-core-io (deps/glitch_core_objects.hpp) is replaced by it. The class
// of the same name in glitch-scene-collada-gui (tb/glitch_collada_CImage.hpp) is glitch::collada::CImage, a different class.
//
// Layout of the original object (32-bit; constructors 0x602028 C2 / 0x60209c C1 (format, size, hasMips), 0x6021f8 /
// 0x602434 (format, size, data, mip data, copy, own), 0x602670 .. 0x602938 and the sub-image / convert forms 0x601b10,
// 0x601d38; initData 0x601988; size 0x2c):
//   +0x00  vptr (vtable 0x978248: the destructor pair and onDelete, nothing else)
//   +0x04  reference count; the constructors store 0
//   +0x08  pixel data (uint8_t*)             +0x0c  null-terminated array of the mip level pointers (levels 1..n)
//   +0x10  width     +0x14  height           +0x18  pitch in bytes      +0x1c  data size in bytes
//   +0x20  E_PIXEL_FORMAT                    +0x24  number of mip levels below level 0
//   +0x28  uint8, the image has a mip chain  +0x29  uint8, the image owns its data (the destructor frees it)
#pragma once

#include <cstdint>

#include "glitch-core-io/core/reference_counted.hpp"
#include "glitch-shared/glitch_core.hpp"
#include "glitch-video/deps/intrusive_ptr.hpp"
#include "glitch-video/video_types.hpp"

namespace glitch::video {

class CImage : public glitch::IReferenceCounted {
public:
    // Port helper (not an original function): sets the base size only, for tests of getDimension.
    CImage(uint32_t width, uint32_t height) : m_width(width), m_height(height) {}

    // 0x602028 (C2) / 0x60209c (C1), and the other constructors of the queue: 0x602110 / 0x602184 (format, size),
    // 0x6021f8 / 0x602434 (format, size, data, mip data, copy, own), 0x602670 / 0x60272c (format, size, data, copy, own),
    // 0x6027e8 / 0x602938 (format, size, data, pitch, size, copy, own), 0x601b10 / 0x601c24 (sub-image) and 0x601d38 /
    // 0x601eb0 (format conversion). Declared only. The destructors (0x601804 D1, 0x6018b8 D2, 0x60196c D0) free the data
    // and the mip chain when the image owns them; the port's implicit destructor does not.
    CImage(E_PIXEL_FORMAT format, const core::dimension2d<int32_t>& size, bool hasMipmaps);
    CImage(E_PIXEL_FORMAT format, const core::dimension2d<int32_t>& size);
    CImage(E_PIXEL_FORMAT format, const core::dimension2d<int32_t>& size, void* data, void** mipData, bool copy, bool own);
    CImage(E_PIXEL_FORMAT format, const core::dimension2d<int32_t>& size, void* data, bool copy, bool own);
    CImage(E_PIXEL_FORMAT format, const core::dimension2d<int32_t>& size, void* data, uint32_t pitch, uint32_t dataSize,
           bool copy, bool own);
    CImage(const boost::intrusive_ptr<CImage>& source, const core::position2d<int32_t>& position,
           const core::dimension2d<int32_t>& size);
    CImage(E_PIXEL_FORMAT format, const boost::intrusive_ptr<CImage>& source);

    // 0x601804 (D1) / 0x6018b8 (D2, the same body) / 0x60196c (D0, the deleting form the compiler generates).
    // Releases the pixel block and the mip chain, but only when m_ownsData is set.
    ~CImage() override;

    // 0x601988: computes the pitch and size, allocates the data and (with a mip flag) the mip chain. Declared only.
    void initData(bool allocateMips);

    // 0x5ff638: dimension of the given mip level; zero-sized axes stay zero.
    core::dimension2d<uint32_t> getDimension(uint32_t mipLevel) const;

    // 0x5ff680 / 0x5ff808 / 0x5ff9b4: single pixel write and read, and a fill. Declared only (they dispatch on the
    // pixel format through the PFDTable converters).
    void setPixel(uint32_t x, uint32_t y, const SColor& color);
    SColor getPixel(uint32_t x, uint32_t y) const;
    void fill(const SColor& color);
    // 0x600104 / 0x600e1c: Bresenham line and rectangle outline. Declared only.
    void drawLine(const core::position2d<int32_t>& from, const core::position2d<int32_t>& to, const SColor& color);
    void drawRectangle(const core::rect<int32_t>& rect, const SColor& color);
    // 0x600708 / 0x60090c / 0x600ed0: blits to another image (with a clip rect, an alpha colour). Declared only.
    void copyTo(const boost::intrusive_ptr<CImage>& target, const core::position2d<int32_t>& position,
                const core::rect<int32_t>& source, const core::rect<int32_t>* clip, uint32_t flags);
    void copyTo(const boost::intrusive_ptr<CImage>& target, const core::position2d<int32_t>& position, uint32_t flags);
    void copyToWithAlpha(const boost::intrusive_ptr<CImage>& target, const core::position2d<int32_t>& position,
                         const core::rect<int32_t>& source, const SColor& color, const core::rect<int32_t>* clip);
    // 0x600fcc / 0x60177c / 0x601800: scaling blits. Declared only.
    void copyToScaling(void* target, uint32_t width, uint32_t height, E_PIXEL_FORMAT format, uint32_t pitch, int32_t flags);
    void copyToScaling(const boost::intrusive_ptr<CImage>& target, int32_t flags);
    void copyToScalingBoxFilter(const boost::intrusive_ptr<CImage>& target, int32_t flags);

    // +0x04 is IReferenceCounted::referenceCounter.
    uint8_t* m_data = nullptr;          // +0x08
    void** m_mipData = nullptr;         // +0x0c
    uint32_t m_width = 0;               // +0x10
    uint32_t m_height = 0;              // +0x14
    uint32_t m_pitch = 0;               // +0x18
    uint32_t m_dataSize = 0;            // +0x1c
    E_PIXEL_FORMAT m_format = 0;        // +0x20
    uint32_t m_mipLevels = 0;           // +0x24
    uint8_t m_hasMipmaps = 0;           // +0x28
    uint8_t m_ownsData = 1;             // +0x29
};

}  // namespace glitch::video
