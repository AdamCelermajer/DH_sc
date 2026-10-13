// VoxSoundManager: the rest of the game-side sound glue (batch p1-vox-audio-VoxSoundManager-3a5f5c-1,
// platform pass). Covers the destructor, the pause and play family, the music cross-fade, the music
// play and safe-zone switch, and the level release. The first half of the class is in
// src/p1-vox-audio-VoxSoundManager-3a5f5c-0.cpp.
//
// Conventions as in the first batch. A JAVA_SOUNDS branch forwards to the native sound bridge
// (nativeXxxSoundBig, declared in sound_manager.hpp). The other branch calls the static vox::VoxEngine
// facade and drops the engine instance argument of the binary. Arrays::Sounds records are reached
// through the typed element pointer (pydata element_table.hpp), never as bytes at a 32-bit stride.
// A body whose behaviour cannot be reproduced yet is marked PARTIAL where it stands.

#include "vox-audio/vox/sound_manager.hpp"

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <vector>

#include "dh2/Application.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/Level.hpp"
#include "dh2/MenuBase.hpp"
#include "dh2/Singleton.hpp"
#include "dh2/global/net_singletons.hpp"
#include "pydata-accessors/structs_records.hpp"
#include "pydata-accessors/w7_arrays_00.hpp"
#include "vox-audio/vox/engine_handles.hpp"
#include "vox-audio/vox/memory.hpp"
#include "vox-audio/vox/random48.hpp"
#include "vox-audio/vox/voxengine.hpp"

namespace {

// Emitter slots the binary sets up for GetEmitterHandles in Pause.
constexpr int kEmitterSlots = 10;
// Fade the binary passes to VoxEngine::Play in PlaySoundPackSound and Play (float constant 0.05).
constexpr float kPlayFade = 0.05f;
// Fade of the music calls (PlayMusic, SetInSafeZoneMusic, CrossfadeMusic), in milliseconds.
constexpr int kMusicFadeMs = 2000;
// Channel argument of nativePlaySoundBig: 1 for music, 2 for SFX and VFX.
constexpr int kChannelMusic = 1;
constexpr int kChannelSfxVfx = 2;
// Play group type of the music calls. Play tests (type - 1) <= 0x1d before it sets the group.
constexpr int kMusicType = 2;
// Level::m_loadStep when the level is fully loaded (Play3D).
constexpr int kLevelLoadedStep = 38;
// Parameters Play3D passes to PlayEvent (the bit patterns of -1.0f).
constexpr float kNoParam = -1.0f;
// Arrays::Sounds record kind of an event (m_w2 == 1). The data index of an event is the event id.
constexpr std::int32_t kEventKind = 1;

// Position and basis vectors of the listing (the Hex-Rays words v18..v23 in PlaySoundPackSound).
struct Vec3 {
    float x;
    float y;
    float z;
};

// The same operation as glitch::core::vector3d<float>::normalize (0x35e8e0) in glitch-shared/glitch_core.hpp:
// scales by 1 / sqrt(|v|^2) unless |v|^2 is zero. Vec3 is local to this listing and does not use the shared type.
void NormalizeInPlace(Vec3& v) {
    const float v2 = (v.x * v.x + v.y * v.y) + v.z * v.z;
    if (v2 != 0.0f) {
        const float inv = 1.0f / std::sqrt(v2);
        v.x = v.x * inv;
        v.y = v.y * inv;
        v.z = v.z * inv;
    }
}

// DebugSwitches::load() and then GetSwitch(name): the pair the binary runs before each switch read.
bool ReadDebugSwitch(const char* name) {
    DebugSwitches::s_inst.load();
    return DebugSwitches::s_inst.GetSwitch(name);
}

// A trace switch: the binary reads it and drops the result.
void ReadTraceSwitch(const char* name) {
    (void)ReadDebugSwitch(name);
}

// Arrays::Sounds element `sound`. The table's members points at element 0 of host Structs::SoundAutoGen
// records (pydata element_table.hpp). The original's 12-byte listing stride is not applied to bytes.
// +0x4 (m_w1) is the sound's data index, which is the event id when m_w2 == kEventKind; +0x8 (m_w2) is the kind.
Structs::SoundAutoGen& SoundRecord(int sound) {
    return reinterpret_cast<Structs::SoundAutoGen*>(Arrays::Sounds::members)[sound];
}

// The EmitterHandle the binary builds on the stack for an empty slot: id -1, no engine, no handlable, zero stamps.
vox::EmitterHandle EmptyEmitterHandle() {
    return vox::EmitterHandle(-1, nullptr, nullptr, 0, 0);
}

std::vector<vox::EmitterHandle> MakeEmptyHandles(std::size_t count) {
    return std::vector<vox::EmitterHandle>(count, EmptyEmitterHandle());
}

// PARTIAL stand-in for the online check in Play and Play3D. The binary reads COnline::isOnlineGame_ (+0x05)
// through GetOnline(). That field is protected in net-session-online/c_online.hpp, so it cannot be read here.
// The stand-in still calls GetOnline() (the singleton is created on first use, as in the binary) and answers
// with the constructor default, false.
bool OnlineGameStandIn() {
    (void)GetOnline();
    return false;
}

// Destroys a DataHandle that LoadSound allocated: the destructor, then the block (as in the first batch).
void DestroyDataHandle(vox::DataHandle* handle) {
    handle->~DataHandle();
    VoxFree(handle);
}

}  // namespace

