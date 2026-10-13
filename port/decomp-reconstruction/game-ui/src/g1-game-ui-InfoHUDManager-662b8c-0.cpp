// InfoHUDManager bodies, batch g1-game-ui-InfoHUDManager-662b8c-0. Rebuilt from the Hex-Rays pseudocode of
// libDungeonHunter2.so. Offsets in comments are the original 32-bit ARM offsets; the members sit at those offsets in
// dh2/InfoHUDManager.hpp.
//
// Rebuilt in full: Draw, initCachedChars, Update, the constructors (C1 and C2) and GetInstance.
// Partial: the destructors (D1 and D2) and the other functions. The destructors have the original's body (Flush(), then
// the member destruction) but inherit the death-timer gap of Flush. Each partial function's comment says which part is
// left out.
//
// Facts used here:
//   - The C1 and C2 constructors, and the D1 and D2 destructors, have identical bodies. One definition carries both
//     addresses. The destructor's implicit member destruction runs in reverse declaration order, which is the order of
//     the original's calls (+0x54c, +0x51c, +0x4ec, +0x4bc, ... +0x0c), so it needs no body beyond Flush().
//   - Calls with the menu as receiver (menu->GotoFrame, menu->SetText, menu->SetPosition) are RenderFX members. The
//     declarations are missing from dh2/RenderFX.hpp (a placeholder), so every call of them is left out. The symbols are
//     RenderFX::GotoFrame(gameswf::character*, int, bool) (_ZN8RenderFX9GotoFrameEPN7gameswf9characterEib),
//     RenderFX::GotoFrame(gameswf::character*, char const*, bool) (_ZN8RenderFX9GotoFrameEPN7gameswf9characterEPKcb),
//     RenderFX::SetText(gameswf::character*, char const*, bool) (_ZN8RenderFX7SetTextEPN7gameswf9characterEPKcb) and
//     RenderFX::SetPosition(gameswf::character*, int, int) (_ZN8RenderFX11SetPositionEPN7gameswf9characterEii).
//   - The byte at +0x9b of gameswf::character (Hex-Rays +155) is written as character::m_visible. vendor-gameswf's
//     gameswf_scene_node reads that byte as the visibility flag.
//   - The AlliesBarDisplay argument array has six values: bool, string, level, HP percent, bar index, death seconds
//     (-1 when none). Flush passes (false, "", 0, 100, i, -1).
//   - The list buttons' get_member call (vtable +0x20, gameswf as_object::get_member) looks up the member "SlotId".
//     The name is not in the Hex-Rays text: it is the literal of the gameswf::tu_string constructor clone that
//     SlowUpdate and FastUpdate call (_ZN7gameswf9tu_stringC1EPKc.clone.1 holds "SlotId").

#include "dh2/InfoHUDManager.hpp"

#include <cstdio>

#include "dh2/Application.hpp"
#include "dh2/Character.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/GameObject.hpp"
#include "dh2/Level.hpp"
#include "dh2/MenuFX.hpp"
#include "dh2/PlayerManager.hpp"
#include "dh2/Singleton.hpp"
#include "dh2/global/IsMultiplayerGame.hpp"  // dword_99F76C, the PlayerManager instance
#include "gameswf/gameswf_character.h"       // gameswf::character (m_visible, get_member)
#include "vendor-gameswf/as_value.hpp"       // gameswf::as_value (DH2 layout)

// 0x41d56c _ZN14InfoHUDManager4DrawEv
void InfoHUDManager::Draw() {
}

// 0x41d570 _ZN14InfoHUDManager19HideLocalDeathTimerEv
// PARTIAL: the original (1) looks up "_root.menu_HUD_<HUDStyle>" with RenderFX::Find and, unless byte_9A600C is set,
// sets its visible byte to 1; (2) calls menu->GotoFrame(death timer, "Hide", true); (3) clears the death timer's visible
// byte. Step (3) is made here. Step (1) waits on the byte_9A600C declaration (dh2/global/Globals.hpp), and step (2)
// waits on the RenderFX::GotoFrame declaration, so both are left out.
void InfoHUDManager::HideLocalDeathTimer() {
    m_deathTimer.GetChar()->m_visible = false;
}

