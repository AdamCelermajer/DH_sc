# Canonical positive loot composition V44

Status: **source subsystem frozen; positive host fixture PASS; live gameplay integration pending**.

New production files: world_loot_canonical_bindings_v44.hpp/.cpp and renderer_canonical_loot_connection_v44.inc. The only shared-header edit is the authorized inline WorldItemGraphV3::source_enabled_event_v44 passthrough. world_item_live_owner_v5.cpp was restored exactly after correcting a mistaken context diagnosis: local s already aliases factory.item, so s.context is correct. The reached regression verifies distinct factory and Item contexts.

## Implemented behavior

- Canonical Spawn uses the existing manager's whole ObjectHandle resolver, cached-address behavior and required NULL assertion. Item identity/type3/Handle/lifetime must match that same published receiver.
- TestEnableCondition reads SAME enabled8a/minimum_ec/disable_f1/compiled_a8/tested_ac. Actual PM.GetLocalPlayer(0,true) leads to an explicit Character14e8 Save borrow. Genuine NULL Character, NULL Save and constructor quest-sync byte0 retain their original early branches.
- Selected +44/+48 executes directly after enabled8a changes. The graph event updates visibility, source85, PF flags, real POItem sensor filter and disabled373 in order. Pre-InitOnce is allowed only with actual constructor-NULL visual2d8/physical2dc and executes those exact NULL branches without an invented graph.
- Current Level borrows only the actual GS global. Published completed C1 exposes SAME mode118/state130 and lifetime. Phase0 is legal; genuinely empty global returns NULL; unavailable global fails. No demo C1 fallback/state38 store exists.
- POItem services resolve SAME Item Handle/type, recognize physical callback addresses through typed projections and require same published peer/visible80/AsCharacter/OOI fields. Unknown addresses and detached copies of canonical borrows fail.
- SM_IsMoving(false) is exact: actual state4 or19; NULL StateInfo yields -1/false. Animation/controller heading never substitutes.
- PickUpType.Automatic uses existing CharacterGameDesign PyDataConstants. Source miss0 stays0; no hard-coded enum/shadow table.
- Renderer construction creates no pool until **actual published Level state29**. It composes V23/V31 and preserves independent callback contexts. It exposes no late-pool InitFinal call.

## Source lifecycle

Frozen loader state29: ItemManager.PreCache → ProjectileManager.PreCache → state130 increment.
PreCache: 29 audiovisual categories ×5 Spawn("Item",name,false,true), actual type3 guard, InitOnce, then DeSpawn.
InitOnce: category3ac; speed3b0=6; authored data/3D/GameObjects/itemdrops.bdae mesh or source bag fallback; **qualified GameObject.InitPost38be5c**; ApplyMeshBox if visual exists. Item virtual InitPost is bx-lr.
InitAgain: real inventory transfer/color lookup/authored clip0/POItem constructor/SetPhysicalObject.

State17 LoadFinalInit precedes145. ProcessNextGameObjectToStartUpdate consumes/splices pending queues and ZoneEntered; **not InitFinal**. Adapter/fixture never call InitFinal. PF.user retains constructor0 through tested body/drop lifecycle. Existing V23/V5 InitFinal APIs/comments do not establish a stage29 caller; never connect them to pending consumption.

Loader V45 genuine GS/C1 publication/frame is independently frozen/tested but stops loading3 on required Level.Update(false). This packet does not implement that producer.

## Evidence

- compile.json: strict portable implementation and test, both Android ABIs, PASS.
- renderer-compile.json: additive exact-current model_renderer fixture containing V4/V23/V44, both ABIs, PASS. Production model_renderer/CMake untouched.
- android-linked/receipt.json: both ABI actual APK-linked executables **COMPILED_NOT_EXECUTED**, exact APK/library/source/binary hashes recorded.
- host.json: O0 ASan+UBSan **PASS 1738 checks**. Original cache constructs145 canonical Items/BRES/qualified InitPost; real condition/event toggles and failure mutations; actual Box2D drop body/sensor; localized Item transfer; authored animation timeline/visibility; despawn/reuse/cleanup; canonical physical-peer rejection; exact moving predicate; actual constants; genuine NULL/missing GS branches; reached >99 notification failure preserving item100.
- Listed graph/canonical/PM/inventory/scene/animation support TUs rebuilt from current source. Remaining helper DSOs are declared older fixture support. Stale scene/animation ABI failures rejected and repaired by rebuilding current support, never suppressed. Exact inputs copied into bounded private Linux /tmp fixtures to avoid mounted-FS linker timeouts.
- Sound phase0/device/offline network/light/selected-item inputs are declared fixtures. No actual GS phase0-positive gameplay, source monster award/full Gear pickup/GLES pixels/APK install/device flow is claimed.
- No emulator/ADB/device operations occurred.

## Root integration

1. Add world_loot_canonical_bindings_v44.cpp to level-world; include its header at TU scope.
2. Include renderer_item_graph_v4.inc, renderer_character_loot_gameplay_v23.inc, then renderer_canonical_loot_connection_v44.inc after canonical World definitions. Do not also construct old standalone V4 Item graphs.
3. Connect/retain V31 actor owner on V23 provider request. Supply existing powers/audiovisual/Gear/presentation/Debug/RNG/status/pickup owners.
4. Construct V44 with SAME manager/PM and actual GS global slot. Supply original Save14e8/state20/physical-address/visible80/OOI and any reached destructor/condition/assertion providers. world Save is not14e8 without its producer.
5. At actual source Level state29 ItemManager.PreCache call, invoke precache_source_stage29; it rejects missing GS/wrong phase/repeat. Real Level subsequently owns ProjectilePreCache and130 increment.
6. Route Kill drop service only after that producer. Other Kill services stay with existing owners.
7. Use gameplay()->frame, actual draw scenes/interaction/Flush/destruction through V23. Supply source frame facts on SAME floor/PF/workspace/motion/camera/auxiliary/online owners; never force PF InitObject or invent source callbacks.
8. Root supplies actual player/NPC/decor physical callback-address projections. Item peers compose through V23. Recognized actual PhysicalObject owner8=NULL requires its lifetime; unknown pointers fail.
9. Preserve real pickup/status/quest/FX/net/tutorial/text continuations. Unsupported reached branches fail explicitly. Runtime acceptance must test kill→drop→move→pickup after real Level/PM/Save producers connect.

## Acceptance boundaries

| Area | Status |
| --- | --- |
| Canonical factory/enable/currentGS/physics/state/constants composition | Implemented; strict ABI/host fixture accepted |
| Source late145/InitOnce/InitAgain/body/animation/ownership | Positive original-cache fixture accepted; live stage29 not invoked |
| Whole Level.Update/_LoadProcess/state29 | Loader/root required; V45 stops loading3 |
| Character14e8 Save association | Typed provider required; progression reports V29 unpublished |
| Player/NPC/decor physical address → canonical fields | Renderer/root projections required; Item fixture accepted |
| Full active Item frame/movement/camera/auxiliary | Existing V5 APIs retained; actual frame provider composition required |
| Monster award/pickup FX/quests/Gear/pending scheduling | Existing APIs retained; integrated acceptance pending |
| Later genuine InitFinal producer for145 | Unresolved; no stage17 replay/pending fake |
| APK/device/GLES/visual acceptance | Pending root controlled QA |

