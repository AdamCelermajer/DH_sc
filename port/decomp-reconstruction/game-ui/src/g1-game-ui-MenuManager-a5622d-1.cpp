// MenuManager bodies, batch g1-game-ui-MenuManager-a5622d-1 (part 2 of 3 of MenuManager).
// Rebuilt from the Hex-Rays pseudocode of libDungeonHunter2.so. Offsets in comments are the original
// 32-bit ARM offsets. A function whose original returns a value that dh2/MenuManager.hpp declares void
// drops the result, as the MenuBase bodies do. The DebugSwitches reads are kept where the original
// makes them, because the first read of the switch file loads it. Partial functions say in their
// comment which part is missing and what it waits on.

#include <map>
#include <string>

#include "dh2/DebugSwitches.hpp"
#include "dh2/EventManager.hpp"
#include "dh2/FlashAnimManager.hpp"
#include "dh2/MenuBase.hpp"
#include "dh2/MenuFX.hpp"
#include "dh2/MenuFlash2DCamera.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/SavegameManager.hpp"
#include "dh2/global/MenuManagerGlobals.hpp"

// The profiling markers are declared in dh2/global/runtime_support.hpp (0x3136b4, 0x3136b8). The same
// two prototypes are repeated here because that header pulls in core-platform's glitch_core stand-in,
// which clashes with the shared glitch_core that FlashAnimManager.hpp includes.
void PushProfilingContext(const char* name);
void PopProfilingContext(const char* name);

namespace {

// The original calls DebugSwitches::load(&s_inst) and then GetSwitch(name) on DebugSwitches::s_inst.
// s_inst is private. DebugSwitches::GetInstance() (0x330740) performs that load and returns &s_inst, so
// one GetInstance() call followed by GetSwitch(name) is the same two steps.
bool ReadDebugSwitch(const char* name) {
    return DebugSwitches::GetInstance()->GetSwitch(name);
}

}  // namespace

// 0x42d234 _ZN11MenuManager7HideAllEv
void MenuManager::HideAll() {
    const int numMenus = GetNumMenus();
    for (int i = 0; i < numMenus; ++i) {
        MenuBase* menu = m_menus[i];
        if (menu->IsVisible()) {
            menu->SetVisible(false);
        }
    }
}

// 0x42d290 _ZN11MenuManager11LoadSWFFileEPKci
// PARTIAL: RenderFX::SetTextBufferingEnabled(fx, true) is not called. The RenderFX header is still the
// MenuFX placeholder and declares no such method. The original allocates the MenuFX with an operator new
// hint of 8 that is not mapped to a swf::MemHint value here, so plain new is used. The rest follows the
// original: the MenuFX goes into m_swfFiles[index] (+0x8c + 4*index) before Load, and the camera goes
// into m_cameras[index] (+0x9c + 4*index).
MenuFX* MenuManager::LoadSWFFile(const char* file, int index) {
    MenuFX* fx = new MenuFX();
    m_swfFiles[index] = fx;
    fx->Load(file, nullptr);
    // RenderFX::SetTextBufferingEnabled(fx, true) omitted: waits on a RenderFX declaration.
    m_cameras[index] = new MenuFlash2DCamera(fx);
    return m_swfFiles[index];
}

// 0x42d30c _ZN11MenuManager19SwitchMenusToIngameEv
// The original returns the MenuFX* of the last LoadSWFFile; the void declaration drops it.
void MenuManager::SwitchMenusToIngame() {
    UnloadSWFFile(2);
    if (Width_Screen == 854) {
        LoadSWFFile("data/menus/dqcharmenu_droid.swf", 1);
        LoadSWFFile("data/menus/dqhud_droid.swf", 3);
    } else if (Width_Screen == 800) {
        LoadSWFFile("data/menus/dqcharmenu_i9000.swf", 1);
        LoadSWFFile("data/menus/dqhud_i9000.swf", 3);
    } else {
        LoadSWFFile("data/menus/dqcharmenu.swf", 1);
        LoadSWFFile("data/menus/dqhud.swf", 3);
    }
}