// 0x36afb0 _ZN15VoxSoundManagerD1Ev
// 0x36b088 _ZN15VoxSoundManagerD2Ev (the second copy of the destructor; the same body)
VoxSoundManager::~VoxSoundManager() {
    if (!JAVA_SOUNDS) {
        CustomFree(unk_04);
        UnloadAllSounds();
        CustomFree(sounds_);
        vox::VoxEngine::DestroyVoxEngine();
    }
    // After this body the members are destroyed in reverse order: sound_pack_ (+0x64), unk_38 (+0x38) and
    // preload_ids_ (+0x0c). The binary runs the same three releases in that order.
}

// 0x36a664 _ZN15VoxSoundManager5PauseEi (partial)
void VoxSoundManager::Pause(int sound) {
    if (sound < 0) {
        return;
    }
    if (JAVA_SOUNDS) {
        nativePauseSoundBig(sound);
        return;
    }
    vox::DataHandle* data = sounds_[SoundRecord(sound).m_w1];
    if (data == nullptr) {
        return;
    }
    // PARTIAL. The binary fills ten EmitterHandles with GetEmitterHandles(data, handles, 10), takes the
    // count it returns and calls VoxEngine::Pause(handle, 0.05f) for each of those handles. The facade's
    // GetEmitterHandles returns void, so the count is missing and no handle is paused. The call is kept.
    std::vector<vox::EmitterHandle> handles = MakeEmptyHandles(kEmitterSlots);
    vox::VoxEngine::GetEmitterHandles(*data, handles.data(), kEmitterSlots);
}

// 0x36a784 _ZN15VoxSoundManager10PauseMusicEv (partial: calls Pause)
void VoxSoundManager::PauseMusic() {
    if (music_id_ == -1) {
        return;
    }
    if (JAVA_SOUNDS) {
        nativePauseSoundBig(music_id_);
    } else {
        Pause(music_id_);
    }
}

