# w5-mudbar report: Mud Lord target bar, skip path, death camera, speaker label (PARTIAL, no code change yet)

Branch `v21.8-w5-mudbar`, worktree `.claude/worktrees/w5-mudbar` (subst `V:`, 936 junctions to the primary `.local-inputs`, not removed). Build dir `C:/dh218/w5-mudbar/build` (Release, baseline build OK, exe copy `C:/dh218/w5/base.exe`). Harness: `C:/dh217/tools/run-case.ps1`, runner `C:/dh218/w5/run-skip-pair.ps1` (outside the repo). No commit, no push, no merge. Packaging `windows-source-clock-v21.8-staged3` NOT done.

## Verdict
- Target bar vanishing (original task): NOT REPRODUCED, no fix. DID NOT WORK (not attempted to completion).
- Skip path (coordinator item 1): PARTIAL. Real skip vs played runs executed; the command sequence is identical, but the presentation differs as the port gates it. The boss end state is identical in both runs. Not yet shown that the skip causes the bug.
- Death cutscene camera (coordinator item 2): NOT REPRODUCED in this pass (no kill run made in this session). Evidence of the camera track only.
- Speaker label "WW" (coordinator item 3): NOT RESOLVED. The log says `speaker=ada`; the screenshot shows "WW". Rendering issue in the NameBox text path, not diagnosed.

## Real runs (hidden window, real keys, frames viewed)
Seed `afterwitch`, lever, Swamp_Serpent_Intro, same clicks in both runs. Skip run: SKIP click at (125,60) two seconds after the S3 sync.
- Played: `C:/dh217/cases/w5-mk-played`. Intro finished at frame 2519; `EndScriptedCutScene` ran.
- Skipped: `C:/dh217/cases/w5-mk-skip`. Log `[campaign] SKIP pressed` at frame 964+; `SKIP fast-forward passes=2 frame=1107 ok=1`; `Swamp_Serpent_Intro finished` at 1107.
- Command trace: both runs execute the same 110 script commands in the same order (diff of the `script=Swamp_Serpent_Intro ... class=` sequence is empty). In the skip run each line carries `skip=1`. `PlayCamera skipped (SKIP sampled); follow camera kept` (8 times). `PlayActorAnim` is a no-op under skip (port code, campaign_host.cpp case 45, matches its IDA comment).
- Final boss record (`Actor final id=14582633645715090783`) is identical in both: `position=-19398.3,11570,0`, `HP=900/900`, `target=<hero>`, `Source floor final ... height=-113.968 room=7 floor=11`. Lifecycle `state=3 enabled=1` in both.
- Frames viewed: played f2600 (boss mouth open, cinematic camera, SKIP visible), skip f2600 (normal follow camera, hero and priest visible, boss head at top edge), played f2790 and skip f2790 (boss body at top of screen, both show the boss away from the hero, skip shows the body as an upright column). The visible difference is camera framing and the boss pose (the authored underwater/emerge clip is skipped), not the boss position.

## What the skip does (original vs port)
- Original (per the port's SKIP16 comments, IDA `ScriptManager::SkipScript`, `ExecuteScript`): every command runs with skip=1; state changes apply; presentation (PlayCamera, PlayActorAnim, dialogs, sounds) is gated. The port follows this.
- Not shown: whether the original also gates `Script_LookActor` heading or `SetActorPosition` placement differently. In the port both run in the skip pass, so heading after the skip is the same code path. The coordinator's claim that the boss is "not looking at the player" after a skip was NOT confirmed in these two runs; the heading was not logged.

## Open items, in order (not done)
1. Target bar: reproduce by targeting the Mud Lord (Tab) in the played and skipped runs after the intro, log `target`, `IsTargetable` and eligibility each frame. Hypothesis (a) from the lead still open. The only lead found: mudlord3/B155 note "SetIsTargetable also blocks damage in the port" (B155 handoff section 7).
2. Brown-mud object enable/targetable state after skip (not logged in these runs).
3. Death cutscene camera: `Swamp_Serpent_Defeated` runs `PlayCamera` with `cs_swampking_outro_camera_scene01..04.bdae`; in the m3f4 log scene03 (camera 506) is the one with the dialog ("Alright, ..."). The screenshot's camera is inside the serpent body, which suggests the camera track starts from a boss position that differs from the original (boss position/heading at death). Not checked.
4. "WW": the caption log has `speaker=ada` for actor 36 (the hero). The name plate is drawn by `text_from_slot` (`features/cinematic_runner/cinematic_runner.cpp:194`) with the raw speaker string, so the "WW" must come from the NameBox font/glyph path. Not diagnosed.

## Not reproduced
- The vanishing target bar (no run in this pass targeted the boss).
- The death-camera problem (no kill in this pass).
