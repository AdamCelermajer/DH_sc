# Retained authored mesh FX owner V1

The bounded production owner constructs, samples and retains the three actual mesh swoosh resources selected by the Prince animation tables: sets253/254/255, dictionary files272/273/274. It owns the BRES bytes, decoded scene, mesh/index/attribute data, node animation player, texture accessors, timeline, active/free instances and set-data identities. Draw snapshots retain geometry, a copied material table and copied material bindings through subsequent sampling, reuse and owner destruction. It does not construct particles, force/cloud/billboard, skin/controller/light/camera families, redirected/multi-step sets, or the original GPU objects. Reached unsupported declarations fail explicitly.

The source cache names are `swoosh_prince_1hand_combo_01/02/03`; paths are `data/3D/interface/<name>.bdae`. Their source timeline ends are333/366/2500ms. Each contains a real mesh instance, a scene animation instance, constant node TRS tracks, a type87 texture-transform track, and type26 color track. Actual extended factory611ae0 returns null for26, so that particular track is genuinely ignored. All three diffuse materials are additive and reference `fx_weapon_trail_blue_gradual.tga`. UVs animate; the authored node TRS does not. Live-anchor movement is a separate tested producer.

The actual Knight/Mage/Rogue animation-table recursive selection reaches these three sets. The starter items themselves all have SwooshFX word6=-1: 1079/1073/1076/664;1080/1074/1077/1025;1081/1075/1078/370; potion925. Loot IDs165/174/213 are loot-table IDs, not item indices. No invented per-item swoosh was added.

## Source lifecycle

- GetAnimFXData4933e4 swaps authored orientation byte+10/+11 on output. The native data projection preserves that swap and source signed/wrapped loop calculation.
- SetAnimFX492e8c delivers Sync(false), Sync(true), Sync(false), then speed; only afterward writes set-data, loop and callback. Play495b54/4956b8 writes position, optional fixed rotation, looping/start, then SetAnimFX; supplied data alone changes the timer. A null anchor does not clear a retained anchor.
- HandleLoopEnd4927a4 returns when speed<=0 and callback is null, or loop<0. Positive loops decrement and restart. Zero sets looping false, writes finished, invokes the captured callback, clears it after return, and performs the current end-time sample.
- Update492f68 clears dead/disabled anchor, subtracts actual Application dt from a nonnegative timer with source32 wrap, follows a nonstationary anchor, delivers genuine debug prefixes, resets speed, and hides a completed nonlooping visible mesh. The source trace switch literal is `isTracingAnim_FX` for both set and instance branches.
- Original AnimController SetCallbacks474d10 is a tail branch to474cac, not an empty routine. It installs real end callbacks. This corrects the initial size4 inference; no no-completion workaround remains.
- Manager callback4963d0 appends pending FX even with null set-data. Manager Update496594 processes active instances in file/list order, then end-samples pending instances and removes/hands back those no longer visible. The owner keeps scene and manager phases separate.
- Drop494978, after list removal/free insertion, clears anchor and Sync(true), zeros position and Sync(false), hides and Sync(false), then nulls the caller pointer. It retains timer/loop/callback/rotation/scale/set-data. Warm Get494ad4 resets material alpha, pops the free LIFO and appends active. Cold allocation is rejected only when active count>5, so six live instances are possible.

The source manager constructor493260/4932c4 starts precache byte+4 at0. PreCacheLibraries495a88 delivers Debug.Load/GetModule(`AnimatedFX`) and only on enabled result calls4933d8 to write1. The native owner also starts false and exposes `precache_libraries()`. Cache0 Drop only nulls the caller; it does not reset or pool. The four-case executed startup probe is in `manager-startup-probe.json`.

## Proof and explicit boundaries

The source ELF SHA256 is36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80. Captures copied into `captures/` preserve original manifest/assembly bytes. Source cache extraction manifests bind the original authorized cache ZIP and exact resources.

Original↔NDK29 O2 ARM64:4530 kernel/accessor cases plus2560 lifecycle cases,10954 ordered service snapshots,65 actually reached synchronous mutation cases. The lifecycle executes SetAnimFX, HandleLoopEnd, Update and full Play/Drop callers; scene sync/animator/visibility/debug/end sampling and Drop free-vector allocation are explicit services. Direct source anchor byte reads and set.finished writes are projected as typed native services; no native STL layout equivalence is claimed.

ASan/UBSan/LSan: all7090 gold cases replay exactly;708 whole owner checks cover three real authored resources, three animated UV draw outputs, three same live35-node Prince-scene anchor movements, three completion returns, LIFO reuse, six-live cap, cache0 behavior, required failure/particle rejection, and snapshots after destruction. The genuine DebugSwitches/DebugModules owners use an actual missing-file filesystem read. Anchor/floor/entity flags are explicit caller projections; the fixture is not a full live GameObject manager. The historical copied central155 DSOs are binary-bound separately, not relabeled as rebuilt from current source.

## Renderer contract

1. Retain EffectsTables::Borrow, synchronous asset provider, genuine Debug/entity/floor providers and the SAME stable live Character scene before constructing owner. These must outlive every active call; no provider may destroy them during a callback.
2. Call `precache_libraries()` at the source startup point. `play_set` accepts only the proved type0 single-step/nonredirected domain. `animation_event` implements only actual `fx_` ordered lookup using caller-supplied actual owner position, NULL anchor/parent; other event families belong to their recovered dispatchers.
3. Advance `scene_frame(actual absolute milliseconds)` in scene order, then `manager_frame(actual Application dt)` in manager order. Retain returned draw snapshots while GPU submission uses them. The geometry/UV/material/outer matrix data are real; texture loading and material/GPU pipeline must be provided separately.
4. Anchor position/rotation/scale must be genuine live source projections. Rotation is radians; a visual matrix producer uses recovered matrix_rotation_degrees followed by source factor bits3c8efa35. Custom FX scale vs ordinary owner scale remains the required anchor_scale provider's source selection. No orientation is inferred from a proxy rendered actor.
5. Quiesce/destroy this owner before replacing its borrowed live Scene. Resource snapshots may survive owner destruction. This is a native lifetime contract, not original scene manager destructor parity.

## Central integration

Add exactly four new TUs to dh2_level_world: `character_fx_kernels_v1.cpp`, `fx_texture_animation_v1.cpp`, `character_fx_state_v1.cpp`, `character_mesh_fx_owner_v1.cpp`. Existing data, scene/math, asset/resource, animation, skinning and visual_timeline dependencies supply the genuine backing.

Audit targets:

- `tests/character_fx_state_v1.cpp` → link dh2_level_world; arg `reference/character-fx-owner-v1/state-fixtures.bin`.
- `tests/character_fx_kernels_v1.cpp` → link dh2_level_world; args kernel-fixtures.bin and the directory containing exact three swoosh BDAEs. Its cosf/sinf/sincosf fixtures supply original captured imported libm values, rather than attributing modern host libm to historical Bionic.
- `tests/character_mesh_fx_owner_v1.cpp` → link dh2_level_world/data/animation/scene/skinning; args effects-table directory, swoosh-resource directory, exact Prince model BDAE.

Reproducible isolated command: `wsl.exe --exec bash /mnt/c/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/character-fx-owner-v1/build-host.sh`. No CMake, renderer, APK or device files were modified. Particle emission/rendering, generic do_fx parser, wider set families, parent-set continuation and complete source factory/GPU parity remain unfinished.
