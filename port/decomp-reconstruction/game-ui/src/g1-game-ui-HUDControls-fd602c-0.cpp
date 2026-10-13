// HUDControls bodies, batch g1-game-ui-HUDControls-fd602c-0.
// Rebuilt from the Hex-Rays pseudocode of libDungeonHunter2.so. Offsets in comments are the original 32-bit
// ARM offsets. Partial functions say in their comment which part is missing; the ledger notes repeat it.
//
// Layout facts used here (dh2/HUDControls.hpp):
//   m_defaultWorldA (+0x674) and m_defaultWorldB (+0x6cc) hold 22 words each: (x, y) per layout element, in the
//   order LayoutElement lists them. The words are float bit patterns copied from the world matrix.
//   m_classicPos (+0x724) and m_stylePos (+0x77c) are float[11][2] in the same order.
//   gameswf::matrix m_[2][3]: the original's +8 word is m_[0][2] (x translation), its +20 word is m_[1][2].
//   Level +0x198 is Level::m_isInGameView, Level +0x1a8 is Level::unk_1a8 (both declared in dh2/Level.hpp).
//   Character +0x378 is the v2Controllable::m_controller base member, +0x149c is m_cursorFX.
//   AnimatedFX +0x34, +0x38, +0x3c are unk_34, unk_38, unk_3c (float bit patterns of the position offset).
//
// Host build note: "gameswf/gameswf_character.h" does not compile on this host (vendor-gameswf base/container.h,
// tu_string, reports 'init_empty', 'm_hash_cache' and 'm_length_plus_one' undeclared). The gameswf character type is
// therefore only forward-declared here. The three helpers in the anonymous namespace stand in for the operations
// that need its complete type (get_world_matrix, the as_object to character downcast). They are stubs, and every
// function that calls them is partial.

#include <cstdint>
#include <cstdio>
#include <cstring>

#include "dh2/HUDControls.hpp"

#include "dh2/AnimatedFX.hpp"
#include "dh2/Application.hpp"
#include "dh2/Character.hpp"
#include "dh2/EvTouchScreenMove.hpp"
#include "dh2/EvTouchScreenPress.hpp"
#include "dh2/EventManager.hpp"
#include "dh2/GameSWFUtils.hpp"
#include "dh2/IEvent.hpp"
#include "dh2/Level.hpp"
#include "dh2/MenuFX.hpp"
#include "dh2/PFWorld.hpp"
#include "dh2/PlayerInfo.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/Point2D.hpp"
#include "dh2/Point3D.hpp"
#include "dh2/Singleton.hpp"
#include "dh2/global/IsMultiplayerGame.hpp"
#include "dh2/global/MenuManagerGlobals.hpp"
#include "dh2/v2Controller.hpp"

