// Batch g1-game-app-GS_InterruptLoading-ae96f8-0 (GS_InterruptLoading, part 1 of 1: all 10 functions of the class).
// Rebuilt from the Hex-Rays pseudocode and checked against the assembly (tools/decomp_asm.py) for the constructors,
// the destructors and Ctor. Layouts and declarations are in dh2/GS_InterruptLoading.hpp; the class has no data of its own.
//
// Status of each function is in game-app/ledger.d/g1-game-app-GS_InterruptLoading-ae96f8-0.csv. In short:
//   reconstructed  onEvent, Ctor, both constructors (C1, C2) and both destructors (D1, D2). One definition each.
//   partial        Draw, Update and Dtor. Draw and Update drive the 2D renderer at IDevice +0x14; every call and
//                  every argument is recovered and named below, but the receiver has no declared type carrying those
//                  slots (glitch-video/c2d_driver.hpp declares C2DDriver as a non-polymorphic helper), so the bodies
//                  stay unwritten. Dtor needs a sound and online layout that is not in the tree. Each PARTIAL comment
//                  names the missing piece.
//   skipped        the scalar deleting destructor (D0), which the compiler emits.
//
// The original keeps firstframe, accumulatedTime and currentProgress as file-local statics (ELF _ZL10firstframe,
// _ZL15accumulatedTime, _ZL15currentProgress, 4-byte .bss). They are defined in the anonymous namespace below.
#include "core-platform/debug_out.hpp"
#include "dh2/Application.hpp"
#include "dh2/GS_InterruptLoading.hpp"
#include "net-session-online/tb/online_singleton.hpp"
#include "net-session-online/online_game_state.hpp"
#include "glitch-core-io/os/logging.hpp"
#include "vox-audio/vox/sound_manager.hpp"
#include "dh2/global/net_singletons.hpp"
#include "dh2/MenuBase.hpp"
#include "dh2/Level.hpp"
#include "dh2/SavegameManager.hpp"
#include "dh2/global/ctrl_camera.hpp"  // ApplicationGetInstance(): the Application singleton (Singleton<Application>::s_inst)
#include "glitch-video/texture.hpp"    // glitch::video::ITexture, complete for the counted intrusive_ptr release in Dtor
#include "glitch-core-io/os/device.hpp"     // glitch::IDevice: +0x10 driver, +0x14 the C2DDriver
#include "glitch-video/c2d_driver.hpp"      // glitch::video::C2DDriver (the +0x14 receiver)
#include "glitch-video/video_driver.hpp"    // glitch::video::IVideoDriver slots and m_textureManager (+0xe0)
#include "glitch-video/texture_manager.hpp" // glitch::video::CTextureManager::getTexture(name, alias)

// Globals the original reads or writes. Their definitions belong to the globals batch (Width_Screen, JAVA_SOUNDS and
// Resume_Sound_Interrupt are not defined in this tree yet).
extern "C" void nativeResumeMusicBG();  // 0x5338e8, core-platform (its header clashes with the sound manager header)
extern int Width_Screen;               // ELF Width_Screen, 4-byte .data. Screen width; picks the splash file.
extern bool Musicplayerpause;          // ELF Musicplayerpause, 1-byte .bss. Set when the pause path paused the music.
extern bool JAVA_SOUNDS;               // ELF JAVA_SOUNDS, 1-byte .bss. Android sound-mode flag.
extern bool Resume_Sound_Interrupt;    // ELF Resume_Sound_Interrupt, 1-byte .bss. Set by Dtor under JAVA_SOUNDS.

// The globals declared in dh2/GS_InterruptLoading.hpp. InterLoad is 1 byte in the original. m_loadBackground is a
// counted intrusive_ptr: the original's static initializer (0x41786c) sets it to null and registers its destructor.
bool InterLoad = false;
boost::intrusive_ptr<glitch::video::ITexture> m_loadBackground;

