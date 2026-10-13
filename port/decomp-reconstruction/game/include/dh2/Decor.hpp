// Decor: static scenery. The visual's mesh box becomes the object's relative AABB, and a visual with a collision
// marker (VisualObject m_hasColBox) also gets a PODecor physical object. The "is_solid" property is copied into a
// flag bit of the room that holds the visual's floor map. LoadFloorMap runs once, from InitPost.
// Original 32-bit size 0x378 (888): GameObject (0x374) plus four bytes.
// Reconstructed from libDungeonHunter2.so pseudocode (ctor 0x3410fc is inlined into GetNewInstance<Decor>).
#pragma once

#include <cstdint>

#include "dh2/GameObject.hpp"

class Decor : public GameObject {
public:
    // No standalone constructor symbol: GetNewInstance<Decor> (0x3410fc) runs GameObject(id), sets m_static (+0x84),
    // sets m_floorMapPending and m_isSolid to 1 and stores the primary and secondary vtables.
    explicit Decor(GO_IDS id);

    ~Decor() override;                                                  // slot 0/1: 0x388428 (D1), 0x388eb0 (D0)

    // Overrides of GameObject slots, in vtable order.
    void DeclareProperties() override;                                  // slot 0x18: 0x3899d0 adds "is_solid", default true
    void InitPost() override;                                           // slot 0x1c: 0x388a98 applies the mesh box, optional PODecor, then LoadFloorMap
    bool IsUpdatable() const override;                                  // slot 0x38: 0x3883d4 false

    // Loads the room of the visual's scene node (once, while m_floorMapPending is set), copies m_isSolid into bit 0 of
    // the room's flags, extends the room's bounding box with the owner's absolute AABB, then clears m_floorMapPending.
    void LoadFloorMap();                                                // 0x388730

    // GameObject occupies +0x00 .. +0x373.
    std::uint8_t pad_374[1];                                            // +0x374 not touched by the constructor or the methods above
    bool m_floorMapPending;                                             // +0x375 1 after construction, 0 once LoadFloorMap has run
    bool m_isSolid;                                                     // +0x376 property "is_solid", default true (property offset 882 is relative to the PropertyMap at +4)
    std::uint8_t pad_377[1];                                            // +0x377
};