namespace {

// Elements in the custom layout: the joystick, the interact button, the spell and skill buttons, the character and
// main menu buttons, the health bar, the potion button and the iTunes controls.
constexpr int kLayoutCount = 11;

// The layout element with the given index, in the order of m_classicPos / m_stylePos and of the default words:
// m_joystick (+0x4c), m_btnInteract (+0x88), m_posBtnSpell (+0x3b8), m_posBtnSkill1 (+0x3e8), m_posBtnSkill2
// (+0x418), m_posBtnSkill3 (+0x448), m_posBtnCharacterMenu (+0x478), m_posHealthBar (+0x4d8), m_posBtnMainMenu
// (+0x4a8), m_posBtnPotion (+0x508), m_posItunesControls (+0x568). The health bar comes before the main menu button
// in the original, as the offsets of their cached words show.
DebugCachedCharacter& LayoutElement(HUDControls& h, int index) {
    switch (index) {
        case 0: return h.m_joystick;
        case 1: return h.m_btnInteract;
        case 2: return h.m_posBtnSpell;
        case 3: return h.m_posBtnSkill1;
        case 4: return h.m_posBtnSkill2;
        case 5: return h.m_posBtnSkill3;
        case 6: return h.m_posBtnCharacterMenu;
        case 7: return h.m_posHealthBar;
        case 8: return h.m_posBtnMainMenu;
        case 9: return h.m_posBtnPotion;
        default: return h.m_posItunesControls;
    }
}

// (float)(int)(float)(v / 20.0): the grid step SetHUDPos applies to each coordinate it moves to.
float GridStep(float v) {
    return static_cast<float>(static_cast<int>(static_cast<float>(v / 20.0)));
}

std::uint32_t FloatBits(float value) {
    std::uint32_t bits;
    std::memcpy(&bits, &value, sizeof bits);
    return bits;
}

float BitsToFloat(std::uint32_t bits) {
    float value;
    std::memcpy(&value, &bits, sizeof value);
    return value;
}

// STUB (see the host build note). The translation of a character's world matrix, the +8 and +20 words in the
// original (get_world_matrix().m_[0][2] and m_[1][2] once gameswf_character.h compiles). Returns false, so callers
// store nothing.
bool WorldTranslation(gameswf::character* /*character*/, float& /*x*/, float& /*y*/) {
    return false;
}

// STUB (see the host build note). The character a RenderFX::Find(name) result names, as a gameswf::character*. The
// original downcasts the as_object* (both types are complete there). Returns null until the header compiles.
gameswf::character* FoundCharacter(MenuFX* /*menu*/, const char* /*name*/) {
    return nullptr;
}

// Identity of an as_object* and a character*: both are the address of one gameswf object. gameswf::character derives
// from as_object as its first base, so the as_object subobject sits at offset 0 and the addresses are equal exactly
// when the two pointers name the same object. Works without the complete gameswf::character type.
bool SameObject(const gameswf::as_object* object, const gameswf::character* character) {
    return static_cast<const void*>(object) == static_cast<const void*>(character);
}

// Stores the (x, y) of every layout element as 22 words (the m_defaultWorldA / m_defaultWorldB capture). Returns
// false when a translation could not be read, and then the words are not complete.
bool CaptureDefaults(HUDControls& h, std::uint32_t (&words)[2 * kLayoutCount]) {
    bool complete = true;
    for (int i = 0; i < kLayoutCount; ++i) {
        float x = 0.0f;
        float y = 0.0f;
        if (!WorldTranslation(LayoutElement(h, i).GetChar(), x, y)) {
            complete = false;
            continue;
        }
        words[2 * i] = FloatBits(x);
        words[2 * i + 1] = FloatBits(y);
    }
    return complete;
}

// The reset button moves the layout to a captured default. SetHUDPos reads float pairs, so the words are converted
// first (the original passes the word array as float (*)[2], which is the same bit pattern).
void ResetLayoutTo(HUDControls& h, const std::uint32_t (&words)[2 * kLayoutCount]) {
    float pos[kLayoutCount][2];
    for (int i = 0; i < kLayoutCount; ++i) {
        pos[i][0] = BitsToFloat(words[2 * i]);
        pos[i][1] = BitsToFloat(words[2 * i + 1]);
    }
    h.SetHUDPos(pos, true);
}

// The custom layout code writes a byte at +155 of gameswf::character (0x9b in the 32-bit build) on the cached
// buttons. The vendored gameswf header does not declare that byte yet (layout request in the ledger notes), and a
// raw offset cannot be used on 64-bit, so the write is not made. Every function that calls this is partial.
void SetCharacterFlag155(gameswf::character* /*character*/, bool /*value*/) {}

}  // namespace

// 0x418238 HUDControls::hasInstance(void)
bool HUDControls::hasInstance() {
    return true;
}

// 0x418240 HUDControls::Flush(void)
void HUDControls::Flush() {}

// 0x418244 HUDControls::Draw(void)
void HUDControls::Draw() {}

// 0x418250 HUDControls::onEvent(glitch::SEvent const&)
// The original returns 0 for every event and does not read the event.
bool HUDControls::onEvent(const glitch::SEvent& /*event*/) {
    return false;
}

