# DH2 Act 1 implementation-to-IDA audit backlog

This backlog is a **verification queue**, not a claim that every row is a known defect. Each row asks for one observable behavior, state transition, or connection to be compared against the supplied game's IDA evidence and current reconstruction source.

## Evidence baseline

- Target: supplied Dungeon Hunter 2 APK; engine hash `36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
- IDA export: `.local-inputs/ida-apk-export-2026-10-07/libraries/` (machine-local, ignored by Git).
- Preferred references: `functions.jsonl`, exact per-function `pseudocode/`, `assembly-functions.asm`, `xrefs.jsonl`, and `basic-blocks.jsonl`. Validate inferred pseudocode details against assembly/callers when ABI, field offsets, order, or control flow matters. Do not use the Ghidra comparison as the behavior authority.
- Current port: source under `port/`; current tracker: `docs/ACT1-DELIVERY-TRACKER-2026-10-06.md`.

## How to read the backlog

One row is one checkable claim. A reviewer should trace the original function and relevant callers/callees in IDA, locate the corresponding reconstruction path, then classify it:

- `implemented`: matching source behavior and connection found; cite exact source lines/symbols.
- `partial`: some behavior exists; name the missing branch/state/caller edge.
- `disconnected`: both sides exist but the runtime path does not connect them.
- `missing`: original behavior is clear and no implementation exists after a scoped source search.
- `unclear`: IDA evidence or source mapping is inconclusive; ask a bounded follow-up question.
- `runtime_verified`: relevant integrated gameplay acceptance passed, with build/version and evidence.

Until reviewed, use `pending` / `not_run`; source presence, compilation, and component checks are not gameplay verification. Do not infer missing functionality solely from no test receipt. Keep evidence for negative claims (search scope and files checked). Preserve field/ABI uncertainty rather than promoting IDA's inferred prototype into a contract.

## Master table columns

The merged CSV will contain: `ID`, `System`, `Subsystem`, `IDA library`, `IDA address`, `IDA function`, `Source path(s)`, `Check / expected behavior`, `IDA evidence`, `Implementation status`, `Implementation evidence`, `Runtime status`, `Acceptance evidence`, `Priority`, and `Owner/session`.

IDs identify rows, not severity. Rows can be split further if one item contains multiple independent branches. Deduplicate only when the expected behavior and acceptance evidence are truly identical; preserve distinct call sites, state edges, and level/data families.

## Scope and acceptance

The queue is intended to exceed 1,000 discrete checks and be assigned across future audit sessions. Completion requires each row to have an evidence-backed classification; it does **not** mean every original behavior must necessarily be implemented if outside the offline Act 1 scope. User-facing milestones still require integrated runtime verification, including a playable Act 1 path and save/reload.

## Initial inventory

The merged table contains **1,156 candidate checks** across seven work areas:

| Fragment | Rows |
|---|---:|
| Character, campaign, and save/gear | 200 |
| Cross-system handoffs | 153 |
| Shared data and serialization | 151 |
| Gameplay and combat | 184 |
| Level loader and world | 155 |
| Presentation, input, and platform | 155 |
| Story, quests, and scripts | 158 |

The CSV validator confirms every row's IDA address/function mapping, cited pseudocode file and line range, and source path. All **1,156 checks** received static source-to-IDA review in 24 parallel batches on 2026-10-08. The first-pass baseline was:

| Source comparison | Rows |
|---|---:|
| Implemented | 474 |
| Partial | 473 |
| Disconnected | 42 |
| Missing | 131 |
| Unclear | 36 |

Reviewers revised the check wording for 581 rows where the original criterion was inaccurate or too broad. Every batch result, including the source and IDA evidence, is preserved in `review-batches/`. The first 24 save/load checks ended with 14 implemented and 10 partial. These are source-level classifications only: all 1,156 rows remain `Runtime status=not_run`, and no gameplay acceptance was performed. The pass did not change game behavior; it clarified one misleading source comment.

### Unclear-row second review

On 2026-10-08, 23 GPT-6.1 Sol agents reviewed all 36 rows that remained unclear. They reclassified 34 and kept two unclear, corrected the checks, and repaired stale IDA anchors and source mappings where needed. Immediately after that pass, before the loader-stage reconciliation below, the status totals were:

| Source comparison | Rows |
|---|---:|
| Implemented | 500 |
| Partial | 477 |
| Disconnected | 44 |
| Missing | 133 |
| Unclear | 2 |

The two remaining unclear rows are GC-0151 (AI death/revive cleanup ownership) and PP-0038 (whether `Application::Init` exposes a comparable failure/rollback contract). “Implemented” means matching source behavior and a connection were found for that row; it does not mean the game behavior passed in-game. All **1,156 rows still have `Runtime status=not_run`**. The full second-pass findings are preserved in `review-batches/unclear-sol61-01.csv` through `unclear-sol61-23.csv`.

### Loader-stage production-path reconciliation

A follow-up source trace found that several earlier loader findings searched only `port/level-loader` for stage callers. The active production binders live in `port/android-native/app/src/main/cpp`; the campaign connector installs the stage callbacks, and `SourceLoadingV43` moves the same external callback array into `LifecycleV36`. The earlier “no production binding” evidence was therefore stale. We corrected the affected loader rows and kept six exact behavior checks partial where field projection or side-effect coverage is still incomplete.

Current merged status totals are:

| Source comparison | Rows |
|---|---:|
| Implemented | 535 |
| Partial | 465 |
| Disconnected | 21 |
| Missing | 133 |
| Unclear | 2 |

The 155-row `Level loading / world` work area now has **100 implemented, 33 partial, 0 disconnected, and 22 missing** checks. Across the 48 checks mapped to the numbered 0–38 stage flow, **43 are implemented and 5 are partial** at source level. Another 15 rows labeled `Level loading` live in the cross-system fragment and remain partial. None has gameplay acceptance: all 1,156 rows still have `Runtime status=not_run`.

### 8 October live-path reconciliation

The Stage17 order check `LW-0066` was stale: its earlier partial rationale said no production InitFinal body was connected. Current source traces the deferred Stage17 callback through `renderer_character_campaign_v62.inc` and `source_campaign_object_loading_deferred_v106.cpp` into `SourceObjectLoadingV95::final`; `LifecycleV36` executes state17 before state18, matching IDA `_LoadProcess`'s `_LoadFinalInit` then `_LoadCharStates` order. The row is now `implemented` at source level. Its runtime status remains `not_run`, and this correction does not establish successful Swamp loading.

The two remaining unclear rows are GC-0151 (AI death/revive cleanup ownership) and PP-0038 (whether `Application::Init` exposes a comparable failure/rollback contract). The initial 23-agent findings remain preserved in `review-batches/unclear-sol61-01.csv` through `unclear-sol61-23.csv`; corrected loader evidence and source paths are in `part-loader-world.csv`.

The current status clusters by topic:

- **Partial:** shared game data (84), level loading/world (33), save/load (27), audio (27), and script runtime (20). These rows usually describe behavior that exists but has a missing branch, format detail, state edge, or handoff.
- **Disconnected:** character menu (9) and application/UI (7) are the largest clusters. These rows describe code whose active route is not connected in the audited source.
- **Missing:** level loading/world (22), shared game data (20), input (18), and menu/HUD (17). These are scoped source searches where the specific behavior was not located.

These counts are findings per check, not percentages of whole systems or proof that every unlocated behavior is absent from all runtime paths. “Missing” reflects the audited source scope; “disconnected” reflects an integration gap; “partial” means some part of the check is implemented. None of the labels is runtime verification.

Run `python validate_checklist.py --merge` with the workspace's bundled Python to rebuild and validate `checklist.csv` from the seven fragments after review updates.