// 0x36a7c0 _ZN15VoxSoundManager18PlaySoundPackSoundEiPKcN3vox11FormatTypesEiiNS2_21VoxSourceLoadingFlagsERKN6glitch4core8vector3dIfEEff
// Parameters: the sound id, the data name, the format, field_16, the priority bank (the binary's a6),
// the loading flags (a7, unused by the body), the position, and the two floats the binary stores as
// 3D emitter parameters 2 and 1.
int VoxSoundManager::PlaySoundPackSound(int sound, const char* name, vox::FormatTypes format, int field_16,
                                        int bank_id, vox::VoxSourceLoadingFlags flags,
                                        const glitch::core::vector3d<float>& pos,
                                        float param2, float param1) {
    (void)name;
    (void)format;
    (void)field_16;
    (void)flags;
    if (JAVA_SOUNDS) {
        return 0;
    }
    vox::DataHandle* data = sounds_[sound];
    if (data == nullptr) {
        LoadSound(sound);
        data = sounds_[sound];
    }
    if (data != nullptr && vox::VoxEngine::IsReady(*data) != 0) {
        vox::VoxEngine::SetPriorityBankId(*data, bank_id);
        int emitter_uid = 0;
        int group = 0;
        bool loop = false;
        vox::Vox3DSoundType type = static_cast<vox::Vox3DSoundType>(0);
        const char* dsp_name = nullptr;
        sound_pack_.GetEmitterInfo(sound, emitter_uid, group, loop, type, dsp_name);
        // The binary passes the emitter uid as the flags argument and no user pointer (a null on the stack).
        vox::EmitterHandle emitter = vox::VoxEngine::CreateEmitter(*data, emitter_uid, nullptr);

        const int type_value = static_cast<int>(type);
        if (type_value != 0) {
            if (param2 >= 0.0f && param1 >= 0.0f) {
                vox::VoxEngine::Set3DEmitterPosition(emitter, pos.X, pos.Y, pos.Z);
                vox::VoxEngine::Set3DEmitterParameterf(emitter, 2, param2);
                vox::VoxEngine::Set3DEmitterParameterf(emitter, 1, param1);
            } else {
                vox::VoxEngine::Set3DEmitterPosition(emitter, pos.X, pos.Y, pos.Z);
                vox::VoxEngine::Set3DEmitterParameterf(emitter, 2, static_cast<float>(unk_54));
                vox::VoxEngine::Set3DEmitterParameterf(emitter, 1, static_cast<float>(unk_58));
            }
        } else {
            vox::VoxEngine::Set3DEmitterParameteri(emitter, 0, 1);
        }
        vox::VoxEngine::Set3DEmitterParameterf(emitter, 3, unk_5c);

        if (type_value == 2) {
            // Relative 3D: the emitter position is written in the listener's frame. The listener's front
            // is normalised, the side vector is the normalised cross(front, up), and the top vector is the
            // normalised cross(side, front). Each cross product is written as the listing writes it.
            // The listing (loc_36AB50) first sets 3D emitter parameter 0 to 1, before it reads the listener.
            vox::VoxEngine::Set3DEmitterParameteri(emitter, 0, 1);
            float fx = 0.0f;
            float fy = 0.0f;
            float fz = 0.0f;
            float ux = 0.0f;
            float uy = 0.0f;
            float uz = 0.0f;
            vox::VoxEngine::Get3DListenerOrientation(fx, fy, fz, ux, uy, uz);
            Vec3 front{fx, fy, fz};
            const Vec3 up{ux, uy, uz};
            NormalizeInPlace(front);
            Vec3 side{(front.z * -up.y) + (up.z * front.y),
                      (front.x * -up.z) + (up.x * front.z),
                      (front.y * -up.x) + (up.y * front.x)};
            NormalizeInPlace(side);
            Vec3 top{(side.z * -front.y) + (front.z * side.y),
                     (side.x * -front.z) + (front.x * side.z),
                     (side.y * -front.x) + (front.y * side.x)};
            NormalizeInPlace(top);

            float ex = 0.0f;
            float ey = 0.0f;
            float ez = 0.0f;
            vox::VoxEngine::Get3DEmitterPosition(emitter, ex, ey, ez);
            float lx = 0.0f;
            float ly = 0.0f;
            float lz = 0.0f;
            vox::VoxEngine::Get3DListenerPosition(lx, ly, lz);
            const float dx = ex - lx;
            const float dy = ey - ly;
            const float dz = ez - lz;
            const float rel_x = (side.x * dx + side.y * dy) + side.z * dz;
            const float rel_y = (top.x * dx + top.y * dy) + top.z * dz;
            const float rel_z = (front.x * dx + front.y * dy) + front.z * dz;
            vox::VoxEngine::Set3DEmitterPosition(emitter, rel_x, rel_y, rel_z);
        }

        vox::VoxEngine::SetDSPEmitterParameter(emitter, 0, const_cast<char*>(dsp_name));

        // The two group uids are function-local statics in the binary (guard variables), read from the
        // pack on first use.
        static const int sfx_3d_pitched = sound_pack_.GetGroupUid("sfx_3D_pitched");
        static const int sfx_3d_pitched_relative = sound_pack_.GetGroupUid("sfx_3D_pitched_relative");
        if (group == sfx_3d_pitched || group == sfx_3d_pitched_relative) {
            const int saved_option = Singleton<Application>::s_inst.GetSavedOption("VolumeFX");
            // The binary multiplies the option by (rand % 123) * 0.001 + 0.96 in double, then stores a float.
            const double scale = static_cast<double>(vox::Lrand48() % 123) * 0.001 + 0.96;
            const float volume = static_cast<float>(static_cast<double>(static_cast<float>(saved_option)) * scale);
            SetSoundVolume(group, volume);
        }
        vox::VoxEngine::SetGroup(emitter, group);
        vox::VoxEngine::Play(emitter, loop, kPlayFade);
    }
    return 0;
}

