# Level catalogue and SWAMP exit validator

This is a host-only parser/validator for the recovered `LevelList` and
`FastTravelList` pydata tables and the three authored SWAMP exit records. It
verifies names, file references, entrypoint IDs, activation-condition strings,
and fast-travel unlock references. It does **not** perform level loading,
module selection, activation-condition evaluation, or spawn-point validation.

## Run

From the repository root:

```powershell
python port/level-catalogue/tests/test_catalogue.py
python port/level-catalogue/catalogue.py --cache-root ..\cache\files
```

The cache root must contain `data/pydata/levels_pyarray.bin`,
`data/pydata/levels_pyarraynames.bin`, and the referenced `data/scene` and
`data/3d/modules` files. The CLI prints a JSON report and exits nonzero on a
truncation, unsafe path, schema/count mismatch, unknown reference, missing
scene file, or SWAMP exit mismatch.

## Recovered format evidence

The format follows the original ARM native readers and the recovered
name-registration trace:

- `recovered/native/decompiled/libDungeonHunter2.so/functions-008.pseudo.c`
  contains the `Arrays::FastTravelList::read` and `Arrays::LevelList::read`
  bodies. Their ELF addresses in `reports/pydata-array-name-trace.json` are
  `0x004ba8e4` and `0x004ba794`; the corresponding `readNames` ELF addresses
  are `0x004b6964` and `0x004b4f28`. Each array starts with a little-endian
  `u32` count. The names file holds the FastTravelList string table first,
  then the LevelList string table; each table uses a `u32` count followed by
  `u32 byte-length + UTF-8 bytes` for each name.
- `recovered/native/decompiled/libDungeonHunter2.so/functions-011.pseudo.c`
  has `Structs::FastTravelDestination::read` around line 1288 and
  `Structs::LevelDeclaration::read` around line 1218. The fast-travel record
  wire order is `i32 DescriptionId`, `i32 EntryPointId`, length-prefixed
  `LevelName`, `i32 LocationType`, `i32 StringId`. The level record wire
  order is `bool Dbg_IsStable`, length-prefixed `DynamicBusRouting`, `i32 Hub`,
  `bool IsRandom`, `i32 LevelDescription`, length-prefixed `LevelFile`, then
  nine `i32` fields in the registered order documented in
  `reports/pydata-struct-name-trace.json`.
- `reports/pydata-array-name-trace.json` records 33 FastTravelList names and
  51 LevelList names for `data/pydata/levels_pyarraynames.bin`.
- `recovered/native/decompiled/libDungeonHunter2.so/functions-002.pseudo.c`
  contains `TriggerZoneExitLevel::Update` (around line 30291): the authored
  `levelName`/`entrypointID` are sent to the fast-travel UI, while the distinct
  `fasttravel` property unlocks a FastTravelList name. The actual selection
  route is `NativeGoToZone` in `functions-005.pseudo.c` (around line 32718),
  which reads the selected LevelName and EntryPointId and resolves the
  LevelList file before calling the native load path. These recovered paths
  are why the validator keeps the exit target and unlock destination separate.

The validator rejects binary source files larger than 8 MiB and XML source
files larger than 16 MiB before reading or parsing them. It uses a capped read
after the file-size check as well, so a file that grows between the check and
read cannot exceed the limit in memory. The binary parser also caps counts and
string lengths before allocating or iterating; checks every byte read,
validates boolean encodings and UTF-8, and rejects trailing data. It validates
unique member names, `LevelName` cross-references, supported `LevelFile`
suffixes, cache containment, and file existence.

The oversized-source regression tests lower the caps to small fixture values
and verify early rejection for both catalogue binaries and the SWAMP scene and
module XML paths.

## SWAMP data verified

`data/scene/001_swamp.mlx` references the selected module MGPs. Reading their
`GameObject` records gives these target fields:

| Module index / MGP | Object record | LevelName / entrypointID | LevelFile | Kind | activate_cond | fasttravel unlock |
| --- | ---: | --- | --- | --- | --- | --- |
| 4 / `merchantcamp_ruins_swe_00.mgp` | 13 | `SWAMP_02` / 0 | `022_swamp2.rule.xml` | procedural rules | `IsAfter_Gothicus2Survivors` | `Invalid` |
| 6 / `deadend_brdwalk_w_00.mgp` | 1 | `SWAMP_CAVE_WITCH_A` / 0 | `002_swamp_witchcave.rule.xml` | procedural rules | absent | `a01_SWAMP_CAMP` |
| 7 / `bossroom_ruins_ns_.mgp` | 7 | `DARKWOOD` / 0 | `003_darkwood.mlx` | static MLX | `IsAfter_Swamp_Escape` | `a01_SWAMP_CAMP` |

`a01_SWAMP_CAMP` is independently decoded from FastTravelList as destination
`SWAMP`, entrypoint 1. The witch cave exit's own target is the cave at
entrypoint 0; its `fasttravel` property names an unlock destination and does
not override that target. Activation-condition strings and unlock rows are
kept as separate `SwampExit` fields for the same reason.

The `.rule.xml` suffix marks a procedural rule input. This validator confirms
the corresponding cache files exist and does not assert that a concrete set of
modules or a matching active SpawnPoint has been assembled from those rules.
