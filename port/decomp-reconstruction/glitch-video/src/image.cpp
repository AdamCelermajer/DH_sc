#include "glitch-video/image.hpp"

#include <cstring>

#include "glitch-video/pixel_format.hpp"
#include "glitch-video/pixel_format_convert.hpp"

namespace glitch::video {

namespace {

// Halves a mip dimension, clamping at 1. Non-positive sizes are returned
// unchanged (the original tests the signed value before shifting).
uint32_t mipDimension(uint32_t size, uint32_t mipLevel) {
    if (static_cast<int32_t>(size) <= 0) {
        return size;
    }
    uint32_t shifted = size >> mipLevel;
    return shifted == 0 ? 1u : shifted;
}

// The "adopt the caller's block" paths of the data-taking constructors store 0x0badf00d into m_data before calling
// initData (0x60270c, 0x6023a4, 0x602868 on the ARM listing). initData allocates the pixel block only when that
// pointer is null, so the stored value is what makes it skip the allocation and leave the caller's block in place;
// the pointer is never dereferenced before it is overwritten (m_hasMipmaps is clear on those paths, which is the
// only thing initData tests the level-array pointer for).
uint8_t* const kAdoptedDataSentinel = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(0x0badf00d));

}  // namespace

// 0x601804 _ZN6glitch5video6CImageD1Ev (D2 0x6018b8 is the same body; D0 0x60196c is the compiler's deleting form)
// Releases the pixel block and then the mip chain, both only when m_ownsData (+0x29) is set, and nulls each level
// pointer as it goes. Read off the binary: the block is freed first, then, when m_hasMipmaps (+0x28) is set and the
// level array exists, every level is freed until a null slot, and the array itself last.
//
// Deviation, deliberate: the original walks the level array with a do/while that stops at a null slot but does not
// bound the index, which only terminates because the array is fully initialised when it is built. This port stops
// at m_mipLevels as well, so a partially built chain cannot run the walk off the end.
//
// The m_ownsData guard really does cover the level array as well as the block (the LDRB at 0x601810 is followed by
// BNE 0x6018a8, which skips the array), and that guard has to stay: on the adopt path of 0x6021f8 the array is the
// caller's, so freeing it unconditionally would free memory the image never allocated. The price is that an image
// built through 0x6027e8 with own clear and a non-zero level count leaks the pointer array this port's own allocator
// made for it (and m_data too when that form also allocated it, i.e. its first bool clear), because nothing in the
// object records who allocated the array. Closing that would mean inventing a member the original does not have, so
// the leak is kept and named here and in the ledger row of 0x601804.
CImage::~CImage() {
    if (m_ownsData == 0) {
        return;
    }
    delete[] m_data;
    m_data = nullptr;
    if (m_mipData != nullptr) {
        if (m_hasMipmaps != 0) {
            for (uint32_t level = 0; level < m_mipLevels; ++level) {
                delete[] static_cast<uint8_t*>(m_mipData[level]);
                m_mipData[level] = nullptr;
            }
        }
        delete[] m_mipData;
        m_mipData = nullptr;
    }
}

