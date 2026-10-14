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
#include <cstdint>
#include <cstdio>
#include <cstring>

#include "dh2/HUDControls.hpp"

#include <cmath>

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
#include "dh2/MenuWorldMap.hpp"
#include "dh2/ScriptManager.hpp"
#include "dh2/StateMachine.hpp"
#include "dh2/GSFlashMenu.hpp"
#include "dh2/MenuManager.hpp"
#include "dh2/v2Controller.hpp"
#include "gameswf/gameswf_character.h"

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

// The translation of a character world matrix, the +8 and +20 words in the original (get_world_matrix().m_[0][2] and
// m_[1][2]). Returns false for a missing character.
bool WorldTranslation(gameswf::character* character, float& x, float& y) {
    if (character == nullptr) {
        return false;
    }
    const gameswf::matrix* world = character->get_world_matrix();
    x = world->m_[0][2];
    y = world->m_[1][2];
    return true;
}

// The character a RenderFX::Find(name) result names: the as_object is the character base.
gameswf::character* FoundCharacter(MenuFX* menu, const char* name) {
    return static_cast<gameswf::character*>(menu->Find(name));
}

// Identity of an as_object* and a character*.
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

// Byte +155 (0x9b) of gameswf::character: the visible flag.
void SetCharacterFlag155(gameswf::character* character, bool value) {
    character->m_visible = value;
}

// (float)(v / 20.0): the drag step of the custom layout.
float Div20(float v) {
    return static_cast<float>(v / 20.0);
}

// The world-matrix (x, y) of a layout element, read like the original does (matrix words +8 and +20).
void ElementTranslation(DebugCachedCharacter& element, float& x, float& y) {
    const gameswf::matrix* world = element.GetChar()->get_world_matrix();
    x = world->m_[0][2];
    y = world->m_[1][2];
}

// The drag step of one custom-layout element: ((event + world) - source) / 20, truncated.
float DragStep(float eventValue, float worldValue, float source) {
    return static_cast<float>(static_cast<int>(Div20((eventValue + worldValue) - source)));
}

}  // namespace

// Global bool set at the top of Update (the original name); nothing reads it.
bool b_IsInCutScreen = false;

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
void HUDControls::BeginCustom() {
    m_customizing = true;
    SetCharacterFlag155(m_btnCustomReset.GetChar(), true);
    SetCharacterFlag155(m_btnCustomSave.GetChar(), true);
}

