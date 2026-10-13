// VoxSoundManager: the game-side sound glue, 0x36921c to 0x36a624 (platform pass).
//
// Each body has two branches. JAVA_SOUNDS (the Android sound mode) forwards to the native sound
// bridge (nativeXxx, declared in sound_manager.hpp). The other branch calls the static
// vox::VoxEngine facade. The binary passes the engine object as the instance argument of those
// calls; the port facade has no instance, so the argument is dropped, and engine_ keeps the null
// checks that the binary makes on *this.
//
// Partial bodies are marked PARTIAL in their comments. Two gaps cause them: the binary takes the
// emitter count from VoxEngine::GetEmitterHandles, but the facade in voxengine.cpp returns void;
// and the binary reads sound data indices from the Arrays::Sounds table, which vox-audio cannot
// reach.

#include "vox-audio/vox/sound_manager.hpp"

#include <cstring>
#include <new>

#include "vox-audio/vox/engine_handles.hpp"
#include "vox-audio/vox/memory.hpp"
#include "vox-audio/vox/voxengine.hpp"
#include "vox-audio/vox/voxutils.hpp"

namespace {

// Group names passed to VoxSoundPackXML::GetGroupMask.
const char kGroupMusic[] = "MUSIC";
const char kGroupSfx[] = "SFX";
const char kGroupVfx[] = "VFX";

// LoadSound places the file under RES_PATH followed by this directory.
const char kSoundDir[] = "data/sounds/";

// Channel argument of nativeSetVolumeBig: 1 for music, 2 for SFX and VFX.
constexpr int kChannelMusic = 1;
constexpr int kChannelSfxVfx = 2;

// Fade times the binary passes (float constants in the listings).
constexpr float kVolumeFade = 0.05f;
constexpr float kResumeFade = 0.2f;
constexpr float kPauseFade = 0.2f;

// Group mask that addresses every group.
constexpr unsigned int kAllGroups = 0xFFFFFFFFu;

// Argument of nativeResumeSoundBig that resumes every sound.
constexpr int kAllNativeSounds = 9999;

// Destroys a DataHandle that LoadSound allocated: the destructor, then the block.
void DestroyDataHandle(vox::DataHandle* handle) {
    handle->~DataHandle();
    VoxFree(handle);
}

}  // namespace

// 0x36921c _ZN15VoxSoundManager11UnloadSoundEi
void VoxSoundManager::UnloadSound(int sound) {
    if (JAVA_SOUNDS || sound < 0 || sound >= sound_count_) {
        return;
    }
    vox::DataHandle* handle = sounds_[sound];
    if (handle != nullptr) {
        // The binary calls the deleting destructor through vtable slot +4, then stores
        // JAVA_SOUNDS (0 on this branch) into the slot.
        DestroyDataHandle(handle);
        sounds_[sound] = nullptr;
    }
}

// 0x369298 _ZN15VoxSoundManager15UnloadAllSoundsEv
void VoxSoundManager::UnloadAllSounds() {
    if (JAVA_SOUNDS) {
        return;
    }
    // The binary runs a do-while from 0 while the count is above the index. Same loop.
    for (int sound = 0; sound < sound_count_; ++sound) {
        UnloadSound(sound);
    }
}

// 0x369514 _ZN15VoxSoundManager13SetMusicStateEPKc (partial)
void VoxSoundManager::SetMusicState(const char* state) {
    if (music_id_ < 0 || JAVA_SOUNDS) {
        return;
    }
    // PARTIAL. The binary maps music_id_ to a data index through Arrays::Sounds (record stride 12,
    // index at +4) and takes sounds_[index]. It then builds one EmitterHandle (id -1, other words
    // 0), calls VoxEngine::GetEmitterHandles(data, handle, 1), and calls
    // VoxEngine::SetInteractiveMusicState(handle, state) only when the returned count is above 0.
    // Missing: the Arrays::Sounds lookup and the count (the facade returns void). The state is not
    // sent to the engine.
    (void)state;
}

