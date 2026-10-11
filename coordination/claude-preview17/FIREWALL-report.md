# FIREWALL report (Preview 17)

Branch `p17/firewall` (worktree `DH_wt/p17firewall`), started from `p17/doors2` (`151cf811`). Commits: `7891c55b` (scene-owned FX resource through the effect frame, symbol binding, colbox rule), `45d86dc2` (doors, levers and decor; single draw; material cache; `DH_PERF` split), and this report. Not pushed. Build `p14_build.ps1 -Name p17firewall -Jobs 6`, EXE `DH_wt/build-p17firewall/dh-foundation.exe`. Quiet runs use the rc3 package (`.local-inputs/windows-source-clock-v19-preview-15-rc3/assets`), run folder `.local-inputs/claude-preview17/firewall-run/`.

## Bottom line

- The fire door (`_prim_SwampDoor_FIRE`) now draws its authored flames and particle emitter through the existing effect frame, and its FX stops when the gate opens. The gate's wooden leaf opens with no fire and no glow left.
- The **violet glow** of the 8:03 frame matches the fire door's data (its magic is violet-blue, not orange). The rendering is in the same colour family, but this is not a same-frame match (the reference is a cutscene camera).
- The **orange wall of fire with bones** (7:59) is **not reproduced**. The data does not identify it as `_prim_SwampDoor_FIRE`. See Identification and Decision needed.

## Identification (from data, not from the video colour)

| Object (module decl) | Model | Evidence |
|---|---|---|
| `_prim_SwampDoor` (-588.5,-488.0) | `go_swamp_door_ruins.bdae` | Wooden leaves (`_bone_left_door`, `_bone_right_door`), env_swamp textures with alpha map. No emitter, no FX family. This is the **wooden barred gate** of 8:03. |
| `_prim_SwampDoor_FIRE` (-754.5,-488.0) | `go_swamp_door_ruins_fire.bdae` | Flames: two skinned `_mesh_fire` instances on `_bone_01..03` (additive, `Standard_8` with `fx_doorfire.tga`, colour grey 0.584). Particle emitter `GL_PCloud01-emitter` with `emission_birth_rate` track (material `Standard_8zxcv`, colour track RGBA 119,91,185, lavender). `_colbox_fire` is collision only. Clips `idleclosed`, `open`, `idleopened`, `close`. |
| Texture `fx_doorfire.tga` (PVRTC4 in BTEX) | decode probe | Average RGB 30,24,69; highlights 198,189,247. The port's decoder is correct: torch textures decode to their names (`anim_torch_fire` orange 70,58,13; `fx_torch_fire_blue` blue; `anim_torch_fire_green` green). So the magic wall is violet-blue in the data. |
| Swamp fire-named declarations | mgp scan | Only the two door declarations (`_prim_SwampDoor_FIRE`, `_prim_Swamp2Door_FIRE`) carry "fire" in name or data. |
| Orange fire candidates | Android data | `animateddecors/swamp_tikitorches.bdae` (orange `anim_torch_fire`, 15 Swamp instances; AnimatedDecor, ungated, not drawn by this owner). `props/swamp/prop_swamp_corpses.bdae` (bones, no fire). `animateddecors/cs_wallbreak_death_cutscene.bdae` (wall bones, no fire). `cs_swamp_intro_cage.bdae` (no fire). |

Conclusion for the two requested objects:
- **8:03 sealed gate**: the wooden gate is `_prim_SwampDoor`; its blue-violet glow is the fire door's magic (`_prim_SwampDoor_FIRE`), which is violet in the data. Consistent with the glow in the baseline (the closed-door frame already showed a violet glow before this change; the flame mesh was drawn through the normal CharacterVisual path, which is the most likely source. Not isolated by a toggle).
- **7:59 orange wall with bones**: not `_prim_SwampDoor_FIRE` by data (violet). The only orange-fire models in the Swamp are the torches, which are small AnimatedDecor objects the port does not draw at all. The bone pile is the existing decor at the start room. Which object makes the orange wall is **not identified**.

