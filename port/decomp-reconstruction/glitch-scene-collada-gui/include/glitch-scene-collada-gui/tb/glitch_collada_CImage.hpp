// glitch::collada::CImage, declared for Track B batch e1-glitch-scene-collada-gui-glitch_collada_CImage-0.
// No other header in this category declares the class under this name. recon/slice_11.hpp has
// CImageStandIn (destructor-only stand-in) and wave_10.hpp notes the class is not mapped in
// collada_db.hpp. Offsets are the original 32-bit layout, kept as comments only; the code does no
// pointer arithmetic on them.
#pragma once

#include <cstdint>

#include "glitch-scene-collada-gui/collada_database.hpp"

namespace glitch {
class IReferenceCounted;
}

namespace glitch::collada {

// Image object built from an SImage entry of a CColladaDatabase. The original derives from IObject
// (which derives from the reference-counted base); the base parts are kept as plain members.
class CImage {
public:
    // 0x60e1d4 _ZN6glitch7collada6CImageC1ERKNS0_16CColladaDatabaseERNS0_6SImageE
    // Partial: see the note on the definition in src/tb_scene-collada-gui-glitch_collada_CImage-0.cpp.
    CImage(const CColladaDatabase& source, SImage& sourceImage);

    // +0x00 vptr: the CImage vtable (_ZTVN6glitch7collada6CImageE + 8) is not reconstructed, so no
    // vptr is stored here.
    // +0x04 reference count (IReferenceCounted base, starts at 1).
    std::int32_t referenceCount = 0;
    // +0x08 name (IObject id slot); the image name, the first word of SImage.
    const char* name = nullptr;
    // +0x0c database handle: record (+0x0c) and factory (+0x10) words of the CColladaDatabase argument.
    CColladaDatabase database{};
    // +0x14 image reference (IReferenceCounted*), from SImage +0x10. SImage does not map +0x10 yet,
    // so this stays null (see the partial note on the constructor).
    glitch::IReferenceCounted* imageRef = nullptr;
    // +0x18 the SImage entry this object was created from.
    SImage* image = nullptr;
};

}  // namespace glitch::collada
