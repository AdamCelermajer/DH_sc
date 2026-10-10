# LEVELUP2 report (level-up light column: billboard mechanism, what is verified, what is not)

Branch `p16/levelup` (worktree `DH_wt/p16levelup`, from `p16/integrate` 261f2e49). Build `DH_wt/build-p16levelup`.
Scratch, captures, tools and debug logs: `.local-inputs/claude-preview16/levelup2/` (tools/bres_dump.py,
bres_anim_keys.py, bres_effects.py, ida_fn.sh, mkjobs.sh, mkjobs_ns.sh, ref30/, jobs/).

Status: the vertical-billboard mechanism is decided from the binary and implemented generally. The level-up column
colour and duration do NOT yet match the reference (section 5). The visual match is not established.

## 1. Investigation (evidence)

### 1a. Decoded `level_up.bdae` (iPad copy, byte-identical to rc4)
- Scene `level_up.max` (7 nodes): `_bone_halolevelup` (identity) with children `_mesh_glow_floor_nobatch` (z +27.3)
  and `_mesh_levelup_nobatch` (identity) -> child `_mesh_levelup_nobatch_PIVOT` (identity, mesh instance
  `_mesh_levelup_nobatch-mesh`). Emitter nodes `glow`, `stars` (x +39.4, animated), `streaks`. Force nodes
  `IrrGravity_Down` (quat 180 about X) and `IrrGravity_Up` (quat 180 about Z: game up = +Z).
- Sheet geometry: 18 verts, x [-100,100], y [-63,1077], z [76.9,84.9]: a flat 200 x 1140 sheet in local XY.
- The only SNode+76 != 0 in the file is on `_mesh_levelup_nobatch_PIVOT` (word 0x6db8).

### 1b. The vertical sheet is a glitch collada BILLBOARD (decided from the binary)
- `CColladaDatabase::constructNode` 0x61b2f4: `if (a3[19])` (SNode+76, word index 19) selects factory vtable slot 64
  (`createBillboard` 0x631830) instead of slot 60 (`createNode` 0x631800). Factory vtable
  `_ZTVN6glitch7collada15CColladaFactoryE` decoded from vtables-and-rtti.json: slot 15 = 0x631800, slot 16 = 0x631830.
- Class `glitch::collada::CBillboardSceneNode` (vtable `_ZTVN6glitch7collada19CBillboardSceneNodeE`,
  `updateAbsolutePosition` 0x60cf88 at slot 53). The SNode+76 block is the billboard record: word0 mode = 1,
  word1 sub = 1, words 2-4 axis A = (0,0,1), words 5-7 axis B = (0,1,0).
- Camera: `this+272` (word 68) holds the scene manager (zeroed by the ISceneNode ctor, set on registration);
  `CSceneManager+228` is the active camera (`setActiveCamera` 0x5890c0). Without a manager the node is plain.
- `updateAbsolutePosition` target branch (mode != 2, sub != 2), as decoded:
  O = parent absolute position; D = normalize(eye - O); C = camera up = row 1 of the camera matrix (m1,m5,m9), the
  glitch camera matrix being the world-to-eye view matrix; S = normalize(D x C); U = normalize(S x D);
  F = [-S, U, D] (columns); A, B = parent-rotated axes; P = normalize(B x A), Q = normalize(A x P);
  absolute = T(O) * F * [P Q A]^T * Mrot(parent) * TRS(node).
- Check: with an identity parent, U is the projection of world up onto the view plane, i.e. screen-up, so the
  1140-unit local Y becomes a screen-vertical column. Numerical check in the EXE (scene space, camera pitched ~50 deg):
  D.C = 0, U = C = (-0.553, 0.553, 0.623), S = (-0.707,-0.707,0). A first attempt that used m1,m5,m9 of the inverse
  camera matrix gave U = (0.863,0.449,-0.233) (horizontal); the view-matrix reading is the consistent one.
- The same billboard path applies to the warrior-bash shockwave (`_mesh_shockwave_nobatch_PIVOT`).

