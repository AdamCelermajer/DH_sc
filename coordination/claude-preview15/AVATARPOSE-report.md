# AVATARPOSE report: B051 Equipment avatar frozen mid-action

Status: fix implemented and verified in the worktree EXE (Knight only). Class matrix and Details-page capture not run (see Open risks).

Branch `p15/avatarpose`, commits: `c9c59b1f` (skeleton), `b1c1aa77` (fix). Build: `DH_wt/build-avatarpose/dh-foundation.exe`.

## Evidence

### Reproduction (pre-fix EXE, quiet batch)
- Job `attack-equip`: Space held frames 30..41 (`--space-key-interval 30:12`), Equipment page opened at frame 36 (`--equipment-page-frame 36`), capture at frame 100. Capture: `.local-inputs/claude-preview15/avatarpose/shots/before-attack-equip.png`. Observed: avatar frozen with both arms raised mid-swing, no idle stance.
- Control `idle-equip` (no Space): `shots/before-idle-equip.png`, idle stance with sword raised.
- Mechanism (code): `main.cpp` sets `gameplayPaused = characterMenu.is_open()`, so `combatSession->update` stops while the page is open. The pane reads the live retained Scene through `RuntimeEquipmentBindingV1::with_preview_borrow` and `VisualSkinOwnerV6::draw_views`, which sample the Scene's current pose. The Scene is frozen at its last published attack frame.
- User screenshot `equipment-avatar-midslash.png` matches this mechanism.

### Original (IDA `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`)
- `MenuCharMenu_InvMain::Show` (0x452b1c) calls `CreateAvatarCamera` (0x4528d0). When the camera is created and the player is not dead, that function calls `CharStateMachine::SM_SetIdleState` (0x3c1a00) on the local player. Opening the page therefore returns the actor to idle.
- `MenuCharMenu_InvMain::RenderCharacterPane` (0x452468) runs on every rendered frame. It calls `CharAnimator::Update` on the local player, advances the root scene with `Application::GetDt`, and resyncs rotation. The pane therefore plays a live idle clock while the page is open. Original gameplay is not stepped by this call.
- Reference video (`Dungeon Hunter 2 (v1.0.3) Part 1`, t=511.0 and 512.5, `.local-inputs/b019-original-equipment/511_5.png`): idle stance, sword held low, pane camera framing. Direct observation from sampled frames only. These frames cannot show animation continuity.
- Uncertainty: the reference sword angle (low) differs from the port's idle with Longsword (raised, as in the user's accepted Details screenshot `equipment-torso-2.png`). Not resolved. Recorded as an open fidelity question, not as a rule.

## Changes
1. `port/windows-foundation/combat_session.hpp/.cpp` (shared core, small additive hunk): `CombatSession::with_locomotion_preview_pose(ActorId, alias, seconds, draw, error)`. It finds the bound explicit alias (player `idle`), samples the authored clip at `start + fmod(seconds*1000, span)`, applies that pose to the actor visual, runs `draw`, and restores the exact live pose through `apply_local_pose`, the same path `RetainedPosePlayback` publishes through. Session time, locomotion policy, attack ownership and the actor's clip are not changed.
2. `features/equipment/runtime_equipment_binding_v1.hpp/.cpp`: `with_preview_packets` draws inside that overlay. After the draw it re-syncs live attachment sockets and restores `render_revision` and `render_change_pending`, so the overlay raises no render change. New `restart_preview_clock()` and `advance_preview_clock(seconds)`.
3. `port/windows-foundation/main.cpp` (shared, 4 lines): restart the pane clock on the pause transition (the original Show resets idle); advance it each frame by real `dt` (the original RenderCharacterPane uses real Dt; the gameplay `dt` is zero while paused).
4. `port/windows-foundation/tests/combat_session_tests.cpp` (+33 lines): records the idle preview clip while the actor is idle. Then, while the attack owns the pose, it checks: the preview clip equals that idle clip for three times, each published pose equals the sampled idle pose, the live pose is restored exactly, and combat ownership is unchanged.

## Tests run
- `p14_build.ps1 -Name avatarpose -Test`: build exit 0. ctest 110 of 111 pass. The only failure is `session_skill_binding`, the known worktree junction issue (brief says ignore).
- Focused: `combat_session_tests.exe <shared-assets>` exit 0. Also `combat_session`, `combat_session_actor_transition`, `equipment_adapter`, `equipment_main_page`, `equipment_inventory_actions`, `combat_session_equipment_multiplicity` pass.
- Mutation check: disabling the `apply_local_pose(presented)` call makes combat_session_tests fail with "Equipment preview did not publish the idle sample it selected". The file was restored and re-verified (exit 0).

## Verification in the EXE (quiet batch, post-fix)
- `attack-equip` (same args): `shots/attack-equip.png`. The avatar stands in the idle stance with the sword held up, matching the control. PSNR between post-fix attack-equip and idle-equip is infinite (identical pixels). Before fix vs idle control: PSNR about 24.
- `late-attack-equip` (same args, capture at frame 160): `shots/late-attack-equip.png`. The idle is still idle, and the pane clock advances: PSNR 28 against frame 100, so the pose is not frozen.
- Logs: `shots/attack-equip.log`, `idle-equip.log`, both exit 0, no equipment or pane errors.

## Package files required
None new. Uses existing Preview 14 package assets (`windows-source-clock-v19-preview-14-rc1`).

## Verifier script
Jobs: `.local-inputs/claude-preview15/avatarpose/jobs.json` (`attack-equip`, `idle-equip`) and `jobs-late.json`. Run from the worktree:
`powershell -NoProfile -File port/windows-foundation/tools/quiet_run.ps1 -JobsFile <jobs.json> -Parallel 2 -Summary <out.json>`
Expected: exit 0 for all jobs. `attack-equip.png` shows idle, not a swing. `idle-equip.png` shows idle. `attack-equip.ppm` and `idle-equip.ppm` are pixel-identical. Before-fix `before-attack-equip.png` shows a swing.

## Open risks / not verified
- Only the Knight profile is verified. Rogue and Mage idle clips are not captured. The clip comes from the class/stance-bound `idle` alias, so it is class-aware by construction, but the EXE has not confirmed this.
- The Details page capture is not run separately. The same `with_preview_packets` path draws both.
- Gameplay after closing the page (resume, attack continuity, root-motion restore with `consume_root_motion`) is not captured in the EXE. The unit test covers the restore on a non-root-motion fixture only.
- Idle stance fidelity: the reference sword angle differs from the port's Longsword idle (see Uncertainties above).
- Shared core files touched: `combat_session.hpp/.cpp` and `main.cpp` (small hunks). Integration owner should review these hunks.
- B019 stays open; this fixes only the frozen pose.
