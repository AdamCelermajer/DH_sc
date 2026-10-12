# V52 actual compiled ScriptManager loader storage handoff

Source ready: `script_manager_owner_v52.hpp/.cpp`, `script_data_schemas_v52.inc`, `script_command_factories_v52.inc`, `stage_loader_v52_script_manager.hpp`. Native normal + ASAN/UBSAN PASS: common15 + SWAMP55 scripts,870 actual data records across46 types,3495 scalar/string/array values against original executed-reader gold,70 names and source stopped contexts,8 failure-prefix cases. Whole engine execution and actual main command factories/Init remain unverified; native main bodies are explicitly fixtures in the test.

## Minimal main adoption

1. Add `script_manager_owner_v52.cpp` once, with the two generated `.inc` data/factory tables and headers. The sole new manager storage exists because main currently has no matching original global ScriptManager implementation; it is separate from LuaScriptOwner44 and EventManager/GameEventManager.
2. Construct ONE application-owned `ScriptManagerOwnerV52` during actual original ScriptManager authority initialization. Keep that SAME manager across level loads; each source stage8 calls UnLoadAllScripts. Do not construct one per Level or publish a parallel global/registry/VM. No new global is created by this code.
3. Supply `ScriptManagerServicesV52.owner` as the genuine native factory/cache provider lifetime (avoid owning-application cycles). Bind `open_file` to the existing real resolver/cache/StreamBuffer byte producer. `ScriptFileV52` pins its actual stream/buffer and exposes found+bytes; true missing stream follows original no-op return. Missing API owner fails explicitly. Use original actual requested filenames; common15 and firstSWAMP55 fixtures are test provenance, not hardcoded map selection.
4. Bind REQUIRED `command_factory(kind,out,error)` to REAL native command-class C1 owners. Return strong SAME actual owner/identity, original factory address, pointers into its real skip4/kind8/data_c and modern retained-data lease, plus true class Init and base-storage release body. These are **not** metadata/count records. There is no permissive fallback command factory; first missing kind23 fails after original common8=15/vector15/context15/first count4 + allocated array, before any Data Read/Init. Exact46 class/factory/Init matrix is `reference/scriptmanager-v52/command-main-services-v52.json`.
5. Loader supplies source-proven pure generated Data C1/read storage for all46 types reached by actual common/SWAMP bytes. It owns typed scalar fields, CString buffers and ExecScript count16/int-array20; main command's actual data_c and actual strong lease point to this SAME owner after Read completes. `ScriptCommandDataV52` is parsed data, never an executable command. Main effects can consume scalar(offset),cstring(pointer_offset),array(pointer_offset). For other command types0..79, optional genuine `data_factory` may return a typed actual data owner/read extension with real complete/identity/field views; missing original data body stops explicitly.
6. Call `bind_script_manager_v52(application_same_manager)` to fill V51's ScriptManager services. This wrapper validates identity and reaches actual owned Unload/LoadCommands/LoadNames; it constructs no authority and replaces no Lua/global. V51 preserves fog→Unload→common commands→common names→current config reread→local commands→local names. Dispatcher state130 increment remains external.

## Exact original storage order

Manager C1455980/4559bc: current0=-1,field4=0,common8=0,contexts0c/commands18/names24 vector headers0,byte30=0. Native fields preserve these stores (context4 is opaque raw32 until main recovers gameplay semantics).

Load45b264: common=true with common8 < actual command-list count returns BEFORE opening file. Otherwise actual cache open/StreamBuffer; missing stream returns. Header script count → common8 +=count BEFORE command/context vector resize → each list count0 → new command pointer array8 → peek kind → actual class C1 factory → publish command pointer slot IMMEDIATELY → real data factory/C1/read → source command.kind8/data_c stores → actual Init virtual0 → read command.skip4 and set list.skip4 when nonzero → after whole list `StopScript(index,false)` stores context.state8=2/context.command0=-1, preserving context4. Data-Read failure keeps actual command C1 and pending data owner, but leaves command.data_c NULL. Init absence/failure preserves assigned data and does not count it initialized.

