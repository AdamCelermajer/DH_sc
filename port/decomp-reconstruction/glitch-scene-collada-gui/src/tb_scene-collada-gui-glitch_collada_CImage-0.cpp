// Track B batch e1-glitch-scene-collada-gui-glitch_collada_CImage-0: glitch::collada::CImage.
// Layout and declarations are in glitch-scene-collada-gui/tb/glitch_collada_CImage.hpp.

#include "glitch-scene-collada-gui/tb/glitch_collada_CImage.hpp"

namespace glitch::collada {

namespace {

// The original grabs the record (its count at +0x04) only when the record is non-null and its count
// is already non-zero; the count is then incremented.
void grabRecordIfCounted(SColladaRecord* record) noexcept {
    if (record != nullptr && record->reserved00[1] != 0) {
        ++record->reserved00[1];
    }
}

}  // namespace

// 0x60e1d4 _ZN6glitch7collada6CImageC1ERKNS0_16CColladaDatabaseERNS0_6SImageE
// Partial. Reconstructed: the two database words copied to +0x0c and +0x10, the counted grab on the
// record, the name copied from the image (+0x08), the reference count set to 1 (+0x04), and the image
// pointer (+0x18). Not reconstructed, so the row stays partial:
//   1. The vptr store at +0x00 (CImage vtable + 8). The class has no reconstructed virtual methods.
//   2. The image reference. The original grabs SImage +0x10 (count +1) and stores it at +0x14. SImage
//      in collada_database.hpp does not map +0x10 yet. Layout request: add an IReferenceCounted*
//      member at +0x10 to SImage. Until then imageRef stays null.
//   3. The IObject base id placeholder written to +0x08 before the name is not modelled; the name
//      overwrites it in the original too, so only the name survives.
// The original returns `this` (the ARM C1 return); a C++ constructor has no return value.
CImage::CImage(const CColladaDatabase& source, SImage& sourceImage) {
    // +0x0c, +0x10: copy the two words of the database argument.
    database.record = source.record;
    database.factory = source.factory;
    grabRecordIfCounted(database.record);

    // +0x18: the image entry.
    image = &sourceImage;
    // +0x08: the name (first word of the SImage entry).
    name = sourceImage.name;
    // +0x04: the reference count starts at 1.
    referenceCount = 1;
}

}  // namespace glitch::collada
