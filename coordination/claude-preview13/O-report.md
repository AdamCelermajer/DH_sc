# Preview 13 O report (wave 3): B005 BashDown ground impact, emitter decode and colour/size path

Status: **B005 NOT closed. No production change.** Partial decode and IDA evidence only. A full headless reproduction and a size/lifetime comparison were not completed.
Repo HEAD observed as 5db15994 (the brief says f6b7f134; the tree has moved). Scratch: `.local-inputs/claude-preview13/O/` (git-ignored; `tools/dump_bres.py`, `tools/tex_sheet.py`, `bashdown.bdae` copy).

## 1. Evidence

### Logic: the BashDown FX file (decoded from the candidate's 34,420-byte BDAE, SHA-256 21a31d37...e483)

- BRES root (file offset 0x2828). Counts: effect 5, material 5, geometry 3, emitter 2, force 2, animation 29, clip 1.
- Clip: `AUTO_Current_Range` 0..1666 ms (the authored FX timeline is 1.666 s).
- Emitters (0x90-byte records, table at root+0x7c):
  - `gl_pcloud_debris-emitter` (type 0 box, record+0x08 = 0): MaxParticles (+0x18) = 20, BirthRate (+0x20) = 0 (authored BirthRate is driven by the channel `gl_pcloud_debris-emitter_emission_birth_rate`), seed word +0x24 = 12345, descriptor at +0x54 (36 bytes, type 3, flags 0), mode +0x50 = 0. Life-like pair at +0x40/+0x44 = 1500 / 0.
  - `gl_pcloud_dust-emitter` (type 1, record+0x08 = 1): MaxParticles 20, BirthRate 0 (channel `gl_pcloud_dust-emitter_emission_birth_rate`), seed 12345, descriptor type 3, mode 0. Pair at +0x40/+0x44 = 600 / 0.2.
  - The field meanings of +0x28..+0x34, +0x58..+0x78 are **not** confirmed against a model struct. The 333.333 at +0x78 is in both records. Lifetime and size keys are therefore **not determined**.
- Animation channel names in the BDAE (strings): only `*_emission_birth_rate` for the two emitters. **No colour channel name exists** (no "color"/"colour" track for either emitter). The only "color" strings are `__irrlicht_Diffuse_color` on the mesh material and the `DiffuseColor` shader parameter.
- Effects (5): `ProfileCOMMON_Standard_9` (mesh), `asde` (mesh), `fx_particles_alpha_001` and `_002` (particle materials, mat2/mat3 with blend words +0x18 = 2 and 3), `swoosh_MH` (mesh). Mesh sub-effect nodes: `_mesh_cracks_nobatch`, `_mesh_shockwave_nobatch`, `_mesh_swoosh_nobatch`, plus `_fx_warriorbash` (node).
- Textures referenced: `atlas_fx_particles_001.tga`, `atlas_fx_particles_002.tga`, `fx_shockwave_05.tga`, `fx_weapon_trail_skill_gradual.tga`. The `.tga` files in the candidate are PowerVR `BTEX` containers (header `BTEXpvr`), not plain TGA. PIL cannot open them, so their pixel colour was **not** checked here.

### IDA: the original particle colour model (`pseudocode-all.c`)

- `glitch::ps::PColorModel<SParticle>::applyPColor` (0x0064e830). Two animation pointers on the model (`a1[1]`, `a1[2]`):
  - **Both null (else branch):** for each particle, `memcpy(p+24, a4, 4)`, where `a4` is the scene colour word. This is exactly `dh2_particle_color_fallback_v1` (port `engine-animation/particle_cloud_models_v1.cpp:65`). **The port's fallback matches the original null branch.**
  - **Colour track present (`a1[2]`):** the original samples the colour track at `v11/33.333` (age in ms, quantised to 30 fps steps) through `SAnimationAccessor::getValue` and writes the colour word into particle +24. **The port has no colour-track model** (`ParticleCloudModelsV1` has life, size, motion, spin, sphere, gravity only). This is a real gap, but it applies only when a colour track is attached.
  - `a1[1]` (angle track) writes cos/sin rotation terms at particle +28..+48 in the same function.
- `initPColor` (0x0064bf00) consumes no random value when both pointers are null and otherwise draws initial colour parameters. This matches the port comment ("null/null init leaves bytes untouched and consumes no RNG").
- Prior note `port/engine-animation/reference/particle-cloud-models-v1/runtime-integration.md`: the descriptor3/mode0 mixin constructor produces **null Color streams**. The BashDown emitters have descriptor type 3 and mode 0, so they should also take the null branch. This is a note-level inference, not verified in the mixin constructor.

### Our runtime (code, not run)

- `particle_cloud_runtime_v3.cpp:25` runs `dh2_particle_color_fallback_v1` every update, then `dh2_particle_size_apply_v1` (size growth/fade over life is present), so size-over-life exists in our code. Colour-over-life does not.
- `runtime_effects_factory_v1.hpp:24,35` and `runtime_effects_particle_color_policy_tests.cpp` select `source_white` for the authored particle branch.

### Visual (K's captures, viewed by me)

