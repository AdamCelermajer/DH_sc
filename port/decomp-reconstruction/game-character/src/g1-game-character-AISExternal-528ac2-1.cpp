// Batch g1-game-character-AISExternal-528ac2-1 (part 2 of 2 of AISExternal): InitVCB, the three destructor
// symbols, the two constructors, and the event hooks from OnCombatResults through OnDied (13 functions).
// Declarations come from dh2/AISExternal.hpp, which already declares every method in this batch.
//
// Two blockers keep some functions partial. Each affected function names the one that applies.
//  - sfc::script::lua::Arguments has no header in this tree (dh2/sfc/script/lua/Arguments.hpp is absent). The Lua
//    hooks build one, push their argument (pushPointer or pushUserData), call LuaScript::Call(name, args) and destroy
//    it. Those calls are not written.
//  - AISDefault declares no constructor that takes the skip-bindings flag. The AISExternal(bool) constructors pass
//    their flag through the AISDefault base to CharAIScript(bool). They are not written until dh2/AISDefault.hpp
//    declares AISDefault(bool).

#include "dh2/AISExternal.hpp"

// 0x3dcec8 AISExternal::InitVCB(void)
// Calls the base InitVCB (AISDefault, 0x3dc7d8) first. Then each Lua hook that the external script defines sets its
// bit in the callback mask unk_b8 (+0xb8). LuaScript::IsInVFTable reports whether the name is in the function table.
// The last test (OnMasterInMeleeRange, bit 0x200) is the value the function returns, as in the original.
int AISExternal::InitVCB() {
    AISDefault::InitVCB();
    unk_b8 |= IsInVFTable("OnUpdate") ? 0x1u : 0u;                       // bit 0: gates the Lua OnUpdate call
    unk_b8 |= IsInVFTable("OnFriendSpotted") ? 0x2u : 0u;                // bit 1: gates the Lua OnFriendSpotted call
    unk_b8 |= IsInVFTable("OnTargetOutOfRange") ? 0x4u : 0u;             // bit 2
    unk_b8 |= IsInVFTable("OnTargetInRangedRange") ? 0x8u : 0u;          // bit 3
    unk_b8 |= IsInVFTable("OnTargetInCloseRange") ? 0x10u : 0u;          // bit 4
    unk_b8 |= IsInVFTable("OnTargetInMeleeRange") ? 0x20u : 0u;          // bit 5
    unk_b8 |= IsInVFTable("OnMasterOutOfRange") ? 0x40u : 0u;            // bit 6
    unk_b8 |= IsInVFTable("OnMasterInRangedRange") ? 0x80u : 0u;         // bit 7
    unk_b8 |= IsInVFTable("OnMasterInCloseRange") ? 0x100u : 0u;         // bit 8
    const int result = IsInVFTable("OnMasterInMeleeRange") ? 0x200 : 0;  // bit 9
    unk_b8 |= static_cast<uint32_t>(result);
    return result;
}

// 0x3dd060 AISExternal::~AISExternal(void)   (D1, complete object)
// 0x3dd0b0 AISExternal::~AISExternal(void)   (D2, base object; the same bytes)
// The original body is the implicit base chain. It stores the AISDefault vtable (_ZTV10AISDefault + 8), with
// AISDefault's destructor inlined, and calls CharAIScript::~CharAIScript. An empty user-declared body compiles to
// the same chain: AISDefault's destructor runs, then CharAIScript's.
AISExternal::~AISExternal() {}

// 0x3dd094 AISExternal::~AISExternal(void)   (D0, deleting destructor)
// SKIPPED: the compiler emits the deleting destructor for `delete` of an AISExternal. The original calls D1 and then
// CustomFree(this). Nothing is written here.

// 0x3dd0e4 AISExternal::AISExternal(bool)   (C1)
// 0x3dd128 AISExternal::AISExternal(bool)   (C2)
// PARTIAL: not written. The original calls CharAIScript C2 (0x3d8fb0) with the flag, reached through the inlined
// AISDefault base. It then stores the vptr (_ZTV11AISExternal + 8), and clears unk_c0 (+0xc0), unk_b8 (+0xb8) and
// unk_bc (+0xbc). The intended definition is
//     AISExternal::AISExternal(bool skipBindings) : AISDefault(skipBindings) { unk_c0 = 0; unk_b8 = 0; unk_bc = 0; }
// It needs AISDefault(bool) declared in dh2/AISDefault.hpp, which is absent (layout request).

// 0x3dd250 AISExternal::OnCombatResults(GameObject*, void*)
// PARTIAL: the Lua call is not written. The original builds an sfc::script::lua::Arguments, pushes the results pointer
// (pushPointer(a3)), calls LuaScript::Call(this, "OnCombatResults", args) and destroys the Arguments. Waits on
// dh2/sfc/script/lua/Arguments.hpp (absent).
void AISExternal::OnCombatResults(GameObject* /*other*/, void* /*results*/) {}

// 0x3dd2a0 AISExternal::OnProjectileHit(GameObject*)
// PARTIAL: the base hook runs. The Lua call is not written: the original pushes target as userdata into an
// sfc::script::lua::Arguments and calls LuaScript::Call(this, "OnProjectileHit", args). Waits on
// dh2/sfc/script/lua/Arguments.hpp (absent).
void AISExternal::OnProjectileHit(GameObject* target) {
    AISDefault::OnProjectileHit(target);
}

// 0x3dd2f4 AISExternal::OnEnemySpotted(Character*)
// PARTIAL: not written. The original pushes c as userdata into an sfc::script::lua::Arguments and calls
// LuaScript::Call(this, "OnEnemySpotted", args) unconditionally. Waits on dh2/sfc/script/lua/Arguments.hpp (absent).
void AISExternal::OnEnemySpotted(Character* /*c*/) {}

// 0x3dd344 AISExternal::OnNeutralSpotted(Character*)
// PARTIAL: not written. The original pushes c as userdata and calls LuaScript::Call(this, "OnNeutralSpotted", args)
// unconditionally. Waits on dh2/sfc/script/lua/Arguments.hpp (absent).
void AISExternal::OnNeutralSpotted(Character* /*c*/) {}

// 0x3dd394 AISExternal::OnFriendSpotted(Character*)
// PARTIAL: the gate is written. Only when bit 1 of unk_b8 (0x2, set by InitVCB when OnFriendSpotted is in the function
// table) does the original push c as userdata and call LuaScript::Call(this, "OnFriendSpotted", args). That call is
// not written. It waits on dh2/sfc/script/lua/Arguments.hpp (absent).
void AISExternal::OnFriendSpotted(Character* /*c*/) {
    if ((unk_b8 & 0x2u) != 0) {
        // Lua call not written (see above).
    }
}

// 0x3dd3f0 AISExternal::OnRevived(GameObject*)
// PARTIAL: not written. The original pushes reviver as userdata and calls LuaScript::Call(this, "OnRevived", args)
// unconditionally. Waits on dh2/sfc/script/lua/Arguments.hpp (absent).
void AISExternal::OnRevived(GameObject* /*reviver*/) {}

// 0x3dd440 AISExternal::OnDied(GameObject*)
// PARTIAL: not written. The original pushes killer as userdata and calls LuaScript::Call(this, "OnDied", args)
// unconditionally. Waits on dh2/sfc/script/lua/Arguments.hpp (absent).
void AISExternal::OnDied(GameObject* /*killer*/) {}
