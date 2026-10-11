# v20.1 Codex continuation — 2026-10-11

This run supersedes the stale running-session labels in STATUS.md. The latest
final-status section of HANDOFF-CODEX.md remains the starting evidence.
Nothing in this document resolves a reported bug or accepts a release.

Root owns integration in `DH_wt/v201int` (`v20.1/integrate`), review scheduling,
the shared tracker, packaging and acceptance. Bug workers and read-only reviewers
use GPT-6.1 Sol HIGH. Concurrency is limited to five workers plus root.

| Scope | Fresh implementation owner | Worktree | Reviewer / state |
|---|---|---|---|
| B102 equipment / input prerequisite | `/root/b102_equipment` | fix102 | `/root/b102_review`: READY WITH CAVEATS; merged, integrated checks pending |
| B096 training | `/root/b096_skill_training` | fix096 | `/root/b096_review`: READY WITH CAVEATS; merged, integrated checks pending |
| B109/B107 rendering | `/root/b109_cavern` | fix109 | First patch committed; temporarily interrupted to open separate B105; resume queued |
| B101 fire/common emitter core | `/root/b101_b105_effects` (scope B101 only) | fixfx | Fresh separate B101 reviewer queued |
| B105 Celeste actor effects | `/root/b105_celeste` | fix105 | Fresh separate B105 worker and reviewer; common-core dependency owned by B101 |
| B110 merchant shop | `/root/b110_merchant` | fix110 | Temporarily interrupted for clarified B094; resume queued; fresh reviewer queued |
| B094/B091 boss cinematic state | `/root/b094_b091_boss_cinematics` | fix091 | Fresh session after user clarification; fresh reviewer queued |
| B095 bag navigation | `/root/b095_bag_navigation` | fix095 | Temporarily interrupted for new B112; corrected cinematic-free reproduction pending |
| B112 green NPC targeting | `/root/b112_npc_green_target` | fix112 / codex/b112 | New user report assigned immediately; fresh read-only reviewer queued |
| B113 original travel-message widget | `/root/b113_travel_prompt` | fix113 / codex/b113 | New screenshot/report assigned immediately; fresh reviewer queued |

Root retains follow-up ownership of B095, B111/B103, B091, B093 and B097 runtime
gaps until their fresh sessions can start (B091 has now been assigned below). Finished branches and already-merged
branches still need individual read-only reviews. B086/B108 require a combined
quit-after-death check after reviewed integration. B092/B098 and B094 remain
unconfirmed, not resolved by non-reproduction; B094 is explicitly reopened below. Sound submission is distinct from
audibility. No new bug was added in the user inbox at dispatch time.

## Baseline and infrastructure

- Accepted v20 full manifest/hash check PASSED: all 3,640 listed files present,
  no extras or missing files, all sizes and content hashes match. EXE SHA256
  `8BA1E537D33D4D4FD5B7DD9822C4CAB5080917FE2427A3903AB7C247827E8CCE`.
  Receipt: `.local-inputs/v20.1/root/v20-baseline-full-hash.json`.
- Existing integration tip at start: `dfc67d8d`, clean. Combined build started;
  result and full ctest pending. This is a baseline check, not release proof.
- `package_v201.py` copies accepted v20, rechecks its full manifest before
  copying, refuses existing targets and writes v20.1 metadata.
  `accept_package_v201.py` accepts only a pending v20.1 candidate after checking
  the frozen hash and manifest. Both CLI help checks pass. Neither tool has
  packaged or accepted anything during this continuation.
- Free disk at dispatch: 31,432,970,240 bytes (about 29.3 GiB). Reuse worktrees;
  preserve accepted packages, user saves and unrelated untracked work.

## Handoff corrections discovered during resumption

- B096 inspected the dependency commits: click latch is `8b5bfcee`, whereas
  `117c2b34` includes the equipment stance fallback/report. Reuse precise hunks.
