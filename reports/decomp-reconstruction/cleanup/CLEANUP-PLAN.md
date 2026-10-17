# Cleanup / rename pass — plan (2026-10-17)

Inventory (`py -3 tools/trackb/rename_inventory.py` -> `rename-inventory.csv`):

| What | Count |
|---|---:|
| .cpp files under port/decomp-reconstruction | 1,268 |
| hash-named files (`<wave>-<category>-<Class>-<sha6>-<n>.cpp`) | 506 |
| ... of which hold more than one class | 47 |
| `unk_` member references in those files | 2,580 |
| `unk_` field declarations/uses in headers | 681 |

The hash names come from the worker batches (one file per batch slice). They are the main obstacle to reading
and modifying the game code once the faithfulness phase is over.

## Rules

1. **One class, one file.** `g1-game-world-RootSceneNode-e402f1-0.cpp` -> `game-world/src/RootSceneNode.cpp`.
   Slices of the same class (`-0`, `-1`, ...) merge into that one file in slice order; the per-function
   `// 0xADDR _Zmangled` headers stay (they are the link to the ledger and to armdiff).
2. **Mixed files** (47): split by class first, then rule 1. `rename-inventory.csv` lists `class_count`.
3. **Header names** already follow `dh2/<Class>.hpp`; no change.
4. **`unk_NNN` fields**: rename only when the meaning is proven (a writer and a reader that name it, or the original's
   pydata/DWARF name); keep the `// +0xNNN` comment. Never rename on a guess. Each rename is a separate commit
   with the evidence in the message.
5. **Ledger** (`<category>/ledger.csv`, column `source_file`): regenerate from the `// 0xADDR` headers after the moves
   (`tools/trackb/merge_ledger.py` reads parts; add a `--rescan` that maps address -> file).
6. **Build**: CMake globs `src/**/*.cpp`, so moves need no CMake edits; `tests/` names are untouched. Each batch of
   moves = one commit, followed by a full build + serial ctest (853) before the next.
7. **Order**: start with the categories nobody is editing (pydata-accessors, net-transport, core-platform), then
   game-* once the open bug owners (Q6/V3/R5/UI2/PX2) have reported, to avoid colliding with their edits.
8. **Do not** rename during an active owner round in the same category; `git mv` + their edits = conflicts.

## Not part of this pass

Behaviour changes, deviation removals, and the `improvements` branch work. This pass must leave the game
byte-for-byte equivalent (same test results, same armdiff results).