## Implementation

- `features/effects/embedded_scene_fx_v1.{hpp,cpp}` (new): one authored FX resource per attached scene visual. `attach` creates it from the model BDAE through the session's resource factory (a model with no FX family returns a reason). `begin_frame`, `update` (sample at the visual's clip time, typed outer transform, emission clock), `active`, `sources`. `embedded_clip_source_ms_v1` maps the clip (start + min(clock, span)).
- `features/effects/effects_render_bridge.{hpp,cpp}`: `EmbeddedFxSourceV1` and a `prepare` overload. Embedded particle sources and composite mesh sources join the same frame and use the same `convert` contract. Bound materials are cached per resource and material; only the colour is refreshed each frame.
- `runtime_combat_effects_v1`, `runtime_effects_factory_v1`: `create_embedded_fx`, `set_embedded_sources` (list replaced each frame).
- `original_character.{hpp,cpp}`: `active_clip_name()` (the clip name, not its path).
- `level-world/authored_fx_mesh_graph_v32.cpp`: (a) COLLADA `instance_material` symbols resolve to the bound target (`material_symbols_v1`); (b) a plain colour material needs no sampler matrix (only textured effects do); (c) `_colbox_` and `_mesh_shadow_` instances are not drawn (same rule as `objects.cpp`; the colbox quad covered the view before); (d) draw primitives carry the bound material id (the `convert` contract). Skill FX behaviour is unchanged where the symbol equals the id.
- `scene-materials/particle_scene_v1.cpp`: keeps each binding's primitive symbol (best effort, as `scene.cpp` does).
- `main.cpp`: attach/update for doors, levers and gated decor; a visual stepped this frame is **not** drawn by CharacterVisual again (the FX frame owns its meshes); `DH_PERF=1` prints mean per-stage costs every 120 frames.
- Channel filter (`engine-animation/channel_filter.cpp`) unchanged: its log-once behaviour stays for channels that truly target nothing. The FX resource handles the emitter and material channels for scene-owned FX.

## Verification (real EXE, quiet hidden runs, rc3 assets)

- Fire door visible through the FX path: `fx1-fd`, `fx2-fa` (closed, 240 frames). `Embedded FX _prim_SwampDoor_FIRE attached`; 4 FX packets per frame; no presentation failure.
- Baseline vs new at the same spot (x=-12700): `base-fa` vs `fx2-fa`. The violet glow is present in both; the authored particles and flames add a small amount of light. The new flames are not visibly orange (as the data says).
- Gate flip (`fx-open-60`, `fx-open-240`, `--campaign-start open_SwampKingDoor`): `[door] command ... _prim_SwampDoor_FIRE ... state=0->3`. The fire and the violet glow are gone by frame 60; at frame 240 the leaf is open with no fire and no glow. The FX packet count drops (810 total over 240 frames, against 960 for the closed run).
- Contact sheet: `.local-inputs/claude-preview17/firewall-run/contact-sheet.png` (row 1: 7:59 and 8:03 references; row 2: baseline and new at x=-12700; row 3: new at the fire door centre, and new after the flip).
- ctest (`-j 6`, toolchain on PATH): 132 of 134. Failing: `session_skill_binding` (allowed) and `character_menu`. `character_menu` fails identically on the DOORS2 baseline EXE (`build-p17doors2/character_menu_tests.exe`: "Tab overlap left part not answered by the nearer Quest Log"); it is pre-existing and not caused here.

### Frame cost (`DH_PERF=1`, closed fire door, 240 frames, 120-frame means)

