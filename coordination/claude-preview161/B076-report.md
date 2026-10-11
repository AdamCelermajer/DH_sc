# B076 report: "something is missing in the Swamp (an effect or a colour)"

Branch `fix/b076` (worktree `DH_wt/fix076`, build `build-fix076`), baseline = Preview 16 EXE (`.local-inputs/windows-source-clock-v19-preview-16`).
Scratch and all evidence images: `.local-inputs/claude-preview161/fix076/` (named below). Commits: 2fd90139, d2eb6a32, 3a0891da (+ this report).

## 1. Verdict

The report is correct. Compared with the video, our Swamp was missing (a) the level's dynamic **PlayerLight** (a point light that follows the hero and brightens/colours the whole environment), (b) the **specular** term of the same shader (wet shine on tiles), (c) every ambient **AnimatedDecor** (tiki torches with animated flames and glow, water caustics, cave-entrance glow and cave-entrance ruin) and the **Decor props** (corpses, corpse piles, bloodstain decals), (d) the level `clear_color`. (a)-(d) are fixed (data-driven, no level names in code). Dust-mote particle clouds, torch bone breathing and actor lighting remain open (section 5).

The user's screenshot (`user-shots/b076-dark-chest-missing-light-or-color.png`) is the opening-cutscene chest shot; P16 frame 500 of the production swamp config reproduces it exactly (`prod16/f500.png`), the fixed build shows the lit ground, bones and bush (`prod/f500.png`, side by side in `prod/ab3.png`).

## 2. Evidence (investigation)

### 2.1 Video (`Dungeon Hunter 2 (v1.0.3) Part 1`, 640x360, no audio)
Frames viewed (extracted with ffmpeg, files in `vid/`): t=150/160/170/175 (start room, quest banner), 190, 200 (chest tutorial camera), 250, 290, 330, 345, 360-440, 470, 490, 520, 540, 560, 600, 620, 650, 680, 690-712 (deadend boardwalk and cave entrance), opening cinematic 62-162 s. Observations (direct):
- The floor/water within roughly 600-1000 units around the hero is bright and strongly saturated (cyan/turquoise), falling off to dark further away (t=175, 290, 600, 690-712). Floor tile edges show wet white-cyan highlights near the hero (t=290, 600).
- Orange flame torches with a warm light pool on the boards/tiles (t=440 right, t=540 right, t=704 upper left, t=150 upper left glow).
- The swamp water carries a bright web/caustic pattern (t=380, 440, 690-712).
- Blood pools / corpse piles on the ground (t=380-420 red area, t=712 red glow at the cave entrance with sparkles).
- Cave entrance (t=704-712): tilted mossy stone slabs standing in the water plus red blood and a glow.
- Light shafts (blue columns) in the chest room at t=200 and in the start room.
Mean luminance of the central region (crop 320x180 of a 640x360 frame; crude, framing differs): hero in gameplay, start room t=175: video 74, P16 40.9, fixed 72.8; deadend t=704: video 91.1, P16 26.6, fixed 95.1. (Frames where the hero is far from the camera, t=200/540/620, are cutscene cameras and not comparable.)

