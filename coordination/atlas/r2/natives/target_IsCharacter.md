# target:IsCharacter

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#49**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `IsCharacter` | bindFunction | `GameObject::_IsCharacter` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105164 |
| `IsCharacter` | bindMethod | `GameObject::_IsCharacter` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105169 |

Address-identified callback: `GameObject::_IsCharacter` at `0x0038ea1c`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea1c.c`, unified lines 105903-105914.
Recovered declaration: `void __fastcall GameObject::_IsCharacter( GameObject *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument none: Receiver-only callback; extra Lua arguments ignored; unit/scale none. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea1c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea1c.c:10`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Invokes receiver virtual at vtable+36 and pushes Boolean. Tests dynamic native object classification, not receiver Lua table spelling. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea1c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea1c.c:10`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea1c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea1c.c:10`.
**State, timers and events:**
- PROVEN: Read-only callback; starts no timers and raises no events. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea1c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea1c.c:10`.
**Return:**
- PROVEN: One Lua value as described; no state mutation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea1c.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea1c.c:10`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** Published catalog lists callback, but CharacterScriptObjects::invoke lacks 0x38ea1c and ends with unreconstructed-method failure (character_script_objects.cpp:144-146). Separate source object admission providers do not prove this Lua method path.
- Source evidence: `port/windows-foundation/features/actor_frame/source_campaign_backend_v1.cpp`:212 — `return need("published object IsCharacter", e);`.
- Source evidence: `port/windows-foundation/features/enemy_ai/runtime_ranged_skill_monster_v1.cpp`:276 — `next.delayed_check_character = contains(delayed_check, "IsCharacter()");`.
- Source evidence: `port/windows-foundation/features/physics/runtime_session_contact_v1.cpp`:69 — `throw ContactServiceFailure("Source IsCharacter request was not the registered physical peer");`.
- Source evidence: `port/level-world/openable_container_owner_v1.cpp`:113 — `bool character=false;if(!call(s_.is_character,"GameObject::IsCharacter",e,actor,character))return false;`.
- Source evidence: `port/level-world/world_item_drop_inventory_v9.cpp`:31 — `bool character{};if(!services.is_character||!services.is_character(services.context,source,character,e)){if(e.empty())e="Required source DropInventory IsCharacter virtual24";return false;}`.

**Concrete behavioral difference:** Published catalog lists callback, but CharacterScriptObjects::invoke lacks 0x38ea1c and ends with unreconstructed-method failure (character_script_objects.cpp:144-146). Separate source object admission providers do not prove this Lua method path.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: receiver virtual IsCharacter returns0; receiver:IsCharacter() False. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea1c.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea1c.c`:10.
2. **PROVEN:** Given: receiver virtual IsCharacter returns1; receiver:IsCharacter() True. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea1c.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea1c.c`:10.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **9** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/largeplant_core.luac:75` | outside scoped roots | `target:IsCharacter()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/edward_rise.luac:327` | outside scoped roots | `target:IsCharacter()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/madruktorso_drainlife.luac:57` | outside scoped roots | `target:IsCharacter()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_assassin_grapling_hook.luac:82` | player_skill_lua | `target:IsCharacter()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_berseeker_berserk.luac:111` | player_skill_lua | `target:IsCharacter()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_mage_stone_escape.luac:71` | player_skill_lua | `target:IsCharacter()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_necromancer_rise.luac:331` | player_skill_lua | `target:IsCharacter()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/prince_rogue_vicious_strike.luac:95` | player_skill_lua | `target:IsCharacter()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/skills/witch_ranged_00.luac:70` | act1_level_boss_script_closure | `target:IsCharacter()` | `[]` |

## Related natives

`GameObject::_IsCharacter`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