// 0x601988 _ZN6glitch5video6CImage8initDataEb
// Computes the pitch and the data size, allocates the pixel data when there is none, and (with allocateMips)
// builds the mip chain: one allocation per level, with the level pointer array terminated by a null entry.
//
// Read field by field off the binary: the pitch is computePitch(m_format, m_width) (the two words it passes are
// +0x20 and +0x10), the size is pitch * m_height unless one was already supplied, and the data block is only
// allocated when m_data is still null - which is what lets a caller install its own buffer first and have
// initData skip the allocation.
//
// The level count is the number of halvings of (width, height) down to 1, and the array is sized 4 * (levels+1)
// bytes; the original picks 4 for a 1x1 image. Each level is allocated as height * bytesPerBlock * width (the
// PFDTable byte at +0x15, not the pitch) and zeroed with a byte that advances by 15 per level; that fill byte is
// cosmetic and the port zeroes instead.
void CImage::initData(bool allocateMips) {
    const uint32_t pitch = pixel_format::computePitch(m_format, m_width);
    m_pitch = pitch;
    if (m_dataSize == 0) {
        m_dataSize = pitch * m_height;
    }
    if (m_data == nullptr) {
        m_data = new uint8_t[m_dataSize];
    }
    if (!allocateMips || m_hasMipmaps == 0 || m_mipData != nullptr) {
        return;
    }

    uint32_t width = m_width;
    uint32_t height = m_height;
    const bool singleLevel = width == 1 && height == 1;
    if (!singleLevel) {
        uint32_t count = 1;
        uint32_t probeW = width;
        uint32_t probeH = height;
        while (true) {
            if (probeW > 1) probeW >>= 1;
            if (probeH > 1) probeH >>= 1;
            ++count;
            if (probeW == 1 && probeH == 1) break;
        }
        m_mipLevels = count - 1;
    }
    const uint32_t slots = singleLevel ? 1 : m_mipLevels + 1;
    m_mipData = new void*[slots];

    if (singleLevel) {
        m_mipData[0] = nullptr;
        return;
    }
    uint32_t index = 0;
    while (true) {
        if (width > 1) width >>= 1;
        if (height > 1) height >>= 1;
        const uint32_t bytes = height * static_cast<uint32_t>(pixel_format::detail::PFDTable[m_format].bytesPerBlock) * width;
        uint8_t* level = new uint8_t[bytes];
        std::memset(level, 0, bytes);
        m_mipData[index] = level;
        ++index;
        if (width == 1 && height == 1) break;
    }
    m_mipData[index] = nullptr;
}

// 0x601b10 _ZN6glitch5video6CImageC1ERKN5boost13intrusive_ptrIS1_EERKNS_4core10position2dIiEERKNS7_11dimension2dIiEE
// (C2 0x601c24 has the same body.) The sub-image form. Every field is cleared first and the format starts at 39, the
// table's "no row matched" value; m_ownsData is 1. With a source image the format, the requested size and the source's
// mip flag are taken over, initData allocates (and builds a level chain when the source has one), and then
// pixel_format::convert copies the sub-rectangle out of the source.
//
// Read off the listing: the call's source pointer is source->m_data + position.Y * source->m_pitch +
// position.X * PFDTable[m_format].bytesPerBlock (the table byte at +0x15), where m_format is the source's own format,
// and both the source and the destination format arguments are that same m_format. The call's result is ignored.
//
// PARTIAL: the pixel transfer is not reproduced. The call takes pixel_format::convert's srcFormat == dstFormat
// "copy()" branch, and that branch is not reconstructed in glitch-video/src/pixel_format_convert.cpp (the port's
// convert returns 0 there and writes nothing), so the destination block is left as initData allocated it. Every other
// part of this constructor - the reset values, the format/size/mip-flag takeover, the allocation, the source offset
// and the call's arguments - is the listing's.
CImage::CImage(const boost::intrusive_ptr<CImage>& source, const core::position2d<int32_t>& position,
               const core::dimension2d<int32_t>& size) {
    referenceCounter = 0;
    m_format = 39;
    m_data = nullptr;
    m_mipData = nullptr;
    m_width = 0;
    m_height = 0;
    m_pitch = 0;
    m_dataSize = 0;
    m_mipLevels = 0;
    m_hasMipmaps = 0;
    m_ownsData = 1;

    CImage* const sourceImage = source.get();
    if (sourceImage == nullptr) {
        return;
    }
    m_format = sourceImage->m_format;
    m_width = static_cast<uint32_t>(size.Width);
    m_height = static_cast<uint32_t>(size.Height);
    m_hasMipmaps = sourceImage->m_hasMipmaps;
    initData(true);

    const uint32_t sourceOffset =
        static_cast<uint32_t>(position.Y) * sourceImage->m_pitch +
        static_cast<uint32_t>(position.X) *
            static_cast<uint32_t>(pixel_format::detail::PFDTable[m_format].bytesPerBlock);
    pixel_format::convert(m_format, sourceImage->m_data + sourceOffset, sourceImage->m_pitch, m_format, m_data, m_pitch,
                          m_width, m_height, false);
}