// 0x418258 HUDControls::onEvent(IEvent const*, EventManager const*)
// Touch press (id 4) and move (id 5) update the pointer position read by Update. A release sets both coordinates
// to -1. The DPad option, an attack held and the unk_84 block turn the handler off.
bool HUDControls::onEvent(const IEvent* event, const EventManager* /*manager*/) {
    if (Singleton<Application>::s_inst.GetSavedOption("DPad") || m_attackHeld || unk_84) {
        return false;
    }
    const std::uint32_t eventId = event->getEventID();
    if (eventId == EvTouchScreenPress::kEventId) {
        const auto* press = static_cast<const EvTouchScreenPress*>(event);
        if (press->pressed) {
            m_pointerY = press->y;
            m_pointerX = press->x;
        } else {
            m_pointerX = -1;
            m_pointerY = -1;
        }
        return false;
    }
    if (eventId == EvTouchScreenMove::kEventId) {
        const auto* move = static_cast<const EvTouchScreenMove*>(event);
        m_pointerX = move->x;
        m_pointerY = move->y;
    }
    return false;
}

// 0x41832c HUDControls::BeginCustom(void)
// PARTIAL: the +155 byte of the reset and save buttons is not written (SetCharacterFlag155, layout request).
void HUDControls::BeginCustom() {
    m_customizing = true;
    SetCharacterFlag155(m_btnCustomReset.GetChar(), true);
    SetCharacterFlag155(m_btnCustomSave.GetChar(), true);
}

// 0x418360 HUDControls::SaveHUDPos(void)
// PARTIAL: the translations are read through WorldTranslation, which is a stub until gameswf_character.h compiles on
// the host. Until then no element is stored. The structure is the original's: m_stylePos when HUDStyle is set,
// otherwise m_classicPos.
void HUDControls::SaveHUDPos() {
    const bool hudStyle = Singleton<Application>::s_inst.GetSavedOption("HUDStyle") != 0;
    for (int i = 0; i < kLayoutCount; ++i) {
        float x = 0.0f;
        float y = 0.0f;
        if (!WorldTranslation(LayoutElement(*this, i).GetChar(), x, y)) {
            continue;
        }
        float(&dst)[2] = hudStyle ? m_stylePos[i] : m_classicPos[i];
        dst[0] = x;
        dst[1] = y;
    }
}

// 0x4187a0 HUDControls::SetHUDPos(float (*)[2], bool)
// pos holds (x, y) per layout element. Each is moved to its grid step, and the layout is saved when asked.
void HUDControls::SetHUDPos(float (*pos)[2], bool save) {
    for (int i = 0; i < kLayoutCount; ++i) {
        gameswf::character* character = LayoutElement(*this, i).GetChar();
        GameSWFUtils::MoveBtnPos(character, GridStep(pos[i][0]), GridStep(pos[i][1]));
    }
    if (save) {
        SaveHUDPos();
    }
}

// 0x418b7c HUDControls::RefreshHUD(void)
// PARTIAL: the +155 bytes of the reset and save buttons are not written (SetCharacterFlag155, layout request).
void HUDControls::RefreshHUD() {
    if (!m_customizing) {
        return;
    }
    m_customizing = false;
    SetCharacterFlag155(m_btnCustomReset.GetChar(), false);
    SetCharacterFlag155(m_btnCustomSave.GetChar(), false);
    SaveHUDPos();
    if (Level* level = Singleton<Application>::s_inst.GetCurrentLevel()) {
        level->unk_1a8 = false;
    }
}

// 0x418bec HUDControls::Touch(void)
// Clears the touch state, then stops the local player's controller (Cmd_Stop on the Character's v2Controller).
void HUDControls::Touch() {
    unk_18 = 0.0f;
    m_cachesReady = false;
    m_attackHeld = false;
    m_moveHeld = false;
    unk_14 = 0;
    Character* player = dword_99F76C->GetLocalPlayer(0, false)->unk_660;
    if (player) {
        player->m_controller->Cmd_Stop();
    }
}

