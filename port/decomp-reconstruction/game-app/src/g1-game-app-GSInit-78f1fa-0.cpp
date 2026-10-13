// Batch g1-game-app-GSInit-78f1fa-0 (GSInit, part 1 of 1: all 11 functions of the class).
// Rebuilt from the Hex-Rays pseudocode. Member offsets are the original 32-bit ones (dh2/GSInit.hpp).
//
// Status of each function is in game-app/ledger.d/g1-game-app-GSInit-78f1fa-0.csv. In short:
//   reconstructed  the constructors, the destructor copies that are not deleting, Dtor and Ctor.
//   partial        Draw, ClearLoadingScreen, SwitchBackground and Update. They need the video driver
//                  (the object at IDevice +0x10, whose texture manager is at +0xe0), the device timer
//                  (IDevice +0x20), or a class header that is not in the tree yet. Each PARTIAL comment
//                  names the missing piece.
//   skipped        the deleting destructor (D0), which the compiler emits.
//
// Globals the original reads. Their definitions belong to the globals batch.
// The unnamed IDA globals (dword_99F7F8, dword_99F7FC, byte_99F819, byte_99FC31, dword_9A247C, dword_9A24C0)
// have no symbol in the ELF table and are not declared here; the PARTIAL comments name them.
#include <cstdlib>

#include "core-platform/debug_out.hpp"
#include "core-platform/device.hpp"
#include "dh2/Application.hpp"
#include "dh2/GSFlashMenu.hpp"
#include "dh2/GSInit.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/Random.hpp"
#include "dh2/SavegameManager.hpp"
#include "dh2/StateMachine.hpp"
#include "dh2/StringManager.hpp"
#include "dh2/TrophyManager.hpp"
#include "dh2/global/ctrl_camera.hpp"
#include "glitch-core-io/os/logging.hpp"
#include "pydata-accessors/w7_pydata12.hpp"

extern bool videoDone;               // ELF videoDone, 1-byte .bss. Set by the intro movie callback.
extern int Width_Screen;             // ELF Width_Screen, 4-byte .data. Screen width, chooses the splash file.
extern bool USE_NATIVE_DRM_GAME;     // ELF USE_NATIVE_DRM_GAME, 1-byte .bss.

// ELF nativeLoadMovie (C linkage, no header in the tree). The prototype is taken from its call sites:
// a movie file name and the language index. The original does not use a result.
extern "C" void nativeLoadMovie(const char* movie, int language);

// 0x3847fc GSInit::GSInit()
GSInit::GSInit() {
    m_step = 0;                      // +0x04 loading step, 0 at the start
    unk_08 = -1;                     // +0x08 spinner state: negative while the spinner runs
    // +0x10 m_splashTexture starts null (the original stores 0 there).
}

// 0x384834 GSInit::GSInit()
// C2 copy of 0x3847fc with the same body. The one definition above serves both addresses.

// 0x38486c GSInit::Dtor(StateMachine const*)
// The original body is empty (asm: BX LR).
void GSInit::Dtor(const StateMachine*) {}

// 0x384a4c GSInit::Draw(StateMachine const*)
// PARTIAL. The original begins the frame on the video driver, which is the object at IDevice +0x10. IDevice
// has no declared +0x10 member yet, so every driver call is missing here. The calls the original makes are:
//   driver->vtbl[+0x0c](driver, machine); driver->vtbl[+0xa8](driver, 1); driver->vtbl[+0x14](driver);
//   ... driver->vtbl[+0x18](driver); driver->vtbl[+0x10](driver); return driver->vtbl[+0xa4](driver, 0);
// The spinner's timing and cell positions do not need the driver, so they are written in full.
void GSInit::Draw(const StateMachine*) {
    if (m_splashTexture) {
        // PARTIAL: the full-screen splash draw is missing: draw2DImage(driver, &m_splashTexture, screenRect,
        // {0, 0, 1280, 752}, 0, 0, 0). screenRect is the viewport rect at *(driver +0xc8) + 0x14, not declared.
        if (unk_08 < 0) {
            // Function-local statics in the original (GSInit::Draw(...)::accumulatedTime and ::currentProgress).
            static int accumulatedTime = 0;
            static int currentProgress = 0;

            accumulatedTime += ApplicationGetInstance().GetDt();
            if (static_cast<unsigned int>(accumulatedTime) > 0x19u) {
                accumulatedTime = 0;
                currentProgress = (currentProgress == 12) ? 0 : currentProgress + 1;
            }

            // The original asks Device::IsIphoneHighRez() four times, in this order. The answers pick the origin
            // and the size of one spinner cell in the splash texture.
            const int originX = Device::IsIphoneHighRez() ? 23 : 770;   // first answer
            const int originY = Device::IsIphoneHighRez() ? 792 : 770;  // second answer
            const int cellW = Device::IsIphoneHighRez() ? 54 : 64;      // third answer
            const int cellH = Device::IsIphoneHighRez() ? 56 : 64;      // fourth answer

            // Column is currentProgress % 4; row is the adjusted frame >> 2 (the adjust only matters for negative
            // progress, which the original never reaches).
            int frame = currentProgress;
            if (currentProgress < 0) {
                frame = currentProgress + 3;
            }
            const int srcLeft = originX + (currentProgress % 4) * cellW;
            const int srcTop = originY + (frame >> 2) * cellH;
            const int srcRight = srcLeft + cellW;
            const int srcBottom = srcTop + cellH;
            (void)srcRight;
            (void)srcBottom;

            // PARTIAL: the spinner draw is missing: draw2DImage(driver, &m_splashTexture, destRect, srcRect, 0, 0, 0).
            // destRect is the bottom-right corner [W - cellW, H - cellH, W, H] of the viewport rect (driver +0xc8),
            // and srcRect is {srcLeft, srcTop, srcRight, srcBottom}.
            (void)srcLeft;
            (void)srcTop;
        }
    }
}

