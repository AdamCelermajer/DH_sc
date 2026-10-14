# target:GetProp

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PORTED**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `GetProp` | bindFunction and/or Method (see registration ledger) | `Character::_GetProp` | `Character::createBindings/BindFunction` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b56bc.c:578 |

Address-identified callback: `Character::_GetProp` at `0x003b9d8c`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c`, unified lines 135419-135516.
Recovered declaration: `void __fastcall Character::_GetProp( Character *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: Number tag 3 converted getUInteger, <=223; unit/scale Property index; return is raw signed word, no division by 256. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c:43`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c:70`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c:91`.
- Lua argument 2: Optional boolean or pointer; unit/scale true -> global temp sheet; false/other type -> owner cached; nonnull pointer -> external sheet. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c:43`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c:70`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c:91`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Default reads owner cached sheet +4084. Boolean true selects CharProperties::s_temp. Nonnull pointer selects external PROPS_GetFromSheet. Does not recalculate or modify property. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c:77`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c:70`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c:91`.
**Gates:**
- PROVEN: Missing/non-number first, converted index >223, or explicit null pointer second emits nothing. getUInteger reads first argument more than once. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c:43`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c:85`.
**State, timers and events:**
- PROVEN: Only the explicitly listed operations occur in this handler; no invented cooldown or animation transition. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c:1`.
**Return:**
- PROVEN: One integer/raw property word on accepted read.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PORTED:** Reusable callback preserves current/temp/external selection and unsigned conversion in ordinary valid sheet domain. Port validates service identity/alignment instead of dereferencing arbitrary original 32-bit pointers.
- Source evidence: `port/level-world/character_property_bindings.cpp`:29.
- Source evidence: `port/level-world/character_property_bindings.cpp`:49.

**Port comparison:** Reusable callback preserves current/temp/external selection and unsigned conversion in ordinary valid sheet domain. Port validates service identity/alignment instead of dereferencing arbitrary original 32-bit pointers.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: Cached property 172=512, temp=768; GetProp(172) 512. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c`:77.
2. **PROVEN:** Given: Same state; GetProp(172,true) 768. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c`:70.
3. **PROVEN:** Given: External sheet property 172=1024; GetProp(172,pointer) 1024. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003b/003b9d8c.c`:91.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **4** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/skills/npc_energybolt.luac:71` | outside scoped roots | `target:GetProp(GetPyStruct("CharacterProperties", "AIFaction"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"AIFaction\")","literal_tokens":["\"CharacterProperties\"","\"AIFaction\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_assassination.luac:86` | player_skill_lua | `target:GetProp(GetPyStruct("CharacterProperties", "Level"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Level\")","literal_tokens":["\"CharacterProperties\"","\"Level\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_petrify.luac:25` | player_skill_lua | `target:GetProp(GetPyStruct("CharacterProperties", "Damage_Min_Main_Hand"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Damage_Min_Main_Hand\")","literal_tokens":["\"CharacterProperties\"","\"Damage_Min_Main_Hand\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_illusionist_petrify.luac:47` | player_skill_lua | `target:GetProp(GetPyStruct("CharacterProperties", "Damage_Min_Main_Hand"))` | `[{"index":1,"expression":"GetPyStruct(\"CharacterProperties\", \"Damage_Min_Main_Hand\")","literal_tokens":["\"CharacterProperties\"","\"Damage_Min_Main_Hand\""]}]` |

## Related natives

`Character::_GetProp`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
