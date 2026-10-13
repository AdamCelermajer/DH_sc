# firstTarget:PlayFX

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#194**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `PlayFX` | bindFunction | `GameObject::_PlayFX` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105403 |
| `PlayFX` | bindMethod | `GameObject::_PlayFX` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105408 |

Address-identified callback: `GameObject::_PlayFX` at `0x003923cc`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`, unified lines 108582-108675.
Recovered declaration: `unsigned int __fastcall GameObject::_PlayFX( unsigned int this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: number tag3 -> unsigned; unit/scale AnimatedEffectTable row. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:40`.
- Lua argument 2..4: optional all-numeric coordinates, only if4+ totalargs; unit/scale raw world point. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:72`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Requires numeric effect ID unsigned < AnimatedEffectTable size. <=3 total args plays receiver-attached set (extra1/2 ignored). >=4 args: if coordinates2..4 all numeric use raw point; otherwise fallback receiver raw position and play world-position set. Manager validates ID, selects random variant only set type2, gets set lifetime/config, stores active set, and chooses contained effect or linked nested set. No Lua effect handle returned. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:32`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:34`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:37`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:39`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:42`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:45`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:46`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:54`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:60`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0049/00495d14.c:31`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:32`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:34`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:37`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:39`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:42`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:45`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:46`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:54`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:60`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0049/00495d14.c:31`.
**State, timers and events:**
- PROVEN: No Lua value; active effect set/variant/nested effect allocation, no wrapper timer. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:32`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:34`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:37`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:39`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:42`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:45`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:46`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:54`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:60`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0049/00495d14.c:31`.
**Return:**
- PROVEN: No Lua value; active effect set/variant/nested effect allocation, no wrapper timer. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:32`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:34`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:37`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:39`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:42`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:45`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:46`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:54`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c:60`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0049/00495d14.c:31`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** Selected Windows skill FX invokes authored events/effect rows; CharacterScriptObjects general0x3923cc missing (:144-146). Original arbitrary receiver-attached vs raw world-position overload and set-variant lifetime ownership cannot be replaced by one spawn-at-player helper.
- Source evidence: `port/windows-foundation/features/bosses/runtime_swampking_core_ai_v1.cpp`:333 — `} else if (contains(skill, "repeat") && contains(skill, "SkillCombatRoll(target)") && contains(skill, "PlayFX(")) {`.
- Source evidence: `port/windows-foundation/features/bosses/runtime_swampking_core_ai_v1.cpp`:337 — `if (!std::regex_search(skill, m, std::regex("PlayFX\\(\\s*GetPyOID\\(\\s*\"AnimatedEffectTable\"\\s*,\\s*\"(\\w+)\"")))`.
- Source evidence: `port/windows-foundation/features/effects/celest_target_fx_dispatch_v1.cpp`:117 — `if (!created) return fail(error, "Original Celest PlayFX did not create a source instance");`.
- Source evidence: `port/windows-foundation/features/enemy_ai/runtime_ranged_skill_monster_v1.cpp`:196 — `if (std::regex_search(callback, fx, std::regex("PlayFX\\(\\s*(\\w+)"))) {`.

**Concrete behavioral difference:** Selected Windows skill FX invokes authored events/effect rows; CharacterScriptObjects general0x3923cc missing (:144-146). Original arbitrary receiver-attached vs raw world-position overload and set-variant lifetime ownership cannot be replaced by one spawn-at-player helper.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: PlayFX(AnimatedEffectTable size) No active FX set; index equals size and fails strict bound. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:32, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:34, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:37, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:39, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:42, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:45, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:46, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:54, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:60, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0049/00495d14.c`:31.
2. **PROVEN:** Given: valid FX row E; PlayFX(E,10,20) Receiver-attached effect path; two coordinate extras ignored. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:32, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:34, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:37, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:39, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:42, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:45, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:46, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:54, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:60, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0049/00495d14.c`:31.
3. **PROVEN:** Given: valid E; PlayFX(E,10,20,30) World-position effect set at(10,20,30). Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:32, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:34, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:37, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:39, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:42, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:45, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:46, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:54, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/003923cc.c`:60, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0049/00495d14.c`:31.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **1** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_fireskullrain.luac:52` | player_skill_lua | `firstTarget:PlayFX(METEOR_FX)` | `[{"index":1,"expression":"METEOR_FX","literal_tokens":[]}]` |

## Related natives

`GameObject::_PlayFX`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