// 0x601d38 _ZN6glitch5video6CImageC1ENS0_14E_PIXEL_FORMATERKN5boost13intrusive_ptrIS1_EE
// (C2 0x601eb0 has the same body.) The format-conversion form: the requested format is stored first, the rest of the
// fields are cleared (m_ownsData 1), and with a source image the source's width, height and mip flag are taken over,
// initData allocates (and builds a level chain when the source has one), and pixel_format::convert runs the source
// format into the requested format over the whole image.
//
// The failure path at 0x601dfc (convert returned 0) is reproduced as the listing has it: the format becomes 39, the
// pixel block is freed and cleared, the level chain is freed level by level until a null slot and then the level
// array itself, and the image is left empty - dataSize 0, ownsData 0, height 0, width 0. Two of the original's
// details are kept: the level-array pointer at +0x0c is NOT cleared after the free (the destructor then skips it
// because ownsData is 0), and m_mipLevels and m_hasMipmaps keep their values.
//
// Which format pairs succeed is pixel_format::convert's business, not this constructor's; the port's convert models a
// subset of the original's arms (its own ledger row), so a pair it refuses takes this failure path here just as an
// unsupported pair does in the original. The constructor's own body is the listing's.
CImage::CImage(E_PIXEL_FORMAT format, const boost::intrusive_ptr<CImage>& source) {
    referenceCounter = 0;
    m_format = format;
    m_data = nullptr;
    m_mipData = nullptr;
    m_width = 0;
    m_height = 0;
    m_pitch = 0;
    m_dataSize = 0;
    m_mipLevels = 0;
    m_hasMipmaps = 0;
    m_ownsData = 1;

    CImage* const sourceImage = source.get();
    if (sourceImage == nullptr) {
        return;
    }
    m_width = sourceImage->m_width;
    m_height = sourceImage->m_height;
    m_hasMipmaps = sourceImage->m_hasMipmaps;
    initData(true);

    if (pixel_format::convert(sourceImage->m_format, sourceImage->m_data, sourceImage->m_pitch, m_format, m_data,
                              m_pitch, m_width, m_height, false) != 0) {
        return;
    }
    m_format = 39;
    delete[] m_data;
    m_data = nullptr;
    if (m_mipData != nullptr) {
        if (m_hasMipmaps != 0) {
            for (uint32_t level = 0; m_mipData[level] != nullptr; ++level) {
                delete[] static_cast<uint8_t*>(m_mipData[level]);
                m_mipData[level] = nullptr;
            }
        }
        delete[] m_mipData;
    }
    m_dataSize = 0;
    m_ownsData = 0;
    m_height = 0;
    m_width = 0;
}

// 0x602028 _ZN6glitch5video6CImageC1ENS0_14E_PIXEL_FORMATERKNS_4core11dimension2dIiEEb
// (format, size, hasMipmaps): the size and format are stored, m_hasMipmaps takes the argument, m_ownsData is 1
// and initData allocates. The reference count is stored as 0, not left at the base's default of 1.
CImage::CImage(E_PIXEL_FORMAT format, const core::dimension2d<int32_t>& size, bool hasMipmaps) {
    referenceCounter = 0;
    m_data = nullptr;
    m_mipData = nullptr;
    m_width = static_cast<uint32_t>(size.Width);
    m_height = static_cast<uint32_t>(size.Height);
    m_pitch = 0;
    m_dataSize = 0;
    m_format = format;
    m_mipLevels = 0;
    m_hasMipmaps = hasMipmaps ? 1 : 0;
    m_ownsData = 1;
    initData(true);
}

// 0x602110 _ZN6glitch5video6CImageC1ENS0_14E_PIXEL_FORMATERKNS_4core11dimension2dIiEE
// As the hasMipmaps form with the flag clear, so no mip chain is built.
CImage::CImage(E_PIXEL_FORMAT format, const core::dimension2d<int32_t>& size) {
    referenceCounter = 0;
    m_data = nullptr;
    m_mipData = nullptr;
    m_width = static_cast<uint32_t>(size.Width);
    m_height = static_cast<uint32_t>(size.Height);
    m_pitch = 0;
    m_dataSize = 0;
    m_format = format;
    m_mipLevels = 0;
    m_hasMipmaps = 0;
    m_ownsData = 1;
    initData(true);
}

