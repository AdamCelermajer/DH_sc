// Slice 01 reconstruction (glitch::gui). Free functions for the rebuilt bodies, plus the
// minimal stand-in types they need. Existing headers are included read-only.
#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "glitch-scene-collada-gui/gui_elements.hpp"

namespace glitch::gui::recon_slice_01 {

// Reference-counted header shared by the stand-ins. The count is the word at +0x08 on
// the 64-bit build (the original 32-bit build keeps it at +0x04).
class Recon01RefCounted {
public:
    virtual ~Recon01RefCounted() = default;
    int referenceCount = 1;  // +0x04 (32-bit build)
};

// Adds one reference. Null is accepted.
void recon01Grab(Recon01RefCounted* object);

// Releases one reference and deletes the object when the count reaches zero. Null is accepted.
void recon01Drop(Recon01RefCounted* object);

// 0x537dac _ZN6glitch3gui15CGUIEnvironment27registerGUIElementFactoryEPNS0_20IGUIElementFactoryE
// Grabs the factory and appends it to the environment's factory list (+0x16c).
void registerGUIElementFactory(std::vector<void*>& factories, Recon01RefCounted* factory);

// 0x538234 _ZN6glitch3gui11IGUIElement14setToolTipTextEPKw
// Replaces the tool-tip text (+0xe8).
void setToolTipText(IGUIElement& element, const wchar_t* text);

// Font-name key used by the built-in font list. The original stores the name as a
// begin/end pointer pair at +0x14/+0x10 and the sort key at +0x18.
struct STTFontKey {
    std::string_view name;  // +0x10..+0x14 (end minus begin, begin pointer)
    int key = 0;            // +0x18

    // 0x539d44 _ZNK6glitch3gui15CGUIEnvironment7STTFontltERKS2_
    // Orders by name; equal names fall back to the key.
    bool operator<(const STTFontKey& other) const;
};

// 0x541de4 _ZN6glitch3gui14CGUIListBox13setSpriteBankEPNS0_15IGUISpriteBankE
// Replaces the sprite bank slot (+0x178): drops the old bank, grabs the new one.
// The original returns the result of the drop call, which is not reproduced here.
void setSpriteBank(Recon01RefCounted*& slot, Recon01RefCounted* bank);

}  // namespace glitch::gui::recon_slice_01
