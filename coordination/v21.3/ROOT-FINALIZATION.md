# Root finalization record

The user requests completion of the current bug batch, commits and pushes to their fork, then cleanup of all extra worktrees to recover space. Primary checkout stays; accepted packages, user saves and unique source/evidence stay recoverable. Cleanup execution is pending release verification.

## Current review

Root inspected the independent frozen 4C60 brown-pier captures `gameplay-review/brown-Type2-fixed/f240.png` and `f600.png`, and the original green death sequence `B126/original-liz-death6915-6935.jpg` under `.local-inputs/v21.3`. The brown axe-bearing Champion matches the reported actor and visibly has a red target circle and named HP plate at frame240. At frame600 the adjacent green lizard stands upright while the brown body has fallen. Original green sequence visibly tips backward and remains horizontal. Death identity/timing is supplied by reviewer logs, not inferred from these stills alone. B127 still needs older-save compatibility; B126 needs the death-pose repair and normal sequence verification.

Audio reviewer accepts B119–B121 source-faithful callback/output repairs on frozen 4C60, including exact PickupGold token and nonzero final WinMM PCM. This is device-bound output evidence; no acoustic comparison is claimed. Final combined regression and B122 active-caller classification remain pending.

## Release and cleanup order

1. Finish named bug implementations and independent frozen production checks, including the shared post-death loot, Equipment and potion failures. Commit integration changes and merge reviewed source into the primary checkout.
2. Run the relevant combined checks, package and verify the accepted candidate. Preserve previous packages and copy user progress separately with hashes.
3. Commit reports, source and cleanup receipts; push to `publish` (AdamCelermajer fork). Preserve every extra worktree HEAD and every unique dirty/untracked source change in recoverable refs/snapshots before removing a checkout. Do not push to the Noam `origin` remote.
4. Recheck the 67-worktree inventory immediately before cleanup. Preserve the primary checkout and snapshot the dirty `generic-level-loader` checkout. Archive the two attached managed crash worktrees through the app; remove other extra checkouts only after absolute target checks and preservation. Active integration stays until its work has been accepted.
5. Remove obsolete reproducible build outputs only after checking references and preserving needed frozen evidence. Record disk space before/after, removed paths and retained snapshots. The existing inventory is read-only preparation, not a cleanup completion receipt.

The verified duplicate-capture cleanup receipt `.local-inputs/v21.3/verified-raw-capture-cleanup.json` records 27 pixel-equivalent raw PPM duplicates removed (67,789,869 bytes). The later disk-space increase was larger and its external cause is unknown; it is not counted as space recovered by this cleanup.
