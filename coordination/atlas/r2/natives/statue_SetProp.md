# statue:SetProp

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#224**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `SetProp` | bindFunction and/or Method (see registration ledger) | `Character::_SetProp` | `Character::createBindings/BindFunction` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56bc.c:569 |

Address-identified callback: `Character::_SetProp` at `0x003b7eac`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7eac.c`, unified lines 134122-134193.
Recovered declaration: `void __fastcall Character::_SetProp( Character *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: Number tag3 -> getUInteger <=223; unit/scale Property index. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7eac.c:30`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7eac.c:42`.
- Lua argument 2: Number tag3 -> signed integer truncation; unit/scale Raw property value; no x256. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7eac.c:30`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7eac.c:42`.
- Lua argument 3: Optional pointer; other types ignored; unit/scale External sheet pointer only; boolean true DOES NOT select temporary sheet. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7eac.c:30`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7eac.c:42`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Ordinary setter: type&32 writes saved sheet then recalculates; else type&8 writes owner cached sheet; other types unchanged. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e07a0.c:6`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e07a0.c:8`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e07a0.c:11`.
- PROVEN: External setter: type&4 writes external sheet then recalculates, else type&8 writes owner cached sheet. Third argument boolean follows ordinary setter. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e0614.c:9`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e0614.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7eac.c:42`.
**Gates:**
- PROVEN: Needs two numeric args and index <=223. Explicit null pointer third prevents any set; invalid index prevents set. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7eac.c:30`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7eac.c:45`.
**State, timers and events:**
- PROVEN: Only the explicitly listed operations occur in this handler; no invented cooldown or animation transition. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7eac.c:1`.
**Return:**
- PROVEN: Zero Lua outputs.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** Frozen setter uses signed integer conversion for property ID then unsigned guard: negative ID is rejected instead of original unsigned conversion clamping to index zero. Ordinary nonnegative valid domain and ignored boolean third match.
- Source evidence: `port/level-world/character_skill_properties_v1.cpp`:33.
- Source evidence: `port/level-world/character_skill_properties_v1.cpp`:31.

**Concrete behavioral difference:** Frozen setter uses signed integer conversion for property ID then unsigned guard: negative ID is rejected instead of original unsigned conversion clamping to index zero. Ordinary nonnegative valid domain and ignored boolean third match.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: Property 172 type8; SetProp(172,512) Owner cache word 172 becomes 512. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e07a0.c`:11, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e07a0.c`:13.
2. **PROVEN:** Given: Property 172 type8; SetProp(172,512,true) Owner cache becomes 512; boolean does not select global temp. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7eac.c`:42, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e07a0.c`:13.
3. **PROVEN:** Given: SetProp(172,512,nullPointer) No mutation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b7eac.c`:45.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **1** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:162` | player_skill_lua | `statue:SetProp(GetPyStruct("CharacterProperties", "Level"), ToFixed(plvl))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Level\")","literal_tokens":["\"CharacterProperties\"","\"Level\""]},{"index":2,"expression":"ToFixed(plvl)","literal_tokens":[]}]` |

## Related natives

`Character::_SetProp`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
