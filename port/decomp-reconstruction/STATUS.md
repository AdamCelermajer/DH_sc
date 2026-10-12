# Decompiled-function reconstruction: status

Updated 2026-10-12 by the integrator, at `41014096`. Totals come from `python3 tools/decomp_progress.py`,
which counts each ledger row once against its category's queue. About 1,570 queued functions have no
ledger row yet; they are still to do.

## Totals

| | functions |
|---|---|
| queued | 15,164 |
| reconstructed | 7,009 |
| upstream (unmodified library code) | 1,796 |
| partial | 2,753 |
| skipped (deliberately not rebuilt) | 2,036 |

Hex-Rays is the primary pseudocode source (`recovered/native/hexrays-all/`). The IDA assembly
listing is in `recovered/native/ida-asm/` for checks (`tools/decomp_asm.py`). Ghidra is the fallback
(`--source ghidra`).

## Per category

| category | queued | reconstructed | upstream | partial | skipped |
|---|---|---|---|---|---|
| core-platform | 653 | 284 | 0 | 82 | 287 |
| glitch-core-io | 1,566 | 951 | 17 | 335 | 235 |
| glitch-scene-collada-gui | 4,354 | 1,279 | 0 | 1,487 | 581 |
| glitch-video | 1,375 | 399 | 0 | 234 | 528 |
| net-session-online | 1,235 | 704 | 0 | 163 | 115 |
| net-transport | 794 | 528 | 0 | 185 | 66 |
| pydata-accessors | 2,201 | 2,082 | 0 | 99 | 20 |
| tinyxml-and-c-libs | 1,846 | 0 | 1,734 | 1 | 58 |
| vox-audio | 1,140 | 782 | 45 | 167 | 146 |

## Decisions in force

- Hex-Rays is the primary source. Ghidra is the fallback only.
- Undefined corners (uninitialised fields, null inputs, reads past a table) stay `partial`.
  That is the no-guessing rule.
- 120 IMaterialParameters Cvt rows (glitch-core-io) stay `partial`. The port refuses type codes 19 and up,
  where the original reads past its table.
- Ghidra's `elf_address` for `FUN_` entries is 0x10000 low. `tools/decomp_categories.py` moves each one
  to the address in its name and drops it when a named entry already sits there.
- Routing matches the function name, not its parameter list (`head(d)` in `tools/decomp_categories.py`).
  A parameter type such as `glitch::io::IIrrXMLReader` no longer moves a `glitch::scene::` or
  `glitch::gui::` function into the core-io queue. The reroute moved 246 core-io queue rows (175 to scene,
  71 to video) and 41 video rows (to scene). The skipped ledger rows for those functions were dropped, so
  their new owners reconstruct them. Two partial rows (the `CColladaBinaryFileLoader` constructors) moved to
  the scene ledger; their code is still in `glitch-core-io/src/collada_binary_file_loader.cpp` until the
  scene category moves it.
- Musepack `mpc_*` queue rows are ledgered as `upstream`, matched by name. The bodies were not
  compared with upstream source. The vox thread later marked four of them (`0x87f28c`, `0x8803d4`,
  `0x8964ec`, `0x8965b4`) `skipped`, since it could not confirm the library function. Those four
  follow the thread.
- Category branches are merged as they are pushed: `glitch-core-video`, `pydata-core`, `vox-audio`,
  `net` and `glitch-scene`. The vox thread keeps its rows in `ledger.parts/`; the integrator
  consolidated them into `ledger.csv`, which is the file the progress tool reads.
- The pydata thread forwarded core-platform memory and touch thunks to their listing targets
  (`0x53419c` checked against `tools/decomp_asm.py`: it branches to `GlitchAlloc`).
- Shared layout header: `port/decomp-reconstruction/shared/include/glitch-shared/glitch_layouts.hpp`
  holds the glitch::core value types (`dimension2d`, `position2d`, `vector3d`, `vector4d`, `aabbox3d`,
  `rect`) and the glitch::video colour words (`SColor`, `SColorf`). Every category adds the shared include
  directory (`CMakeLists.txt`). Categories include it instead of copying a stand-in into `deps/`. The video
  and scene `deps/` headers now include it. Core-io keeps its richer copy with methods.
