// MenuMainMenu bodies, batch g1-game-ui-MenuMainMenu-3de27c-0 (the whole class, part 1 of 1).
// Rebuilt from the Hex-Rays pseudocode and the assembly of libDungeonHunter2.so. Offsets in comments are the original
// 32-bit ARM offsets. A function marked PARTIAL says in its comment which step is missing and why. The value tables and the
// declarations the bodies use are in dh2/global/menu_main_menu.hpp.
//
// Facts shared by the bodies:
//   - MenuMainMenu adds no instance members to MenuBase (0xc4 bytes). Its members are all static.
//   - Initial values of the statics, read from the binary: mSavegameSlot is -1, m_isDestroyed is true, and the camera,
//     the scene root and the preview character start null.
//   - The glitch device is Singleton<Application>::s_inst.m_device. It holds the 2D video driver at +0x10 (dword_99F73C+16)
//     and the scene manager at +0x1c (dword_99F73C+28). dh2 and glitch-core-io do not declare those two words, so every
//     step that goes through the device is MISSING in the bodies below.
//   - The model root (VisualObject +0x08) is a RootSceneNode, which does not derive from ISceneNode in dh2, so the ISceneNode
//     calls on it are MISSING too. glitch::collada::CRootSceneNode has no class definition in the tree.
//   - The compiler emits the vptr stores and the base destructors of the constructors and destructors, so the bodies carry
//     only the calls the original makes. The deleting destructor (D0) is not rebuilt: delete emits it.
//   - Application members are read through Singleton<Application>::s_inst. dword_99F764 (ObjectManager), dword_99F770
//     (PhysicalWorld), dword_99F778 (SavegameManager), dword_99F780 (MenuManager), dword_99F758 (PyDataConstants) and
//     dword_99F760 (StringManager) are the members +0x38, +0x44, +0x4c, +0x54, +0x2c and +0x34 (dh2/global/Globals.hpp).

#include <cstdint>
#include <cstring>
#include <string>

#include "core-platform/debug_out.hpp"
#include "vox-audio/vox/sound_manager.hpp"
#include "dh2/AnimSetManager.hpp"
#include "dh2/Application.hpp"
#include "dh2/Character.hpp"
#include "dh2/CharAnimator.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/MenuBase.hpp"
#include "dh2/MenuFX.hpp"
#include "dh2/MenuMainMenu.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/ObjectManager.hpp"
#include "dh2/PhysicalWorld.hpp"
#include "dh2/PlayerSavegame.hpp"
#include "dh2/RenderFX.hpp"
#include "dh2/SavegameManager.hpp"
#include "dh2/Singleton.hpp"
#include "dh2/StringManager.hpp"
#include "dh2/global/Globals.hpp"
#include "dh2/global/menu_main_menu.hpp"
#include "dh2/global/native_app_hud_level.hpp"
#include "glitch-core-io/core/reference_counted.hpp"
#include "glitch-scene-collada-gui/scene/CCameraSceneNode.hpp"
#include "glitch-shared/glitch_core.hpp"
#include "pydata-accessors/w7_pydata12.hpp"

// Statics. Definitions are here (MenuMainMenu owns them). Initial values as the binary holds them (see the file comment).
glitch::scene::CCameraSceneNode* MenuMainMenu::m_mainmenuPaneCam = nullptr;               // the avatar camera
glitch::collada::CRootSceneNode* MenuMainMenu::m_characterPlaneSceneNode = nullptr;        // the scene of the bdae file
std::int32_t MenuMainMenu::mSavegameSlot = -1;                                            // slot shown, -1 when none
Character* MenuMainMenu::mCharacterToRender = nullptr;                                    // the preview character
bool MenuMainMenu::m_isDestroyed = true;                                                  // set by Hide, cleared by Show

namespace {

// The StrID text of a constant. The original reads the constant (PyDataConstants) and then the string (StringManager).
std::string StrIdText(const char* constantName) {
    Application& app = Singleton<Application>::s_inst;
    const int constant = app.m_pyDataConstants->getConstant("StrID", constantName);
    return std::string(app.m_stringManager->getString(constant));
}

}  // namespace