- I viewed `K/ov/ours_118_126_crop.png` (frames f110..f128 from K's isolated runs). The large orange flame arc appears at f122, spans about 4..6 frames, and sweeps from the Knight across the target. At f128 a cracked lattice appears at the feet. The arc is a large, smooth, flame-textured band with spark sprites on top. It is **not** a field of small dust sprites.
- Reference (K and E): a small orange dust puff at the feet at 282.60..282.67 s (about 0.1 s), with no large arc.
- Inference: the large arc is much more likely to be a **mesh** sub-effect (shockwave/swoosh/cracks, on flame textures) than the `gl_pcloud_dust` particle cloud. The particle sprites are only 20 per emitter, and the dust cloud cannot make a band that size. This was **not proven**: I did not isolate the mesh nodes or check texture colours.

## 2. Diagnosis

- **Colour over life is ignored, but it is probably not the cause here.** The original's null branch equals our fallback, and the BDAE has no colour channel for these emitters. Under the usual reading of this data, the dust colour is therefore the texture atlas colour times the scene colour (white on GLES2). The orange would come from the atlas pixels, not from a missing colour key. This is inference from channel names and the null-branch logic. The colour-track gap is real in the code but is **not shown to apply** to BashDown.
- **Size and lifetime:** our code applies size growth/fade, but I did not map the authored size/life fields onto `ParticleSizeModelV1`/`ParticleLifeModelV1` for BashDown. **Not determined.**
- **Spawn rate:** BirthRate is an animation channel (`*_emission_birth_rate`); its keys were **not decoded**.
- **Likely divergence:** the big orange arc at f122..f128 comes from the mesh sub-effects (`_mesh_shockwave_nobatch`, `_mesh_swoosh_nobatch`, `_mesh_cracks_nobatch`). Their scale and timeline versus the reference are the next thing to check. This is an inference and needs the mesh timelines.

## 3. Our output (K, plus this pass)

- Set 164 dispatches at frame 90 (`sequence=347 dispatched=1 sets=164`, K). The orange arc is present at f122..f128 in K's isolated runs. Reference burst at 282.60..282.67 s. Attribution of the reference burst to BashDown is still inference (no HUD/key check).
- No headless WGL reproduction was run by me in this pass.

## 4. Changes

None. No `port/` or `coordination/` file other than this report was edited. Scratch only under `.local-inputs/claude-preview13/O/`.

## 5. Package files required

| file (under `assets/`) | SHA-256 / note | present in candidate |
|---|---|---|
| data/3d/interface/skill_dh2_prince_warrior_bash_down.bdae | 21a31d374a80b9b6f7d1f9b7f273c62459dde9bfd9a2fabfa629fc2fae83a483 | yes (hash checked by K, file size 34,420 confirmed by me) |
| data/3d/textures/atlas_fx_particles_001.tga | not hashed (PVR BTEX) | yes (checked by K) |
| data/3d/textures/atlas_fx_particles_002.tga | not hashed (PVR BTEX), 524,348 B | yes (K; size checked by me) |
| data/3d/textures/fx_shockwave_05.tga | not hashed, 8,252 B | yes (K; size checked by me) |
| data/3d/textures/fx_weapon_trail_skill_gradual.tga | not hashed, 2,108 B | yes (K; size checked by me) |

## 6. Tests

- No code touched; no runner executed.
- Tool runs: `tools/dump_bres.py bashdown.bdae emitter` (2 emitters, decoded fields above); `dump_bres.py ... effect mat clip` (5 effects, 5 materials, clip 0..1666 ms); a string scan of the BDAE; `libs.py` counts (from E) matched.
- `tools/tex_sheet.py` failed on the PVR `BTEX` textures (PIL cannot open them). Not a test result.

## 7. Uncertainties / not verified

- Emitter record field semantics (size model, life model, colour pointers at +0x0c/+0x4c, the 333.333 at +0x78) are not confirmed against a source struct.
- Whether the emitter colour-track pointers are actually null for these records. Inferred from channel names and the descriptor3/mode0 note; not verified in the mixin constructor (IDA `654a74`/`654ec4` not read here).
- Texture colours (orange vs white) not measured: PVR decode needed.
- Whether the f122..f128 arc is a mesh sub-effect or a particle; not isolated.
- Reference BashDown attribution and key timing: unchanged from K/E.
- BirthRate key decode: not done.

## 8. Next steps (for the root or a follow-up worker)

1. Isolate the mesh sub-effects: run the candidate with the particle nodes disabled, or dump the mesh timelines (`_mesh_shockwave_nobatch`, `_mesh_swoosh_nobatch`, `_mesh_cracks_nobatch`) from the animation library and compare their visible window with reference 282.60..282.70 s.
2. Decode the two `*_emission_birth_rate` channel keys and the size/life fields for both emitters, then compare the dust visible span (about 0.1 s in the reference) with the authored life (600 ms, variation 0.2 in the dust record, if that reading holds).
3. Decode the PVR textures (port texture decoder) to measure their RGB; only then decide whether the orange is from atlas pixels.
4. Only if a colour track is proven attached to a BashDown emitter, add the colour-track branch of `applyPColor` (age quantised to 1/30 s) to the port.

## Verifier script

Not applicable: no code change. To check the arc on the candidate: copy the package to a fresh folder, set `--fixed-step .016 --skill-key-frame 90:2 --frames N --capture <out>/frame.ppm` (N = 118..130), and look for the orange arc at about f122 and the crack lattice at about f128. Expected (observed by K): arc present. Expected if the arc is a mesh effect: the particle-only run without mesh nodes shows no arc. Compare with reference 282.60..282.67 s.