### 2.2 Logic (original data and IDA)
- Shader source (shipped `shaders.pak`, `assets/shaders/GL_Diffuse_L1_iPhone_VS.glsl/_FS.glsl`): vColor = (Light0.ambient + Light0.diffuse * max(0,N.L) * attenuation) * Color.rgb; attenuation = 1/(x + y*d + z*d^2); specular term `Light0.specular * SpecularMap.z * att * pow(max(0,N.H), Glossiness)` added after the texture; the clamp of vColor is commented out in the VS.
- Material parameters of `swamp.bdae` (dumped with a throw-away tool): `Material__11610/11611` use effect `GL_Diffuse_L1_VC_iPhone.bdae`, GLES2 technique `L1_Vc_[Al]_Sp_...` (no fog slot, no lightmap), `Specular = env_swamp_spec.tga`, `Glossiness = 10`, `Spec_Level = 1`. These two materials are every textured level surface. (The ES1.1 technique has no `Sp`.)
- Light data: `light_set` of `001_swamp.mlx` -> `swamp.lightset_xml`: PlayerLight, ambient/diffuse/specular 255, attenuation (0.265625, 0, 0.132813) -> after `LightBase::InitPost` 0x40b0c0 (colours/255, y/1000, z/1000000) (0.265625, 0, 1.328e-7), radius 3.5, attachedTo Player, attachedOffset (0,0,200). The ES1.1 `fixed_light_set` differs (0.15625, 0, 0.484375): not used (the shader pipeline is the video's).
- IDA (`libDungeonHunter2.so`, pseudocode-all.c): `LightPoint::RefreshAttachment` 0x40bac0 (line 193169): a light named `_prim_PlayerLight_*` is bound to the local player (`PlayerManager::GetLocalPlayer`) and `LightSetManager::AddActiveLight(..., attachedOffset)` is called unless debug switch `EnablePlayerHeadLight` is set; `LightPoint::Update` 0x40ba0c (line 193121): every update the CLight position = attached object's position + attachedOffset. `LightSetManager::ApplySettings` 0x40c9a8 (existing evidence, `SOURCE_ACTOR_LIGHTING.md`) binds slot lights to the shader's `light0` uniforms. `AnimatedDecor::InitPost` 0x389128 (line 102010): `startanim` defaults to "idle", the clip is looked up by name and played looped; the decor is physical only if its mesh box flag is set. The ambient Swamp AnimatedDecor declarations are in each `mvp` (module visual props): `swamp_tikitorches` (15), `swamp_watercaustics` (10), `swamp_rayolight_dust` (3), `swamp_caveentrance_effect` (3), `swamp_cave_entrance_troll` (1) and `Decor` props `prop_swamp_corpses` (10).
- Decor clips (dumped with a throw-away tool): they animate materials, not nodes: channel type 88 = texture-matrix offsetU, 87 = offsetV (flip-book of `anim_torch_fire.tga`, 8x4, 1066 ms loop; `anim_caustics.tga`, 4x4, 1500 ms loop), 86 = diffuse colour RGBA 0..255 (`wiimaterial_shader*` of the cave entrance glow, 5333 ms). The engine's character-animation loader rejects these channel types (`Unsupported compiled animation channel (type 86/87/88)`), so the P16 decor path could never load them.

### 2.3 Inference (not proven)
- The `lowp` varyings run unclamped on the video's GPU (clamp commented out, light attenuation 1/(0.27+...) is 3.7 near the hero, so (1 + att*N.L) reaches 4.7). We allow a factor of 4 (`kLevelLightScale`, see 3.2). Support: the two hero-in-gameplay luminance comparisons above (74 vs 72.8, 91.1 vs 95.1). A factor of 2 left the deadend water at 52 (video 91).
- Video device uses the ES2 shader profile (Sp specular present in the video: wet tile highlights). Not provable from 640x360 footage.

## 3. Implementation (what changed)

1. `renderer.hpp/.cpp`: `LevelPointLight` + `Renderer::setLevelLight`; GL_LIGHT1 as the source point light (constant/linear/quadratic = source attenuation); `Material::levelLit` ranges are drawn with lights scaled by 1/4 and a `GL_COMBINE` texture environment with `GL_RGB_SCALE` 4 (`GL_RGB_SCALE` only acts in COMBINE mode; this was found by measurement); unlit/other draws reset to MODULATE/LIGHT0. `Material::uvOffset` (texture-matrix translation, reset after every draw so HUD/overlay never see it). Specular: `Material::specularTexture/glossiness`, opaque lit ranges get an additive second pass (light1 specular only, local viewer, shininess = Glossiness, specular map modulates). `setClearColor`.
2. `scene-materials/scene.hpp/.cpp`: parse material params `Specular` (image) and `Glossiness`.
3. `original_scene.cpp/.hpp`: `levelLit` for effect files `GL_Diffuse_L1*`, spec map/glossiness when the GLES2 technique's 4th slot is `Sp`, authored NORMAL stream (attribute 1) instead of face-accumulated normals when present.
4. `features/level_lighting/level_light_v1.*`: reads `LevelConfig.light_set` (or `fixed_light_set`) and `clear_color` of the level, returns the Player-attached light (units after InitPost). `main.cpp`: per frame light position = hero position + attachedOffset.
5. `features/scene_decor/ambient_decor_v1.*`: loads every `AnimatedDecor`/`Decor` declaration (except cutscene `cs_*` libraries, which stay script driven) through `decode_original_scene_module(xrefobject + "-node", declaration placement)` (root TRS replaced by the declaration, children kept), parses the material clips (86/87/88), animates `uvOffset`/colour per frame (looping segment length), additive effect quads unlit. Disable for A/B with `--no-ambient-decor`, `--no-level-light`.
6. `engine-animation/animation.cpp`: the rejection message now names the channel type and target (diagnostic only).

Rules kept: data-driven (light set, xrefobject, material ids come from the original files), no map-specific code, GL code in the renderer; Linux/Android parity: only fixed-function GL calls available there too (combine is GL 1.3 / GLES1.1 `GL_COMBINE`).

## 4. Differences found and what was done

| # | Difference (original vs P16) | Evidence | Status |
|---|---|---|---|
| 1 | No dynamic PlayerLight on the environment (floor/water brightness and tint around the hero) | video t=175, 290, 600, 690-712; shader; IDA 0x40ba0c/0x40bac0 | FIXED |
| 2 | No specular (wet tile highlights) | video t=290, 600; materials: Sp, Glossiness 10 | FIXED (opaque ranges; alpha ranges `L1_Vc_Al_Sp` have no spec pass, see 5) |
| 3 | Ambient AnimatedDecor missing: tiki torches (flame flip-book + glow, warm pool), water caustics, cave-entrance glow, cave-entrance ruin | video t=150, 440, 540, 704; clips type 86-88; IDA 0x389128 | FIXED (not the torch bone breathing, dust motes) |
| 4 | Decor props missing (corpses, piles, bloodstain decals) | `mvp/mgp` Decor declarations, `prop_swamp_corpses.bdae`; video t=380-420, 712 | FIXED (needs the package file) |
| 5 | Clear colour hard-coded blue-grey | LevelConfig `clear_color="0,0,0"` | FIXED (from level data) |
| 6 | Authored vertex normals ignored | NORMAL attribute exists | FIXED |
| 7 | Cutscene chest shot black (user screenshot) | `prod16/f500.png` vs `prod/f500.png` | resolved by 1-4 |
| 8 | Light shafts | shaft meshes are module geometry (additive `Material__11598`), already drawn in P16 at the same places (video t=200 vs `fix-C1.png`) | no difference |
| 9 | Fog | Swamp materials have no `Fg` technique slot; LevelConfig fog 4000..5800 is beyond the view; nothing to draw | no difference (not implemented) |
| 10 | Scene ambient 0.5 | `GL_Diffuse_L1` ignores it (SceneAmbientLight is only in the COMMON lit branch) | no difference |
| 11 | Post-processing / bloom | none found for the Swamp (RadialBlur shader is for hit effects) | no difference |

## 5. Remaining gaps (with timestamps; not done)
- **Dust motes / light-ray particles** `GL_PCloud01` in `swamp_rayolight_dust.bdae` (3 declarations: deadend (527,458,853), obj_4of4 (1244,-136,853) and (-1826,704,853)) and `GL_PCloud01/02` in `swamp_caveentrance_effect.bdae`: particle clouds need the glitch particle-system runtime; no frame in the 640x360 video shows individual motes (t=200 chest room, t=704-712 cave entrance), so they are unverified. The logged notice `ambient decor _anim_swamp_dustmotes ... no static geometry` marks them.
- **Torch bone breathing** (node scale 1.0..1.03, `_bone_swamp_torch_*`, 1066 ms): not applied (imperceptible).
- **Alpha-tested/blended lit ranges** (`Material__11611`, `L1_Vc_Al_Sp`) get the diffuse light but no specular pass.
- **Characters** (hero, NPCs, enemies) still use the preview ambient 0.65 + directional light; the original uses LightSet slots (MonsterLight/SceneLight) whose Swamp contents are undefined in the data (`SOURCE_ACTOR_LIGHTING.md`). Not changed.
- **Fog** for other levels' `Fg` materials (not in the Swamp) and the skybox (`skybox_swamp.bdae`, never visible with the gameplay camera in the video).
- **Light caps**: the original's real value range of the lowp varying is unknown (see 2.3); our cap is 4x.
- Decor placement is taken from the declarations; the decl `_mesh_animwater_deadend_brdwalk_w__nobatch_001` names a node that does not exist in the library (the original would fail too): skipped with a notice.
- `GL_DEBUG`: ES1.1 `fixed_light_set` path (different attenuation) is parsed but not selected.

## 6. Tests and verification
- New `ambient_decor` test (`ctest -R ambient_decor`): sampler hold/step/linear/empty, real Swamp torch/caustic/cave libraries parse (channel kinds, 1066 ms loop, colour keys normalised), garbage rejected, swamp PlayerLight read (offset 200, attenuation after unit conversion, colours, clear colour), ES1.1 light set, missing light set file -> error and no light, missing level rejected.
- Regression: `ctest -j6`, 141 tests, only `session_skill_binding` fails (known, worktree-only).
- Runtime (real EXE, quiet_run, hidden desktop, 960x540, fixed step): 11 positions of the swamp modules, before/after frames and the production opening config (800 frames, cinematic with SKIP and captions) - no crash, no GL error after the fix (a temporary `glGetError` probe found GL_INVALID_ENUM from my first combine setup; it also dropped the first glyph texture of the HUD ("SKIP" rendered "KIP"); fixed by using the combine defaults, `prod/ab3.png` shows "SKIP").
- Side-by-side sheets: `sheet-L1.png`, `sheet-L2.png`, `sheet-L3.png`, `sheet-L4.png`, `sheet-L6.png`, `sheet-L7.png`, `sheet-L9.png`, `sheet-L10.png` (video | P16 | fixed), `big1.png`, `big2.png`, `bigA.png`; `chests.png` (user chest, two chests), `prod/ab3.png`.
- Locations (world position -> nearest video time; positions were matched by landmark, so framing differs): L1 start room (1090,-212) / t=175; L2 chest room (-2160,880) / t=200; L3 boardwalk chest (-6040,4960) / t=345; L4 merchant camp (-12000,12000) / t=540; L6 ruin (-6000,12000) / t=620; L7 deadend water (-5300,18700) / t=704; L9 corner boardwalk (-12000,18000) / t=420; L10 sea boardwalk (0,6000) / t=380; plus C2 (1790,1730) the chest of the user's shot and L5, L8.
- Not verified: a real-time performance budget (the 120-frame jobs take the same ~7 s as the baseline), the Linux/Android builds, the ES1.1 profile.

## 7. Package files required
New asset files (taken from `.local-inputs/assets-extra/android/data/3d`, same relative names under `assets/data/3d/`):
- `animateddecors/swamp_tikitorches.bdae`, `animateddecors/swamp_watercaustics.bdae`, `animateddecors/swamp_caveentrance_effect.bdae`, `animateddecors/swamp_cave_entrance_troll.bdae`, `animateddecors/swamp_rayolight_dust.bdae` (parsed; no geometry yet)
- `props/swamp/prop_swamp_corpses.bdae`
All textures they use are already in the P16 package (no `Missing texture` in any log). `assets/original-cache/data/3d/light/swamp.lightset_xml` and `swamp_es_1_1.lightset_xml` (already present). Without the new decor files the game logs a notice per missing library and still runs (the light, specular and clear colour fixes need no new files).

## 8. Process note (honest)
While preparing a scratch asset root I ran a `Copy-Item` whose destination resolved through a junction into the P16 package: it created a nested `assets/data/3d/animateddecors/animateddecors` (91 files) in `windows-source-clock-v19-preview-16`. I deleted exactly that nested folder immediately and verified the package listing (only the two original `cs_*` files remain); two package files were also overwritten in place by an identical copy (SHA256 identical to the source). The folder timestamp changed. The disk was full for a while (other sessions); I freed only my own scratch/test binaries.