// 0x41d658 _ZN14InfoHUDManager19ShowLocalDeathTimerEv
// PARTIAL: menu->GotoFrame(death timer, "Show", true) is left out (RenderFX::GotoFrame is not declared). The root
// lookup and the two visible bytes are made as in the original.
void InfoHUDManager::ShowLocalDeathTimer() {
    char hudName[20];
    std::snprintf(hudName, sizeof hudName, "_root.menu_HUD_%d", Singleton<Application>::s_inst.GetSavedOption("HUDStyle"));
    static_cast<gameswf::character*>(m_menu->Find(hudName))->m_visible = false;
    m_deathTimer.GetChar()->m_visible = true;
}

// 0x41d728 _ZN14InfoHUDManager18applyOneTimeValuesEv
// PARTIAL: menu->GotoFrame(charMenuHudChar, <class frame>, false) is left out (RenderFX::GotoFrame is not declared). The
// class frame is computed as in the original: class 290..292 gives 2, 325..327 gives 1, any other class gives 0.
void InfoHUDManager::applyOneTimeValues() {
    m_joystick.GetChar()->m_visible = Singleton<Application>::s_inst.GetSavedOption("DPad") != 0;

    Character* ch = dword_99F76C->GetLocalPlayer(0, false)->unk_660;
    if (ch == nullptr) {
        return;
    }
    int classFrame = 0;
    switch (ch->SG_GetPlayerClass()) {
        case 290:
        case 291:
        case 292:
            classFrame = 2;
            break;
        case 325:
        case 326:
        case 327:
            classFrame = 1;
            break;
        default:
            classFrame = 0;
            break;
    }
    static_cast<void>(classFrame);  // PARTIAL: the frame is not applied (see above)
}

// 0x41d880 _ZN14InfoHUDManager15initCachedCharsEv
void InfoHUDManager::initCachedChars() {
    if (m_menu == nullptr) {
        return;
    }

    const int savedOption = Singleton<Application>::s_inst.GetSavedOption("HUDStyle");
    char hudName[20];
    std::snprintf(hudName, sizeof hudName, "_root.menu_HUD_%d", savedOption);
    // RenderFX::Find is declared as returning as_object*. The original casts the result to a character, and the path
    // names a movie clip, so the cast is kept.
    gameswf::character* hudRoot = static_cast<gameswf::character*>(m_menu->Find(hudName));

    m_playerHpBar.RefreshCache("HUDelements.HealthBars.player.bar_hp", m_menu, hudRoot);
    m_potionDistressGlow.RefreshCache("HUDelements.HealthBars.btn_potion.DistressGlow", m_menu, hudRoot);
    m_hurtCorners.RefreshCache("_root.HurtCorners", m_menu, nullptr);
    m_playerMpBar.RefreshCache("HUDelements.HealthBars.player.bar_mp", m_menu, hudRoot);
    m_playerXpBar.RefreshCache("HUDelements.HealthBars.player.bar_xp", m_menu, hudRoot);
    m_potionCount.RefreshCache("HUDelements.HealthBars.btn_potion.cnt.value", m_menu, hudRoot);
    m_spellCoolDown.RefreshCache("HUDelements.controls.controls.btn_spell.CoolDown", m_menu, hudRoot);
    m_spellGrey.RefreshCache("HUDelements.controls.controls.btn_spell.Grey", m_menu, hudRoot);

    if (savedOption <= 1) {
        // Skill branch: btn_skill1..3 (the skill numbers are 1-based in the path).
        for (int i = 0; i != 3; ++i) {
            char path[64];
            std::snprintf(path, sizeof path, "HUDelements.controls.controls.btn_skill%d.CoolDown", i + 1);
            m_skillCoolDown[i].RefreshCache(path, m_menu, hudRoot);
            std::snprintf(path, sizeof path, "HUDelements.controls.controls.btn_skill%d.Grey", i + 1);
            m_skillGrey[i].RefreshCache(path, m_menu, hudRoot);
        }
    } else {
        // List branch: the three list buttons. Their CoolDown and Grey children use the skill slots (see the header).
        m_listBtn0.RefreshCache("HUDelements.controls.controls.list.btn_0", m_menu, hudRoot);
        m_skillCoolDown[0].RefreshCache("HUDelements.controls.controls.list.btn_0.CoolDown", m_menu, hudRoot);
        m_skillGrey[0].RefreshCache("HUDelements.controls.controls.list.btn_0.Grey", m_menu, hudRoot);
        m_listBtnPre0.RefreshCache("HUDelements.controls.controls.list.btn_pre0", m_menu, hudRoot);
        m_skillCoolDown[1].RefreshCache("HUDelements.controls.controls.list.btn_pre0.CoolDown", m_menu, hudRoot);
        m_skillGrey[1].RefreshCache("HUDelements.controls.controls.list.btn_pre0.Grey", m_menu, hudRoot);
        m_listBtnPost0.RefreshCache("HUDelements.controls.controls.list.btn_post0", m_menu, hudRoot);
        m_skillCoolDown[2].RefreshCache("HUDelements.controls.controls.list.btn_post0.CoolDown", m_menu, hudRoot);
        m_skillGrey[2].RefreshCache("HUDelements.controls.controls.list.btn_post0.Grey", m_menu, hudRoot);
    }

    m_charMenuLevelUp.RefreshCache("HUDelements.btn_charactermenu.anim_levelup", m_menu, hudRoot);
    m_joystick.RefreshCache("HUDelements.controls.controls.Joystick", m_menu, hudRoot);
    m_enemy.RefreshCache("HUDelements.HealthBars.enemy", m_menu, hudRoot);
    m_enemyName.RefreshCache("HUDelements.HealthBars.enemy.enemy_name.text", m_menu, hudRoot);
    m_enemyLevel.RefreshCache("HUDelements.HealthBars.enemy.enemy_level.value", m_menu, hudRoot);
    m_enemyHpBar.RefreshCache("HUDelements.HealthBars.enemy.HpBar.bar_hp", m_menu, hudRoot);
    m_ally0.RefreshCache("HUDelements.HealthBars.Ally0", m_menu, hudRoot);
    m_ally1.RefreshCache("HUDelements.HealthBars.Ally1", m_menu, hudRoot);
    m_ally2.RefreshCache("HUDelements.HealthBars.Ally2", m_menu, hudRoot);
    m_charMenuHudChar.RefreshCache("HUDelements.btn_charactermenu.btimg.HudChar", m_menu, hudRoot);
    m_deathTimer.RefreshCache("_root.DeathTimer", m_menu, nullptr);
    m_deathTimerText.RefreshCache("_root.DeathTimer.DeathNumberTimeMc.TimerNumber_txt", m_menu, nullptr);

    m_cachesReady = true;  // *(this + 4) = 1
}

