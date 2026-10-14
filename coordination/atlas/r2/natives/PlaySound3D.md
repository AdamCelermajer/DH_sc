# PlaySound3D

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **MISSING**.

Engine functions to implement next ranking: **#98**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `PlaySound3D` | bindFunction | `GameObject::_PlaySound3D` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105435 |

Address-identified callback: `GameObject::_PlaySound3D` at `0x0039212c`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`, unified lines 108464-108580.
Recovered declaration: `int __fastcall GameObject::_PlaySound3D( GameObject *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: getString conversion, no explicit tag guard; unit/scale exact Sounds row name. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:43`.
- Lua argument 2..4: optional numeric coords at totalargs>=4; unit/scale raw 3D game point. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:80`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Converts required arg1 to string and finds exact Sounds row by strcmp (miss-1). <=3 args uses receiver position. >=4 numeric coords2..4 selects raw point, otherwise receiver position. Calls Play3D(soundID,point,0,1,-1,-1). Play3D rejects IsDisablingSounds, no current Level, Level+304 !=38, online in-game menu open, or negative row; JAVA_SOUNDS branch plays sound ID, else sound row event-type1 routes PlayEvent, other rows sound-pack emitter. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:35`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:40`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:42`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:48`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:58`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:71`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:72`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:73`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:77`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:82`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0036/0036b5d8.c:23`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:35`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:40`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:42`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:48`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:58`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:71`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:72`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:73`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:77`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:82`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0036/0036b5d8.c:23`.
**State, timers and events:**
- PROVEN: No Lua result; conditional sound/event emitter play only; zero/too-few required string args invalid vector domain. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:35`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:40`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:42`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:48`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:58`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:71`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:72`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:73`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:77`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:82`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0036/0036b5d8.c:23`.
**Return:**
- PROVEN: No Lua result; conditional sound/event emitter play only; zero/too-few required string args invalid vector domain. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:35`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:40`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:42`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:48`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:58`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:71`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:72`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:73`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:77`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c:82`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0036/0036b5d8.c:23`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**MISSING:** No generic scene0x39212c method in CharacterScriptObjects (:144-146); current audio playback selected helpers lack original current-level38/menu/debug gate proof. Registration was found after bounded Binder scanner fixed nullptr binding.

**Concrete behavioral difference:** No generic scene0x39212c method in CharacterScriptObjects (:144-146); current audio playback selected helpers lack original current-level38/menu/debug gate proof. Registration was found after bounded Binder scanner fixed nullptr binding.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: unknown sound name, current Level ready; PlaySound3D("missing") No audio play; row-1 rejected by manager. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:35, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:40, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:42, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:48, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:58, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:71, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:72, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:73, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:77, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:82, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0036/0036b5d8.c`:23.
2. **PROVEN:** Given: valid sound S, receiver(1,2,3); PlaySound3D(S,10,20,30), Level ready offline sounds enabled 3D event/emitter requested at(10,20,30). Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:35, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:40, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:42, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:48, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:58, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:71, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:72, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:73, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:77, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:82, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0036/0036b5d8.c`:23.
3. **PROVEN:** Given: valid S, online in-game menu open; PlaySound3D(S) No audio play. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:35, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:40, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:42, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:48, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:58, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:71, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:72, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:73, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:77, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/0039212c.c`:82, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0036/0036b5d8.c`:23.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **16** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:239` | outside scoped roots | `PlaySound3D("sfx_spell_lightning_small")` | `[{"index":1,"expression":"\"sfx_spell_lightning_small\"","literal_tokens":["\"sfx_spell_lightning_small\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:241` | outside scoped roots | `PlaySound3D("sfx_spell_lightning_medium")` | `[{"index":1,"expression":"\"sfx_spell_lightning_medium\"","literal_tokens":["\"sfx_spell_lightning_medium\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:243` | outside scoped roots | `PlaySound3D("sfx_spell_lightning_big")` | `[{"index":1,"expression":"\"sfx_spell_lightning_big\"","literal_tokens":["\"sfx_spell_lightning_big\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_celest.luac:248` | outside scoped roots | `PlaySound3D("StaticBallKilled")` | `[{"index":1,"expression":"\"StaticBallKilled\"","literal_tokens":["\"StaticBallKilled\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_hotty.luac:78` | outside scoped roots | `PlaySound3D("sfx_spell_fire_small")` | `[{"index":1,"expression":"\"sfx_spell_fire_small\"","literal_tokens":["\"sfx_spell_fire_small\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_hotty.luac:80` | outside scoped roots | `PlaySound3D("sfx_spell_fire_medium")` | `[{"index":1,"expression":"\"sfx_spell_fire_medium\"","literal_tokens":["\"sfx_spell_fire_medium\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_hotty.luac:82` | outside scoped roots | `PlaySound3D("sfx_spell_fire_big")` | `[{"index":1,"expression":"\"sfx_spell_fire_big\"","literal_tokens":["\"sfx_spell_fire_big\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_hotty.luac:86` | outside scoped roots | `PlaySound3D("sfx_mage_staff_elemental_fire")` | `[{"index":1,"expression":"\"sfx_mage_staff_elemental_fire\"","literal_tokens":["\"sfx_mage_staff_elemental_fire\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_rocky.luac:78` | outside scoped roots | `PlaySound3D("RockyEarthSpell")` | `[{"index":1,"expression":"\"RockyEarthSpell\"","literal_tokens":["\"RockyEarthSpell\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_rocky.luac:80` | outside scoped roots | `PlaySound3D("sfx_mage_staff_elemental_earth")` | `[{"index":1,"expression":"\"sfx_mage_staff_elemental_earth\"","literal_tokens":["\"sfx_mage_staff_elemental_earth\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_wetty.luac:80` | outside scoped roots | `PlaySound3D("sfx_spell_water_small")` | `[{"index":1,"expression":"\"sfx_spell_water_small\"","literal_tokens":["\"sfx_spell_water_small\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_wetty.luac:82` | outside scoped roots | `PlaySound3D("sfx_spell_water_medium")` | `[{"index":1,"expression":"\"sfx_spell_water_medium\"","literal_tokens":["\"sfx_spell_water_medium\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_wetty.luac:84` | outside scoped roots | `PlaySound3D("sfx_spell_water_big")` | `[{"index":1,"expression":"\"sfx_spell_water_big\"","literal_tokens":["\"sfx_spell_water_big\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_wetty.luac:87` | outside scoped roots | `PlaySound3D("sfx_mage_staff_elemental_ice")` | `[{"index":1,"expression":"\"sfx_mage_staff_elemental_ice\"","literal_tokens":["\"sfx_mage_staff_elemental_ice\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_windy.luac:78` | outside scoped roots | `PlaySound3D("WindyWhirlwindSpell")` | `[{"index":1,"expression":"\"WindyWhirlwindSpell\"","literal_tokens":["\"WindyWhirlwindSpell\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/faerie_windy.luac:80` | outside scoped roots | `PlaySound3D("sfx_mage_staff_elemental_wind")` | `[{"index":1,"expression":"\"sfx_mage_staff_elemental_wind\"","literal_tokens":["\"sfx_mage_staff_elemental_wind\""]}]` |

## Related natives

No exact related name established.

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
