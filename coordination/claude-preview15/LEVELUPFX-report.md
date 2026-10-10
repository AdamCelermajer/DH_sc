# LEVELUPFX report (I026 fidelity: level-up light column colour/orientation)

Branch p15/levelupfx, worktree DH_wt/levelupfx, build DH_wt/build-levelupfx.
Scratch, jobs, captures and debug patch: `.local-inputs/claude-preview15/levelupfx/` (mkjobs.sh, jobs/, png/, tools/, pkg-android/).
Status: INVESTIGATION ONLY. The visual divergence is NOT fixed. No production code changed. All debug hooks were reverted; the debug diff is kept at `.local-inputs/claude-preview15/levelupfx/tools/debug-dump-levelupfx.patch`.

## 1. Input facts (verified)
- rc4 `assets/data/3d/interface/level_up.bdae`: 29292 B, SHA256 5c6399447c9f...efecc. Byte-identical to the iPad copy `.local-inputs/assets-extra/ios/...`.
- Android copy `assets-extra/android/...`: same size, different bytes. The only difference is the FX-id suffix in material names (`fx1288847105` iOS vs `fx1302961745` Android; about 80 bytes across 10 strings). Geometry, nodes and textures are identical. So iOS and Android art match, and a version difference is NOT evidenced by these copies. The v1.0.3 video cannot be compared to file bytes locally.
- Render test with the Android copy swapped into a copy of the rc4 assets (`pkg-android/`): captures are identical to rc4 at f160 (`png/android-f160.png` vs `png/rc4-f160.png`).

