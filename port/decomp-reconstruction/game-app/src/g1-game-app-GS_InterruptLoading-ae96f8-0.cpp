// Batch g1-game-app-GS_InterruptLoading-ae96f8-0 (GS_InterruptLoading, part 1 of 1: all 10 functions of the class).
// Rebuilt from the Hex-Rays pseudocode and checked against the assembly (tools/decomp_asm.py) for the constructors,
// the destructors and Ctor. Layouts and declarations are in dh2/GS_InterruptLoading.hpp; the class has no data of its own.
//
// Status of each function is in game-app/ledger.d/g1-game-app-GS_InterruptLoading-ae96f8-0.csv. In short:
//   reconstructed  onEvent, Ctor, both constructors (C1, C2) and both destructors (D1, D2). One definition each.
//   partial        Draw, Update and Dtor. They drive the video driver (the object at IDevice +0x10, which IDevice does
//                  not declare yet), the splash textures, or a sound and online layout that is not in the tree. Each
//                  PARTIAL comment names the missing piece.
//   skipped        the scalar deleting destructor (D0), which the compiler emits.
//
// The original keeps firstframe, accumulatedTime and currentProgress as file-local statics (ELF _ZL10firstframe,
// _ZL15accumulatedTime, _ZL15currentProgress, 4-byte .bss). They are defined in the anonymous namespace below.
#include "core-platform/debug_out.hpp"
#include "dh2/Application.hpp"
#include "dh2/GS_InterruptLoading.hpp"
#include "dh2/SavegameManager.hpp"
#include "dh2/global/ctrl_camera.hpp"  // ApplicationGetInstance(): the Application singleton (Singleton<Application>::s_inst)