// 0x42ba98 _ZN12MenuMainMenu8GotFocusEv
// Stores 1 to lastOpenMenuID and clears the two multiplayer flags. The original returns the address of the IGM flag in r0;
// that is the leftover of the last store, not part of the void override, so it is dropped.
void MenuMainMenu::GotFocus() {
    lastOpenMenuID = 1;
    Is_In_Multiplayer_IGM = false;
    Is_In_Multiplayer_Mode = false;
}

// 0x42bae8 _ZN12MenuMainMenu9LostFocusEv
// Empty in the original (BX LR).
void MenuMainMenu::LostFocus() {}

// 0x42baec _ZN12MenuMainMenu19DestroyAvatarCameraEv
// Removes the avatar camera from its scene (the ISceneNode remove slot, vtable +0x68), drops the reference, and clears the
// static pointer. The original has no active-camera step here. The remove call goes to CCameraSceneNode::remove, which is
// PARTIAL in glitch-scene-collada-gui (its active-camera check is missing), so this body is exact but its callee is not.
void MenuMainMenu::DestroyAvatarCamera() {
    if (m_mainmenuPaneCam) {
        m_mainmenuPaneCam->remove();
        static_cast<glitch::IReferenceCounted*>(m_mainmenuPaneCam)->drop();
        m_mainmenuPaneCam = nullptr;
    }
}

// 0x42bb44 _ZN12MenuMainMenu19RenderCharacterPaneERN7gameswf12render_stateEPv
// Empty in the original (BX LR). It is the display callback of the "RenderedBG" movie.
void MenuMainMenu::RenderCharacterPane(gameswf::render_state&, void*) {}

// 0x42bb48 _ZN12MenuMainMenu7OnEventERN8RenderFX5EventE
// Event kind 6 (a button) whose name contains "btn_GLLive" opens the Game Center page; one containing "btn_MENU_MORE_GAMES"
// opens the in-game page. Both pass the savegame language through the same table (dh2/global headers). The name is tested
// with strstr, which the original reaches through LC_API_STRSTR_0 (the libc call).
void MenuMainMenu::OnEvent(RenderFX::Event& e) {
    if (static_cast<int>(e.type) == 6) {
        if (std::strstr(e.name, "btn_GLLive") != nullptr) {
            const int language = Singleton<Application>::s_inst.m_savegameManager->getLanguage();
            appDebugLog("****OpenGLive called****", "");
            nativeOpenGLive(GLLiveLanguageArg(language));
        } else if (std::strstr(e.name, "btn_MENU_MORE_GAMES") != nullptr) {
            const int language = Singleton<Application>::s_inst.m_savegameManager->getLanguage();
            appDebugLog("****OpenIGP called****", "");
            nativeOpenIGP(IGPLanguageArg(language));
        }
    }
    MenuBase::OnEvent(e);
}

// 0x42bcbc _ZN12MenuMainMenu16DestroyCharacterEv
// Flushes the object manager (dword_99F764, Application +0x38) and the animation set manager, then clears the preview character.
void MenuMainMenu::DestroyCharacter() {
    Singleton<Application>::s_inst.m_objectManager->Flush();
    Singleton<AnimSetManager>::s_inst.Flush();
    mCharacterToRender = nullptr;
}

// 0x42bd08 _ZN12MenuMainMenu14SetupCharacterEv
// PARTIAL. Written: the test of the slot and the scene, the slot test SG_Exists (0x464d5c, the uint overload; the original
// also passes a second register, which the overload does not read), the two CreatePlayer calls ("PlayerCharacter_0"; the
// exists path passes (false, false) and then SG_Load(4); the other path passes (true, false), as the assembly shows) and the
// result store.
// Missing, in the original order (all of them go through the device or through the model root, see the file comment):
//   1. CSceneManager::setActiveCamera(scene, m_mainmenuPaneCam), scene = device +0x1c.
//   2. v = mCharacterToRender->m_visualObject->m_node (VisualObject +0x08); scene->rootNode->addChild(v) (scene +0x04,
//      vtable +0x5c).
//   3. v->setPosition(0, -200, -20) (ISceneNode vtable +0xa4): kPreviewCharacterPositionY/Z.
//   4. v->setRotation(quaternion set (0, 0, 3.1416 * -0.125)) (vtable +0x9c): kPreviewCharacterRoll.
//   5. v->updateAbsolutePosition(true) (vtable +0xb8).
void MenuMainMenu::SetupCharacter() {
    if (mSavegameSlot == -1 || !m_characterPlaneSceneNode) {
        return;
    }
    const std::uint32_t slot = static_cast<std::uint32_t>(mSavegameSlot);
    if (PlayerSavegame::SG_Exists(slot)) {
        mCharacterToRender = Character::CreatePlayer(slot, 0, 0, "PlayerCharacter_0", false, false);
        mCharacterToRender->SG_Load(4);
    } else {
        mCharacterToRender = Character::CreatePlayer(slot, 0, 0, "PlayerCharacter_0", true, false);
    }
    // MISSING steps 1 to 5: see the comment above.
}