// 0x418d28 HUDControls::OnEvent(RenderFX::Event &)
// PARTIAL. Not rebuilt, because RenderFX::Event declares the event position only as pad_0c (the original reads a
// float x at +0x0c and y at +0x10; layout request):
//   - the joystick stick press (type 4) and move (type 5): the start point (unk_14, unk_18) and the direction
//     (m_direction, m_moveScale, atan2f, sqrtf, RenderFX::SetPosition with the computed position, Point3D::rotateXYBy,
//     m_moveHeld);
//   - the custom layout drag: the start point (static srcX, srcY) recorded on type 4 and the moves on type 5 of the
//     cached elements.
// Also not rebuilt: the menu_Ingame lookup and StateMachine::PushState of the save path (dword_9A247C and dword_99F744
// are not declared yet); the MenuWorldMap::GetInstance()->IsVisible() term of the final test, because MenuWorldMap.hpp
// includes core-platform/deps/glitch_core.hpp, which redefines the glitch types that HUDControls.hpp's
// glitch-core-io include defines (the clash InputManagerEventReceiver.hpp describes). The +155 bytes go through
// SetCharacterFlag155. The return value is the leftover float register of the original; this returns 0.
float HUDControls::OnEvent(RenderFX::Event& event) {
    const int eventType = static_cast<int>(event.type);
    gameswf::as_object* target = event.target;

    if (m_customizing) {
        if (eventType == 6) {
            if (SameObject(target, m_btnCustomSave.GetChar())) {
                m_customizing = false;
                SetCharacterFlag155(m_btnCustomReset.GetChar(), false);
                SetCharacterFlag155(m_btnCustomSave.GetChar(), false);
                SaveHUDPos();
                if (Singleton<Application>::s_inst.GetSavedOption("HUDStyle")) {
                    m_hasStylePos = true;
                } else {
                    m_hasClassicPos = true;
                }
                if (Level* level = Singleton<Application>::s_inst.GetCurrentLevel()) {
                    level->unk_1a8 = false;
                }
                // Not rebuilt: dword_9A247C = MenuManager::GetInstance()->GetMenuByName("menu_Ingame") and
                // StateMachine::PushState(dword_99F744, &GSFlashMenu::s_inst, false). Both globals are undeclared.
            } else if (SameObject(target, m_btnCustomReset.GetChar())) {
                if (Singleton<Application>::s_inst.GetSavedOption("HUDStyle")) {
                    ResetLayoutTo(*this, m_defaultWorldB);
                } else {
                    ResetLayoutTo(*this, m_defaultWorldA);
                }
            }
        }
        // Types 4 and 5 (drag start and drag) are not rebuilt, see the note above.
        event.unk_24 = 1;
        return 0.0f;
    }

    if (SameObject(target, m_joystickStick.GetChar())) {
        // Types 4 and 5 are not rebuilt, see the note above.
        if (eventType == 6 || eventType == 7) {
            unk_14 = 0;
            unk_18 = 0.0f;
            m_menu->SetPosition(m_joystickStick.GetChar(), 0, 0);
            m_moveHeld = false;
            if (Character* player = dword_99F76C->GetLocalPlayer(0, false)->unk_660) {
                player->m_controller->Cmd_Stop();
            }
        }
        event.unk_24 = 1;
        return 0.0f;
    }

    if (SameObject(target, m_btnInteract.GetChar())) {
        if (eventType == 4) {
            m_attackHeld = true;
        } else if (eventType == 6 || eventType == 7) {
            m_pointerY = -1;
            m_attackHeld = false;
            m_pointerX = -1;
        }
        event.unk_24 = 1;
        return 0.0f;
    }

    // The other cached buttons: a hit clears the pointer and sets unk_84. The visible world map also does (not rebuilt).
    DebugCachedCharacter* const buttons[] = {
        &m_btnPotion,
        &m_btnSkill1,
        &m_btnSkill2,
        &m_btnSkill3,
        &m_btnSpell,
        &m_btnCharacterMenu,
        &m_btnMainMenu,
        &m_btnItunes,
        &m_btnItunesPlay,
        &m_btnItunesPause,
        &m_btnItunesNext,
        &m_btnItunesPrevious,
        &m_btnItunesPlaylist,
        &m_btnFastTravelMenu,
    };
    bool hit = false;
    for (DebugCachedCharacter* button : buttons) {
        if (SameObject(target, button->GetChar())) {
            hit = true;
            break;
        }
    }
    if (hit) {
        m_pointerY = -1;
        unk_84 = true;
        m_pointerX = -1;
    }
    return 0.0f;
}