// Globals the original reads or writes. Their definitions belong to the globals batch (Width_Screen, JAVA_SOUNDS and
// Resume_Sound_Interrupt are not defined in this tree yet).
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
// PARTIAL. The original draws through the video driver, the object at IDevice +0x10 (Application::m_device->+0x10).
// IDevice declares no +0x10 member yet, so every driver call is missing:
//   firstframe <= 1: driver->vtbl[+0x14](driver, machine); driver->vtbl[+0xa8](driver, 3);
//                    driver->vtbl[+0x18](driver); return driver->vtbl[+0xa4](driver, 0).
//   otherwise:       driver->vtbl[+0x14](driver, machine); glitch::video::C2DDriver::draw2DImage(driver) before the
//                    spinner update, draw2DImage(driver) after it; driver->vtbl[+0x18](driver); return
//                    driver->vtbl[+0xa4](driver, 0).
// The spinner needs only Application::GetDt, so its update is written in full.
void GS_InterruptLoading::Draw(const StateMachine*) {
    if (firstframe <= 1) {
        // PARTIAL: the four driver calls of the first frame are missing (see above).
        return;
    }

    // PARTIAL: driver->vtbl[+0x14](driver, machine) and draw2DImage(driver), the first pass, are missing.

    accumulatedTime += ApplicationGetInstance().GetDt();
    if (static_cast<unsigned int>(accumulatedTime) > 0x19u) {  // more than 25 ms
        const int previous = currentProgress;
        accumulatedTime = 0;
        ++currentProgress;
        if (previous == 3) {
            currentProgress = 0;  // the spinner has four cells
        }
    }

    // PARTIAL: draw2DImage(driver), the second pass, driver->vtbl[+0x18](driver) and the return of
    // driver->vtbl[+0xa4](driver, 0) are missing.
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
// PARTIAL. The original runs three phases, selected by firstframe:
//   firstframe 0:    counts up to 1 (written).
//   firstframe 1:    driver->vtbl[+0x44](driver, machine, LODWORD(dt)) and driver->vtbl[+0x38](driver); then
//                    m_loadBackground = CTextureManager::getTexture(driver->+0xe0, width splash), and for language 4
//                    or 5 again with the language splash; then the texture checks at LABEL_16; then counts up to 2.
//   firstframe > 1:  this = driver->vtbl[+0x3c](driver, machine, LODWORD(dt)). When it returns null the original calls
//                    driver->vtbl[+0x4c](driver) and StateMachine::PopState(Application::m_stateMachine, this as
//                    StateBase*), which pops the splash state.
// The splash file names are written (SplashFileForWidth and LanguageSplashFile). The driver calls, the texture loads and
// the texture checks are missing, so the splash never loads and the state never pops itself yet. The texture checks
// (LABEL_16) read the texture the original stores in m_loadBackground:
//   ITexture +0x2c data pointer (null when the data is not in memory), ITexture +0x3c uint16 texture id (index into the
//   texture manager's collection), and ITexture +0x3f bit 0x08. When the data is present and that bit is set, the
//   virtual at vtbl +0x10 (slot 4) runs. On the no-data path the virtual runs when the collection entry is valid, its
//   name check passes and the bit is set; the path logs "ERROR LOADING" when the entry is invalid or the name check
//   fails. They also log "TEXTURE FILE [%s]" and "NO HAS DATA".
void GS_InterruptLoading::Update(StateMachine*, double) {
    if (firstframe > 1) {
        // PARTIAL: the driver Update and the pop of this state are missing (see above).
        return;
    }
    if (firstframe != 1) {
        ++firstframe;  // frame 0 only counts up (LABEL_6)
        return;
    }

    // Frame 1. The original makes the same calls on every device class. The device flags (SHARP_DEVICES, HTC_DEVICES,
    // MOTOROLA_DEVICES, LG_DEVICES) only pick the stack slot of the temporary texture, so they are not tested here.
    // PARTIAL: driver->vtbl[+0x44](driver, machine, LODWORD(dt)) and driver->vtbl[+0x38](driver) are missing.

    // The width splash is loaded first. Its load assigns m_loadBackground in the original.
    const char* widthSplash = SplashFileForWidth(Width_Screen);
    // PARTIAL: m_loadBackground = CTextureManager::getTexture(driver->+0xe0, widthSplash) is missing. The driver and
    // getTexture are not declared (see GSInit::SwitchBackground for the same gap).
    (void)widthSplash;

    // A Japanese or Korean language then replaces the background with its own splash.
    const char* languageSplash = LanguageSplashFile(ApplicationGetInstance().m_savegameManager->getLanguage());
    // PARTIAL: when languageSplash is not null, m_loadBackground = getTexture(driver->+0xe0, languageSplash) is missing.
    (void)languageSplash;

    // PARTIAL: LABEL_16, the texture checks described above, is missing.

    ++firstframe;  // the original stores v3 + 1, which is 2 here
}

// 0x417fe8 GS_InterruptLoading::Dtor(StateMachine const*)
// PARTIAL. The original logs, resumes the music that the pause path stopped, sets the sound state, clears the pause-menu
// flag, checks the online connection, releases the splash texture and clears InterLoad. Written here: the logs, the
// music flag, Resume_Sound_Interrupt under JAVA_SOUNDS, m_pauseMenuPushed, the texture release and InterLoad. Missing:
//   - nativeResumeMusicBG(<the log call's result>) when Musicplayerpause is set. It is Android JNI glue that core-platform
//     skips (its ledger row), so no call is written.
//   - if (VoxSoundManager::s_instance): when Application::IsInIGPOrGameCenter, or the current level (if any) has
//     m_isInGameView (+0x198), or MenuBase::s_igmOpened, or m_pauseMenuPushed (+0xa5) is set, the original calls
//     ResumeMusic(s_instance, 500) when m_resumeMusic (+0xa6) is set. Otherwise, under JAVA_SOUNDS, it calls
//     nativeSetVolumeBig(-1) twice and VoxSoundManager::Resume(...); without JAVA_SOUNDS it calls
//     ResumeAllSounds(s_instance). VoxSoundManager's instance and methods are not declared in the tree.
//   - the online check: when GetOnline()->isOnlineGame_ (COnline +0x05, private, no getter) is set and either the device
//     timer (IDevice +0x20, vtbl +0x0c) minus Application::m_pauseTime (+0xa0) is over 15000 ms, or OnlineGameState
//     +0x34 is not 2 while IsWifiEnabled() is false, the original sets Application::m_goToMainMenuPending (+0xaa).
void GS_InterruptLoading::Dtor(const StateMachine*) {
    _DEBUG_OUT("\n\n  GS_InterruptLoading::Dtor  ----> unloading splash screen. \n");
    if (Musicplayerpause) {
        _DEBUG_OUT("\n\n  in Resumeeeee    musicccccccccccccc \n");
        // PARTIAL: nativeResumeMusicBG(...) is missing (see above).
        Musicplayerpause = false;
    }
    if (JAVA_SOUNDS) {
        Resume_Sound_Interrupt = true;
    }

    // PARTIAL: the sound block (VoxSoundManager::s_instance) is missing (see above).

    ApplicationGetInstance().m_pauseMenuPushed = false;  // +0xa5, cleared after the sound block

    // PARTIAL: the online check (see above) is missing.

    m_loadBackground = nullptr;  // releases the splash texture; the counted pointer drops it when it was set
    InterLoad = false;
}