// 0x3696dc _ZN15VoxSoundManager14IsSoundPlayingEi (partial)
bool VoxSoundManager::IsSoundPlaying(int sound) {
    if (sound < 0) {
        return false;
    }
    if (JAVA_SOUNDS) {
        return nativeIsMediaPlaying(sound) != 0;
    }
    // The binary indexes sounds_ with no bound check against sound_count_. Kept as is.
    if (sounds_[sound] == nullptr) {
        return false;
    }
    // PARTIAL. Missing: the array of 10 EmitterHandles filled by GetEmitterHandles, whose int result
    // is the count, and the IsPlaying loop over those handles. The facade returns void, so the loop
    // cannot run and the result is reported as false.
    return false;
}

// 0x369804 _ZN15VoxSoundManager14IsMusicPlayingEv (partial)
bool VoxSoundManager::IsMusicPlaying() {
    if (music_id_ == -1) {
        return false;
    }
    // PARTIAL. Missing: the Arrays::Sounds lookup of music_id_, the GetEmitterHandles count and the
    // IsPlaying test on the first handle. The result is reported as false. (The binary does not
    // check JAVA_SOUNDS here.)
    return false;
}

// 0x3698f0 _ZN15VoxSoundManager12StopAllMusicEi
void VoxSoundManager::StopAllMusic(int fade_ms) {
    if (JAVA_SOUNDS) {
        nativeStopAllBigSound(-1, fade_ms);
    } else {
        int mask = 0;
        sound_pack_.GetGroupMask(kGroupMusic, mask);
        vox::VoxEngine::StopAllEmitters(static_cast<unsigned int>(mask), static_cast<float>(fade_ms));
    }
    prev_music_id_ = music_id_;
    music_id_ = -1;
}

// 0x369990 _ZN15VoxSoundManager13StopAllSoundsEi
void VoxSoundManager::StopAllSounds(int fade_ms) {
    if (JAVA_SOUNDS) {
        nativeStopAllBigSound(-1, fade_ms);
        return;
    }
    if (engine_ != nullptr) {
        vox::VoxEngine::StopAllEmitters(kAllGroups, static_cast<float>(fade_ms) / 1000.0f);
    }
}

// 0x3699fc _ZN15VoxSoundManager9LoadSoundEi
void VoxSoundManager::LoadSound(int sound) {
    if (JAVA_SOUNDS || sound < 0 || sound > sound_count_) {
        return;
    }
    const char* name = nullptr;
    vox::FormatTypes format = {};
    int field_16 = 0;
    int field_15 = 0;
    vox::VoxSourceLoadingFlags flags = {};
    sound_pack_.GetDataSourceInfo(sound, name, format, field_16, field_15, flags);
    // The binary tests the slot again after GetDataSourceInfo; the bound is <= (not <), as in the
    // listing. The slot is written at index sound_count_ when that is the id.
    if (sound <= sound_count_ && sounds_[sound] == nullptr) {
        char path[512];
        std::strcpy(path, RES_PATH);
        std::strcpy(path + std::strlen(path), kSoundDir);
        std::strcat(path, name);
        // operator new(40, MemoryHintState 4) in the binary. The port's VoxAlloc takes its hint as
        // VoxMemHint, which names only the default (0); 4 is passed through as the binary has it.
        // The block is host-sized (sizeof(DataHandle)), not 40 bytes.
        void* block = VoxAlloc(sizeof(vox::DataHandle), static_cast<vox::VoxMemHint>(4));
        sounds_[sound] = new (block) vox::DataHandle(vox::VoxUtils::LoadDataSourceFromFileEx(
            path, static_cast<int>(format), static_cast<int>(flags), field_16));
    }
}

// 0x369b38 _ZN15VoxSoundManager14GetSoundVolumeEi
float VoxSoundManager::GetSoundVolume(int type) {
    if (JAVA_SOUNDS) {
        return 1.0f;
    }
    int group = 0;
    if (type == 2) {
        sound_pack_.GetGroupMask(kGroupMusic, group);
        return vox::VoxEngine::GetGroupGain(group) * 100.0f;
    }
    if (type == 3) {
        sound_pack_.GetGroupMask(kGroupVfx, group);
        return vox::VoxEngine::GetGroupGain(group) * 100.0f;
    }
    // Type 1 reads the SFX group. Any other type reads group 0 (the binary's JAVA_SOUNDS value).
    if (type == 1) {
        sound_pack_.GetGroupMask(kGroupSfx, group);
    }
    return vox::VoxEngine::GetGroupGain(group) * 100.0f;
}