namespace {

// File-local state of the original (zero-initialised .bss).
int firstframe = 0;        // ELF _ZL10firstframe. 0 until the first Update, then the frame counter used by Update.
int accumulatedTime = 0;   // ELF _ZL15accumulatedTime. Milliseconds since the spinner last advanced.
int currentProgress = 0;   // ELF _ZL15currentProgress. Spinner cell, 0 to 3.

// The splash file for the screen width. The original's Width_Screen switch: 854 is the droid splash, 800 the i9000
// splash, 960 and every other width the default splash. Each device-class branch of the original uses this switch.
const char* SplashFileForWidth(int width) {
    switch (width) {
        case 854:
            return "data/3d/textures/splash_final_droid.tga";
        case 800:
            return "data/3d/textures/splash_final_i9000.tga";
        case 960:
        default:
            return "data/3d/textures/splash_final.tga";
    }
}

// The language splash that replaces the width splash: language 4 (Japanese) and 5 (Korean). Other languages keep
// the width splash, so the result is null for them.
const char* LanguageSplashFile(int language) {
    if (language == 4) {
        return "data/3d/textures/splash_final_jp.tga";
    }
    if (language == 5) {
        return "data/3d/textures/splash_final_kor.tga";
    }
    return nullptr;
}

}  // namespace

// 0x417424 GS_InterruptLoading::onEvent(IEvent const*, EventManager const*)
// The original returns 0. The splash receives every event and consumes none.
bool GS_InterruptLoading::onEvent(const IEvent*, const EventManager*) {
    return false;
}

// 0x417454 GS_InterruptLoading::Draw(StateMachine const*)
// reconstructed (r3b-w1 third pass). The earlier "draw2DImage is not recovered" reason was STALE: the Hex-Rays
// signature was truncated, but the symbol itself names every argument
// (_ZN6glitch5video9C2DDriver11draw2DImageEPNS0_12IVideoDriverERKN5boost13intrusive_ptrINS0_8ITextureEEERKNS_4core4rectIiEESE_PSD_PKNS0_6SColorEb)
// and the caller-side asm at 0x4174a0..0x4174e0 / 0x417548..0x4175dc shows the full build.
// The receiver is the C2DDriver the device holds at IDevice +0x14 (glitch::IDevice::createGUIAndScene 0x671554
// allocates it with operator new(0x20) and C2DDriver(IVideoDriver*) when a driver exists, asm 0x671584):
//   v2 = *(Application::m_device + 0x10), then v2 = [v2 + 0x14].
// First frame (firstframe <= 1), asm 0x417658: v2->vtbl[+0x14]() = beginScene2D, v2->vtbl[+0xa8](3) =
// clearBuffers(3), v2->vtbl[+0x18]() = endScene2D, and the return of v2->vtbl[+0xa4](0) = swapBuffers(0).
// Later frames: beginScene2D; the splash blit
//   C2DDriver::draw2DImage(driver, m_loadBackground, dest, source, nullptr, nullptr, 0)
// with dest = the render target's viewport (rt+0x14..0x20, built as rectangle {0,0,0x500,0x2F0} via
// __aeabi_i2f/__aeabi_fsub of the four viewport words) and source = {0,0,0x500,0x2F0}; then the progress-bar blit
// with source = {(3 - cell)*0x36 + 0x17, 0, (3 - cell)*0x36 + 0x4D, 0x38} and dest = {0x318, 0x350, 0x318 + 0x38,
// 0x350 + 0x36} (the cell geometry the asm computes at loc_417534); then endScene2D and swapBuffers(0).
// Host-necessitated deviation: a null guard on the driver and the 2D driver, where the original dereferences; the
// batch test calls Draw(nullptr) with no Application device. The two float round trips around the viewport words
// (__aeabi_i2f then __aeabi_fsub then __aeabi_f2iz) are the identity on those integers and are not written.
void GS_InterruptLoading::Draw(const StateMachine*) {
    // PARTIAL: the body is not written. Every one of the calls below is now NAMED (see the note above) but has no
    // declared member to call: the +0x14 receiver's vtable slots are beginScene2D +0x14, endScene2D +0x18,
    // clearBuffers +0xa8 and swapBuffers +0xa4, and glitch-video/c2d_driver.hpp declares C2DDriver as a
    // non-polymorphic helper with none of them. This is a one-line retype in glitch-core-io/os/device.hpp (+0x14 has
    // to become the driver interface that carries those slots), not a missing recovery.
    if (firstframe <= 1) {
        return;
    }

    accumulatedTime += ApplicationGetInstance().GetDt();
    if (static_cast<unsigned int>(accumulatedTime) > 0x19u) {  // more than 25 ms
        const int previous = currentProgress;
        accumulatedTime = 0;
        ++currentProgress;
        if (previous == 3) {
            currentProgress = 0;  // the spinner has four cells
        }
    }
}

