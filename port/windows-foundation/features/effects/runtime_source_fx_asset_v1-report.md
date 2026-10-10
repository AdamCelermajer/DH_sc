# B005 strict source FX asset lookup

## Reproduction and evidence

The frozen production reproduction is `.local-inputs/v19-frontend-hotfix/knight-step-fx/dh-foundation.exe`, SHA-256 `227F229B81592D550C9A5684C8A6042F6FCC2E396882EEBFE4E8C7DD83186D78`, with `knight-active28.args` and `knight-active28.log`. At frame 20 the normal PC2 path selects Knight slot 0 BashDown and enters AnimTable root 347, step 0, occurrence 4. The log reports `dispatched=0`, `packetFrames=0`, and the generic anchored `PlayAnimFXSet` failure. The accompanying `knight-bashdown28.ppm` still shows the pause/continue overlay over gameplay; it does not demonstrate the expected effect or identify the older v1.0.3 observed flash. Existing visual review remains limited to the v1.0.3 source frames at 4:41.995, 4:42.495, 4:43.278, and 4:43.995 recorded in `warrior-bashdown-fx-evidence.md`; those frames do not identify the skill, so that association remains an inference.

The recovered content mapping is explicit: SkillTable row 7 selects AnimTable root 347 (`Knight_BashDown`), whose step 0 names FX set 164. Set 164 requests the complete URI `data/3D/interface/skill_dh2_prince_warrior_bash_down.bdae`. The actual 34,420-byte effect is `port/level-world/reference/shared-target-facing-v1/cache/skill_dh2_prince_warrior_bash_down.bdae`, SHA-256 `21a31d374a80b9b6f7d1f9b7f273c62459dde9bfd9a2fabfa629fc2fae83a483`. A distinct same-basename animation clip is `port/level-world/reference/shared-target-facing-v1/cache/animations/skill_dh2_prince_warrior_bash_down.bdae`, 15,528 bytes, SHA-256 `087068efba8eb4626510292c4cf9257e119e209965e7ab591e5250ea2609be0b`.

The gap is the generic resolver in `port/windows-foundation/content_paths.cpp`: after exact and `original-cache` candidates, `.bdae` paths add basename-only `animations/` candidates. `RuntimeEffectsFactoryV1::Impl::read_asset` in `runtime_effects_factory_v1.cpp` calls that generic `read_content` path for FX requests. When the canonical interface effect is absent from the production asset root, the generic search can return the animation clip instead. `load_particle_scene_v1` in `port/scene-materials/particle_scene_v1.cpp` then correctly rejects that clip with “Particle scene has no authored nodes.” The source effect's authored graph includes `_mesh_cracks_nobatch`, `_mesh_shockwave_nobatch`, `_mesh_swoosh_nobatch`, debris, and dust emitters; no nodes or assets are synthesized here.

## Fix and focused verification

`runtime_source_fx_asset_v1.cpp` adds an effects-owned strict reader for `data/3D/interface/` URIs. It tries only the exact normalized URI and `original-cache/` plus that full URI, under `AssetCatalog` root containment. It never searches `models/`, `animations/`, or basename-only candidates. Non-interface URIs are not handled by this reader, leaving their existing type-specific resolution to the caller. An optional SHA-256 verifier supports pinned source checks without hardcoding one asset into the general resolver.

The focused test was designed to exercise the real source collision and malformed-resource branches: resolve the canonical URI with both same-basename resources present; resolve the full-path `original-cache/` case; reject resolution when only the animation basename exists; reject the animation bytes against the effect digest; and parse the selected actual effect with `dh2_bres_open` and `load_particle_scene_v1`, requiring authored graph nodes and mesh instances.

Run with:

```powershell
.local-inputs/ida-ghidra-review/venv/Scripts/python.exe port/windows-foundation/features/effects/run_runtime_source_fx_asset_v1_tests.py
```

Result: **PASS**. The exact effect hash and 34,420-byte length match; BRES and the particle-scene projection return authored nodes and mesh instances; the same-basename animation fallback is rejected when the exact path is absent; the wrong hash is rejected; and the full original-cache/full-URI candidate passes. Test artifacts and build/run logs are under `.local-inputs/runtime-source-fx-asset-v1/`.

## Integration handoff and limits

Production callback owner is `RuntimeEffectsFactoryV1::Impl::read_asset` at `runtime_effects_factory_v1.cpp`; the current factory creates that callback at `MeshFxAssetsV1 assets{impl.get(), Impl::read_asset}`. The lead can route interface FX URIs through `read_runtime_source_fx_asset_v1` there (or pass the reader from `main.cpp::bindSourceEffects`) and preserve existing `read_content` behavior for non-interface resources. The new resolver is not yet wired into the frozen executable. The normal source capture, visible FX, and post-fix same-session renderer packet receipt remain integration checks; this helper pass is not a B005 closure or a visual-parity claim.
