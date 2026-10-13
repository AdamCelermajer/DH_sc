// Batch g1-game-character-AISPlayer-a14aa9-0 (all of AISPlayer): the AI for a player-controlled character.
// Declarations and layout come from dh2/AISPlayer.hpp, which already declares every method in this batch.
//
// Offsets in comments are the original 32-bit offsets. AISPlayer's own members are the vector at +0xc4 (unk_c4),
// the aggro count at +0xd0 (m_aggroCount) and the aggro-for-music value at +0xd4 (m_aggroForMusic). The callback
// mask at +0xb8 is CharAIScript::unk_b8. m_character (+0x98) is the owner, and its CharAI base (+0x3c8) holds
// m_targetSeeking (CharAI +0x4a, Character +0x412), so the owner's fields are reached through the named members.

#include <cstdint>
#include <cstdio>
#include <string>

#include "dh2/AISPlayer.hpp"
#include "dh2/Character.hpp"
#include "dh2/CharAI.hpp"
#include "dh2/CharStateMachine.hpp"
#include "dh2/DebugSwitches.hpp"
#include "dh2/LuaScript.hpp"
#include "dh2/v2Controller.hpp"

// 0x3dd7ec _ZN9AISPlayer8OnAttackEiib AISPlayer::OnAttack(int, int, bool)
// Thunk: the binary is a single branch to AISDefault::OnAttack. Forwarded, not reimplemented (TRACK-B decision 4).
// The Hex-Rays return type (CMessaging*) is the r0 residue of that branch; the hook is void in AISDefault.
void AISPlayer::OnAttack(int a, int b, bool flag) {
    AISDefault::OnAttack(a, b, flag);
}

// 0x3dd7f0 _ZN9AISPlayer20OnTargetInMeleeRangeEv AISPlayer::OnTargetInMeleeRange()
// The base hook runs only when the owner is not attacking and its target is still being sought. The owner's
// m_targetSeeking (+0x4a of CharAI) is cleared on both paths. The Hex-Rays return value is r0 residue: the hook
// is void, as the base AISDefault::OnTargetInMeleeRange is.
void AISPlayer::OnTargetInMeleeRange() {
    if (!m_character->CharStateMachine::SM_IsAttacking() && m_character->CharAI::AI_IsTargetSeeking()) {
        AISDefault::OnTargetInMeleeRange();
    }
    m_character->m_targetSeeking = false;
}

// 0x3dd84c _ZN9AISPlayer12OnTargetDiedEv AISPlayer::OnTargetDied()
// Stops the owner's controller, clears the target without forcing it, and syncs the last target. The original
// returns the AI_SyncLastTarget result as r0 residue; the hook is void, so the result is not returned.
void AISPlayer::OnTargetDied() {
    m_character->m_controller->Cmd_Stop();
    m_character->CharAI::AI_SetTarget(nullptr, false);
    m_character->CharAI::AI_SyncLastTarget();
}

// 0x3dd884 _ZN9AISPlayer7InitVCBEv AISPlayer::InitVCB()
// Runs the AISDefault mask setup, then ORs the OnKill bit (0x400) into the callback mask at +0xb8 when the script
// defines OnKill. The function returns that bit (the value of its last test), not the whole mask.
int AISPlayer::InitVCB() {
    AISDefault::InitVCB();
    const std::uint32_t mask = unk_b8;                              // +0xb8 callback mask
    const int onKillBit = IsInVFTable("OnKill") ? 0x400 : 0;
    unk_b8 = mask | static_cast<std::uint32_t>(onKillBit);
    return onKillBit;
}

// 0x3dd99c _ZN9AISPlayerD1Ev AISPlayer::~AISPlayer() (complete-object destructor)
// The original sets the AISPlayer vtable, frees the Character* vector at +0xc4, sets the AISDefault vtable and calls
// CharAIScript::~CharAIScript. A C++ destructor runs its member destructors (the vector, which frees the buffer)
// before the base destructors, which is the same order. The body is therefore empty. The original's node-allocator
// versus CustomFree split (blocks of 0x80 bytes or less) is an allocator detail the host std::vector does not repeat.
AISPlayer::~AISPlayer() {}

// 0x3dda18 _ZN9AISPlayerD0Ev AISPlayer::~AISPlayer() (deleting destructor)
// SKIPPED: compiler-generated. It calls the complete-object destructor above, then CustomFree(this); the C++ delete
// expression produces it.