// 0x6021f8 _ZN6glitch5video6CImageC1ENS0_14E_PIXEL_FORMATERKNS_4core11dimension2dIiEEPvPS8_bb
// (C2 0x602434 has the same body.) The (format, size, data, mip data, copy, own) form.
//
// Read field by field off the listing: size -> +0x10/+0x14, format -> +0x20, m_hasMipmaps (+0x28) clears, the FIRST
// bool is the one tested at 0x60226c and the SECOND is stored into m_ownsData (+0x29). First bool set is the adopt
// path: +0x08 and +0x0c are set to 0x0badf00d, initData runs (and, since its data pointer is not null, allocates
// nothing), the caller's two pointers are stored over them, the level count restarts at 0, and the caller's level
// array is walked to recover the count (0x6023d0 - 0x602400). m_hasMipmaps is set only when that walk reaches a null
// entry with width and height both 1, so a short caller chain leaves it clear.
//
// First bool clear is the copy path. With mip data present m_hasMipmaps is set BEFORE initData, so initData allocates
// the block *and* a level chain; level 0 is copied in with the pitch * height byte count, then every caller level is
// copied into the level initData allocated for it, with the per-level size (height * PFDTable bitsPerPixel(+0x16) *
// width) >> 3 over the halved dimensions (0x6022e4 - 0x602338). Without mip data only the block is allocated and
// filled.
//
// Two things in the listing are not reproduced; both are unreachable, and they are named here rather than hidden:
//   - the cleanup at 0x60234c, which frees the levels initData built but the copy loop did not fill, is dead code. It
//     is reached only by falling out of the copy loop at 0x602338, and on that path m_mipLevels was cleared to 0
//     (0x6022ac) while the loop copied at least one level, so the BLS at 0x602348 always returns. (Its free loop at
//     0x60236c is an unconditional branch back to itself, so the original would spin in place there.) The port stores
//     the copied count and omits the dead block.
//   - neither the counting walk nor the copy loop bounds itself, so a caller chain shorter than the image's chain
//     makes the original read the next slot - which is not a level pointer - and memcpy from it. The port keeps that
//     shape: no caller in the original passes such a chain, and this constructor has no call sites at all (its only
//     cross-reference is the C2/C1 table entry at 0x7e258).
CImage::CImage(E_PIXEL_FORMAT format, const core::dimension2d<int32_t>& size, void* data, void** mipData, bool copy,
               bool own) {
    referenceCounter = 0;
    m_data = nullptr;
    m_mipData = nullptr;
    m_width = static_cast<uint32_t>(size.Width);
    m_height = static_cast<uint32_t>(size.Height);
    m_ownsData = own ? 1 : 0;
    m_pitch = 0;
    m_dataSize = 0;
    m_format = format;
    m_mipLevels = 0;
    m_hasMipmaps = 0;

    if (copy) {
        // Adopt the caller's block and level array. The sentinel is the original's 0x0badf00d.
        m_mipData = reinterpret_cast<void**>(kAdoptedDataSentinel);
        m_data = kAdoptedDataSentinel;
        initData(true);
        m_data = static_cast<uint8_t*>(data);
        m_mipData = mipData;
        m_mipLevels = 0;
        if (mipData == nullptr) {
            return;
        }
        uint32_t width = m_width;
        uint32_t height = m_height;
        uint32_t index = 0;
        while (true) {
            void* const entry = mipData[index];
            if (entry == nullptr && width == 1) {
                if (height == 1) {
                    m_hasMipmaps = 1;
                    return;
                }
            } else if (width > 1) {
                width >>= 1;
            }
            if (height > 1) {
                height >>= 1;
            }
            ++index;
            ++m_mipLevels;
        }
    }

    if (mipData == nullptr) {
        initData(true);
        std::memcpy(m_data, data, m_pitch * m_height);
        return;
    }

    m_hasMipmaps = 1;
    initData(true);
    std::memcpy(m_data, data, m_pitch * m_height);

    const uint32_t bitsPerPixel = static_cast<uint32_t>(pixel_format::detail::PFDTable[m_format].bitsPerPixel);
    uint32_t width = m_width;
    uint32_t height = m_height;
    uint32_t index = 0;
    uint32_t copied = 0;
    const uint8_t* level = static_cast<const uint8_t*>(mipData[0]);
    // One register in the listing (r3/v19) gates both the width halving and the loop itself: it is true while the
    // walk has not reached a null entry with the width already down to 1.
    bool keepGoing = !(level == nullptr && width == 1);
    while (true) {
        if (keepGoing && width > 1) {
            width >>= 1;
        }
        if (height > 1) {
            height >>= 1;
        }
        ++copied;
        std::memcpy(m_mipData[index], level, (height * (bitsPerPixel * width)) >> 3);
        ++index;
        level = static_cast<const uint8_t*>(mipData[index]);
        keepGoing = !(level == nullptr && width == 1);
        if (keepGoing) {
            continue;
        }
        if (height == 1) {
            break;
        }
    }
    // The listing clears this counter after initData (0x6022ac) and stores the copied count here (0x602340).
    m_mipLevels = copied;
}

