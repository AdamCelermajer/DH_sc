# AVATARPOSE report (B051: Equipment avatar frozen mid-action)

Status: IN PROGRESS (skeleton, updated as work proceeds).

## Evidence
- Reproduction (quiet batch, pre-fix EXE `DH_wt/build-avatarpose`): `attack-equip` (Space 30..41, Equipment page at frame 36, capture frame 100) shows the avatar frozen with arms raised mid-swing; `idle-equip` (no Space) shows the idle stance. Captures: `.local-inputs/claude-preview15/avatarpose/shots/attack-equip.png`, `idle-equip.png`.
- Mechanism: `main.cpp` sets `gameplayPaused = characterMenu.is_open()`, so `combatSession->update` is not called while the Equipment page is open. The avatar is drawn from the live retained Scene (`RuntimeEquipmentBindingV1::with_preview_borrow` -> `VisualSkinOwnerV6::draw_views`), whose pose is frozen at the last published (attack) frame.
- Original (IDA `pseudocode-all.c`): `MenuCharMenu_InvMain::CreateAvatarCamera` (0x4528d0, Show at 0x452b1c) calls `SM_SetIdleState` on the local player when the camera is created. `MenuCharMenu_InvMain::RenderCharacterPane` (0x452468) calls `CharAnimator::Update` and advances the scene with `Application::GetDt` every render, so the pane shows an idle clock that runs while the page is open.
- Reference frame `.local-inputs/b019-original-equipment/511_5.png`: idle stance, sword held low.

## Plan
1. CombatSession: `with_locomotion_preview_pose` publishes the bound idle locomotion clip at the pane's own clock for the synchronous draw and restores the live pose afterward (no Session time, policy or attack ownership change).
2. RuntimeEquipmentBindingV1: pane clock (`restart_preview_clock` on page open, `advance_preview_clock(dt)` per frame using real dt); preview draw goes through (1); live attachment sockets and render-change flags are restored after the draw.
3. main.cpp: call the clock hooks at the pause transition and each frame.
4. Test: combat_session_tests checks the selected preview clip is the idle alias clip while combat owns the attack pose, and that the live pose and attack clip are unchanged afterward.