- `.gitattributes` forces LF on every ledger (`ledger.csv` and `ledger.parts/*.csv`). The scene,
  tinyxml-and-c-libs and vox ledgers were CRLF and were normalized once. No CRLF ledger remains.
- 58 ledger rows at addresses no queue holds were dropped (core-platform 24, net-session-online 34). They
  were ledgered from pre-`FUN_`-fix addresses, and their notes record that the lookup there found nothing.
  HEAD queues were regenerated and checked (`tools/decomp_categories.py` gives no diff).
- The Windows toolchain is installed by `tools/decomp_setup.sh` (see the README Setup section).

## Open

- intrusive_ptr exists in three forms. Core-io's `boost/intrusive_ptr.hpp` counts references
  (`intrusive_ptr_add_ref` and `intrusive_ptr_release`, with `IReferenceCounted`). Video's and scene's
  `deps/intrusive_ptr.hpp` are non-counting stubs. The runtime behaviour differs, so the shared header does
  not lift it. The owning threads decide.
- Core-io's `deps/glitch_core.hpp` has the same `rect` layout as the shared header, with names X1..Y2 and
  methods. Both are visible only in separate translation units so far. An ODR risk remains if one
  translation unit sees both.
- Core-io's stubs for video types (CImage, CLight, CMaterial, CVertexStreams, ITexture, IVideoDriver,
  IBuffer, IShader, IShaderCode, CVideoModeList, IMaterialParameters, IReadFile, SShaderParameterDef), and
  scene's collision with the real `CColladaBinaryFileLoader` in `scene_simple_accessors.hpp`, plus ILogger,
  are ODR and ownership problems across categories. The shared header does not address them.
- core-video's new types, `SIDedCollection` (`glitch-core-io/core/sid_collection.hpp`) and the GL function
  table (`glitch-video/deps/gl_function_table.hpp`), are not yet checked for cross-category need.
- glitch-scene-collada-gui: scene's `layouts.hpp` is a notes file, not a type header, so it is not lifted.
- glitch-scene-collada-gui: 80 generated stubs in `generated_stubs.hpp` are address-named templates, not yet
  attached to their owning classes.
- Two partial `CColladaBinaryFileLoader` constructors (`0x6b9310`, `0x6b9360`) are in the scene ledger,
  but their code is still in `glitch-core-io/src/collada_binary_file_loader.cpp`.
- net-transport: `connection_manager` uses the real `CConnection` since `d14bdc02`, and the stand-in is
  deleted from the code. `net-transport/NOTES.md` still describes the stand-in. The net thread should update
  its NOTES.
- net-session-online: its NOTES say 1,268 queued. The regenerated queue has 1,235. The NOTES count is stale.
- pydata-accessors: `0x4f2750` (`Structs::CharacterProperties::read`) is ledgered but not queued. The
  inventory marks it claimed. It is the one ledger row the progress tool reports as ignored. Three thunks
  forward to it.
- pydata-core branch: its core-platform queue is stale (679 rows against 653 at HEAD). HEAD's queue, as
  regenerated by the tool, is authoritative.
- vox-audio: `ledger.parts/w9-fun.csv` still holds the three `FUN_` rows at pre-fix addresses (`0x89xxxx`).
  `ledger.csv` is the file the progress tool reads, and it has the rows at the corrected addresses. The
  parts file should be deleted or kept in step.
- Thunks reimplemented rather than forwarded (core-platform 5, glitch-video 1, net-session-online 2)
  still need review.

Closed since the last update: the shared layout header; the scene ledger's CRLF line endings; the
`recon_slice_10_test` failure (fixed on the scene branch in `3feb4171`, merged as `d8b52ae2`); the vox
`FUN_008a0364`, `FUN_008a2330` and `FUN_008a7e90` rows (now ledgered in `ledger.csv`); and the net stand-in
(deleted in `d14bdc02`).

## Process

Each category works on its own branch, `feature/decomp-<category>`, cut from
`feature/decomp-reconstruction`. Each touches only its own folder and ledger. The integrator merges
them back as they push, keeps both builds green (`tools/decomp_build.sh host` and `windows`), and runs
the cross-category checks: the thunk check, and the asm spot-checks with `tools/decomp_asm.py`.
