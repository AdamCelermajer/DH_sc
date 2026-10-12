// Slice 03 reconstruction. See include/glitch-scene-collada-gui/recon/slice_03.hpp.
#include "glitch-scene-collada-gui/recon/slice_03.hpp"

#include <cstdlib>
#include <string>

namespace glitch::collada::recon_03 {

void grab(ReferenceCounted* object) {
    if (object) {
        ++object->refCount;
    }
}

void drop(ReferenceCounted* object) {
    if (object && --object->refCount == 0) {
        delete object;
    }
}

// 0x54f790 _ZN6glitch3gui14CGUISpriteBankC1EPNS0_13IGUIEnvironmentE
// 0x54f820 _ZN6glitch3gui14CGUISpriteBankC2EPNS0_13IGUIEnvironmentE
CGUISpriteBank::CGUISpriteBank(IGUIEnvironment* environment_) : environment(environment_) {
    if (environment) {
        driverTexture = environment->getDefaultTexture();
        grab(driverTexture);
    }
}

// 0x54fbc0 _ZN6glitch3gui14CGUISpriteBankD1Ev
// 0x54fc68 _ZN6glitch3gui14CGUISpriteBankD2Ev
CGUISpriteBank::~CGUISpriteBank() {
    for (Texture*& slot : textures) {
        Texture* texture = slot;
        slot = nullptr;
        drop(texture);
    }
    drop(driverTexture);
    textures.clear();
}

// 0x54f8d4 _ZNK6glitch3gui14CGUISpriteBank10getTextureEj
Texture* CGUISpriteBank::getTexture(unsigned int index) const {
    if (index >= textures.size()) {
        return nullptr;
    }
    Texture* texture = textures[index];
    grab(texture);
    return texture;
}

// 0x54fe14 _ZN6glitch3gui14CGUISpriteBank10addTextureERKN5boost13intrusive_ptrINS0_5video8ITextureEEE
void CGUISpriteBank::addTexture(Texture* texture) {
    textures.push_back(texture);
    grab(texture);
}

// 0x54fe5c _ZN6glitch3gui14CGUISpriteBank10setTextureEjRKN5boost13intrusive_ptrINS0_5video8ITextureEEE
void CGUISpriteBank::setTexture(unsigned int index, Texture* texture) {
    while (textures.size() <= index) {
        textures.push_back(nullptr);
    }
    grab(texture);
    Texture* old = textures[index];
    textures[index] = texture;
    drop(old);
}

// 0x54f188 _ZNK6glitch3gui11CGUISpinBox8getValueEv
float CGUISpinBox::getValue() const {
    const wchar_t* text = editBox ? editBox->getText() : nullptr;
    if (!text) {
        return 0.0f;
    }
    std::string narrow;
    for (const wchar_t* p = text; *p; ++p) {
        narrow.push_back(static_cast<char>(*p));
    }
    return std::strtof(narrow.c_str(), nullptr);
}

// 0x54f638 _ZN6glitch3gui11CGUISpinBox16setDecimalPlacesEi
void CGUISpinBox::setDecimalPlaces(int places) {
    decimalPlaces = places;
    if (places == -1) {
        format = L"%f";
    } else {
        format = L"%." + std::to_wstring(places) + L"f";
    }
    // The original calls setValue(getValue()) through vtable slots +0x80 and +0x84.
}

}  // namespace glitch::collada::recon_03
