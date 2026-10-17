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

## Progress and lessons (2026-10-17, round 2)

* Round 1 (afe43ad1): net-transport, net-session-online, glitch-scene-collada-gui groups.
* Round 2: game-anim (AnimApplicator, AnimController, AnimSetController, AnimSetManager, AnimationSet, Animator,
  AnimatorBlender, AnimatorSet, AnimatorSynchronizedBlender, BlendedAnimSetController, CharAnimator [3 slices, manual
  dedup of `AnimTableRecord`]) and game-items-save (ConditionData, ItemManager, Objective, Objective_Automatic, Objective_EventReceiver, Objective_GatherLoot, PropertyMap, Reward, RewardList, Savegame, TrophyManager, UserProperties).
* `tools/trackb/merge_slices.py` fixes: a single slice is a pure rename (its text is kept byte for byte); extra includes of
  later slices are inserted after the first slice's last TOP-LEVEL include (never inside an `#if` block: the first include
  of several files sits in an `#ifdef` debug block and hoisting into it broke the compile); the check_tu result is read
  from the `OK ` line (compiler warnings precede it); `git rm -f` for slices with staged edits.
* `tools/trackb/ledger_rescan.py --category X --apply`: re-points `source_file` by the `// 0xADDR` headers; rows without a
  header (compiler-generated members, "Not written") follow their old file when all of its headered rows went to one
  new file. Totals (`tools/decomp_progress.py`) are unchanged by the pass: 27,467 / 19,913 reconstructed.
* **Rule 9 (new). A test that stands in for a sibling function of the same class blocks the merge.** The tests link
  against the whole library with `--allow-multiple-definition` and rely on the stand-in replacing the real function; once
  the caller and the callee are in one translation unit the compiler binds (or inlines) the call directly and the stand-in
  is never reached. Broken by the merge and therefore kept as slices: `AnimatedFX` (r2a-game-anim-pack0-392f84-0_test
  stands in for SetSpeed) and `LevelSavegame` (g1-game-items-save-LevelSavegame-0b011d-1_test stands in for the
  checkpoint-name builders), and `ObjectiveList`, `PlayerStatManager`, `SavegameManager` (their `-1_test`s segfault once
  the real sibling runs on the test's zero-filled fixture). Merge these only together with a rewrite of the test (test the
  real sibling or split the helper into its own file).
* Still split, waiting for a manual dedup of file-local helpers: game-items-save ItemInstance, ItemInventory (6 slices),
  ItemObject, PlayerSavegame (6), Quest, QuestSavegame; vox-audio VoxSoundManager. gameswf (67 hash-named files) is not
  covered by the inventory's class regex (lower-case class names) and needs its own pass.
