// Checks the MenuBase FSCommand handlers of batch g1-game-ui-MenuBase-8948cc-2 on the paths that run without the engine:
// the argument checks, the save-slot test that reaches PlayerSavegame::SG_Exists, the menu push and pop names, the
// sound-name search, and the state push of a menu that is not guarded. The handlers that need a MenuBase object or the
// local player are not called: the test does not build a MenuBase or a Character.
//
// Everything below the includes is a link-only stub or a recorder. The real definitions live in other categories.
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>

#include "dh2/Application.hpp"
#include "dh2/GSFlashMenu.hpp"
#include "dh2/MenuBase.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/PlayerSavegame.hpp"
#include "dh2/StateMachine.hpp"
#include "dh2/global/menu_base_fs.hpp"
#include "pydata-accessors/member_names.hpp"

// ---- Fake singletons. Only the fields the handlers read are set. ----
namespace {
alignas(Application) unsigned char g_appStorage[sizeof(Application)];
unsigned char g_machineTag;
unsigned char g_managerTag;
unsigned char g_menuTag;
}  // namespace

Application& ApplicationGetInstance() {
    return *reinterpret_cast<Application*>(g_appStorage);
}

// ---- Recorders for the calls the handlers make. ----
namespace {
std::vector<std::string> g_pushed;
std::vector<std::string> g_popped;
std::vector<std::string> g_menuLookups;
std::vector<std::uint32_t> g_existsCalls;
int g_pushStateCalls = 0;
const void* g_pushStateState = nullptr;
bool g_pushStateOwned = true;
int g_failures = 0;

void Check(bool ok, const char* what) {
    if (!ok) {
        std::printf("FAIL: %s\n", what);
        ++g_failures;
    }
}
}  // namespace

// Link-only: the MenuManager singleton and its menu calls. The handlers never dereference the manager pointer.
MenuManager* MenuManager::GetInstance() {
    return reinterpret_cast<MenuManager*>(&g_managerTag);
}
void MenuManager::PushMenu(const char* name) {
    g_pushed.emplace_back(name);
}
void MenuManager::PopMenu(const char* name) {
    g_popped.emplace_back(name);
}
MenuBase* MenuManager::GetMenuByName(const char* name) {
    g_menuLookups.emplace_back(name);
    return reinterpret_cast<MenuBase*>(&g_menuTag);
}

// Link-only: MenuFX calls. FS_ContinueGame, FS_PopAllAbove and IsTopMost reference them; the test calls none of them.
void MenuFX::PopAll() {}
MenuFX::State* MenuFX::GetState(const char*) {
    return nullptr;
}
MenuFX::State* MenuFX::GetCurrentState() {
    return nullptr;
}

// Link-only: the save file test. Slots 2 and 3 exist.
int PlayerSavegame::SG_Exists(std::uint32_t slot) {
    g_existsCalls.push_back(slot);
    return slot == 2 || slot == 3 ? 1 : 0;
}

// Link-only: the state machine. Records the pushed state.
void StateMachine::PushState(StateBase* state, bool owned) {
    ++g_pushStateCalls;
    g_pushStateState = state;
    g_pushStateOwned = owned;
}

// Link-only: the class name used by FS_GetPlayerClass2.
const char* playerClassAsStr() {
    return "Warrior";
}

// Link-only: the GSFlashMenu singleton and the StateBase virtuals it inherits, as in the game-app GSEndGame test.
StateBase::~StateBase() {}
void StateBase::Ctor(const StateMachine*) {}
void StateBase::Dtor(const StateMachine*) {}
void StateBase::Update(StateMachine*, double) {}
void StateBase::Draw(const StateMachine*) {}
void StateBase::Pause(const StateMachine*) {}
void StateBase::Resume(const StateMachine*) {}
void StateBase::Draw2D(const StateMachine*) {}
void StateBase::Sleep(const StateMachine*) {}
void StateBase::WakeUp(const StateMachine*) {}

GSFlashMenu::GSFlashMenu() {}
GSFlashMenu::~GSFlashMenu() {}
void GSFlashMenu::Ctor(const StateMachine*) {}
void GSFlashMenu::Dtor(const StateMachine*) {}
void GSFlashMenu::Update(StateMachine*, double) {}
void GSFlashMenu::Draw(const StateMachine*) {}

GSFlashMenu GSFlashMenu::s_inst;

// Link-only: the sound name table that FS_PlayMusic and FS_PlaySoundFX search.
pydata::NameTable Arrays::Sounds::m_memberNames;

namespace {
char g_soundMusic[] = "music_town";
char g_soundClick[] = "sfx_click";
char* g_soundNames[] = {g_soundMusic, g_soundClick};
}  // namespace

