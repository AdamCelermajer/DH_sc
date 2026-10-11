# v20.1 Codex continuation — 2026-10-11

This run supersedes the stale running-session labels in STATUS.md. The latest
final-status section of HANDOFF-CODEX.md remains the starting evidence.
Nothing in this document resolves a reported bug or accepts a release.

Root owns integration in `DH_wt/v201int` (`v20.1/integrate`), review scheduling,
the shared tracker, packaging and acceptance. Existing Sol HIGH sessions continue;
the newly supplied AGENTS.md selects Luna HIGH for fresh ordinary workers,
with Sol MEDIUM escalation. Root and integration retain their current models.
User explicitly corrected the concurrency cap to23 active
sessions; the earlier six-session queue is revoked. All remaining implementation
follow-ups and reviews are launched in parallel, with disjoint worktree ownership.

| Scope | Fresh implementation owner | Worktree | Reviewer / state |
|---|---|---|---|
| B102 equipment / input prerequisite | `/root/b102_equipment` | fix102 | `/root/b102_review`: READY WITH CAVEATS; merged, integrated checks pending |
| B096 training | `/root/b096_skill_training` | fix096 | `/root/b096_review`: READY WITH CAVEATS; merged, integrated checks pending |
| B109/B107 rendering | `/root/b109_cavern` | fix109 | repaired and re-reviewed READY; merged, root exit/cave checks pass; user accepts rendering |
| B101 fire/common emitter core | `/root/b101_b105_effects` (scope B101 only) | fixfx | core reviewed and merged, residual appearance OPEN; owner resumed normal door/camera audit |
| B105 Celeste actor effects | `/root/b105_celeste` | fix105 | /root/b105_review READY WITH CAVEATS; merged and root glow check passes |
| B110 merchant shop | `/root/b110_merchant` | fix110 | source admission repair re-reviewed READY919a0a3d; merged, combined checks pending |
| B094/B091 boss cinematic state | `/root/b094_b091_boss_cinematics` | fix091 | /root/boss_cinematics_review repaired gate READY; merged, short combined check pending |
| B095 bag navigation | `/root/b095_bag_navigation` | fix095 | repaired closed-input gate READY4a5f0012; merged, root no-mutation check passes |
| B112 green NPC targeting | `/root/b112_npc_green_target` | fix112 / codex/b112 | /root/b112_review READY WITH CAVEATS; docs integrated, final package checks pending |
| B113 original travel-message widget | `/root/b113_travel_prompt` | fix113 / codex/b113 | /root/b113_review READY WITH CAVEATS9623f483; merged, B106 held-key repair pending |

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
  +138 is enabled (SetEnable0x33ddc8); +128 is visible (SetVisible0x33dcf8).
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

## User release naming — 2026-10-12

The user requests the completed package as v21. Working v20.1 coordination paths remain for continuity; package folder, manifest, receipt, acceptance status and README use v21. No candidate has yet been packaged or accepted.

B097 tip af2bf2cd independently reviewed READY WITH CAVEATS and merged into integration. Targeted rank1/rank2 casts verify actual MP9/12 and enemy damage; targetless/hurt remains B083. B088 adc667b7 is in same-reviewer re-review, with a requested IsSummoned predicate correction pending. B092/B098 owner resumed to investigate legitimate skill, ally/script, pause and restore lethal producers rather than closing on sword-only non-reproduction.

## Current gate snapshot — 2026-10-12 00:40 local

Reviewed and merged: B097af2bf2cd, B0882a95fed7, B114 narrow specf6720b2d, B115 texture reportb0499ca2 and marker-motiond9dbfc60. Combined root checks pass genuine eighth-kill reward/cue/duplicate, mixed allocations both orders/outcomes, death→revive→actual close→normal frontend resume, and held E/repress with correct B113 upper prompt. Full prior suite171/173 retains known asset-root failure plus campaign timeout; unchanged isolated retry passed0.92s. New NPC/spec focused6/6 passes. Final full suite remains after remaining code changes.

Active implementation gates: B083 departure/Pre target ordering with same reviewer; B092/B098 reproduced restored terminal corpse never retiring with same reviewer; B101 matched original closed fire appearance; B114 global material/light enrollment/distance darkness. All have named active owners. No final package is created; final delivered name v21.

## User accepted fire caveat — 2026-10-12

The user explicitly selected Include the fire caveat in v21 after reviewing original616-vs-normal390.png. B101 remains an active visual issue and is not resolved, but does not gate v21. No arbitrary shader/rate/size change is authorized or required. Door lifecycle and compiled cinematic path remain reviewed. Final Android/iOS sampler startup oracles both prove LINEAR/REPEAT/one mip with auto-mips disabled; no bloom/gamma/postprocess admission found. Raw iPad1.0.0 versus Android BDAE consumed fields match after explicit magic/buildtag/unused masks; smoke decoded bytes match, flame RGB compression differs while alpha matches. Exact reference1.0.3 remains unverified. Pending B083, B092/B098 and B114/B115 behavior/appearance gates continue.