- The claimed B096 P2 507–510 s skills-page reference is gameplay/map footage;
  the worker is locating the actual skills sequence before implementation.
- User clarified B094: "the boss lizard the brown one". The earlier companion
  audit does not cover that actor and its NOT A BUG conclusion must not be used
  to close B094. A fresh shared cinematic session now owns B094/B091 and the
  remaining B080 lizard/troll branch. Merchant work was interrupted to free a
  slot; its worktree and changes are preserved for resumption.
- Packaging guard checks pass: the v20.1 packager refuses an existing accepted
  v20 target, and the v20.1 acceptance tool refuses the accepted v20 receipt.
  Evidence: `.local-inputs/v20.1/root/package-guard-tests.json`.
- User reiterated one fresh session per bug. B101/B105 continuation ownership
  is now split: B101 retains the shared core, B105 consumes its committed API
  and owns actor/glow integration in its own worktree. Cinematic bugs remain
  together under the user's explicit exception. B109 was temporarily interrupted
  after commit `24873fb8` to keep the concurrency cap; its unfinished cave
  validation must resume. Each sound bug also needs a separate reviewer.

## Initial combined baseline result

Source `v20.1/integrate` at `dfc67d8d` built successfully (1,042 Ninja steps).
Frozen test copy `.local-inputs/v20.1/root/dh-foundation-initial-integration.exe`
SHA256 `18708E183185B68C512807AEA448357A072A6B65BD76B033D06D168014D05CEB`.
Full PowerShell ctest with toolchain PATH: 167/168 pass; only allowed worktree
`session_skill_binding` fails. Log `.local-inputs/v20.1/root/ctest-initial-integration.log`.
This checks the earlier merged batch only; it is not a v20.1 acceptance result.
Short hidden isolated chest/vase/NPC integrated fixtures are in progress.

## Reviewed input/equipment/training integration

B102 tip `e40538b8` independently reviewed by `/root/b102_review`:
READY WITH CAVEATS, no blocking defect. Source stance enumeration completeness,
retained live actor/pose ownership, focused tests, frozen worker hash and all
781 asset hashes checked. Preserve three matching existing models, add 778
missing models only to NEW package. Correct historical fixed-axe wording: its
save paths were scratch-relative defaults, not distinct explicit per-run paths.

B096 tip `5dc852ea` independently reviewed by `/root/b096_review`:
READY WITH CAVEATS, no blocking defect. Source signed gates/class-list mapping,
independent regression, retained success/refusal/reload captures and worker
frozen hash verified. Continue used actual frontend actions via diagnostic
`--menu-actions`; Skills opened via `--skills-page-frame`, then real 10 ms
selection/training clicks. Natural leveling and physical menu navigation remain
unverified; passive skill details/effects are an existing gap.

Both merged into integration at `389c88064c0b4a96178205031317ff78bb8c3ec8`.
Incremental build and combined executable checks pending. Bugs remain active.

B109 and B110 resumed when those reviews freed slots. New user B112 report then
opened a fresh worker immediately; B095 was temporarily interrupted to free
its slot. User clarified that related work may share the existing session/
worktree; keep evidence separate and prevent duplicate shared-core writes.

## Subsequent review/integration updates

- B112 clarified ground circle ONLY. `/root/b112_review` READY WITH CAVEATS;
  original quest/merchant status marker meshes contain the circle, type3 target
  FX set265 has no file. No speculative pointer targeting added. B100 is a
  private verification dependency and still needs its own review. Correct B112
  dialogue timestamp to about407s (406/406.5 retain marker).
- B105 `/root/b105_review` READY WITH CAVEATS for normal Celeste. Core/pose/
  billboard evidence and3/3 independent tests checked. Model-switch material
  cache pointer reuse is an inherited source-only follow-up, not a proven current
  Celeste failure. Broader hide/visibility/parameter clips remain unverified.
