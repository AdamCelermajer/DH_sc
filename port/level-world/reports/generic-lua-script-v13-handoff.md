# Generic LuaScript V13 for the actual Level constructor

New production TU: `port/level-world/generic_lua_script_owner_v13.cpp`, linked into the existing level-world target against its existing script-runtime/game-data dependencies. No existing Character owner, VM, shared classes, globals, renderer or CMake were changed.

## Ownership recovered from original

`LuaScriptC1(bool)37c584` ALWAYS constructs its embedded `sfc::script::lua::Instance` through `31b268`. That Instance sets owned byte8=1 and allocates a fresh private state through `newstate31b224 → lua_newstate8579f8`, then installs the panic handler. It does not borrow a global interpreter. The bool only controls whether `BindFunction37b5a0` runs immediately (false) or is deferred (true). Four executed original constructor/destructor cases produce four distinct states, all closed once.

Generic LuaScript binds exactly33 functions. Character Stage1 adds RegisterAIState/ChangeAIState to make35; those TWO derived globals are deliberately absent from this owner. The original generic binder opens Base → Math → Table → String before binding. The existing Lua5.1.4 native source-library primitives reproduce the successful stack residues (5 entries), including repeated binding/opening (10 after a second bind).

`LuaScript.Load37b574` gets the shared LuaManager through the actual service pointer+3c and delegates to `LuaManager.AddFile37b23c`. LuaManager C1/C2 `379eb0/379e68` own an empty file-cache map (native24-byte object); this manager is a file-buffer owner, NOT a shared Lua state. One `LuaScriptCacheOwnerV13` belongs to that actual global manager publication/lifetime, and every private GenericLuaScript borrows it. Do not create another world, global VM, Character session, property/inventory authority or arbitrary RNG.

`LuaManager.FlushBufferedFiles379fe8` disposes buffered stream objects in filename order then resets the map; `flush_buffered_files()` supplies its native owned-byte-buffer equivalent. Per-Script successful-loaded membership survives manager flush, as original. Parent must preserve the actual AppInit/global LuaManager creation/publication phase; this module does not invent that phase or substitute a missing global manager.

## Concrete Level composition

Root's source Level script at44 owns the SAME LuaScript path at `44+68=ac`. There is no second Level.ac string. Keep the single script lease and perform the actual source path assignment on it:

```cpp
auto shared_cache = /* SAME actual global LuaManager cache owner */;
scripts::LuaScriptServicesV13 services;
services.owner = actual_world_design_and_backend_lease;
services.design = *same_design_borrow.design();
services.scalar = actual_scalar_identity_and_divzero_services;
services.integer_context = actual_identity_and_format_context;
services.integer_identity = actual_source_identity_projection;
services.integer_fraction = actual_source_printf_fraction;
services.objects = actual_world_object_services; // required for object outputs
services.resolve_native = actual_native_binding_resolver;

auto script = scripts::GenericLuaScriptOwnerV13::create(
    false, shared_cache, services, 16u*1024u*1024u, error);
if (!script || !script->assign_path("data/scripts/", error)) return false;
bool source_loaded;
if (!script->load("level/combat_formulas", source_loaded, error)) return false;
// Level C1 ignores the original Load BOOL, so delivered source_loaded=false
// is distinct from required native delivery failure above.
if (!script->load("level/death_scripts", source_loaded, error)) return false;
```

`LuaScriptCacheServicesV13::read_file` borrows the actual Application resource/cache reader. Returntrue/foundfalse only for a genuine source missing-file result; a missing producer returnsfalse. Actual bytes are retained without filename fallback or synthetic script content. Cached bytes are shared between private Instances, while aliases/int maps/loaded sets remain per-Script as original. The bounded native memory budget is existing host-runtime policy, not recovered original heap budgeting.

## Binding and required backends

The source33-name order/callback addresses are captured in `constructor-binding-original.json`. Real internal implementations compose Include, literal Trace (`37ee80 bx lr`), SetInt/GetInt, AddToVFTable/PushVFTable/PopVFTable, the eight fixed/bit kernels, and the three design-query kernels. Identity/fraction/division-zero/design providers remain actual required dependencies when their source branches reach them; guards run through the existing proved kernels. Missing or rejected providers use the runtime's explicit required-failure epoch even if script pcall catches the error.

