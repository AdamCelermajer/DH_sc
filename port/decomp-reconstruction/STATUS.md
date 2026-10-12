# Decompiled-function reconstruction: status

Updated 2026-10-12 by the integrator, after the glitch-core-io and glitch-video reroute. Totals come from
`python3 tools/decomp_progress.py`, which counts each ledger row once against its category's queue.

## Totals

| | functions |
|---|---|
| queued | 15,164 |
| reconstructed | 5,287 |
| upstream (unmodified library code) | 1,791 |
| partial | 1,029 |
| skipped (deliberately not rebuilt) | 1,107 |

Hex-Rays is the primary pseudocode source (`recovered/native/hexrays-all/`). The IDA assembly
listing is in `recovered/native/ida-asm/` for checks (`tools/decomp_asm.py`). Ghidra is the fallback
(`--source ghidra`).

## Per category

| category | queued | reconstructed | upstream | partial | skipped |
|---|---|---|---|---|---|
| core-platform | 653 | 259 | 0 | 54 | 116 |
| glitch-core-io | 1,566 | 951 | 17 | 335 | 235 |
| glitch-scene-collada-gui | 4,354 | 1,058 | 0 | 71 | 4 |
| glitch-video | 1,375 | 399 | 0 | 234 | 532 |
| net-session-online | 1,235 | 554 | 0 | 99 | 69 |
| net-transport | 794 | 285 | 0 | 52 | 28 |
| pydata-accessors | 2,201 | 1,329 | 0 | 1 | 0 |
| tinyxml-and-c-libs | 1,846 | 0 | 1,734 | 1 | 58 |
| vox-audio | 1,140 | 452 | 40 | 182 | 65 |

## Decisions in force

- Hex-Rays is the primary source. Ghidra is the fallback only.
- Undefined corners (uninitialised fields, null inputs, reads past a table) stay `partial`.
  That is the no-guessing rule.
- 120 IMaterialParameters Cvt rows (glitch-core-io) stay `partial`. The port refuses type codes 19 and up,
  where the original reads past its table.
- Ghidra's `elf_address` for `FUN_` entries is 0x10000 low. `tools/decomp_categories.py` moves each one
  to the address in its name and drops it when a named entry already sits there.
- Routing matches the function name, not its parameter list. A parameter type such as
  `glitch::io::IIrrXMLReader` no longer moves a `glitch::scene::` or `glitch::gui::` function into the
  core-io queue. The reroute moved 246 core-io queue rows (175 to scene, 71 to video) and 41 video rows
  (to scene). The skipped ledger rows for those functions were dropped, so their new owners reconstruct
  them. Two partial rows (the `CColladaBinaryFileLoader` constructors) moved to the scene ledger; their
  code is still in `glitch-core-io/src/collada_binary_file_loader.cpp` until the scene category moves it.
- Musepack `mpc_*` queue rows are ledgered as `upstream`, matched by name. The bodies were not
  compared with upstream source.
- The Windows toolchain is installed by `tools/decomp_setup.sh` (see the README Setup section).

## Open

- Shared layout header for the glitch categories: requested, not started. The glitch-scene thread is
  blocked on unmapped class layouts.
- vox-audio: three `FUN_` rows with no named twin (`FUN_008a0364`, `FUN_008a2330`, `FUN_008a7e90`).
- pydata-accessors: `0x4f2750` (`Structs::CharacterProperties::read`) is ledgered but not queued. The
  inventory marks it claimed.
- glitch-scene-collada-gui: 80 generated stubs in `generated_stubs.hpp` are address-named templates,
  not yet attached to their owning classes.
- Thunks reimplemented rather than forwarded (core-platform 5, glitch-video 1, net-session-online 2)
  still need review.

## Process

Each category works on its own branch, `feature/decomp-<category>`, cut from
`feature/decomp-reconstruction`. Each touches only its own folder and ledger. The integrator merges
them back as they push, keeps both builds green (`tools/decomp_build.sh host` and `windows`), and runs
the cross-category checks: the thunk check, and the asm spot-checks with `tools/decomp_asm.py`.
