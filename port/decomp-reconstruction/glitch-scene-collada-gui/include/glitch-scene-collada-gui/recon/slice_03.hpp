// Slice 03: glitch::gui spin box, sprite bank, static text, tab control and table
// reconstruction. Types here are stand-ins named with the recon_slice_03 prefix
// (namespace glitch::collada::recon_03) so this slice does not collide with the
// existing gui headers. Offsets are kept as comments; code uses typed members only.
#pragma once

#include <cstddef>
#include <string>
#include <vector>

namespace glitch::collada::recon_03 {

// Stand-in for glitch::IReferenceCounted. The count is at +0x04 in the original.
class ReferenceCounted {
public:
    virtual ~ReferenceCounted() = default;
    int refCount = 1;  // +0x04
};

// Stand-in for glitch::video::ITexture.
class Texture : public ReferenceCounted {};

// Reference helpers: grab is ++refCount; drop decrements and deletes at zero.
void grab(ReferenceCounted* object);
void drop(ReferenceCounted* object);

// Stand-in for glitch::gui::IGUIEnvironment. Only the slot the sprite bank uses is named.
class IGUIEnvironment {
public:
    virtual ~IGUIEnvironment() = default;
    // Vtable slot +0x20 (name not recovered). Returns a texture handle the bank keeps.
    virtual Texture* getDefaultTexture() = 0;
};

// Stand-in for glitch::gui::CGUISpriteBank. Only the members the reconstructed
// bodies touch are mapped.
class CGUISpriteBank : public ReferenceCounted {
public:
    explicit CGUISpriteBank(IGUIEnvironment* environment);
    ~CGUISpriteBank();

    // Returns a new reference (grabbed) or nullptr when index is out of range.
    Texture* getTexture(unsigned int index) const;
    void addTexture(Texture* texture);
    // Grows the list with null entries up to index, then replaces entry index.
    void setTexture(unsigned int index, Texture* texture);

    IGUIEnvironment* environment = nullptr;  // +0x2c
    Texture* driverTexture = nullptr;        // +0x30
    std::vector<Texture*> textures;          // +0x20 (begin) .. +0x28 (capacity end)
};

// Stand-in for the edit box child of CGUISpinBox. Slot +0x48 is getText.
class IGUIEditBox {
public:
    virtual ~IGUIEditBox() = default;
    // Vtable slot +0x48 (slot 18). Returns the wide text, or nullptr.
    virtual const wchar_t* getText() const = 0;
};

// Stand-in for glitch::gui::CGUISpinBox (only members used by this slice).
class CGUISpinBox {
public:
    // Parses the edit box text. The original returns float bits in r0 (soft-float ABI).
    float getValue() const;
    // Sets the number format: -1 gives "%f", n gives "%.<n>f".
    void setDecimalPlaces(int places);

    IGUIEditBox* editBox = nullptr;     // +0x158 (this[86])
    int decimalPlaces = 0;              // +0x1b8 (this[110])
    std::wstring format;                // +0x170 (this[92]), the printf format string
};

}  // namespace glitch::collada::recon_03