// 0x419b4c HUDControls::initCachedChars(void)
// Looks up the HUD elements under the HUD movie and the custom menu, captures the default layout the first time each
// style is used, and applies the saved layout. PARTIAL, three parts are not rebuilt:
//   - the Find results are gameswf::character* through FoundCharacter, a stub until gameswf_character.h compiles, so
//     the roots are null here;
//   - the +155 bytes go through SetCharacterFlag155 (layout request);
//   - the `bg` width (the virtual call at vtable +0x128 on the RenderFX::Find(menu, "bg", joystick) result, halved and
//     stored in unk_0c and unk_10) is not rebuilt because the virtual slot is not named.
// The default capture is skipped while WorldTranslation is a stub, so the default words are never marked captured.
void HUDControls::initCachedChars() {
    MenuFX* menu = m_menu;
    if (!menu) {
        return;
    }
    const int hudStyle = Singleton<Application>::s_inst.GetSavedOption("HUDStyle");
    char hudName[20];
    std::snprintf(hudName, sizeof hudName, "_root.menu_HUD_%d", hudStyle);
    m_currentRoot.RefreshCache(FoundCharacter(menu, hudName), menu, nullptr);

    m_joystickStick.RefreshCache("HUDelements.controls.controls.Joystick.stick", menu, m_currentRoot.GetChar());
    m_btnInteract.RefreshCache("HUDelements.controls.controls.btn_interact", menu, m_currentRoot.GetChar());
    m_joystick.RefreshCache("HUDelements.controls.controls.Joystick", menu, m_currentRoot.GetChar());
    m_pinch.RefreshCache("pinch", menu, nullptr);
    m_btnPotion.RefreshCache("HUDelements.HealthBars.btn_potion", menu, m_currentRoot.GetChar());
    m_posBtnSpell.RefreshCache("HUDelements.controls.controls.btn_spell", menu, m_currentRoot.GetChar());
    m_posBtnSkill1.RefreshCache("HUDelements.controls.controls.btn_skill1", menu, m_currentRoot.GetChar());
    m_posBtnSkill2.RefreshCache("HUDelements.controls.controls.btn_skill2", menu, m_currentRoot.GetChar());
    m_posBtnSkill3.RefreshCache("HUDelements.controls.controls.btn_skill3", menu, m_currentRoot.GetChar());
    m_posBtnCharacterMenu.RefreshCache("HUDelements.btn_charactermenu", menu, m_currentRoot.GetChar());
    m_posBtnMainMenu.RefreshCache("HUDelements.btn_mainmenu", menu, m_currentRoot.GetChar());
    m_posHealthBar.RefreshCache("HUDelements.HealthBars.player", menu, m_currentRoot.GetChar());
    m_posBtnPotion.RefreshCache("HUDelements.HealthBars.btn_potion", menu, m_currentRoot.GetChar());
    m_posBtnItunes.RefreshCache("HUDelements.itunes_controls.btn_itunes", menu, m_currentRoot.GetChar());
    m_posItunesControls.RefreshCache("HUDelements.itunes_controls", menu, m_currentRoot.GetChar());

    if (hudStyle <= 1) {
        m_btnSkill1.RefreshCache("HUDelements.controls.controls.btn_skill1", menu, m_currentRoot.GetChar());
        m_btnSkill2.RefreshCache("HUDelements.controls.controls.btn_skill2", menu, m_currentRoot.GetChar());
        m_btnSkill3.RefreshCache("HUDelements.controls.controls.btn_skill3", menu, m_currentRoot.GetChar());
    } else {
        m_btnSkill1.RefreshCache("HUDelements.controls.controls.list.dragHandler", menu, m_currentRoot.GetChar());
        m_btnSkill2.RefreshCache("HUDelements.controls.controls.list.dragHandler", menu, m_currentRoot.GetChar());
        m_btnSkill3.RefreshCache("HUDelements.controls.controls.list.dragHandler", menu, m_currentRoot.GetChar());
    }

    m_btnSpell.RefreshCache("HUDelements.controls.controls.btn_spell", menu, m_currentRoot.GetChar());
    m_btnCharacterMenu.RefreshCache("HUDelements.btn_charactermenu", menu, m_currentRoot.GetChar());
    m_btnMainMenu.RefreshCache("HUDelements.btn_mainmenu", menu, m_currentRoot.GetChar());
    m_btnItunes.RefreshCache("HUDelements.itunes_controls.btn_itunes", menu, m_currentRoot.GetChar());
    m_btnItunesPlay.RefreshCache("HUDelements.itunes_controls.itunes_bar.btn_play", menu, m_currentRoot.GetChar());
    m_btnItunesPause.RefreshCache("HUDelements.itunes_controls.itunes_bar.btn_pause", menu, m_currentRoot.GetChar());
    m_btnItunesNext.RefreshCache("HUDelements.itunes_controls.itunes_bar.btn_next", menu, m_currentRoot.GetChar());
    m_btnItunesPrevious.RefreshCache("HUDelements.itunes_controls.itunes_bar.btn_previous", menu,
                                     m_currentRoot.GetChar());
    m_btnItunesPlaylist.RefreshCache("HUDelements.itunes_controls.itunes_bar.btn_playlist", menu,
                                     m_currentRoot.GetChar());
    m_btnFastTravelMenu.RefreshCache("LocationsMenu.btn_FastTravel", menu, nullptr);

    // Not rebuilt: the `bg` width, see the note above.
    menu->SetPosition(m_joystickStick.GetChar(), 0, 0);

    m_currentRoot.RefreshCache(FoundCharacter(menu, "_root.menu_custom"), menu, nullptr);
    m_btnCustomReset.RefreshCache("btn_MENU_CUSTOM_RESET", menu, m_currentRoot.GetChar());
    m_btnCustomSave.RefreshCache("btn_MENU_CUSTOM_SAVE", menu, m_currentRoot.GetChar());
    SetCharacterFlag155(m_currentRoot.GetChar(), true);

    m_currentRoot.RefreshCache(FoundCharacter(menu, "_root.LocationsMenu"), menu, nullptr);
    m_btnFastTravel.RefreshCache("btn_FastTravel", menu, m_currentRoot.GetChar());
    m_btnLocTravel.RefreshCache("btn_LocTravel", menu, m_currentRoot.GetChar());

    if (m_customizing) {
        SetCharacterFlag155(m_btnCustomReset.GetChar(), true);
        SetCharacterFlag155(m_btnCustomSave.GetChar(), true);
        SetCharacterFlag155(m_posBtnMainMenu.GetChar(), false);
        SetCharacterFlag155(m_posItunesControls.GetChar(), false);
        SetCharacterFlag155(m_btnFastTravel.GetChar(), false);
        SetCharacterFlag155(m_btnLocTravel.GetChar(), false);
    } else {
        SetCharacterFlag155(m_btnCustomReset.GetChar(), false);
        SetCharacterFlag155(m_btnCustomSave.GetChar(), false);
        SetCharacterFlag155(m_posBtnMainMenu.GetChar(), true);
        SetCharacterFlag155(m_posItunesControls.GetChar(), true);
    }

    if (hudStyle == 0) {
        if (!m_defaultsCaptured && CaptureDefaults(*this, m_defaultWorldA)) {
            m_defaultsCaptured = true;
        }
        if (m_hasClassicPos) {
            SetHUDPos(m_classicPos, false);
        }
    } else if (hudStyle == 1) {
        if (!m_altDefaultsCaptured && CaptureDefaults(*this, m_defaultWorldB)) {
            m_altDefaultsCaptured = true;
        }
        if (m_hasStylePos) {
            SetHUDPos(m_stylePos, false);
        }
    }
    m_cachesReady = true;
}

