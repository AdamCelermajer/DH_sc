# HasPath

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#45**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `HasPath` | bindFunction | `GameObject::_HasPath` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105295 |
| `HasPath` | bindMethod | `GameObject::_HasPath` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105300 |

Address-identified callback: `GameObject::_HasPath` at `0x0038e98c`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c`, unified lines 105834-105868.
Recovered declaration: `void __fastcall GameObject::_HasPath( GameObject *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument none: Receiver-only callback; extra Lua arguments ignored; unit/scale none. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c:30`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c:33`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Walks receiver path linked list at +512 and returns Boolean nonempty. An empty sentinel is false; any number of nodes greater than zero is true. Does not infer path from speed/destination. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c:30`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c:33`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c:30`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c:33`.
**State, timers and events:**
- PROVEN: Read-only callback; starts no timers and raises no events. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c:30`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c:33`.
**Return:**
- PROVEN: One Lua value as described; no state mutation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c:30`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c:33`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** Enemy/companion Lua exposes HasPath through retained operation providers; callback catalog alone does not establish the actual receiver linked-list path, and CharacterScriptObjects dispatcher has no 0x38e98c branch (:144). Full arbitrary receiver path ownership not proven.
- Source evidence: `port/windows-foundation/features/companions/live_companion_scripts.cpp`:119 — `{"HasPath",Operation::has_path,true,false},{"GetState",Operation::state,false,false},`.
- Source evidence: `port/windows-foundation/features/companions/source_owner_scripts.cpp`:58 — `{"GetHostPlayer",0x37ca60},{"IsMasterHostPlayer",0x3b6fc4},{"HasPath",0x38e98c},{"GetState",0x3b6d78},`.
- Source evidence: `port/windows-foundation/features/enemy_ai/runtime_ranged_skill_monster_v1.cpp`:221 — `if (next.teleport.candidates.empty() || warps != next.teleport.candidates.size() || !contains(callback, "HasPath()"))`.
- Source evidence: `port/windows-foundation/features/enemy_ai/runtime_ranged_skill_monster_v1.cpp`:222 — `return fail(error, "Teleport callback does not pair each relative MoveTo/HasPath with a WarpTo");`.
- Source evidence: `port/level-world/character_script_session.cpp`:175 — `if(t.commands_refresh&&t.commands_refresh(t.commands_refresh_context))return fail(error,size,"Required live NPC HasPath projection");`.
- Source evidence: `port/level-world/character_script_session.cpp`:176 — `return t.commands&&dh2_character_script_command(t.commands->state,script_has_path,args,count,&t.commands->services,out,capacity,returned)==1?0:fail(error,size,"Native session HasPath delivery failed");`.
- Source evidence: `port/level-world/character_script_session_v2.cpp`:162 — `auto& t=*static_cast<Impl*>(opaque);return t.commands&&dh2_character_script_command(t.commands->state,script_has_path,args,count,&t.commands->services,out,capacity,returned)==1?0:fail(error,size,"Native session HasPath delivery failed");`.
- Source evidence: `port/level-world/character_script_session_v3.cpp`:182 — `auto& t=*static_cast<Impl*>(opaque);return t.commands&&dh2_character_script_command(t.commands->state,script_has_path,args,count,&t.commands->services,out,capacity,returned)==1?0:fail(error,size,"Native session HasPath delivery failed");`.
- Source evidence: `port/level-world/player_target_out_range_v2.hpp`:25 — `bool path=false;if(!s.has_path||s.has_path(s.context,path))return missing("HasPath before movement");`.
- Source evidence: `port/level-world/player_target_out_range_v2.hpp`:30 — `bool path=false;if(!s.has_path||s.has_path(s.context,path))return missing("HasPath after movement");`.

**Concrete behavioral difference:** Enemy/companion Lua exposes HasPath through retained operation providers; callback catalog alone does not establish the actual receiver linked-list path, and CharacterScriptObjects dispatcher has no 0x38e98c branch (:144). Full arbitrary receiver path ownership not proven.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: path sentinel points to itself; HasPath() False. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c`:16, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c`:30, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c`:33.
2. **PROVEN:** Given: path has one node; HasPath() True. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c`:16, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c`:30, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e98c.c`:33.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **32** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:124` | outside scoped roots | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:129` | outside scoped roots | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:134` | outside scoped roots | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:139` | outside scoped roots | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/vip_core.luac:131` | outside scoped roots | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:317` | outside scoped roots | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/madruk_core.luac:340` | outside scoped roots | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/miniboss_state.luac:80` | outside scoped roots | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/monster.luac:110` | act1_level_boss_script_closure | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/rene.luac:84` | act1_level_boss_script_closure | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:59` | act1_level_boss_script_closure | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:64` | act1_level_boss_script_closure | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:69` | act1_level_boss_script_closure | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:74` | act1_level_boss_script_closure | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/witch.luac:182` | act1_level_boss_script_closure | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_summons_00.luac:77` | outside scoped roots | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_summons_00.luac:84` | outside scoped roots | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_summons_00.luac:91` | outside scoped roots | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/darkqueen2_summons_00.luac:98` | outside scoped roots | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/edward_rise.luac:236` | outside scoped roots | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/edward_rise.luac:241` | outside scoped roots | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/edward_rise.luac:246` | outside scoped roots | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/edward_rise.luac:251` | outside scoped roots | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/generic_teleport_skill.luac:42` | outside scoped roots | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/generic_teleport_skill.luac:48` | outside scoped roots | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/generic_teleport_skill.luac:54` | outside scoped roots | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/generic_teleport_skill.luac:60` | outside scoped roots | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:126` | player_skill_lua | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_rise.luac:231` | player_skill_lua | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_rise.luac:236` | player_skill_lua | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_rise.luac:241` | player_skill_lua | `HasPath()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_rise.luac:246` | player_skill_lua | `HasPath()` | `[]` |

## Related natives

`GameObject::_HasPath`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
