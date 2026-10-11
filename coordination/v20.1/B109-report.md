# B109 (and B107): Bogwitch Cavern exit area and the cavern render wrongly

Status: root cause found for the swamp exit area (not yet fixed). Cave-level look not yet diagnosed. Sessions: Sonnet, then Fable, then Opus 5.5. All three were cut off by usage limits. HANDOFF below.

## Resumed investigation, before implementation (2026-10-11)

- Visual evidence re-inspected: P1 sequence `e_sheet.jpg` (700–715 s) and `t_sheet.jpg` (680–760 s), with the original exit arch to the upper-right of the blood, travel at 716 s, and cave entrance at 733 s. Compared preserved runtime `runA/f190.png` (arch engulfs hero) and `runC/f650.png` (cave wall texture looks smeared). Rotation diagnosis is supported by the exit sequence; cave cause remains unknown.
- IDA confirms the matrix convention directly: `VisualObject::SetRotation` 0x472874 converts Y,-X,-Z and only writes an existing scene node when the quaternion changed, then recalculates/applies its mesh box. `SyncRotation` 0x472948 reads GameObject +364; `GameObject::SetRotation` 0x3938a0 stores the rotation and synchronizes when a visual exists and virtual +112 admits it. `VisualObject::Sync` 0x38ba74 synchronizes position/rotation/scaling. `ISceneNode::setRotation` 0x5970f4 stores quaternion +184 and sets dirty bit 4; `getRelativeTransformation` 0x598908 on dirty bits 2/4 calls `quaternion::getMatrix_transposed` 0x5602d0, post-scales, writes translation, clears dirty bits. Direct SetRotation calls also come from `AnimatedFX::SyncIrrData` 0x492b10, gated by an existing FX visual and rotation-follow/explicit-rotation flags; they use the same scene-node path.
- Port consumers audited: actor profile scale preserves placement columns (`actor_population.cpp:15`); containers, doors, gated decor and scripted decor retain definition placement; ambient decor decodes with it; `campaign_host/trigger_zones.cpp:35,139` uses normalized placement columns for oriented boxes. No compensating sign conversion found in these consumers. Existing trigger tests cover axis-aligned/-90 boxes but cannot detect inverse arbitrary angles.
- Expected minimal reusable implementation: one authored Euler TRS helper using the same transposed quaternion convention as BDAE nodes, called by both XML loaders. Explicit matrix attributes retain precedence. Level geometry previews retain static decor; gameplay defers decor to its runtime owners so the same declaration is never both merged and submitted as ambient geometry. Ambient excludes conditional/script-owned/hidden declarations. No asset-name or map-specific correction.
- Focused tests defined before coding: module/decor and actor rotations on all axes with nonuniform scale; arbitrary rotated trigger inclusion/exclusion (inverse-angle point rejected); container/NPC facing matrices; static-preview vs runtime-owned level decor counts; gated/hidden decor excluded from ambient. Hidden production EXE exit frames at the blood and exit trigger, then travel into cavern and capture the cave walls. Compare reference layout, record EXE SHA, run focused tests and full ctest.
- Uncertainties: no claim yet that cave smearing is fixed; original film is v1.0.3 and assets are Android export. Module-local actor expansion currently adds only translation (pre-existing behavior) and is outside the inverse-quaternion defect; do not silently change it without separate source evidence. Long unchanged cinematics will not be rerun.

## HANDOFF

### Additional cave investigation (before the material fix)

The P1 733–739 s cave sequence shows broad caustic cells on the entrance floor, while `sol-20261011/first-cave/f200.png` shows dense cells. Diagnostic `scene_probe --params` proves the cave water `Standard_2` material has no matrix override, but its local effect `ProfileCOMMON_Standard_2-fx1302961542_swamp_cave_watercaustics` supplies `diffuse-sampler-matrix = diag(0.25,0.25,1,1)`. The current loader returns identity and UV 0..1, displaying the complete 4×4 atlas. This is a general missing effect-default consumer, not a cave-specific scale rule. The wall atlas itself (`sol-20261011/env_swamp.png`) decodes cleanly; wall smearing is not yet diagnosed as a separate defect.

Source evidence: `CColladaFactory::createMaterial` 0x632410 constructs the effect renderer before `createMaterial` 0x631ce8 applies explicit material parameters. `createMaterialRendererForProfile` 0x6361e8 installs reflected effect defaults via `setMaterialParameter<SNewParam>` 0x6324c4; its matrix branch 0x6326e8–0x6327b0 copies BRES matrix values, skips identity installation and retains nonidentity defaults. Existing recovered handoffs `port/scene-materials/reference/material-matrix-v4-integration.md` and `effect-material-gles-v4-integration-validation.md` independently verify that consumer and actual BRES effect-default matrices. The generic preview currently reads only material-level `texture-matrix`. Expected fix: initialize the diffuse UV matrix from a resolved local effect default, then apply explicit material overrides; preserve external-effect behavior and explicit identity overrides. Test before coding: real caustic default 0.25; explicit material override wins; invalid matrix fails transactionally; hidden production cave sequence shows atlas-cell scale, exit layout and actor/chest/trigger regressions retained.