// 0x42bebc _ZN12MenuMainMenu12DestroySceneEv
// PARTIAL. Written, in the original order: the scene root pointer is cleared, DestroyCharacter, and Application::CleanGlitch.
// Missing:
//   1. m_characterPlaneSceneNode->remove() (scene root, vtable +0x68) before the pointer is cleared, and the reference drop.
//      glitch::collada::CRootSceneNode has no class definition in the tree.
//   2. The scene manager's vtable +0x68 call (device +0x1c). Its result is passed to DestroyCharacter, which ignores it.
//   3. glitch::video::C2DDriver::freeTextures on the device's 2D driver (device +0x10, dword_99F73C+16).
void MenuMainMenu::DestroyScene() {
    if (m_characterPlaneSceneNode) {
        // MISSING 1: remove() and the drop of the scene root. The pointer is cleared as in the original.
        m_characterPlaneSceneNode = nullptr;
    }
    // MISSING 2: the scene manager call (device +0x1c, vtable +0x68).
    DestroyCharacter();
    // MISSING 3: C2DDriver::freeTextures (device +0x10).
    Singleton<Application>::s_inst.CleanGlitch();
}

// 0x42bf3c _ZN12MenuMainMenu18CreateAvatarCameraEv
// PARTIAL. Written: the camera (CCameraSceneNode, id -1, position (0, -900, 150), target (0, 0, 225), far-to-infinity false),
// the up vector (0, 0, 1), the aspect ratio, the FOV, one extra reference, the near plane 10 and the far plane 2000.
// Missing, in the original order, all through the device or the model root:
//   1. scene->rootNode->addChild(camera) (scene +0x04, vtable +0x5c).
//   2. The drop of the creation reference. It follows the addChild and must not be made before it, or the camera would be freed
//      while the code below still uses it. The extra reference written below takes the count to the same value as the original.
//   3. CSceneManager::setActiveCamera(scene, camera), scene = device +0x1c.
void MenuMainMenu::CreateAvatarCamera() {
    if (m_mainmenuPaneCam) {
        return;
    }
    const glitch::core::vector3d<float> position{0.0f, kAvatarCameraPositionY, kAvatarCameraPositionZ};
    const glitch::core::vector3d<float> target{0.0f, 0.0f, kAvatarCameraTargetZ};
    m_mainmenuPaneCam = new glitch::scene::CCameraSceneNode(-1, position, target, false);
    // MISSING 1 to 3: see the comment above.
    m_mainmenuPaneCam->setUpVector(glitch::core::vector3d<float>{0.0f, 0.0f, 1.0f});  // vtable +0x114
    m_mainmenuPaneCam->setAspectRatio(kAvatarCameraAspectRatio);                      // vtable +0x138
    m_mainmenuPaneCam->setFOV(kAvatarCameraFov);                                      // vtable +0x13c
    ++static_cast<glitch::IReferenceCounted*>(m_mainmenuPaneCam)->referenceCounter;   // one extra reference
    m_mainmenuPaneCam->setNearValue(kAvatarCameraNear);                               // vtable +0x130
    m_mainmenuPaneCam->setFarValue(kAvatarCameraFar);                                 // vtable +0x134
}

