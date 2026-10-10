# Preview 13 Group A report: B007 creation flow and arrows

Status: implementer report. B007 is NOT closed. One concrete, reproducible arrow defect was found and fixed in isolated tests. The slot-4 lead could not be reproduced: the current code refuses full-slot creation. The on-device intermittent symptom is not reproduced end to end; see Uncertainties.

Package note: the Preview 12 package moved during this run. `.local-inputs/windows-source-clock-v19-preview-12-candidate` no longer exists; the same build is now `.local-inputs/windows-source-clock-v19-preview-12/` (EXE SHA256 `1D43942ECD52DF7B86BDF77BA2E5CB306AFC76944813F38CA7CE9E6CCB252672`, matches the brief). I ran a copy at `.local-inputs/claude-preview13/A-run/exe/dh-foundation.exe`.

## 1. Evidence

### 1a. Slot-4 lead (source allocator)

Logic:
- `features/frontend/creation/source_indexed_slot_service_v1.cpp` (`create_metadata`, ~lines 76-84): loops `candidate` 0..3, `slot` starts at -1, and `if(slot<0) fail("No free source campaign profile slot")`. Code reading: when all four slots are occupied the function returns failure, it does NOT return 4. The `campaign_profile_exists_v1` probe errors are also propagated.
- The service has no production caller (`grep SourceIndexedSlotServiceV1` finds only its own files), and `source_indexed_slot_service_v1_tests.cpp` has no all-occupied or metadata-allocator case (it tests the assignment receipt only).
- Original (IDA, ARM32 pseudocode): `NativeCreateSaveSlot` at `0x43f630` calls `PlayerSavegame::SG_GetNextFreeSlot` at `0x4660c8`. That returns the count of contiguous existing slots from 0 (first free slot in practice). With four occupied it returns 4, and `NativeCreateSaveSlot` then creates and saves slot 4 with no bound check. So the source itself does not refuse; refusal in the source comes from the menu gate (Single Player opens name entry only for an empty selected slot). I did not verify that AS gate in IDA; the port encodes it in `flow/menu_flow.cpp` `single_player`.
- Production path (`main.cpp` ~499-527): `select_new_profile` uses the arrow-selected slot (`selectedMenuSlot`, 0..3), refuses an existing file, and `slotPath` throws for slot >3. `menu_flow.cpp` `confirm_class` latches `result_unmapped` for a created slot outside 0..3, and `select_adjacent_slot` bounds 0..3.

Conclusion: the port never returns slot 4 in a reachable path. Full-slot creation is refused at three layers. No allocator change is needed for B007.

### 1b. Creation and arrow behaviour, deterministic runs (candidate EXE, isolated saves)

Run with `--start-mode menu --assets <pkg>/assets --save <isolated>/saves/profile.savegame --save-slot N --menu-actions "<dispatches>"`. Scripted actions dispatch sequentially before the frame loop (they bypass key/pointer batching). Results are from real EXE runs:

| Branch | Result |
|---|---|
| All free, create slot 0 (Rogue) | exit 0, `menu_StartGame`, `profile.savegame` written |
| Some occupied, create in empty slot 2 | exit 0, `profile-slot-2.savegame`, `menu_StartGame` |
| Selected slot occupied, Single Player | goes to `menu_StartGame`; name accept refused ("Source button is not active"), exit 1 |
| All four occupied, Single Player at slot 0 / Back | `menu_StartGame`, Back to `menu_MainMenu` OK |
| All four occupied, Right x3 then Left x3 | ends at slot 0, exit 0 |
| All four occupied, Right x4 | 4th Right refused, exit 1 (bound holds) |
| All four occupied, Single Player at slot 3 | goes to StartGame, creation not reachable |
| Delete slot 0 then Single Player, create | file renamed `profile.savegame.removed-<t>`, recreate succeeds, `menu_StartGame` |
| Delete, Refuse, Delete, Accept, recreate | OK |
| Delete slot 0, move Right to occupied slot 1, create | refused (exit 1), as designed |
| Create, Right, Back (class->name), Back (name->main), Single Player, create again | OK, `menu_StartGame` |
| Seed sweep: `--combat-seed` 1..10 x Knight/Rogue/Mage (30 runs) | 30/30 reached `menu_StartGame`, no seed-dependent creation failure |

So deterministic creation, back, remove, recreate and bounds behave as designed on the candidate build. The intermittent symptom is not from these paths.

### 1c. Class-select arrows (the real defect)

