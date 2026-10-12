# Loader save/return source integration

This coherent working candidate supplies actual state33 save/restoration orchestration and complete state34 prefix plus checkpoint-tail/CheckpointSave orchestration over the SAME native Level, Character, ObjectManager and save owners. No campaign/save state, VM, manager, Character, checkpoint cache or filename authority is created. Shared Root is untouched. The earlier state33 header remains unchanged except its ordinary source error clearing; parent is adopting it separately.

## Adopt source files together

- stage_loader_save_restore_v1.hpp: complete source state33 body, bound with bind_stage33_restore_v1 to existing LifecycleServicesV36; refuses replacement.
- stage_loader_stage34_v1.hpp: complete original state34 driver, bind_stage34_v1 on existing LifecycleServicesV36, actual_owner exposes SAME checkpoint controller for later real checkpoint calls. Uses existing camera and scene-frame owners, required typed Main engine primitives and the checkpoint tail below.
- port/level-loader/level_constructor_v3.hpp: current-Root header copy corrects SAME source19c/1a0/1a4 cells from uintptr_t to float. Their original C1 zero stores remain unchanged. Apply to matching selected physical constructor header as well; the stage34 static_assert rejects stale pointer-cell ABI.
- stage_loader_checkpoint_v1.hpp: LevelCheckpointOrchestrationV1 implements original signed-map GetSpawnPoint and whole Level.CheckpointSave. run_tail implements ONLY state34's checkpoint tail after genuine camera/environment/material prefix. It must not be installed alone as a completed whole stage34 body.
- port/level-world/level_savegame_owner_v1.hpp/.cpp: additive typed validate_checkpoint and save_checkpoint entry points on existing SAME owner/cache/fields. Existing busy/released/readiness guards protect genuine Main delivery. Original validity is an output distinct from delivery bool. Missing primitives fail explicitly.
- port/level-world/level_savegame_runtime_v1.hpp/.cpp: additive Main application callbacks and same-cache/same-fields checked bridges. Callback slots are appended after existing fields; existing constructor/save/load behavior is retained. No implementation library/second save runtime is introduced.
- port/level-world/level_savegame_cache_v1.hpp: exposes SAME existing mutable filename cell and actual cache-clear adoption point. source_clear_cache_v1 clears only current native cache/index observation after genuine Main clear; it preserves section registrations and ready owner. Existing retained diagnostic borrows remain owned. It performs no I/O, constructor replay or empty profile fabrication.

Copies of the five world files originate from current Root source, preserving its existing bodies. Apply their surgical additive API changes to any subsequently changed Root version rather than overwriting independent Main edits. Link existing save owner/runtime/cache and game_object_set_position_v2.cpp ONCE on dh2_level_world; these headers add no CPP target. Include coherent loader/world directories. Default callback omissions are required stops, never assumed checkpoint validity or persistence.

## Source-backed state33

Original _LoadProcess33 begins3f7630: Debug GetInstance/GetSwitch(isTracingLevel_Loading), result ignored; fresh actual byteF1 gates SAME LevelEC Load. Existing LevelSavegameOwnerV1 performs INFO then OBJS on actual retained cache. Offline skips position restoration. Online host floor query may update actual host160 Z, then propagates that SAME live point; nonhost byte6d0 controls propagation from actual manager6d4. Both loops reread genuine signed GetNumLocalPlayers(true) each iteration and GetLocalPlayer(index,true), preserving live values. Finally fresh local0/character660 reaches real SG_SetUseSpawnPoint(false), then actual byteF3=0; existing dispatcher owns130 increment/progress.

Stage33RestoreServicesV1 lends actual Debug, Network byte5, Application40 manager, local records, checked Character/base, PFWorld.GetFloorHeightAt(point,&height,NULL,NULL,NULL,false), independent attached/PF/visual position leaves and Character.SG_SetUseSpawnPoint. Whole SetPosition393db4/SetDestination393600 remain existing canonical source code. Weak actual Level/App/save references avoid retaining containing World/App; scoped native receiver pins exist only during delivery. Failed prefixes latch and are not replayed.

## Source-backed state34 checkpoint tail