// 0x41a780 HUDControls::Update(void)
// PARTIAL, one part is not rebuilt:
//   - b_IsInCutScreen = byte_9A600C != 0 at the top: the global is not declared yet.
// The attack test reads the word at Character +0x14a4 (Hex-Rays *(player + 5284)), which is Character::m_objectOfInterest.
// When it is null, the attack stores 0 at Character +0x413 (Hex-Rays *(player + 1043)), which is CharAI::m_stickyTarget
// (CharAI at Character +0x3c8, its m_stickyTarget at CharAI +0x4b), and then calls Cmd_Attack(nullptr).
void HUDControls::Update() {
    unk_84 = false;
    Level* level = Singleton<Application>::s_inst.GetCurrentLevel();
    if (level && !level->m_isInGameView) {
        if (m_attackHeld) {
            m_attackHeld = false;
        }
        if (m_moveHeld) {
            m_moveHeld = false;
        }
        return;
    }
    if (!m_menu) {
        return;
    }
    if (!m_cachesReady) {
        initCachedChars();
        m_pointerY = -1;
        m_pointerX = -1;
    }
    Character* player = dword_99F76C->GetLocalPlayer(0, false)->unk_660;
    if (!player) {
        return;
    }

    if (m_attackHeld) {
        if (player->m_objectOfInterest != nullptr) {
            player->m_controller->Cmd_UseOOI(nullptr);
        } else {
            player->m_stickyTarget = false;
            player->m_controller->Cmd_Attack(nullptr);
        }
    }

    if (m_moveHeld && player->CTRLIsAllowed()) {
        const float scale = m_moveScale;
        Point3D<float> direction;
        direction.X = m_direction[0] * scale;
        direction.Y = m_direction[1] * scale;
        direction.Z = m_direction[2] * scale;
        player->m_controller->Cmd_HeadTowards(direction);
    }

    if (!m_attackHeld && m_pointerX > 0 && m_pointerY > 0) {
        Point2D<float> screen;
        screen.X = static_cast<float>(m_pointerX);
        screen.Y = static_cast<float>(m_pointerY);
        Point3D<float> world;
        if (Singleton<PFWorld>::s_inst.TranslateScreenToWorld(screen, world)) {
            if (AnimatedFX* cursor = player->m_cursorFX) {
                cursor->unk_34 = static_cast<int>(FloatBits(world.X));
                cursor->unk_38 = static_cast<int>(FloatBits(world.Y));
                cursor->unk_3c = static_cast<int>(FloatBits(world.Z));
                cursor->SyncIrrData(false);
                if (AnimatedFX* synced = player->m_cursorFX) {
                    synced->SetVisible(true);
                }
            }
            player->m_controller->Cmd_MoveTo(world);
        }
    }
}

