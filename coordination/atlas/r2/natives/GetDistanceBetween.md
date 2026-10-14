# GetDistanceBetween

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PORTED**.

Engine functions to implement next ranking: **#28**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `GetDistanceBetween` | bindFunction | `GameObject::_GetDistanceBetween` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105227 |

Address-identified callback: `GameObject::_GetDistanceBetween` at `0x00391438`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`, unified lines 108032-108103.
Recovered declaration: `void __fastcall GameObject::_GetDistanceBetween( GameObject *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1,2: string tag4; unit/scale object names; result squared raw game coordinates. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:31`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Requires >=2 string arguments; resolves both object names via ObjectManager even if first is missing. If both exist, returns raw squared 3D position distance; if either missing returns-1. Wrong types/too few arguments return no value. Registration audit corrected original global binding with nullptr receiver; this is not IsInRange. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:28`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:29`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:30`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:35`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:40`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:41`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:47`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:52`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:53`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:28`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:29`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:30`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:35`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:40`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:41`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:47`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:52`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:53`.
**State, timers and events:**
- PROVEN: Read-only callback; starts no timers and raises no events. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:28`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:29`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:30`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:35`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:40`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:41`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:47`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:52`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:53`.
**Return:**
- PROVEN: One Lua value as described; no state mutation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:28`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:29`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:30`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:35`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:40`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:41`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:47`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:52`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c:53`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PORTED:** character_spatial_bindings.cpp:43-55 implements names/type gate, both synchronous resolutions and squared distance; reusable callback path parity supported, no runtime test.
- Source evidence: `port/level-world/character_spatial_bindings.cpp`:48 — `if(!valid(source)||a[0].reserved||a[1].reserved||!a[0].text||!a[1].text||!out||!capacity)return fail(error,ec,"Malformed GetDistanceBetween receiver/output");`.
- Source evidence: `port/level-world/character_spatial_bindings.cpp`:59 — `const char* names[]={"GetPosition","GetDistanceFrom","GetDistanceBetween"};`.

**Port comparison:** character_spatial_bindings.cpp:43-55 implements names/type gate, both synchronous resolutions and squared distance; reusable callback path parity supported, no runtime test.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: A=(0,0,0), B=(3,4,0); GetDistanceBetween("A","B") Returns25, not5. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:28, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:29, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:30, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:35, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:40, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:41, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:47, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:52, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:53.
2. **PROVEN:** Given: A missing, B valid; GetDistanceBetween("A","B") Still resolves B, returns-1. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:28, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:29, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:30, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:35, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:40, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:41, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:47, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:52, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:53.
3. **PROVEN:** Given: GetDistanceBetween("A",7) No Lua result. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:28, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:29, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:30, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:35, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:40, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:41, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:47, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:52, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00391438.c`:53.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **25** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:302` | outside scoped roots | `GetDistanceBetween(GetName(), target:GetName())` | `[{"index":1,"expression":"GetName()","literal_tokens":[]},{"index":2,"expression":"target:GetName()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/darkqueen1_core.luac:305` | outside scoped roots | `GetDistanceBetween(GetName(), target:GetName())` | `[{"index":1,"expression":"GetName()","literal_tokens":[]},{"index":2,"expression":"target:GetName()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:258` | outside scoped roots | `GetDistanceBetween(GetName(), target:GetName())` | `[{"index":1,"expression":"GetName()","literal_tokens":[]},{"index":2,"expression":"target:GetName()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/flaminite_core.luac:261` | outside scoped roots | `GetDistanceBetween(GetName(), target:GetName())` | `[{"index":1,"expression":"GetName()","literal_tokens":[]},{"index":2,"expression":"target:GetName()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:296` | outside scoped roots | `GetDistanceBetween(GetName(), target:GetName())` | `[{"index":1,"expression":"GetName()","literal_tokens":[]},{"index":2,"expression":"target:GetName()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:299` | outside scoped roots | `GetDistanceBetween(GetName(), target:GetName())` | `[{"index":1,"expression":"GetName()","literal_tokens":[]},{"index":2,"expression":"target:GetName()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee_vip_escort_big.luac:79` | outside scoped roots | `GetDistanceBetween(GetName(), player.obj:GetName())` | `[{"index":1,"expression":"GetName()","literal_tokens":[]},{"index":2,"expression":"player.obj:GetName()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/vip_core.luac:166` | outside scoped roots | `GetDistanceBetween(GetName(), player:GetName())` | `[{"index":1,"expression":"GetName()","literal_tokens":[]},{"index":2,"expression":"player:GetName()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/vip_core.luac:212` | outside scoped roots | `GetDistanceBetween(GetName(), monster:GetName())` | `[{"index":1,"expression":"GetName()","literal_tokens":[]},{"index":2,"expression":"monster:GetName()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:189` | outside scoped roots | `GetDistanceBetween(sk_spawn_points[1], target:GetName())` | `[{"index":1,"expression":"sk_spawn_points[1]","literal_tokens":["1"]},{"index":2,"expression":"target:GetName()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_core.luac:192` | outside scoped roots | `GetDistanceBetween(obj, target:GetName())` | `[{"index":1,"expression":"obj","literal_tokens":[]},{"index":2,"expression":"target:GetName()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:217` | outside scoped roots | `GetDistanceBetween(sk_spawn_points[1], GetSelf():GetName())` | `[{"index":1,"expression":"sk_spawn_points[1]","literal_tokens":["1"]},{"index":2,"expression":"GetSelf():GetName()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small.luac:218` | outside scoped roots | `GetDistanceBetween(target:GetName(), GetSelf():GetName())` | `[{"index":1,"expression":"target:GetName()","literal_tokens":[]},{"index":2,"expression":"GetSelf():GetName()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:127` | outside scoped roots | `GetDistanceBetween(sk_spawn_points[1], GetSelf():GetName())` | `[{"index":1,"expression":"sk_spawn_points[1]","literal_tokens":["1"]},{"index":2,"expression":"GetSelf():GetName()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/sandworm_small_core.luac:128` | outside scoped roots | `GetDistanceBetween(target:GetName(), GetSelf():GetName())` | `[{"index":1,"expression":"target:GetName()","literal_tokens":[]},{"index":2,"expression":"GetSelf():GetName()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:320` | act1_level_boss_script_closure | `GetDistanceBetween(sk_spawn_points[1], target:GetName())` | `[{"index":1,"expression":"sk_spawn_points[1]","literal_tokens":["1"]},{"index":2,"expression":"target:GetName()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:323` | act1_level_boss_script_closure | `GetDistanceBetween(obj, target:GetName())` | `[{"index":1,"expression":"obj","literal_tokens":[]},{"index":2,"expression":"target:GetName()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/sandworm_spit.luac:50` | outside scoped roots | `GetDistanceBetween(GetName(), g_target:GetName())` | `[{"index":1,"expression":"GetName()","literal_tokens":[]},{"index":2,"expression":"g_target:GetName()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_1.luac:39` | act1_level_boss_script_closure | `GetDistanceBetween(GetName(), target:GetName())` | `[{"index":1,"expression":"GetName()","literal_tokens":[]},{"index":2,"expression":"target:GetName()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swamp_king_attack_2.luac:39` | act1_level_boss_script_closure | `GetDistanceBetween(GetName(), target:GetName())` | `[{"index":1,"expression":"GetName()","literal_tokens":[]},{"index":2,"expression":"target:GetName()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/swampking_atk_spit.luac:50` | act1_level_boss_script_closure | `GetDistanceBetween(GetName(), g_target:GetName())` | `[{"index":1,"expression":"GetName()","literal_tokens":[]},{"index":2,"expression":"g_target:GetName()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:97` | outside scoped roots | `GetDistanceBetween(g_target_list[currIndex]:GetName(), g_bomb_spawns[k]:GetName())` | `[{"index":1,"expression":"g_target_list[currIndex]:GetName()","literal_tokens":[]},{"index":2,"expression":"g_bomb_spawns[k]:GetName()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/winddragon_aoebombs.luac:101` | outside scoped roots | `GetDistanceBetween(g_target_list[currIndex]:GetName(), g_bomb_spawns[k]:GetName())` | `[{"index":1,"expression":"g_target_list[currIndex]:GetName()","literal_tokens":[]},{"index":2,"expression":"g_bomb_spawns[k]:GetName()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/worm_attack_1.luac:39` | outside scoped roots | `GetDistanceBetween(GetName(), target:GetName())` | `[{"index":1,"expression":"GetName()","literal_tokens":[]},{"index":2,"expression":"target:GetName()","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/worm_attack_2.luac:39` | outside scoped roots | `GetDistanceBetween(GetName(), target:GetName())` | `[{"index":1,"expression":"GetName()","literal_tokens":[]},{"index":2,"expression":"target:GetName()","literal_tokens":[]}]` |

## Related natives

`GameObject::_IsInRange`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
