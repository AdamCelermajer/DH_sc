# Original world records and module placement

This source component imports the owner's original exported MLX/MGP/MVP records
into an owned world description. The first verified area is **SWAMP**, using
`data/scene/001_swamp.mlx`. It does not execute the original engine, copy its
object ABI, render a level, or make the original game playable by itself.

## Source-backed milestone

The static level file contains one `LevelConfig` and nine `Module` instances.
The importer preserves every attribute, original object name, exported transform,
file path, zero-based record number and original byte span. All eighteen selected
MGP/MVP files import successfully: **148 gameplay records and 47 visual records**.
These include 50 characters, 11 spawn points, three level exits, 37 animated decors
and ten decors. Records describe both active objects and templates/hidden actors;
importing them does not automatically spawn them.

The original SWAMP entry zero is `_prim_EntryPoint`, at
`(1090.75,-212.202,258)`, with `entrypointID="0"`. The introductory lizardman
`_prim_Monster_LizManIntro1` has local position `(2274.25,252.509,250)` in
module `obj_3of4_brdwalk_sw_00_1`; its world position is
`(-3725.75,252.509,250)`. Its original `ai_state="Limbus"`, `auto_spawn="0"`
and `ai_state_visible="0"` are retained. They have not been executed.

`Level.name` is supplied by the caller. This component does not decode the
LevelList table or independently derive the key `SWAMP`; the recovered table
associates LevelList row 41 with static LevelFile `001_swamp.mlx`.

## Catalogue coordinates are not level coordinates

`data/3d/modules/swamp/swamp.bdae` is a catalogue with multiple roots.
The selected root for each module is its original `xrefobject` plus `-node`.
The first module root is `_module_obj_4of4_brdwalk_sw_00-node`, at catalogue
position `(52000,3000,0)`; the original level places this module at `(0,0,0)`.

`dh2_world_bind_module` validates a unique root with identity rotation and unit
scale. `dh2_world_module_records` returns this root and its complete descendant
subtree in preorder, without flattening geometry. `dh2_world_place_matrix`
replaces its catalogue origin by its MLX origin while keeping descendant
rotation, scale and translation. `dh2_world_placement_matrix` supplies the same
translation as a correction matrix for left multiplication. All nine selected
SWAMP roots pass the checks. Rotated or scaled module/root placement is rejected
until its original conventions are reconstructed.

Original native evidence in `recovered/native/decompiled/libDungeonHunter2.so/`
and `recovered/native/assembly/libDungeonHunter2.so/`, using **ELF addresses**:

| Original routine | Address | Relevant behavior |
| --- | --- | --- |
| `SceneManager::LoadScene` | `0x3596f8` | Appends `-node` to `xrefobject` and constructs that scene node. |
| `VisualObject::SyncPosition` | `0x470cb8` | Sets the loaded visual root position from its owning object. |
| `Module::LoadModule` | `0x38a88c` | Sets the level's current module origin/ID and loads selected MGP/MVP XML containers. |
| `ObjectManager::LoadFromXML` | `0x34b868` | Adds the current module origin to an exported object's XYZ position; explicit additions appear at `0x34ba8c..0x34bad8`. |

Ghidra pseudo addresses use a `+0x10000` base. This port performs the evidenced
translation-only placement. It does not claim general original transform parity.

## Integration and ownership

Public interfaces are in `world.hpp` and `world_scene.hpp`. Initialize `Level`
with `{}`, then call `dh2_world_import_level`, import each selected module's two
files with `dh2_world_import_module_objects`, and finally call `dh2_world_free`.
The level owns its arrays and strings; source XML bytes may be freed immediately
after import. Object `name`/`gametype` alias owned attribute values. Object pointers
can move when further module files append entities, so retain indices rather
than pointers during loading. Failed imports leave the previous level intact.

The source reference tuple `(Level.name, module instance name, RecordKind,
source_record)` distinguishes an object instance. It is a port provenance key,
not a reconstruction of original runtime integer object IDs. Original names may
repeat, so they must not be assumed globally unique.

The caller supplies file bytes. Explicit supplied-cache normalization lowercases
ASCII, changes backslashes to slashes and maps `data/iphone/...` to `data/...`.
Original field strings remain unchanged. The selected eighteen files and BRES
resolve with this policy. Absolute/traversal paths and empty segments are rejected;
the original filesystem aliasing implementation has not been recovered here.

The reader supports the supplied export subset: `Level`/`Module` containers,
self-closing `GameObject` records, comments, UTF-8 quoted attributes and standard
XML character references. It rejects child elements, custom entities and DTDs.
Limits are 8 MiB per XML file, 4,096 records per file, 128 attributes per object,
256 modules, 32,768 total entities, 4,096 bytes per attribute value and 1,024 bytes
per normalized path. No new parser dependency or C++ exception runtime is used.

## Validation

From the repository root:

```text
python port/world-data/build.py --report port/world-data/build/build-validation.json
python port/world-data/tests/check_world.py --library port/world-data/build/libdh2_world_host.dll --cache ../cache/files --report port/world-data/build/cache-validation.json
python port/world-data/build.py --ndk ../emulator-test/sdk/ndk/29.0.14206865 --report port/world-data/build/build-validation.json
```

Host compilation uses warnings as errors. ARM64 compilation uses API 26 and
16 KiB maximum page alignment; it is a compile check, not an ARM64 device run.
The validation script independently compares attributes, source byte spans,
transforms and record counts against Python's XML reader for all original selected
files. It checks each catalogue subtree against recursive scene traversal,
corrected root positions, insufficient record capacity, malformed/truncated input,
bad paths/numbers/UTF-8, duplicate imports, unsupported transforms and ownership
preservation. No original cache files are added to this component. Reports and
compiled artifacts remain ignored under `build/`; source and input SHA-256 values
are recorded without timestamps.

## Remaining work

Renderer integration must select entire module subtrees and batch geometry within
buffer limits, preserve per-draw materials/UVs, and load the original visual decors.
World navigation still needs floor selection, height and module connections.
Object factories, property defaults, activation condition evaluation, scripted
spawn/limbo behavior, camera/dialog/tutorial commands, original quest transitions,
collision zones and level changes are separate systems. The native command
programs and their recovered conditions have not been executed by this importer.
This component loads the real original area data; it is not a completed level.
