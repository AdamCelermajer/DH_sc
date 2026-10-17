# F_DotAttack and its direct result continuation

This unit reconstructs the complete normal caller of
`Character::F_DotAttack(AttackResult&,Character*,Character*,int,int)`,412 bytes
at `0x3b2e68`, with its retained nonnull actor domain. Original null diagnostic,
logger/crash behavior and stack-corruption traps are excluded port domains.
The manifest credits **one new caller, zero complete dependency bodies**.

The `Runtime` supplies a separate bounded dependency adapter for only the
`_F_CalculateResult` mask `0x20080000` continuation. It does not claim the full
2096-byte calculation body. No existing frozen combat/Debug/runtime file is
changed, and this unit is not wired into the APK.

## Caller and mandatory Debug effects

`r7/r5/r11` retain attacker/defender/raw amount; the element is fetched from
the original caller's stack. The caller captures DebugSwitches singleton into
`sl` at `0x3b2eac`, then unconditionally performs:

1. `DebugSwitches::load()` at `0x3b2eb4`.
2. Genuine string construction for **`isTracingChar_Attack`** at `0x3b2ec8`.
3. `GetSwitch` on the same captured owner/string at `0x3b2ed4`; its return is
   discarded, including for zero/negative amounts.
4. String block destruction at `0x3b2edc`.
5. `_F_CalculateResult` at `0x3b2f08` with mask `0x20080000`, category `-1`,
   original element and raw amount.

The literal is `0x8c3cc0` (its terminated bytes are hashed). This particular
source string-release dependency is `_String_base::_M_deallocate_block`,52B
at `0x3139ac`; it is not the regeneration caller's different destructor.
Native owned std::string storage replaces the old ARM allocator/layout; an
original allocator body is not credited.

## Exact direct-mask calculation adapter

The original calculation body starts with a profiling call. Both actual
Push/PopProfilingContext symbols are proven 4-byte `bx lr` functions; the
oracle executes them rather than substituting a successful observer for a
nonempty function.

`_F_ResetResult` precedes mask/category/element stores. `CF_SetCombatants` then
publishes attacker/defender to shared context, performs two cached Level19
reads, captures their wrap32 difference, clears critical, stores reverse
difference/element/offhand/magic/difference, then clears blocked. The native
context separately projects source offsets `+0x1c..0x33`. The adapter reads
the real normalized 224-word resolved sheets. Original valid cached getters
are pure offset-table reads; no uncached property recomputation is substituted.

After those context effects, the source calculation calls Debug load/string/
GetSwitch/release **again** at `0x3b277c..0x3b27a4`. This second singleton is
selected freshly; it is not inherited from the outer caller's captured owner.
The existing arithmetic kernel alone omits this query, so invoking it directly
would not provide the required side effects. The adapter executes the actual
maintained Debug Runtime and real provider services at both phases.

The direct branch (`0x3b2d60`) freshly reads context combatant identities after
Debug. Either null writes amount0. With both nonnull, the original
CF__CalcDamage type3 subpath (`0x3b23f0`) returns the raw direct amount without
dereferencing the two objects or consuming RNG. The adapter reuses that verified
`dh2_combat_damage` arithmetic; it does not approximate resistance, reroll damage,
query equipment or apply HP. Other AttackResult fields retain the reset values
and original mask/category/element. Full F_ApplyResult is a subsequent dependency.

## Ownership and errors

The public caller exposes explicit borrowed service boundaries. The reusable
Runtime adapter owns genuine retained strings, uses the supplied original
Debug Runtime/Globals and IO/persistence services, and owns no fabricated Debug
query answers. Source argument identities and callback bindings are captured;
live context and singleton selection remain fresh at their source points.

One owning thread retains Actor descriptors/224-word sheets, shared context,
random, Debug controls, retired owners and resource backing through synchronous
return. Controls/output are aligned and disjoint. Self attacks may share the
same sheet. Provider-driven overwrite of control/output, destruction of active
backing or reentry on the same Runtime/context/output is excluded; independent
owners can nest, and genuine internal Debug save/load/query recursion is allowed.

Errors preserve prior reset/context/debug/map/file effects. No rollback or extra
string destruction is invented after query failure. Runtime retains strings
left by failed operations. Unsupported actor resolution stops after the actual
outer Debug phase and calculation reset/context identity writes. Original
null-argument diagnostics are explicitly rejected before the caller executes.

## Proof and real filesystem composition

The original ARM comparison executes the complete valid-nonnull F_DotAttack
caller and its exact direct-mask calculation continuation. Original result reset,
CF_SetCombatants, cached getter and CF__CalcDamage direct instructions run.
Mandatory Debug phases are hooked as named provider fixtures for this CPU proof;
neither block is skipped. No PLT/numeric helper is guessed or modeled. The native
side runs this new Runtime plus the actual owned Debug map/string code and existing
arithmetic. Caller-only cases separately compare singleton capture and discarded
raw query words with explicit calculation fixtures; they are not credited as
calculation-body comparisons.

The host composition also uses the frozen production native_debug_files backend
with real binary reads/writes/close and unchanged 665-byte configuration. Positive
damage passes both Debug phases, executes the five real nested live-map saves on
initial load, inserts the previously absent Attack tracing key, and produces the
actual direct result. A persisted true tracing key remains true and is discarded
as a calculation decision. Missing-file, invalid-header and returned-error-after-
real-close cases verify retained file/guard/map effects and the source prefix.

The gate has 104 direct-mask original comparisons,16 separate caller-capture
comparisons,54 host behavior cases,14 guards,10 returned-error/exception cases,
and5 real-file cases. Per-instruction coverage excludes intercepted provider
entries and their skipped bodies; basic-block coverage is not used to claim
execution of a hooked block's tail.

Run:

```powershell
python port/level-world/tests/run_character_dot_attack_host.py --compiler C:/Users/noamc/.local/mingw/mingw64/bin/g++.exe --original-elf C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/test_strategy/libDungeonHunter2.so --cache C:/Users/noamc/Documents/Codex/2026-10-02/contineu-from-where-they-left-off/work/cache/files --output port/level-world/build/character-dot-attack/host.exe --report port/level-world/build/character-dot-attack/validation.json
```