- B101 `/root/b101_review` READY FOR SCOPED CORE WITH B101 OPEN. Moving puffs
  improve but orange-wall appearance still differs. Color peak audit correction:
  raw133.333–400ms means particle ages roughly0.333–1s, not late-life peak.
  An inherited random-direction predicate differs from IDA; fire(0,0,1) unaffected.
- B091/B094 initial review NEEDS WORK caught missing scripted flag. Owner repaired
  exact current-state !scripted return-Idle gate (not clip-end/Unset-edge only).
  Re-review READY WITH CAVEATS at3a4aec63, frozen96413324...99C680A2,
  captain Unset301→release302 and five runtime branches checked. Initial schema
  replacement error5 retained, retries/full167/168 pass, cause unproven.
- Reviewed B101 core, B105 and B091/B094 now merged into integration. Build and
  focused combined executable checks still needed before any resolution.
- B095 reviewer NEEDS WORK for closed-menu queued release reaching hidden page
  provider. Original accepted-tab reset is sound; owner repairing that admission
  boundary with a queued Close→second-release regression before integration.
- B109 reviewer NEEDS WORK for specular transport lost on removal of baked decor;
  source repair8b1eb74c/tip1a3e199f binds full diffuse/alpha/specular generically,
  checks source alpha channel and preserves lookup failure atomics. Re-review
  pending. User explicitly confirms render appearance fixed in supplied B113
  screenshot; prior silhouette/scale comparison gaps are now bounded by that
  user confirmation, not claimed original pixel parity.
- B113 is travel-text/presentation only. Original P1~716/725s has bronze
  destination button with destination name only, plus separate World map route.
  Owner restores exact upper widget and PC[E]hint; unsupported separate world
  map/fast-travel route remains a feature gap, not an inert new control.
- Fresh B111 worker `/root/b111_stat_pages` and B093 `/root/b093_quests` now run
  their preserved worktrees. B093 owns actual quest accept/dialogue/script proof;
  merchant B110 does not supply that dependency.

## Latest reviewed repairs and integration

- B095 repaired456b5e23/tip4a5f0012 re-reviewed READY WITH CAVEATS. Actual queued
  Close→UNEQUIP changes old count4→3; repaired frozen preservesGarb/count4.
  Merged with B109 into integrationef6360b1; combined checks pending.
- B109 re-review READY FOR SCOPED INTEGRATION at1a3e199f; source material lookup
  atomics, alpha policy and specular transport independently checked6/6. User
  rendering confirmation supersedes stale OPEN appearance wording in its report.
- B100 fresh review READY WITH CAVEATS as B110 dependency; merchant shop remains
  B110 gate. Review checks circles/Space/sourcecaller and16/16 B112 runs. Source
  +138 is visibility, not enabled; wider IsInteractive gates remain unverified.
- B110 fresh reviewer is checking authored equipped-item SELL admission; native
  transfer unequipping side effect cannot establish UIpermission.
- B111 fresh review ongoing; B093 MAKE ACTIVE counter-loss reproduced and fixed,
  genuinekills/rewardonce proof ongoing. Deadmothresume failure is covered by
  unreviewed B086 role-gate repair, not a newbug on current evidence.
- B086 fresh review is checking WM_CLOSE saving, which --quit-frame cannot prove.
- Integration586a3587 build/ctest169/170 passed (knownassetrootonly), frozen
  2DF560F3...594DE23F normal Celesteglow360frames passes. Newer B109/B095 build
  passed; nextfrozencombined check pending. No release accepted.

## Acceptance gates still open

Finish actual production paths, inspect each report/diff/tests with a fresh
reviewer, merge reviewed fixes preserving each intent, run focused integrated
checks and full ctest, freeze v20.1, obtain independent A/B verification,
correct release caveats, update tracker and resolution history, merge to
windows-foundation and push only publish. Existing long scenes with unchanged
code and valid passing evidence are reused. No v20.1 release is claimed yet.