Stored _LoadProcess3f6990 source proves3f6df8..6e8c plus3f7f00..7f50:

1. Fresh GetOnline byte5. Offline enters validation branch. Online nonhost or fresh manager719 nonzero enters checkpoint creation directly; eligible host enters validation branch.
2. Validation branch reads actual LevelEC. If nonnull, pass current seed114/difficulty40/row3c to SAME owner ValidateCheckpoint463288. Original valid=false creates. Valid=true, or absent save_ec, creates only if fresh byteF5 is nonzero.
3. Creation uses original GetSpawnPoint3ef614, then fresh local0 lookup even when a SpawnPoint exists. It selects actual SpawnPoint160, else actual Character160, else the original Vec3f_Origin. The exact original GOT slot9989c4 points to Vec3f_Origin99f854, size12, three zero words; this is a proven source constant, not an invented missing-object default.
4. Copy point words0/1, read actual byteF5 as force, copy word2, call original Level.CheckpointSave3f04b4, then clear SAME byteF5. Existing dispatcher increments130 afterward. A skipped creation does not fabricate a save.

GetSpawnPoint is implemented on existing canonical signed-order map: original ObjectHandle C1 and existing manager.resolve_handle_v4(false); actual typeF4==13, actual visible8a nonzero and SpawnPoint.entrypoint374==fresh Level110. Main lends only checked conversion to SAME mapped SpawnPoint alias and actual Handle constructor. No copied map, pooled prefab or name-based invented spawn is used. Native signed cursor traversal is the same retained manager traversal already used by loading adapters.

The complete stage_loader_stage34_v1.hpp orchestrates the original camera/level-id/environment/room/light/material prefix using existing typed owners and reached genuine Main primitives, then calls run_tail. run_tail by itself still proves only the checkpoint tail and must not be installed as a whole stage. bind_stage34_v1 refuses replacement of an existing body. Separate original LoadCheckpoint is not this branch.

## Whole Level.CheckpointSave order

Bounded named ARM source decode: CheckpointSave3f04b4 size232, GetSpawnPoint3ef614 size276, ValidateCheckpoint463288 size604 and SaveCheckPoint463130 size344. No original execution or broad binary scan occurred.

CheckpointSave first calls GetLocalPlayer(0,true) BEFORE guards. Fresh save_ec absent, signed phase130<=33, NULL Character, or actual Character virtual IsDead+34=true yields the authentic completed guard return. Otherwise it writes existing Character1468/146c/1470 from the input point word by word, preserving pointer aliases. Existing Character1474/1478/147c then receive that point when force is true, otherwise the SAME live Character160 point. Main's checked metadata lending must reference existing produced CharacterSaveMetadataV3/actual save fields, not fresh storage. Existing field labels are preserved; no speculative reinterpretation of raw points is introduced.

Then genuine Character.SG_SaveCheckpoint3bc494 executes. AFTER that leaf, fresh row3c/save_ec/seed114/difficulty40 are reread and passed to SAME LevelSavegame.SaveCheckPoint463130. The adapter allocates no save and performs no fake disk write. save_point is reusable at genuine later checkpoint call sites on SAME Level; reentrancy/failure prefixes latch. Its native guard returns are distinct from missing leaf failure.

## Precise real Main persistence primitives still required

LevelSavegameApplicationV1.validate_checkpoint receives the SAME mutable LevelSavegameCacheV1 and LevelSavegameFieldsV1 plus seed/difficulty/row and bool& original_valid. Original method derives checkpoint filename from seed and SAME mode0c, with online nonhost/manager719 selecting multi suffix. It probes actual FileManager existence, then real backup name if primary absent. Neither exists returns original valid=false. Present file retargets SAME Savegame filename, genuine _cacheFile(NULL) reloads SAME cache, and Savegame.load("INFO",actual callback, SAME LevelSavegame) writes actual loaded_row2c. Compare that row with requested row. Then derive original normal GetFilename(seed,difficulty,row,mode0c), retarget SAME cache filename and genuine _cacheFile(NULL) recaches it before returning original validity. Retain actual source prefixes on genuine failure. No copied saved-row predicate, hardcoded true or second checkpoint cache is accepted.