// 0x602670 _ZN6glitch5video6CImageC1ENS0_14E_PIXEL_FORMATERKNS_4core11dimension2dIiEEPvbb
// (C2 0x60272c has the same body.) The (format, size, data, copy, own) form: size -> +0x10/+0x14, format -> +0x20,
// m_hasMipmaps (+0x28), m_pitch (+0x18), m_dataSize (+0x1c) and m_mipLevels (+0x24) all clear, and m_ownsData (+0x29)
// takes the SECOND bool. There is no level array in this form, so m_mipData stays null.
//
// CORRECTION to this row's earlier note: the branch at 0x6026d8 tests the FIRST bool (LDRB R6,[SP,#arg_0]; CMP R6,#0;
// BNE 0x602704), not m_ownsData, and the two paths are the opposite way round from what that note claimed. The first
// bool SET stores 0x0badf00d into +0x08 (0x60270c) - which is what makes initData, whose allocation test is "only
// when the data pointer is null", leave the buffer alone - and then stores the caller's pointer over it (0x602718).
// The first bool CLEAR lets initData allocate and memcpy's pitch * height bytes of the caller's block into it
// (0x6026f8). So that flag means "adopt the caller's block", and m_ownsData alone decides which side frees it. The
// earlier port branched on own instead, reproducing neither path; the JPG loader (0x604e6c) passes adopt 1 with own 1
// for the block it allocated for the decode, which is the case this form exists for.
CImage::CImage(E_PIXEL_FORMAT format, const core::dimension2d<int32_t>& size, void* data, bool copy, bool own) {
    referenceCounter = 0;
    m_data = nullptr;
    m_mipData = nullptr;
    m_width = static_cast<uint32_t>(size.Width);
    m_height = static_cast<uint32_t>(size.Height);
    m_ownsData = own ? 1 : 0;
    m_format = format;
    m_hasMipmaps = 0;
    m_pitch = 0;
    m_dataSize = 0;
    m_mipLevels = 0;

    if (copy) {
        m_data = kAdoptedDataSentinel;
        initData(true);
        m_data = static_cast<uint8_t*>(data);
        return;
    }
    initData(true);
    // The original memcpy's without testing the argument; the port keeps the null guard it already had.
    if (data != nullptr && m_data != nullptr && m_dataSize != 0) {
        std::memcpy(m_data, data, m_pitch * m_height);
    }
}

// 0x6027e8 _ZN6glitch5video6CImageC1ENS0_14E_PIXEL_FORMATERKNS_4core11dimension2dIiEEPvjjbb
// (C2 0x602938 has the same body.) The (format, size, data, size in bytes, mip level count, copy, own) form: the
// fourth argument after the data pointer is stored at +0x1c (the data size) and the fifth at +0x24 (the number of mip
// levels below level 0), which is what the DDS, ATC and PVR loaders pass. Read off the listing:
//
//   * m_ownsData (+0x29) takes the second bool, m_hasMipmaps (+0x28) is cleared, and +0x18 (the pitch) is NOT stored
//     here - both paths call initData, which computes it.
//   * first bool set adopts the caller's single buffer (the 0x0badf00d store at 0x602868 makes initData skip its
//     allocation, then the pointer is stored over it); first bool clear makes initData allocate dataSize bytes and
//     memcpy's exactly that many bytes from the caller's buffer (0x6028a4).
//   * with a non-zero level count the constructor then allocates its own pointer array, new void*[levels + 1]
//     (0x6028b4), and fills it with pointers into the single buffer: the entry for level i + 1 is the buffer plus the
//     sum of computeMipmapSizeInBytes(format, width, height, 0 .. i), so each entry sits one level past the previous
//     one, and a null terminates the array (0x6028e8 - 0x602928).
//   * m_hasMipmaps is NOT set on this path, and that is the point of the level array: the levels live inside m_data,
//     so the destructor must free the block once and the array once, never the levels individually. Whether it frees
//     either is m_ownsData's business, as before.
CImage::CImage(E_PIXEL_FORMAT format, const core::dimension2d<int32_t>& size, void* data, uint32_t dataSize,
               uint32_t mipLevels, bool copy, bool own) {
    referenceCounter = 0;
    m_data = nullptr;
    m_mipData = nullptr;
    m_width = static_cast<uint32_t>(size.Width);
    m_height = static_cast<uint32_t>(size.Height);
    m_mipLevels = mipLevels;
    m_ownsData = own ? 1 : 0;
    m_dataSize = dataSize;
    m_format = format;
    m_hasMipmaps = 0;

    uint32_t levels;
    if (copy) {
        m_data = kAdoptedDataSentinel;
        initData(false);
        levels = m_mipLevels;
        m_data = static_cast<uint8_t*>(data);
    } else {
        initData(false);
        std::memcpy(m_data, data, dataSize);
        levels = m_mipLevels;
    }
    if (levels == 0) {
        return;
    }

    m_mipData = new void*[levels + 1];
    uint8_t* level = m_data;
    uint32_t index = 0;
    do {
        level += pixel_format::computeMipmapSizeInBytes(m_format, m_width, m_height, static_cast<uint8_t>(index), false);
        m_mipData[index] = level;
        ++index;
    } while (m_mipLevels > index);
    m_mipData[index] = nullptr;
}