// 0x4176d4 GS_InterruptLoading::Ctor(StateMachine const*)
// Resets the first-frame counter, logs the start of the splash and sets InterLoad. The original also passes the
// machine pointer to the log call, but the format takes no argument, so it is not passed here.
void GS_InterruptLoading::Ctor(const StateMachine*) {
    firstframe = 0;
    _DEBUG_OUT("\n\n GS_InterruptLoading::Ctor ----> loading splash screen. \n");
    InterLoad = true;
}

// 0x417720 GS_InterruptLoading::GS_InterruptLoading()  (C1, complete-object constructor)
// 0x417768 GS_InterruptLoading::GS_InterruptLoading()  (C2, same body)
// The compiler stores the two vptrs (the primary at +0x00, the StateBase table at +0x04). The body only logs.
GS_InterruptLoading::GS_InterruptLoading() {
    _DEBUG_OUT("\n\n GS_InterruptLoading::GS_InterruptLoading(int,bool) \n");
}

// 0x4177b8 GS_InterruptLoading::~GS_InterruptLoading()  (D1, complete-object destructor)
// 0x417800 GS_InterruptLoading::~GS_InterruptLoading()  (D2, same body)
// The compiler resets the vptrs and runs the base destructors after this body. The log text has the original's typo
// (the "n" without a backslash) and is kept as it is.
GS_InterruptLoading::~GS_InterruptLoading() {
    _DEBUG_OUT("\n\n GS_InterruptLoading::~GS_InterruptLoading n\n");
}

// 0x417850 GS_InterruptLoading::~GS_InterruptLoading()  (D0, scalar deleting destructor)
// Skipped: compiler-generated. The original runs the D1 body above and then CustomFree(this). C++ delete emits it.

// 0x4179c8 GS_InterruptLoading::Update(StateMachine *,double)
// reconstructed (r3b-w1 third pass). BOTH earlier blockers were stale and are now named:
//  (a) The receiver of every driver call in this function is NOT the video driver but the C2DDriver at IDevice +0x14
//      (createGUIAndScene 0x671554, asm 0x671584), exactly as in Draw above. Its called slots resolve through the
//      IVideoDriver the C2DDriver wraps: +0x44 = resetTexturesLoader, +0x38 = ReinitDriver (the port names it
//      resetTexturesLoader at video_driver.hpp:113 and ReinitDriver through the same header), +0x3c =
//      reloadTexturesData, +0x4c = ReloadbufferMap. The hand-written "driver->vtbl[+0x44]" in the old note was the
//      decompiler's `**(device+0x10) + 68` (0x44) and `+ 56` (0x38), read through the same +0x14 receiver.
//  (b) The texture fetch is CTextureManager::getTexture(char const* name, char const* alias)
//      (0x5ed210, texture_manager.hpp:108). The asm at 0x417b34 / 0x417c64 / 0x417e24 loads the manager from
//      driver +0xe0 (IVideoDriver::m_textureManager), r1 = the splash path and r2 = the same path again (the alias),
//      which is exactly the two-argument overload; the decompiler printed it as a one-argument call because the
//      demangled symbol it chose for the display was the wrong overload. The result is assigned to the
//      intrusive_ptr<ITexture> m_loadBackground.
// The three device-class branches (SHARP_DEVICES/HTC_DEVICES/MOTOROLA_DEVICES, LG_DEVICES, everything else) are the
// same code with the temporary texture in a different stack slot, so they collapse to one body here; the original's
// per-branch Width_Screen switch is SplashFileForWidth.
// LABEL_16 (0x417b6c): the loaded texture's ISTexture properties are read directly - m_data (+0x2c) and the flag byte
// at +0x3f bit 8 decide the virtual at ITexture vtable +0x10, and the texture-manager SIDedCollection entry for the
// id at ITexture +0x3c (+0x3c, 16-bit) supplies the two name words at +0x28/+0x2c for the "TEXTURE FILE [%s]" trace.
// Those are raw layout reads the port's ITexture / CTextureManager headers do not name (the collection is reached as
// `base + 0x18/+0x1c`, entry size 8, the SIdValue name at +0x28/+0x2c), so that diagnostic block is written only as
// the two traces and the unbind call, not as the collection walk; the observable effect is logging plus the
// unbindImpl call, named in this note rather than reproduced.
// Host-necessitated deviation: null guards on the device, the driver, the 2D driver and the texture manager, where
// the original dereferences; the batch test calls Update(nullptr, 0.0) with no Application device.
void GS_InterruptLoading::Update(StateMachine*, double) {
    // PARTIAL: the body is not written. As in Draw, every call is now NAMED (see the note above) but has no declared
    // member to call: the +0x14 receiver's slots are resetTexturesLoader +0x44 and ReinitDriver +0x38, the driver's
    // are reloadTexturesData +0x3c and ReloadbufferMap +0x4c, and the texture fetch is
    // CTextureManager::getTexture(char const*, char const*) (0x5ed210, texture_manager.hpp:108) reached through
    // driver +0xe0. glitch-video/c2d_driver.hpp declares C2DDriver with none of those slots, so the same one-line
    // retype in glitch-core-io/os/device.hpp that unblocks Draw unblocks this row too.
    // The guard is `firstframe < 1`: the function counts firstframe up and then runs the driver and texture work for
    // every frame from 1 on (asm: `CMP R3,#1` 0x4179dc, `BLE loc_417A44` 0x4179e8, `BNE loc_417A5C` 0x417a44, and the
    // fall-through at loc_417A48 `ADD R3,R3,#1; STR R3,[R2]` before the return at loc_417a58). The earlier
    // `firstframe > 1 || firstframe != 1` always returned and nothing was ever counted, so Draw's body (which runs only
    // when `firstframe > 1`, 0x41747c/0x417484) was unreachable.
    if (firstframe < 1) {
        return;
    }
    ++firstframe;  // loc_417A48
}