// 0x41de54 _ZN14InfoHUDManager10SlowUpdateEv
// PARTIAL: a list button whose SlotId is below 0 would index the as_value padding in the original. That case is not
// reproduced: that button's grey flag keeps its value. A SlotId above 2 is read as 0, as in the original.
void InfoHUDManager::SlowUpdate() {
    Character* ch = dword_99F76C->GetLocalPlayer(0, false)->unk_660;
    if (ch == nullptr) {
        return;
    }

    // Usability of the three skill slots (v16[12..14] in the original).
    bool skillUsable[3] = {false, false, false};
    for (int slot = 0; slot != 3; ++slot) {
        const int skill = ch->SG_GetSkillInSlot(slot);
        if (skill != -1) {
            skillUsable[slot] = ch->AI_IsSkillUsable(static_cast<unsigned int>(skill));
        }
    }

    // The spell grey overlay is visible while the spell is not usable.
    gameswf::character* spellGrey = m_spellGrey.GetChar();
    spellGrey->m_visible = !ch->AI_IsSpellUsable();

    if (Singleton<Application>::s_inst.GetSavedOption("HUDStyle") <= 1) {
        for (int j = 0; j != 3; ++j) {
            gameswf::character* grey = m_skillGrey[j].GetChar();
            if (grey != nullptr) {
                grey->m_visible = !skillUsable[j];
            }
        }
        return;
    }

    // List branch: each list button reads its "SlotId" member; grey overlay k follows skill slot SlotId.
    DebugCachedCharacter* listButtons[3] = {&m_listBtn0, &m_listBtnPre0, &m_listBtnPost0};
    for (int k = 0; k != 3; ++k) {
        gameswf::as_value slotId;
        listButtons[k]->GetChar()->get_member("SlotId", &slotId);
        int slot = static_cast<int>(slotId.to_number());
        if (slot > 2) {
            slot = 0;
        }
        if (slot < 0) {
            continue;  // PARTIAL: see the comment above the function
        }
        gameswf::character* grey = m_skillGrey[k].GetChar();
        if (grey != nullptr) {
            grey->m_visible = !skillUsable[slot];
        }
    }
}