// 0x5ff638 _ZNK6glitch5video6CImage12getDimensionEj
core::dimension2d<uint32_t> CImage::getDimension(uint32_t mipLevel) const {
    core::dimension2d<uint32_t> result;
    result.Width = mipDimension(m_width, mipLevel);
    result.Height = mipDimension(m_height, mipLevel);
    return result;
}

// 0x5ff808 _ZNK6glitch5video6CImage8getPixelEjj
// Outside the image, and for a format other than 2, 5, 8, 10, 12 and 13, the answer is 0. The data is read row by row (width is the
// row length in pixels). Format 2 (8-bit): the byte becomes the top byte of the colour. Formats 5 (16-bit 5-6-5) and 8 (1-5-5-5) widen
// the channels by replicating the high bits; format 10 (24-bit) reads three bytes and sets the top byte to 255; format 12 (32-bit) rotates
// the word by one byte; format 13 (32-bit) swaps its first and third bytes and keeps the others.
SColor CImage::getPixel(uint32_t x, uint32_t y) const {
    SColor result;
    if (m_width <= x || m_height <= y) {
        return result;
    }
    uint32_t b0 = 0;  // v6 (colour byte 0)
    uint32_t b1 = 0;  // v5
    uint32_t b2 = 0;  // v4
    uint32_t b3 = 0;  // a3
    switch (m_format) {
        case 2:
            b3 = m_data[m_width * y + x];
            break;
        case 5:
        case 8: {
            uint32_t value = *reinterpret_cast<const uint16_t*>(m_data + 2 * (x + m_width * y));
            uint32_t low;
            uint32_t green;
            if (m_format == 5) {
                low = (static_cast<uint8_t>((value >> 13) | (static_cast<uint16_t>(value & 0xF800u) >> 8)) << 8) | 0xFFu;
                green = (static_cast<int32_t>(value & 0x7E0u) >> 2) | (static_cast<uint16_t>(value & 0x7E0u) >> 8);
            } else {
                const uint32_t alpha = (value & 0x8000u) != 0 ? 0xFFu : 0u;
                low = alpha | (((static_cast<int32_t>(value & 0x7C00u) >> 7) | (static_cast<uint16_t>(value & 0x7C00u) >> 12)) << 8);
                green = (static_cast<int32_t>(value & 0x3E0u) >> 2) | (static_cast<uint16_t>(value & 0x3E0u) >> 8);
            }
            const uint32_t packed = (low & 0xFFFFu) | ((static_cast<uint8_t>(green) << 16) & 0xFFFFFFu) |
                                    ((((value >> 2) & 7u) | static_cast<uint8_t>(8u * value)) << 24);
            b2 = packed >> 24;
            b3 = packed & 0xFFu;
            b0 = (packed >> 8) & 0xFFu;
            b1 = (packed >> 16) & 0xFFu;
            break;
        }
        case 10: {
            const uint8_t* pixel = m_data + 3 * (x + m_width * y);
            b3 = 0xFF;
            b0 = pixel[0];
            b1 = pixel[1];
            b2 = pixel[2];
            break;
        }
        case 12: {
            const uint32_t value = *reinterpret_cast<const uint32_t*>(m_data + 4 * (x + m_width * y));
            b2 = value >> 24;
            b3 = value & 0xFFu;
            b0 = (value >> 8) & 0xFFu;
            b1 = (value >> 16) & 0xFFu;
            break;
        }
        case 13: {
            const uint32_t value = *reinterpret_cast<const uint32_t*>(m_data + 4 * (x + m_width * y));
            b2 = value & 0xFFu;
            b3 = value >> 24;
            b0 = (value >> 16) & 0xFFu;
            b1 = (value >> 8) & 0xFFu;
            break;
        }
        default:
            break;
    }
    result.color = (b0 & 0xFFu) | ((b1 & 0xFFu) << 8) | (((b2 & 0xFFu) << 16) & 0xFFFFFFu) | ((b3 & 0xFFu) << 24);
    return result;
}

