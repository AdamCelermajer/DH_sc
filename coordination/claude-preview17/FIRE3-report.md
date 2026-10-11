# FIRE3 report (Preview 17): red door wall of fire (`_prim_Swamp2Door_FIRE`)

Branch `p17/fire3` (worktree `DH_wt/p17fire3`), started from `p17/fix` (`a78a882f`). Not pushed.

## Investigation

Evidence folder (not committed): `DH_sc/.local-inputs/claude-preview17/fire3/` (`bres.py`, `anim.py`, `keys.py` decoders; `base/` baseline run; `ref/` reference frames).

### Decoded model `go_door_fire_red.bdae` (rc3, BRES v. library tables, offsets as in `engine-resources/resources.cpp`)
- Libraries: 7 animations, 4 animation clips, 3 materials, 3 effects, 2 images, 2 geometries (`_colbox_fire-mesh`, `_mesh_fire-mesh`), 1 controller (skin), 1 emitter (`GL_PCloud01-emitter`), no cameras/lights.
- Clips (ms): `close` 8000-9333, `idleclosed` 0-1000, `idleopened` 5333-6666, `open` 2666-4000.
- Flame mesh `_mesh_fire-mesh`: 63 vertices, 1 primitive (60 indices). Bind bounds x -414..-106, y -763..753, z -138..538. The sheet is a vertical wall (y along the door, z up to about 540). It is not a floor band.
- Skin: one controller, joints `Bone1..4`/`_bone_01..03` (scene nodes).
- Animation channels (types per `engine-animation/channel_filter.cpp` and `authored_fx_transform_v5.cpp`): type 1 translation (3 comps) on `_bone_01/_02` nodes, types 2/4 single-axis translations on `_bone_03`, `_bone_collisionbox`, `_bone_fx_doorfire`. Type 28 emitter birth rate. Type 86 diffuse colour (uchar4) on `Standard_20`. **No scale channels** (types 10-13 absent), so the wall height is not a bone-scale animation.
- Bone_01 translation repeats a 1 s pattern (206.9 -> 130.2 -> 206.9 units). The bone_fx_doorfire z track is 0 until 3000 ms and then falls in steps.
- Colour (Standard_20 diffuse, linear, uchar4): black at 0 ms, orange (243,47,1) by 133 ms, held to 400 ms, fades to black at 667 ms, black until 1000 ms. So in the 1 s idle loop the flame pulses and is black for the last third.
- Emitter (`GL_PCloud01-emitter`, record at +0x20..+0x80, layout from `particle_resource_init_v32.cpp`): BirthRate base 15/s, `Life` 1.667 s, `LifeVariation` 0, `TargetSize` 500, `SizeGrowthTime` 0.067, `SizeFadeTime` 1.267, `Speed` 300 (`SpeedVariation` 0.5), direction (0,0,1) (up), `AnimLength` 0.667, `SpinTime` 11.667.
- Birth-rate track (step): 15/s flat over the idle loop; burst to 150/s around 2.83 s (open clip window 2.67-4.0 s); a second burst 30 -> 150 -> 15 around 8.3-8.6 s (close clip window).
- Textures: `FX_smoke_03.tga`, `fx_doorfire.tga`. Materials: `ColorMaterial_00E59BB8` (particles), `Standard_20` (flames), `Standard_202`.

### Reference (local Part 1 video, 640x360)
- Frames at 610, 614, 618, 622 s (`fire3/ref/ref-sheet-4.png`): 610 is HUD with no cutscene. At 614 the Celeste cutscene is running and the orange fire is already at full brightness (a large orange mass with particles rising around the bone pile). 618 and 622 look the same. **The 10 s ramp is not visible in this local video**: the fire is already bright when the camera reaches it. The brief's ramp claim is not supported by these frames; the earlier "10 s" evidence needs a different source.
- The reference camera is the cutscene pan, so it is not a same-frame match with a gameplay camera.

### Current runtime (baseline `build-p17fix`, `fire3/base/`)
- Frame 240 (`base-240.png`): the flame is a flat red-orange band on the floor from the bone pile to the lower-left. It is not a wall. The bind mesh is 676 units tall, so the flat result means the vertical extent is lost in the FX path (skinning or placement), not in the authored data.
- The composite used by the embedded path is `CharacterAuthoredResourceFactoryV32` -> `AuthoredFxMeshGraphV32` (`runtime_effects_factory_v1.cpp` line 25).
- Mesh path: skinned receivers use palette = bone world * inverse bind * bind shape (`engine-skinning/skinning.cpp`), then world = outer. Bone world matrices are rebuilt in `AuthoredFxTransformOwnerV5::sample`.

### Runtime skin diagnostic (`DH_FX_DEBUG=1`, build `p17fire3` at `cb89b539`, run `fire3/dbg/`)
- The skinned flame mesh (node 1, 63 vertices, 4 joints) has bind (undeformed) bounds x 21..818, y -752..752, z -139..529 as read from the receiver geometry.
- After `skinning::palette`/`positions` the deformed bounds are x 177..1037, y -716..808, **z 259..967**. The whole sheet is lifted about 400 units and its base floats at z 259 (door origin z 74). The sheet is no longer on the floor and also not tall in the expected place.
- The palette offset is the cause: each joint's palette is `world * inverse_bind * bind_shape` with the animated bone world. Bone_01 animated translation is (206, 146, 904), which is absolute in the clip. If the inverse bind assumes a different rest pose, the offset appears. The joint-level breakdown is being measured (`FXJOINT` lines, build `p17fire3` after `joint diagnostic` commit).

## Implementation
- (pending)

## Isolated tests
- (pending)

## Integrated runtime verification (real EXE, quiet_run, rc3 package)
1. Red door same-frame vs reference (`ref-wall-of-fire-0759.png`, Part 1 about 610-624 s): (pending)
2. Ramp sequence (4 frames): (pending)
3. Contact sheet: (pending)
4. Frame cost `DH_PERF=1`: (pending)
5. ctest (`-j 6`, llvm-mingw on PATH; expect only `session_skill_binding` to fail): (pending)

## Gaps
- (pending)

## Placeholders
- (pending)

## Package files required
- rc3 package (`windows-source-clock-v19-preview-15-rc3`).

## Verifier script
- (pending)
