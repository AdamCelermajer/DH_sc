# B109 (and B107): Bogwitch Cavern exit area and the cavern render wrongly

Status: three general fixes implemented and tested; B109 remains OPEN pending independent visual review of remaining arch shape/scale and wall appearance differences. Worker `/root/b109_cavern` retains follow-up ownership; root assigns the fresh reviewer. Historical HANDOFF below is superseded by this verification section.

## Implemented and verified checkpoint (2026-10-12)

### Review repair investigation, before coding

Independent review `review-sol-20261012/REVIEW.md` reproduced a material-transport regression: removing baked decor leaves ambient's diffuse-only binding, with `Material::specularTexture=0`. The real entrance opaque Material__11610 has380 triangles, Sp enabled, Specular `q:/data/iphone/3d/textures/env_swamp_spec.tga`, Glossiness10. The decoder preserves these values; Renderer::draw only runs its existing opaque specular consumer when the bound texture is nonzero. Visual source sequence P1 700–715 / cave733–739 was reused and re-inspected by the reviewer; highlights differ, but no silhouette root cause is inferred from them.

Original exported GL_Diffuse_L1_iPhone_FS.glsl samples **both alpha and specular from blue (`.z`)** for its Al/At/Sp variants; VS calculates attenuation and half-vector specular, FS adds that term. Current baked and ambient alpha bindings also restrict blue to one exact non-Sp technique or force red. Expected reusable repair: one shared material-texture transport for baked level and ambient owners, using source shader-family alpha policy, source specular metadata, and staged publication so a thrown lookup leaves previous bindings intact. Keep the existing opaque renderer pass; alpha-Sp rendering remains a disclosed pre-existing gap. No brightness/scale change.

Focused test defined before implementation: load the real entrance in AmbientDecorRuntime, assert authored opaque Sp metadata/flags and nonzero specular binding, verify all L1 Al/At variants request blue alpha, preserve generic red-alpha behavior, no-Sp/missing-map/zero-binding cases stay0, thrown lookup preserves previous binding, and geometry count remains one instance. Rebuild/freeze, repeat only the short production entrance fixture against the retained one-owner EXE and inspect frames plus binding counts. B109 silhouette/scale/wall appearance remain OPEN independently.

### Review repair verified (supersedes material-transport readiness at 51b5a774)

Implemented `8b1eb74c`: shared `original_material_texture_binding.hpp` transports diffuse/alpha plus source specular map/glossiness for both baked level and ambient owners. The original L1 shader's blue alpha policy applies to Al/At/Sp variants; generic masks retain existing red policy. Texture lookups stage both IDs before publication; a thrown second lookup preserves previous material bindings. No renderer brightness, geometry, angle, scale or FOV change. Source alpha-Sp additive specular pass limitation remains unchanged.

Build exit0, focused6/6 pass, full ctest167/168 with only the same permitted session_skill_binding failure. Real entrance regression loads one instance, proves opaque380-triangle Sp flags and Glossiness10, receives nonzero specular binding, and checks alpha-Sp transport without claiming its renderer pass; no-Sp, generic mask, missing texture, and thrown-lookup branches pass. Full log `sol-20261011/ctest-specular.log`.

New frozen `sol-20261011/specular.exe` SHA256 **358608F1612C09E9098F03336DE7B12301210C1C6BC5C19793CA930731F0AB1E**. A/B uses retained11322C25... one-owner EXE; both sides retain rotation/UV fixes and the same source-progress fixture. `runtime-specular/jobs.json`/`summary.json`: four short hidden jobs (exit150frames, cave350frames), all exit0. Inspected exitf100/f140 and cavef140/f200/f300. Log proves decor0/ambient37 at exit, 107 ambient ranges with14 specular bindings including8 opaque Sp; cave decor0/ambient15,17 ranges with7 specular bindings including1 opaque Sp. Source PlayerLight is active. No missing/unsupported texture error was logged.

Frames `specular-exit-ab.png`, `specular-cave-ab.png` preserve the one-owner structure while changing source material/highlight pixels. Deterministic PPM comparison `runtime-specular/pixel-difference.json`: exitf100 changes226766 of2594160 channels, maxdelta96, meanabsolute0.407; cavef200 changes120220 channels, maxdelta9, meanabsolute0.061. These prove the binding change reaches rendered pixels, **not** that silhouette/scale or all lighting matches the film. The review's concrete missing specular transport is repaired and ready for re-review. Overall B109/B107 remains OPEN for the already-recorded visual gaps. Accepted v20 EXE hash rechecked unchanged.

Code commits: `24873fb8` (shared authored TRS and render ownership), `64afbfc1` (local effect UV defaults and regressions), `772712fb` (isolated ownership fixture). No merge/push. One main wiring hunk passes `LevelLoadOptions{includeStaticDecor=false}` when gameplay has its decor runtime; previews retain static decor. Ambient excludes hidden, script-owned and conditional declarations (`Invalid` = no-condition sentinel). Local effect defaults initialize the diffuse UV matrix before explicit material overrides. No model/map-specific correction.