int main() {
    ApplicationGetInstance().m_stateMachine = reinterpret_cast<StateMachine*>(&g_machineTag);
    Arrays::Sounds::m_memberNames.names = g_soundNames;
    Arrays::Sounds::m_memberNames.size = 2;

    // FS_IsSaveSlotValid: null args and slot -1 never reach SG_Exists; any other slot does.
    Check(MenuBase::FS_IsSaveSlotValid("", nullptr, nullptr), "FS_IsSaveSlotValid(null) returns true");
    Check(g_existsCalls.empty(), "FS_IsSaveSlotValid(null) does not call SG_Exists");
    Check(MenuBase::FS_IsSaveSlotValid("", "-1", nullptr), "FS_IsSaveSlotValid(-1) returns true");
    Check(g_existsCalls.empty(), "FS_IsSaveSlotValid(-1) does not call SG_Exists");
    Check(MenuBase::FS_IsSaveSlotValid("", "2", nullptr), "FS_IsSaveSlotValid(2) returns true");
    Check(g_existsCalls.size() == 1 && g_existsCalls[0] == 2, "FS_IsSaveSlotValid(2) calls SG_Exists(2) once");

    // Handlers whose return value is fixed on every path.
    Check(MenuBase::FS_GetSaveSlot("", "", nullptr), "FS_GetSaveSlot returns true");
    Check(MenuBase::FS_ReturnToGame("", "", nullptr), "FS_ReturnToGame returns true");
    Check(MenuBase::FS_SetFocus("", "btn", nullptr), "FS_SetFocus returns true");
    Check(MenuBase::FS_GetPlayerClass("", "", nullptr), "FS_GetPlayerClass returns true");
    Check(MenuBase::FS_GetCharProperty("", "Strength", nullptr), "FS_GetCharProperty returns true");
    Check(MenuBase::FS_GetPlayerClass2("", "", nullptr), "FS_GetPlayerClass2 returns true");
    Check(!MenuBase::FS_GetHasOffHandWeapon("", "", nullptr), "FS_GetHasOffHandWeapon returns false without a character");
    Check(!MenuBase::FS_GetHasMainHandWeapon("", "", nullptr), "FS_GetHasMainHandWeapon returns false without a character");
    Check(!MenuBase::FS_GetHasTwoHandWeapon("", "", nullptr), "FS_GetHasTwoHandWeapon returns false without a character");

    // Menu push and pop by name.
    Check(MenuBase::FS_PushMenu("", "menu_Shop", nullptr), "FS_PushMenu returns true");
    Check(g_pushed.size() == 1 && g_pushed[0] == "menu_Shop", "FS_PushMenu pushes menu_Shop");
    Check(MenuBase::FS_PopMenu("", "menu_Shop", nullptr), "FS_PopMenu(name) returns true");
    Check(g_popped.size() == 1 && g_popped[0] == "menu_Shop", "FS_PopMenu(name) pops menu_Shop");
    Check(MenuBase::FS_SwitchMenu("", "menu_Main", nullptr), "FS_SwitchMenu returns true");
    Check(g_pushed.size() == 2 && g_pushed[1] == "menu_Main", "FS_SwitchMenu pushes menu_Main");

    // Sound-name search: a known cue is found, an unknown or empty one is not.
    Check(MenuBase::FS_PlayMusic("", "music_town", nullptr), "FS_PlayMusic(known) returns true");
    Check(!MenuBase::FS_PlayMusic("", "music_none", nullptr), "FS_PlayMusic(unknown) returns false");
    Check(!MenuBase::FS_PlayMusic("", nullptr, nullptr), "FS_PlayMusic(null) returns false");
    Check(!MenuBase::FS_PlayMusic("", "", nullptr), "FS_PlayMusic(empty) returns false");
    Check(MenuBase::FS_PlaySoundFX("", "sfx_click", nullptr), "FS_PlaySoundFX(known) returns true");
    Check(!MenuBase::FS_PlaySoundFX("", "sfx_none", nullptr), "FS_PlaySoundFX(unknown) returns false");
    Check(!MenuBase::FS_PlaySoundFX("", nullptr, nullptr), "FS_PlaySoundFX(null) returns false");

    // Equip: null and empty args return false before the character is read.
    Check(!MenuBase::FS_AutoEquipSlot("", nullptr, nullptr), "FS_AutoEquipSlot(null) returns false");
    Check(!MenuBase::FS_AutoEquipSlot("", "", nullptr), "FS_AutoEquipSlot(empty) returns false");

    // State push: a menu that is not guarded is stored as the pending menu and pushed, owned false.
    Check(MenuBase::FS_PushState("", "menu_Shop", nullptr), "FS_PushState(menu_Shop) returns true");
    Check(g_menuLookups.size() == 1 && g_menuLookups[0] == "menu_Shop", "FS_PushState looks up menu_Shop");
    Check(GSFlashMenu::s_inst.m_pendingMenu == reinterpret_cast<MenuBase*>(&g_menuTag),
          "FS_PushState stores the looked-up menu as the pending menu");
    Check(g_pushStateCalls == 1 && g_pushStateState == &GSFlashMenu::s_inst && !g_pushStateOwned,
          "FS_PushState pushes GSFlashMenu::s_inst, not owned");

    // A guarded name needs the stack top, which is not reachable: no push, false.
    Check(!MenuBase::FS_PushState("", "menu_Ingame", nullptr), "FS_PushState(menu_Ingame) returns false");
    Check(g_pushStateCalls == 1, "FS_PushState(menu_Ingame) does not push");

    if (g_failures == 0) {
        std::printf("MenuBase batch g1-game-ui-MenuBase-8948cc-2: all checks passed\n");
    }
    return g_failures == 0 ? 0 : 1;
}
