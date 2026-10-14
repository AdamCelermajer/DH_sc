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
#include <string>
#include <vector>
#include "dh2/global/Globals.hpp"
#include "glitch-video/texture_manager.hpp"
#include "glitch-video/video_driver.hpp"
#include "glitch-core-io/os/device.hpp"
#include "net-session-online/tb/online_singleton.hpp"
#include "net-session-online/online_game_state.hpp"
#include "net-session-online/a_license_check.hpp"
#include "net-session-online/cxplayer_manager.hpp"
#include "net-session-online/game_center.hpp"
#include "core-platform/gc_singleton.hpp"
#include "core-platform/w7_console_01.hpp"
#include "dh2/Singleton.hpp"
#include "dh2/PlayerSavegame.hpp"
#include "vox-audio/vox/sound_manager.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/Random.hpp"
#include "dh2/SavegameManager.hpp"
#include "dh2/StateMachine.hpp"
#include "dh2/StringManager.hpp"
#include "dh2/TrophyManager.hpp"
#include "dh2/global/ctrl_camera.hpp"
#include "glitch-core-io/os/logging.hpp"
#include "glitch-video/texture.hpp"          // glitch::video::ITexture, complete for the counted intrusive_ptr release
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
// Logs. When the splash texture is set it is removed from the driver's texture manager (IVideoDriver +0xe0), which also clears
// this smart pointer, the pointer is cleared and the spinner flag (+0x08) is set to -1.
void GSInit::ClearLoadingScreen() {
    _DEBUG_OUT("----------------ClearLoadingScreen");
    if (m_splashTexture) {
        ApplicationGetInstance().m_device->videoDriver->m_textureManager->removeTexture(m_splashTexture);
        m_splashTexture = nullptr;   // +0x10 released
        unk_08 = -1;                 // +0x08
    }
}

// 0x384f64 GSInit::SwitchBackground(void)
// Clears the loading screen on the static instance (GSInit::s_inst, not this) and loads the splash file for the language
// (Korean gets splash_final_kor.tga, every other language splash_final.tga; the Japanese branch of the original loads the same
// file as the default) through the driver's texture manager (getTexture(file, null)). The result is stored in GSInit::s_inst's
// splash texture (dword_9A24C0, +0x10) as a counted pointer.
void GSInit::SwitchBackground() {
    GSInit::s_inst.ClearLoadingScreen();
    const int language = ApplicationGetInstance().m_savegameManager->getLanguage();
    const char* textureFile = (language == 5) ? "data/3d/textures/splash_final_kor.tga"
                                              : "data/3d/textures/splash_final.tga";
    glitch::video::CTextureManager* textures = ApplicationGetInstance().m_device->videoDriver->m_textureManager;
    GSInit::s_inst.m_splashTexture = textures->getTexture(textureFile, nullptr);
}

// 0x3850c8 GSInit::Update(StateMachine *,double)
// One loading step per call, selected by m_step (+0x04). A step that advances ends with ++m_step, as the original's LABEL_22 does.
// The data loads (PyDataConstants, PyDataArrays) and the menu init repeat inside one call until they succeed or 50 ms of the
// device timer have passed. Steps: 0 sound manager created, 1 settings (language) loaded, 2 and 3 pydata constants and arrays,
// 4 full settings, 5 intro movie, 6 wait for the video, 7 splash texture, 8 and 9 sound init, 10 string pack, 11 trophies,
// 12 menu init, 13 random seed and clock, 14 menu reset, first menu, orientation, online and DRM start-up.
void GSInit::Update(StateMachine*, double) {
    Application& app = ApplicationGetInstance();

    switch (m_step) {
        case 0:
            VoxSoundManager::CreateInstance();
            ++m_step;
            return;

        case 1:
            app.m_savegameManager->loadSettings(true);
            ++m_step;
            return;

        case 2: {
            const unsigned int start = glitch::os::Timer::getRealTime();  // device timer (IDevice +0x20), getRealTime
            while (!app.m_pyDataConstants->Load()) {
                if (glitch::os::Timer::getRealTime() - start > 49) {
                    return;
                }
            }
            ++m_step;
            return;
        }

        case 3: {
            const unsigned int start = glitch::os::Timer::getRealTime();
            while (!app.m_pyDataArrays->Load()) {
                if (glitch::os::Timer::getRealTime() - start > 49) {
                    return;
                }
            }
            ++m_step;
            return;
        }

        case 4:
            // The original calls loadSettings(false) only when SavegameManager::m_optionsLoaded (+0x28) is clear, and
            // the step advances either way (asm: both paths store step + 1).
            if (!app.m_savegameManager->m_optionsLoaded) {
                app.m_savegameManager->loadSettings(false);
            }
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
            glitch::video::CTextureManager* textures = app.m_device->videoDriver->m_textureManager;  // IVideoDriver +0xe0
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
            m_splashTexture = textures->getTexture(textureFile, nullptr);
            ++m_step;
            return;
        }

        case 8:
            m_step = 9;
            return;

        case 9:
            VoxSoundManager::s_instance->Initialize();
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

        case 12: {
            MenuManager* menus = MenuManager::GetInstance();
            const unsigned int start = glitch::os::Timer::getRealTime();
            while (!menus->Init()) {
                if (glitch::os::Timer::getRealTime() - start > 49) {
                    return;
                }
            }
            ++m_step;
            return;
        }

        case 13:
            Random::s_seed = glitch::os::Timer::getRealTime();
            Random::s_syncedSeed = 0;
            app.m_lastTickTime = static_cast<int>(glitch::os::Timer::getRealTime());  // dword_99F79C, +0x70
            app.unk_6c = 30;                            // +0x6c, 30 as the original does (dword_99F798)
            app.ComputeDt();
            ++m_step;
            return;

        case 14: {
            MenuManager* menus = MenuManager::GetInstance();
            menus->Reset();
            app.unk_ed = true;  // byte_99F819 (Application +0xed)
            if (app.m_uncompiledData.empty()) {
                // No uncompiled data: the language menu when the save is blocked (the language is not chosen yet), else the splash.
                GSFlashMenu::s_inst.m_pendingMenu =
                    app.m_savegameManager->m_saveBlocked ? menus->GetMenuByName("menu_language") : menus->GetMenuByName("menu_splash");
            } else {
                // A debug level request: the first start tries the named level, and when it starts the step advances at once.
                const std::vector<std::string> savegames = PlayerSavegame::SG_GetSavegameList(false);
                if (Console::DBG_TryStartingLevel(app.m_uncompiledData.c_str())) {
                    ++m_step;
                    return;
                }
                GSFlashMenu::s_inst.m_pendingMenu = menus->GetMenuByName("menu_splash");
            }
            OnlineSingleton<OnlineGameState>::GetInstance()->flag26_ = false;  // +0x26
            app.m_stateMachine->SwitchState(&GSFlashMenu::s_inst, false);
            const int autoOrientation = app.GetSavedOption("AutoOrientation");
            app.ResetOrientation(autoOrientation);
            GCSingleton<GameCenter>::GetInstance()->resultFlag_ = true;  // +0x0e
            if (app.IsInternetAccessEnabled()) {
                Singleton<CXPlayerManager>::s_inst.flag05_ = true;  // byte_99FC31, +0x05
                Singleton<CXPlayerManager>::s_inst.Init();
            }
            if (USE_NATIVE_DRM_GAME) {
                ALicenseCheck::ValidateServer(false);  // ALicenseCheck_ValidateLicense(0), a thunk to this
            }
            ++m_step;
            return;
        }

        default:
            std::exit(-1);
            return;
    }
}