// 0x369c2c _ZN15VoxSoundManager16SetInitialVolumeEfff
// The listing's Java branch stores the music and SFX values in the globals and passes them with
// channels 1 and 2 (the Hex-Rays view drops those two arguments).
void VoxSoundManager::SetInitialVolume(float music, float sfx, float vfx) {
    if (JAVA_SOUNDS) {
        sfx_vfx_volume = sfx;
        music_volume = music;
        nativeSetVolumeBig(music_id_, music, kChannelMusic);
        nativeSetVolumeBig(music_id_, sfx_vfx_volume, kChannelSfxVfx);
        return;
    }
    if (initial_volume_set_) {
        return;
    }
    int mask = 0;
    sound_pack_.GetGroupMask(kGroupMusic, mask);
    vox::VoxEngine::SetGroupGain(static_cast<unsigned int>(mask), music / 100.0f, kVolumeFade);
    sound_pack_.GetGroupMask(kGroupSfx, mask);
    vox::VoxEngine::SetGroupGain(static_cast<unsigned int>(mask), sfx / 100.0f, kVolumeFade);
    sound_pack_.GetGroupMask(kGroupVfx, mask);
    vox::VoxEngine::SetGroupGain(static_cast<unsigned int>(mask), vfx / 100.0f, kVolumeFade);
    initial_volume_set_ = true;
}

// 0x369da0 _ZN15VoxSoundManager14SetSoundVolumeEif
// As in SetInitialVolume, the Java branch passes the volume with channel 1 (music) or 2 (SFX, VFX).
void VoxSoundManager::SetSoundVolume(int type, float volume) {
    if (JAVA_SOUNDS) {
        if (type == 2) {
            music_volume = volume;
            nativeSetVolumeBig(music_id_, volume, kChannelMusic);
        } else if (type == 3 || type == 1) {
            sfx_vfx_volume = volume;
            nativeSetVolumeBig(music_id_, volume, kChannelSfxVfx);
        }
        return;
    }
    unsigned int group = 0;  // the binary's default is the JAVA_SOUNDS value, 0
    int mask = 0;
    switch (type) {
        case 2:
            sound_pack_.GetGroupMask(kGroupMusic, mask);
            group = static_cast<unsigned int>(mask);
            break;
        case 3:
            sound_pack_.GetGroupMask(kGroupVfx, mask);
            group = static_cast<unsigned int>(mask);
            break;
        case 1:
            sound_pack_.GetGroupMask(kGroupSfx, mask);
            group = static_cast<unsigned int>(mask);
            break;
        default:
            break;
    }
    vox::VoxEngine::SetGroupGain(group, volume / 100.0f, kVolumeFade);
}

// 0x369ee8 _ZN15VoxSoundManager15GetMasterVolumeEv
// The Java branch returns 0x3f800000, the bit pattern of 1.0f, so the result is a float.
float VoxSoundManager::GetMasterVolume() {
    if (JAVA_SOUNDS) {
        return 1.0f;
    }
    return vox::VoxEngine::GetMasterGain();
}

// 0x369f1c _ZN15VoxSoundManager15SetMasterVolumeEf
void VoxSoundManager::SetMasterVolume(float volume) {
    if (!JAVA_SOUNDS) {
        vox::VoxEngine::SetMasterGain(volume, kVolumeFade);
    }
}

// 0x369f50 _ZN15VoxSoundManager14SetListenerPosERKN6glitch4core8vector3dIfEES5_S5_iif
void VoxSoundManager::SetListenerPos(const glitch::core::vector3d<float>& pos,
                                     const glitch::core::vector3d<float>& front,
                                     const glitch::core::vector3d<float>& up,
                                     int a5, int a6, float a7) {
    if (JAVA_SOUNDS) {
        return;
    }
    vox::VoxEngine::Set3DListenerPosition(pos.X, pos.Y, pos.Z);
    vox::VoxEngine::Set3DListenerOrientation(front.X, front.Y, front.Z, up.X, up.Y, up.Z);
    unk_5c = a7;
    unk_54 = a5;
    unk_58 = a6;
}

