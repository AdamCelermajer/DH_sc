# TargetListBackup

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#37**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `TargetListBackup` | bindFunction | `GameObject::_TargetListBackup` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105358 |
| `TargetListBackup` | bindMethod | `GameObject::_TargetListBackup` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105363 |

Address-identified callback: `GameObject::_TargetListBackup` at `0x00390bd4`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c`, unified lines 107810-107827.
Recovered declaration: `int __fastcall GameObject::_TargetListBackup( GameObject *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: optional string tag4; other types default; unit/scale backup key. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c:14`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Optional string arg1 chooses backup name; absent/nonstring uses "default". BackupResults creates/accesses named backup, clears old contents, copies current queue object identities in underlying deque order, reserves count, resets backup iteration cursors. Does not pop/drain live queue. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c:13`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c:14`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/004a/004a36b0.c:38`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c:13`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c:14`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/004a/004a36b0.c:38`.
**State, timers and events:**
- PROVEN: No Lua value; named identity backup replaced, live queue unchanged; no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c:13`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c:14`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/004a/004a36b0.c:38`.
**Return:**
- PROVEN: No Lua value; named identity backup replaced, live queue unchanged; no timer/event. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c:13`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c:14`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c:15`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/004a/004a36b0.c:38`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** character_skill_native_v5.cpp:111-113 snapshots complete Target24 heap records; source BackupObjectList stores object identities only and owns handle retention. Port query converts its snapshots through live resolver, but lifetime/identity retention parity with original handles needs confirmation.

**Concrete behavioral difference:** character_skill_native_v5.cpp:111-113 snapshots complete Target24 heap records; source BackupObjectList stores object identities only and owns handle retention. Port query converts its snapshots through live resolver, but lifetime/identity retention parity with original handles needs confirmation.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: queue[A,B]; TargetListBackup("x") Backup x contains A,B; queue still A,B. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c`:13, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c`:14, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c`:16, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/004a/004a36b0.c`:38.
2. **PROVEN:** Given: backup x[A,B], current queue empty; TargetListBackup("x") Replaces x with empty snapshot. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c`:13, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c`:14, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c`:16, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/004a/004a36b0.c`:38.
3. **PROVEN:** Given: queue[A]; TargetListBackup(7) Writes backup "default" containing A. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c`:13, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c`:14, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c`:15, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00390bd4.c`:16, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/004a/004a36b0.c`:38.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **14** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:173` | outside scoped roots | `TargetListBackup()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:129` | outside scoped roots | `TargetListBackup()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:65` | player_skill_lua | `TargetListBackup("MaxAngleSearch")` | `[{"index":1,"expression":"\"MaxAngleSearch\"","literal_tokens":["\"MaxAngleSearch\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:62` | player_skill_lua | `TargetListBackup("MaxAngleSearch")` | `[{"index":1,"expression":"\"MaxAngleSearch\"","literal_tokens":["\"MaxAngleSearch\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_coldray.luac:60` | player_skill_lua | `TargetListBackup("MaxAngleSearch")` | `[{"index":1,"expression":"\"MaxAngleSearch\"","literal_tokens":["\"MaxAngleSearch\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_paladin_divine_judgement.luac:64` | player_skill_lua | `TargetListBackup("MaxAngleSearch")` | `[{"index":1,"expression":"\"MaxAngleSearch\"","literal_tokens":["\"MaxAngleSearch\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_jump_kick.luac:59` | player_skill_lua | `TargetListBackup("MaxAngleSearch")` | `[{"index":1,"expression":"\"MaxAngleSearch\"","literal_tokens":["\"MaxAngleSearch\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_vicious_strike.luac:67` | player_skill_lua | `TargetListBackup("MaxAngleSearch")` | `[{"index":1,"expression":"\"MaxAngleSearch\"","literal_tokens":["\"MaxAngleSearch\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:45` | outside scoped roots | `TargetListBackup("breath_sweep")` | `[{"index":1,"expression":"\"breath_sweep\"","literal_tokens":["\"breath_sweep\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_breathsweep.luac:239` | outside scoped roots | `TargetListBackup("breath_sweep")` | `[{"index":1,"expression":"\"breath_sweep\"","literal_tokens":["\"breath_sweep\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:50` | outside scoped roots | `TargetListBackup("insideSweep")` | `[{"index":1,"expression":"\"insideSweep\"","literal_tokens":["\"insideSweep\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_insidesweep.luac:127` | outside scoped roots | `TargetListBackup("insideSweep")` | `[{"index":1,"expression":"\"insideSweep\"","literal_tokens":["\"insideSweep\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:47` | outside scoped roots | `TargetListBackup("insideSweep")` | `[{"index":1,"expression":"\"insideSweep\"","literal_tokens":["\"insideSweep\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_outsidesweep.luac:124` | outside scoped roots | `TargetListBackup("insideSweep")` | `[{"index":1,"expression":"\"insideSweep\"","literal_tokens":["\"insideSweep\""]}]` |

## Related natives

`GameObject::_TargetListBackup`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
