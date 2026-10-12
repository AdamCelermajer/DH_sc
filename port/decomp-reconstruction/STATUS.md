# Decompiled-function reconstruction: status

Updated 2026-10-12 by the integrator, after the glitch-core-io and glitch-video reroute. Totals come from
`python3 tools/decomp_progress.py`, which counts each ledger row once against its category's queue.

## Totals

| | functions |
|---|---|
| queued | 15,164 |
| reconstructed | 5,676 |
| upstream (unmodified library code) | 1,792 |
| partial | 2,390 |
| skipped (deliberately not rebuilt) | 1,754 |

Hex-Rays is the primary pseudocode source (`recovered/native/hexrays-all/`). The IDA assembly
listing is in `recovered/native/ida-asm/` for checks (`tools/decomp_asm.py`). Ghidra is the fallback
(`--source ghidra`).

## Per category

| category | queued | reconstructed | upstream | partial | skipped |
|---|---|---|---|---|---|
| core-platform | 653 | 259 | 0 | 54 | 116 |
| glitch-core-io | 1,566 | 951 | 17 | 335 | 235 |
| glitch-scene-collada-gui | 4,354 | 1,277 | 0 | 1,301 | 555 |
| glitch-video | 1,375 | 399 | 0 | 234 | 532 |
| net-session-online | 1,235 | 554 | 0 | 99 | 69 |
| net-transport | 794 | 285 | 0 | 52 | 28 |
| pydata-accessors | 2,201 | 1,329 | 0 | 1 | 0 |
| tinyxml-and-c-libs | 1,846 | 0 | 1,734 | 1 | 58 |
| vox-audio | 1,140 | 622 | 41 | 313 | 161 |

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
  compared with upstream source. The vox thread later marked four of them (`0x87f28c`, `0x8803d4`,
  `0x8964ec`, `0x8965b4`) `skipped`, since it could not confirm the library function. Those four
  follow the thread.
- Category branches are merged as they are pushed: `glitch-core-video`, `pydata-core`, `vox-audio`,
  `net` and `glitch-scene`. The vox thread keeps its rows in `ledger.parts/`; the integrator
  consolidated them into `ledger.csv`, which is the file the progress tool reads.
- The net thread's queue on its branch is a snapshot from before the `FUN_` address fix. HEAD keeps the
  regenerated queue (1,235 rows, not 1,268). Its NOTES still say 1,268, and that count is stale.
- The pydata thread forwarded core-platform memory and touch thunks to their listing targets
  (`0x53419c` checked against `tools/decomp_asm.py`: it branches to `GlitchAlloc`).
- The Windows toolchain is installed by `tools/decomp_setup.sh` (see the README Setup section).

## Open

- Shared layout header for the glitch categories: requested, not started. The glitch-scene thread is
  blocked on unmapped class layouts.
- vox-audio: three `FUN_` rows with no named twin (`FUN_008a0364`, `FUN_008a2330`, `FUN_008a7e90`),
  so `ledger.csv` has 1,137 of the 1,140 queued rows.
- net-session-online and net-transport: the threads are still working. Their branch code is merged, but
  `ledger.csv` is ledgered only as far as the branch snapshot.
- net-transport: `connection_manager` still runs on a stand-in class, `CConnectionStandIn`
  (`deps/connection_deps.hpp`). The real `CConnection` (`connection.hpp`) has another layout and lacks the
  accessors the manager calls (`State`, `MemberId`, `NetworkId`, `Key`, `SetTimeout`). The two classes
  cannot share a name: the first merge linked the stand-in's virtuals to the real methods and corrupted
  the heap. The integrator renamed the stand-in so the library links. The net thread has to reconcile the
  manager with the real class.
- Two partial `CColladaBinaryFileLoader` constructors (`0x6b9310`, `0x6b9360`) are in the scene ledger,
  but their code is still in `glitch-core-io/src/collada_binary_file_loader.cpp`.
- glitch-scene-collada-gui: `recon_slice_10_test` fails on the branch's own code, the same on host and
  in the branch tip. Line 144 expects `quaternion[3] == 41.0f` for a quaternion-angle key read with a
  default; the code returns another value. The host build is red on this one test (279 of 280 pass).
  The scene thread has to decide whether the code or the expectation is wrong.
- The scene ledger on the branch is CRLF. Integrator merges keep that line ending.
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