// 0x5ff9b4 _ZN6glitch5video6CImage4fillERKNS_6SColorE
// Packs the colour for the format and writes the resulting word over the whole data block, eight words at a
// time and then the remaining (m_dataSize >> 2) & 7 words - the loop shape the original has (it walks the block
// as int* and advances by 8 words per pass). A format outside 5, 8, 12, 13 and 14 writes nothing.
//
// The two 16-bit cases replicate their packed halfword into both halves of the word: R5G6B5 (PFDTable row 5,
// masks 0xf800/0x07e0/0x001f, shifts 11/5/0) for 5, and A1R5G5B5 (row 8, 0x7c00/0x03e0/0x001f plus 0x8000,
// shifts 10/5/0/15) for 8. The three 32-bit cases feed four 8-bit channels in the byte order each row stores;
// the names are the row masks, so 12 is 0x0000ff00/0x00ff0000/0xff000000/0x000000ff.
void CImage::fill(const SColor& color) {
    const uint8_t c0 = color.component(0);
    const uint8_t c1 = color.component(1);
    const uint8_t c2 = color.component(2);
    const uint8_t c3 = color.component(3);

    uint32_t packed = 0;
    switch (m_format) {
        case 5: {  // R5G6B5: halfword (R5 << 11) | (G6 << 5) | B5, replicated
            const uint32_t half = (static_cast<uint32_t>(c0 >> 3) << 11) |
                                  (static_cast<uint32_t>(c1 >> 2) << 5) |
                                  static_cast<uint32_t>(c2 >> 3);
            packed = half | (half << 16);
            break;
        }
        case 8: {  // A1R5G5B5: halfword (A1 << 15) | (R5 << 10) | (G5 << 5) | B5, replicated
            const uint32_t half = (static_cast<uint32_t>(c3 >> 7) << 15) |
                                  (static_cast<uint32_t>(c0 >> 3) << 10) |
                                  (static_cast<uint32_t>(c1 >> 3) << 5) |
                                  static_cast<uint32_t>(c2 >> 3);
            packed = half | (half << 16);
            break;
        }
        case 12:  // byte order r, g, b, a
            packed = static_cast<uint32_t>(c3) | (static_cast<uint32_t>(c0) << 8) |
                     (static_cast<uint32_t>(c1) << 16) | (static_cast<uint32_t>(c2) << 24);
            break;
        case 13:  // byte order a, r, g, b
            packed = static_cast<uint32_t>(c2) | (static_cast<uint32_t>(c3) << 8) |
                     (static_cast<uint32_t>(c0) << 16) | (static_cast<uint32_t>(c1) << 24);
            break;
        case 14:  // byte order g, b, a, r
            packed = static_cast<uint32_t>(c1) | (static_cast<uint32_t>(c2) << 8) |
                     (static_cast<uint32_t>(c3) << 16) | (static_cast<uint32_t>(c0) << 24);
            break;
        default:
            return;
    }

    uint32_t* words = reinterpret_cast<uint32_t*>(m_data);
    const uint32_t wholeBlocks = m_dataSize >> 5;  // (m_dataSize >> 2) / 8
    for (uint32_t block = 0; block < wholeBlocks; ++block) {
        for (int i = 0; i < 8; ++i) {
            words[i] = packed;
        }
        words += 8;
    }
    const uint32_t tail = (m_dataSize >> 2) & 7u;
    for (uint32_t i = 0; i < tail; ++i) {
        words[i] = packed;
    }
}

}  // namespace glitch::video
