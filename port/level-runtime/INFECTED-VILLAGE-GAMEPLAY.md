# Infected Village authored gameplay map

This note records what can be established from the recovered Infected Village
cache files and the existing bounded PyData decoder. It is an authored-data
audit. It does not run the game scripts or instantiate their referenced
characters, triggers, exits, or quest events.

## Reproduce

The audit uses the caller-supplied extracted cache, Python's standard library,
`port/level-catalogue/catalogue.py`, and
`port/pydata-scripts/pydata_scripts.py`. From the repository root:

```powershell
python port/level-runtime/tests/audit_infected_village_gameplay.py --cache-root ../cache/files --report ../emulator-test/infected-village-gameplay-audit.json
```

The script fails if the catalogue entry, authored record order/counts, script
names/command counts, trigger-to-script references, exit targets, or quest-zone
record change. The generated JSON includes every record's source file and
zero-based record index, decoded command payloads and byte offsets, and SHA-256
hashes for the exact inputs below. It neither uses nor modifies the Android
build, `runtime.py`, or the asset-bundle code.

## Cache input closure

The LevelList table identifies `INFECTED_VILLAGE_01` as
`data/scene/005_infectedvillage.mlx`. That MLX's `LevelConfig.scriptFile`
selects the level script-table pair using the recovered `.pyscript` filename
convention. Four `ExecScript` commands use absolute IDs in the common table,
which is why the common script pair is also decoded.

The audit reads these eleven files relative to `--cache-root`:

```text
data/pydata/levels_pyarray.bin
data/pydata/levels_pyarraynames.bin
data/scene/005_infectedvillage.mlx
data/3d/modules/infectedvillage/mgp/infected01.mgp
data/3d/modules/infectedvillage/mvp/infected01.mvp
data/3d/modules/infectedvillage/mgp/infected02.mgp
data/3d/modules/infectedvillage/mvp/infected02.mvp
data/pydata/scripts/005_infectedvillage_pyscriptnames.bin
data/pydata/scripts/005_infectedvillage_pyscripts.bin
data/pydata/scripts_pyscriptnames.bin
data/pydata/scripts_pyscripts.bin
```

No BDAE, model, animation, or other level's geometry is read by this gameplay
audit.

## The 53 authored source records

The count is three records in the MLX (`LevelConfig` plus two `Module`
instances) and 50 records in the two modules' MGP/MVP files. Module indices
follow the order of the two `Module` GameObjects in the MLX. Per-file record
indices are zero-based `GameObject` order.

| Module | Source | Records | Game types in source order counts |
| ---: | --- | ---: | --- |
| MLX | `005_infectedvillage.mlx` | 3 | `LevelConfig` 1, `Module` 2 |
| 0 | `infected01.mgp` | 39 | `Character` 26, `DestructibleContainer` 3, `Door` 3, `OpenableContainer` 2, `SoundEmitter` 1, `SpawnPoint` 1, `TriggerZone` 1, `TriggerZoneExitLevel` 2 |
| 0 | `infected01.mvp` | 4 | `AnimatedDecor` 4 |
| 1 | `infected02.mgp` | 2 | `QuestMoveInZone` 1, `TriggerZone` 1 |
| 1 | `infected02.mvp` | 5 | `AnimatedDecor` 5 |
| **Total** |  | **53** | **50 module-owned records + 3 MLX records** |

The full names and `(source file, record index, module index, game type)` for
all 53 records are in the audit JSON's `records` array.

## Ambush triggers and script requests

There are two authored `TriggerZone` records named
`_prim_TriggerZone_ambush`. Module 0's `infected01.mgp` record 20 and module
1's `infected02.mgp` record 0 both have `script="Ambush"` and
`triggercount="1"`. The decoded level script table has six scripts and 36
commands; script index 0 is `Ambush`, containing five `SpawnCharacter` requests
in this order:

| Command offset | Requested object | Matching static `Character` record in these two modules |
| ---: | --- | --- |
| 8 | `_prim_tmp_infected05` | module 0, `infected01.mgp` record 6; `auto_spawn="0"`, `ai_state="Limbus"` |
| 36 | `_prim_tmp_infected07` | module 0, `infected01.mgp` record 8; `auto_spawn="0"`, `ai_state="Limbus"` |
| 64 | `_prim_tmp_infected17` | No object with this name in either module's MGP or MVP records |
| 92 | `_prim_tmp_infected06` | module 0, `infected01.mgp` record 7; `auto_spawn="0"`, `ai_state="Limbus"` |
| 120 | `_prim_tmp_infected16` | module 0, `infected01.mgp` record 19; `auto_spawn="0"`, `ai_state="Limbus"` |

The audit searches exact names across all 50 MGP/MVP GameObject records from
both modules, not only `Character` records. `_prim_tmp_infected17` therefore
has no matching name in the audited static module source. This is an unresolved
static name reference, not proof of a game data defect: the audit does not
inspect every possible engine-side or dynamically registered object. The same
all-record search finds no names matching the three `ExitParty` requests
(`_prim_tmp_infected_exit01`, `02`, and `03`).