// 0x36b2cc _ZN15VoxSoundManager16ReleaseLvlSoundsEv
void VoxSoundManager::ReleaseLvlSounds() {
    if (JAVA_SOUNDS) {
        return;
    }
    for (int sound = 0; sound < sound_count_; ++sound) {
        if (sounds_[sound] == nullptr) {
            continue;
        }
        // Sounds listed in preload_ids_ stay loaded.
        if (std::find(preload_ids_.begin(), preload_ids_.end(), sound) != preload_ids_.end()) {
            continue;
        }
        ReadTraceSwitch("isTracingPreload_SFX");
        vox::DataHandle* handle = sounds_[sound];
        if (handle != nullptr) {
            DestroyDataHandle(handle);
        }
        sounds_[sound] = nullptr;
    }
}

// 0x36b420 _ZN15VoxSoundManager9PlayEventEiRKN6glitch4core8vector3dIfEEff
int VoxSoundManager::PlayEvent(int event, const glitch::core::vector3d<float>& pos,
                               float param2, float param1) {
    if (!ReadDebugSwitch("IsDisablingSounds") && event >= 0) {
        if (JAVA_SOUNDS) {
            nativePlaySoundBig(event, sfx_vfx_volume, 0, kChannelSfxVfx);
        } else {
            int sound_uid = 0;
            sound_pack_.GetEventSoundUid(event, sound_uid);
            if (sound_uid >= 0) {
                ReadTraceSwitch("isTracingSFX");
                const char* name = nullptr;
                vox::FormatTypes format = static_cast<vox::FormatTypes>(0);
                int field_16 = 0;
                int bank_id = 0;
                vox::VoxSourceLoadingFlags flags = static_cast<vox::VoxSourceLoadingFlags>(0);
                sound_pack_.GetDataSourceInfo(sound_uid, name, format, field_16, bank_id, flags);
                PlaySoundPackSound(sound_uid, name, format, field_16, bank_id, flags, pos, param2, param1);
            }
        }
    }
    return 0;
}

// 0x36b5d8 _ZN15VoxSoundManager6Play3DEiRKN6glitch4core8vector3dIfEEbiff (partial: online stand-in)
int VoxSoundManager::Play3D(int sound, const glitch::core::vector3d<float>& pos, bool loop, int reserved,
                            float param2, float param1) {
    (void)reserved;  // the binary's int argument is not read by the body
    if (ReadDebugSwitch("IsDisablingSounds")) {
        return 0;
    }
    Level* level = Singleton<Application>::s_inst.GetCurrentLevel();
    if (level == nullptr) {
        return 0;
    }
    // PARTIAL: the online half of the gate uses OnlineGameStandIn (see above).
    if (level->m_loadStep == kLevelLoadedStep &&
        (!OnlineGameStandIn() || !MenuBase::s_igmOpened) && sound >= 0) {
        if (JAVA_SOUNDS) {
            nativePlaySoundBig(sound, sfx_vfx_volume, loop ? 1 : 0, kChannelSfxVfx);
        } else {
            const Structs::SoundAutoGen& record = SoundRecord(sound);
            const std::int32_t data = record.m_w1;
            if (record.m_w2 == kEventKind) {
                PlayEvent(data, pos, kNoParam, kNoParam);
            } else {
                const char* name = nullptr;
                vox::FormatTypes format = static_cast<vox::FormatTypes>(0);
                int field_16 = 0;
                int bank_id = 0;
                vox::VoxSourceLoadingFlags flags = static_cast<vox::VoxSourceLoadingFlags>(0);
                sound_pack_.GetDataSourceInfo(data, name, format, field_16, bank_id, flags);
                ReadTraceSwitch("isTracingSFX");
                PlaySoundPackSound(data, name, format, field_16, bank_id, flags, pos, param2, param1);
            }
        }
    }
    return 0;
}

