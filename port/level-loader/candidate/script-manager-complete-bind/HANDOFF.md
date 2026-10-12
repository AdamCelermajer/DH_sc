# Actual complete 80 ScriptManager composition

This candidate closes one concrete source integration gap without adding another manager implementation. The existing V52 load path gives services.data_factory precedence over loader storage for every kind. V63 originally preserved that callback as an extension for missing Data kinds. The complete command batch now supplies every original slot0..79; V52 rejects all other kinds. An overriding data_factory therefore lies outside the claim that these 80 source schemas are being used.

script_manager_complete_bind.hpp adds bind_complete_script_manager_services<Types>(cache, actual_init, actual_execute, out, error). It rejects a populated data_factory and missing callback interfaces for any of the five required Init bodies. It then delegates to existing V63. It never replaces an existing provider, instantiates a manager or command, opens a file, calls an Init/Execute leaf, or creates runtime state. Output remains unchanged on rejection. This is a one-time complete-contract construction route; the older partial V63 entry point remains available for an explicitly separate provider contract.

The strict check covers structural callback presence only. A populated placeholder does not establish a real engine body, an actual owner, or functional behavior. Reached body/owner failures keep the original V52/V59 prefix and failure behavior. Binding rejection occurs before manager construction and does not reproduce a Load prefix.

## Integration paths and owner authority

Only the new header needs adoption from this candidate. There is no ScriptManager .cpp/.hpp copy here and no link target change.

Use the existing active private loader include directory, which supplies:
script_manager_bind_v63.hpp -> script_command_init_v62.hpp ->
script_command_receivers_v59.hpp -> script_manager_owner_v52.hpp.
The active V59 HPP/CPP/descriptors/Data include must be the complete 80 batch already adopted by parent. Compile/link the existing script_manager_owner_v52.cpp exactly once; its quoted schema include selects the active 80 file. Do not add the isolated historical command candidate's copied manager implementation to a build.

At Main's existing actual App ScriptManager authority construction point:
1. Obtain existing independent cache/stream services with real open_file. Keep App separate from services.owner; no direct or alias App ownership or hidden strong App capture.
2. Populate all 12 typed V62 Init interfaces with existing actual Main leaves; preserve weak App captures and scoped actual-owner borrows.
3. Supply genuine Main Execute dispatch for SAME V59 receiver/Data and original skip/context arguments.
4. Compose with bind_complete_script_manager_services. Construct the ONE actual App-owned ScriptManagerOwnerV52 only after composition succeeds. There is no mutable manager service rebinding API.
5. Bind THAT SAME retained manager with existing bind_script_manager_v52 and put those services in the stage8 actual loader composition. That stage adapter creates no manager or global. Its strong reference pins the existing manager; V63 callbacks retain App weakly. Preserve distinction from actual Level.script44/Lua owner.

The complete entry point checks no cache internal graph or callback captures. Main must maintain their independent/weak ownership. Existing genuine command_factory remains incompatible with installing another factory and is rejected by V63. Existing data_factory is now explicitly rejected by the complete route and remains untouched for its own real provider route.

## Load and C1 audit

V52 still peeks one source kind and rejects values outside 0..79. All 80 descriptor factory addresses align with the retained original factory table and all 80 concrete classes return that exact kind. The common base storage fields remain skip4 byte 0, kind8 = -1, data_c NULL.

V52 immediately publishes each actual C1 owner before Data creation/read, checks its original factory and actual field pointers, then owns pending Data. It assigns SAME kind8/retained_data/data_c only after Read completes. The canonical class validates SAME descriptor, concrete kind, complete Data source kind and identity before Init. It calls literal base 0x455628 for 75 slots and the V62 body adapter only for 10/20/22/23/45. Init success is guarded against actual header/Data replacement; failed prefixes do not replay.

Each reached source Init uses the authentic callback and live/cached operand order retained in the complete batch HANDOFF.md. The complete binder does not move those reads or invoke leaves during checking. New command kinds add no nontrivial Init interfaces.

Manager Unload clears Data and data_c before qualified actual base storage release; no Execute/Finish runs. Command teardown/names clearing/context/common reset order remains the existing V52 implementation. No real loader body was copied or replaced by this candidate.

Stage8 preserves original config/fog then Unload -> common commands -> common names -> reborrow current config -> local commands -> local names. It keeps context failure/state guards and source .pyscript suffix/slash handling. Empty, missing and no-marker paths retain existing source behavior; successful return does not manufacture script availability or gameplay readiness.

## Interfaces Main still needs

* Application, Constants, MenuManager, Menu, FXManager, ObjectManager, ObjectHandle, Object, Character and Animator must be actual Main types. The loader cannot supply stand-ins.
* Required Main functional FX registration and animation dictionary registration are actual engine mutations, not optional success leaves.
* Main owns StartScript/Update/context scheduling, Execute effects and every IsBlocking dispatch. V52 exposes actual retained command records/contexts, but supplies no scheduler or IsBlocking API. The complete binder adds neither. Main may dispatch only its trusted factory's canonical actual receivers; no unrelated void owner should be cast as a V59 receiver.
* Execute's bridge bool signals successful actual dispatch. Literal source BX LR bodies define no gameplay effect or source bool result. Consult the complete batch MAIN-RUNTIME-BOUNDARIES.md for all 80 exact runtime method boundaries.
* Main must coordinate its scheduler with manager Unload and checked receiver/Data borrows. A retained stale receiver rejects Init/Execute after base release; a pin is not permission to run after teardown.
* Correct script byte buffers and real cache/file lifetimes are required. Missing compiled assets remain original absent-file behavior; stage completion does not imply names and commands exist or are paired. Main must reject invalid runtime context IDs/command indices using its actual source dispatcher.

## Unsupported and pending milestone paths

This source inspection does not implement real Main leaves, validate hidden capture graphs, establish an installed Main authority, or prove runtime behavior. An independently supplied Data provider is unsupported by this complete route. The legacy route can retain a genuine provider only under its own explicit provenance contract; it cannot claim use of complete 80 storage.

The existing comprehensive fixture suite should include complete-binding output preservation, rejection of an existing Data override without invoking it, each missing required Init interface, existing factory/direct/alias-App rejection, and genuine weak App lifetime behavior. These are one milestone binding/lifetime checks, not individual kind tests. No tests, compilation or runtime jobs were launched here.

Retained whole-parser evidence covers common/SWAMP 70 scripts, 870 records, 3495 values and 46 reached kinds. The 80-slot extension remains source-complete and uncompiled/unrun. Other 37 local pairs remain unsampled; authored map reach, actual IsBlocking/Execute effects, mob/chest renderer admission and full gameplay readiness are still Main/milestone work.
