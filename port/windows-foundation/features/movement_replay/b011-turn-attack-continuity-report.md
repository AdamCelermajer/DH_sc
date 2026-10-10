# B011 turn and attack continuity evidence

## Result

No gameplay-policy change is justified by this pass. The new connected fixture reproduces a repeated camera-left arc for Knight and Rogue under W/S reversal, and that arc follows the recovered source frame order. Input release stops translation on the next neutral tick. The selected non-collinear target turns from source heading `+pi/4` to `-pi/4` during the retained attack pose. The first ten attack ticks emitted MoveGO callbacks but no nonzero attack root displacement. No source mismatch was reproduced.

Keep B011 open. The current result is a strict helper/Session trace. It does not match the user’s exact camera, input cadence, target, or frozen executable, and the source footage does not show a comparable W/S reversal or lateral attack sequence.

## Visual evidence

Reused the existing direct review of `.local-inputs/reference-video/dh2-act1/Dungeon Hunter 2 (v1.0.3) Part 1 [720p] [z_Zky7qQdYs].mp4`, recorded in `reports/session-wsws-movement-replay.json`:

- At 180–190 seconds, one-second samples show the movement tutorial, virtual joystick, player, and a separate touch-control instruction. This is direct evidence of the mobile interaction, not a W/S reversal.
- At 212, 212.5, 213, and 213.5 seconds, the virtual stick and HUD remain visible on the stone walkway. The sampled sequence contains no legible alternating input history, exact reversal, or lateral attack displacement.

The reference title identifies v1.0.3; the recovered assets used here are v1.0.2. No version-specific movement difference is established. The visual evidence therefore cannot determine whether the reported PC behavior differs from the original game.

## Logic evidence

- Original `Character::UpdateRotation` at `0x393710` applies the resolved rotation speed from property 47 and the strict one-step antipodal wrap branches. `reference/actor-rotation/NOTES.md` records the differential against the original function. At ordinary modifier 1, the rate is `4*pi` radians/second.
- The recovered live frame order is early scene/timeline/root displacement, then `UpdatePath` at `0x38ccc4`, `UpdateRotation` at `0x38cccc`, and `UpdateSubObjects` at `0x38ccd4`. Thus the sampled root displacement uses the current visual facing; the requested heading changes the facing later in that frame.
- `RootSceneNode`/visual root displacement rotates local root delta by current visual facing. A stride during a bounded reversal can therefore produce a world-space lateral arc. `reports/source-reversal-diagnosis.json` already reproduces that source mechanism with original Knight clips and reports no need for a lateral clamp.
- Original `LookTowards` at `0x393b1c` derives the source heading and `SetHeadingDirection` at `0x393be8` publishes it. The existing B010 regression separately covers the Session attack target admission path for Knight and Rogue.
- The PC adapter in `features/platform_input/semantic_input.cpp` maps W and S to opposite values on `move2D.y`; this is an intentional PC adaptation of the original mobile virtual-stick interaction.
- Current `main.cpp` samples retained root motion and applies it before the late source heading/rotation phase. It also uses the current-facing movement motor for root displacement. This focused test follows that order; it does not run the whole main loop or GUI.

## Expected behavior and uncertainty

Preserve the source bounded turn and current-facing root displacement while the requested direction changes. W and S remain opposite camera-forward commands under the PC adapter. Releasing the key removes movement intent and selects the idle clip. An admitted attack tracks the selected live target through source heading updates without replacing the retained attack pose. A lateral arc alone does not establish a fidelity defect because the recovered source predicts it.

Unknowns include the exact executable, starting facing, camera basis, key cadence, body scale, location, obstacle/floor admission, and attack phase from the report. No matched original reversal or attack-motion frame sequence has been identified. The test intentionally uses a clear admitted floor and a synthetic moving target; it does not infer wall, campaign, or native physics behavior.

## Focused verification

Added `b011_turn_attack_continuity_tests.cpp` and `run_b011_turn_attack_continuity_tests.ps1`. The runner uses a private output directory and the recovered v1.0.2 source asset set. It compiles with C++17 `-Wall -Wextra -Werror` and tests the actual Knight and Rogue visual plans in separate same-Session fixtures against a Lizard:

- Four 20-frame alternating W/S segments use the original Run and Walk MoveGO leaves, a non-aligned camera basis, retained player pose, source root callbacks, and the recovered bounded rotation oracle. Each of the three non-collinear 180-degree reversals settles within 16 frames.
- Releasing S produces neutral semantic input, selects the source Idle leaf, and produces no translation on the following tick.
- Target selection and ten admitted attack ticks retain the same Session actor/attack pose. The target begins at source heading `+pi/4`, then moves across the actor to `-pi/4`; the source heading/rotation kernels turn the actor to `-pi/4` without replacing the pose.
- Both class fixtures pass. Each has 93 motion callbacks, 79 nonzero root samples overall, a combined admitted W/S displacement of `(-415.322, -118.909)`, and camera-left projection `-377.759`. The first ten attack ticks have 12 MoveGO callbacks including target selection, but zero nonzero attack-root samples (`attackLocalRoot=(0,0)`).

Runner result: PASS for Knight and Rogue. The shared helper test `reports/session-wsws-movement-replay.json` remains the separate evidence for deterministic CSV replay and source clip phase-boundary behavior. Neither helper proves normal executable visual acceptance.

Source SHA256: `060B8914A9D5685A933203A1DED32D2F7B44317FA776AC7ADF96708580C0A73B`. Runner SHA256: `E1E42FBA707E84EA998C32DCEA783D6CDF6332D998E06D4D7D2BAF8B6D1D518C`.

## Required integrated proof

The integration lead should capture both classes on the same frozen normal executable with a recorded non-aligned starting heading, camera basis, exact W/S/attack key edges, target coordinates, and per-frame actor yaw plus root displacement. Include the matched original visual sequence if available. Compare the observed transition against the source order above before changing turn or root-motion policy. Preserve the exact executable hash and use an isolated save/window.