### Verified (observed in real EXE renders, hidden quiet runs, isolated saves)
- **The giant stretched/tilted stone blocks in shot-23/shot-24 come from ONE decor instance:** `_anim_caveentrance_troll__001`. It is declared in `data/3d/modules/swamp/mvp/deadend_brdwalk_w_00.mvp` (AnimatedDecor, dae `data/3D/AnimatedDecors/swamp_cave_entrance_troll.bdae`, xref `_anim_caveentrance_troll_`, position `1154.75,1055.82,0`, rotation `0,0,-47.5383`, no activate condition). The module is `deadend_brdwalk_w_00_7` at `-6000,18000,0`.
  - Bisection used the diagnostic env var `DH_LEVEL_SKIP` (added in `assembled_level.cpp`) together with `--no-ambient-decor`. Frames are in `.local-inputs/claude-v20.1/fix109/`:
    - `s3_sheet.png`: base / no ambient decor / no level decor / decor only.
    - `s4_sheet.png`: everything except the troll entrance / troll entrance only.
  - The blocks disappear when only that decor is skipped, and they remain when only that decor is drawn.
- The mesh itself decodes sanely: a static batch with pos, normal, ubyte4 colour (real baked colours, not packed indices) and uv. It has 939 vertices, bounds about 1650 x 1835 x 1170, and a child `_mesh_big_blood` (a large blood decal) at local `426,-498,143.5`. There is no controller and no node animation (`animations=0`, 1 animation_clip).
- **Video P1 (around 708-716 s; frames in `fix109/vid2/e_*.jpg`, `fix109/e_sheet.jpg`, `fix109/t_sheet.jpg` = 680-760 s at 1 fps):** the hero walks from the pond (screen left) onto a big diagonal blood decal. The stone archway with a dark cave mouth and rubble stands to the RIGHT / upper right of the blood. The "Bogwitch Cavern / World Map" menu appears while the hero stands on the blood (about 715.5 s). Loading starts at about 716 s, and the cave arrives at about 733 s ("Bogwitch Cavern - Entrance").
- In our build the stone mass sits WEST / north-west of the blood and envelops the area where the hero stands (runA `fix109/runA/f190.png`, hero at `-4190,19034` = blood under the current rotation). This does **not match** the video.
- **Double draw (separate defect, verified from logs):** every Decor/AnimatedDecor is drawn twice. P17 PROCGEN2 merges it into the level mesh (`assembled_level.cpp`, "Decor instantiated: 38"), and B076 `AmbientDecorRuntimeV1` draws it again ("ambient AnimatedDecor instances=38"). The placements are identical, so the copies overlap: z-fighting, and additive layers become twice as bright. One of the two paths must be dropped. Ambient keeps the animated UV/colour, so the likely choice is to skip decor in `load_level_with_module_zones` when the ambient runtime is active (or the reverse). Check that procedural levels keep their decor.

### Root cause (inferred from code; the IDA cross-check is still open)
The authored Euler-to-quaternion conversion is correct (IDA `VisualObject::SetRotation` 0x472874, ARM args Y,-X,-Z; see `port/level-loader/.../visual_transform_v1.cpp`). The quaternion-to-matrix step is the **non-transposed** form in our code:
- `port/windows-foundation/level_manifest.cpp` `placement()`: m[1]=2xy+2zw.
- `port/windows-foundation/actor_definitions.cpp` `transform()`: the same inline matrix.

Original BDAE node TRS goes through `dh2_node_matrix` -> `dh2_quat_matrix_transposed` (`port/scene-materials/scene.cpp:223`, `port/engine-math/math.cpp:184`; `dh2_quat_matrix_value` = transposed). That is the convention which renders character and module nodes correctly. So every rotated level/mvp/mgp declaration is currently rotated by the **inverse** angle. The swamp modules all have rotation 0, which is why this went unnoticed. Decor is the first rotated content made visible (P17 PROCGEN2 / B076).

With the transposed (original) convention, the troll entrance turns 47.5° the other way:
- The big blood lands at about `-4924,18405`. That is exactly the "runB" position Fable chose; `fix109/runB/f190.png` was rendered with the old rotation.
- The stone mass lies north / north-east of the blood, which is consistent with the video framing.
- The exit zone centre `-4588,18915` (z 143.5, the same height as the blood node) lies between the two candidates, so it does not decide the question.