// 0x418360 HUDControls::SaveHUDPos(void)
// The world translation of each layout element is stored in m_stylePos when HUDStyle is set, otherwise in m_classicPos.
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
    unk_18 = 0;
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
// While customizing: a press (type 4) on a layout element records the drag start (static srcX, srcY); a move (type 5) drags the
// element (the interact button and the three skill buttons move as a group, the character menu button drags the health bar with
// it, a stick drag moves the joystick); a release (type 6) on the save button stores the layout and returns to the game menu, on
// the reset button restores the captured defaults. Outside customizing: the joystick stick press and move set the move direction
// and scale, its release stops the player; the interact button holds the attack; any other HUD button (or a visible world map)
// clears the pointer and blocks the touch handler. The return value is the leftover float register of the original; this returns 0.
float HUDControls::OnEvent(RenderFX::Event& event) {
    static float srcX = 0.0f;  // OnEvent(RenderFX::Event&)::srcX
    static float srcY = 0.0f;  // OnEvent(RenderFX::Event&)::srcY
    Character* player = dword_99F76C->GetLocalPlayer(0, false)->unk_660;
    MenuWorldMap* worldMap = MenuWorldMap::GetInstance();
    const int eventType = static_cast<int>(event.type);
    gameswf::character* target = event.target;

    if (m_customizing) {
        if (eventType == 4) {
            DebugCachedCharacter* const pressable[] = {&m_joystickStick, &m_btnInteract,     &m_posBtnSpell,
                                                       &m_posBtnSkill1,  &m_posBtnSkill2,    &m_posBtnSkill3,
                                                       &m_posBtnCharacterMenu, &m_posBtnMainMenu, &m_posBtnPotion,
                                                       &m_posBtnItunes};
            for (DebugCachedCharacter* element : pressable) {
                if (target == element->GetChar()) {
                    srcX = event.x;
                    srcY = event.y;
                    break;
                }
            }
        }
        if (eventType == 5) {
            float x = 0.0f;
            float y = 0.0f;
            if (target == m_joystickStick.GetChar()) {
                ElementTranslation(m_joystick, x, y);
                GameSWFUtils::MoveBtnPos(m_joystick.GetChar(), DragStep(event.x, x, srcX), DragStep(event.y, y, srcY));
            } else if (target == m_btnInteract.GetChar() || target == m_posBtnSkill1.GetChar() ||
                       target == m_posBtnSkill2.GetChar() || target == m_posBtnSkill3.GetChar()) {
                ElementTranslation(m_btnInteract, x, y);
                const int dy = static_cast<int>(Div20((event.y + y) - srcY));
                const int dx = static_cast<int>(Div20((event.x + x) - srcX));
                const std::uint32_t* defaults = Singleton<Application>::s_inst.GetSavedOption("HUDStyle") ? m_defaultWorldB : m_defaultWorldA;
                auto offset = [&](int element, int axis) {
                    return static_cast<int>(Div20(BitsToFloat(defaults[2 + axis]) - BitsToFloat(defaults[2 * element + axis])));
                };
                GameSWFUtils::MoveBtnPos(m_btnInteract.GetChar(), DragStep(event.x, x, srcX), DragStep(event.y, y, srcY));
                GameSWFUtils::MoveBtnPos(m_posBtnSkill1.GetChar(), static_cast<float>(dx - offset(3, 0)), static_cast<float>(dy - offset(3, 1)));
                GameSWFUtils::MoveBtnPos(m_posBtnSkill2.GetChar(), static_cast<float>(dx - offset(4, 0)), static_cast<float>(dy - offset(4, 1)));
                GameSWFUtils::MoveBtnPos(m_posBtnSkill3.GetChar(), static_cast<float>(dx - offset(5, 0)), static_cast<float>(dy - offset(5, 1)));
            } else if (target == m_posBtnSpell.GetChar()) {
                ElementTranslation(m_posBtnSpell, x, y);
                GameSWFUtils::MoveBtnPos(m_posBtnSpell.GetChar(), DragStep(event.x, x, srcX), DragStep(event.y, y, srcY));
            } else if (target == m_posBtnCharacterMenu.GetChar()) {
                ElementTranslation(m_posBtnCharacterMenu, x, y);
                const int dy = static_cast<int>(Div20((event.y + y) - srcY));
                const int dx = static_cast<int>(Div20((event.x + x) - srcX));
                GameSWFUtils::MoveBtnPos(m_posHealthBar.GetChar(), static_cast<float>(dx - 43), static_cast<float>(dy));
                GameSWFUtils::MoveBtnPos(m_posBtnCharacterMenu.GetChar(), static_cast<float>(dx), static_cast<float>(dy));
            } else if (target == m_posBtnMainMenu.GetChar()) {
                ElementTranslation(m_posBtnMainMenu, x, y);
                GameSWFUtils::MoveBtnPos(m_posBtnMainMenu.GetChar(), DragStep(event.x, x, srcX), DragStep(event.y, y, srcY));
            } else if (target == m_posBtnPotion.GetChar()) {
                ElementTranslation(m_posBtnPotion, x, y);
                GameSWFUtils::MoveBtnPos(m_posBtnPotion.GetChar(), DragStep(event.x, x, srcX), DragStep(event.y, y, srcY));
            }
        }
        if (eventType == 6) {
            if (target == m_btnCustomSave.GetChar()) {
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
                GSFlashMenu::s_inst.m_pendingMenu = MenuManager::GetInstance()->GetMenuByName("menu_Ingame");  // dword_9A247C
                Singleton<Application>::s_inst.m_stateMachine->PushState(&GSFlashMenu::s_inst, false);
            } else if (target == m_btnCustomReset.GetChar()) {
                if (Singleton<Application>::s_inst.GetSavedOption("HUDStyle")) {
                    ResetLayoutTo(*this, m_defaultWorldB);
                } else {
                    ResetLayoutTo(*this, m_defaultWorldA);
                }
            }
        }
        event.unk_24 = 1;
        return 0.0f;
    }

    if (target == m_joystickStick.GetChar()) {
        if (eventType == 4) {
            unk_14 = static_cast<int>(static_cast<float>(event.x / 20.0));
            unk_18 = static_cast<int>(static_cast<float>(event.y / 20.0));
        } else if (eventType == 5) {
            const gameswf::matrix& local = target->get_matrix();
            int dx = static_cast<int>(static_cast<float>((event.x + local.m_[0][2]) / 20.0) - static_cast<float>(unk_14));
            int dy = static_cast<int>(static_cast<float>((event.y + local.m_[1][2]) / 20.0) - static_cast<float>(unk_18));
            const float angle = atan2f(static_cast<float>(dy), static_cast<float>(dx));
            int clampedX = dx >= -unk_0c ? (dx >= unk_0c ? unk_0c : dx) : -unk_0c;
            int clampedY = dy >= -unk_10 ? (dy >= unk_10 ? unk_10 : dy) : -unk_10;
            const float length = sqrtf(static_cast<float>(clampedX) * static_cast<float>(clampedX) +
                                       static_cast<float>(clampedY * clampedY));
            const float radius = static_cast<float>(unk_0c);
            m_moveScale = length / radius;
            if (m_moveScale > 1.0f) {
                clampedY = static_cast<int>(static_cast<float>(unk_10) * sinf(angle));
                clampedX = static_cast<int>(radius * cosf(angle));
            }
            m_menu->SetPosition(m_joystickStick.GetChar(), clampedX, clampedY);
            if (player != nullptr && player->CTRLIsAllowed()) {
                m_direction[0] = 1.0f;
                m_direction[1] = -1.0f;
                m_direction[2] = 0.0f;
                reinterpret_cast<Point3D<float>&>(m_direction).normalize();
                const Point3D<float> origin{0.0f, 0.0f, 0.0f};
                reinterpret_cast<Point3D<float>&>(m_direction).rotateXYBy(angle * -57.296f + 90.0f, origin);
                m_moveHeld = true;
            }
        } else if (eventType == 6 || eventType == 7) {
            unk_14 = 0;
            unk_18 = 0;
            m_menu->SetPosition(m_joystickStick.GetChar(), 0, 0);
            m_moveHeld = false;
            if (player != nullptr) {
                player->m_controller->Cmd_Stop();
            }
        }
        event.unk_24 = 1;
        return 0.0f;
    }

    if (target == m_btnInteract.GetChar()) {
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

    // The other cached buttons, or a visible world map: the pointer is cleared and unk_84 set.
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
        if (target == button->GetChar()) {
            hit = true;
            break;
        }
    }
    if (!hit && worldMap != nullptr && worldMap->IsVisible()) {
        hit = true;
    }
    if (hit) {
        m_pointerY = -1;
        unk_84 = true;
        m_pointerX = -1;
    }
    return 0.0f;
}

// 0x419b4c HUDControls::initCachedChars(void)
// Looks up the HUD elements under the HUD movie and the custom menu, captures the default layout the first time each style is
// used, and applies the saved layout. The "bg" width gives the stick reach (half of its width / 20).
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

    gameswf::character* bg = menu->Find("bg", m_currentRoot.GetChar());
    const int stickReach = static_cast<int>(static_cast<float>(bg->get_width() / 20.0) * 0.5f);  // vtable +0x128
    unk_0c = stickReach;
    unk_10 = stickReach;
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
// The cut-scene byte (ScriptManager::m_inCutScene, byte_9A600C) is copied to b_IsInCutScreen at the top.
// The attack test reads the word at Character +0x14a4 (Hex-Rays *(player + 5284)), which is Character::m_objectOfInterest.
// When it is null, the attack stores 0 at Character +0x413 (Hex-Rays *(player + 1043)), which is CharAI::m_stickyTarget
// (CharAI at Character +0x3c8, its m_stickyTarget at CharAI +0x4b), and then calls Cmd_Attack(nullptr).
void HUDControls::Update() {
    b_IsInCutScreen = ScriptManager::s_inst.m_inCutScene;
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
    unk_18 = 0;
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
