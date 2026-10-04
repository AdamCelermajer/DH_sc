# Persistent CharacterScriptSession composition

This adapter composes the individually recovered source kernels and executes
the original `_commons` and `monster` bytes in each genuine privately owned
native Lua VM. It does not reconstruct Application selection, every registered
game callback, full AI update/expiry, renderer ownership, or a whole-session
ARM32 differential. Its report is a host composition and lifetime proof.

## Ownership and API

`CharacterScriptSession::create(CharacterGameDesign::Borrow&&, input, error)`
pins the decoded design snapshot; retains shared `PropertyState` and
`CombatActorState`; owns the float position projection, actor name, copied
AI-script name, exact script/cache bytes, source timer storage, and a native
integer-mode `ScriptOwner`. The adapter is noncopyable and nonmovable. Caller
may retain a heap pointer through vector movement or renderer recreation.

`start()`/`advance()` execute the existing source seven-stage lifecycle. The AI
row comes from resolved property 1 through the actual decoded `ai_props` getter
and fallback. `source_is_character` is the caller's genuine IsCharacter virtual
projection, not a script-kind inference. The actual Crypt Characters skip the
delayed noncharacter budget branch. If that branch is reached, an absent budget
provider fails explicitly. No budget value is invented.

Host context, Level/Debug services, timer-expiry services, integer identity and
fraction-formatting providers remain borrowed. Their contexts must survive this
session and all VM finalizers. Shared properties/combat, design Borrow and owned
position survive until **after** owner destruction. `ScriptOwner` is declared
last and explicitly reset first. Its recovered teardown clears aliases then
the private integer map before VM close; both map wrappers and every installed
callback context remain alive through `__gc` and are destroyed afterward.

`position()` borrows the stable float array. `set_position()` synchronizes this
caller-produced projection only; it does not claim the original GameObject
SetPosition scene/physics operation. `properties()`, `combat_state()`,
`property_view()`, `owner()`, `view()` and `timers()` expose the pinned state for
the genuine caller bridge. Borrowed owner access does not authorize rebinding,
replacement, destruction or retargeting during a callback.

## Exact registration delivery

Original source descriptors are retained from the owner registration captures:
35 Stage1 globals, then 265 Stage2 entries, whose 130 methods are skipped by the
actual null method-binder receiver gate. The resulting **170 globals** are
installed in their original delivery order. No bind-all helper installs future
names early. Native SetInt precedes its Stage1 index 2 delivery and GetInt its
index 3 delivery, preserving the already proved integer-owner behavior.

The 27 supported descriptor deliveries are Include, SetInt/GetInt, genuine
empty shipping Trace, the three alias operations, GetPyCst/GetPyStruct/GetPyOID,
three host-context getters, GetProp, SetLevel, GetPosition, StartTimer/StopTimer,
and the eight fixed/bit callbacks. Their callback addresses are tested against
the original 300-entry descriptor inventory, not inferred from names. Examples:
Include `0x37efe4`; SetInt/GetInt `0x37de5c/0x37ec14`; GetProp `0x3b9d8c`;
SetLevel `0x3b73a4`; GetPosition `0x38e700`; Start/StopTimer
`0x3b7590/0x3b7064`.

Each other source global is installed as a protected **failing** callback with
a stable name/address context. Its presence matches the descriptor delivery;
calling it returns no successful effect and raises a descriptive unavailable
source-backend error. `registrations().supported` and `.installed` distinguish
implemented behavior from actual function presence; `missing_bindings()` lists
unimplemented names. Methods retain the source gate. Global callback arguments
still undergo the existing source Value projection before delivery.

Missing temporary-property backing makes `GetProp(...,true)` fail explicitly.
No temporary sheet or external identity mapping is invented. Unknown design
namespaces retain the design owner's required-delivery failure rather than
reporting a fabricated ordinary miss.

## Files, initialization and timers

The session copies `_commons` and the decoded external-script bytes under the
same `data/scripts/ai/` and `.lua`/`.luac` spelling rule used by the source owner.
Additional caller-provided cache files are owned immutable byte snapshots.
Include uses the dedicated live-frame capability and the genuine persistent
`owner.include` path, preserving nested loads and the per-private-VM loaded set.
Generic VM operations remain guarded while Lua executes. Include during VM
close remains explicitly unsupported, consistent with the current runtime
capability boundary; ordinary alias/integer/design/position callbacks remain
valid through close. Session replacement/termination requires the unrecovered
genuine backend and fails explicitly.

Owner design-tick requests use actual `CharacterDesign/AI_Tick` and `DoT_Tick`
lookup and preserve signed word bits. The current real constants supply
**3000 ms and 1000 ms**. Source OnInit creates genuine repeat `-1` timers for
events `0x33` and `0x34`, `user_ref=0`. Lua StartTimer creates event `0x35` through
the already verified source Value conversion and actual timer storage.

`update_timers()` refuses to mutate any timer unless the caller supplies an
expiry callback. The two callbacks in the host audit are explicitly stopping
fixtures, not full CharAI event routing. Source Lua errors during external Init
remain diagnostics while the source lifecycle continues publication and timer
creation. The audit distinguishes a clean source load from a failed Init call:
the root loads can have status 0 while `owner.error()` retains a Call failure.

## Evidence and scope

`tests/character_script_session.cpp` uses real decoded tables and all 26 caller
ordered constant streams from the persistent-design fixture, the exact original
common/monster script bytes, and the **11 selected Character rows/positions** in
`crypt01.dact`. DACT is the existing development placement descriptor; this does
not claim reconstruction of conditional scene factories or every raw MLX row.

The test uses an explicit borrowed Crypt01/tier0 host projection, whose player
cache is synchronized from the actual Knight property view. The Level rows are
decoded real ranges. DebugSwitches itself executes the source-backed owned
missing-file algorithm; the open provider is a declared missing-file fixture,
not an actual filesystem observation. Android's real filesystem and active
Application/session selection are outside this host report.

Every selected monster executes actual common, monster top-level and aliased
Init with `last_source_load_status()==0` and empty `owner.error()`. Assertions
cover saved positions, `m_flee_flag`, Buff_Speed lookup, ordered registration,
private integers, source timer records, nested Include, loaded-set skips,
required-host failure prefixes, denied expiry without a caller, generic busy
guards, source-byte copies, snapshot retention and an actual `newproxy` close
finalizer. During close it observes cleared integer/alias contents, then invokes
both again successfully, reads position/design data, and receives the explicit
unsupported Attack error.

`tests/character_script_session_host.py` builds only the new adapter and genuine
Debug backend, links real sanitized native DSOs, captures compiler arguments,
binary hashes, input hashes and source hashes, and checks dependency stability.
The central DSOs are the previously frozen historical stage bound by
`reports/character-init-services-main-linked-host-audit.json`. Concurrent new
runtime source is **not** represented as those libraries' implementation. The
compile-interface hashes are separate from historical implementation binding.
Parent's later central rebuild must produce a new compiler/source-bound report.

Reproduction: run the new Python host driver with its default build path, or
run the resulting `character_script_session_audit` with GDO1, original common,
original monster and `crypt01.dact` paths. No APK, emulator, physical ARM64,
full enemy AI, startup constant order or complete game-manager claim is made.
