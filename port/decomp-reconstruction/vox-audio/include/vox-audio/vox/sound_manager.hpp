// VoxSoundManager: the game-side sound glue at 0x36xxxx. Only the members whose bodies are
// rebuilt so far are declared here. Most of the class calls game globals and game classes
// that are not in the port (JAVA_SOUNDS, Arrays::Sounds, MenuManager, RenderFX, ...), so the
// layout is not complete; see vox-audio/NOTES.md (VoxSoundManager).
#pragma once

#include "vox-audio/deps/glitch_vector3d.hpp"

class VoxSoundManager {
public:
    // 0x369348 VoxSoundManager::SetIPodShuffle(void). The original body is empty.
    void SetIPodShuffle();

    // 0x36934c VoxSoundManager::UpdateIPodAsync(void). The original body is empty.
    void UpdateIPodAsync();

    // 0x3692f0 VoxSoundManager::ConvertVisual3DToSound3D(glitch::core::vector3d<float> const&).
    // Scales each visual component by 0.01f (constant 0x3c23d70a in the ARM listing).
    static glitch::core::vector3d<float> ConvertVisual3DToSound3D(
        const glitch::core::vector3d<float>& visual);
};
