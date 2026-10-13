// Tests for the VoxSoundManager bodies of batch p1-vox-audio-VoxSoundManager-3a5f5c-0.
// The engine facade, the sound pack, the file loader, the native bridge and the game globals are
// stubs here that record their calls, so the test runs without the rest of the engine.
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <new>
#include <string>
#include <vector>

#include "vox-audio/vox/engine_handles.hpp"
#include "vox-audio/vox/memory.hpp"
#include "vox-audio/vox/sound_manager.hpp"
#include "vox-audio/vox/voxengine.hpp"
#include "vox-audio/vox/voxutils.hpp"

// ---- Game globals (defined by the globals batches in the game build).
bool JAVA_SOUNDS = false;
bool Resume_Sound_Interrupt = false;
int Save_Current_Music_ID = -1;
float music_volume = 0.0f;
float sfx_vfx_volume = 0.0f;
const char* RES_PATH = "./";

namespace {

int g_failures = 0;

#define CHECK(cond)                                                  \
    do {                                                             \
        if (!(cond)) {                                               \
            std::fprintf(stderr, "%s:%d: CHECK failed: %s\n", __FILE__, \
                         __LINE__, #cond);                           \
            ++g_failures;                                            \
        }                                                            \
    } while (0)

struct Call {
    std::string name;
    std::vector<double> args;
};

std::vector<Call> g_calls;
int g_destroyed = 0;
std::string g_last_path;
int g_last_format = -1;
int g_last_flags = -1;
int g_last_user = -1;

void Rec(const char* name, std::initializer_list<double> args = {}) {
    g_calls.push_back(Call{name, std::vector<double>(args)});
}

size_t CountCalls(const char* name) {
    size_t n = 0;
    for (const Call& c : g_calls) {
        if (c.name == name) {
            ++n;
        }
    }
    return n;
}

// True when a call with this name and these arguments was recorded (floats within 1e-4).
bool HasCall(const char* name, std::initializer_list<double> args) {
    for (const Call& c : g_calls) {
        if (c.name != name || c.args.size() != args.size()) {
            continue;
        }
        bool same = true;
        size_t i = 0;
        for (double a : args) {
            if (std::fabs(c.args[i++] - a) > 1e-4) {
                same = false;
                break;
            }
        }
        if (same) {
            return true;
        }
    }
    return false;
}

bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

// Group gains the stubs report: MUSIC mask 4, VFX mask 2, SFX mask 1, anything else 0.
float GainFor(int group) {
    switch (group) {
        case 4: return 0.5f;
        case 2: return 0.25f;
        case 1: return 0.125f;
        default: return 0.0f;
    }
}

void ResetState() {
    g_calls.clear();
    g_destroyed = 0;
    g_last_path.clear();
    JAVA_SOUNDS = false;
    Resume_Sound_Interrupt = false;
    Save_Current_Music_ID = -1;
    music_volume = 0.0f;
    sfx_vfx_volume = 0.0f;
}

// A DataHandle allocated the way LoadSound allocates one, so UnloadSound can free it.
vox::DataHandle* MakeHandle() {
    void* mem = VoxAlloc(sizeof(vox::DataHandle), vox::kVoxMemHintDefault);
    return new (mem) vox::DataHandle(1, nullptr, nullptr, 0, 0);
}

void TestUnloadSound() {
    ResetState();
    VoxSoundManager m;
    m.sound_count_ = 3;
    m.sounds_ = new vox::DataHandle*[3]{};
    m.sounds_[0] = MakeHandle();
    m.sounds_[2] = MakeHandle();

    m.UnloadSound(-1);
    m.UnloadSound(3);
    CHECK(g_destroyed == 0);
    m.UnloadSound(1);  // empty slot
    CHECK(g_destroyed == 0);
    m.UnloadSound(2);
    CHECK(g_destroyed == 1);
    CHECK(m.sounds_[2] == nullptr);
    CHECK(m.sounds_[0] != nullptr);

    JAVA_SOUNDS = true;
    m.UnloadSound(0);
    CHECK(g_destroyed == 1);
    CHECK(m.sounds_[0] != nullptr);
    JAVA_SOUNDS = false;

    m.UnloadAllSounds();
    CHECK(g_destroyed == 2);
    CHECK(m.sounds_[0] == nullptr);
    delete[] m.sounds_;
}

void TestLoadSound() {
    ResetState();
    VoxSoundManager m;
    m.sound_count_ = 2;
    m.sounds_ = new vox::DataHandle*[2]{};

    m.LoadSound(1);
    CHECK(m.sounds_[1] != nullptr);
    CHECK(g_last_path == "./data/sounds/snd.ogg");
    CHECK(g_last_format == 3);
    CHECK(g_last_flags == 5);
    CHECK(g_last_user == 77);
    CHECK(CountCalls("LoadDataSourceFromFileEx") == 1);

    // A loaded slot is not loaded again.
    m.LoadSound(1);
    CHECK(CountCalls("LoadDataSourceFromFileEx") == 1);
    // Out-of-range ids load nothing.
    m.LoadSound(-1);
    m.LoadSound(3);
    CHECK(CountCalls("LoadDataSourceFromFileEx") == 1);

    m.UnloadAllSounds();
    CHECK(g_destroyed == 1);
    delete[] m.sounds_;
}

void TestVolumes() {
    ResetState();
    VoxSoundManager m;
    CHECK(Near(m.GetSoundVolume(2), 50.0f));  // MUSIC: gain 0.5 * 100
    CHECK(Near(m.GetSoundVolume(3), 25.0f));  // VFX: gain 0.25 * 100
    CHECK(Near(m.GetSoundVolume(1), 12.5f));  // SFX: gain 0.125 * 100
    CHECK(Near(m.GetSoundVolume(0), 0.0f));   // other types read group 0
    CHECK(Near(m.GetMasterVolume(), 0.75f));
    CHECK(HasCall("GetGroupGain", {4}));
    CHECK(HasCall("GetGroupGain", {2}));
    CHECK(HasCall("GetGroupGain", {1}));

    JAVA_SOUNDS = true;
    CHECK(Near(m.GetSoundVolume(2), 1.0f));
    CHECK(Near(m.GetMasterVolume(), 1.0f));
    JAVA_SOUNDS = false;
}

void TestSetInitialVolume() {
    ResetState();
    VoxSoundManager m;
    m.SetInitialVolume(80.0f, 60.0f, 40.0f);
    CHECK(HasCall("SetGroupGain", {4, 0.8, 0.05}));  // MUSIC
    CHECK(HasCall("SetGroupGain", {1, 0.6, 0.05}));  // SFX
    CHECK(HasCall("SetGroupGain", {2, 0.4, 0.05}));  // VFX
    CHECK(m.initial_volume_set_);

    const size_t before = g_calls.size();
    m.SetInitialVolume(10.0f, 10.0f, 10.0f);
    CHECK(g_calls.size() == before);  // runs once
}

void TestSetInitialVolumeJava() {
    ResetState();
    VoxSoundManager m;
    m.music_id_ = 5;
    JAVA_SOUNDS = true;
    m.SetInitialVolume(80.0f, 60.0f, 40.0f);
    CHECK(Near(music_volume, 80.0f));
    CHECK(Near(sfx_vfx_volume, 60.0f));
    CHECK(HasCall("nativeSetVolumeBig", {5, 80, 1}));
    CHECK(HasCall("nativeSetVolumeBig", {5, 60, 2}));
    CHECK(CountCalls("SetGroupGain") == 0);
    CHECK(!m.initial_volume_set_);
    JAVA_SOUNDS = false;
}

void TestSetSoundVolume() {
    ResetState();
    VoxSoundManager m;
    m.SetSoundVolume(2, 30.0f);
    CHECK(HasCall("SetGroupGain", {4, 0.3, 0.05}));
    m.SetSoundVolume(1, 30.0f);
    CHECK(HasCall("SetGroupGain", {1, 0.3, 0.05}));
    m.SetSoundVolume(3, 30.0f);
    CHECK(HasCall("SetGroupGain", {2, 0.3, 0.05}));
    m.SetSoundVolume(9, 30.0f);
    CHECK(HasCall("SetGroupGain", {0, 0.3, 0.05}));

    ResetState();
    m.music_id_ = 5;
    JAVA_SOUNDS = true;
    m.SetSoundVolume(2, 30.0f);
    CHECK(Near(music_volume, 30.0f));
    CHECK(HasCall("nativeSetVolumeBig", {5, 30, 1}));
    m.SetSoundVolume(1, 40.0f);
    CHECK(Near(sfx_vfx_volume, 40.0f));
    CHECK(HasCall("nativeSetVolumeBig", {5, 40, 2}));
    const size_t before = g_calls.size();
    m.SetSoundVolume(7, 1.0f);  // no branch for this type
    CHECK(g_calls.size() == before);
    JAVA_SOUNDS = false;
}

void TestMasterVolume() {
    ResetState();
    VoxSoundManager m;
    m.SetMasterVolume(0.5f);
    CHECK(HasCall("SetMasterGain", {0.5, 0.05}));
    JAVA_SOUNDS = true;
    const size_t before = g_calls.size();
    m.SetMasterVolume(0.25f);
    CHECK(g_calls.size() == before);
    JAVA_SOUNDS = false;
}

void TestListenerPos() {
    ResetState();
    VoxSoundManager m;
    glitch::core::vector3d<float> pos{1.0f, 2.0f, 3.0f};
    glitch::core::vector3d<float> front{0.0f, 0.0f, 1.0f};
    glitch::core::vector3d<float> up{0.0f, 1.0f, 0.0f};
    m.SetListenerPos(pos, front, up, 11, 22, 0.5f);
    CHECK(HasCall("Set3DListenerPosition", {1, 2, 3}));
    CHECK(HasCall("Set3DListenerOrientation", {0, 0, 1, 0, 1, 0}));
    CHECK(m.unk_54 == 11);
    CHECK(m.unk_58 == 22);
    CHECK(Near(m.unk_5c, 0.5f));

    JAVA_SOUNDS = true;
    const size_t before = g_calls.size();
    m.SetListenerPos(pos, front, up, 1, 2, 3.0f);
    CHECK(g_calls.size() == before);
    CHECK(m.unk_54 == 11);
    JAVA_SOUNDS = false;
}

void TestStopAllMusicAndState() {
    ResetState();
    VoxSoundManager m;
    m.music_id_ = 5;
    m.StopAllMusic(250);
    CHECK(HasCall("StopAllEmitters", {4, 250}));
    CHECK(m.music_id_ == -1);
    CHECK(m.prev_music_id_ == 5);

    ResetState();
    m.music_id_ = 6;
    JAVA_SOUNDS = true;
    m.StopAllMusic(250);
    CHECK(HasCall("nativeStopAllBigSound", {-1, 250}));
    CHECK(m.music_id_ == -1);
    CHECK(m.prev_music_id_ == 6);
    JAVA_SOUNDS = false;
}

void TestStopAllSoundsPauseResume() {
    ResetState();
    VoxSoundManager m;
    // Without an engine (engine_ null) the facade calls are skipped.
    m.StopAllSounds(500);
    m.PauseAllSounds();
    m.ResumeAllSounds();
    CHECK(g_calls.empty());

    static int dummy_engine = 0;
    m.engine_ = reinterpret_cast<vox::VoxEngine*>(&dummy_engine);
    m.StopAllSounds(500);
    CHECK(HasCall("StopAllEmitters", {4294967295.0, 0.5}));
    m.PauseAllSounds();
    CHECK(HasCall("PauseAllEmitters", {4294967295.0, 0.2}));
    m.ResumeAllSounds();
    CHECK(HasCall("ResumeAllEmitters", {4294967295.0, 0.2}));

    ResetState();
    JAVA_SOUNDS = true;
    m.StopAllSounds(500);
    CHECK(HasCall("nativeStopAllBigSound", {-1, 500}));
    JAVA_SOUNDS = false;
}

void TestResumeAllSoundsJava() {
    ResetState();
    VoxSoundManager m;
    JAVA_SOUNDS = true;
    Resume_Sound_Interrupt = true;
    Save_Current_Music_ID = 7;
    m.ResumeAllSounds();
    CHECK(HasCall("nativeResumeSoundBig", {9999}));
    CHECK(HasCall("nativeResumeSoundBig", {7}));
    CHECK(!Resume_Sound_Interrupt);

    ResetState();
    JAVA_SOUNDS = true;
    Resume_Sound_Interrupt = true;
    Save_Current_Music_ID = -1;
    m.ResumeAllSounds();
    CHECK(CountCalls("nativeResumeSoundBig") == 1);  // only the all-sounds resume
    CHECK(!Resume_Sound_Interrupt);
    JAVA_SOUNDS = false;
}

void TestStopMusic() {
    ResetState();
    VoxSoundManager m;
    m.StopMusic(100);  // no music playing: nothing happens
    CHECK(g_calls.empty());
    CHECK(m.music_id_ == -1);

    JAVA_SOUNDS = true;
    m.music_id_ = 5;
    m.StopMusic(100);
    CHECK(HasCall("nativeStopSoundBig", {5}));
    CHECK(Save_Current_Music_ID == -1);
    CHECK(m.music_id_ == -1);
    CHECK(m.prev_music_id_ == 5);
    JAVA_SOUNDS = false;

    // The non-Java branch updates the music ids; the partial Stop(int, int) stops nothing yet.
    ResetState();
    m.music_id_ = 8;
    m.StopMusic(100);
    CHECK(m.music_id_ == -1);
    CHECK(m.prev_music_id_ == 8);
}

void TestJavaForwarders() {
    ResetState();
    VoxSoundManager m;
    JAVA_SOUNDS = true;
    m.Stop(4, 100);
    CHECK(HasCall("nativeStopSoundBig", {4}));
    m.Stop3D(4, 100, nullptr, 1.0f);
    CHECK(CountCalls("nativeStopSoundBig") == 2);
    m.Resume(4);
    CHECK(HasCall("nativeResumeSoundBig", {4}));
    CHECK(m.IsSoundPlaying(4));
    CHECK(HasCall("nativeIsMediaPlaying", {4}));
    CHECK(!m.IsSoundPlaying(-1));
    JAVA_SOUNDS = false;
}

}  // namespace

// ---- Stubs for the symbols the body links against. They record their arguments.

extern "C" int nativeStopSoundBig(int sound_id) {
    Rec("nativeStopSoundBig", {static_cast<double>(sound_id)});
    return 0;
}

extern "C" int nativeResumeSoundBig(int sound_id) {
    Rec("nativeResumeSoundBig", {static_cast<double>(sound_id)});
    return 0;
}

extern "C" int nativeStopAllBigSound(int sound_id, int fade_ms) {
    Rec("nativeStopAllBigSound", {static_cast<double>(sound_id), static_cast<double>(fade_ms)});
    return 0;
}

extern "C" int nativeSetVolumeBig(int sound_id, float volume, int channel) {
    Rec("nativeSetVolumeBig", {static_cast<double>(sound_id), static_cast<double>(volume),
                               static_cast<double>(channel)});
    return 0;
}

extern "C" int nativeIsMediaPlaying(int sound_id) {
    Rec("nativeIsMediaPlaying", {static_cast<double>(sound_id)});
    return 1;
}

void* VoxAlloc(std::size_t size, vox::VoxMemHint /*hint*/) { return std::malloc(size); }

void VoxFree(void* ptr) { std::free(ptr); }

namespace vox {

float VoxEngine::GetGroupGain(int group) {
    Rec("GetGroupGain", {static_cast<double>(group)});
    return GainFor(group);
}

int VoxEngine::SetGroupGain(unsigned int group, float gain, float time) {
    Rec("SetGroupGain", {static_cast<double>(group), static_cast<double>(gain),
                         static_cast<double>(time)});
    return 0;
}

float VoxEngine::GetMasterGain() {
    Rec("GetMasterGain");
    return 0.75f;
}

int VoxEngine::SetMasterGain(float gain, float time) {
    Rec("SetMasterGain", {static_cast<double>(gain), static_cast<double>(time)});
    return 0;
}

void VoxEngine::StopAllEmitters(unsigned int group, float fade) {
    Rec("StopAllEmitters", {static_cast<double>(group), static_cast<double>(fade)});
}

void VoxEngine::ResumeAllEmitters(unsigned int group, float fade) {
    Rec("ResumeAllEmitters", {static_cast<double>(group), static_cast<double>(fade)});
}

void VoxEngine::PauseAllEmitters(unsigned int group, float fade) {
    Rec("PauseAllEmitters", {static_cast<double>(group), static_cast<double>(fade)});
}

void VoxEngine::Set3DListenerPosition(float x, float y, float z) {
    Rec("Set3DListenerPosition", {static_cast<double>(x), static_cast<double>(y),
                                  static_cast<double>(z)});
}

void VoxEngine::Set3DListenerOrientation(float fx, float fy, float fz, float ux, float uy,
                                         float uz) {
    Rec("Set3DListenerOrientation", {static_cast<double>(fx), static_cast<double>(fy),
                                     static_cast<double>(fz), static_cast<double>(ux),
                                     static_cast<double>(uy), static_cast<double>(uz)});
}

void VoxSoundPackXML::GetGroupMask(const char* name, int& mask) const {
    if (std::strcmp(name, "MUSIC") == 0) {
        mask = 4;
    } else if (std::strcmp(name, "VFX") == 0) {
        mask = 2;
    } else if (std::strcmp(name, "SFX") == 0) {
        mask = 1;
    } else {
        mask = 0;
    }
}

int VoxSoundPackXML::GetDataSourceInfo(int index, const char*& name, FormatTypes& format,
                                       int& field_16, int& field_15,
                                       VoxSourceLoadingFlags& flags) const {
    Rec("GetDataSourceInfo", {static_cast<double>(index)});
    name = "snd.ogg";
    format = static_cast<FormatTypes>(3);
    field_16 = 77;
    field_15 = 0;
    flags = static_cast<VoxSourceLoadingFlags>(5);
    return 1;
}

VoxSoundPackXML::~VoxSoundPackXML() {}

DataHandle VoxUtils::LoadDataSourceFromFileEx(const char* path, int format, int load_flags,
                                              int user) {
    Rec("LoadDataSourceFromFileEx");
    g_last_path = path;
    g_last_format = format;
    g_last_flags = load_flags;
    g_last_user = user;
    return DataHandle(7, nullptr, nullptr, 0, 0);
}

DataHandle::DataHandle(std::int64_t id, VoxEngineInternal** engine, Handlable* object,
                       std::uint32_t first_stamp, std::uint32_t second_stamp) {
    (void)id;
    (void)engine;
    (void)object;
    (void)first_stamp;
    (void)second_stamp;
}

DataHandle::~DataHandle() { ++g_destroyed; }

Handle::~Handle() {}

}  // namespace vox

// The destructor's body is in batch p1-vox-audio-VoxSoundManager-3a5f5c-1.cpp, which this test does not link.
// The test's stack objects only need the members destroyed, so an empty body is enough here.
VoxSoundManager::~VoxSoundManager() {}

int main() {
    TestUnloadSound();
    TestLoadSound();
    TestVolumes();
    TestSetInitialVolume();
    TestSetInitialVolumeJava();
    TestSetSoundVolume();
    TestMasterVolume();
    TestListenerPos();
    TestStopAllMusicAndState();
    TestStopAllSoundsPauseResume();
    TestResumeAllSoundsJava();
    TestStopMusic();
    TestJavaForwarders();
    if (g_failures == 0) {
        std::printf("p1-vox-audio-VoxSoundManager-3a5f5c-0: all checks passed\n");
    }
    return g_failures == 0 ? 0 : 1;
}