| Stage | Cost |
|---|---|
| Embedded block (update + list) | 0.032 to 0.036 ms |
| Prepare (`prepare_render_frame`) | 0.03 to 0.19 ms (was 18.7 ms before the material cache) |
| Queued draw (`draw_queued`) | 0.010 to 0.023 ms |
| Drain (`finish_and_drain`, `glFinish`) | 2.8 to 3.2 ms, only when FX frames are queued |
| Frame rate | 66 fps new, 66 to 67 fps baseline (no measurable change) |

The embedded block and draw are within the few-tenths budget. The drain is above it: `finish_and_drain` calls `glFinish` every frame that has FX packets (shared renderer policy). This is the one cost I could not bring within budget without changing the shared renderer contract.

## Gaps (explicit)

1. **Orange wall of fire (7:59) not reproduced; the object is unidentified.** See Decision needed.
2. **Violet glow vs 8:03 not matched same-frame.** The reference is a cutscene camera; the gameplay camera differs. Colour family and placement are similar. Brightness and extent were not tuned.
3. **Scale of the wall.** The fire door's flames are small at the gameplay camera; the reference wall spans much more of the floor. Not reproduced.
4. **`glFinish` drain of about 2.8 ms per frame** while FX packets are queued (shared policy). A fence-based release would remove it; not done here because it changes the shared renderer contract for skill FX.
5. **Save and reload of scene FX not verified.** Door state is restored by the door runtime; the FX should follow the restored clip, but this was not run.
6. **Decor and torches.** Ungated AnimatedDecor (torches, dust, waterfalls) is still not drawn (the owner reports `ported_elsewhere`). Only gated decor and the doors and levers reach the scene FX path. Most Swamp AnimatedDecor models are also missing from the rc3 package (`swamp_tikitorches`, `prop_swamp_corpses` are in the Android data).
7. **Resource lifetime.** Packets borrow the resource's scene and image pointers until the frame is drained. The resources are cleared with the FX factory (level change or reset). Not stress-tested.
8. **No new unit tests.** Symbol binding, the composite defaults rule and the clip mapping are covered only by the EXE runs.
9. Red door `_prim_Swamp2Door_FIRE` also attaches (`go_door_fire_red.bdae`); it was not inspected visually.

## Decision needed

- Which object draws the orange wall with bones at 7:59? The data says it is not `_prim_SwampDoor_FIRE`. Candidates: the Swamp torches (orange, but small and not drawn by the port), or a cutscene effect. Please confirm with a timestamped frame that shows the object, or accept the violet fire door as the only fire wall in the Swamp.

## Placeholders

- None added. `DH_PERF=1` is a diagnostic switch (default off).

## Package files required

- rc3 package: `assets/data/3d/gameobjects/go_swamp_door_ruins.bdae`, `go_swamp_door_ruins_fire.bdae`, `go_door_fire_red.bdae`; `assets/data/3d/textures/fx_doorfire.tga`, `fx_smoke_03.tga`, `pvr2_env_swamp_alpha.tga`; the game_objects tables and `original-campaign.xml` as in DOORS2.
- Android data only (not in rc3, for decor identification): `data/3d/animateddecors/swamp_tikitorches.bdae`, `data/3d/props/swamp/prop_swamp_corpses.bdae`.

## Verifier script

1. Build: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p17firewall -Jobs 6`.
2. ctest (toolchain on PATH): expect exactly `session_skill_binding` and `character_menu` to fail.
3. Fire door, closed: `quiet_run.ps1 -JobsFile firewall-run/jobs-fx1.json -Parallel 1`; grep `Embedded FX _prim_SwampDoor_FIRE attached` and `Source FX final packetFrames=240`.
4. Gate flip: `quiet_run.ps1 -JobsFile firewall-run/jobs-open.json -Parallel 2`; grep `state=0->3` and compare `cap/fx-open-240.png` with `cap/base-fa.png` (no fire, no glow after the flip).
5. Perf: `$env:DH_PERF='1'`, then `quiet_run.ps1 -JobsFile firewall-run/jobs-perf.json -Parallel 1`; grep `DH_PERF`.
