# OPENING2 report (Preview 16, wave 2): caption timing, scripted objects, camera, hands, start/continue, completion

Branch `p16/opening2` (worktree `DH_wt/p16opening2`), base `p16/integrate` (29bcec59, which includes `p16/opening`). Build `DH_wt/build-p16opening2/dh-foundation.exe`. Not pushed. HUD art files were not edited (HUDART session owns them).

## Status (living, updated per step)
- (1) Caption timing: DONE in code and unit tests (commit d94ba82f). Model decoded from the original Flash dialog box. Runtime verification against the reference: pending (section 4).
- (2)-(7): in progress.

## 1. Caption timing (investigation)
Logic (IDA, `pseudocode-all.c`): `Script_StartDialog::Execute` (line 248929) enqueues `DialogMsg(text id = scalar 16, style = scalar 12, actor = scalar 8)`. `Script_WaitDialog` blocks while `Level::hasActiveDialog` (queue non-empty). Nothing in the C++ side times a line: the Flash box advances it through the `StopDialog` FS command (`MenuBase::FS_StopDialog`, line 212363), which pops the line and starts the next one. The Flash box is `dqhud_droid.swf` (30 fps):
- DialogBox = sprite 727 (one frame label per DialogStyles value). Per frame it sets `isSkipable` and the visibility of `btn_next`.
- dialogBox = sprite 730 (labels show = frame 1, hide = frame 19; `onShowAnimEnd` on frame 12; `onHideAnimEnd` on frame 39; no Stop inside).
- AS: `onShowAnimEnd` stops the box when `isSkipable`; `btn_next.onRelease` calls `hideDialog` (gotoAndPlay('hide')) only when `isSkipable`; `onHideAnimEnd` calls `NativeStopMessage('dialog')` (or 'quest' for styles 5 and 19).
- Styles (value from `data/dialogs_pycst.bin`): non-skippable = ScrollingDialogFull 6, TopBubble 7, TopBubbleAvatarLeft 8, TopBubbleAvatarRight 9, Warning 10, QuestCompletedMsgDialog 19. Every other style waits for a tap.
- Swamp intro captions use style 4 (MiddleBubble, tap). CombatTuto uses style 4, 11 and 12 (all tap).
Original rule (implemented): show 11 steps = 367 ms; a tap-wait line stops there until the player taps; a tap starts the hide (20 steps = 667 ms), then the next line starts; a non-skippable line plays on by itself (38 steps = 1267 ms). Taps are the only thing that ends an intro caption; the reference durations (2-8 s) are the viewer's taps.
Implementation: `features/cinematic_runner/cinematic_runner.{hpp,cpp}` (phases Showing, WaitTap, Hiding, AutoRun), tap via `CampaignHost::caption_tap` from any pointer release (PC adaptation of btn_next; the button art is not drawn), test harness `--caption-auto-tap-ms N` (default 0 = player only). The placeholder hold (2000 ms + 25 ms/byte, cap 6000) is removed.
Tests: `cinematic_runner_tests` (durations, tap-driven queue, tap during show, auto line, auto-tap harness, flush, SKIP, frame) and `campaign_host_tests` (CombatTuto waits 3 s for a tap, then the auto-tap harness completes the script).