The native `TriggerZone::InitPost` resolves the authored script name, and
`TriggerZone::Update` can start a trigger script after its activation and
condition checks. Native `Script_SpawnCharacter::Execute` performs a named
`ObjectManager::GetObjectByName` lookup and calls `SM_SetSpawnState` only when
the handle converts to a `Character`; a failed lookup or non-Character handle
silently skips that call in this routine. That caveat makes the missing static
name worth following up in a live object-registration trace, while leaving open
whether another system registers it dynamically. These native paths explain
the relationship between the fields and the commands; this audit does not
invoke them. The two triggers reference the same five requests, but this does
not establish that either trigger activates, when it activates, or how
repeated/spatially overlapping activation is handled.

## Other scripts, exits, and quest zone

| Index | Script | Commands | Authored command summary |
| ---: | --- | ---: | --- |
| 0 | `Ambush` | 5 | `SpawnCharacter` ×5 |
| 1 | `ExitParty` | 5 | `SpawnCharacter` ×3, `Wait` ×2 |
| 2 | `Inspect_Cliff` | 12 | cutscene `ExecScript` ×2, `StartDialog` ×4, `WaitDialog` ×4, lock/unlock |
| 3 | `Inspect_Well` | 12 | cutscene `ExecScript` ×2, `SetCameraTarget` ×2, `PlayCamera` ×2, `StartDialog` ×2, `WaitDialog` ×2, lock/unlock |
| 4 | `ResumeLevelMusic` | 1 | `PlayLevelMusic` ×1 |
| 5 | `enterLocation_Infected` | 1 | `StartDialog` ×1 |

The two `Inspect_*` scripts' absolute common script IDs 1 and 3 decode to
`BeginScriptedCutScene` and `EndScriptedCutScene`. `enterLocation_Infected`
contains `StartDialog(-1, 3, 1704040)`. These are parsed command payloads, not
dialogue or camera playback.

The only two authored exits are in module 0, `infected01.mgp`:

| MGP record / name | Direct `levelName` / `entrypointID` target | LevelList file | Raw `fasttravel` field |
| ---: | --- | --- | --- |
| 0 / `_prim_ExitLevelZone_toDW` | `DARKWOOD` / 9 | `003_darkwood.mlx` (static MLX) | `a08_DARKWOOD_INFECTED_VILLAGE_01_ENTRANCE` |
| 3 / `_prim_ExitLevelZone_toBasement` | `SECRET_BASEMENT_01` / 0 | `006_basement.rule.xml` (procedural rules) | `SWAMP_BEFORE_SWAMPKING` |

The first `fasttravel` value is a separate FastTravelList row pointing to
`INFECTED_VILLAGE_01` entrypoint 0; it is not the exit's direct `DARKWOOD`/9
target. The second raw value is not a FastTravelList name in the recovered
table. The audit preserves these fields separately and does not infer an
unlock or transition from either string. The direct destination catalogue
rows resolve to a static MLX and procedural rule file respectively, but this
audit does not open either destination's contents or verify that the requested
entrypoint is usable. The native exit and UI dispatch paths are
`TriggerZoneExitLevel::Update` and `NativeGoToZone`; neither is run here.

The one `QuestMoveInZone` is module 1, `infected02.mgp` record 1,
`_prim_QuestMoveInzone_wellstart`, with authored local position
`-3401.95,-4100.47,500.0` and an empty `activate_cond`. Native
`QuestMoveInZone::OnCollisionBegins` raises an asynchronous
`v2QuestObjectiveType/MoveInZone` event for a qualifying collision. This audit
only identifies the record; it does not create contact or dispatch a quest
event.

## Evidence boundaries

Relevant recovered native routines are in
`recovered/native/decompiled/libDungeonHunter2.so/`:

- `functions-004.pseudo.c`, ELF `0x00403a58`, `Level::_LoadScripts`, loads the
  common script tables and then derives the level table paths from LevelConfig.
- `functions-002.pseudo.c`, ELF `0x003ac0a8` `TriggerZone::InitPost` and
  `0x003ab458` `TriggerZone::Update`; ELF `0x003ac33c`
  `TriggerZoneExitLevel::Update`; ELF `0x003a6120`
  `QuestMoveInZone::OnCollisionBegins`.
- `functions-006.pseudo.c`, ELF `0x0046f400`,
  `Script_SpawnCharacter::Execute`; the routine does a named object lookup and
  only changes spawn state after conversion to a Character handle.
- `functions-006.pseudo.c`, ELF `0x004707f4`,
  `Script_ExecScript::Execute`; an absolute-script flag prevents adding the
  level-script base to the ID.
- `functions-005.pseudo.c`, ELF `0x004422b8`, `NativeGoToZone`.

What this host evidence does **not** activate or establish:

- Character construction/lifecycle, spawn transitions in a running scene, or AI.
- Collision/contact and trigger activation conditions.
- Dialogues, camera changes, or music playback.
- Exit menus, level transitions, FastTravel effects, or quest dispatch.
- A walkable connection between the two module placements. Their MLX
  translations alone say nothing about connected floors, navigation edges,
  collision, or traversability.