## 2. Decoded FX content (BRES root, scene, geometry; standalone Python in `tools/`)
- Scene (10 nodes, Z-up game space): `Helix01` (scale 0.39); `IrrGravity_Down` (quat 180 deg about X, col2 = -Z); `IrrGravity_Up` (quat 180 deg about Z, col2 = +Z); `_bone_halolevelup` (identity) with children `_mesh_glow_floor_nobatch` (z +27.3) and `_mesh_levelup_nobatch` (identity) -> `_mesh_levelup_nobatch_PIVOT`; emitter nodes `glow` (z -12.6), `stars` (x +39.4, animated by `stars-node-translation`), `streaks`.
- Geometry (type 0, from SMesh bounds): `_mesh_glow_floor_nobatch`: 4 verts, square +-260 in XY at z=0 (floor glow). `_mesh_levelup_nobatch`: 18 verts, x [-100,100], y [-63,1077], z [76.9,84.9]. That is a flat sheet 200 wide, 1140 long, 8 thick, lying horizontal in Z-up space. It is not a vertical column in the data.
- No skin controller (controller=-1 on all instances). No rotation animation channel for the light-ray mesh (channel names seen: `*-emitter_emission_birth_rate`, `stars-node-translation`, `AUTO_SceneAnims_Range`).
- Materials are additive (`ProfileCOMMON_fx_particles_additive_000/_streak`, `gloow`). Textures: `atlas_fx_particles_000/002.tga`, `fx_aura_paladin_lightray_halo.tga`, `fx_magic_lenz_flares_005.tga`. No colour track names, so the particle colour fallback applies (colour = scene colour = white 0xffffffff under the port's source_white policy).

## 3. Runtime facts (our EXE, rc4 assets, quiet runs)
- Level-up presentation: `Level up presentation frame=148 ... fx=135:level_up:played text=LEVEL UP! drawn=0`. Frames 144-146 have no presentation line.
- FX record for the set: orient_once=0, orient_with_anchor=0, scale_with_anchor=1, fixed_rotation=0; anchor = player position (-6790.4, 938.1, 255.0); visual_rotation = 0.
- IDA: the overload `PlayAnimFXSet(manager,135,GameObject)` passes `&Vec3f_Origin` as position and a zero rotation (`Vec3f_Origin`, `dword_99F858`, `dword_99F85C` are all 0 in the IDA data section). The identity rotation therefore matches the original for this set.
- Particle draw sources for the level-up FX are very sparse: 5 parts in the whole run (glow 1, stars 1, streaks 2, in 1-2 frames). Scalar BirthRate samples for the FX are 0 for the first ~700 ms, then 60 and 90 for about 100 ms (ms 707-790; the FX-local timeline is not confirmed). The warrior-bash FX in the same run shows the usual 64-65 parts per emitter, so the sparse particles are specific to this FX.
- The diagonal gold beam in our captures (f150-f200) is the static flat sheet `_mesh_levelup_nobatch` drawn with identity orientation. Seen obliquely it reads as a diagonal beam. It comes from the mesh path (`mesh_draw_sources_v4`, 3-4 mesh draws per frame during the FX).
- Gravity: `dh2_particle_gravity_apply_v1` uses matrix column 2 (m[8..10]) and strength*1000, which matches IDA `PGravity::apply` (direction from object offsets +32..+40, strength `(*a1)[1]*1000`, dt from ctx+80). So the gravity code agrees with IDA. IrrGravity_Up's direction is +Z, which is up in the game's Z-up world (the hero's Bip01 root sits at z=188 above the feet).

## 4. IDA draw-path trace (requested by the coordinator)
- `Character::LevelUp` 0x3beb88 (pseudocode line 138700): the only FX call is `VisualFXManager::PlayAnimFXSet(&Singleton<VisualFXManager>::s_inst, 135, this, 0)` (line 138795). No code-built column, ring, or colour constant in that function.
- `VisualFXManager` (pseudocode lines 288743-291800): all FX are data-driven (`PlayAnimFX`, `PlayAnimFXSet`, `PlayAnimFXStep`, `_BuildAnimFXDictEv`, `GetAnimFXData`, `Update`). No primitive or colour construction inside the manager. Set 135 -> file index 63 -> `data/3D/interface/level_up.bdae`.
- Billboard: glitch Collada has a billboard node type (`ColladaFactory::createBillboard` 003507bc; `CColladaFactory::createBillboard` 00631830; `CBillboardSceneNode::render` 00454663; `CBillboardSceneNode::updateAbsolutePosition` 0060cf88). A camera-facing node with a world-up axis would turn a 1140-long flat sheet into a vertical column. NOT CONFIRMED: I did not find which node type `_mesh_levelup_nobatch` has in the BDAE (the node type dispatch in the database reader was not decoded), and the port's scene loader has no scene-node billboard (only particle billboards: `particle_billboard_v1/v32`). This is the leading hypothesis for the vertical column, not a verified fact.
- Colour: no level-up colour constant found in the code paths above. Not searched exhaustively in the material/blend code (`__irrlicht_Additive` state is data in the material).

## 5. Video reference (LOOK, dense frames)
- `levelup/ref/burst_768_8_770.jpg` (12 frames at 10 fps, 768.8-770.0): t 768.8-769.0 hero with a small white-blue flash at body height; t 769.1-769.4 a tall white column with bloom from above the hero down to the feet, fading by ~769.5. No "LEVEL UP!" text in any of these frames. The original column is vertical; our sheet is horizontal in game space.
- Timing: the original column lasts about 0.2-0.4 s; our sheet persists 0.8 s or more (f150-f200).

## 6. Placeholders
- Visual column/burst: NOT implemented. Proposed labelled placeholder (needs a coordinator decision, not coded): a camera-facing additive white column at the hero's feet, height ~1140 units, shown at reference t 769.1-769.4, labelled `LEVELUP_PLACEHOLDER_COLUMN`. Reason: the billboard orientation is unconfirmed, so a permanent fix must not invent it.
- "LEVEL UP!" HUD text: still not drawn (no status-message owner). No text is visible in the reference frames 768.8-770.0 (checked) nor in the earlier 770-774.5 sample.

## 7. Changes
- None to production code. Debug hooks were used and reverted; the patch is in `.local-inputs/claude-preview15/levelupfx/tools/`.

## 8. Tests
- Build `p14_build.ps1 -Name levelupfx` green (with debug hooks; reverted afterwards).
- No new tests (no fix).

## 9. Package files required
- None new (level_up.bdae is already in rc4).

## 10. Verifier script
1. Build: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name levelupfx`.
2. Jobs: `.local-inputs/claude-preview15/levelupfx/mkjobs.sh <EXE> <tag> 144 146 148 150 152 155 160 170 180 200` (writes `jobs/<tag>.json`; args retarget the rc4 assets and the XP fixture `fixtures/character-xp-high.save`).
3. Run: `quiet_run.ps1 -JobsFile jobs/<tag>.json -Parallel 10 -Summary ...` (exit 0 for all jobs).
4. Expect: `Level up presentation frame=148 ... fx=135:level_up:played` in logs of f148 and later only. Captures f150-f170 show the gold diagonal sheet, not the white vertical column of the reference.
5. Alternate package: `PKG=<dir with assets + gameplay.save>` (pkg-android) gives identical captures.

## 11. Open risks / not verified
- Whether the original draws `_mesh_levelup_nobatch` vertically (billboard) or the vertical column comes from particles: not decided. Dense particle sampling in the original was not done.
- The FX-local timeline for birth/particles (ms 707-790) is not mapped to frames with certainty.
- v1.0.0 vs v1.0.3 art: iOS and Android copies match in content; the video version cannot be checked against files here.