// 0x369fec _ZN15VoxSoundManager4StopEii (partial)
void VoxSoundManager::Stop(int sound, int fade_ms) {
    if (sound < 0) {
        return;
    }
    if (JAVA_SOUNDS) {
        nativeStopSoundBig(sound);
        return;
    }
    // PARTIAL. The binary maps the sound to a data index through Arrays::Sounds (record stride 12,
    // index at +4), takes sounds_[index] and calls IsReady on it. When that holds it builds 10
    // EmitterHandles, takes the count from GetEmitterHandles and calls Stop(handle, fade_ms / 1000)
    // for each handle, then destroys the handles. Missing: the Arrays::Sounds lookup and the count
    // (the facade returns void). Nothing is stopped on this branch.
    (void)fade_ms;
}

// 0x36a1a0 _ZN15VoxSoundManager9StopMusicEi (partial)
// The Hex-Rays body is missing for this row. The body below is from the Ghidra export, and the ARM
// listing agrees with it: the Java branch stores -1 in Save_Current_Music_ID before the native stop.
void VoxSoundManager::StopMusic(int fade_ms) {
    if (music_id_ == -1) {
        return;
    }
    if (!JAVA_SOUNDS) {
        // PARTIAL: Stop(int, int) is partial (see above), so the emitters are not stopped here.
        Stop(music_id_, fade_ms);
    } else {
        Save_Current_Music_ID = -1;
        nativeStopSoundBig(music_id_);
    }
    prev_music_id_ = music_id_;
    music_id_ = -1;
}

// 0x36a218 _ZN15VoxSoundManager6Stop3DEiiP7Point3DIfEf (partial)
void VoxSoundManager::Stop3D(int sound, int fade_ms, const float* pos, float radius) {
    if (sound < 0) {
        return;
    }
    if (JAVA_SOUNDS) {
        nativeStopSoundBig(sound);
        return;
    }
    // PARTIAL. The binary takes the sound's handles through Arrays::Sounds and GetEmitterHandles,
    // then for each handle reads its 3D position, takes the distance to pos (x, y, z floats) and
    // stops the handle with fade_ms / 1000 when radius is below that distance. With pos null it
    // stops handles without the distance test, in a loop the Hex-Rays view does not structure
    // cleanly. Missing: the Arrays::Sounds lookup, the count and that loop. Nothing is stopped here.
    (void)fade_ms;
    (void)pos;
    (void)radius;
}

// 0x36a470 _ZN15VoxSoundManager6ResumeEi (partial)
void VoxSoundManager::Resume(int sound) {
    if (sound < 0) {
        return;
    }
    if (JAVA_SOUNDS) {
        nativeResumeSoundBig(sound);
        return;
    }
    // PARTIAL. The binary maps the sound to a data index through Arrays::Sounds, takes sounds_[index]
    // and resumes each of its handles (GetEmitterHandles, count, then VoxEngine::Resume(handle,
    // 0.05f)). Missing: the Arrays::Sounds lookup and the count. Nothing is resumed here.
}

// 0x36a590 _ZN15VoxSoundManager15ResumeAllSoundsEv
void VoxSoundManager::ResumeAllSounds() {
    if (JAVA_SOUNDS) {
        nativeResumeSoundBig(kAllNativeSounds);
        if (Resume_Sound_Interrupt) {
            Resume_Sound_Interrupt = false;
            if (Save_Current_Music_ID != -1) {
                nativeResumeSoundBig(Save_Current_Music_ID);
            }
        }
    } else if (engine_ != nullptr) {
        vox::VoxEngine::ResumeAllEmitters(kAllGroups, kResumeFade);
    }
}

// 0x36a624 _ZN15VoxSoundManager14PauseAllSoundsEv
void VoxSoundManager::PauseAllSounds() {
    if (!JAVA_SOUNDS && engine_ != nullptr) {
        vox::VoxEngine::PauseAllEmitters(kAllGroups, kPauseFade);
    }
}
