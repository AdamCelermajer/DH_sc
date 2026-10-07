# SWAMP connected-owner class gaps

All 205 retained source declarations span nine modules. The frozen dispatcher supplies three allocation hooks covering 92 declarations; the 13 additional class types below cover 113 declarations. Hook availability does not prove complete runtime construction.

| Class | Count | Original factory | First source | Context |
|---|---:|---|---|---|
| CheckpointZone | 4 | `0x340f2c` | `data/3d/modules/swamp/mgp/obj_1of4_brdwalk_nse_00.mgp` element 13 | actual initialized Module runtime ID and post-property offset |
| Decor | 10 | `0x3410fc` | `data/3d/modules/swamp/mvp/obj_3of4_brdwalk_sw_00.mvp` element 6 | actual initialized Module runtime ID and post-property offset |
| DestructibleContainer | 9 | `0x340d5c` | `data/3d/modules/swamp/mgp/obj_4of4_brdwalk_sw_00.mgp` element 10 | actual initialized Module runtime ID and post-property offset |
| Door | 4 | `0x340824` | `data/3d/modules/swamp/mgp/merchantcamp_ruins_swe_00.mgp` element 5 | actual initialized Module runtime ID and post-property offset |
| Dummy | 41 | `0x3410a4` | `data/3d/modules/swamp/mgp/obj_4of4_brdwalk_sw_00.mgp` element 4 | actual initialized Module runtime ID and post-property offset |
| LevelConfig | 1 | `0x340ca8` | `data/scene/001_swamp.mlx` element 1 | Level reset context |
| Module | 9 | `0x340f98` | `data/scene/001_swamp.mlx` element 2 | Level reset context |
| QuestMoveInZone | 1 | `0x340f50` | `data/3d/modules/swamp/mgp/merchantcamp_ruins_swe_00.mgp` element 15 | actual initialized Module runtime ID and post-property offset |
| SoundEmitter | 1 | `0x340ccc` | `data/3d/modules/swamp/mgp/deadend_brdwalk_w_00.mgp` element 7 | actual initialized Module runtime ID and post-property offset |
| SpawnPoint | 11 | `0x340e10` | `data/3d/modules/swamp/mgp/obj_4of4_brdwalk_sw_00.mgp` element 11 | actual initialized Module runtime ID and post-property offset |
| TriggerObject | 1 | `0x340f0c` | `data/3d/modules/swamp/mgp/merchantcamp_ruins_swe_00.mgp` element 7 | actual initialized Module runtime ID and post-property offset |
| TriggerZone | 18 | `0x340ec4` | `data/3d/modules/swamp/mgp/obj_4of4_brdwalk_sw_00.mgp` element 15 | actual initialized Module runtime ID and post-property offset |
| TriggerZoneExitLevel | 3 | `0x340ea0` | `data/3d/modules/swamp/mgp/merchantcamp_ruins_swe_00.mgp` element 14 | actual initialized Module runtime ID and post-property offset |

Factory addresses are original ARM provenance, never callable native pointers.

The first original source declaration is `LevelConfig`, `data/scene/001_swamp.mlx` element 1, name `level_config`. Its missing dispatcher constructor fails before Add or early InitPost. The next nine declarations are Module. Do not omit either class to reach mobs/chests.

After genuine Module properties and registration, use the actual module ID, class position and ChooseXmls provider. Gameplay then visual source loading stays in that module context; reset offset to zero and ID to -1 afterwards. Inspection module indices and authored map projections remain diagnostic only.

Further acceptance requires signed-key lifecycle ordering, real condition evaluation, per-class InitFinal, SceneManager/PF attachment, failed-candidate cleanup, canonical restoration and commit. Conditions that evaluate false are valid source outcomes. None of these are replaced by generic success callbacks.

This census is separate from the executed first-failure receipt. It does not claim the loader traversed beyond the first failure.
