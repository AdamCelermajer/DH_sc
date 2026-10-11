# I027 door collision: verification and fix (v20doorfix)

Branch `v20/doorfix` from `v20/integrate` (base `107f2a86`). Fix commit `cd9b09b6`.

## Investigation

Logic evidence (source/recovered logic, from the door worktree report `coordination/claude-preview20/DOOR-COLLISION-report.md` and IDA notes it cites):
- `Door::Close` admits only state 1 and not busy; the collision filter stays in its prior state during the clip and is enabled at the authored `closed` marker or at clip end (`Door::__EventCallback` / `__Callback`).
- The active Witch bank closes `_prim_BlueDoor` with `Intro_Witch` command 63, kind 55 `Script_CloseDoor` (INTEGRATION-report.md). This is the compiled-bank route used for the tests below. The XML `DiagnosticCloseBlueDoor` route is not used.

Visual evidence: no new footage was inspected in this session. Captured PPM frames exist but were not viewed; visual verification is a remaining gap (see below).

## Implementation

Bug found and fixed (`port/windows-foundation/features/doors/door_runtime_v1.{hpp,cpp}`):
- `CharacterVisual::update` keeps the clock relative to the clip start and clamps it to `end-start`.
- `DoorRuntimeV1::update` compared that relative clock against the absolute authored `marker_ms` and `clip_end_ms`.
- The source close clip spans 8000..9333 ms. The clock stopped at 1333 ms (= 9333-8000). The marker never fired and clip end was never reached, so the door stayed `busy`, `state=2`, `filter=0` (open) indefinitely. A closed door did not block.
- Fix: record `clip_start_ms[direction]` at adoption; compare with `door_clip_reached_v1(start, elapsed_seconds, target_ms)`, which adds the start back. Zero-start clips behave as before.

Diagnostics used during investigation were temporary and reverted before commit (`main.cpp` and the door trace). `main.cpp` and `CMakeLists.txt` are not modified.

## Isolated tests

- `door_runtime_restore_v1_tests` (with asset root): PASS, exit 0. New checks: `door_clip_reached_v1` at 8000 start for 1.332 s (not reached) and 1.333 s (reached), markers 0.499/0.5 s, zero-start clip, and a regression that the relative-clock comparison does not reach 9333 ms.
- `door_collision_v1_tests`: PASS, exit 0.
- `door_collision_geometry_v1_tests` (with asset root): exit 0.
- Full ctest (`build-v20doorfix`, `ctest -j 4 --timeout 120`): 155 of 161 passed; log `.local-inputs/v20/doorfix/ctest-full.log`.
  - `session_skill_binding`: Failed, "Asset path escapes asset root". Expected in a worktree (brief).
  - `despawn_after_death_v1`, `container_open_script_v1`, `frontend_music_v1`, `winmm_pump_priority_v1`, `startup_loading_art_v1`: ctest reported `0xc0000135`. Run directly through PowerShell (`Start-Process -Wait`), each exits 0. Environment launch issue, not a code failure.

## Integrated runtime (quiet, hidden, isolated fresh saves)

Frozen EXE (fix build): `C:/Users/adamc/Desktop/workspace/DH_wt/build-v20doorfix/dh-foundation.exe`
SHA256 `0D7620097F8B3F3A098B458916454C9DD5EDC1ACC4685992FFADDF96002C97E3`. Build exit 0. Pre-fix EXE SHA256 `A43CA7E7823B3F0A1ACA1387154BF449D7250FF6C8020D79DB11FCD939D6C3BA` (build exit 0).

Common args: the `witch-open-cross` argument set (Witch Cave, `--original-floor-motion`, `--original-native-bodies`, `--original-heading-rotation`, `--original-target-position`, `--fresh-player`, `--procedural-seed 1234`, `--fixed-step 0.0166667`), start `--position 5203.18,-11704.4,87.2637`. Jobs run with `port/windows-foundation/tools/quiet_run.ps1` (`DH_AUDIO_SILENT=1`, private desktop). Logs and summaries: `.local-inputs/v20/doorfix/`.