// 0x36b80c _ZN15VoxSoundManager4PlayEibiib (partial: online stand-in)
int VoxSoundManager::Play(int sound, bool loop, int fade_ms, int type, bool force) {
    if (ReadDebugSwitch("IsDisablingSounds") || sound < 0) {
        return 0;
    }
    // PARTIAL: the online half of the gate uses OnlineGameStandIn (see above). The binary tests force first.
    if (!(force || !OnlineGameStandIn() || !MenuBase::s_igmOpened)) {
        return 0;
    }
    if (JAVA_SOUNDS) {
        nativePlaySoundBig(sound, sfx_vfx_volume, loop ? 1 : 0, kChannelSfxVfx);
        return 0;
    }
    const std::int32_t data = SoundRecord(sound).m_w1;
    const char* name = nullptr;
    vox::FormatTypes format = static_cast<vox::FormatTypes>(0);
    int field_16 = 0;
    int bank_id = 0;
    vox::VoxSourceLoadingFlags flags = static_cast<vox::VoxSourceLoadingFlags>(0);
    sound_pack_.GetDataSourceInfo(data, name, format, field_16, bank_id, flags);
    vox::DataHandle* handle = sounds_[data];
    if (handle == nullptr) {
        LoadSound(data);
        handle = sounds_[data];
    }
    if (handle == nullptr || vox::VoxEngine::IsReady(*handle) == 0) {
        return 0;
    }
    vox::VoxEngine::SetPriorityBankId(*handle, bank_id);
    int emitter_uid = 0;
    int group = 0;
    bool flag = false;
    vox::Vox3DSoundType emitter_type = static_cast<vox::Vox3DSoundType>(0);
    const char* dsp_name = nullptr;
    sound_pack_.GetEmitterInfo(data, emitter_uid, group, flag, emitter_type, dsp_name);
    vox::EmitterHandle emitter = vox::VoxEngine::CreateEmitter(*handle, emitter_uid, nullptr);
    vox::VoxEngine::Set3DEmitterParameteri(emitter, 0, 1);
    vox::VoxEngine::SetDSPEmitterParameter(emitter, 0, const_cast<char*>(dsp_name));
    if (static_cast<unsigned int>(type - 1) <= 0x1Du) {
        vox::VoxEngine::SetGroup(emitter, group);
    }
    vox::VoxEngine::Play(emitter, flag, static_cast<float>(static_cast<double>(static_cast<float>(fade_ms)) / 1000.0));
    return 0;
}