// 0x42d4bc _ZN11MenuManager10UnloadMenuEi
// PARTIAL, not rebuilt. The original (1) calls MenuCharMenu_Map::GetInstance()->ClearAllIcons() when the
// index is 1, (2) reads the MenuFX in m_multiMenuManager->m_menuFX[index] (MultiMenuManager +0x134),
// (3) erases the m_menus entries that hold that MenuFX (deleting those with m_deleteOnUnload), (4) calls
// FlashAnimManager::ResetScanForAnims with that MenuFX, and (5) calls MultiMenuManager::UnloadSWFFile.
// dh2/MenuCharMenu_Map.hpp and dh2/MultiMenuManager.hpp are absent, so nothing is done here.
void MenuManager::UnloadMenu(int index) {
    (void)index;
}

// 0x42d59c _ZN11MenuManager21SwitchMenusToMainMenuEv
// The original returns the MenuFX* of the last LoadSWFFile; the void declaration drops it.
void MenuManager::SwitchMenusToMainMenu() {
    UnloadSWFFile(3);
    UnloadSWFFile(1);
    switch (Width_Screen) {
        case 854:
            LoadSWFFile("data/menus/dqmenus_droid.swf", 2);
            return;
        case 960:
            LoadSWFFile("data/menus/dqmenus.swf", 2);
            return;
        case 800:
            if (HTC_DEVICES || !NO_IGP) {
                LoadSWFFile("data/menus/dqmenus_i9000.swf", 2);
            } else {
                LoadSWFFile("data/menus/dqmenus_i9000_lg.swf", 2);
            }
            return;
        default:
            if (dword_99F778->getLanguage() == 5) {
                LoadSWFFile("data/menus/dqmenus_Kor.swf", 2);
            } else if (dword_99F778->getLanguage() != 4) {
                LoadSWFFile("data/menus/dqmenus.swf", 2);
            } else {
                LoadSWFFile("data/menus/dqmenus_jp.swf", 2);
            }
            return;
    }
}

// 0x42d72c _ZN11MenuManager10ResetFontsEv
// PARTIAL, not rebuilt. The original forwards to MultiMenuManager::ResetFonts() on m_multiMenuManager
// (+0xf4). dh2/MultiMenuManager.hpp is absent, so the forward is omitted.
void MenuManager::ResetFonts() {}

// 0x42d73c _ZN11MenuManager5ResetEv
// PARTIAL: the first loop of the original (MenuFX::PopAll on each of m_multiMenuManager->m_menuFX[0..3])
// is not made, because dh2/MultiMenuManager.hpp is absent. The rest follows the original: every visible
// menu gets Hide() (vtable +0x10), and m_resetListeners (+0x88) is set.
void MenuManager::Reset() {
    const int numMenus = GetNumMenus();
    for (int i = 0; i < numMenus; ++i) {
        MenuBase* menu = m_menus[i];
        if (menu->IsVisible()) {
            menu->Hide();
        }
    }
    m_resetListeners = true;
}

// 0x42dbb4 _ZN11MenuManagerD1Ev
// 0x42dd38 _ZN11MenuManagerD2Ev (second emitted copy of the same body; one definition covers both)
// PARTIAL. Rebuilt: the three EventManager detaches (events 5, 4, 7) on dword_99F740, the deletion of
// the menus flagged m_deleteOnUnload, the clear of m_menus, and the clear of m_listeners. Omitted:
// (a) the delete of m_multiMenuManager (MultiMenuManager vtable +0x04), because dh2/MultiMenuManager.hpp
// is absent; (b) RenderFX::Finalize(...) followed by CharProperties::UnLoadPropNames(...): the RenderFX
// placeholder declares no Finalize, and the decompiled call passes an argument to UnLoadPropNames while
// dh2/CharProperties.hpp declares it static with no parameters, so the call is not guessed at.
// The member destructors run after this body in the original's order: m_menuFXDeque (+0xcc), m_listeners
// (+0x70), m_menus storage (+0x64), then DebugCachedCharacter (+0x20). The MultiMenuManager is leaked
// until (a) is filled in.
MenuManager::~MenuManager() {
    if (dword_99F740 != nullptr) {
        dword_99F740->Detach(5, this);
        dword_99F740->Detach(4, this);
        dword_99F740->Detach(7, this);
    }
    // (a) delete of m_multiMenuManager omitted.
    for (MenuBase* menu : m_menus) {
        if (menu->m_deleteOnUnload) {
            delete menu;
        }
    }
    m_menus.clear();
    // (b) RenderFX::Finalize and CharProperties::UnLoadPropNames omitted.
    m_listeners.clear();
}