| Run | Args (differences from common) | Door at first admission / contact | End position (`Actor final`) |
|---|---|---|---|
| Open baseline, pre-fix | `--move-segment 10:160:-1,0 --frames 170` | filter=0 | 3665.27,-12272.2 (room 6, floor 8) |
| Closed walk 120, pre-fix | `--campaign-command Intro_Witch:63:0 --campaign-triggers --move-segment 120:280:-1,0 --frames 290` | filter=0 at all times; clock stalled at 1333 ms, `busy` forever | 5469.54,-12686.4 (passed through, Door_Teleport at frame 274) |
| Closed walk 330/240/180, pre-fix | same with later walk starts | filter=0 throughout (DOORFIX trace, 153 samples) | passed through |
| Closed walk 120, fixed | same as pre-fix closed | filter=1 at admission; contact at x=5145.35 (door max X 5030.81 + radius 113.7 + 0.5 slop) at about frame 150 | 5027.74,-13052.8 (frame 270 log 4987.04,-13012) |
| Open baseline, fixed | `--move-segment 10:160:-1,0 --frames 170` (no close) | filter=0 | 3665.27,-12272.2 (matches the fact; open path unchanged) |
| Save at frame 100 (closed), fixed | `--campaign-command Intro_Witch:63:0 --campaign-triggers --frames 101 --save-frame 100` | closed at frame 100 | (save written: `fix1-restore-save1-game.save`) |
| Resume from that save, fixed | `--resume-game --campaign-triggers --move-segment 0:180:-1,0 --frames 190` (no close command) | restored door `state=0`, `filter=1` at admission; contact at x=5145.35 | 5018.01,-13043.2 |

Door box (source ApplyMeshBox, `boundsXY`): 3899.98,-12799.1,5030.81,-11432.2. Player body radius 113.7 (player category 4, mask 0xd7f; door category 2, mask 0xffff, so the filter admits collision).

Interpretation:
- Closed door blocks: the walk stops at the face at x=5145.35 (box max X plus radius and slop), with the door at `filter=1`.
- Open door crosses: the open baseline reaches 3665.27,-12272.2, as expected.
- Floor is not the reason for the stop at the face. The only rejected floor samples logged in these runs are at (4417.17,-12501.6) and (4995.89,-13003.65); none is near the face. The stop coincides with the door box face plus radius (x=5145.35 against 5145.0 expected), and the closed run only stops there while `filter=1`. Per-frame position logs were not enabled for the open run, so no direct floor-support sample at the face is claimed.
- The closed walk's root motion includes a -Y component, so after contact the player slides south along the face and rounds the south corner. The final stop at about (4987,-13012) is a separate floor-support gap: the rejected floor sample `Source PF first clamp ... current=4995.88965,-13003.6533 requested=4984.69238,-13014.6963 valid=0 cachedHit=0 worldHit=0`. The same type of rejection occurred at (4417.17,-12501.6), and is pre-existing (INTEGRATION-report.md notes the floor gap).
- The closed endpoint therefore is not the same "stops short" end as the earlier wrong fixture. The door blocks the direct path at the face; the walk then continues around the corner and stops at the floor gap.

## Transitions and restore

- Restore from save: verified. Door restored as terminal `state=0` with `filter=1`, and the walk was blocked at the same face.
- open→close→open: NOT verified at runtime. No compiled-bank open command (kind 54, `_prim_BlueDoor`) was identified; the bank has no readable open string. The open leg uses the same fixed clock (`door_clip_reached_v1`), but no runtime pass demonstrates it. Gap.

## Remaining gaps

1. Visual verification: captured PPM frames were not inspected; no visual claim is made.
2. open→close→open runtime transitions: open command not identified (see above).
3. Floor support gap at (4995,-13003) and (4417,-12501): the closed walk ends there. Pre-existing, not door-related.
4. `container_runtime_v1.cpp` (around lines 190-208) compares its relative `animation_elapsed` against absolute `opened_marker_ms`/`clip_end_ms` the same way. Containers with nonzero clip start may stall similarly. Not verified and not changed here.
5. Integration: the fix is on `v20/doorfix` only; the root must merge it. The worktree `.local-inputs` junction must be removed (`cmd /c rmdir`) before removing the worktree.