Logic:
- `features/frontend/input/screen_interaction.cpp` `key()` checks the class-arrow gate `enabled(path)` at key RELEASE time, against the class index before the frame's `flush()`. `flush()` then dispatches the queued releases in order, but the gate was already decided on stale state.
- Probe at HEAD (`.local-inputs/claude-preview13/A-build/probe/class_arrow_probe.cpp`, source `screen_interaction_HEAD.cpp`): from index 2, Left+Right+Right released in the same frame gives index 1, not 2. The second Right is valid after the Left, but was dropped. From index 1, Left+Left in one frame gives `flush()` false with "Source button is not active" and drops the rest of the frame.
- Frames batch releases when the frame stalls. `frontend_presentation_v1.cpp` calls `loadClass()` (blocking) on class change, so batching is plausible. That link is inference, not observed.
- Original semantics (IDA): `MenuCharacterSelect::OnEvent` at `0x4281b8` tests the live index per event. Right increments only when index `<=1` (`this+0xEC`), Left decrements only when `>0`. So per-press live evaluation is the source behaviour. The port's batched gate is a port artifact. Key input is a PC adaptation of the original touch/drag event path; only the bound semantics are matched here.
- Visual: I did not capture a class-select screen from the reference video (see Uncertainties).

### 1d. Reference video

Checked `dh2_video_research/sheets1/s001.jpg` (Gameloft logo, intro), `sheets1/s004.jpg` (intro cutscene, 1:12-1:32), `sheets2/s001.jpg` (Part 2 start, gameplay). No creation or select screen was in those sheets. I did not find the creation screens in the sampled sheets; the visual check of arrows is NOT done.

## 2. Expected behaviour and minimal implementation

Expected: each release of Left/Right/Confirm on the class screen is applied in order against live state. A release whose button is not enabled at that point (for example a bound arrow after an earlier press in the same frame) is ignored silently. A non-bound press is not lost because another press landed in the same frame.

Implementation (two hunks, no shared-file change):
1. `key()`: queue every SelectClass release without the stale gate check.
2. `flush()`: skip a queued release whose `enabled(path)` is now false, then dispatch the rest in order.

No slot allocator change (see 1a).

## 3. Changes

- `port/windows-foundation/features/frontend/input/screen_interaction.cpp`
  - `ScreenInteraction::key`, SelectClass branch (after `if(screen_=="menu_SelectClass"&&(vk==0x25||vk==0x27||vk==13))`): the `if(enabled(path))` gate on release was removed; comment added.
  - `ScreenInteraction::flush`, the `for(const auto& path:pc_paths)` loop: added `if(!enabled(path))continue;` before dispatch; comment added.
- `port/windows-foundation/features/frontend/input/screen_interaction_tests.cpp`: three same-frame assertions added after `check(!ui.enabled("menu_SelectClass.btn_right"),"right bound visible");`.
- New: `coordination/claude-preview13/A-report.md` (this file).
- Scratch (git-ignored): `.local-inputs/claude-preview13/A-build/`, `.local-inputs/claude-preview13/A-run/`.
- Not touched: `main.cpp`, `CMakeLists.txt`, the tracker, the user's saves, the Preview 12 package. No `git` write operations.

