# Native asset and gameplay overrides

This guide describes the override paths routed by the Android source port at
checkpoint `c55d6fa1`. Overrides are a port feature, not recovered behavior of
the original game. The table below separates the one device-verified case from
paths that have source call sites but have not been verified with a device mod.

| Input | Status at this checkpoint |
| --- | --- |
| `worlds/crypt01.dwld` in the development/inspection Crypt | Device-verified on API 37 with 16 KiB pages |
| Data tables and compiled scripts listed below | Source-routed only |
| Existing model, animation, actor, and texture resources listed below | Source-routed only |
| Normal menu Crypt and SWAMP inputs listed below | Source-routed only |
| Arbitrary new names, extensions, worlds, or files with no loader call site | Unsupported |

## Install an override

Create the exact relative path under the app's files `mods` directory. For the
current package, the normal external path is:

```
/sdcard/Android/data/com.example.dh2/files/mods/
```

If external app storage is unavailable, the app uses its internal files
directory. Examples below are paths relative to `mods/`.

The resolver checks for an override first. If it is absent, the app reads the
packaged asset. If an override exists but cannot be read or parsed, loading
fails; the resolver does not silently fall back to the packaged copy. Removing
the override and restarting restores the packaged baseline.

The path lists use brace notation as shorthand for the literal alternatives.
For example, `{pyarray,pyarraynames}.bin` means the two filenames
`pyarray.bin` and `pyarraynames.bin`. It does not mean the app supports glob
patterns or arbitrary filenames.

## Data tables — source-routed only

These exact paths under `mods/` have source call sites:

```text
data/DebugSwitches.savegame
data/character_properties_{pyarray,pyarraynames,pystructnames}.bin
data/character_models_dictionary_{pyarray,pyarraynames}.bin
data/character_classes_{pyarray,pyarraynames,pystructnames}.bin
data/skills_{pyarray,pyarraynames,pystructnames}.bin
data/faeries_{pyarray,pyarraynames,pystructnames}.bin
data/faeries_pycst.bin
data/ai_pycst.bin
data/design_pycst.bin
data/levels_{pyarray,pyarraynames,pystructnames}.bin
data/worldmap_{pyarray,pyarraynames,pystructnames}.bin
data/v2quests_{pyarray,pyarraynames}.bin
data/v2quests_pycst.bin
data/ai_{pyarray,pyarraynames,pystructnames}.bin
data/ai_factions_{pyarray,pyarraynames,pystructnames}.bin
data/animations_{pyarray,pyarraynames,pystructnames}.bin
data/animations_dictionary_{pyarray,pyarraynames}.bin
data/loot_table_{pyarray,pyarraynames,pystructnames}.bin
data/loot_audiovisual_{pyarray,pyarraynames,pystructnames}.bin
data/prince-animation-bank.bin
data/animations_pycst.bin
data/effects_pyarraynames.bin
data/projectiles_pyarraynames.bin
```

The following recovered-cache tables are also explicitly routed:

```text
original-cache/data/pydata/trophies_{pyarray,pyarraynames,pystructnames}.bin
original-cache/data/pydata/design_pyarray.bin
original-cache/data/pydata/item_powers_{pyarray,pyarraynames,pystructnames}.bin
original-cache/data/pydata/item_powers_monopoly_{pyarray,pyarraynames,pystructnames}.bin
```

These inputs can change values the current port consumes, including character
properties and vitals, class/model mappings, skill and faerie declarations,
level-selection metadata, AI configuration and factions, animation mappings,
loot tables, audiovisual item mappings, item powers, and selected debug/design
values. They do not provide a general API for arbitrary `.bin` files.

Keep related `pyarray`, names, and schema files coherent. The readers validate
row identifiers, counts, property indices, dictionary references, animation
IDs, and character/model links. Only SWAMP row 41 and Crypt row 23 have native
source level loaders; other rows fail with `Selected LevelList row has no
native source loader yet`. Full AI pursuit/state-machine behavior is not
implemented. Trophy changes are in-memory and are not persisted.

## Lua and script inputs — source-routed only

The runtime routes these fixed monster-AI scripts:

```text
scripts/ai/_commons.luac
scripts/ai/monster.luac
```

The player skill and faerie scripts are selected by existing validated table
names. Their paths are:

```text
scripts/skills/_commons.luac
scripts/skills/faerie_{celest,hotty,rocky,wetty,windy}.luac
scripts/skills/prince_mage_{chain_lightning,coldray,cyclone,elemental_mastery,faeryblood,magi_shielding,staffmaster,stone_escape}.luac
scripts/skills/prince_rogue_{acrobat,ambidextrous,combomaster,findweakness,jump_kick,quickness,roundhouse,vicious_strike}.luac
scripts/skills/prince_warrior_{bashdown,charge,combat_master,fast_metabolism,ground_slam,hardiness,leadership,potion_addict}.luac
```

