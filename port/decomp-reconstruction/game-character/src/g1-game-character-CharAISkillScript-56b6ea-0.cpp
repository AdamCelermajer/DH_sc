// Batch g1-game-character-CharAISkillScript-56b6ea-0 (CharAISkillScript, 16 functions). Declarations are in dh2/CharAISkillScript.hpp.
// Offsets in comments are the original 32-bit offsets inside CharAISkillScript (this) unless a line says Character or CharAI.
//
// Status by function (the ledger rows in game-character/ledger.d/g1-game-character-CharAISkillScript-56b6ea-0.csv repeat them):
//   reconstructed: GetCooldown.
//   partial:       the constructor, the D1 destructor, both SpawnProjectile overloads, every Lua-driven hook, both projectile
//                  callbacks and the GetInfo result read.
//   skipped:       the D0 deleting destructor (compiler-generated from delete).
//
// Why most bodies are partial: every hook builds sfc::script::lua::Arguments and sfc::script::lua::ReturnValues (a local,
// or the embedded Arguments at +0x0c), calls LuaScript::Call with them and reads the returned Value vector. The Value
// stride is 112 bytes in the original, so the count of returned values is (end - begin) / 112. Those three classes have no
// header in the tree (dh2/sfc/script/lua/ holds only UserData.hpp), so none of those lines can be written. The header keeps
// the Arguments as pad_0c[8].
//
// What is written exactly: the no-script path of each hook. When CharAI::m_script is null the original constructs and
// destroys the ReturnValues and returns the value given below, so those paths are reconstructed.

#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "core-platform/memory.hpp"            // gAssertLevel
#include "dh2/CharAI.hpp"
#include "dh2/CharAISkillScript.hpp"
#include "dh2/CharTimers.hpp"
#include "dh2/Character.hpp"
#include "pydata-accessors/w7_arrays_00.hpp"   // Arrays::ProjectileTable::size

namespace {

// The original's assert file name, as the pseudocode spells it.
const char* const kSkillScriptHeader =
    "..\\..\\project_vs2005\\Game/..\\..\\sources\\Game\\Objects\\Characters\\AI\\/CharAISkillScript.h";

// The original ASSERT(expr) macro. Level 2 stops the process (the original stores to address 0); level 1 prints the
// message; level 0 is silent. Callers go on after a failed assert, as the original does.
void AssertFailed(const char* expr, int line) {
    if (gAssertLevel == 2) {
        std::abort();
    } else if (gAssertLevel == 1) {
        std::fprintf(stderr, "ASSERT(%s) FAILED: %s:%d\n", expr, kSkillScriptHeader, line);
    }
}

// The live Lua script of the owning character: CharAI::m_script (CharAI +0x1c, Character +0x3e4 in the original). Null until
// the script process has loaded. The CharAI qualification is required: GameObject also has an m_script (+0x300).
CharAIScript* LiveScript(const CharAISkillScript* skill) {
    return skill->m_character->CharAI::m_script;
}

}  // namespace

// 0x3cc238 CharAISkillScript::~CharAISkillScript()   (D1, complete-object destructor)
// The embedded Arguments (m_arguments, +0x0c) is destroyed after this body, as the original's ~Arguments call does.
CharAISkillScript::~CharAISkillScript() {}

// 0x3cc448 CharAISkillScript::~CharAISkillScript()   (D0, deleting destructor)
// SKIPPED: compiler-generated. It calls D1 above and then CustomFree(this), which is what delete on a CharAISkillScript
// pointer emits. Not written.

// 0x3cde2c CharAISkillScript::CharAISkillScript(Character*,char const*,unsigned int)
// The stores, the Arguments constructor (m_arguments), the cooldown slot of -1 and both asserts ("user" then "name", line 46)
// run in the original order; the script name and the skill index are then pushed onto the Lua arguments.
CharAISkillScript::CharAISkillScript(Character* character, const char* scriptName, unsigned int skillIndex)
    : m_character(character),
      m_scriptName(scriptName),
      m_skillIndex(skillIndex),
      m_cooldownTimerId(-1) {
    if (!character) {
        AssertFailed("user", 46);
    }
    if (!scriptName) {
        AssertFailed("name", 46);
    }
    m_arguments.pushString(scriptName);
    m_arguments.pushInteger(static_cast<int>(skillIndex));
}

