# v20.1 Codex continuation — 2026-10-11

This run supersedes the stale running-session labels in STATUS.md. The latest
final-status section of HANDOFF-CODEX.md remains the starting evidence.
Nothing in this document resolves a reported bug or accepts a release.

Root owns integration in `DH_wt/v201int` (`v20.1/integrate`), review scheduling,
the shared tracker, packaging and acceptance. Bug workers and read-only reviewers
use GPT-6.1 Sol HIGH. Concurrency is limited to five workers plus root.

| Scope | Fresh implementation owner | Worktree | Reviewer / state |
|---|---|---|---|
| B102 equipment / input prerequisite | `/root/b102_equipment` | fix102 | Root preliminary audit; fresh read-only Sol HIGH reviewer queued |
| B096 training | `/root/b096_skill_training` | fix096 | Root preliminary audit; fresh read-only Sol HIGH reviewer queued |
| B109/B107 rendering | `/root/b109_cavern` | fix109 | Root preliminary audit; fresh read-only Sol HIGH reviewer queued |
| B101/B105 particle/fire effects | `/root/b101_b105_effects` | fixfx | Root preliminary audit; fresh read-only Sol HIGH reviewer queued |
| B110 merchant shop | `/root/b110_merchant` | fix110 | Temporarily interrupted for clarified B094; resume queued; fresh reviewer queued |
| B094/B091 boss cinematic state | `/root/b094_b091_boss_cinematics` | fix091 | Fresh session after user clarification; fresh reviewer queued |

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

## Acceptance gates still open

Finish actual production paths, inspect each report/diff/tests with a fresh
reviewer, merge reviewed fixes preserving each intent, run focused integrated
checks and full ctest, freeze v20.1, obtain independent A/B verification,
correct release caveats, update tracker and resolution history, merge to
windows-foundation and push only publish. Existing long scenes with unchanged
code and valid passing evidence are reused. No v20.1 release is claimed yet.