// 0x41aa40 HUDControls::~HUDControls() (D1); 0x41ac10 (D2) has the same body.
// The cached DebugCachedCharacter members are destroyed after this body, in reverse declaration order, as in the
// original.
HUDControls::~HUDControls() {
    if (dword_99F740) {
        dword_99F740->Detach(4, this);
        dword_99F740->Detach(5, this);
    }
    Flush();
}

// 0x41af1c HUDControls::HUDControls(void) (C1); 0x41b1a4 (C2) has the same body.
// The vtable pointers are set by the compiler. The DebugCachedCharacter members use their default constructors.
HUDControls::HUDControls() {
    m_cachesReady = false;
    m_attackHeld = false;
    m_moveHeld = false;
    unk_0c = 0;
    unk_10 = 0;
    unk_14 = 0;
    unk_18 = 0.0f;
    m_pointerX = -1;
    m_pointerY = -1;
    unk_84 = false;
    m_menu = nullptr;
    m_direction[0] = 0.0f;
    m_direction[1] = 0.0f;
    m_direction[2] = 0.0f;
    m_moveScale = 0.0f;
    m_customizing = false;
    m_defaultsCaptured = false;
    m_altDefaultsCaptured = false;
    m_hasClassicPos = false;
    m_hasStylePos = false;
    dword_99F740->Attach(4, this, 0);
    dword_99F740->Attach(5, this, 0);
}

// 0x41b11c HUDControls::GetInstance(void)
// The singleton is a function-local static, constructed on first use (the guard the original emits).
HUDControls* HUDControls::GetInstance() {
    static HUDControls s_singleton;
    return &s_singleton;
}