### 1c. Animation, emitter and timing data (30 fps key units; `tools/bres_anim_keys.py`)
- Segment [0, 2000] ms; keys in frames (the port's own 1000/30 ms conversion), last key 60 = 2000 ms.
- Sheet material `_7_-_Default1` diffuse colour (track type 86, bytes read as RGBA): f3 (85,50,12), f4-5 (172,122,24),
  f18 (172,122,24), f20 (137,93,19), f24 (68,36,10), f26 (33,7,5), f28+ black. Sheet diffuse default = (255,235,0).
  Ambient (0.584 grey) and emission (black) defaults are not white.
- Emitters (birth rate, keys 30 fps): glow 30/s to key 20, stars 90/s to key 30, streaks 30/s to key 15 (0.5 s).
  Material per emitter: glow `_7_-_Default1111`, stars `fx_particles_additive_000`, streaks `fx_particles_additive_streak`.
- Birth-rate sampling in the EXE is correct (traced: the streaks binding gives 30 at 0 ms). The earlier "zero birth
  rate" note was a debug-counter artefact of the trace, not a bug.
- Particle draw: the port renders billboards only (`particle_billboard_v32`). The glitch::ps IDA classes are billboard
  render models only (PRenderDataBillboardModel, PSBillboard*Baker). No streak or stretch geometry was found in IDA
  names or strings, so a velocity-stretched streak renderer cannot be read from this binary.
- Swoosh FX set 253 (`swoosh_prince_1hand_combo_01`) has no billboard node (checked: SNode+76 = 0).

### 1d. Reference video (`ref30/`, part1 640x360 30 fps; contact sheet `ref30/sheet_a.jpg`)
- 769.03-769.10 s: small white-blue flash at body height (direct observation).
- 769.13-769.47 s: tall white column with bloom from above the hero down to the feet; thin jagged white lines at the
  top. 769.50 s: column gone, green halo remains.
- Top-5% colour of the column core (`tools/ppm_stats.py`): reference (247,251,250) = white. Our gold beam: (244,219,102).
  The reference core is saturated in all three channels; a gold tint cannot produce that.
- No "LEVEL UP!" text in the reference frames 768.8-774.5 (checked earlier and in the 30 fps sheet): stays absent.

## 2. Implementation (this pass)
- `port/scene-materials/scene.hpp`: `BillboardRecordV1 {present, mode, sub, axis_a, axis_b}` in `NodeStorageV91`.
- `port/scene-materials/particle_scene_v1.cpp` and `scene.cpp`: parse the SNode+76 record (32 bytes) for every node.
- `port/level-world/source_fx_node_matrix_v4.{hpp,cpp}`: `source_fx_rebuild_graph_world_billboards_v1` (full graph
  rebuild with billboard nodes oriented to the scene camera; scene-space camera = inverse(outer) applied to the view
  matrix and the eye) and `source_fx_scene_has_billboards_v1`. Non-billboard nodes use the same TRS composition.
- `port/level-world/character_authored_resource_v32.cpp`: the composite keeps the scene camera services and runs the
  billboard rebuild in `source_scene_frame_v4` before emitters and mesh draws. Plain FX skip the camera call.
- Billboard modes 2 or sub 2 fall back to the plain node (not decoded; only the level-up and warrior-bash records
  were seen in the corpus checked).
- Debug prints and experiments (scalar-binding traces, RGB swap) were reverted; the commit contains only the above.

## 3. Tests
- Build `p14_build.ps1 -Name p16levelup -Jobs 6` green after the final cleanup.
- ctest result: see section 6.
- No unit test for the billboard basis yet (gap, section 5).

## 4. Runtime verification (EXE, quiet runs, rc3 package, `tools/mkjobs.sh`)
- Baseline (`exe/baseline`) and billboard (`exe/clean`) both log `Level up presentation ... fx=135:level_up:played`.
- The billboard build changes the frames from f156 on (checksums differ; f146-f154 identical).
- Frames LOOKED AT: with the warrior-bash skill key in the run, the billboard frames at f160 and f175 show a gold
  column rising above the hero to the top of the frame (`jobs/bb2-f160`, `strip-bb2.png`). Without the skill key
  (`strip-ns.png`, `jobs/nsclean-*`), baseline and billboard both show a gold beam that reads as diagonal at f160/f175.
  The billboard effect is not cleanly isolated from the attack FX, so this is not a conclusive visual match.
- Reference comparison: colour gold vs white (no match); duration: colour envelope ~0.9 s vs reference ~0.47 s.

## 5. Gaps (not matched)
1. Column colour: the reference is white; our sheet is gold (authored diffuse track read as RGBA; the RGBA reading is
   also what the artist's default (255,235,0) suggests). Not decided: the reference column may be the streak
   particles (white, `fx_particles_additive_streak`, birth stops at 0.5 s, matching the reference end), or the
   original's bloom/exposure, which the port lacks (windows-foundation visual-gap-ledger). Streak-particle is NOT
   verified: no streak geometry in IDA.
2. Duration: the sheet colour envelope fades at 0.6-0.93 s; the reference column ends at ~0.47 s.
3. Which element is the column (sheet billboard vs streak particles) is not decided.
4. No unit test for the billboard basis; the U = C (camera up) result is verified numerically in the EXE only.
5. Warrior-bash shockwave now renders as a billboard; not yet checked against a reference bash frame.

## Placeholders
- LEVELUP_PLACEHOLDER_COLUMN (not coded): the reference column (white, ~200 units wide, from the feet to above the
  hero, visible 0.10-0.47 s after the flash, reference 769.13-769.47 s) should replace the gold sheet colour and
  duration until the streak-particle or bloom behaviour is decoded. The sheet is drawn as the data says for now.
- Level-up "LEVEL UP!" HUD text: stays absent (the reference shows none).

## Package files required
- None new (level_up.bdae is in the rc3/rc4 assets). The verifier needs the `original-cache` layout (rc3 used).

## Verifier script
1. Build: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p16levelup -Jobs 6`.
2. Jobs: `PKG=.../windows-source-clock-v19-preview-15-rc3 .local-inputs/claude-preview16/levelup2/tools/mkjobs.sh <EXE> <tag> 146 148 150 ...`.
3. Run: `quiet_run.ps1 -JobsFile jobs/<tag>.json -Parallel 10`.
4. Expect: `Level up presentation frame=148 ... fx=135:level_up:played`; frames from 156 differ between baseline and billboard.
5. Convert `cap-N.ppm` to PNG with ffmpeg and crop around the hero; compare with `ref30/f_0NN.jpg` (30 fps reference).
