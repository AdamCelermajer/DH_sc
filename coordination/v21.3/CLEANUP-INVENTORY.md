# Cleanup inventory (read-only)

Captured 2026-10-12 for post-release cleanup planning. No worktree was changed, archived, removed, committed, or recursively scanned.

## Space and scope

C: had 39,096,467,456 bytes free (~36.4 GiB / 39.1 GB). The primary checkout's `.local-inputs` contained 11,557,352,340 bytes in files directly at that directory level, plus 917 immediate subdirectories; this is not a recursive total. Keep the whole location intact: it contains accepted Preview 11/12/13 packages, user saves, test evidence, source backups, and build caches. The inventory JSON records the measured immediate-file lower bounds for the 25 largest immediate `.local-inputs` subdirectories. Those are investigation candidates only, not deletion recommendations. Directory descendants were not traversed.

Large immediate-file entries include `ida-ghidra-review`, `windows-foundation-build`, `emulator-guards-v36`, `windows-toolchain`, `font-text-discovery`, `b117-build`, and `runtime-test-20261009`. Their contents may hold source evidence or frozen test results; check their consumers and accepted-build links before any cleanup.

## Preserve first

- Primary `DH_sc` stays. It has 1 tracked edit (`docs/BUGS-AND-IMPLEMENTATION.md`) and 19 untracked coordination reports, including the current v21.3 evidence. Preserve the user's tracker additions and reports.
- Active integration `DH_wt/v201int` stays until v21.3 release is finished. It has 13 tracked edits and 2 untracked persistence migration files.
- `generic-level-loader` is another chat's managed worktree with 893 tracked edits and 1,357 untracked paths, including extensive native engine source, tests, tools, and reports. Do not archive or delete it as routine cleanup. Its owner must first snapshot the source and branch into a recoverable location; confirm the snapshot includes untracked files and any ignored evidence/builds.
- `b117-chest-crash` and `b118-continue-crash` are the two task-attached app-managed worktrees. Root handles their app archive operations after release. They are clean at this capture.
- Accepted package directories and user saves under `.local-inputs` remain preserved.


## Reparse points and ignored data

A bounded follow-up checked only immediate entries in each worktree and immediate children of `.local-inputs`. It found 61 top-level `.local-inputs` junctions in extra worktrees; all target `C:\Users\adamc\Desktop\workspace\DH_sc\.local-inputs`. The primary `.local-inputs` is a regular ignored directory, not a reparse point, and its 4,957 immediate entries had no reparse points. Every worktree’s ignored top-level entry is `.local-inputs` under `.gitignore` rule `/.local-inputs/`. During cleanup, remove a worktree junction itself without traversing its target; a recursive operation that follows it could enter the primary accepted packages, saves, source captures, or evidence.

Three extra worktrees have no `.local-inputs`: `b118-continue-crash`, `generic-level-loader`, and `v20rene`. Two have their own ignored local data: `b117-chest-crash/.local-inputs/b117/baseline`, and `npc-marker-motion/.local-inputs/npc-marker-motion/runtime` plus `npc-marker-motion/.local-inputs/windows-foundation-build`. The latter immediate build directory contains `dh-foundation.exe` (20,019,712 bytes), build metadata, and libraries. Preserve these local entries as worktree-owned test/runtime artifacts until root rechecks them. No recursive contents were scanned.

The primary ignored `.local-inputs` includes 941 immediate entries with names matching package, preview, save, source, evidence, campaign, release, frozen, build, executable, or zip markers. The machine safety inventory lists these absolute paths for review. Confirmed accepted packages are `windows-source-clock-v19-preview-11`, `-12`, and `-13`; all three contain frozen `dh-foundation.exe` files (15,079,936; 15,114,752; 15,150,592 bytes) and package receipts. Preview 11 also contains `character.save` and `gameplay.save`. Preserve these packages, plus `.local-inputs/dh2-save-slot0-backup.bin` and `dh2-save-slot0-current.bin` (9,249 bytes each). Keep all remaining evidence/source captures in `.local-inputs` unless root verifies each item is obsolete and recoverable.

See [CLEANUP-SAFETY.json](CLEANUP-SAFETY.json) for the full immediate-entry target map and artifact candidates. The earlier `CLEANUP-SAFETY.jsonl` is a short pointer to that compact inventory.

