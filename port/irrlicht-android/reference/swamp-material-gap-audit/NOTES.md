# SWAMP `Standard_19` floor-band audit

## Finding

The white bands are the module's navigation meshes being included in the
diagnostic scenery draw list. They are not evidence that the source floor
material should be changed to black.

The two module-zero floor draws are `_floor_obj_4of4_brdwalk_sw_-node`
(`Standard_19`, 198 indices) and
`_floor_water_obj_4of4_brdwalk_sw_-node` (`Standard_19`, 99 indices). Both
serialized BRES nodes have `visible=1` and a type-3 geometry instance. Their
user properties identify their gameplay role: `floortypes = %22wood%22` and
`floortypes = %22water%22`. Their transforms and floor type records are needed
by navigation/collision; the visible bit by itself does not decide whether
`PFWorld` submits them as scenery.

In the original-backed room assembly, `PFWorld::LoadRoom`'s recovered prefix
table includes `floor` and `_exit_` (plus `minimap`; see
[`level-world/README.md`](../../../level-world/README.md#L159)). The live
reconstruction in [`world.cpp`](../../../level-world/world.cpp#L37) sends each
`/_floor_` instance to the floor/navigation builder and erases it from the
scene instance list. It also erases the room-root bounds and `_exit_` meshes
before the scenery list is rendered ([lines 38–40](../../../level-world/world.cpp#L38)).
The SWAMP smoke instead assembles the Collada subtree using serialized node
visibility and draws those floor meshes as ordinary `EMT_SOLID` buffers. That
is why the diagnostic view shows large white polygons where the native room
path keeps navigation geometry out of the visual scenery set.

This means “collision/navigation-only in the room renderer” is accurate, but
“authored hidden node” is not: the BRES visibility words are true. The
navigation bridge must continue to receive those source meshes even when the
scenery render adapter filters them.

## Material and shader evidence

The checked source material record `Standard_19` has no material-level
parameters and resolves to local effect
`ProfileCOMMON_Standard_19-fx1302961842_swamp`. The effect record contains
`default`, `fog`, `lighting`, and `fog+lighting` named variants, no image
bindings, and serialized ambient/diffuse `(0,0,0,1)`, specular `(0,0,0,0)`,
emission `(0,0,0,1)`. The source SWAMP BRES has zero Light-library records.
These are BRES values, not proof that a particular original game shader pass
was selected or executed. The exact customized `glitch::` effect manager and
its rendered output are not in this diagnostic slice, so the effect's
no-light `default` pass color cannot be asserted from the serialized records
alone.

The current Irrlicht path does explain the white output. The scene adapter
creates vertices with opaque white color and disables material lighting
([`scene_mesh_adapter.cpp`](../../game/scene_mesh_adapter.cpp#L124)). The
Irrlicht GLES2 solid vertex shader starts with that vertex color and only
applies material ambient/diffuse/emissive values inside `if (uLightCount > 0)`
([`COGLES2Solid.vsh`](../../upstream/media/Shaders/COGLES2Solid.vsh#L102)).
Its callback computes light count from the material lighting flag and active
driver lights ([`COGLES2FixedPipelineRenderer.cpp`](../../upstream/source/Irrlicht/COGLES2FixedPipelineRenderer.cpp#L98)); this SWAMP path disables lighting and adds no lights. Consequently its
material DiffuseColor assignment does not turn these untextured buffers black:
the shader remains on the white vertex-color path. A new diffuse-to-vertex
projection would therefore be an unverified visual approximation here, not an
evidence-backed repair for the band.

## Implemented source-role filter

The Irrlicht adapter now has an opt-in `build_source_room_scenery_mesh` entry
point. It filters draws whose `node_record` equals the exact bound
`ModuleBinding.node_record`, plus IDs with `_floor_` or `_exit_` as a complete
node component at string start or after `/`. It does not use material IDs,
arbitrary name substrings, or a guessed root label. `build_mesh` retains its
generic all-visible behavior. The module-zero caller passes the binding's
record and skips the same classified records when mapping per-buffer source
materials. The source `SceneMesh`, floor bridge, navigation surfaces and
physics inputs are not edited.

The actual nine selected SWAMP source module roots produce these results:

| Source module | Visible draws | Scenery | Root bounds | Floor draws | Floor triangles | Exit markers |
| --- | ---: | ---: | ---: | ---: | ---: | ---: |
| `obj_4of4_brdwalk_sw_00_0` | 54 | 49 | 1 | 2 | 99 | 2 |
| `obj_3of4_brdwalk_sw_00_1` | 55 | 51 | 1 | 1 | 43 | 2 |
| `obj_1of4_brdwalk_nse_00_2` | 46 | 41 | 1 | 1 | 53 | 3 |
| `corner_ruin_ws_00_3` | 59 | 55 | 1 | 1 | 96 | 2 |
| `merchantcamp_ruins_swe_00_4` | 62 | 55 | 1 | 2 | 105 | 4 |
| `corner_brdwalk_se_00_5` | 52 | 48 | 1 | 1 | 53 | 2 |
| `deadend_brdwalk_w_00_7` | 43 | 39 | 1 | 2 | 35 | 1 |
| `bossroom_ruins_ns__8` | 9 | 3 | 1 | 4 | 60 | 1 |
| `obj_2of4_brdwalk_sw_00_9` | 56 | 51 | 1 | 2 | 82 | 2 |
| **Total** | **436** | **392** | **9** | **16** | **626** | **19** |

The adapter host gate also rejects no legitimate name based on incidental
substrings such as `floorboards`, `decor_floor_lamp`, `bridge_floor_trim`,
`my_exit_sign`, or `decor_exit_arch`. It verifies malformed nonterminated IDs
do not accidentally match a truncated role token. On module zero, the two
filtered floor draws contain exactly 99 triangles. An independent rebuild of
the source navigation/floor bridge still imports those same two surfaces and
all 99 triangles; its boardwalk and water IDs/flags/heights, water mask `0x2`,
mask-zero rejection, and outside rejection all pass. The navigation snapshot
hash remains `8e00e4a415552f8160137ab2f4c33c71b4d0e0da5b1e63185e1985e60340f66e`.

The root agent subsequently rebuilt and tested the wired renderer. APK
`c7094b5356cbc8f15af458112c2473a9925387024e35dd13c5d4f7d62d40431e`
(72,063,674 bytes) passes Android 17/API37/x86_64 with 16 KiB pages and an
installed-byte match. The runtime reports 49 scenery draws, one filtered room
root, two navigation floors and two exits, while preserving 10,816 source
vertices / 13,284 indices and source navigation boundary sliding. Walk/Idle,
Stop/Pin, and same-process HOME/resume checks pass. Manual inspection of Idle,
held boundary movement and resume screenshots confirms the former white
navigation polygons are absent. Dark material patches and exact custom shader
behavior remain unresolved. See the separate root build/runtime evidence in
`port/android-app/build/irrlicht-swamp-room-roles-runtime/`.

This later root result is distinct from the original host/source-record audit.
Neither result claims complete scene or game parity.

Do not set the `Standard_19` diffuse color to black and do not invent texture or
lightmap bindings. A separate future “show navigation mesh” diagnostic may
render the floors deliberately with an explicit debug material, but it must
be labeled as a visualization and must not be confused with the game room's
scenery pass.

## Scope and reproduction

This is a source-record and renderer-path audit, not an APK rebuild, device
test, original-GPU comparison, or proof of full-game parity. The exact
source-scene hash is
`89da80c60a7ebecd0e8a27a9d46f2625e3a5ec112933e5aa7cab251412b8364d` for
`data/3d/modules/swamp/swamp.bdae`.

Evidence in this directory:

- [`module-zero-draw-dump.txt`](module-zero-draw-dump.txt) records all source
  draw descriptors and confirms the two untextured `Standard_19` bands.
- [`serialized-material-records.json`](serialized-material-records.json)
  records the local ProfileCOMMON variants/material values and source hashes.
- [`source-floor-node-visibility.json`](source-floor-node-visibility.json)
  records all 29 floor-named BRES nodes and their `visible` words; all are 1.
  It captures the two selected module-zero floors and exact user-property text.
- [`inspect_floor_source_nodes.py`](inspect_floor_source_nodes.py) reproduces
  the node visibility/property dump using the checked scene-payload reader and
  the supplied local cache.
- [Room-role host report](../../build/swamp-room-roles-host/validation.json)
  records all nine module draw counts, role categories, source hashes and the
  cross-check against the unchanged 99-triangle module-zero floor bridge.

Commands used from the repository root:

```powershell
python port/scene-payloads/build.py --output port/irrlicht-android/build/swamp-material-gap-audit/scene-payloads --report port/irrlicht-android/build/swamp-material-gap-audit/scene-payloads/build-validation.json
python port/irrlicht-android/reference/swamp-material-gap-audit/inspect_floor_source_nodes.py --repo . --cache ..\cache\files --library port\irrlicht-android\build\swamp-material-gap-audit\scene-payloads\libdh2_scene_host.dll --output port\irrlicht-android\reference\swamp-material-gap-audit\source-floor-node-visibility.json
python port/irrlicht-android/game/tests/run_swamp_room_roles_host.py --cache ..\cache\files --output port\irrlicht-android\build\swamp-room-roles-host
python port/irrlicht-android/game/tests/compile_android.py
```
