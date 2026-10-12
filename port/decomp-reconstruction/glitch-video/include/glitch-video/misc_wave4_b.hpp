// Wave 4 (glitch-video slice 3) reconstructions that cannot be declared as members yet.
//
// The member declarations for these functions belong in texture_manager.hpp, which is
// outside this slice's files. Until those headers are opened, the bodies are free
// functions that take the object explicitly. Each one names its original member and
// address so the move into the class is mechanical.
#pragma once

#include <cstdint>

#include "glitch-video/deps/intrusive_ptr.hpp"
#include "glitch-video/texture.hpp"
#include "glitch-video/texture_manager.hpp"

namespace glitch::video::misc_wave4_b {

// The original E_TEXTURE_TYPE argument value that means "take the type from the texture".
constexpr uint32_t kTextureTypeFromTexture = 255;

// 0x5ea228 CTextureManager::setPlaceHolder(E_PLACE_HOLDER, intrusive_ptr<ITexture> const&, E_TEXTURE_TYPE).
// The placeholder table is m_placeHolders[4 * placeHolder + type] (the original word index
// is 18 + 4 * placeHolder + type from the manager base). The old occupant is removed first,
// and removed again when its reference count is one.
void setPlaceHolder(CTextureManager& manager, int32_t placeHolder,
                    const boost::intrusive_ptr<ITexture>& texture, uint32_t textureType);

}  // namespace glitch::video::misc_wave4_b
