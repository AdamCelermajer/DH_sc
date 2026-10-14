# myTarget:IsPlayer

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#80**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `IsPlayer` | bindFunction | `GameObject::_IsPlayer` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105173 |
| `IsPlayer` | bindMethod | `GameObject::_IsPlayer` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105178 |

Address-identified callback: `GameObject::_IsPlayer` at `0x0038e9f0`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9f0.c`, unified lines 105890-105901.
Recovered declaration: `void __fastcall GameObject::_IsPlayer( GameObject *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument none: Receiver-only callback; extra Lua arguments ignored; unit/scale none. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9f0.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9f0.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a49f0.c:7`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Invokes receiver virtual at vtable+40. Character::IsPlayer 0x3a49f0 first checks CharType: nonzero type returns type==1. Only type0 checks name begins with "PlayerCharacter" (name==strstr return), not substring anywhere. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9f0.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9f0.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a49f0.c:7`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9f0.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9f0.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a49f0.c:7`.
**State, timers and events:**
- PROVEN: Read-only callback; starts no timers and raises no events. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9f0.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9f0.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a49f0.c:7`.
**Return:**
- PROVEN: One Lua value as described; no state mutation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9f0.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9f0.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a49f0.c:7`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** character_script_objects.cpp:96 accepts strstr!=nullptr for type0, so XPlayerCharacter incorrectly returns true; original requires prefix pointer equality.
- Source evidence: `port/windows-foundation/main.cpp`:6405 — `const auto* traits=combatSession->world()->traits(id);if(!traits){e="Dead Focus requires current IsPlayer";return false;}`.
- Source evidence: `port/windows-foundation/playable_actor_bodies.cpp`:22 — `if(tracing) {bool ignored=false;if(!services.is_player) {error="Required original tracing IsPlayer query";return false;}if(!services.is_player(owner,ignored,error))return false;}`.
- Source evidence: `port/windows-foundation/features/cinematics/source_camera_critical.cpp`:13 — `bool player=false;if(!providers_.isPlayer||!providers_.isPlayer(character,player,e))return missing("actual Character.IsPlayer",e);`.
- Source evidence: `port/windows-foundation/features/cinematics/source_cinematics.cpp`:114 — `auto player=[&](std::uintptr_t actor,bool& out){return providers_.isPlayer?providers_.isPlayer(actor,out,e):missing("SAME Character.IsPlayer",e);};`.
- Source evidence: `port/windows-foundation/features/physics/runtime_session_contact_v1.cpp`:74 — `throw ContactServiceFailure("Source owner IsPlayer request changed Character identity");`.
- Source evidence: `port/windows-foundation/features/physics/runtime_session_contact_v1.cpp`:273 — `if (!traits) { error = "Tracing collision IsPlayer owner left Session"; return false; }`.
- Source evidence: `port/windows-foundation/features/physics/session_actor_transition_v1.cpp`:297 — `return fail(error,"Reached Dead Focus after flags241 requires actual source IsPlayer getter");`.
- Source evidence: `port/level-world/application_spawn_random_owner_v4.cpp`:6 — `if(!cached||!prob||!s.handle_as_player){e="Required actual Handle-AsChar-IsPlayer";return false;}`.
- Source evidence: `port/level-world/canonical_character_candidate_v60.cpp`:262 — `b.handle_as_player_character=[weak](bool& value,std::string& e){auto same=weak.lock();if(!same){e="Expired canonical Handle-AsChar-IsPlayer receiver";return false;}return same->is_player(value,e);};`.
- Source evidence: `port/level-world/character_language_scene_connection_v1.cpp`:27 — `if(skills::dh2_character_skill_target_query_v6(&value,target_providers::is_player,actor.character,nullptr,self.services_.ai_types,self.services_.target_services)){self.error_="Source IsPlayer delivery failed";return -1;}`.
- Source evidence: `port/level-world/character_live_body_projection_v6.cpp`:10 — `if(candidate.character_type==0||candidate.character_type==1){error="Required source player/archetype IsPlayer branch";return false;}`.
- Source evidence: `port/level-world/character_loot_actor_binding_v31.cpp`:19 — `if(!s.invoke||s.invoke(s.context,&q,&result)){e=world_.targets().error();if(e.empty())e="Required source IsPlayer virtual for loot";return false;}out=result!=0;return true;`.

**Concrete behavioral difference:** character_script_objects.cpp:96 accepts strstr!=nullptr for type0, so XPlayerCharacter incorrectly returns true; original requires prefix pointer equality.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: Character CharType2 named "PlayerCharacterFoo"; receiver:IsPlayer() False: nonzero type takes priority. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9f0.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9f0.c`:10, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a49f0.c`:7.
2. **PROVEN:** Given: Character CharType0 named "XPlayerCharacter"; receiver:IsPlayer() False: matching substring must start at name pointer. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9f0.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9f0.c`:10, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a49f0.c`:7.
3. **PROVEN:** Given: Character CharType0 named "PlayerCharacterFoo"; receiver:IsPlayer() True. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9f0.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038e9f0.c`:10, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a49f0.c`:7.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **3** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/generic_boss_core.luac:235` | outside scoped roots | `myTarget:IsPlayer()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/swampking_core.luac:190` | act1_level_boss_script_closure | `myTarget:IsPlayer()` | `[]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/winddragon_core.luac:235` | outside scoped roots | `myTarget:IsPlayer()` | `[]` |

## Related natives

`GameObject::_IsPlayer`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