There are also fixed Crypt scripted-spawn inputs:

```text
scripts/crypt-ghost01.dctr
scripts/scripts_pyscriptnames.bin
scripts/scripts_pyscripts.bin
scripts/007_crypt_01_pyscriptnames.bin
scripts/007_crypt_01_pyscripts.bin
```

These routes can alter existing skill/faerie behavior, routed monster-AI
callbacks, and the existing Crypt scripted-spawn trigger. The runtime reads
compiled `.luac` files; loose source `.lua` files are not routed. Script names
come from validated tables, so adding unrelated identifiers is unsupported.
Only native script services implemented at this checkpoint are available. In
particular, animated-effect load/object/enable and remote-online branches fail;
there is no complete skill or gameplay scripting API. Script, table, or service
failures abort the owning initialization rather than silently using the bundled
script. Packaged provenance JSON such as
`scripts/ai/monster-ai-provenance.json` has no loader call site and is not an
override input.

## Models, animations, actors, and textures — source-routed only

The inspection picker lists only these packaged model names:

```text
models/candle_flame.bdae
models/class_selection.bdae
models/main_menu_charactere_swamp.bdae
models/prince_modular.bdae
```

Gameplay directly reloads `models/prince_modular.bdae`. Adding a new model under
`mods/models/` does not add it to the picker or to a fixed renderer model name.

Animation routes include:

```text
animations/prince_menu_idle_{knight,mage,rogue}.bdae
animations/prince_idle_shield.bdae
animations/prince_walk_1hand.bdae
animations/<path named by data/prince-animation-bank.bin>
```

Bank-selected replacements must use the authored path under `animations/`,
match the selected clip ID, and have the byte length recorded by that bank.

Actor routes include the exact names referenced by current validated inputs:

```text
actors/<validated DACT object.model>
actors/<basename of a validated monster animation-table path>
actors/itemdrops.bdae
actors/<equipment URI from the selected item data>
actors/data/3d/camera/cameratests.bdae
actors/data/3d/camera/playercamera.bdae
actors/data/3d/camera/animations/common/camera_idle.bdae
```

These names are resolved from the selected tables and fixed call sites; the app
does not scan `actors/` for arbitrary new entries. For the `actors/data/3d/`
paths, packaged fallback reads are redirected to
`original-cache/data/3d/...`; the override path remains under
`actors/data/3d/...`.

Texture lookup uses the lowercased material texture name:

```text
textures/<lowercased existing material texture name>
```

Use the exact existing name and extension referenced by the material. Texture
paths are lowercased by this call site; other resource paths are not. These
routes can change referenced mesh/material appearance, existing player and
actor visuals, compatible animation poses/timing/events, camera animation,
equipment and item-drop visuals, and referenced textures. They do not add new
content identifiers.

BRES-backed model, world-mesh, and actor resources must pass the existing BRES
header and scene parsers. Geometry needs valid positions and triangle indices;
the current limits are 65,536 primitive vertices, 3,000,000 indices, and
1,000,000 total vertices. Animation clips must bind to a compatible rest
scene/skeleton, and actor animation banks must have expected states and bounded
redirect depth. Textures must decode through the native texture loader and fit
`GL_MAX_TEXTURE_SIZE`. Do not assume every BRES subtype is supported; type-1
geometry is not supported at this checkpoint.

## Levels, worlds, and spawns

### Development/inspection Crypt — device-verified descriptor

```text
worlds/crypt01.dwld
```

This is the eight-room development/inspection level, not the normal menu's
generated Crypt. The device smoke test changed its spawn X coordinate by 100
game units, observed native world initialization, rejected a malformed
replacement, then removed the override and recovered baseline loading.

Its fixed routed dependencies are source-routed only:

```text
worlds/crypt.bdae
worlds/crypt01.spwn
worlds/crypt01.dact
worlds/x07_crypt_backup.mlx
```

`DWLD` is a port format, not the original `.mlx` format. Version 1 has a
24-byte header: magic `DWLD`, little-endian version 1, room count, then three
float32 spawn coordinates at offsets 12, 16, and 20. Each room record is 128
bytes and contains its template identifier and placement. The reader validates
identifiers, finite values, table size, and spawn/floor compatibility. The JNI
descriptor limit is 65,560 bytes. Use the level exporter where possible rather
than editing binary offsets by hand.

### Normal menu Crypt — source-routed only

```text
worlds/007_crypt_01.rule.xml
original-cache/<rule-folder>/mgx/mgxlist.txt
original-cache/<rule-folder>/mgx/<module>.mgx
original-cache/<rule-folder>/mvx/<module>.mvx
original-cache/<rule-folder>/mgp/<gameplay>.mgp
original-cache/<rule-folder>/mvp/<visual>.mvp
worlds/crypt.bdae
```

