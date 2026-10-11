# RENE report: do Rene and Celeste follow in the integrated v20 build (B077 recheck)

Branch v20/rene (from v20/integrate 743c56f5). Verdict: NO regression. Followers work in the integrated build without `--condition-active RENE_FOLLOW`. The "no follow lines" in package-smoke runs B/C is a logging/fixture effect. One real, separate gap found (save resume does not restore companions). Only change: an env-gated diagnostic trace.

## 1. Investigation
- Reference expectation (video, from B077 report, not re-watched): after the opening Rene and Celeste stand beside the hero, trail and catch up. Evidence kind: direct video observation (B077 s1).
- Logic evidence: unchanged B077 (CharAI::_UpdateMaster 0x3cc5a4, AI_IsInSight 0x3d4ed8, _WarpBehind 0x3b8d98, follower.luac / rene.luac). Re-read of controller `features/companions/runtime_follower_controller_v1.cpp`: it logs only on action edges (approach/stop/none, `rene` catch-up) and the periodic `walking` line only while moving. After the opening the hero stands still (frames 3775..4900 in runs B/C), Rene/Celeste are already in range, so there is nothing to log. This explains the missing lines. C (with the flag) shows the same two lines as expected.
- RENE_FOLLOW gate: without the flag the generic condition evaluator produces it from quest state: `Source condition lifecycle actor=_prim_NPC_PriestGood activate=true deactivate=none tested=10 enabled=true` (fresh start; resumed save of run B quest row 8 not yet 13). The flag is NOT needed. With the flag the Priest would stay enabled even after quest 8 reaches state 13, which is wrong (SCRIPTSVC q8-resume shows the natural disable). So the flag must be removed from shipped args.

## 2. Implementation
- `runtime_follower_controller_v1.cpp/.hpp`: env `DH_FOLLOWER_DEBUG=1` prints `[follower] track actor=.. script=.. pos=.. master=.. distance=.. moving=.. catching_up=..` every 30 follower ticks. No behaviour change.
- No defect found, so no gameplay fix and no new unit test (follower_decision_v1 tests from B077 still pass).

## 3. Isolated tests
ctest (llvm-mingw bin and cmake dir on PATH, PowerShell): `99% tests passed, 1 tests failed out of 166`; only `session_skill_binding` failed. (Run from Git Bash with a POSIX PATH, 6 extra tests failed with 0xc0000135 DLL-not-found: PATH artefact, not code.) Build `p14_build.ps1 -Name v20rene -Jobs 3` exit 0.

## 4. Integrated runtime (frozen EXE)
EXE `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/v20/rene/final/dh-foundation.exe`
SHA256 `ccf5d3e6538d7fa7dde8185959f7737a458e8c73d8b21d6af01991ba98dad53f` (build-v20rene; includes the trace only). The shipped v20 package EXE (windows-source-clock-v20) shows the same follower behaviour in run r1 (below).
All runs hidden/silent via quiet_run.ps1, isolated dirs under `.local-inputs/v20/rene/`, args = package-smoke run B args (no `--condition-active`, `--enemy-ai`, `--animation-only WanderingPriest/DefaultFairy`, `--campaign-triggers`).

### f1: full 4900-frame opening, then walk (args B + `--frames 5800 --move-segment 4950:5450:1,0`), exit 0, 100 s
Positions (follower ticks counted after the cutscene; hero moves from tick ~1100):
| | start (opening end) | after walk |
|---|---|---|
| hero | 791.8,-416.2 | 1576.5,-200.5 |
| Rene (`rene`) | 794.1,-618.9 (dist 202.7) | 1260.8,-365.9 (dist 356; log: approach haste=on at 566.8, stop at 352.0) |
| Celeste (`follower`) | 549.8,-499.9 (dist 256.0) | 1060.1,-416.5 (dist 555.4; approach at 565.4, stop at 555.4) |
Rene's `Buff_Rene_Haste` walk factor 1.2 logged. Frame f5200 (capture): Rene beside the hero, Celeste glow/faery upper left of Rene. Stop distances equal the B077 formula values (346-356 Rene, 556 Celeste).
### s2: skip fixture (`--campaign-skip-frame 200`), long walk 700:1300 (1,0), 1300:1900 (0,1), 1900:2400 (-1,0), 2600 frames, exit 0
Hero path ended at -1278,-1273 (about 3000 units of travel with turns). Rene ended -813,-1148 (dist 482; haste catch-up observed moving=1 catching_up=1 at tick 1890, dist 457), Celeste ended -750,-1104 (dist 554; was 950 behind at tick 1890 and caught up). Both followed through the turns.
### s1 (skip, 700:1000 walk): same: Rene stop 353, Celeste stop 556.
### r1: resume fixture (see gap 1) with the SHIPPED v20 package EXE: Rene followed (final 1262,-356 vs hero 1581,-199), Celeste did not (stuck at 544.2,414.7,-108.2).
Not observed in runtime: warp-behind and Rene's no-path WarpTo (max distance reached 950 < view radius 1000 / 2300; unit-tested only).

## 5. Visual verification
Frames inspected: r1 f10, f900; package C f4900 (Rene beside hero, a white faery near, the player's own faery or Celeste not distinguishable); f1 f5200. Original-video comparison not repeated (see B077 section 1).

## 6. Celeste/other companions
Same controller path: every actor whose AI row script is `follower` (Celeste, `_prim_Faery`), `attacking_follower`, or `rene` is driven by it, selected by AI row script only. Celeste follows (f1, s1, s2). The player's own native AISFaery (`__faery__`) is not part of it.

## 7. Gaps found (not fixed, honest)
1. Fast resume is not faithful for companions: `--fresh-player --save character.save` restores the character only. The level world snapshot (`character.world-001_swamp.gsave`) is restored only by arrival through a transition (`arrivedByTransition`, main.cpp ~2140) or a save with an entry component. A script-quicksave profile (Swamp_Intro SaveGame at frame 3775) has none, so resumed Rene/Celeste start at authored placements (Celeste at 544.2,414.7,-108.2: off-floor, no route, never moves and never warps because she is 907 < 2300 from the hero). In the live session, Celeste is at 549.8,-499.9 on the floor. So "Continue" after the opening would leave Celeste stuck. Needs an owner (load of the saved world on continue); proposed as a follow-up bug, not fixed here.
2. Rene ranged attack, IsMyTurn throttling, master-death events remain as in B077 report section 7.

## 8. Release notes wording
Replace the Rene/B077 line with: "B077: Rene and Celeste now follow the hero after the opening (walk after you, stop at their source follow distance, Rene hurries to catch up, warp behind you when left far behind). They do not yet attack, and after loading a save made right after the opening Celeste can be left behind (known issue)."
Do not mention a launch flag.

## 9. Correct shipped args set
Required: the normal swamp args including `--enemy-ai`, `--population-templates`, `--animation-only WanderingPriest` / `--combat-idle WanderingPriest:Idle:0:0`, `--animation-only DefaultFairy` / `--combat-idle DefaultFairy:Idle:0:0`, `--campaign-triggers`. Must NOT contain: `--condition-active RENE_FOLLOW`, `--source-root-scopes`, `--campaign-commands original-campaign.xml`. Quest state decides the Priest's enable.