// 0x42c0d0 _ZN12MenuMainMenu24ChangeCharacterToDisplayEib
// The first register is the slot and the second the force flag; the third register of the listing is unused. When the slot
// changes (or the flag is set), the old character is destroyed if a slot was shown, the new slot is stored, the character is
// set up and the avatar camera is created.
void MenuMainMenu::ChangeCharacterToDisplay(int savegameSlot, bool force) {
    if (savegameSlot != mSavegameSlot || force) {
        if (mSavegameSlot != -1) {
            DestroyCharacter();
        }
        mSavegameSlot = savegameSlot;
        SetupCharacter();
        CreateAvatarCamera();
    }
}

// 0x42c128 _ZN12MenuMainMenu4HideEv
// The IsValidMenu test, MenuBase::Hide, the Hide of the "menu_bg" menu, DestroyScene, DestroyAvatarCamera, m_isDestroyed = true,
// mSavegameSlot = -1, then VoxSoundManager::StopMusic(s_instance, 1000). (DestroyScene has its own partial row.)
void MenuMainMenu::Hide() {
    if (!IsValidMenu()) {
        return;
    }
    MenuBase::Hide();
    MenuBase* background = MenuManager::GetInstance()->GetMenuByName("menu_bg");
    background->Hide();
    DestroyScene();
    DestroyAvatarCamera();
    m_isDestroyed = true;
    mSavegameSlot = -1;
    VoxSoundManager::s_instance->StopMusic(1000);
}

// 0x42c1c8 _ZN12MenuMainMenu6UpdateEv
// PARTIAL. Written: the IsValidMenu test and the animation of the preview character (CharAnimator at +0x49c, which is a base of
// Character). Missing: the scene manager update, scene->update(0.0f, false) (device +0x1c, vtable +0x60).
void MenuMainMenu::Update() {
    if (!IsValidMenu()) {
        return;
    }
    if (mCharacterToRender) {
        mCharacterToRender->CharAnimator::Update();
    }
    // MISSING: scene manager update, vtable +0x60 with (0.0f, false), on the scene manager of device +0x1c.
}

// 0x42c240 _ZN12MenuMainMenuD1Ev (complete-object destructor)
// 0x42c290 _ZN12MenuMainMenuD2Ev (base-object destructor, the same bytes)
// The vptr store and the MenuBase base destructor are the implicit ones of an empty body. Chain note: MenuBase::~MenuBase is
// PARTIAL in this tree (the weak_ptr proxy releases in MenuFX::State::~State are missing), so the chain is partial too.
MenuMainMenu::~MenuMainMenu() {}

// 0x42c274 _ZN12MenuMainMenuD0Ev (deleting destructor) is not rebuilt: the C++ delete expression emits it.

// 0x42c2c4 _ZN12MenuMainMenuC1Ev (complete-object constructor)
// 0x42c310 _ZN12MenuMainMenuC2Ev (base-object constructor, the same bytes)
// MenuBase("menu_MainMenu"), then the vptr store (compiler), then the registration with the MenuManager singleton. Chain note:
// MenuManager::RegisterMenu (0x42ee94) has an empty body in this tree, so the menu is not yet put in the manager's list.
MenuMainMenu::MenuMainMenu() : MenuBase("menu_MainMenu") {
    MenuManager::GetInstance()->RegisterMenu(this);
}

// 0x42c35c _ZN12MenuMainMenu11GetInstanceEv
// Function-local static, constructed on first use. The guard and the atexit registration are what the compiler emits for a
// static local with a non-trivial constructor and destructor, as the original does.
MenuMainMenu* MenuMainMenu::GetInstance() {
    static MenuMainMenu menu;
    return &menu;
}