// 0x42dd1c _ZN11MenuManagerD0Ev
// SKIPPED (see the ledger): the compiler emits the deleting destructor for delete. It runs ~MenuManager()
// and then frees the object, so no body is written.

// 0x42dfd8 _ZN11MenuManager17RegisterDummyMenuEP6MenuFXP8MenuBase
void MenuManager::RegisterDummyMenu(MenuFX* fx, MenuBase* menu) {
    m_menus.push_back(menu);
    menu->m_menu = fx;             // +0x04 of the menu
    menu->m_registered = true;     // +0x7c of the menu
}

// 0x42e098 _ZNK11MenuManager11GetMenuNameEi
// The original builds the name from the menu's inline m_name (+0x08) when the index is in range, and
// otherwise returns "unknown". The range test is 0 <= index < GetNumMenus().
std::string MenuManager::GetMenuName(int id) const {
    if (id >= 0 && id < GetNumMenus()) {
        return std::string(m_menus[id]->m_name);
    }
    return "unknown";
}

// 0x42e110 _ZN11MenuManager18UnRegisterListenerEP8MenuBase
// The tracing switch is read and its result discarded, as in the original. The original finds the
// listener's map node (the lower bound of the key) and sets its bool to false only when the key is
// exactly present, which is what find() does here.
void MenuManager::UnRegisterListener(MenuBase* menu) {
    (void)ReadDebugSwitch("isTracingMenuManager");
    const auto it = m_listeners.find(menu);
    if (it != m_listeners.end()) {
        it->second = false;  // +0x14 of the node, the pair's bool
    }
}

// 0x42e208 _ZN11MenuManager7PopMenuEP8MenuBase
// PARTIAL: when the menu has a MenuFX (+0x04), the original asks the MultiMenuManager (+0xf4) through its
// vtable +0x40 whether touch input is pending. If so it calls Application::ResetTouch() on
// Singleton<Application>::s_inst, TouchScreenBase::ProcessEvents() on dword_99F74C, and MultiMenuManager
// vtable +0x38 with false. dh2/MultiMenuManager.hpp and dh2/TouchScreenBase.hpp are absent, and no
// Singleton<Application>::s_inst is declared, so that block is omitted. The compare with the HUD root and
// the UnRegisterListener call are as in the original.
// A null menu is undefined in the original (it reads address 4). This port returns without action.
void MenuManager::PopMenu(MenuBase* menu) {
    if (menu == nullptr) {
        return;
    }
    if (menu->m_menu != nullptr) {
        // Touch-reset block omitted, see above.
    }
    if (menu->m_menu == GetHUDRoot()) {
        UnRegisterListener(menu);
    }
}

// 0x42e2b0 _ZN11MenuManager7PopMenuEPKc
void MenuManager::PopMenu(const char* name) {
    if (MenuBase* menu = GetMenuByName(name)) {
        PopMenu(menu);
    }
}

// 0x42e2d4 _ZNK11MenuManager8DBG_DrawEv
// PARTIAL: the dead-zone overlay and both switch reads are rebuilt. Omitted: the bounding-box overlay
// (isDisplayingMenuBoundingBoxes). It draws each visible menu's character bounds through the glitch video
// driver (dword_99F73C + 0x10: material lookup, setMaterial and the 2D line calls) and walks
// RenderFX::FindCharacters and RenderFX::GetAlpha. None of these are declared, so that block is not made.
void MenuManager::DBG_Draw() const {
    if (ReadDebugSwitch("isDisplayingDeadZones")) {
        DBG_Draw2DDeadZones();
    }
    if (ReadDebugSwitch("isDisplayingMenuBoundingBoxes")) {
        // Bounding-box overlay omitted, see above.
    }
}

// 0x42e75c _ZN11MenuManager11OnFSCommandEPKcS1_
// The original returns the bool of the HUD menu's MyFSCommand (vtable +0x38), and this void override drops
// it. There is no null check: the original calls through the result of GetMenuByName("menu_HUD_0") directly.
void MenuManager::OnFSCommand(const char* command, const char* args) {
    (void)ReadDebugSwitch("isTracingMenuManager");
    MenuBase* hud = GetMenuByName("menu_HUD_0");
    hud->MyFSCommand(command, args);
}

