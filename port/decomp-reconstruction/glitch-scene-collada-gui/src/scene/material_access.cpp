// The material parameter seam of the scene classes (scene/material_access.hpp). Not an original function: see the header.
#include "glitch-scene-collada-gui/scene/material_access.hpp"

#include "glitch-video/material.hpp"
#include "glitch-video/texture.hpp"

namespace glitch::scene {

bool setMaterialTexture(video::CMaterial&, std::uint16_t, std::uint32_t, const boost::intrusive_ptr<video::ITexture>&) {
    return false;
}

bool getMaterialTexture(const video::CMaterial&, std::uint16_t, std::uint32_t, boost::intrusive_ptr<video::ITexture>& texture) {
    texture.reset();
    return false;
}

bool setMaterialColorCvt(video::CMaterial&, std::uint16_t, std::uint32_t, video::SColor) {
    return false;
}

}  // namespace glitch::scene