## Git reachability notes

The 67 registered worktrees break down as 64 clean and 3 dirty. The dirty counts are recorded per tree in the table and JSON. The root branch tracks cached `publish/windows-foundation` and is 1 commit ahead / 0 behind at this capture. Most feature worktree branches have no upstream configured, so a zero upstream-ahead count must not be inferred. No fetch/network check was made. Cached local refs not merged into `windows-foundation` include `codex/b112`, `codex/b114`, `codex/b115`, `codex/b117-chest-crash`, `codex/v20-death-flow`, `codex/v20-gate-teleport-fx`, `fix/b058`, `fix/b059`, `fix/b068`, `fix/b079`, `fix/b092`, `fix/b094`, `fix/b101-b105`, `p15/levelupfx`, `p16/fire2`, `p16/levelup3`, `p17/fire3`, `v20.1/integrate`, and `v20/rene`. Retain branches or cherry-pick/publish required work before unregistering their worktrees.

After release, prioritize identifying and removing obsolete per-worktree build outputs only after confirming each output is reproducible and not the frozen executable/test evidence for a report. Use the JSON's absolute paths and recheck status, ignored data, branch reachability, and save/package ownership immediately before any cleanup. `.local-inputs` remains protected as a whole in this plan.

## Per-worktree status
| Worktree | Branch | HEAD | Tracked edits | Untracked | Handling |
|---|---|---|---:|---:|---|
| `C:/Users/adamc/Desktop/workspace/DH_sc` | `windows-foundation` | `d948811938` | 1 | 19 | clean |
| `C:/Users/adamc/.codex/worktrees/b117-chest-crash/DH_sc` | `codex/b117-chest-crash` | `07854c9696` | 0 | 0 | task-attached-preserve-until-root-app-archive |
| `C:/Users/adamc/.codex/worktrees/b118-continue-crash/DH_sc` | `(detached)` | `18e0a3e58f` | 0 | 0 | task-attached-preserve-until-root-app-archive |
| `C:/Users/adamc/.codex/worktrees/generic-level-loader/DH_sc` | `(detached)` | `45c5348e80` | 893 | 1357 | other-chat-owned-preserve-snapshot-before-cleanup |
| `C:/Users/adamc/Desktop/workspace/DH_wt/b078` | `codex/b078-gameloft-audio` | `a0062e71ad` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix068` | `fix/b068` | `48c721f855` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix069` | `fix/b069` | `96c5d3ae2b` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix070` | `fix/b070` | `4a08d8c8cb` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix071` | `fix/b071` | `8bd4019fa6` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix072` | `fix/b072` | `6c76e90ffc` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix073` | `fix/b073` | `88847f66f8` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix074` | `fix/b074` | `ecbabee7c7` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix075` | `fix/b075` | `51f2f31c73` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix076` | `fix/b076` | `99827d8808` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix077` | `fix/b077` | `ed1c39b0d4` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix079` | `fix/b079` | `45571237d7` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix080` | `fix/b080` | `e39df81591` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix081` | `fix/b081` | `ca0c28a3f6` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix082` | `fix/b082` | `97d9b87cf8` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix083` | `fix/b083` | `2f994d0b59` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix085` | `fix/b085` | `c993bf1450` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix086` | `fix/b086` | `90565f73cb` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix087` | `fix/b087` | `f2f6377229` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix088` | `fix/b088` | `468e382f72` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix088fix` | `fix/b088-preserveverify` | `2a95fed7bc` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix089` | `fix/b089` | `a9dea86232` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix091` | `fix/b091` | `3a4aec63f7` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix092` | `fix/b092` | `9934db35fc` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix093` | `fix/b093` | `a2e1e5f702` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix094` | `fix/b094` | `5da46335c9` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix095` | `fix/b095` | `4a5f001265` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix096` | `fix/b096` | `5dc852ea6e` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix097` | `fix/b097` | `af2bf2cd90` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix100` | `fix/b100` | `a66b7f9c2a` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix101` | `fix/b101` | `dfc67d8df4` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix102` | `fix/b102` | `e40538b87d` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix103` | `fix/b103` | `1d69d39eb4` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix105` | `fix/b105` | `51e9a43c49` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix106` | `fix/b106` | `092b350128` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix108` | `fix/b108` | `ab1ee317eb` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix109` | `fix/b109` | `1a3e199fa9` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix110` | `fix/b110` | `919a0a3d9b` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix111` | `fix/b111` | `8903fff5bf` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix112` | `codex/b112` | `3c802731c3` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix113` | `codex/b113` | `9623f483cd` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix114` | `codex/b114` | `61e5966a5d` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fix115` | `codex/b115` | `3243b32a6a` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fixfx` | `fix/b101-b105` | `f882ad8307` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/fixsounds` | `fix/sounds` | `758d970ac6` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/npc-marker-motion` | `codex/npc-marker-motion` | `d9dbfc60ac` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/p161int` | `p16.1/integrate` | `2f0030e2d7` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/p16lifecycle2` | `p16/lifecycle2` | `3f5536746e` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/p17fire3` | `p17/fire3` | `e447e6687b` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/p17int` | `p17/integrate` | `955d486bf3` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/v201int` | `v20.1/integrate` | `4f7eb6b929` | 13 | 2 | active-integration-preserve |
| `C:/Users/adamc/Desktop/workspace/DH_wt/v20cine` | `v20/cine` | `9f0c698b2b` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/v20death` | `codex/v20-death-flow` | `afd9fca1bf` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/v20deathwire` | `v20/deathwire` | `612e96f820` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/v20doorfix` | `v20/doorfix` | `df50a082bf` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/v20doors` | `codex/v20-door-collision` | `810f6a9adc` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/v20fxverify` | `v20/fxverify` | `bacd123d8d` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/v20gatefx` | `codex/v20-gate-teleport-fx` | `60e60e852d` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/v20general` | `v20/general` | `db64218b4d` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/v20int` | `v20/integrate` | `743c56f547` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/v20rene` | `v20/rene` | `76b5e86c8c` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/v20savefix` | `v20/savefix` | `0c66a57a63` | 0 | 0 | clean |
| `C:/Users/adamc/Desktop/workspace/DH_wt/v20scriptsvc` | `v20/scriptsvc` | `63e1ac82f6` | 0 | 0 | clean |

