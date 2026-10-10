# Preview 13 K report (wave 2): B005 BashDown ground impact

Status: **B005 NOT closed. No production change.** The earlier "no orange burst" conclusion (E-report, verifier) was drawn from a capture window that ended before the BashDown impact. In a complete window, our candidate does draw orange/yellow impact FX, but it does not match the reference burst in size, placement or timing, and the reference burst is not tied to a key press. B041 was not re-worked (see section 6).

Scratch: `.local-inputs/claude-preview13/K/` (git-ignored; `pkg/` is my own copy of the candidate, `runs/` holds captures, `png/` converted frames, `ov/` contact sheets, `scripts/run.sh` runner).

## 1. Reference (v1.0.3, 640x360, 30 fps, frame n at t0+(n-1)/30)

Observed by me on full frames and Knight crops (`K/ov/s30_282_283.png`, `K/ov/crop_knight_2.png`):
- **282.07..282.10 s (k_003..k_004):** pale blue translucent arc/ring around the Knight, wider than the blade, with the sword mid-swing. The E report did not mention this; its "no blue ring" statement covered only 282.6 s onward.
- **282.60..282.63 s (k_019..k_020):** orange/yellow dust burst at the Knight's front feet, expanding.
- **282.67 s (k_021):** yellow glow fading to red. **282.70..282.80 s (k_022..k_024):** red floor patch under the Knight; it persists.
- Target is a Bogwomp with a lure ring. Map is the Act 1 tile floor, not the swamp map we use.
- Not checked: the HUD cooldown circles and the pressed key. The skill identity is therefore still unknown (inference only).

## 2. What the authored FX set contains (data)

- Set 164 `skill_dh2_prince_warrior_bash_down`, exact file `assets/data/3d/interface/skill_dh2_prince_warrior_bash_down.bdae`, 34,420 B, SHA-256 `21a31d374a80b9b6f7d1f9b7f273c62459dde9bfd9a2fabfa629fc2fae83a483`. Verified in the candidate package (`.local-inputs/windows-source-clock-v19-preview-13-candidate/`). The 15,528-byte file under `assets/animations/` is the animation decoy and is not the effect.
- Named nodes/textures inside it (strings): `_mesh_cracks_nobatch`, `_mesh_shockwave_nobatch`, `_mesh_swoosh_nobatch`, `_fx_warriorbash`, emitters `gl_pcloud_dust` and `gl_pcloud_debris`, textures `atlas_fx_particles_001.tga`, `atlas_fx_particles_002.tga`, `fx_shockwave_05.tga`, `fx_weapon_trail_skill_gradual.tga`. All four textures are present in the candidate under `assets/data/3d/textures/`.
- The dust is therefore a particle emitter on the atlas textures. The orange colour, if authored, would be in the atlas pixels or in an emitter colour key. I did not decode the emitter colour keys. The BRES emitter record layout was not parsed here, so the emitter colour is **not determined**.
- Colour path in code: `particle_scene_color_v1` (port/engine-animation) returns white for driver `&7==0`, which is the GLES2 path, and `source_white` selects that. `dh2_particle_color_fallback_v1` (particle_cloud_models_v1.cpp:65) then writes the scene colour into every particle colour word, and `ParticleCloudRuntimeV3::update` has no colour-over-life step. So any per-emitter colour keys are not applied on our path. Whether the original applies them after the fallback is **not verified** (no IDA check done in this pass). This is a candidate gap for the dust colour, not a confirmed one.

## 3. Our candidate, complete windows (isolated runs of my copy of the preview-13 candidate)

Runner: `K/scripts/run.sh <name> <frames> "90:2"` (fixed-step .016, `--skill-key-frame 90:2`, `--capture` writes the last frame, one process per frame, about 6 s each). Log line at frame 90: `Source player step FX frame=90 ... sequence=347 step=0 occurrence=1 dispatched=1 sets=164`. So the set **is** dispatched in the candidate.