Remaining15 globals require original native backends: Rand/RandF, CallPyScript, GetNumPlayers, GetHostPlayer/Level/Difficulty, GetCurrentLevelRange, GetGameObjectsByType, SetGameType, GetGameScript, OnTargetDied, PlayMusic/PlaySound/StopSound. The resolver selects a real source-values, source-object-output, or source-scoped-values callback and retains its actual receiver lease. Source-object output also requires the actual World object services. Scoped values and object results cannot be combined by the current runtime; that combination explicitly rejects rather than guessing another bridge.

Absent external backends install named **required-failure carriers**, labeled `actual_backend=false` in `bindings()`. These are not successful fake implementations or fabricated values; invocation fails explicitly and loading will not publish successful loaded membership even if Lua catches it. Constructor binding alone is not a claim that all world/audio/RNG branches are implemented. A full positive call on those branches requires the supplied genuine backend. In particular LocalGameOver requires actual PlaySound; no silent audio success is introduced.

`call()` implements the source overload that projects then discards ReturnValues, resolving aliases freshly. Its source_success is protected-call status, not the script's first returned value. Full ReturnValues capture/object-result overloads are outside this bounded public method; do not interpret discarded returns as damage/gameplay results. Existing source-value argument/output limits remain explicit. Native bindings/finalizers must not destroy or arbitrarily reenter the owner; nested Include uses the dedicated same-VM Include capability.

## Load/teardown semantics

Name NULL or empty gives delivered source false before file access. Build `path+name`; search requested name for the FIRST `.lua` substring. If absent append `.luac`; if that substring is not `.luac`, append `c` to the whole assembled filename. This is intentionally NOT an ends-with extension check (`x.lua.tail → x.lua.tailc`, `x.luac.tail` unchanged).

Check per-Script successful-loaded membership before shared cache access. Cached input starts at source position0 for each Instance load through the existing1024-byte source reader and `loadFile()` chunk name. Successful parsing/execution inserts membership only afterward. Missing file or positive Lua status delivers source_loaded=false; prior Lua mutations persist and retry remains allowed. Unsupported runtime/required native failures returnfalse separately and do not insert membership. Include ignores delivered Load false as original; proper required provider failure still propagates through the runtime capability/epoch boundary.

Teardown clears loaded-file membership, path, alias backup/main maps and int map before closing its owned VM. The wrappers, actual native contexts, services and cache lease remain alive through lua_close finalizers; finalizers can observe cleared maps and mutate those still-live wrappers. Wrapper storage is destroyed after VM close. No global VM is closed and no Character owner is involved.

## Verification and current limits

* Original four C1(false/true)/embedded Instance/D1 cases execute source stores, generic binder order and own-state close; primitive VM/library/Binder/CString receivers are explicitly fixtures. Exactly33 source globals, four distinct private states, four closes.
* Whole original Load37b574/AddFile37b23c wrapper oracle10 cases: path/substring rules, successful-loaded bypass, cached seek0, loader statuses, success insertion and genuine file-open NULL result. CString/libc/cache-tree/Instance parser receivers are declared fixtures; native positive parsing/assets are tested separately.
* Actual native Lua runtime/design/cache test PASS483: both original Level script files, actual CharacterProperties schema lookup, source CF_ClearCombatants/CF_CalcDamage no-combatants branch, separate VM/int maps, shared file cache, nested Include, successful membership, Lua-error mutation/retry, pcall-caught required backend failure, actual LocalGameOver missing PlaySound boundary, alias-discard calls, source destructor map/VM ordering, repeated binding, substring edge cases and buffer flush lifetime.
* Strict ARM64 owner/test syntax PASS. Native receipt links accepted APK11c8219e and compiles this new owner inline; no app install/input or live Level acceptance is claimed.

Actual files are the unmodified original cache payloads under `.local-inputs/character-script-assets-v1/bootstrap-cache/data/scripts/level`. Their `.luac` extension does not mean these two payloads are binary bytecode: their actual bytes are Lua source text. The native runtime consumes those exact bytes; nothing was decompiled/replaced/compiled to fake a missing asset. Their hashes and the actual design fixture closure are recorded in `native-cache-inputs.json` and match the existing extraction TSV.

Root still owns complete Level/GSLevel construction/publication and actual Application/global LuaManager publication plus positive world/audio/RNG/object backend composition. This handoff supplies the real generic script owner and reusable file-buffer graph rather than claiming those outer lifetimes or all native game functions.
