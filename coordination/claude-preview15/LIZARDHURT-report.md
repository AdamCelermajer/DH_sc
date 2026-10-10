# LIZARDHURT: B028 hurt cue (uid 284) report

Branch p15/lizardhurt, worktree DH_wt/lizardhurt. Status: hurt cue submitted in the EXE; audibility not verified.

## Premise correction (verifier assets3)

- The verifier said `make_combat_audio` has no caller. It is called: `RuntimeSessionAudioV1::bind` (features/audio/runtime_session_audio_v1.cpp, `host_->make_combat_audio`), and `start()` calls `bind()`.
- The hurt cue is not a CharSounds hit-list entry. It is the AnimTable step-0 sound of the Injured reaction clip. The verifier's runs never injured a lizard (no `outcomes=0x10`), so uid 284 was never reached. The hook was already generic; nothing was missing in wiring.

## Original evidence

- IDA: `F_ApplyResult` (3b10b4 / 3b01b8) calls `SM_SetInjureState` on outcome bit 0x10, then `F_ApplyCombatSound` (3afee0 / 3afd38). The injury clip is the Injured state.
- AnimTable: row 377 `LizardMan_Injured`, step 0 Sound 477 -> binding uid 284 `sfx_lizardman_hurt.wav`. Row 374 `LizardMan_Died`, step 0 Sound 476 -> uid 285 (assertions already in features/audio/runtime_combat_audio_v1_tests.cpp).
- Injury gate: `ActorCombatRuntime::start_injure_reaction` (actor_combat_runtime.cpp) starts the react pose only on bit 0x10.
- Reaction and death step entries are published per admitted pose (combat_session.cpp, `poseStepEntries`) and notified at clip start from `pose_motion_binding`. `RuntimeAttackSoundV1::dispatch` submits `step.sound` for any sequence.
- Prior EXE run (preview 13 B037 hold36, seed 1234, `.local-inputs/claude-preview13/verify-final/runs/b037-hold36-R/run.log`): `sequence=377 step=0 sound=477` already reached the observer at the injured reaction start; status 4 only because the preview 13 package lacked the WAV.
- Injury occurs for lizards hit by the Knight (the lizard is the victim), not for the Knight hit by Type1 lizards (J-report).

## Changes (small, generic)

1. `combat_session.hpp`: `enum class CombatSessionStepRole {action, hurt, death}` and `role` on `CombatSessionStepEntry` (default action).
2. `combat_session.cpp` (~line 1286): reaction/death pose step entries set `role` (hurt for `policy.reaction`, death otherwise). Covers any actor with an Injured/Died row.
3. `features/audio/runtime_attack_sound_v1.{hpp,cpp}`: diagnostic carries `role`.
4. `features/audio/runtime_audio_host_v1.{hpp,cpp}`: `source_uid(ordinal)` (diagnostic only, via bindings row uid).
5. `features/audio/runtime_session_audio_v1.cpp` (attack diagnostic callback): for hurt/death steps with a sound, prints
   `Combat cue uid=<uid> event=<hurt|death> actor=<id> frame=<n> status=<submitted|asset_missing|failed>`.
6. `tests/combat_session_pose_step_tests.cpp`: asserts role hurt on the Injured leaf and role death on the Died leaf. This test was not registered in CMake, so I registered it (`combat_session_pose_step`, CMakeLists.txt).

## Tests

- `p14_build.ps1 -Name lizardhurt -Test`: build exit 0. ctest 116/117 pass. Only `session_skill_binding` fails, the known worktree junction issue.
- `combat_session_pose_step_tests.exe <rc3 assets>`: `PASS direct and retained same-Session original Injury/death step entries ...` (includes the new role assertions).

## Verifier script (quiet, hidden, silent; EXE = DH_wt/build-lizardhurt/dh-foundation.exe)

Args: `.local-inputs/claude-preview15/lizardhurt/inj<seed>/run.args` (B037 hold36 scenario, repointed to the rc3 assets, `--audio --audio-assets .../windows-source-clock-v19-preview-15-rc3/audio-assets`, 260 frames). Jobs: `.local-inputs/claude-preview15/lizardhurt/jobs-post.json`, run with
`quiet_run.ps1 -JobsFile jobs-post.json -Parallel 8 -Summary summary-post.json` (seeds 1234, 1-7).

Observed (all exit 0, no exception/assert/error lines):
- seed 1234: `sequence=377 step=0 sound=477 status=1`; `Combat cue uid=284 event=hurt actor=7118915781085668844 frame=167 status=submitted`; death `uid=285 ... frame=199 status=submitted`.
- seed 2: `Combat cue uid=284 event=hurt actor=4141719441445850732 frame=184 status=submitted`; death uid 285 submitted.
- seeds 1, 3-7: no hurt cue (no injury); death cue `uid=285 status=submitted` in every seed.
- Totals: hurt uid=284 submitted x2, death uid=285 submitted x9.

Expected for a verifier: any run with `sequence=377` must show one `Combat cue uid=284 event=hurt ... status=submitted`.

## Package files required

- `audio-assets/data/sounds/sfx_lizardman_hurt.wav` (uid 284), `sfx_lizardman_die.wav` (285), `sfx_lizardman_attack_1.wav` (282). All present in rc3.

## Open risks / not verified

- Audibility not verified (silent mode, dispatch only).
- Injury is rare for the level's lizards (2 of 8 seeds). Injury through a Type3 lizard hitting the Knight was not reproduced in the EXE; the Type3 placement in corner_ruin_ws_00 is not instantiated by the current condition policy.
- The hurt cue fires only when the Injured clip starts, following the source gate (3000 ms, one per admission). That matches the source; no rate change was made.