Frames 100..140 in 2-frame steps (`K/ov/ours_100_140.png`, `K/ov/ours_118_126_crop.png`):
- f122 (about 32 frames after the key): an orange arc begins at the Knight's front.
- f124..f126: a large bright orange-yellow fire arc with spark sprites sweeps across the target area.
- f128..f138: an orange-yellow crack/lattice pattern spreads on the floor at the Knight's feet, with a red glow.
- f140: lattice still visible.

Versus the reference: the reference burst is small (about 3 frames, about 0.1 s), sits at the feet, and has no large arc. Ours is much larger and longer, about 0.4 s or more, and the arc is the dominant element. The two scenes differ (map, enemy, camera), so I did not measure a pixel match; the comparison is qualitative and at sheet resolution.

Timing: our impact-like burst starts about 32 frames after the key. The reference's burst starts 0.53 s after its blue arc, but the key-press time in the reference is unknown, so no timing match is claimed.

Verifier window: the verifier's frames 90..112 (verify-swoosh) end before the burst begins at about f122. Their "no orange burst" result is therefore a window artifact, not evidence of absence. This also means their blue-only check was not a fair test of the burst.

## 4. Diagnosis

- The report's question (why no orange) is answered by the window: the burst exists in the candidate but after frame 112.
- Whether our large arc is the correct authored shockwave/swoosh with the wrong scale or timing, or is a different set, is **not determined** here. No evidence supports changing `features/effects/*`. I did not make any change to production code, and no regression test was added because no defect was proven.
- The emitter colour gap in section 2 is a candidate only. It needs an IDA check of the particle colour path (`CNew::onAnimate` / particle colour) before any change.

## 5. Package files required

| file (under `assets/`) | SHA-256 | present in candidate |
|---|---|---|
| data/3d/interface/skill_dh2_prince_warrior_bash_down.bdae | 21a31d37...e483 | yes, hash matches |
| data/3d/textures/atlas_fx_particles_001.tga | not hashed | yes |
| data/3d/textures/atlas_fx_particles_002.tga | not hashed | yes |
| data/3d/textures/fx_shockwave_05.tga | not hashed | yes |
| data/3d/textures/fx_weapon_trail_skill_gradual.tga | not hashed | yes |

Candidate EXE used for captures: SHA-256 `AA4442AD...` (matches the brief). The candidate folder was not modified.

## 6. B041 (combo_03 trail length)

Not run in this pass. E's proposed probe needs a forced dispatch of combo_03 through the Session, which is not cheap; no change made.

## 7. Verifier script (for the root/verifier)

Reproduce the complete window on the integrated EXE:
1. Copy the package to a fresh folder (do not write into the candidate or live game folders). Put the saves `K/scripts/gameplay.orig.save` and `character.orig.save` (copies of the verifier's test saves) into that folder as `gameplay.save` and `character.save`.
2. Write a config file next to `swamp.args` with `--fixed-step .016`, `--frames N`, `--capture <out>/frame.ppm`, `--skill-key-frame 90:2`, then run `dh-foundation.exe --startup-config <cfg>`. Use N in 110..140 (one process per N).
3. Expected log line: `Source player step FX frame=90 ... sequence=347 step=0 ... dispatched=1 sets=164`.
4. Expected frames: no orange at the Knight's feet before about f120; an orange arc from about f122; a large orange/yellow arc at f124..f126; a lattice at the feet at f128 onward. Judge orange by colour, not blue-only.
5. Reference to compare: v1.0.3 frames 282.60..282.67 s (small orange burst at the feet, about 0.1 s). A match on size and duration is not established.

## 8. Uncertainties / not verified

- Skill identity and key press in the reference (HUD cooldowns not checked).
- Whether the reference burst is BashDown's impact at all (inference).
- Emitter colour keys in the BDAE (not decoded), and whether the original applies colour keys after the scene-colour fallback (no IDA check).
- Pixel-level comparison (different maps; sheet-scale visual only).
- Live game not run; only my isolated copy.
