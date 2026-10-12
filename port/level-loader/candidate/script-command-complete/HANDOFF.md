# Complete command storage integration

This single source extension contains 80 distinct actual named receiver classes/factory cases and 80 original Data schemas. It extends the immutable kind7/v65 baseline 48 with 32 remaining table slots. It implements storage construction and reading only; it does not make an all-map runtime claim.

## Integration

Apply integration.patch to the four active private loader files only:
script_command_receivers_v59.hpp, script_command_receivers_v59.cpp,
command_c1_descriptors_v59.inc, script_data_schemas_v52.inc.
The patch baseline is candidate/command-kind7-v65, which already contains kind4/kind7. Frozen V59/V62/V63/kind4/kind7 candidates are preserved.

The working tree copies script_command_init_v62.hpp and script_manager_bind_v63.hpp unchanged. Their quoted dependency chain is V63 -> V62 -> V59 -> script_manager_owner_v52.hpp. Use the existing loader include directory for that owner header. The copied script_manager_owner_v52.cpp is unchanged from the actual manager and selects this local schema include; it is an isolated milestone compile input and must not be linked alongside another owner implementation.

Main constructs its ONE App-owned actual V52 manager from V63 composed services. Cache/stream services.owner must have independent ownership; direct and alias App owners and replacement of an existing real factory are rejected. Main callbacks and their hidden captures must retain App weakly. No new manager, VM, global registry, command executable record, or alternate cache was added.

## Actual source storage

Original ARM offsets describe provenance, not a modern host byte layout. All command C1s produce skip4 byte0, kind8=-1, data_c=NULL. Only source-produced extra cells are available. No allocation padding or unproduced words are invented.

Kinds 56..59 source C1 +0x10 NULL is now an actual native-width uintptr_t member with a weak actual target alias. Actual Execute stores the object pointer at 0x45d480/45d490,45d384/45d394,45d28c/45d29c,45d190/45d1a0 respectively. Main supplies the real resolution/type checks/target mutation and writes through write_operand_pointer with an alias owner whose get() equals the pointer. checked_pointer_borrow pins that SAME weak target and exposes the SAME native cell; expiry/mismatch fails. The scoped pin preserves lifetime during a real body without retaining App between calls. Existing Show/Hide menu +0x10 uses the same checked native cell route and existing menu-specific borrow. Other unproved old extras remain opaque until an actual typed producer writes them.

All 32 new Data readers use retained flat scalar/byte/CString helper calls. Raw words and float IEEE bits remain exact; there is no numeric reinterpretation or generated zero fill. CString reads follow their actual produced length fields. Explicit C1 scalar defaults are included only where bytes were source-written. The original 46 executed evidence remains unchanged; kind4/kind7 and remaining 32 use bounded static source evidence.

STORAGE-SCHEMAS.md lists the 32 added storage layouts and ordered read actions; the source JSON retains each instruction location.

## Actual Init contracts

75 Init table slots are exact source base 0x455628 BX LR. The same existing five nontrivial V62 adapters remain required and contain no permissive engine leaves:

* 10 StartDialog 0x459034: cached Data_c, actual App.constants2c, Data+c scalar read BEFORE required DialogStyles/EnterLocationDialog getConstant; actual comparison writes SAME skip4 exactly 0/1.
* 20 PlayEffect 0x4598b4: cached Data+8 scalar read BEFORE required current FX singleton projection; Main RegisterFXSetToLoad 0x4967e8 is a real functional registration leaf.
* 22 ShowFlash 0x45983c and 23 HideFlash 0x4597e8: required MenuManager GetInstance BEFORE checked CURRENT Data_c read. Read CString+10 from current data, case-sensitive strstr(name,"HUD"); match stores NULL, otherwise real GetMenuByName using original name produces checked actual menu alias.
* 45 PlayActorAnim 0x45a2ec: cache SAME Data before actual App.objects38; required GetObjectByName and handle GetObject(false), actual Character conversion; genuine returned null is original early return. SAME Character-owned Animator49c executes two real AddAnimDictToSet 0x3c9b14 leaves in source order. Read cached Data+8 before first and re-read cached SAME Data+c after first callback; reuse cached SAME Animator. Opaque receiver extras are not speculatively interpreted.

Functional FX registration and animation dictionary registration remain required Main leaf calls. Header replacement after an Init callback retains the original V59 failure prefix rather than silently completing/replaying. These callbacks can mutate source values; V62 preserves exact cached/live load points already captured.

## Runtime boundary and lifetime

MAIN-RUNTIME-BOUNDARIES.md lists all 80 actual Init/Execute/IsBlocking addresses, exact symbol sizes and proven short literal leaves, with reached direct Main/helper addresses. Execute/dialogue/cinematic/quest/sound/animation/object/level/tutorial effects and all blocking dispatch belong to Main.

V52 publishes the actual command C1 before Data reading, assigns SAME kind/Data/data_c, calls Init, then folds actual skip4. Failed reads/Init retain the reached prefix. Unload resets actual Data/pendingData then data_c before qualified base-storage release; command cleanup does not dispatch Execute/Finish. It then releases commands/names and resets original current/common fields. Extra original manager fields survive as in the retained source.

## Milestone verification boundary

This 80-slot candidate has not been compiled, linked, executed or checked against new authored maps. Earlier frozen V59 production syntax and V62 template syntax passes do not cover this extension. No per-kind tests/jobs were run. Existing useful fixtures remain frozen; one comprehensive milestone suite must update coverage expectations to 80, exercise all constructor/Data schemas plus weak native pointer expiry/high-bit aliases, preserve prior meaningful lifecycle/failure-prefix and callback-mutation cases, and invoke only genuine Main runtime leaves where admitted.

Retained original whole-parser evidence covers common+SWAMP 70 scripts/870 records/3495 values and 46 reached kinds. 51-level metadata contains 37 other unique local script pairs without command execution receipts; these maps remain unsampled. Static 80-slot storage coverage does not establish authored reach, behavior completion, mob/chest rendering or real saved-game integration.
