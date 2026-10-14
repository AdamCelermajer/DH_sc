# w5-faery: key-4 faery cast delay (owner report from a boss-area fight)

Branch `v21.8-w5-faery`, worktree `.claude/worktrees/w5-faery` (subst `X:`), build dir `C:/dh218/w5-faery/build`. Evidence: `coordination/v21.8/w5-faery-evidence/`.

## Verdict: WORKED (diagnosis complete; the delay is the original's authored cast wind-up, not a port bug)

The port adds about 1 to 2 frames (16 to 32 ms) between key press and the Use. Nothing else is added. No engine change is proposed.

## Question and candidates

Owner: "when the player presses 4 there is a noticeable delay before the faery's attack launches".

The faery does not cast. The hero casts the faery spell. In the original, `scripts/skills/faerie_hotty.luac` runs `OnPreSkill_` at the key press (UseMana, SetSpellCooldown, sound, Player_Pre FX). Damage (`OnSkill_`, one `SpellCombatRoll` per target) runs when the `do_spell` animation event fires in state 7. The source is the hero (`attacker=18446744073709551615` in the log; the faery body does not move).

Candidates from the brief:
- (a) faery approaches or turns before casting: RULED OUT. The faery body stays `Idle` at (204.255,556.767) for every frame of the cast window (run w5f-hotty-7, frames 280 to 420). No movement or walk clip. The hero casts from where it stands; `TargetListSearch(range)` is from the hero.
- (b) marker and wind-up timing in the clip: CONFIRMED as the cause (see timeline and authored data below).
- (c) our own latency: at most 1 frame between the key and the sequence start. The cast is accepted in the same frame as the key. No retry loop, no queue, no busy state. The cooldown gate is not on the delay path (it is set at pre and only affects the next press).
- (d) faery far from boss / teleport: not applicable to the cast path.

## Authored (original) stage lengths

Source: the port's authored sequence data as read from the original animation tables (`Retained clip range` diagnostic, clip start/end in ms), plus atlas PROVEN marker data.

| Stage | Hotty (seq 351) | Source |
|---|---|---|
| pre clip (`player-source-skills/source-skill-sequence-351/phase-0`) | clip range 0 to 400 ms, rate 1.3 | `Retained clip range`, `Faery cast phase speed=1.3` |
| do_spell clip (`phase-1`) | clip range 400 to 1133 ms, rate 1.3 | same |
| `do_spell` marker | 433 ms in `prince_spell_hotty.bdae` (raw tick 13 at 30 fps), i.e. 33 ms into phase-1 | atlas `do_spell` record (PROVEN) and the port's own range |
| Expected key to do_spell at rate 1.3 | 400/1.3 + 33/1.3 = 308 + 25 = 333 ms (about 21 frames) | derived |

Other faeries (clip ranges from the same diagnostic; rates were only printed for seq 351):
- Rocky (355), Wetty (356): phase-0 0 to 400, phase-1 400 to 1133/1166.
- Windy (354): phase-0 0 to 433, phase-1 433 to 1299.
- Celest (348): one phase, 0 to 1299 ms.

The original's Lua timers: none on the cast path (no `Timer`/`Delay`). `Faery_Cooldown` (5000 ms default) gates only the next press.

## Measured timeline (real key 4, hidden window, `--fixed-step .016`, frames as logged)

Real key injected with the harness at t=16.1 s (`key t=16.10 vk=52`). Frame times are 16 ms (`STATE_TRACE dt=0.016`).

| Stage | Hotty run w5f-hotty-7 | Notes |
|---|---|---|
| Key press to accepted (`Source Faery key=4`) | frame 277 (0 frames) | same frame |
| Pre phase starts (`Audio step ... step=0`) | frame 278 (+1) | the log's update serial is one ahead of the frame number, so this is within one frame |
| Pre-cast sound (`Faery cast sound ... status=dispatched`) | frame 278 | |
| do_spell phase starts (`step=1`) | frame 298 (+20 from pre start) | 20 frames = 320 ms, vs 308 ms expected at rate 1.3 (frame quantisation) |
| do_spell event and damage (`Damage ... SpellCombatRoll`) | frame 300 (+2 from phase start, 32 ms) | the marker is 33 ms into phase-1 at rate 1.3, i.e. 2 frames |
| Sequence complete (`phase=3`) | frame 334 | |
| Total key to damage | 23 frames = 368 ms | authored 333 ms; difference about 35 ms (about 2 frames of entry/serial offset and quantisation) |