// 0x3dda34 _ZN9AISPlayer14OnEnemySpottedEP9Character AISPlayer::OnEnemySpotted(Character*)
// Runs the AISDefault hook, then reads the AutomaticKill debug switch. When it is set, the spotted enemy's controller
// is told to kill with the owner as killer (Cmd_Kill(killer, silent = false)). The spotted character's controller is
// at its +0x378, the v2Controllable base's m_controller.
void AISPlayer::OnEnemySpotted(Character* c) {
    AISDefault::OnEnemySpotted(c);
    DebugSwitches::s_inst.load();
    const bool automaticKill = DebugSwitches::s_inst.GetSwitch("AutomaticKill");
    if (automaticKill) {
        c->m_controller->Cmd_Kill(m_character, false);
    }
}

// 0x3ddb10 _ZN9AISPlayer6OnKillEP9Character AISPlayer::OnKill(Character*)
// PARTIAL: waits on dh2/sfc/script/lua/Arguments.hpp. The Arguments class is only forward-declared in the tree and
// its pushUserData is not declared, so the Lua call cannot be built. Written: the AISDefault hook and the test of the
// OnKill bit (0x400) in the callback mask at +0xb8. Not written: the Arguments object that pushes the victim as user
// data, and LuaScript::Call("OnKill", args) inside the bit test.
void AISPlayer::OnKill(Character* victim) {
    AISDefault::OnKill(victim);
    if ((unk_b8 & 0x400u) != 0) {
        // PARTIAL: the script call is not written (see above).
    }
}

// 0x3ddb70 _ZN9AISPlayer7OnAggroEP9Character AISPlayer::OnAggro(Character*)
// PARTIAL: the body below the aggro counter update is not written. The original does the rest only when
// !GetOnline()->isOnlineGame_ or PlayerManager::IsLocalPlayer(dword_99F76C, owner) is true. Blocked by:
//   - COnline +0x05 (isOnlineGame_) is private in net-session-online/c_online.hpp and has no getter, so the gate
//     cannot be evaluated.
//   - Not written, inside the gate: m_aggroForMusic += the aggressor's Character::GetCharAI()->m_w5 (AIProps +0x14),
//     the combat music switch (Level::m_levelConfig (+0x38) and LevelConfig::m_combatMusicEnabled (+0x1c8), the
//     threshold int at Arrays::DesignSettingsTable::members +0x14, VoxSoundManager's bytes +0x31 and +0x32 and its
//     SetMusicState("combat")), the current level read through Singleton<Application>::s_inst (Application has no
//     accessor for it), and the post-trace line.
// Written: the AISDefault hook, the isTracingAggroCount pre-trace and the m_aggroCount increment, which the original
// does for every owner.
void AISPlayer::OnAggro(Character* c) {
    AISDefault::OnAggro(c);
    DebugSwitches::s_inst.load();
    const bool tracePre = DebugSwitches::s_inst.GetSwitch("isTracingAggroCount");
    if (tracePre) {
        std::fprintf(stderr, "OnAggro-Pre - AggroCount: %d\t AggroForMusic:%d\n", m_aggroCount, m_aggroForMusic);
    }
    m_aggroCount += 1;
}

// 0x3dde48 _ZN9AISPlayer9OnDeAggroEP9Character AISPlayer::OnDeAggro(Character*)
// PARTIAL: the body below the aggro counter update is not written, for the same two reasons as OnAggro (the
// !GetOnline()->isOnlineGame_ gate, and the current level and VoxSoundManager state). Not written, inside the gate:
// m_aggroForMusic -= the aggressor's Character::GetCharAI()->m_w5; when the music value reaches 0 and the music is
// not in combat mode (VoxSoundManager byte +0x31), SetMusicState("ambient") and, when byte +0x32 is set, PlayMusic
// of the level's ambient sound id (Level::unk_120, +0x120), with +0x31 set to 1; when m_aggroCount is 0 and byte
// +0x32 is clear, unk_c4 is cleared (end = begin); and the post-trace line.
// Written: the AISDefault hook, the isTracingAggroCount pre-trace and the m_aggroCount decrement.
void AISPlayer::OnDeAggro(Character* c) {
    AISDefault::OnDeAggro(c);
    DebugSwitches::s_inst.load();
    const bool tracePre = DebugSwitches::s_inst.GetSwitch("isTracingAggroCount");
    if (tracePre) {
        std::fprintf(stderr, "OnDeAggro-Pre - AggroCount: %d\t AggroForMusic:%d\n", m_aggroCount, m_aggroForMusic);
    }
    m_aggroCount -= 1;
}

// 0x3de100 _ZN9AISPlayerD2Ev AISPlayer::~AISPlayer() (base-object copy)
// SKIPPED: a second copy of the destructor above (same vtable stores, same vector release, same CharAIScript call).
// One C++ destructor serves both forms, so the body is defined once at 0x3dd99c.
