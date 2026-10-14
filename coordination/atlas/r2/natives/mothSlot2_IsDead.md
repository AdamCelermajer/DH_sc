# mothSlot2:IsDead

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#202**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `IsDead` | bindFunction | `GameObject::_IsDead` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105155 |
| `IsDead` | bindMethod | `GameObject::_IsDead` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105160 |

Address-identified callback: `GameObject::_IsDead` at `0x0038ea48`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea48.c`, unified lines 105916-105927.
Recovered declaration: `void __fastcall GameObject::_IsDead( GameObject *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument none: Receiver-only callback; extra Lua arguments ignored; unit/scale none. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea48.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea48.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a2ed4.c:3`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Invokes receiver virtual at vtable+52 and pushes its result as Boolean. Character::IsDead 0x3a2ed4 reads byte +5193; it is not an HP<=0 comparison. GameObject base IsDead returns zero. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea48.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea48.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a2ed4.c:3`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea48.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea48.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a2ed4.c:3`.
**State, timers and events:**
- PROVEN: Read-only callback; starts no timers and raises no events. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea48.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea48.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a2ed4.c:3`.
**Return:**
- PROVEN: One Lua value as described; no state mutation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea48.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea48.c:10`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a2ed4.c:3`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** Character callback at character_script_objects.cpp:84-85 faithfully reads retained life->dead but does not provide general GameObject subclass virtual dispatch. The callback therefore covers published Characters; other object kinds require their own virtual providers.
- Source evidence: `port/windows-foundation/features/cinematics/source_cutscene_mode.cpp`:29 — `bool dead=false;if(!providers_.isDead||!providers_.isDead(info->character660,dead,e))return missing("actual Character.IsDead",e);`.
- Source evidence: `port/windows-foundation/features/companions/source_master.cpp`:11 — `if(!f.master50||!b.is_dead){error="Required reloaded actual master IsDead receiver";return false;}`.
- Source evidence: `port/windows-foundation/features/status_effects/source_status_owners.cpp`:66 — `if(q->subject!=self.b_.character)fail(-1,"Source CharAI IsDead targets foreign Character");`.
- Source evidence: `port/windows-foundation/features/status_effects/source_status_owners.cpp`:67 — `std::int32_t dead{};if(dh2_character_timer_owner_query(&dead,self.b_.life,1)!=1)fail(-2,"Required actual Character IsDead fields failed");`.
- Source evidence: `port/level-world/character_kill_live_v21.cpp`:68 — `case kill_is_dead:{auto entry=actors_.find(q.subject);if(entry==actors_.end()){error_="Required actual Kill IsDead Character";return -1;}out.word=std::int32_t(entry->second->borrow.life->dead);return 0;}`.
- Source evidence: `port/level-world/character_use_ooi_v47.cpp`:69 — `if(!require(bool(services_.is_dead),"selected Character.IsDead",e)||`.
- Source evidence: `port/level-world/character_world_target_owner_v1.cpp`:33 — `if(q->service==virtual_dead){if(!a.character&&a.base_target_query){std::int32_t value;if(w.query(is_dead,q->subject,0,value))return -1;*out=std::uint32_t(value);return 0;}if(!a.life){w.error_="Required genuine virtual IsDead owner";return -`.
- Source evidence: `port/level-world/player_level_quicksave_v29.cpp`:16 — `if(!services_.character||!services_.character(info->character660,player,error)||!player.owner||player.identity!=info->character660||!player.dead){if(error.empty())error="Required actual QuickSave Character IsDead";return false;}`.
- Source evidence: `port/level-world/player_manager_death_queries_v70.cpp`:10 — `bool character_dead{};if(!services.character_is_dead34||!services.character_is_dead34(p->character660,character_dead,e)){if(e.empty())e="Required Character virtual34 IsDead";return false;}`.

**Concrete behavioral difference:** Character callback at character_script_objects.cpp:84-85 faithfully reads retained life->dead but does not provide general GameObject subclass virtual dispatch. The callback therefore covers published Characters; other object kinds require their own virtual providers.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: Character dead byte0, HP0; receiver:IsDead() False; callback uses the virtual dead result rather than HP. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea48.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea48.c`:10, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a2ed4.c`:3.
2. **PROVEN:** Given: Character dead byte2, HP100; receiver:IsDead() True because nonzero dead byte is Boolean true. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea48.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0038/0038ea48.c`:10, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003a/003a2ed4.c`:3.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **1** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/mothcocoon_core.luac:91` | outside scoped roots | `mothSlot2:IsDead()` | `[]` |

## Related natives

`GameObject::_IsDead`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