`git diff` for the two files is 16 insertions and 2 deletions. Line endings are unchanged (git CRLF warnings are the repo's usual warning).

## 4. Tests

Build folder: `.local-inputs/claude-preview13/A-build/`. Compiler: `.local-inputs/windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe`, `-std=c++17 -Wall -Wextra -Werror`. One deviation: `-Wno-missing-field-initializers` is needed for the existing `menu_flow_tests.cpp` (it has pre-existing `-Wmissing-field-initializers` errors under `-Werror`; unrelated to this change). `menu_flow_tests.cpp` also needs `flow/presentation.cpp` and `flow/canonical_navigation.cpp` linked.

Commands and real output:
- `clang++ ... -o A-build/menu_flow_tests.exe flow/menu_flow.cpp flow/presentation.cpp flow/menu_flow_tests.cpp flow/canonical_navigation.cpp` then run: `PASS menu_flow: 91 assertions`, exit 0.
- `clang++ ... -o A-build/screen_interaction_tests.exe input/screen_interaction_tests.cpp input/screen_interaction.cpp input/frontend_input.cpp flow/menu_flow.cpp flow/presentation.cpp flow/canonical_navigation.cpp` then run (with fix): `PASS original frontend paths: QWERTY/shift/space/delete/cap/nameaccept/classbounds/Confirm/service failure`, exit 0.
- Same test with the HEAD `screen_interaction.cpp` (`A-build/probe/screen_interaction_HEAD.cpp`): `same-frame Left,Right,Right dropped a press`, exit 1. The new assertion reproduces the bug; the fix passes.
- Probe (`class_arrow_probe.exe`), HEAD source before fix: B `left+right+right same frame: flush=1 idx=1`; C `left+left same frame at 1: flush=0 idx=0 err='Source button is not active on the current menu'`. After fix: B `idx=2`, C `flush=1 idx=0 err=''`. Sequential presses (A) unchanged: 0->1->2, third press at bound silent.
- `run_source_indexed_slot_service_v1_tests.ps1 -OutputDirectory .local-inputs/claude-preview13/A-build/slot`: `PASS indexed slot public loan contract; no fake native creation success; checks=23`, exit 0.
- `run_profile_create_cancel_remove_regression_v1.ps1`: NOT RUN. It links `.local-inputs/frontend-feature-build/libfrontend_components.a` (dated 11:40, which the B007 row calls stale). Root must rebuild the archive before it can count as evidence. The fix is in `input/` and is part of that archive.
- Candidate EXE runs listed in 1b (isolated saves, copied EXE). These predate the fix and cover the baseline only.

Not run: any integrated test of the fix in the real window. Scripted actions dispatch directly and bypass the key-release path.

## 5. Package files required

None. The change is code only. It adds no runtime asset. Nothing regenerated (no exporter run; no generated file diffs).

## 6. Uncertainties and not verified

- Real-window intermittency: not reproduced end to end. The probe proves the batching defect at unit level; whether a user sees it depends on frame stalls (inference).
- Pointer clicks: the hit-test closure in `frontend_presentation_v1.cpp` (`set_surface` callback, ~line 322) evaluates `interaction.enabled()` at pointer event time, before the frame's `flush()`. Two clicks in one frame on arrows have the same stale-gate risk. NOT fixed here (shared presentation loop, small hunks only). Root should decide: flush after each pointer event, or re-evaluate the hit at flush. Not tested.
- Profile arrows: mouse path has the same caveat (see above). Keyboard on MainMenu uses focus, not batched arrows.
- Creation slot choice divergence: the source creates in `SG_GetNextFreeSlot` (first hole), while the port creates in the arrow-selected slot. These can differ (for example slot 0 occupied, selected slot 2 empty, first hole 1). Not changed; fidelity question for the fidelity agent.
- `SourceIndexedSlotServiceV1` is unwired. Its allocator uses `dh2_NNN.savegame` (`campaign_profile_exists_v1`), while production uses `profile.savegame` / `profile-slot-N.savegame`. If it is wired later, these must be reconciled. No change now.
- Video: creation/select screens not located in sampled sheets (see 1d). The class-arrow visual and the creation screens are NOT checked against the reference.
- The `Update` function (`0x428498`) named in the brief was not examined; `OnEvent` (`0x4281b8`) was.

## 7. Verifier script

Integrated EXE (after root build), isolated saves only. Use a fresh folder `<iso>` and copy the package assets reference (`<pkg>` = the Preview 12/13 package `assets` folder).

1. Creation route, empty slot 0 (expect pass):
   `dh-foundation.exe --start-mode menu --assets <pkg>/assets --save <iso>/saves/profile.savegame --save-slot 0 --menu-frames 2 --menu-capture-directory <iso>/out --menu-capture-every 1 --menu-actions "menu_MainMenu.btn_MENU_SINGLE_PLAYER|text:Hero|menu_EnterName.buttons.btn_Accept|menu_SelectClass.btn_right|menu_SelectClass.btn_Confirm"`
   Expect exit 0, stdout line `"menu":"menu_StartGame"`, file `saves/profile.savegame`. Expected from 1b.
2. Occupied-slot refusal (expect exit 1): copy a profile into `saves/profile-slot-1.savegame`, run with `--save-slot 1` and the same actions. Expect exit 1 with "Source button is not active on the current menu".
3. Class-arrow fix, real input (NOT possible via `--menu-actions`): the verifier must send Left and Right key events in one frame on `menu_SelectClass` (index 2 start). Expect the class index to end at the value the presses imply (Left,Right,Right from index 2 ends at 2). Before the fix the second Right is lost. Log the stderr line "Required source frontend operation failed" as the failure signal.

Frame logs to capture: `run.log` JSON line with `"menu"`, `run.err`, and the PPM frames under `<iso>/out`.