Frozen EXE `.local-inputs/claude-v20.1/fix109/sol-20261011/final.exe`, SHA256 **11322C25B92F37E99222C1E07E53ADEF813364B07ABE94B3327D2F1F574D2AFD**. Rotation-only `rotation.exe` SHA256 161112BCF64D8D2C77C794FBD452C4A3A93F9B0E6FFF35F62A9AB8B8DE33A3C2 retained. A/B accepted v20 EXE SHA256 **8BA1E537D33D4D4FD5B7DD9822C4CAB5080917FE2427A3903AB7C247827E8CCE** rechecked. Troll-entrance asset equals Android source export: SHA256 937ED6FCC95182AEF204791E90FFE2A3CDB35DB884A997B02E0427A8E994B70C.

### Isolated tests

`p14_build.ps1 -Name fix109 -Jobs 2`: exit0. Focused `level_manifest`, `actor_definitions`, `campaign_trigger_zones`, `ambient_decor`, `container_open_script_v1`, `level_module_zones`: **6/6 pass**. Axis signs/nonuniform scale, actor+35/chest-60 facing, arbitrary+35 trigger admission/rejection, geometry ownership once, hidden/gated/script exclusions, 0.25 atlas default, explicit identity override and malformed-override atomic rejection are covered. An initial fixture wrongly expected virtual-only paths to resolve through the filesystem content resolver; corrected to its own temp directory, with no accepted-asset writes.

Full ctest **167/168 passed**, sole known worktree `session_skill_binding` failure = `Asset path escapes asset root`. Log `sol-20261011/ctest-final.log`. No other failure.

### Actual EXE/render verification

All jobs: quiet_run hidden desktop, DH_AUDIO_SILENT=1, isolated scratch profile/game saves and cwd. All exited0. Normal production arguments derive from v20 `swamp.args`, using read-only absolute asset/audio paths. `runtime-source/jobs.json`/`summary.json` record 9 jobs. Existing B068 source CQPG progress fixture (all rows13 except Act1 rows8/50=6, active index-1) prevents opening autoplay while preserving native physics, compiled campaign bank, source camera and renderer; each job records this limitation. No condition overrides/force-starts. `runtime-final` first attempt started Swamp_Intro, so those captures are **excluded** as exit/cave/NPC proof.

- Exit A/B, same position(-4757,18680,260), f100/f140: `exit-ab.png` and `runtime-source/{before,after}-exit/f100.png`. Arch now upper-right of blood rather than inverse/surrounding. Level decor38->0; ambient38->37 (conditional decor belongs to gated owner). **Matches video** for arch/blood/pond ordering; **matches IDA** for quaternion/node matrix convention. Full shape/scale/color fidelity **not verified**.
- Travel f60 -> real procedural cave sessionf61 -> entry0 `_prim_EntryPoint` (203.611,557.532,37.0598). f140/f200/f300 inspected: `cave-ab.png` shows full4x4 atlas/dense cells replaced by one authored cell/broad caustics, animated across the sequence. Cave decor0/ambient15, no duplicate submission. **Matches video** for caustic scale/entrance layout; **matches IDA** for effect-default/override order. No separate wall fix claimed.
- Angled chest: `runtime-source/{before,after}-chest/f50.png,f90.png`, hero(1600,1500,100). `_prim_OpenableContainer_1` z=-10.9727 stays in place with corrected facing. Normal physics/enemy gameplay stayed live. Container-open behavior unchanged/not retested.
- Merchant NPC first normal run started Swamp2_Intro and is excluded. Separate `jobs-npc-static.json`/`summary-npc-static.json` 2 hidden jobs suppress only `--campaign-triggers` to inspect idle rendering: f50/f90 show `_prim_NPC_04` at the stall (authored z36.4548), side/back->front matches source matrix. **IDA placement verified; video idle-facing equivalence not verified** because film can include LookActor. Talk/shop/campaign outside this test.
- Affected lizard short regression: original trigger(-2897.05,-3.3969,256.739), frames30/60/120/180/240/300/400 inspected against P1 224â€“226 s `fix068/vid/liz_sheet1.png`. Both enter from wall side and land on platform; unlockf125, DoTutorialf126. Explicit +pi rig-facing correction in combat_session.cpp **retained**. No long unchanged cinematic rerun.

### Remaining gaps / acceptance boundary

The exit structure still appears more close-up/fragmented than the reference; cave wall shading/magnification also differs. Neither has a proven additional source defect yet. Arch = valid939-vertex mesh, baked color alpha255, no controller, identity root, matching source asset hash; wall atlas decodes cleanly. Do not invent an asset-version explanation or per-model scale/angle, or mark B109 resolved solely because these supported fixes pass. Independent review and root acceptance required; worker retains follow-up ownership.

Package files required: none new. Separate particle-only loader has the same omission for nonidentity local effects, outside this patch's proved consumer. Read-only fire-door audit sent to root: Standard_20/202 effect matrices both identity. Effect packed colours0xFF000000/0xFF00115B versus material floats approximately(0,0,0,255)/(92,18,1,255); no hidden brighter default or atlas scale.

## Resumed investigation, before implementation (2026-10-11)

