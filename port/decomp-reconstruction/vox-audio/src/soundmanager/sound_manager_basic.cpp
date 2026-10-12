// VoxSoundManager members whose bodies depend only on their own arguments or are empty.
#include "vox-audio/vox/sound_manager.hpp"

// 0x369348 _ZN15VoxSoundManager14SetIPodShuffleEv
// The iPod playlist path is compiled out on this build; the original body is a bare return.
void VoxSoundManager::SetIPodShuffle() {}

// 0x36934c _ZN15VoxSoundManager15UpdateIPodAsyncEv
// Empty in the original as well (the iPod update loop is not present in this build).
void VoxSoundManager::UpdateIPodAsync() {}

// 0x3692f0 _ZN15VoxSoundManager24ConvertVisual3DToSound3DERKN6glitch4core8vector3dIfEE
// The original returns through a hidden result pointer (r0) and reads the input from r1.
glitch::core::vector3d<float> VoxSoundManager::ConvertVisual3DToSound3D(
    const glitch::core::vector3d<float>& visual) {
    constexpr float kVisualToSoundScale = 0.01f;  // 0x3c23d70a
    glitch::core::vector3d<float> sound;
    sound.X = visual.X * kVisualToSoundScale;
    sound.Y = visual.Y * kVisualToSoundScale;
    sound.Z = visual.Z * kVisualToSoundScale;
    return sound;
}
