// 0x537dac, 0x538234, 0x539d44, 0x541de4 reconstructed in slice 01.
#include "glitch-scene-collada-gui/recon/slice_01.hpp"

namespace glitch::gui::recon_slice_01 {

void recon01Grab(Recon01RefCounted* object) {
    if (object) {
        ++object->referenceCount;
    }
}

void recon01Drop(Recon01RefCounted* object) {
    if (object && --object->referenceCount == 0) {
        delete object;
    }
}

// 0x537dac _ZN6glitch3gui15CGUIEnvironment27registerGUIElementFactoryEPNS0_20IGUIElementFactoryE
void registerGUIElementFactory(std::vector<void*>& factories, Recon01RefCounted* factory) {
    if (!factory) {
        return;
    }
    recon01Grab(factory);
    factories.push_back(factory);
}

// 0x538234 _ZN6glitch3gui11IGUIElement14setToolTipTextEPKw
void setToolTipText(IGUIElement& element, const wchar_t* text) {
    element.toolTipText.assign(text);  // +0xe8
}

// 0x539d44 _ZNK6glitch3gui15CGUIEnvironment7STTFontltERKS2_
bool STTFontKey::operator<(const STTFontKey& other) const {
    if (name.size() == other.name.size() && name == other.name) {
        return key < other.key;
    }
    return name < other.name;
}

// 0x541de4 _ZN6glitch3gui14CGUIListBox13setSpriteBankEPNS0_15IGUISpriteBankE
void setSpriteBank(Recon01RefCounted*& slot, Recon01RefCounted* bank) {
    recon01Drop(slot);  // +0x178 old bank
    slot = bank;
    recon01Grab(bank);
}

}  // namespace glitch::gui::recon_slice_01