Same stages for the other faeries (key frame, do_spell/damage frame, key to damage):
- Hotty: 277 to 300 = 23 frames (368 ms), run 7.
- Celest: 230 to 255 = 25 frames (400 ms). One phase, so the marker position inside the clip is not split out.
- Rocky: 278 to 303 = 25 frames (400 ms). Phase-1 entry at frame 299.
- Wetty: 294 to 319 = 25 frames (400 ms). Phase-1 entry at frame 315.
- Windy: 427 to 453 = 26 frames (416 ms). Phase-1 entry at frame 449. Windy's phase-0 is 433 ms long, not 400.

Earlier runs by the faery agent (`coordination/v21.8/faery-evidence/f8v3-*`, older build, key at about 16 s) show the same 22 to 26 frame key-to-do_spell gap.

Clock caveat: the `clipMs=` field on the `Source animation` line is the session wall time in ms since start (4816 ms at frame 300), not the clip-local time. The clip-local marker time is derived from the authored range above.

## Visual evidence (frames viewed)

- `hotty-key-plus7-f284.png` (key + 7 frames): the hero is casting, a fire flare at the hero's hand (Player_Pre FX), the faery HUD slot 4 shows the cooldown ring.
- `hotty-dospell-f300.png` (do_spell frame, key + 23): fire burst leaving the hero toward the two lizards; the damage lines are in the log at this frame.

The original gameplay footage is not in the repo, so the visuals are not compared to the original.

## What is NOT confirmed

- The port applies the authored leaf `speed` (1.3) as the clip rate. I did not verify in IDA how the original's sequencer applies that rate for state-7 Cast (`CSCast` is empty in the export; the sequencer code was not located in this pass). If the original ignored the rate, its wind-up would be 433 ms (about 27 frames) instead of 333 ms: the port would then be faster than the original, not slower. Either way the port adds no delay of its own. This is the only open point in the timing.
- Per-faery rates other than Hotty (351) were not printed (the diagnostic prints only the sequence the bank reports as active). The gap in Rocky/Wetty/Windy (do_spell 4 frames after phase-1 entry) is consistent with the same rate and offset.
- Sound audibility not checked (dispatch only).

## Changes (diagnostics only, no behaviour change)

Commit `e46a2de6` (branch `v21.8-w5-faery`):
- `port/windows-foundation/main.cpp`: `Faery cast phase sequence=... clip=... speed=... blendOut=...` line after the skill bank is built; `clipMs=` and `clip=` appended to `Source animation` lines.
- `port/windows-foundation/combat_session.{hpp,cpp}`: `CombatSessionAnimationDispatch` carries `wall_timestamp_ms` and `clip_id` (appended after the existing fields).
- `port/windows-foundation/retained_animation_owner.cpp`: `Retained clip range clip=... start=... end=...` for `source-skill-sequence` clips.

Merged `windows-foundation` (61f28852) into the branch (merge commit `c7b8a661`, no conflicts).

## Tests

Full CTest on the merged build: **222/222 passed** (`C:/dh218/w5-faery/ctest-final.log`, 538 s). The count is 222 (the brief said 221; the merge brought one more test).

## Package

Requested target `.local-inputs/windows-source-clock-v21.8-staged3` already existed (a v21.7-labelled package, manifest exe_sha256 A5008B28...). I replaced its `dh-foundation.exe` with this build (sha256 e09f84f9...) and wrote `w5-faery-package.json` beside it. The other files were not modified, so the folder's `manifest.json` and `package-receipt.json` still describe the old exe. The lead should check this before using the folder.

## Files

- `coordination/v21.8/w5-faery-report.md` (this report)
- `coordination/v21.8/w5-faery-evidence/` (per-faery `*-cast-timeline.txt`, two frames)