Names45ad40: same common gate before actual open, header count → append zero slots to actual names vector → each length/CString bytes + native NUL terminator → publish owned name pointer. No copied name registry. `name_from_id` and `id_from_name(name,include_common)` implement original455b8c/4591f0; include_common=false starts at current common8, true starts0; exact CString comparison, missing=-1.

Unload45a1c8: every ScriptCmds.Free45632c BEFORE freeing names: static BASE ScriptCmdImpl::~455a68 releases data through virtual dataFree/dtor, clears data_c, then CustomFree command. It does not call command Execute/Finish or derived gameplay methods. Native clears SAME actual command data lease/pointer, then requires real main base-storage release; failed actual release is never replayed. Keeps reached prefix/strong command owner on failure. Only successful all-command teardown frees names, resets all vectors, current0=-1 (`StopSkipping455be0`), common8=0. **field4 and byte30 survive** Unload; Flush45a2ac is a different operation. Destructor-only native lease release is not claimed as original runtime teardown.

## Required Main Init/effects

All870 data readers and real ARM command/data C1 factories were executed in the original oracle. Actual command Init was an observation fixture, explicitly skipped there. Matrix resolves only SIX reached Init addresses: base455628 is a verified BX LR; five nontrivial bodies are StartDialog459034, HideFlash4597e8, ShowFlash45983c, PlayEffect4598b4, PlayActorAnim45a2ec. Exact source dumps are frozen; main may bind true base no-op only for classes whose original vtable points there. It must bind genuine five native services/owners for other classes. Execute/IsBlocking/Finish/dialogue/cinematic/quest effects remain Main, over SAME retained class/data/context instances. There is no count-derived command readiness or forced Level38.

## Proof and failures

Original oracle executes full C1/parser/registry/storage and actual Unload over common15/SWAMP55, with explicit filesystem/stream/allocation and command-Init transport fixtures. Gold covers870 records/3495 reads; values, offsets, variable CString lengths and ExecScript nested int arrays are compared bit-for-bit; ARM pointer/vtable addresses normalized. Original stop-before-first-factory proof explicitly confirms cursor8,common15,commands15,contexts15,first_count4,array allocated. This fixed the initial TEST assumption7; no production semantics were changed for that correction.

Native test exercises real parsed data and native manager storage, with explicit main command owner/Init/release fixtures. Eight required-stop cases: missing factory, failed Init, missing Init, foreign factory tag, failed release without replay, factory reentry, truncated names with previous prefix retained, truncated data with command/pending data retained. Successful Unload releases all870 fixture command leases and preserves field4/byte30 as source. No selected/shared/common/CMake/device mutation or old archive links.

| Subgoal | Subtask | Status |
|---|---|---|
| Source bodies | C1/LoadCommands/LoadNames/Unload/Stop/GetID/GetName order | Finished |
| Original execution |70 scripts/870 data records + true whole storage Unload | Finished; Init transport is fixture |
| Exact data |46 source C1/read schemas including ExecScript int array | Finished |
| Native loading owner | Actual single command/context/name storage and failure prefix | Finished |
| Stage8 adapter | One-owner bind for actual V51 services | Finished |
| Native verification |3495 value/70 name/context comparisons +8 failures, normal/sanitizer | Finished |
| Main class factories |46 actual C1 owners / proper fields and leases | Main required |
| Main Init/effects |5 nontrivial Init bodies + Execute/dialogue/cinematic/quest | Main required |
| Main actual App route |Sole ScriptManager authority/cache + actual class runtime proof | Awaiting coherent adoption |
| Full Act1 readiness |Contiguous loading/real rendered gameplay | Not established by V52 |

Receipts: `reports/scriptmanager-v52-final-build.json`, `reports/scriptmanager-v52-verified.json`, `reference/scriptmanager-v52/original-v52.json`, `data-schemas-v52.json`, `command-main-services-v52.json`. Freeze hashes come from actual read_bytes and archive readback.
