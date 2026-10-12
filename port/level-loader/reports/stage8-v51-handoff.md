# V51 stage8 config/fog/script loading handoff

Source-ready unique headers reconstruct stage8 order and actual path parsing. Original ARM proof passes18 path cases and6 original stage8 branches. Native normal + ASAN/UBSAN pass18 gold path cases and13 required-stop/ownership/control-flow cases, using actual CanonicalLevelConfig constructor/property fields with one proposed float getter. No whole actual-Level C1/stage8/main services readiness claim.

## Critical ownership correction

Original `Level::_LoadScripts3f3a58` uses the global **ScriptManager** compiled command/context/name authority, not `GenericLuaScriptOwnerV13` or Level.script44. Original source does not write44 here. The existing Lua44/cache/VM stays untouched. Current main source search under game-data/script-runtime/level-world finds no complete matching ScriptManager owner or methods; GenericLuaScriptOwnerV13 is a different engine. Do not substitute its Lua `load()` and mark stage8 done.

## Minimal adoption

1. Add header-only `level_script_paths_v51.hpp` and `stage_loader_v51_config_scripts.hpp`; bind external stage8 via `stage8_config_scripts_v51` over `borrow_stage8_level_v51(actual completed canonical C1)`.
2. Apply ONLY `reference/stage8-v51/config-float-accessor-proposed.diff` + `config-float-accessor-cpp-proposed.diff` to existing CanonicalLevelConfig. It exposes the already produced float210 in the existing map; no new field/default/world/storage. `stage_loader_v51_canonical_config.hpp` borrows actual strings150/dfog204/float210 and uses existing native property writer to normalize SAME string150. Private full config-candidate copies are test inputs/proposal evidence, not additional production classes/files.
3. `borrow_actual_config` must resolve identity from the SAME actual manager/retained receiver list and pin that typed receiver. No copied registry or pointer reinterpretation.
4. For real config204 nonempty only, provide actual object-manager owner and full InitModulesFogColor347100 renderer service. Source converts produced float210 to int toward zero before this call. Empty vector skips it genuinely.
5. Bind the actual global ScriptManager owner with UnLoadAllScripts45a1c8 → LoadScriptFile45b264(common,true) → LoadScriptFileNames45ad40(common,true). Actual methods may naturally return after a genuine missing stream; a required service absence must fail, never a successful empty callback.
6. Following common callbacks, reread actual Level.config38, reborrow CURRENT config/string150, then recover local filenames and call command/name methods(false). Real replacement config during common-name callback is preserved by the implementation and original proof; it never loads the prior config's script copy. Missing/NULL current config fails after reached prefix explicitly.
7. Only if initial config38 is NULL, `produce_missing_config` must implement original Application cc/d0 override list test and e8 prefix + `LevelConfig.xml`, actual Level.LoadFile(...,"Level") continuation, reread38; then if stillNULL genuine ObjectManager.Spawn("LevelConfig","RuntimeDefaultLevelConfig",false,true), resolve(false),type4 gate and actual SetLevelConfig3f150c. Existing context publication v2 is a binding kernel; this code does not fake Spawn/LoadFile success. Source normal-root SWAMP should already publish config in stage7, but production must preserve the reached fallback boundary.

The wrapper does not increment state130 or write progress. Existing lifecycle dispatcher advances8→9 only after actual whole required body calls complete. Required service failure/reentry retains reached prefixes. Native fallback `pending` is bounded scheduling; original spins LoadFile synchronously untiltrue. No actual whole Init/state38/current global/VM/cache construction occurs here.

## Original paths/data

Common: `data/pydata/scripts_pyscripts.bin` then `data/pydata/scripts_pyscriptnames.bin`, both common=true. Actual verified common asset has15 compiled scripts. Verified first SWAMP assets `data/pydata/scripts/001_swamp_pyscripts.bin`18117bytes and `_pyscriptnames.bin`1155bytes have55 scripts each. These file counts are asset evidence, not a decoded/executed command catalog or a hardcoded LevelConfig filename binding.

Original `_LoadScriptFile` skips empty/length<=8, reverse-searches LAST exact case-sensitive `.pyscript` anywhere, normalizes all backslashes in SAME config string only if marker reached, and replaces entire suffix from marker with `_pyscripts.bin`/`_pyscriptnames.bin`. It does not require marker at end. Native parser matches original ARM18 cases, including markers in tail/multiple markers/uppercase/backslashes/UTF8 bytes. Standard std::string transport helpers are fixture boundaries in the oracle; actual reverse-search and source normalization/store/call ordering execute original code.

Original command loading45b264 is a different substantial body: common8 gate against actual commands vector count; actual cache open/read; grows command/context vectors; for each command uses real source static command and PyData factories, read virtual method, stores command.kind8/data_c, invokes Init virtual, accumulates skip byte and StopScript(id,false). Name loading45ad40 appends original raw length-prefixed CString names to actual names24 vector. Unload45a1c8 calls each ScriptCmds.Free45632c, frees names, resets vectors, StopSkipping455be0 and common8=0. None is replaced with Lua services or false whole-method success.

## Proof boundaries and subgoals

| Subgoal | Subtask | Status |
|---|---|---|
| Source recovery | Entire stage8 path incl fog/config fallback/ScriptManager distinction | Finished |
| Original execution |18 path +6 branch cases, Lua44 untouched | Finished; body transports are fixtures |
| Native parser | Reverse marker search, slash normalization, exact output filenames | Finished |
| Existing owner adapter | Canonical config script150/dfog204/float210 | Finished; surgical getter proposal |
| Stage orchestration | Ordered calls, current config reread, failure/reentry/pending semantics | Finished in13 fixtures |
| Actual C1 integration | Same actual state130/config38 pointer borrow | Source-ready; coherent runtime proof pending adoption |
| Source body providers | Fog347100 and genuine global compiled ScriptManager methods | Main required |
| Fallback services | Actual override LoadFile/default Spawn/publication | Required only ifNULL38 branch reached |
| Whole Swamp readiness | Contiguous full stages + real render/gameplay | Not established by V51 |

Receipts: `reports/stage8-v51-final-build.json` native PASS normal/sanitizer; `reference/stage8-v51/original-v51.json` original18/6 proof; `source-strings.json` exact source addresses/strings; `common-swamp-script-assets-v51.json` actual original asset hashes/counts. No shared/selected/CMake/device mutations; native test links its unique candidate source directly without old archive graph mixing.
