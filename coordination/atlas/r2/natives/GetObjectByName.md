# GetObjectByName

Contract status: **DONE**. Static IDA/Lua/port source investigation only. Frozen port status: **PARTIAL**.

Engine functions to implement next ranking: **#134**.

## Binding and handler

| Public Lua name | Binding | Handler | Registration owner | Evidence |
| --- | --- | --- | --- | --- |
| `GetObjectByName` | bindFunction | `GameObject::_GetObjectByName` | `_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE` | .local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c:105150 |

Address-identified callback: `GameObject::_GetObjectByName` at `0x00392848`; source `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c`, unified lines 108776-108800.
Recovered declaration: `void __fastcall GameObject::_GetObjectByName( GameObject *this, const sfc::script::lua::Arguments *a2, sfc::script::lua::ReturnValues *a3, void *a4)`. **Binder ABI caveat:** IDA often mislabels the three callback registers as a member `this`, Arguments, and ReturnValues. The mangled source callback accepts `const sfc::script::lua::Arguments&`, `sfc::script::lua::ReturnValues&`, and `void*` bound receiver; infer roles from accessor/push operations, not IDA parameter names. This declaration is an artifact, not a Lua arity assertion.

## Lua argument mapping and units

bindFunction is a global call; bindMethod supplies a receiver. Native argument types and units are asserted only where handler accessors and callers prove them.
- Lua argument 1: exactly one string tag4; unit/scale global object name. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c:16`.

## Recovered behavior, gates, state, timers, events and return

**Behavior:**
- PROVEN: Requires exactly1 string tag4. Global ObjectManager lookup(name,-1,nullptr,0), converts returned handle to GameObject and pushes userdata; missing object is null userdata. Two arguments rejected even if first valid. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c:22`.
**Gates:**
- PROVEN: See explicit conditional branches in behavior; valid Binder receiver is assumed, with no wrapper-level actor state/cooldown gate unless stated. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c:22`.
**State, timers and events:**
- PROVEN: Read-only callback; starts no timers and raises no events. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c:22`.
**Return:**
- PROVEN: One Lua value as described; no state mutation. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c:1`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c:16`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c:19`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c:20`, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c:22`.

## Lua wrapper/helper traces

No helper-to-native trace was proven. Missing trace does not prove direct-only use.

## Source-backed behavior cases

PROVEN behavior cases are curated from the callback and its consumers below; see Known answers.

## Current frozen port source

**PARTIAL:** Selected scene object publisher/name resolution may find actors; general CharacterScriptObjects lacks0x392848 (:144-146). Original exact arity and global lookup are not satisfied by a catalog/name hit alone.
- Source evidence: `port/level-world/canonical_object_manager_v1.cpp`:72 — `out={0,UINT32_MAX,0};if(!name){e="source GetObjectByName null name is outside safe native domain";return false;}`.
- Source evidence: `port/level-world/canonical_trigger_zone_v22.cpp`:39 — `if(!names_[5].empty()){std::uintptr_t object{};std::uint32_t type{};if(!required(services_.find_named_object,"GetObjectByName(room=-1,create=false)/Handle/GetObject",e,names_[5].c_str(),-1,false,object,type))return false;if(object)door7b8_=`.
- Source evidence: `port/level-world/level_savegame_objects_v2.cpp`:26 — `else{std::int32_t signed_room;std::memcpy(&signed_room,&room,4);if(!s.by_name||!s.by_name(s.context,name.c_str(),signed_room,false,nullptr,a,e))return fail(e,"Required same ObjectManager GetObjectByName(name,room,false,NULL)");}`.

**Concrete behavioral difference:** Selected scene object publisher/name resolution may find actors; general CharacterScriptObjects lacks0x392848 (:144-146). Original exact arity and global lookup are not satisfied by a catalog/name hit alone.

Round-1 atlas port labels are reference-only. Identifier matches do not establish semantic parity. No runtime test was run.

## Known answers

1. **PROVEN:** Given: lookup name A resolves object P; GetObjectByName("A") Pushes userdata P. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c`:16, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c`:20, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c`:22.
2. **PROVEN:** Given: name absent; GetObjectByName("missing") Pushes null userdata. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c`:16, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c`:20, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c`:22.
3. **PROVEN:** Given: GetObjectByName("A",1) No Lua values. Evidence: `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c`:1, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c`:16, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c`:19, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c`:20, `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0039/00392848.c`:22.

## Original Lua call sites

Global exact-name/function-ID ledger, including calls outside the selected roots: **3** sites. Each occurrence is preserved below.

| Original file:line | Scope membership | Exact expression | Argument expressions and literal tokens |
| --- | --- | --- | --- |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:413` | outside scoped roots | `GetObjectByName(name)` | `[{"index":1,"expression":"name","literal_tokens":[]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:643` | outside scoped roots | `GetObjectByName("Hellhole")` | `[{"index":1,"expression":"\"Hellhole\"","literal_tokens":["\"Hellhole\""]}]` |
| `.local-inputs/assets-extra/android/data/scripts/ai/machineroom/referee.luac:663` | outside scoped roots | `GetObjectByName("vip_start")` | `[{"index":1,"expression":"\"vip_start\"","literal_tokens":["\"vip_start\""]}]` |

## Related natives

`GameObject::_GetObjectByName`

## Unresolved reasons


## Evidence boundary

This is static Lua/IDA/frozen-port-source investigation, not runtime or visual verification. These API-level details have no direct footage counterpart; the source-facing callback result and state invariant must be checked in an integrated build.