// 0x384db8 GSInit::~GSInit()
// 0x384e58 GSInit::~GSInit()
// The D2 and D1 copies have the same body. The original sets the vtable to GSInit's, then releases
// m_splashTexture (+0x10) when it is set. The intrusive_ptr member does that release when the members are
// destroyed after this body, through the same drop() call.
GSInit::~GSInit() {}

// 0x384e98 GSInit::~GSInit() (deleting destructor D0)
// Skipped: compiler-generated. It runs the D1 body above and then CustomFree(this). C++ delete emits it.

// 0x384e30 GSInit::Ctor(StateMachine const*)
// The original returns this (or the drop() result), and StateBase's slot is void, so the return is dropped.
void GSInit::Ctor(const StateMachine*) {
    m_splashTexture = nullptr;       // +0x10 cleared, then the old texture (if any) is released
    m_step = 0;                      // +0x04
    unk_08 = -1;                     // +0x08
}

// 0x384ef0 GSInit::ClearLoadingScreen(void)
// PARTIAL. The original logs first. When the splash texture is set it calls
// glitch::video::CTextureManager::removeTexture(manager, &m_splashTexture), with manager = *(driver +0xe0) and
// driver = *(IDevice +0x10). Neither is declared yet, so that removal is missing and the texture is released only here.
void GSInit::ClearLoadingScreen() {
    _DEBUG_OUT("----------------ClearLoadingScreen");
    if (m_splashTexture) {
        // PARTIAL: removeTexture(manager, m_splashTexture) is not called (see above).
        m_splashTexture = nullptr;   // +0x10 released
        unk_08 = -1;                 // +0x08
    }
}

// 0x384f64 GSInit::SwitchBackground(void)
// PARTIAL. Clears the loading screen on the static instance (GSInit::s_inst, not this) and picks the splash file
// for the language. Language 5 (Korean) gets splash_final_kor.tga; the other languages get splash_final.tga (the
// Japanese branch of the original loads the same file). The original then calls
// glitch::video::CTextureManager::getTexture(manager, file) with manager = *(driver +0xe0), and stores the result
// in the global dword_9A24C0 as a counted pointer (the new texture gets a use, the old one is dropped). getTexture
// is not declared in glitch-video/texture_manager.hpp and the driver is not declared, so that step is missing.
void GSInit::SwitchBackground() {
    GSInit::s_inst.ClearLoadingScreen();
    const int language = ApplicationGetInstance().m_savegameManager->getLanguage();
    const char* textureFile = (language == 5) ? "data/3d/textures/splash_final_kor.tga"
                                              : "data/3d/textures/splash_final.tga";
    (void)textureFile;
}