// 0x41e064 _ZN14InfoHUDManager10FastUpdateEv
// PARTIAL. Made: the early return without a local character, the charMenuLevelUp visibility, the enemy target choice,
// the enemy bar state (m_lastEnemy, m_enemy visibility, DebugSwitches::load) and the hide path. Left out, each with its
// reason in place below: the RenderFX frames and texts, the Character fields that dh2/Character.hpp does not declare,
// the cooldown reads (the skill-script table), the online allies block (COnline's online flag is protected) and the
// enemy name text.
void InfoHUDManager::FastUpdate() {
    PlayerInfo* local = dword_99F76C->GetLocalPlayer(0, false);
    Character* ch = local->unk_660;
    if (ch == nullptr) {
        return;
    }

    // PARTIAL: the potion count text (RenderFX::SetText on m_potionCount, "%d" of ItemInventory::GetNumPotions()).
    // PARTIAL: the frames of m_playerHpBar, m_playerMpBar, m_playerXpBar, m_potionDistressGlow and m_hurtCorners. They
    //          need Character fields at +0x1088 and +0x1090 (HP current and maximum), +0x109c and +0x10a4 (MP) and
    //          +0x107c and +0x1080 (XP), none declared in dh2/Character.hpp.

    m_charMenuLevelUp.GetChar()->m_visible = ch->PROPS_GetInt(148, false) != 0;

    // PARTIAL: the cooldown frames. Each skill slot's cooldown is CharAISkillScript::GetCooldown of the table at Character
    //          +0x47c (the spell script at +0x488). The table is not declared, and the frames need RenderFX::GotoFrame.
    //          HUDStyle <= 1 uses m_skillCoolDown[0..2]; otherwise the list buttons' SlotId picks the frame.

    // PARTIAL: the online allies block. It runs only while the online-game flag (COnline +0x05) is set. That flag is
    //          protected in net-session-online/include/net-session-online/c_online.hpp and has no public getter, so the
    //          block is left out. It sets the level, potion, XP and name of up to three remote players in the ally slots
    //          m_ally0..m_ally2, and calls AlliesBarDisplay with six arguments.

    // Enemy bar.
    bool monsterTarget = false;  // v38 in the original
    if (!ch->IsDead()) {
        GameObject* objectOfInterest = ch->m_objectOfInterest;
        if (objectOfInterest != nullptr && objectOfInterest->IsCharacter() &&
            static_cast<Character*>(objectOfInterest)->IsMonster()) {
            monsterTarget = true;
        }
    }

    Character* enemy = nullptr;  // v40 in the original
    Character* aiTarget = ch->AI_GetTargetAsCharacter();
    if (aiTarget != nullptr && aiTarget->IsMonster()) {
        enemy = aiTarget;
    } else if (monsterTarget) {
        enemy = static_cast<Character*>(ch->m_objectOfInterest);
    }

    if (enemy != nullptr) {
        if (enemy != m_lastEnemy) {
            m_lastEnemy = enemy;
            m_enemy.GetChar()->m_visible = true;
            // PARTIAL: menu->GotoFrame(m_enemy, "Show", false) (RenderFX::GotoFrame is not declared).
            DebugSwitches::s_inst.load();
            // PARTIAL: the enemy name and level text. The name is StringManager::getString of the enemy's name id
            //          (Character +0x1040, not declared), or the net id and name when the switch
            //          "ShowMonstersNamesAndNetID" is set (name at +0x44, net id at +0x108, not declared). The level
            //          text is "??" for a boss or when GetLevel() is -1, else the decimal level. Both texts go to
            //          m_enemyName and m_enemyLevel through RenderFX::SetText, which is not declared.
        }

        // The enemy HP frame is the HP percent times 100 (as a float), capped at 100, minus one.
        int hpFrame = static_cast<int>(static_cast<float>(enemy->GetHPPercent() * 100.0));
        if (hpFrame >= 100) {
            hpFrame = 100;
        }
        static_cast<void>(hpFrame);  // PARTIAL: menu->GotoFrame(m_enemyHpBar, hpFrame - 1, false) is left out
        return;
    }

    // No enemy: hide the bar once.
    if (m_lastEnemy != nullptr) {
        m_lastEnemy = nullptr;
        m_enemy.GetChar()->m_visible = false;
        // PARTIAL: menu->GotoFrame(m_enemyHpBar, 0, false) and menu->GotoFrame(m_enemy, "Hide", false) are left out.
    }
}

