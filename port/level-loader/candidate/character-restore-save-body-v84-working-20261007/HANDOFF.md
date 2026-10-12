# Character restoration source body V84

Source-ready private proposal; UNCOMPILED. No shared Root changes, tests, builds, hashes, fixtures, native execution or persistence. No Checkpoint class/header modified. Main owns integrated validation and the current Checkpoint388 constructor-empty Character set correction.

Candidate: `C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc/port/level-loader/candidate/character-restore-save-body-v84-working-20261007`.

## One narrow same-owner batch

- `player_savegame_v1.hpp`: two additive source stores on existing SavedLocationV1.use_spawn_point and levels. No new fields/Save/serialization/disk operation. Apply the narrow integration.patch to current Main header rather than overwriting later changes with the full proposal copy.
- `renderer_character_restore_save_v84.inc`: include in existing model_renderer namespace immediately after renderer_character_campaign_lookup_v62.inc. Public typed methods reuse its actual current-world Character borrower; actual Save14e8 association/controlblock checked. No runtime/factory/registry creation.
- `renderer_source_stage34_v80.inc`: successor of corrected V80 composition only. Fills absent Stage33.set_use_spawn_point and Stage34.set_level_id with these source bodies; fills absent checkpoint_fields using current Main actor checkpoint1468_v83()/save_position1474_v83(). Supplied genuine providers remain authoritative. Checkpoint set/save/is_dead and all scene/light/material callbacks are preserved, not replaced.
- `integration.patch`: existing game-data header stores, two declarations in renderer_character_campaign_v62.hpp, and one textual include in existing model_renderer.cpp. No new TU or source-owner CMake target.

Dependencies: current Main fresh-canonical RetainedCharacterActorV1 V83 fields/accessors must exist. Historical/adopted actors whose checkpoint_c1_v83 is false are rejected; this proposal does not replay C1 or invent metadata. V80 retains exact scene(float,bool) signature and parent float camera fields. Shared renderer_source_stage34 V80 callback installation remains atomic.

## Exact original bodies

SG_SetUseSpawnPoint3bb798: load SAME Character14e8, NULL BX before difficulty read; else load process PlayerSavegame.m_difficultyLevel9a6060, write raw bool byte at Save4c+difficulty. Proposal uses actual record.services.difficulty_global, not App4c selected difficulty or Level constructor difficulty.

SG_SetLevelId3bb854: load14e8, NULL BX first. Explicit difficulty writes raw signed level at Save50+4*difficulty without global read. Only -1 fetches SAME process global then writes same location cell. Negative/out-of-retained-array indices reject unsafe host memory access; arbitrary signed level values remain unvalidated. This is location metadata, not Save30 player progression/class/entrypoint.

Both setters preserve the exact NULL-save early return and do not call PlayerSavegame.Save, checkpoint serialization, profile IO or quest logic. Genuine record and actual Save ownership are pinned synchronously; weak Stage33/34 wrappers capture no containing World/Level/actor strongly.

SG_SaveCheckpoint3bc494: load14e8, NULL BX, otherwise tail-call existing PlayerSavegame.SG_SaveCheckpoint466184. Its actual serialization/network/job/save delivery remains Main's supplied checkpoint.character_save_checkpoint. Do not substitute ordinary SG_Save flags, success or a new Save writer.

## 1468/1474 semantics and ownership

Original Level.CheckpointSave3f0520..3f0574 stores three freshly reread input words at Character1468/146c/1470; then chooses force?original input:actual Character160 and stores three freshly reread words1474/1478/147c; finally SG_SaveCheckpoint. The first triple is a checkpoint point here, not rotation despite historical CharacterSaveMetadataV3 labels. Existing body already preserves individual load/store alias behavior.

Main's current actor V83 produces the six words only at fresh CharacterC1/C2 zero-store prefix3aa46c/3a95f8; getters return NULL otherwise. The new metadata receipt owns SAME actor and returns these exact mutable arrays, with SAME source Character identity. No CharacterSaveMetadataV3 object/copy/tags/alternate pose is allocated. Current Main Checkpoint388 Character set and save callbacks remain incoming genuine providers.

Existing historical RetainedCharacterSaveConnectionV3 extra.metadata/respawn labels remain a separate integration concern: when Main wires the actual fresh Character serializer, it must lend the same actor words rather than allocate a second pose object and copy them. This proposal changes only reached Stage33/34 in-memory setters and metadata borrow, leaving the real save/destruction owner intact.