- Visual evidence re-inspected: P1 sequence `e_sheet.jpg` (700â€“715 s) and `t_sheet.jpg` (680â€“760 s), with the original exit arch to the upper-right of the blood, travel at 716 s, and cave entrance at 733 s. Compared preserved runtime `runA/f190.png` (arch engulfs hero) and `runC/f650.png` (cave wall texture looks smeared). Rotation diagnosis is supported by the exit sequence; cave cause remains unknown.
- IDA confirms the matrix convention directly: `VisualObject::SetRotation` 0x472874 converts Y,-X,-Z and only writes an existing scene node when the quaternion changed, then recalculates/applies its mesh box. `SyncRotation` 0x472948 reads GameObject +364; `GameObject::SetRotation` 0x3938a0 stores the rotation and synchronizes when a visual exists and virtual +112 admits it. `VisualObject::Sync` 0x38ba74 synchronizes position/rotation/scaling. `ISceneNode::setRotation` 0x5970f4 stores quaternion +184 and sets dirty bit 4; `getRelativeTransformation` 0x598908 on dirty bits 2/4 calls `quaternion::getMatrix_transposed` 0x5602d0, post-scales, writes translation, clears dirty bits. Direct SetRotation calls also come from `AnimatedFX::SyncIrrData` 0x492aa0, gated by an existing FX visual and rotation-follow/explicit-rotation flags; they use the same scene-node path.
- Port consumers audited: actor profile scale preserves placement columns (`actor_population.cpp:15`); containers, doors, gated decor and scripted decor retain definition placement; ambient decor decodes with it; `campaign_host/trigger_zones.cpp:35,139` uses normalized placement columns for oriented boxes. No compensating sign conversion found in these consumers. Existing trigger tests cover axis-aligned/-90 boxes but cannot detect inverse arbitrary angles.
- Expected minimal reusable implementation: one authored Euler TRS helper using the same transposed quaternion convention as BDAE nodes, called by both XML loaders. Explicit matrix attributes retain precedence. Level geometry previews retain static decor; gameplay defers decor to its runtime owners so the same declaration is never both merged and submitted as ambient geometry. Ambient excludes conditional/script-owned/hidden declarations. No asset-name or map-specific correction.
- Focused tests defined before coding: module/decor and actor rotations on all axes with nonuniform scale; arbitrary rotated trigger inclusion/exclusion (inverse-angle point rejected); container/NPC facing matrices; static-preview vs runtime-owned level decor counts; gated/hidden decor excluded from ambient. Hidden production EXE exit frames at the blood and exit trigger, then travel into cavern and capture the cave walls. Compare reference layout, record EXE SHA, run focused tests and full ctest.
- Uncertainties: no claim yet that cave smearing is fixed; original film is v1.0.3 and assets are Android export. Module-local actor expansion currently adds only translation (pre-existing behavior) and is outside the inverse-quaternion defect; do not silently change it without separate source evidence. Long unchanged cinematics will not be rerun.

## HANDOFF

### Additional cave investigation (before the material fix)

The P1 733â€“739 s cave sequence shows broad caustic cells on the entrance floor, while `sol-20261011/first-cave/f200.png` shows dense cells. Diagnostic `scene_probe --params` proves the cave water `Standard_2` material has no matrix override, but its local effect `ProfileCOMMON_Standard_2-fx1302961542_swamp_cave_watercaustics` supplies `diffuse-sampler-matrix = diag(0.25,0.25,1,1)`. The current loader returns identity and UV 0..1, displaying the complete 4Ã—4 atlas. This is a general missing effect-default consumer, not a cave-specific scale rule. The wall atlas itself (`sol-20261011/env_swamp.png`) decodes cleanly; wall smearing is not yet diagnosed as a separate defect.

Source evidence: `CColladaFactory::createMaterial` 0x6323d0 constructs the effect renderer before `createMaterial` 0x631ce8 applies explicit material parameters. `createMaterialRendererForProfile` 0x6361e8 installs reflected effect defaults via `setMaterialParameter<SNewParam>` 0x6324c4; its matrix branch 0x6326e8â€“0x6327b0 copies BRES matrix values, skips identity installation and retains nonidentity defaults. Existing recovered handoffs `port/scene-materials/reference/material-matrix-v4-integration.md` and `effect-material-gles-v4-integration-validation.md` independently verify that consumer and actual BRES effect-default matrices. The generic preview currently reads only material-level `texture-matrix`. Expected fix: initialize the diffuse UV matrix from a resolved local effect default, then apply explicit material overrides; preserve external-effect behavior and explicit identity overrides. Test before coding: real caustic default 0.25; explicit material override wins; invalid matrix fails transactionally; hidden production cave sequence shows atlas-cell scale, exit layout and actor/chest/trigger regressions retained.

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

With the transposed (original) convention, the troll entrance turns 47.5Â° the other way:
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
   - Then inspect the materials with `SCENE_PROBE_MATERIALS=1 scene_probe swamp_cave.bdae _module_<name>-node`. Wall range: `env_swamp.tga` (PVRTC 1024Â², stored with a .tga name), uv 0.32..1 x 0..0.5, identity texture matrix.
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