// 0x3da3d0 CharAISkillScript::GetCooldown()
// The cooldown is the CharTimers slot in m_cooldownTimerId (+0x18). Without a slot (-1) or when the timer lookup fails the
// result is 0. Otherwise the result is 1 - elapsed / interval, both read from the owner's CharTimers subobject (Character +0x3b4).
// The division is done in float and the subtraction in double, as the pseudocode writes "1.0 - (float)(...)".
float CharAISkillScript::GetCooldown() {
    if (m_cooldownTimerId == -1) {
        return 0.0f;
    }
    unsigned int elapsed = 0;
    unsigned int interval = 0;
    if (!m_character->CharTimers::TMR_TimeLeft(static_cast<unsigned int>(m_cooldownTimerId), elapsed, interval)) {
        return 0.0f;
    }
    const float fraction = static_cast<float>(elapsed) / static_cast<float>(interval);
    return static_cast<float>(1.0 - fraction);
}

// 0x3da494 CharAISkillScript::SpawnProjectile(unsigned int,float)
// PARTIAL: the table test is written (a projectile id below Arrays::ProjectileTable::size; otherwise null, which is exact).
// The spawn is not. The original calls ProjectileManager::Spawn(id, m_character, nullptr, _ProjectileCheck, _ProjectileHit,
// this, angle) on Singleton<ProjectileManager>::s_inst (game-world 0x3e6edc, the float overload). That singleton is not
// declared in the tree (SEAM), so a valid id also returns null here.
Projectile* CharAISkillScript::SpawnProjectile(unsigned int projectileId, [[maybe_unused]] float angle) {
    if (projectileId < Arrays::ProjectileTable::size) {
        // SEAM: ProjectileManager::Spawn on Singleton<ProjectileManager>::s_inst, see above.
    }
    return nullptr;
}

// 0x3da510 CharAISkillScript::SpawnProjectile(unsigned int,bool)
// PARTIAL: same as the float overload above, with the flag passed to the bool overload of Spawn (game-world 0x3e701c).
// The spawn waits on the ProjectileManager singleton (SEAM).
Projectile* CharAISkillScript::SpawnProjectile(unsigned int projectileId, [[maybe_unused]] bool flag) {
    if (projectileId < Arrays::ProjectileTable::size) {
        // SEAM: ProjectileManager::Spawn on Singleton<ProjectileManager>::s_inst, see above.
    }
    return nullptr;
}

// 0x3da6c0 CharAISkillScript::OnPostSkill()
// With no script the original does nothing. PARTIAL with a script: it calls the script's SetSkill with the Arguments at +0x0c,
// and when that succeeds empties the returned values and calls OnPostSkill. Waits on the Arguments and ReturnValues headers.
void CharAISkillScript::OnPostSkill() {
    if (!LiveScript(this)) {
        return;
    }
    // PARTIAL: the SetSkill and OnPostSkill calls, see above.
}

// 0x3da794 CharAISkillScript::OnSkill()
// With no script the result is 0. PARTIAL with a script: SetSkill, then the values are emptied and OnSkill runs. A failed call
// gives 0, no returned value gives 1, otherwise the boolean value of the first returned value. Waits on the Arguments and
// ReturnValues headers, so 0 is returned here (the failed-call result).
int CharAISkillScript::OnSkill() {
    if (!LiveScript(this)) {
        return 0;
    }
    // PARTIAL: the SetSkill and OnSkill calls and the result read, see above.
    return 0;
}

// 0x3da8b8 CharAISkillScript::OnPreSkill()
// With no script the result is false. PARTIAL with a script: SetSkill, then OnPreSkill. A failed call gives false, no returned
// value gives true, otherwise the boolean value of the first returned value. Waits on the Lua value headers; false is returned.
bool CharAISkillScript::OnPreSkill() {
    if (!LiveScript(this)) {
        return false;
    }
    // PARTIAL: the SetSkill and OnPreSkill calls and the result read, see above.
    return false;
}