// 0x42c510 _ZN12MenuMainMenu10SetupSceneEv
// PARTIAL. Written: the test of the scene root, and PhysicalWorld::load(0, 0, 1, 1) on the physical world (dword_99F770,
// Application +0x44). Missing, in the original order:
//   1. The collada database over "data/3d/menu/main_menu_charactere_swamp.bdae" with the factory ColladaFactory::s_factory
//      (ELF 0x9a1ed8, 8 bytes). dh2/ColladaFactory.hpp does not declare the static.
//   2. constructScene(video driver), where the driver is device +0x10 (dword_99F73C+16). The device words are not declared.
//   3. scene->rootNode->addChild(the scene root) (scene +0x04, vtable +0x5c), with the scene manager at device +0x1c.
//   4. The drop of the scene root (the creation reference). glitch::collada::CRootSceneNode is not defined in the tree.
void MenuMainMenu::SetupScene() {
    if (m_characterPlaneSceneNode) {
        return;
    }
    Singleton<Application>::s_inst.m_physicalWorld->load(0.0f, 0.0f, 1.0f, 1.0f);
    // MISSING 1 to 4: see the comment above.
}

// 0x42c62c _ZN12MenuMainMenu4ShowEv
// PARTIAL. Written, in the original order: the IsValidMenu test, the "menu_bg" Show, MenuBase::Show, the debug switch reads,
// the avatar camera, the scene, the preview character when none is set, m_isDestroyed = false, the "RenderedBG" display
// callback, and the error event's StrID message.
// Missing:
//   1. The GameCenter prelude: GCSingleton<GameCenter> +0x13 = 1, and GameCenter::ResetPlayerList when the bytes at +0x05 and
//      +0x07 are set. dh2 has no GameCenter header (waits on dh2/GameCenter.hpp).
//   2. The debug trace when the "MenuMainMenu" switch is set: gameswf::weak_ptr<character>::check_proxy on m_characterProxy
//      (+0x48), then RenderFX::DBG_TraceContextInformation(m_menu, m_character) (+0x4c). Neither is declared in dh2.
//   3. RenderFX::SetAlpha(m_menu, "btn_Debug1", 0) and RenderFX::SetAlpha(m_menu, "btn_debug2", 0). Not declared in dh2.
//   4. The "MainMenuError" callback on the root movie (MenuFX::GetRoot, then RenderFX::GetRoot and gameswf::root::get_root_movie,
//      then InvokeASCallback with a gameswf::as_value holding the message, count 1). Left out because vendor-gameswf
//      as_value.hpp does not compile on this host (base/container.h: tu_string has no m_hash_cache or m_length_plus_one).
//      The call is otherwise exact; MainMenuErrorShowsCallback in dh2/global/menu_main_menu.hpp gives its condition.
//   5. The common tail, which every path reaches: GameCenter +0x14 = 0 and the unnamed byte at 0x99f7d7 (byte_99F7D7) = 1.
//      Event 7 also sets two GameCenter bytes (+0x0e and +0x0a) before the tail. Both need the GameCenter header and the byte
//      declaration.
void MenuMainMenu::Show() {
    if (!IsValidMenu()) {
        return;
    }
    // MISSING 1: the GameCenter prelude. See the comment above.
    MenuBase* background = MenuManager::GetInstance()->GetMenuByName("menu_bg");
    background->Show();
    MenuBase::Show();
    DebugSwitches::s_inst.load();
    (void)DebugSwitches::s_inst.GetSwitch("isTracingMenuMainMenu");
    DebugSwitches::s_inst.load();
    const bool tracing = DebugSwitches::s_inst.GetSwitch("MenuMainMenu");
    if (tracing) {
        // MISSING 2: the debug trace of the menu context. See the comment above.
    }
    // MISSING 3: RenderFX::SetAlpha of "btn_Debug1" and "btn_debug2" to 0.
    CreateAvatarCamera();
    SetupScene();
    if (!mCharacterToRender) {
        SetupCharacter();
    }
    m_isDestroyed = false;
    m_menu->RegisterDisplayCallback("RenderedBG", RenderCharacterPane, this);

    // GetGoToMainMenuEvent(true) clears the stored event, so it is read even though the callback is missing (MISSING 4).
    const int event = static_cast<int>(Singleton<Application>::s_inst.GetGoToMainMenuEvent(true));
    std::string message;
    if (const char* constant = MainMenuErrorStrId(event)) {
        message = StrIdText(constant);
    }
    // MISSING 4: the "MainMenuError" callback with `message`, when MainMenuErrorShowsCallback(event). See the comment above.
    // MISSING 5: the common tail (GameCenter +0x14 = 0, byte_99F7D7 = 1). See the comment above.
    (void)message;
}
