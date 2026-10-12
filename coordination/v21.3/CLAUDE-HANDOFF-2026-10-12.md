# Handoff to Claude — 2026-10-12

## Current state

The requested bug batch is completed and v21.3 is accepted. Cleanup is completed. No implementation agents remain active and no new behavior change is currently requested. The user asked for this handoff after asking about the flat NPC head icons.

Primary checkout: `C:\Users\adamc\Desktop\workspace\DH_sc`, branch `windows-foundation`. Verified HEAD: `45a0c602fad74f086ddd10ef9e906df7732afa31` (Complete v21.3 cleanup and record recovered space). Working tree was clean immediately before writing this handoff; this handoff itself is a new, uncommitted document.

All preceding source, review and cleanup changes were committed and pushed to the user's `publish` remote, `https://github.com/AdamCelermajer/DH_sc.git`. Do not push to `origin` (`Noamcelermajer/DH_sc`). Only the primary checkout remains; old worktree paths in historical reports no longer exist.

## Launch and release evidence

- Launch: `.local-inputs/windows-source-clock-v21.3/Play.cmd` (main menu); `Play-swamp.cmd` is the direct Swamp launcher.
- Shipping EXE SHA256: `276C83146351F41E879F296143F8F59F74A0D3552F224D9015AEF87C3037779A`.
- Production source commit: `51c4ac5c469b01969b39d6af1be45a68e5eb2c57`; merged into primary at `af10fa0a`.
- Full CTest: **177/177 passed**. Raw roster and pass log: `.local-inputs/v21.3/integration/ctest-177-51c4ac5c469b01969b39d6af1be45a68e5eb2c57.log`.
- All 4,420 package manifest entries and seven copied progress files were checked after cleanup. Previous accepted packages and original user saves were preserved.
- Root tested final moving/idle chest interactions and packaged main-menu Continue with isolated copies of the accepted progress. See `FINAL-REVIEW.md` for hashes, exact scope and excluded harness failures.

Read these before changing or making completion claims: `coordination/v21.3/FINAL-REVIEW.md`, `AUDIO-REVIEW.md`, `MENU-REVIEW.md`, `GAMEPLAY-REVIEW.md`, `PACKAGE-REVIEW.md`, and `PACKAGE-METADATA.json`. Per-bug evidence is in `B119-report.md` through `B136-report.md` and associated reports.

## What was fixed in this batch

The failures were reported from **v21.1 (chest crash hotfix)**. The user confirmed loot pickup, potions and Equipment stopped together after the same fight/death. Stable IDs B119–B136 are accepted and moved from the active tracker into `docs/RESOLVED-BUGS.md`.

- B119–B122: original sale, buy, point-allocation and reachable menu/equipment/skill/Faery sound paths. Exact source cues and refusal/duplicate ordering; nonzero device-bound PCM verified. This is not a claim that every possible sound in the whole game is complete.
- B123: merchant stock retains distinct generated instances even when names repeat; powers, prices and item details survive buy/sell/buyback/save/Continue. Repeated base names are valid original behavior.
- B124: PC wheel, keyboard and drag list navigation with scaled layouts, boundaries, modal/outside suppression and empty/single/reopen cases.
- B125/B134/B135: authentic ring powers/details/effects, source item-name colors and actual equipped Save→Continue. A bare Rusted Ring legitimately has no base stat bonus; do not invent one.
- B126: old-save admission compatibility and dead-mob/native-body restoration. New-save enabled/targetable states remain preserved; strict live/unknown guards remain in force.
- B127: exact brown axe-bearing Bogmop Champion targeting/combat/death, including per-placement old-save compatibility. The user's earlier B094 clarification was the brown boss lizard, not Celeste or Rene.
- B128–B131: checkpoint physical body and current consumers restored; actual death→Continue→combat, potions, Equipment navigation and source loot pickup verified. Source Limbus/respawn state is narrowly admitted, not generalized.
- B132: normal postfight BashDown registry completeness; no locomotion-guard bypass.
- B133: authentic Drop/Transmute Yes/No dialogs and shade, current item lease, one-shot mutations/cues and restore cancellation.
- B136: source camera providers are bound for normal Continue without requiring scripted commands; matching merchant save, movement and repeated F9 verified.

The original chest-crash B117 regression also passes on the final shipping EXE. The log had complained that physical locomotion must be admitted through the current authored locomotion selection; do not remove that invariant to mask future problems.

## Latest NPC icon question

User: “the icon on top of the npc head are really 2d is it as intended?”

Answered: the flat icon appearance is consistent with the original (merchant money bag and quest exclamation mark), but our NPC markers still use static meshes and the original loops their effect animator. Animation is an outstanding fidelity gap. The user has not yet asked to implement this gap.

Evidence checked for this answer:

- Original merchant frame at P1 `z_Zky7qQdYs`, approximately 585 s: `.local-inputs/claude-v20.1/fix100/keep/video-t585.png` was visually inspected and shows a flat money-bag icon above the merchant.
- `coordination/v20.1/B087-report.md`: original quest marker observed around 404–407 s; sequence `.local-inputs/claude-v20.1/b087/keep/video-t404-411.jpg`. `Objective_TalkToNPC::InstallObjectiveMarker` 0x47d8e8 attaches AnimatedFX and loops its animator.
- `coordination/v20.1/B100-report.md`: `Character::Update` 0x3abe98 selects merchant effect 139 while enabled and not a registered quest NPC, and loops its animator.
- Current `port/windows-foundation/main.cpp` loads the original `quest_marker`, `quest_marker_blue`, `quest_marker_merchant` BDAE assets and directly submits their static meshes, around lines 2489–2495 and 8350–8394. These are world meshes, not newly invented UI icons. Do not claim full camera-facing/animation parity from the still frame.
- `B087-B100-marker-motion-followup.md` documents live NPC-position attachment. `B112-report.md` explains that the green ground circle belongs to the authored quest/merchant marker geometry; the original type-3 NPC target FX row has no separate circle asset.
- B105's camera-facing animated glow fix is for Celeste's embedded effect; it does not establish animated NPC-marker completion.

If the user requests NPC marker animation, open a fresh bounded implementation session and reviewer, inspect an original sequence plus the exact marker asset animation/billboard records and callers before coding, and use the existing reusable effects system where justified.

## Remaining qualifications

- Accepted fire B101 appearance caveat: recovered effect is sharper/swirlier than softer original footage. User explicitly authorized shipping v21 with this caveat.
- Broad lighting/fog/water appearance caveats B114/B115 remain; do not claim complete original visual parity.
- Earlier F9 mutable-targeting caveat B116 remains documented. B136 camera binding fixes do not erase unrelated F9 gaps.
- NPC marker effect animation remains incomplete as above.
- Conditional/unreached audio and original focus-in cue paths remain qualified. No acoustic parity by ear claim.
- Merchant normal 179-purchase exhaustion UI soak was not run. Source/Shop refill cycles and initial-empty/nonempty reopen were verified.
- Broad RoomZone/class/companion/later-Act coverage remains partial; this is not whole-game or whole-Act-1 completion.
- B126 restored-moth close-up used a separately hashed camera-only observer with an alive-before-restore moth. Actual lethal/death→Continue proof is a separate shipping production run. The independent reviewer accepted that combined scope; never describe the observer as a same-run lethal visual proof. Details are in `FINAL-REVIEW.md`.

## Cleanup and recovery

Removed **67 extra Git worktrees**, **61 obsolete build directories**, and verified reproducible caches. Measured free-space increase: **118,586,933,248 bytes, approximately 110.44 GiB**. Only DH_sc remains registered.

Every removed worktree HEAD is preserved locally and on the user's fork under `codex/archive-wt-20261012-*`; mapping is `WORKTREE-PRESERVATION.json`. Dirty generic-loader source was separately committed at `8b322ec56fe2c778d7e759a3103838c4d0f5ca50` and pushed on `codex/archive-generic-level-loader-20261012`. Do not assume the primary checkout contains every archived experiment.

Unique ignored files, configuration, captures, saves and executable evidence were preserved and hash-checked under `.local-inputs/cleanup-preserved-20261012`; obsolete build evidence uses `build-objects/<SHA>.bin` with receipt mappings. Shared `.local-inputs` junctions were unlinked nonrecursively; primary data remains. Eleven unregistered scratch directories were intentionally retained because they are not disposable Git worktrees.

Read `CLEANUP-RESULT.md/.json` and raw receipts in `.local-inputs/v21.3/root` before any further deletion. Old compiler/debug products were intentionally discarded; a rebuild may now require a fresh build directory.

## User workflow requirements

Read the repository's actual `AGENTS.md` and `docs/BUGS-AND-IMPLEMENTATION.md` user inbox before assignment and after integration updates. Preserve stable IDs and user additions.

- Investigate original visual behavior and recovered source/IDA logic **before implementation**, including callers/admission gates. Record observed facts, expected behavior, uncertainties and a focused test. Reuse valid evidence, but do not turn an unknown into a guessed original rule.
- Bugs take priority. Launch disjoint bug owners in parallel promptly; the user strongly objected to serial bug handling. Every newly reported bug gets a **fresh implementation session and named reviewer**, with exclusive ownership for shared files.
- Ordinary workers: **GPT-6 Luna HIGH**. If a session gives up or cannot solve its bug, launch a **fresh GPT-6.1 Sol MEDIUM** retry. Root and integration retain their models.
- Helper/audit/compile-only success is not resolution. Integrate and verify the actual production path on a frozen identified EXE, including outcome/refusal/duplicate cases and visual checks where relevant.
- Use isolated saves and test windows. Never control the user's live game window or overwrite their progress.
- After acceptance, move stable IDs and evidence to `docs/RESOLVED-BUGS.md`; preserve history. Package when actively assigned preview fixes pass, without waiting for unrelated future features.
- User authorized committing/pushing to their forks and removing completed worktrees/build clutter after preservation. Preserve accepted packages, unique source and saves. Never recursively delete through shared junctions or unknown scratch paths.

Next action depends on the user's next request. Do not reopen accepted bugs or begin broad speculative changes merely because this handoff lists qualified gaps.