// 0x3da9dc CharAISkillScript::OnSkillCheck_Usable()
// With no script the result is false. PARTIAL with a script: SetSkill, then the function named "OnSkillCheck" (not
// "OnSkillCheck_Usable"). A failed call or no returned value gives false, otherwise the boolean value of the first returned
// value. Waits on the Lua value headers; false is returned.
bool CharAISkillScript::OnSkillCheck_Usable() {
    if (!LiveScript(this)) {
        return false;
    }
    // PARTIAL: the SetSkill and OnSkillCheck calls and the result read, see above.
    return false;
}

// 0x3daafc CharAISkillScript::OnSkillCleanUp()
// With no script the original does nothing. PARTIAL with a script: SetSkill, empty the returned values, then OnSkillCleanUp.
void CharAISkillScript::OnSkillCleanUp() {
    if (!LiveScript(this)) {
        return;
    }
    // PARTIAL: the SetSkill and OnSkillCleanUp calls, see above.
}

// 0x3dabd0 CharAISkillScript::OnSkillUpdate()
// With no script the original does nothing. PARTIAL with a script: SetSkill, empty the returned values, then OnSkillUpdate.
void CharAISkillScript::OnSkillUpdate() {
    if (!LiveScript(this)) {
        return;
    }
    // PARTIAL: the SetSkill and OnSkillUpdate calls, see above.
}

// 0x3daca8 CharAISkillScript::GetInfo(unsigned int,float*)
// With no script the original writes 0 to *out (when out is not null) and does nothing else. PARTIAL with a script: SetSkill,
// then OnSkillInfo with the info id pushed as an integer. When both calls succeed *out is set to 0, and when the first
// returned value is a number n, *out = 1 - elapsed / interval of the CharTimers slot n (TMR_TimeLeft, the same formula as
// GetCooldown). Waits on the Arguments and ReturnValues headers, so *out is left as it is on the script path.
void CharAISkillScript::GetInfo([[maybe_unused]] unsigned int infoId, float* out) {
    if (!LiveScript(this)) {
        if (out) {
            *out = 0.0f;
        }
        return;
    }
    // PARTIAL: the SetSkill and OnSkillInfo calls and the result read, see above.
}

// 0x3dae7c CharAISkillScript::_ProjectileHit(Projectile*,void*)
// The callback's user data is the CharAISkillScript. With no script the result is 1 (the hit is ignored). PARTIAL with a
// script: SetSkill, then OnDelayedSkill with the projectile's hit object (Projectile +0x3cc, pushed as userdata) and the
// projectile pointer. The result is 1 only when the first returned value is the boolean false. A failed call gives 0.
// Waits on the Lua argument and value headers; 0 is returned on the script path.
int CharAISkillScript::_ProjectileHit([[maybe_unused]] Projectile* projectile, void* user) {
    auto* skill = static_cast<CharAISkillScript*>(user);
    if (!LiveScript(skill)) {
        return 1;
    }
    // PARTIAL: the SetSkill and OnDelayedSkill calls and the result read, see above.
    return 0;
}

// 0x3daff4 CharAISkillScript::_ProjectileCheck(Projectile*,void*)
// Same shape as _ProjectileHit, with the function named "OnDelayedSkillCheck". With no script the result is 1. PARTIAL with a
// script: the result is 1 only when the first returned value is the boolean false, and a failed call gives 0.
int CharAISkillScript::_ProjectileCheck([[maybe_unused]] Projectile* projectile, void* user) {
    auto* skill = static_cast<CharAISkillScript*>(user);
    if (!LiveScript(skill)) {
        return 1;
    }
    // PARTIAL: the SetSkill and OnDelayedSkillCheck calls and the result read, see above.
    return 0;
}

// 0x3db16c CharAISkillScript::OnSkillCheck_Active()
// With no script the result is false. PARTIAL with a script: SetSkill, then "OnSkillCheck". A failed call, or fewer than two
// returned values, gives false; otherwise the result is the boolean value of the second returned value (byte offset 112 of
// the Value vector). Waits on the Lua value headers; false is returned.
bool CharAISkillScript::OnSkillCheck_Active() {
    if (!LiveScript(this)) {
        return false;
    }
    // PARTIAL: the SetSkill and OnSkillCheck calls and the second-value read, see above.
    return false;
}