// 0x41ec00 _ZN14InfoHUDManager6UpdateEv
void InfoHUDManager::Update() {
    Level* level = Singleton<Application>::s_inst.GetCurrentLevel();
    if ((level != nullptr && !level->m_isInGameView) || m_menu == nullptr) {
        return;
    }

    if (!m_cachesReady) {
        initCachedChars();
        applyOneTimeValues();
        // The original stores the old m_cachesReady byte here, which is 0 on this path, into +0x00.
        m_lastEnemy = nullptr;
    }

    if (m_slowUpdateTimer >= 0) {
        m_slowUpdateTimer -= Singleton<Application>::s_inst.GetDt();
        FastUpdate();
        return;
    }

    m_slowUpdateTimer = 500;
    SlowUpdate();
    FastUpdate();
}

// 0x41ecb0 _ZN14InfoHUDManagerC1Ev and 0x41ee58 _ZN14InfoHUDManagerC2Ev
// The two constructors have identical bodies; this one stands for both. The DebugCachedCharacter members are
// default-constructed in declaration order, the same order as the original's calls (+0x0c .. +0x54c).
InfoHUDManager::InfoHUDManager() {
    m_lastEnemy = nullptr;   // +0x00: the original does not write it here; the port clears it
    m_slowUpdateTimer = -1;  // +0x08
    m_cachesReady = false;   // +0x04
    m_menu = nullptr;        // +0x57c
}

// 0x41edd0 _ZN14InfoHUDManager11GetInstanceEv
InfoHUDManager* InfoHUDManager::GetInstance() {
    static InfoHUDManager s_singleton;
    return &s_singleton;
}

// 0x41ef78 _ZN14InfoHUDManager5FlushEv
// PARTIAL: the death-timer branch is left out. When m_deathTimer has a cached character (+0x2c) and its proxy (+0x28) is
// alive, the original plays menu->GotoFrame(m_deathTimer, "Hide", true) and clears the visible byte. Otherwise it
// releases the proxy and clears the cache. The GotoFrame waits on the RenderFX declaration, and DebugCachedCharacter
// has no method for the release.
void InfoHUDManager::Flush() {
    Level* level = Singleton<Application>::s_inst.GetCurrentLevel();
    if (level == nullptr || !level->m_isLoaded) {
        return;
    }

    // Three AlliesBarDisplay calls that clear the ally slots: (false, "", 0, 100, i, -1).
    for (int i = 0; i != 3; ++i) {
        gameswf::as_value args[6];
        args[0].set_bool(false);
        args[1].set_string("");
        args[2].set_double(0.0);
        args[3].set_double(100.0);
        args[4].set_double(i);
        args[5].set_double(-1.0);
        m_menu->InvokeASCallback(m_menu->GetFlashRoot(), "AlliesBarDisplay", args, 6);
    }

    m_enemy.GetChar()->m_visible = false;
}

// 0x41f16c _ZN14InfoHUDManagerD1Ev and 0x41f29c _ZN14InfoHUDManagerD2Ev
// PARTIAL: the two destructors have identical bodies; this one stands for both. The original calls Flush(this) first,
// then destroys the 29 DebugCachedCharacter members in reverse offset order (+0x54c .. +0x0c). The implicit member
// destruction below runs in reverse declaration order, which is that same order. The original also tests this + offset
// against the constants -12, -684, -540 and -396 before four member groups (the ally trio and the two skill arrays and
// the list buttons). For a live object those tests always pass, so they are not written. The body is exact, but it
// inherits the death-timer gap of Flush(), which is partial.
InfoHUDManager::~InfoHUDManager() {
    Flush();
}