// 0x36baa0 _ZN15VoxSoundManager14CrossfadeMusicEii (partial: calls Stop and Play)
void VoxSoundManager::CrossfadeMusic(int music_a, int music_b) {
    if (JAVA_SOUNDS || music_b == -1) {
        return;
    }
    // music_slot_ picks the stopping sound and the playing sound.
    const int stop_sound = music_slot_ ? music_a : music_b;
    const int play_sound = music_slot_ ? music_b : music_a;
    Structs::SoundAutoGen& play_record = SoundRecord(play_sound);
    Structs::SoundAutoGen& stop_record = SoundRecord(stop_sound);
    const std::int32_t play_data = play_record.m_w1;
    const std::int32_t stop_data = stop_record.m_w1;
    // The stop record takes the play record's data index and kind.
    stop_record.m_w1 = play_data;
    stop_record.m_w2 = play_record.m_w2;
    if (sounds_[stop_data] == nullptr) {
        LoadSound(stop_data);
    }
    if (sounds_[play_data] == nullptr) {
        LoadSound(play_data);
    }
    std::vector<vox::EmitterHandle> handles = MakeEmptyHandles(1);
    vox::VoxEngine::GetEmitterHandles(*sounds_[stop_data], handles.data(), 1);
    const float cursor = vox::VoxEngine::GetPlayCursor(handles[0]);
    Stop(stop_sound, kMusicFadeMs);
    Play(play_sound, true, kMusicFadeMs, kMusicType, false);
    vox::VoxEngine::GetEmitterHandles(*sounds_[play_data], handles.data(), 1);
    vox::VoxEngine::SetPlayCursor(handles[0], cursor);
    music_slot_ = !music_slot_;
}

// 0x36bd78 _ZN15VoxSoundManager9PlayMusicEibbi (partial: resume branch and byte_99F7E0 gate)
void VoxSoundManager::PlayMusic(int music, bool loop, bool stop_if_none, int fade_ms) {
    // GetSoundVolume type 2 is the music group (the binary's literal 2).
    if (GetSoundVolume(2) < 0.5) {
        return;
    }
    if (ReadDebugSwitch("IsDisablingSounds")) {
        return;
    }
    // PARTIAL: the binary returns unless the unnamed byte at 0x99f7e0 is set. That byte has no symbol in the
    // tree, so this body runs as if it were set.
    if (music == -1) {
        if (stop_if_none) {
            music_id_ = -1;
            StopMusic(fade_ms);
        }
        return;
    }
    if (music_id_ == music && JAVA_SOUNDS) {
        Save_Current_Music_ID = music_id_;
        nativePlaySoundBig(Save_Current_Music_ID, music_volume, loop ? 1 : 0, kChannelMusic);
        return;
    }
    // PARTIAL: for the same music, the binary resumes the playing emitter when GetEmitterHandles reports a
    // count above 0 (VoxEngine::Resume(handle, 0.05f), then return). The facade returns void, so that branch
    // is never taken and the music is started again below.
    const int previous = music_id_;
    prev_music_id_ = previous;
    if (previous != music) {
        StopMusic(fade_ms);
    }
    music_id_ = music;
    music_loop_ = loop;
    if (JAVA_SOUNDS) {
        Save_Current_Music_ID = music;
        nativePlaySoundBig(music, music_volume, loop ? 1 : 0, kChannelMusic);
    } else {
        Play(music, loop, fade_ms, kMusicType, true);
    }
}

// 0x36c014 _ZN15VoxSoundManager18SetInSafeZoneMusicEb (partial: calls PlayMusic)
void VoxSoundManager::SetInSafeZoneMusic(bool in_zone) {
    if (JAVA_SOUNDS) {
        in_safe_zone_ = in_zone;
        Level* level = Singleton<Application>::s_inst.GetCurrentLevel();
        PlayMusic(level->m_musicSoundId, true, false, kMusicFadeMs);
    } else if (in_zone) {
        const int zone_music = Singleton<Application>::s_inst.GetCurrentLevel()->unk_120;
        if (zone_music < 0) {
            // ASSERT(false) in VoxSoundManager.cpp:1440. The binary writes through a null pointer at assert
            // level 2 (a deliberate fault), which is not reproduced. Level 1 prints the message.
            if (gAssertLevel == 1) {
                std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", "false",
                             "..\\..\\project_vs2005\\Game/..\\..\\sources\\Core\\VOXSoundManager\\VoxSoundManager.cpp",
                             1440);
            }
        } else {
            const bool slot_set = music_slot_;
            in_safe_zone_ = true;
            if (slot_set) {
                PlayMusic(zone_music, true, false, kMusicFadeMs);
            }
        }
    } else {
        in_safe_zone_ = false;
        Level* level = Singleton<Application>::s_inst.GetCurrentLevel();
        PlayMusic(level->m_musicSoundId, true, false, kMusicFadeMs);
    }
}
