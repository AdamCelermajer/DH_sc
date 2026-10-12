# First SWAMP script scheduler source handoff

Source inventory and ownership interface only. No scheduler, cinematic, command execution, compilation or runtime verification is claimed. Reuses the already decoded cache inventory; no new parser or command arena.

The existing App `ScriptManagerOwnerV52` is the sole PYS manager. Load common C00 first (15 scripts / 81 commands), then SWAMP L01 (55 scripts / 789 commands). This produces 70 scripts / 870 commands with common IDs 0–14 and local IDs 15–69. These IDs apply to that original common-first load order; another level appends its own local data and must use its actual name lookup.

C00: `data/pydata/scripts_pyscripts.bin` + `data/pydata/scripts_pyscriptnames.bin`.<br>L01: `data/pydata/scripts/001_swamp_pyscripts.bin` + `data/pydata/scripts/001_swamp_pyscriptnames.bin`.

`id_from_name(name,true)` searches from 0; `id_from_name(name,false)` searches from SAME `fields.common8` (15 after common load). Names are byte strings from actual source records; the adjacent JSON preserves raw name bytes. SWAMP_02 has separate L22 local resources and identities, despite shared swamp artwork.

The 46 reached command kinds are: 0, 1, 2, 3, 5, 6, 8, 10, 12, 13, 14, 15, 16, 17, 19, 20, 21, 22, 23, 24, 25, 26, 27, 29, 30, 31, 32, 39, 40, 41, 42, 43, 44, 45, 46, 51, 52, 54, 55, 63, 68, 69, 70, 77, 78, 79. The existing source receivers/data schemas cover all 80 classes. Runtime work should consume these same loaded receivers and retained Data objects.

## Same-owner APIs

- `script_manager_owner_v52.hpp/.cpp`: `fields()`, mutable checked `context_at(id)`, read-only `commands()`, `names()`, `name_from_id`, `id_from_name`, original `load_commands/load_names/unload_all`.
- `ScriptContextV52`: actual command0 / field4 / state8. `ScriptCommandsV52`: actual count0 / skip4 / optional storage8. Command borrow contains actual_owner / skip4 / kind8 / data_c / retained_data / init / release_storage.
- `script_command_receivers_v59.hpp/.cpp`, `command_c1_descriptors_v59.inc`, `script_data_schemas_v52.inc`: actual concrete command factory and retained Data; use `checked_data_borrow` and the existing typed ActualWeakTargets56–59 for current Main owners.
- `script_command_init_v62.hpp`, `script_manager_bind_v63.hpp`, `script_manager_complete_bind.hpp`: exact Init transport and strict complete binding on the existing manager, not replacement Execute/StartScript.
- `candidate/script-command-complete/MAIN-RUNTIME-BOUNDARIES.md`: all 80 original Execute / IsBlocking addresses, classes and recovered literal returns for the runtime owner.
- Menu/Main scheduler owns StartScript, IsRunning, ExecuteAllScripts, IsBlocking and command effects. Add a bounds-checked mutable script accessor on THIS existing manager if required for original skip4/storage writes; do not const-cast or create another context/command/name registry.
- The App PYS manager is distinct from actual Level.script44 Lua and GameObject300 Lua. `candidate/all-map-script-source/SWAMP-OBJECT-SCRIPTS.md` covers separate object Lua: five chest empty-script paths and ten NPC script references. Keep actual Script300 deletion/creation ordering.
- Cinematic/dialogue/quest/actor/camera execution remains Main/Menu-owned; loader supplies original identities, source Data, actual Init resources and trigger references.

## Original common and local script IDs