// 0x42e818 _ZN11MenuManager4DrawEb
// PARTIAL: the two switch checks are rebuilt. When IsDeactivatingFlashMenus or
// IsDeactivatingFlashMenusRender is set, the original returns without drawing, and so does this. Omitted:
// the drawing itself. It needs the video-state snapshot (driver words [30], [31], [32] of dword_99F73C + 0x10
// into unk_08, unk_0c and unk_10 before the draw, and the same words into unk_14, unk_18 and unk_1c after),
// and the driver layout is not declared. It also walks m_multiMenuManager->m_menuFX[0..3] and m_cameras,
// the HUD root, and the FlashAnimManager and HUD draw calls, and dh2/MultiMenuManager.hpp is absent.
// The argument is never read by the original.
void MenuManager::Draw(bool /*hudOnly*/) {
    if (ReadDebugSwitch("IsDeactivatingFlashMenus")) {
        return;
    }
    if (ReadDebugSwitch("IsDeactivatingFlashMenusRender")) {
        return;
    }
    // Drawing omitted, see above.
}

// 0x42ea04 _ZN11MenuManager6UpdateEb
// PARTIAL. Rebuilt: the profiling scopes, the switch checks that return early (IsDeactivatingFlashMenus and
// IsDeactivatingFlashMenusUpdate), the listener reset (m_resetListeners clears m_listeners), the
// FlashAnimManager update, and the Update() of every visible menu. Omitted, with the reason:
//  - leader and fillLeaderBoard(): neither is declared.
//  - the MenuDebugHUD update, and the menu-update loop over m_multiMenuManager->m_menuFX: MenuDebugHUD and
//    MultiMenuManager are absent. The loop's start index depends on the argument (3 when set, else 0).
//  - the InteractionType block: PlayerManager::GetLocalPlayer returns a PlayerInfo (dh2/PlayerInfo.hpp is
//    absent), and the block reads the Character field at +0x14a8 and the CSWTCH_6518 table, neither known.
//  - RenderFX::SetWireFrame and RenderFX::InvokeASCallback: not declared in the RenderFX placeholder.
//  - the frame time (Application::GetDt) and the in-game HUD update (IsCurrentlyInGameView, the InfoHUDManager
//    and HUDControls updates): no Singleton<Application>::s_inst is declared.
// The wireframe switch is still read, as in the original, so the switch file is loaded at the same point.
void MenuManager::Update(bool /*hudOnly*/) {
    PushProfilingContext("MenuManager::Update");
    PushProfilingContext("MenuManager::InteractionType");
    // InteractionType block omitted, see above.
    PopProfilingContext("MenuManager::InteractionType");
    (void)ReadDebugSwitch("IsDisplayingMenuWireframe");  // SetWireFrame omitted

    if (ReadDebugSwitch("IsDeactivatingFlashMenus")) {
        PopProfilingContext("MenuManager::Update");
        return;
    }
    if (ReadDebugSwitch("IsDeactivatingFlashMenusUpdate")) {
        PopProfilingContext("MenuManager::Update");
        return;
    }

    if (m_resetListeners) {
        m_listeners.clear();
        m_resetListeners = false;
    }

    // Per-frame update of the MultiMenuManager menus and the in-game HUD update omitted, see above.

    PushProfilingContext("MenuManager::HUDManager");
    FlashAnimManager::GetInstance()->Update();
    PopProfilingContext("MenuManager::HUDManager");

    PushProfilingContext("MenuManager::RegisteredUpdate");
    const int numMenus = GetNumMenus();
    for (int i = 0; i < numMenus; ++i) {
        MenuBase* menu = m_menus[i];
        if (menu->IsVisible()) {
            menu->Update();
        }
    }
    PopProfilingContext("MenuManager::RegisteredUpdate");
    PopProfilingContext("MenuManager::Update");
}

// 0x42ee94 _ZN11MenuManager12RegisterMenuEP8MenuBase
// PARTIAL, not rebuilt. The original looks for the first m_multiMenuManager->m_menuFX[0..3]
// (MultiMenuManager, absent) whose RenderFX::Find(fx, menu->m_name) is non-null, and RenderFX::Find is not
// declared. When one is found, the original pushes the menu on m_menus, calls MenuFX::RegisterState(menu,
// nullptr) on that fx, and sets m_registered. When none is found, it reads the isTracingMenuManager switch
// and returns. Nothing is done here.
void MenuManager::RegisterMenu(MenuBase* menu) {
    (void)menu;
}