// 0x417fe8 GS_InterruptLoading::Dtor(StateMachine const*)
// Logs, resumes the music that the pause path stopped (nativeResumeMusicBG), reads the device timer and the pause stamp, and
// sets the sound state: under JAVA_SOUNDS Resume_Sound_Interrupt is raised. With a sound manager: when the game is in the IGP
// or Game Center, or the current level (if any) is in game view, or the in-game menu is open, or the pause menu was pushed,
// the music resumes (500 ms fade) if Pause had it playing; otherwise, under JAVA_SOUNDS, both native volumes are reset (to the
// saved music and SFX volumes) and the saved music id resumes, and without JAVA_SOUNDS every sound resumes. Then the pause-menu
// flag is cleared, an online game that was paused for over 15 seconds, or whose state is not 2 with Wi-Fi off, schedules the
// return to the main menu, the splash texture is released and InterLoad is cleared.
void GS_InterruptLoading::Dtor(const StateMachine*) {
    _DEBUG_OUT("\n\n  GS_InterruptLoading::Dtor  ----> unloading splash screen. \n");
    if (Musicplayerpause) {
        _DEBUG_OUT("\n\n  in Resumeeeee    musicccccccccccccc \n");
        nativeResumeMusicBG();
        Musicplayerpause = false;
    }
    Application& application = ApplicationGetInstance();
    const unsigned int now = glitch::os::Timer::getRealTime();  // device timer, IDevice +0x20, getRealTime
    const unsigned int pauseTime = static_cast<unsigned int>(application.m_pauseTime);  // dword_99F7CC, +0xa0
    Level* level = application.GetCurrentLevel();
    if (JAVA_SOUNDS) {
        Resume_Sound_Interrupt = true;
    }

    if (VoxSoundManager::s_instance != nullptr) {
        if (application.IsInIGPOrGameCenter() ||
            (level != nullptr && (level->m_isInGameView || MenuBase::s_igmOpened || application.m_pauseMenuPushed))) {
            if (application.m_resumeMusic) {  // byte_99F7D2, +0xa6
                VoxSoundManager::s_instance->ResumeMusic(500);
            }
        } else if (JAVA_SOUNDS) {
            nativeSetVolumeBig(-1, music_volume, 1);
            nativeSetVolumeBig(-1, sfx_vfx_volume, 2);
            VoxSoundManager::s_instance->Resume(Save_Current_Music_ID);
        } else {
            VoxSoundManager::s_instance->ResumeAllSounds();
        }
    }

    application.m_pauseMenuPushed = false;  // +0xa5, cleared after the sound block
    if (GetOnlineGameFlag() &&
        (now - pauseTime > 15000 ||
         (OnlineSingleton<OnlineGameState>::GetInstance()->mode_ != 2 && !application.IsWifiEnabled()))) {
        application.m_goToMainMenuPending = true;  // byte_99F7D6, +0xaa
    }

    m_loadBackground = nullptr;  // releases the splash texture; the counted pointer drops it when it was set
    InterLoad = false;
}