// 0x3850c8 GSInit::Update(StateMachine *,double)
// PARTIAL. One loading step per call, selected by m_step (+0x04). A step that advances ends with ++m_step, as the
// original's LABEL_22 does. A step that the original repeats until a timer runs out is tried once per call here, and
// returns without advancing until it succeeds. Each PARTIAL comment names what is missing.
void GSInit::Update(StateMachine*, double) {
    Application& app = ApplicationGetInstance();

    switch (m_step) {
        case 0:
            // PARTIAL: VoxSoundManager::CreateInstance(this) is missing. VoxSoundManager is not declared in the tree.
            ++m_step;
            return;

        case 1:
            app.m_savegameManager->loadSettings(true);
            ++m_step;
            return;

        case 2:
            // PARTIAL: the original loops PyDataConstants::Load() on Application::m_pyDataConstants (+0x2c) until it
            // returns nonzero, or until 50 device-timer ticks pass (the timer at IDevice +0x20 is not declared). Here
            // Load() is tried once per Update, as in step 12: the step advances when it succeeds.
            if (!app.m_pyDataConstants->Load()) {
                return;
            }
            ++m_step;
            return;

        case 3:
            // PARTIAL: the same loop, for PyDataArrays::Load() on Application::m_pyDataArrays (+0x30). The timeout
            // budget is missing, as in step 2.
            if (!app.m_pyDataArrays->Load()) {
                return;
            }
            ++m_step;
            return;

        case 4:
            // PARTIAL: the conditional loadSettings(false) is missing. The original calls it only when
            // SavegameManager::m_optionsLoaded (+0x28) is clear, and that field is private. The step advances either way.
            ++m_step;
            return;

        case 5: {
            const int language = app.m_savegameManager->getLanguage();
            if (language == 4) {
                nativeLoadMovie("intro_jp.mp4", language);
            } else if (language == 5) {
                nativeLoadMovie("intro_kr.mp4", language);
            } else {
                nativeLoadMovie("intro.mp4", language);
            }
            ++m_step;
            return;
        }

        case 6:
            if (videoDone) {
                m_step = 7;
            }
            return;

        case 7: {
            const int language = app.m_savegameManager->getLanguage();
            _DEBUG_OUT("\n\n  ---------------LoadingStep_LoadBackground   %d", language);
            // The file depends on the language first, and then on the screen width.
            const char* textureFile;
            if (language == 4) {
                textureFile = "data/3d/textures/splash_final_jp.tga";
            } else if (language == 5) {
                textureFile = "data/3d/textures/splash_final_kor.tga";
            } else if (Width_Screen == 800) {
                textureFile = "data/3d/textures/splash_final_i9000.tga";
            } else if (Width_Screen == 854) {
                textureFile = "data/3d/textures/splash_final_droid.tga";
            } else {
                textureFile = "data/3d/textures/splash_final.tga";
            }
            // PARTIAL: getTexture(manager, textureFile) and the store into m_splashTexture (+0x10) are missing
            // (see SwitchBackground). The step still advances, as the original does.
            (void)textureFile;
            ++m_step;
            return;
        }

        case 8:
            m_step = 9;
            return;

        case 9:
            // PARTIAL: VoxSoundManager::Initialize(VoxSoundManager::s_instance) is missing (not declared). The original
            // falls through to the string pack step.
            [[fallthrough]];

        case 10: {
            StringManager* strings = app.m_stringManager;
            strings->switchPack(static_cast<unsigned int>(app.m_savegameManager->getLanguage()), false);
            ++m_step;
            return;
        }

        case 11:
            TrophyManager::CreateInstance();
            TrophyManager::s_instance->Init();          // virtual slot 0 of the TrophyManager vtable
            TrophyManager::s_instance->LoadTrophies();
            ++m_step;
            return;

        case 12:
            // PARTIAL: the original loops MenuManager::Init() until it returns true, or until 50 device-timer ticks
            // pass (timer at IDevice +0x20, not declared). Here Init() is tried once per Update: the step advances
            // when it returns true, and otherwise waits for the next call.
            if (!MenuManager::GetInstance()->Init()) {
                return;
            }
            ++m_step;
            return;

        case 13:
            Random::s_seed = glitch::os::Timer::getRealTime();
            Random::s_syncedSeed = 0;
            // PARTIAL: Application::m_lastTickTime (+0x70, global dword_99F79C) = the device timer tick at IDevice +0x20
            // is not written: that timer is not declared.
            app.unk_6c = 30;                            // +0x6c, 30 as the original does (dword_99F798)
            app.ComputeDt();
            ++m_step;
            return;

        case 14: {
            MenuManager* menus = MenuManager::GetInstance();
            menus->Reset();
            // PARTIAL: byte_99F819 = 1 (unnamed global) is not written.
            // PARTIAL: the menu choice is not made. When dword_99F7F8 == dword_99F7FC the original picks
            // "menu_language" when SavegameManager +0x37 (m_saveBlocked, private) is set and "menu_splash" otherwise,
            // and stores it in dword_9A247C. The other branch is a debug level request: PlayerSavegame::
            // SG_GetSavegameList(false) and Console::DBG_TryStartingLevel(console, level). DBG_TryStartingLevel is not
            // declared and the branch condition types are not known, so the menu, the debug path and the early
            // return it can make are all missing. The tail below runs as on the common path.
            // PARTIAL: OnlineSingleton<OnlineGameState>::GetInstance() byte +0x26 = 0 is missing: OnlineGameState
            // (net-session-online/online_game_state.hpp) declares no member at +0x26.
            app.m_stateMachine->SwitchState(&GSFlashMenu::s_inst, false);
            const int autoOrientation = app.GetSavedOption("AutoOrientation");
            app.ResetOrientation(autoOrientation);
            // PARTIAL: GCSingleton<GameCenter>::GetInstance() + 14 = 1 is missing. The GameCenter header
            // (net-session-online/game_center.hpp) names +0x0e as resultFlag_, cleared by its reports, so the
            // meaning of this write is not established.
            if (app.IsInternetAccessEnabled()) {
                // PARTIAL: byte_99FC31 = 1 (unnamed global) is not written, and CXPlayerManager::Init() on
                // Singleton<CXPlayerManager>::s_inst is not called: there is no Singleton<T> accessor.
            }
            if (USE_NATIVE_DRM_GAME) {
                // PARTIAL: ALicenseCheck_ValidateLicense(0) is missing: not declared in the tree.
            }
            ++m_step;
            return;
        }

        default:
            std::exit(-1);
            return;
    }
}