## Largest immediate .local-inputs subdirectories

| Directory | Immediate files | Child directories |
|---|---:|---:|
| `ida-ghidra-review` | 885.1 MiB | 11 |
| `windows-foundation-build` | 417.4 MiB | 12 |
| `emulator-guards-v36` | 408.0 MiB | 0 |
| `windows-toolchain` | 343.0 MiB | 1 |
| `font-text-discovery` | 303.7 MiB | 8 |
| `b117-build` | 299.7 MiB | 3 |
| `runtime-test-20261009` | 228.0 MiB | 1 |
| `swampking-ai-target-host-build` | 201.0 MiB | 7 |
| `authored-fx-v32` | 140.1 MiB | 0 |
| `frontend-feature-build` | 132.9 MiB | 7 |
| `runtime-test-20261008` | 126.9 MiB | 0 |
| `combat-hit-fx-v1` | 109.8 MiB | 1 |
| `original-hud-packaged-1f2be85b` | 103.4 MiB | 0 |
| `original-hud-fixed-cef8471c` | 103.4 MiB | 3 |
| `original-hud-first-25ab5ce4` | 98.7 MiB | 0 |
| `loot-v44-host-objects` | 91.0 MiB | 0 |
| `windows-source-clock-checkpoint-v18` | 86.3 MiB | 1 |
| `windows-source-clock-v19-preview-1` | 81.7 MiB | 1 |
| `github-publication-2026-10-04` | 81.0 MiB | 0 |
| `windows-source-clock-checkpoint-v17` | 74.3 MiB | 1 |
| `blender-player-study` | 72.9 MiB | 1 |
| `ui-layout-discovery` | 68.0 MiB | 0 |
| `character-menu-connected-v2-host` | 68.0 MiB | 0 |
| `character-kill-discovery` | 64.8 MiB | 2 |
| `windows-main-frontend-v1` | 55.5 MiB | 1 |

Sizes count files directly inside each listed directory only; nested files are excluded.



