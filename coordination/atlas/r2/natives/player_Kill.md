# player:Kill

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#219**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `Kill` | bindFunction | `Character::_Kill` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132388 |
| `Kill` | bindMethod | `Character::_Kill` | `_ZN9Character14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:132393 |

Address-identified callback: `Character::_Kill` at `0x003b7e40`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c`, unified lines 134105-134120.
Recovered declaration: `int __fastcall Character::_Kill(Character *this, const sfc::script::lua::Arguments *a2, v2Controller **a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: optional userdata tag7; every other type null killer; unit/scale killer attribution. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c:10`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Optional arg1 userdata tag7 is killer attribution; missing/other tag passes null. Calls controller Cmd_Kill with flagfalse; controller immediately dispatches Character Kill without blocked/locked gate. Character Kill first checks virtual dead: duplicate returns. Otherwise sets dead byte1, HP property36=0. Player branch increments death stat25 and local thresholds10/50/100 trophies, online local-player query then returns. Nonplayer game type0 drops loot. Killer stored when supplied; aggression recipients get event4 and player kill stats23/24/trophies100/500/1000/2000. Eligible killer gets XP; non-summoned/non-suppressed actor emits async KillXEnemies/ClearEnemies and template events when template!=-1. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c:11`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c:13`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c:14`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/0040570c.c:3`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a5b18.c:66`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c:11`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c:13`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c:14`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/0040570c.c:3`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a5b18.c:66`.
**State, timers and events:**
- PROVEN: No Lua result; death/HP/stat/loot/XP/quest events conditional as above; duplicate dead gate prevents repeated side effects. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c:11`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c:13`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c:14`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/0040570c.c:3`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a5b18.c:66`.
**Return:**
- PROVEN: No Lua result; death/HP/stat/loot/XP/quest events conditional as above; duplicate dead gate prevents repeated side effects. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c:11`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c:13`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c:14`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/0040570c.c:3`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a5b18.c:66`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** character_kill_live_v21.cpp and status/loot consumers retain parts of death flow, but generic Lua userdata-attributed controller path for every actor is not installed in CharacterScriptObjects (:144-146). Normal player-death/loot verification is separate from this source contract.
- Source evidence: `port/windows-foundation/features/loot/source_death_reward_binding_v1.cpp`:20 — `error = "Required retained source Kill services and owner graph";`.
- Source evidence: `port/level-world/character_ai_groups_v87.cpp`:63 — `bool CharacterAiGroupV87::on_died(Record& r,std::uintptr_t attacker,const Kill& kill,std::string& e){`.
- Source evidence: `port/level-world/character_ai_groups_v87.cpp`:95 — `bool source_character_group_on_died_v87(Record& r,std::uintptr_t group,std::uintptr_t attacker,const CharacterAiGroupV87::Kill& kill,std::string& e){`.
- Source evidence: `port/level-world/character_ai_groups_v87.hpp`:22 — `using Kill=std::function<bool(world::CanonicalCharacterCandidateRecordV60&,std::uintptr_t,std::string&)>;`.
- Source evidence: `port/level-world/character_ai_groups_v87.hpp`:23 — `bool on_died(world::CanonicalCharacterCandidateRecordV60&,std::uintptr_t,const Kill&,std::string&);`.
- Source evidence: `port/level-world/character_ai_groups_v87.hpp`:30 — `std::uintptr_t actual_group,std::uintptr_t actual_attacker,const CharacterAiGroupV87::Kill&,std::string&);`.
- Source evidence: `port/level-world/character_kill_fields_v21.cpp`:4 — `if(produced){e="Character Kill C1 fields cannot replay";return false;}`.
- Source evidence: `port/level-world/character_kill_fields_v21.cpp`:11 — `if(produced){e="Character Kill metadata adoption cannot replay";return false;}`.
- Source evidence: `port/level-world/character_kill_level_events_v23.cpp`:17 — `const auto* constructor=current.level()->constructor_owner_v3();if(!constructor||constructor->phase()!=loader::LevelConstructorPhaseV3::complete){e="Required whole SAME Level C1 before Kill publication";return false;}`.
- Source evidence: `port/level-world/character_kill_level_events_v23.cpp`:21 — `if(!q.event||q.event->kind<1||q.event->kind>4||q.event->reserved||q.event->reserved2||q.event->flag0||q.event->flag1||q.event->network_id!=-1||q.subject!=q.event->level){e="Required exact source stack Kill QE payload";return false;}`.
- Source evidence: `port/level-world/character_kill_live_v21.cpp`:27 — `e="Required SAME produced Character Kill/controller/metadata/property/World graph";return false;`.
- Source evidence: `port/level-world/character_kill_live_v21.cpp`:38 — `if(!b.fields->produced||b.life->dead>1){error_="Required valid live Kill scalar authorities";return false;}`.

**Concrete behavioral difference:** character_kill_live_v21.cpp and status/loot consumers retain parts of death flow, but generic Lua userdata-attributed controller path for every actor is not installed in CharacterScriptObjects (:144-146). Normal player-death/loot verification is separate from this source contract.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: already-dead Character; Kill() No repeated HP/stat/loot/quest event mutation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c`:10, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c`:11, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c`:13, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c`:14, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/0040570c.c`:3, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a5b18.c`:66.
2. **PROVEN:** Given: alive Player with death stat9; Kill() Dead1, HP0, death stat10; local Player unlocks quest_died_10_times, player branch stops before monster loot/events. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c`:10, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c`:11, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c`:13, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c`:14, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/0040570c.c`:3, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a5b18.c`:66.
3. **PROVEN:** Given: alive nonplayer, no killer, game type0; Kill() Dead1/HP0, DropLoot(null), then eligible quest events; no killer-attributed XP. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c`:10, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c`:11, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c`:13, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7e40.c`:14, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0040/0040570c.c`:3, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a5b18.c`:66.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **1** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/vip_core.luac:181` | outside scoped roots | `player:Kill()` | `[]` |

## Related natives

`Character::_Kill`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
