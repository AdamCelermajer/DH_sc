# General Windows game foundation

This is a separate native Windows C++17 runtime for the whole game's foundations.
It reuses recovered content decoders without the Android application, JNI,
original callback graph, or original ARM32 engine. Maps and characters are inputs;
the runtime contains no Act 1 campaign route or Swamp-specific game rules.

The files in `reports/preview-assets.json` and `preview-launch.json` select original
content for validation. They are sample data, not architectural restrictions.

## Current checkpoint

- Windows x64 EXE, Win32 window, WGL, input, resize and clean context destruction.
- Original BDAE meshes, textures, static scenes and generic XML module placement.
- Original skeletal animation and CPU skinning with arbitrary named clips,
  loop/one-shot playback, optional template pose and configurable equipment selection.
- Camera movement and a timeline mechanism with interpolation and cuts.
- Character identity, stats, XP, inventory, equipment, skills and unlocks in a
  versioned save; malformed loads preserve current data, saves replace atomically.
- Stable world object IDs and separate temporary character action state.
- Explicit asset selection, portable packaging and reproducible validation.

Six CTest checks pass: saves, camera, assets, XML placement, original character
animation and native window resize/reopen. GPU captures demonstrate original
textured world/character display. Separate processes verified save restoration.

This is a foundation preview, not completed gameplay or a complete renderer.
Still required: authored effect/shader coverage, audio, original cinematic track
integration, collision/navigation, combat/AI, UI, quest/world persistence and
campaign systems. The renderer currently uses OpenGL compatibility drawing,
with diffuse textures, vertex tint, alpha cutouts, additive blending and sorted
transparent ranges. Separate alpha maps are composed for static scene previews;
character alpha maps and global ordering across independent transparent meshes
remain limitations. No complete original lighting/skybox/effect parity is claimed.

## Build

Run `tools/build.ps1` from PowerShell. It accepts `-Compiler`, `-CMake`, `-Ninja`
and `-Jobs`, or discovers the portable compiler in `.local-inputs/windows-toolchain`
and the installed Android SDK's CMake/Ninja tools. It builds a native Windows
application; Android is not required to run it. Output is
`.local-inputs/windows-foundation-build/dh-foundation.exe` at the repository root.

The local portable LLVM-MinGW acquisition is recorded in `reports/toolchain.json`.
The tools are local build inputs; they are not embedded in the game.

## Content and launch

`tools/prepare_assets.py` requires an explicit JSON selection and an original
cache source. It writes only selected files and a hash manifest into a new folder.
Use the supplied validation selections, or supply any other recovered content:

```powershell
python tools/prepare_assets.py --cache ORIGINAL_CACHE.zip --staged ORIGINAL_ASSETS --output OUTPUT_ASSETS --manifest reports/preview-assets.json
```

Launch the EXE with `--assets DIR` and either `--scene RELATIVE_BDAE`, optionally
`--module NODE`, or `--level RELATIVE_MLX`. Character model, template, named clips
and skin selector are also supplied through options. Run `--help` for details.
The EXE does not silently select a map when no scene is supplied.

Controls: WASD/QE move the development camera; arrows rotate; R reloads content; T toggles the demo
timeline; 1/2/3 select preview idle/walk/attack names; F5 saves; F9 restores;
Escape exits. Character movement and combat are not implemented by these keys.

The portable package in `.local-inputs/windows-foundation-preview` contains
`start-preview.cmd`, `world-preview.cmd` and `character-preview.cmd`. These read the content preset
and launch the same executable. The package can be copied to another folder.

## Verification

`tools/verify_preview.py` takes an EXE, assets, explicit launch preset and output
folder. It verifies separate-process save restoration, bounded GPU preview runs,
scene/character reload and camera captures before/after a cut. PPM captures and JSON/log receipts are
retained so pixel output and reached failures can be inspected.

Module helpers are filtered by generic original export roles, not map names;
module packing transforms are replaced by authored level placements. Both Swamp
and Crypt resources were decoded. Conditional modules are reported and skipped
until a campaign evaluator is connected. Gameplay objects, lights and skyboxes
are not synthesized from a geometry-only preview.

Existing Android and compatibility work is preserved. Original content and
third-party components retain the repository's provenance and notices.