These inputs drive procedural module selection and topology, generated spawn,
imported actors, visual decoration, and environment/collision geometry. The
rule-folder component must be a safe relative name: no leading or trailing
slash, `..`, backslash, doubled slash, dots, or characters outside ASCII
letters, digits, `_`, `-`, and `/`. Module files must have the matching `.mgx`,
`.mvx`, `.mgp`, or `.mvp` suffix and a safe basename; the assembled module
catalogue must be complete. `generated-crypt.dwld`, `.spwn`, and `.dact` are
internal generated results, not paths to place under `mods/`.

### SWAMP — source-routed only

```text
worlds/001_swamp.dwld
original-cache/data/scene/001_swamp.mlx
original-cache/data/3d/modules/swamp/swamp.bdae
```

The routed SWAMP module also reads each of these exact basenames once under
`original-cache/data/3d/modules/swamp/mgp/` with `.mgp`, and once under
`original-cache/data/3d/modules/swamp/mvp/` with `.mvp`:

```text
obj_4of4_brdwalk_sw_00
obj_3of4_brdwalk_sw_00
obj_1of4_brdwalk_nse_00
corner_ruin_ws_00
merchantcamp_ruins_swe_00
corner_brdwalk_se_00
deadend_brdwalk_w_00
bossroom_ruins_ns_
obj_2of4_brdwalk_sw_00
```

`worlds/001_swamp.dwld` selects the SWAMP route, but the route then replaces its
layout/spawn bytes with data compiled from the MLX/MGP inputs. Editing that
DWLD is not an authoritative way to change SWAMP layout or spawn.

The following packaged paths are not read as overrides at this checkpoint:

```text
worlds/001_swamp.spwn
worlds/001_swamp.dact
worlds/crypt*.mgp
worlds/crypt*.mvp
worlds/*-provenance.json
worlds/*-actors.json
arbitrary new worlds/*.dwld
```

The SWAMP route compiles spawn/DACT data instead of reading its packaged
`.spwn`/`.dact`; the picker does not add new worlds. Only the explicitly listed
source routes are covered by this guide.

## Loader safety, diagnostics, and restoring the baseline

Every override must be a regular file from 1 through 33,554,432 bytes. Paths
reject leading/trailing slashes, empty components, `.`, `..`, backslashes,
colons/drive prefixes, and NUL. Canonical path checks reject symlinks that
escape the mods directory. Missing files use the packaged asset; present but
invalid or unreadable overrides fail without fallback. Parsers also enforce the
format-specific restrictions above.

Useful `DH2Native` Logcat messages include:

```text
Mod asset loaded | <relative-path> | bytes <N>
World ready | rooms ...
World load failed: <reason>
Model load failed: <reason>
3D upload OK | ...
Asset load failed: <selected-name>
World descriptor rejected
```

Loader errors can include:

```text
Invalid mod asset path
Cannot resolve mod directory
Cannot resolve mod asset: <path>
Mod asset escapes directory: <path>
Mod asset is not a readable file: <path>
Mod asset size rejected: <path>
Mod asset changed or could not be read: <path>
```

Texture diagnostics are also logged under `DH2Native`; examples include
`Texture input outside size limit`, `Texture exceeds GPU limit`, and
`RGBA upload OK`. Parser and initialization failures generally include the
resource path or owning world/model failure reason in Logcat.

To restore the packaged baseline, delete the override, force-stop the app, and
launch it again. The development smoke tool also tests baseline restoration
and preserves any override that existed before the run. A debug-only world
reload broadcast exists, but data/script owners may be retained without
preparation or `OnInit` replay. It is not a safe general-purpose hot-reload
mechanism. There is no in-app mod-pack picker, lifecycle/version system,
validation tool for all fan content, or full skill/script API yet.

## Development emulator smoke

Build the debug APK, then create a first spawn mod without editing binary
offsets:

```powershell
python tools/build_spawn_mod.py --apk app/build/outputs/apk/debug/app-debug.apk --shift-x 100 --output app/build/example-mod
```

Install it on the development emulator and restart the app:

```powershell
adb -s emulator-5558 shell mkdir -p /sdcard/Android/data/com.example.dh2/files/mods/worlds
adb -s emulator-5558 push app/build/example-mod/worlds/crypt01.dwld /sdcard/Android/data/com.example.dh2/files/mods/worlds/crypt01.dwld
adb -s emulator-5558 shell am force-stop com.example.dh2
adb -s emulator-5558 shell am start -n com.example.dh2/.MainActivity
```

The committed device smoke check changes the spawn by 100 game units, verifies
native world initialization, rejects a malformed descriptor, and restores
baseline loading. It preserves any pre-existing override file.