### Hypotheses ranked
1. (Most likely, see above.) Use `dh2_node_matrix(out, position, q, scale)`, i.e. the transposed quaternion matrix, for authored placements in `level_manifest.cpp::placement()`. Probably also `actor_definitions.cpp::transform()`; do it in one shared helper, with no per-level hack.
   - Before changing `actor_definitions`: check every consumer of `ActorDefinition::placement` (actor yaw/facing, oriented zone boxes, spawn points, chests, NPC facing). Some may have compensated empirically, so re-test NPC/chest facing and zone triggers in the swamp.
   - Cross-check in IDA that `VisualObject::SetRotation` (0x472874) stores a quaternion that the glitch scene node turns into its matrix through the same path as BDAE node rotation. Look at the callees after 0x4728a4: ISceneNode setRotation and getRelativeTransformation.
2. The decor root's authored TRS should be composed with, not replaced by, the declaration TRS. Both are identity for this asset, so this does not explain B109.
3. The camera yaw differs from the original. Less likely: the pond and walking direction match the video.

### Not verified / remaining steps
1. Apply hypothesis 1 (shared helper). Add a unit test: placement of rotation `0,0,90` must equal `dh2_node_matrix` of the converted quaternion, and +X must map to the original's direction.
2. Rebuild. Render the exit area at hero `-4924,18405` and `-4588,18915` (args: `fix109/runB/runB.args`, change `--position`), then A/B against video frame `vid2/e_15.jpg` / `fix109/vid/ent_011.jpg`. Say "match" only if the archway/rubble/blood layout really matches.
3. Fix the decor double draw (above).
4. Cave (002 witch cave, procedural, modules from `swamp_cave.bdae`): the walls look smeared/stretched compared with the video (`fix109/cave_ours_crop.png` vs `cave_vid_crop.png`, `runC/f650.png` vs `vid/cave_005.jpg`, video 733-739 s).
   - Not diagnosed. Re-check after fix 1: the cave mvps have rotated decor too (B106 says it arrives in the bog-cobble area).
   - Then inspect the materials with `SCENE_PROBE_MATERIALS=1 scene_probe swamp_cave.bdae _module_<name>-node`. Wall range: `env_swamp.tga` (PVRTC 1024², stored with a .tga name), uv 0.32..1 x 0..0.5, identity texture matrix.
5. Full ctest; freeze the EXE and record its SHA256; write before/after frames into this report.
6. Leftover open items from the release notes: `swamp_watercaustics.bdae` has no node `_mesh_animwater_deadend_brdwalk_w__nobatch` (the original handles "nobatch" in `batch::BatchNodeCompiler::MakeNoBatchVisible` 0x50cfa8). The 'Scene has no visible geometry' notices are particle-only dust-mote roots (expected; B105 territory).

### Files touched (branch fix/b109)
- `port/windows-foundation/tests/scene_probe.cpp`: `--nodes/--anims/--dump/--plan` diagnostics (Fable), plus `SCENE_PROBE_MATERIALS` (per-range material/uv) and `SCENE_PROBE_COLORS` (vertex-colour sample).
- `port/windows-foundation/assembled_level.cpp`: diagnostic `DH_LEVEL_SKIP` env var (comma substrings of module/decor declaration names; `@modules`, `@decor`). Inert when unset. Remove it or keep it as a documented diagnostic.
- `port/windows-foundation/main.cpp`: exit-zone log prints the zone centre (Fable).

### How to build / run
- Build: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name fix109 -Jobs 2` (output in DH_wt/build-fix109).
- Fixtures, in `.local-inputs/claude-v20.1/fix109/tools/`:
  - `mkjob.py <base.args> <ABSOLUTE outdir> <name> <frames> <captureFrames> [pos=x,y,z] [extra args]` builds args and `jobs.json`. Absolute paths are required, otherwise CreateProcess error 267.
  - `run.sh <ABSOLUTE outdir>` runs `quiet_run.ps1` (hidden, silent) and converts ppm to png.
  - Env vars apply per `run.sh` invocation, so use one outdir per env setting.
  - Example: `DH_LEVEL_SKIP=caveentrance_troll bash tools/run.sh $R/s4a`.
- Cave run (E travel): `fix109/runC/runC.args` (`--transition-key 60`, `--player-to _prim_ExitLevelZone_42@400`).
- Plan view: `scene_probe --plan out.ppm x0 y0 x1 y1 px --level <levels-root> data/scene/001_swamp.mlx`. Grey = opaque, red = transparent, grid every 500 units.

### Traps
- Disk is at 98%: keep scratch PNGs small (`run.sh` scales to 800 px and deletes the ppm).
- The Bash tool mangles backslashes in heredocs: write Python helpers with the Write tool.
- `quiet_run` inherits env from the caller.
- Do not kill processes by name.