| ID | Pair | Local index | Source name (display) | Commands | Reached kinds |
|---|---|---|---|---|---|
| 0 | C00 | 0 | BeginCutScene | 4 | 22, 23, 39, 79 |
| 1 | C00 | 1 | BeginScriptedCutScene | 8 | 1, 22, 23, 24, 32, 39, 70, 79 |
| 2 | C00 | 2 | EndCutScene | 3 | 12, 22, 23 |
| 3 | C00 | 3 | EndScriptedCutScene | 8 | 2, 12, 22, 23, 25, 31, 32, 69 |
| 4 | C00 | 4 | OnEnterLevel01 | 12 | 22, 23, 24, 25, 26, 31, 32, 39 |
| 5 | C00 | 5 | OnEnterLevel02 | 12 | 22, 23, 24, 25, 26, 31, 32, 39 |
| 6 | C00 | 6 | PlayerDeathScript | 9 | 0, 3, 22, 23, 63 |
| 7 | C00 | 7 | PlayerDeathScript_01 | 2 | 10, 12 |
| 8 | C00 | 8 | PlayerDeathScript_02 | 2 | 10, 12 |
| 9 | C00 | 9 | PlayerDeathScript_03 | 2 | 10, 12 |
| 10 | C00 | 10 | PlayerDeathScript_04 | 2 | 10, 12 |
| 11 | C00 | 11 | PlayerDeathScript_05 | 2 | 10, 12 |
| 12 | C00 | 12 | PlayerLevelUp | 1 | 13 |
| 13 | C00 | 13 | ShowUnlockedTrophies | 7 | 24, 25, 31, 32, 39, 68 |
| 14 | C00 | 14 | WaitQuestUpdate | 7 | 12, 24, 25, 31, 32, 39 |
| 15 | L01 | 0 | Camp_Intro | 29 | 0, 8, 10, 12, 24, 25, 26, 41, 46, 79 |
| 16 | L01 | 1 | CombatTuto | 24 | 0, 8, 10, 12, 24, 25, 77 |
| 17 | L01 | 2 | Door_Inspection_South | 13 | 0, 8, 10, 12, 24, 25, 26 |
| 18 | L01 | 3 | FaeryBackFlip | 1 | 45 |
| 19 | L01 | 4 | FaeryIdle | 1 | 45 |
| 20 | L01 | 5 | FaeryLoop | 1 | 45 |
| 21 | L01 | 6 | FaeryTwist | 1 | 45 |
| 22 | L01 | 7 | Faerycrazy | 1 | 45 |
| 23 | L01 | 8 | FirstQuest_Start | 1 | 3 |
| 24 | L01 | 9 | Flushit | 2 | 3, 79 |
| 25 | L01 | 10 | HackSave | 1 | 46 |
| 26 | L01 | 11 | Hide_Troll | 2 | 3, 26 |
| 27 | L01 | 12 | LizBlood_Activate | 17 | 0, 8, 10, 12, 24, 25, 26, 41 |
| 28 | L01 | 13 | LizBlood_PostAccept | 5 | 0, 8, 10, 12, 25 |
| 29 | L01 | 14 | LizCaptain_Intro | 23 | 0, 8, 13, 24, 25, 26, 30, 41, 44, 45 |
| 30 | L01 | 15 | LizardMan_Ambush1 | 4 | 26, 30 |
| 31 | L01 | 16 | LizardMan_Ambush2 | 2 | 26, 30 |
| 32 | L01 | 17 | LizardMan_Intro | 12 | 0, 8, 24, 25, 26, 30, 78 |
| 33 | L01 | 18 | MothIntro | 23 | 0, 5, 8, 13, 24, 25, 30, 31, 41, 45, 46 |
| 34 | L01 | 19 | Movement_Tuto | 1 | 78 |
| 35 | L01 | 20 | Movement_Tuto2 | 17 | 0, 10, 12, 24, 25, 26, 77, 79 |
| 36 | L01 | 21 | PlayerDialog | 1 | 45 |
| 37 | L01 | 22 | PlayerIdle | 1 | 45 |
| 38 | L01 | 23 | ResumeLevelMusic | 1 | 15 |
| 39 | L01 | 24 | Swamp2_Intro | 19 | 0, 8, 10, 12, 24, 25, 26 |
| 40 | L01 | 25 | Swamp_Camp_PrisonerB_a | 12 | 0, 10, 12, 24, 25, 41 |
| 41 | L01 | 26 | Swamp_Camp_PrisonerB_b | 8 | 0, 10, 12, 24, 25, 41 |
| 42 | L01 | 27 | Swamp_Camp_PrisonerC_a | 8 | 0, 10, 12, 24, 25 |
| 43 | L01 | 28 | Swamp_Camp_PrisonerC_b | 11 | 0, 10, 12, 24, 25, 26, 41 |
| 44 | L01 | 29 | Swamp_Camp_PrisonerE_a | 10 | 0, 10, 12, 24, 25 |
| 45 | L01 | 30 | Swamp_Intro | 164 | 0, 5, 6, 8, 10, 12, 14, 19, 20, 21, 24, 25, 26, 27, 29, 30, 31, 41, 42, 43, 45, 46, 51, 52 |
| 46 | L01 | 31 | Swamp_Serpent_Defeated | 104 | 0, 3, 5, 6, 8, 10, 12, 13, 19, 20, 21, 24, 25, 31, 41, 43, 45, 46, 51, 52, 54 |
| 47 | L01 | 32 | Swamp_Serpent_Intro | 113 | 0, 5, 6, 8, 10, 12, 13, 14, 20, 24, 25, 29, 31, 41, 42, 43, 45, 46, 55 |
| 48 | L01 | 33 | Swamp_Witch_Activate | 29 | 0, 8, 10, 12, 24, 25, 26, 40, 41 |
| 49 | L01 | 34 | SwimKing_01 | 3 | 3, 26, 30 |
| 50 | L01 | 35 | SwimKing_02 | 3 | 3, 26, 30 |
| 51 | L01 | 36 | SwimKing_03 | 3 | 3, 26, 30 |
| 52 | L01 | 37 | TrollReturn | 19 | 0, 5, 8, 13, 24, 25, 26, 31, 39, 40, 45 |
| 53 | L01 | 38 | Tutorial_Merchant | 22 | 0, 8, 10, 12, 24, 25 |
| 54 | L01 | 39 | chest_tuto | 16 | 0, 8, 10, 12, 24, 25, 26, 77 |
| 55 | L01 | 40 | cinematic_Tuto_levelUp | 14 | 0, 10, 12, 24, 25 |
| 56 | L01 | 41 | cinematic_Tuto_potionUse | 16 | 0, 10, 12, 24, 25 |
| 57 | L01 | 42 | enterLocation_MerchantCamp | 2 | 10, 16 |
| 58 | L01 | 43 | enterLocation_North_Boardwalk | 1 | 10 |
| 59 | L01 | 44 | enterLocation_South_Boardwalk | 1 | 10 |
| 60 | L01 | 45 | enterLocation_StartRoom | 1 | 10 |
| 61 | L01 | 46 | enterLocation_Swamp | 1 | 10 |
| 62 | L01 | 47 | enterLocation_Swamp_EDWARD | 6 | 3, 10, 42, 46 |
| 63 | L01 | 48 | enterLocation_Swamp_King | 1 | 10 |
| 64 | L01 | 49 | enterLocation_Swamp_RENE | 6 | 3, 10, 42, 46 |
| 65 | L01 | 50 | enterLocation_Swamp_RENE2 | 5 | 3, 42, 46 |
| 66 | L01 | 51 | enterLocation_To_Witch_Cave | 1 | 10 |
| 67 | L01 | 52 | exitLocation_MerchantCamp | 2 | 10, 17 |
| 68 | L01 | 53 | open_SwampKingDoor | 3 | 26, 54 |
| 69 | L01 | 54 | tutorial_treasure | 1 | 78 |

Source basis: `candidate/all-map-script-source/source-script-coverage.json`, pairs C00/L01; original source-backed V52 load/name lookup. This handoff is ready for Main to relay to its scheduler delegate.
