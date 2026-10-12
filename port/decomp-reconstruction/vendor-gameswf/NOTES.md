# vendor-gameswf: GameSWF r1714 in libDungeonHunter2.so

Queue: `queue.csv`, 2370 rows. The integrator moved 101 `RenderFX` rows to `game-ui` and 3 `glitch::` rows to the glitch categories.

## Ledger: 1397 rows, status `upstream`

- `ledger.d/upstream-1.csv`: 1395 unclaimed exact name matches inside the size gate.
- `ledger.d/upstream-2.csv`: 2 claimed exact name matches inside the size gate.

Size gate: bytes per source token within 4x of the median (1.85). `source_file` is under `upstream/gameswf-r1714/`, usually a header with inline bodies or a unit in the copied set.

## Left for reconstruction or review: 973 rows

| Group | Unclaimed | Claimed | Status |
|---|---:|---:|---|
| Absent from the vendored tree (Gameloft or another revision) | 277 | 7 | reconstruct |
| Glitch and scene backend (`render_handler_glitch`, `scene_node`, `texture_cache`) | 99 | 1 | reconstruct, belongs with glitch categories |
| Constructors and destructors with no source body | 142 | 0 | review: `upstream` only if compiler-generated |
| Namespace fallback (binary `gameswf::`, source global or `utf8::`/`image::`) | 34 | 0 | review |
| Render-handler name collisions (OGL, D3D, Xbox names, glitch signatures) | 21 | 0 | reconstruct, do not ledger upstream |
| Outside the size gate (flagged) | 210 | 1 | review |
| Tiny bodies (under 8 bytes) | 181 | 0 | not ledgered (tiny rule is for Box2D only) |

Totals: unclaimed 277 + 99 + 142 + 34 + 21 + 210 + 181 = 964; claimed 7 + 1 + 1 = 9; overall 973.

## Vendored source: `upstream/gameswf-r1714/`

- 170 files: 68 units from `gameswf_sources.cmake` that have a compile unit in the DH2 build, plus 102 headers from their `#include "..."` closure.
- Not copied: the 27 cmake units with no DH2 compile unit, the Track A overlays, and the six Gameloft units absent from the tree.
- No DH2 changes applied. This is upstream as found, so it may include unmarked portability edits from Track A (the `reference/gameswf-core` fork patches are not in the tree).
- Details and build notes: `upstream/gameswf-r1714/README.md`.

## Build needs (not added to CMake)

- 68 units, C++17, include directory = `upstream/gameswf-r1714/` (system).
- Definitions `TU_CONFIG_LINK_TO_JPEGLIB=0 TU_CONFIG_LINK_TO_LIBPNG=0 TU_CONFIG_LINK_TO_FREETYPE=0 TU_CONFIG_LINK_TO_THREAD=0`. Flags `-w -fno-fast-math -ffp-contract=off` (plus `-fpermissive` on GCC). Link zlib (`zlib_adapter.cpp`).
- The game must supply the render handler, sound and file hooks (the glitch backend is not in the tree). The subset does not link on its own.