LevelSavegameApplicationV1.save_checkpoint derives original checkpoint filename using same online/hosting/719/mode policy, retargets SAME Savegame filename, performs genuine saveAll with registered INFO/OBJS, actual serialization, global jobs/file ownership, then restores original normal filename from fresh source args/mode. Original SaveCheckPoint does not recache the normal file afterward. Existing save_all backend and real FileManager/CFS/jobs remain Main authorities. source_filename_slot_v1 and source_clear_cache_v1 are storage access/adoption hooks at those real reached operations, not substitutes for performing them.

CheckpointServicesV1 additionally needs weak actual App/LevelSave runtime, actual manager40/byte719/local0 records, checked SAME Character/base, actual virtual IsDead, SAME produced checkpoint metadata cells, genuine Character SG_SaveCheckpoint and SAME Application38 object manager/Handle/SpawnPoint aliases. Every callback must capture containing App/World/Level weakly; independent service storage may be strongly retained. No whole loader-stage provider can replace the implemented ordering.

Separate Level.LoadCheckpoint3f0428 remains unimplemented integration: byteF4=1, optional real LevelSavegame.LoadCheckPoint46259c and GameEventManager.ReInit, actual local Character SG_LoadCheckpoint(16)/SG_Update(true), finally byteF4=0. The candidate does not call or claim that path.

LNAM/LEPT/LUSP/LVLS remain actual selected PlayerSavegame/profile owners. SWAMP row41/file001_swamp.mlx and SWAMP_02 row42/file022_swamp2.rule.xml stay distinct; this generic orchestration changes no selected row/name/seed or campaign state.


## Complete state34 prefix integration

The prefix preserves native ordering: ignored isTracingLevel_Loading query; actual manager40 and cached camera128 identity before local0; nullable Character SG_SetLevelId(row3c,-1) and SAME camera.SetTarget(0); online CMatching.Get/virtualA4 before fresh row3c dispatch(3,row); existing camera activation; genuine scene virtual60(0,0); existing camera.update; live count/local loop writing SAME controller378 logical byte8=1; fresh128 and existing source_positions_v67; Y,Z,X subtraction and direct stores1a0,19c,1a4. These three Level cells are floats, not pointers. Native PLT30e3ac is __aeabi_fsub.

It borrows existing actual Application58 fog parameter cells, not a new SceneManager. Config1e0 RGB converts through existing exact source_fog_color_byte_v68, preserving alpha/B/R/G store order. Fresh GetLevelConfig reads config1d8 and1dc SIGNED INTS and converts them to float params110/114; native PLT30e964 is __aeabi_i2f, not float-to-int. Config1f8 direction is written into live App58 params11c..124, then freshly borrowed config/scene1c writes existing SceneManagerFrameFieldsV67.fog_transform458. Fresh scene1c is pinned before actual RENDERING_DisableAllLighting query, whose inverse writes SAME lighting430. Existing parameter_revision metadata advances with those two groups.

Then fresh Application38 ObjectManager reaches genuine HandleNoRoomObjects345954, and exact Level.UpdateFog(NULL), UpdateLightSet(true,4), UpdateMaterial(false,true), followed by authentic checkpoint tail. No whole stage body is delegated. Completion requires all reached real operations.

Main must lend: actual PM/local records/counts; checked SAME Character/base and existing logical controller cell; genuine SG_SetLevelId; SAME camera128 via current V23.world()->camera->level() directory and existing RuntimeV11 methods; checked live Config38; actual App58 fog RGBA/start/end/direction cells; actual App10->scene1c registry and real virtual60; actual objects38 and no-room/room graph operation; real fog/light/material methods. Views are checked read-only owner resolutions; none may allocate alternate source storage or perform hidden stage effects. Strong delivery pins are scoped, all provider captures of containing App/World/Level remain weak. Missing real App58 parameter backing or engine leaves are explicit integration stops, not fabricated defaults. No new CPP is required for this header.
Source-only, uncompiled/unlinked/unrun. No builds, syntax checks, tests, hashes, freezes, proof packets, fixtures or native/original execution. Connect real Main primitives and validate only with the next BIG integrated first-SWAMP milestone.
