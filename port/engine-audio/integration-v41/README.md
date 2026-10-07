# V41 loader nullable manager and precache addendum

This extends the frozen V40 packet without changing it or shared application files. The loader report `container-audio-units-v50-original.json` proves 207 original Container prefixes over 68 real rows plus one missing name. Its LoadSound body was an observer. This packet independently executes the original LoadSound prefix and CreateInstance/DeleteInstance operations; backend construction leaves remain observers. No World, real audio driver or audible acceptance is claimed.

## Exact raw unit and reached gates

Container captures the global SoundManager before GetSound. Null skips GetSound and LoadSound altogether; it is a legitimate source branch only when the actual global is null. A missing getter/unknown initialization state is a required authority, not evidence of null. Preserve that captured identity even if GetSound changes the global.

LoadSound3699fc reads the actual disabled global first; disabled returns. Negative argument returns; arguments greater than manager+1c return. The original comparison is inclusive (`uid <= bound`), even though the constructor allocates arrays using a count. Do not silently change this boundary or index an unproved endpoint slot: the actual selected metadata/cache service must validate safe storage. It queries soundpack bank metadata at manager+64 using the raw argument, rereads the bound, then checks the same manager's cached slot at manager+8. Cached returns; an empty slot reaches exact source construction/loading. This path has no generated Sounds mapping, event selection, GS/Level phase, listener, focus, device timestamp or play command.

For the supplied chest rows, InitPost passes raw33 to LoadSound. XML33 is a direct selected-pack UID; interaction separately passes source ordinal33 to Play3D, which maps to XML162. Do not convert this precache argument to162 or replace precache with a muted play. The interaction clip remains missing. The late LoadSound construction tail has not been differentially ported here; V34 exact loading is a modern transport boundary after these proved source gates.

`audio_precache_v41.hpp` provides a new synchronous complete leaf. It captures an actual nullable manager plus a real lifetime lease, then calls the actual GetSound and source disabled/bound/metadata/recheck/cache prefix in order. Null, disabled, out-of-range and cached returns are typed source skips; failed/missing providers are required. Root supplies actual services; there are no default false/success callbacks. The final exact-pack loader receives the captured manager and raw UID unchanged.

## Construction, publication and lifetime

The original global s_instance9a238c starts zero. CreateInstance36ca88 returns unchanged if nonnull; otherwise it allocates0xc4 bytes with alignment4, calls C1 while the global is still null, then publishes the returned object after C1. Original C1 chooses data/sounds/sounds.xml and establishes source pack/engine fields; separate Initialize36c2e4 applies source settings/general initialization. Publish availability only after the actual corresponding operation completes. Parsed catalog readiness, a nonnull manager, initialized Vox settings and World/listener/output readiness are distinct facts.

DeleteInstance36b048 skips null, otherwise calls D1 then frees. This operation does not clear the global itself in the executed source. Its surrounding caller cleanup is unproved here. The modern owner must prevent stale published pointers: unpublish/stop accepting under its producer/owner lease, then perform V40 close/join/drain and only then free the manager/provider lease. Never infer that source DeleteInstance automatically makes the pointer null, and never free a captured receiver during GetSound/precache. A new World/session cannot replace a failed-close owner.

Before manager construction root may report actual null only if that is the true global ownership state. Once it constructs/publishes the actual source manager, it must keep it alive throughout loader InitPost, container receivers and gameplay services. If root needs to construct that manager, implement its source construction/initialization and exact assets first; bypassing the loader with has_sound_manager=false would change the reached source branch.

## Root insertion contract

Bind loader Container InitPost to the complete captured-manager precache leaf at its reached sound prefix. Existing separated has_sound_manager/GetSound/load_sound callbacks need a retained per-invocation snapshot: has_sound_manager captures the identity/lease, GetSound must not overwrite it, and load_sound uses that captured identity. Nested/reentrant receiver calls need separate scopes; a global last-manager variable is insufficient. Prefer a new complete-emission callback rather than silently changing shared service signatures. The rest of spawn/MeetCondition/visual/timeline/script and derived InitPost ordering stays unchanged.

Root's last loader closure checks actual manager identity against `runtime.manager()`, genuine exact selected-pack initialization, actual disabled/bound/cache fields and no teardown. Only then call V40 `load_actual_xml_uid(rawUID,error)` on its producer. Stop precache and native-state mutations before requesting session close. No Play3D gate or fabricated phase38 belongs here. The loader report proves the argument and snapshot order; it does not prove successful full InitPost or an installed SoundManager.

Validation is recorded in original-proof.json and host-validation.json. The frozen V40 manifest is retained as a hashed dependency. Shared build, loader integration and all-category audible acceptance remain open.
