# Skill Post to Space attack reentry audit

## User report and evidence boundary

The reported behavior is that a basic Space attack is unavailable after using
key 1. This is a reported current-game failure; actor, exact skill, timestamp,
weapon/equipment, and whether the attempted Space command reached the game loop
were not supplied. Existing v1.0.3 footage observations in
`docs/TARGETING-SKILL-REFERENCE-2026-10-06.md` show skill/attack visuals and
target HP changes around 4:41.995–4:46.995, but do not show a Skill Post followed
by a PC Space command. Those frames do not establish the reported transition.

The intended PC input is a host adaptation. Recovered `HUDControls::Update`
(`0x41a780`) routes held attack with no object of interest to `Cmd_Attack`
(`0x405b04`) and clears the sticky-target byte. `Cmd_Attack` dispatches through
`Character::Ctrl_Attack` (`0x3ad87c`) to `CharAI::AI_DoMeleeAttack` (`0x3d01ac`).
The latter checks the current target with `AI_CanAttack` (`0x3d67f4`), then uses
the source target search and assigns a candidate if the current target is absent
or invalid. Evidence is summarized in
`port/level-world/reference/prince-live-ai/NOTES.md` and
`port/windows-foundation/reports/combat-session.json`.

Skill Post may deliberately clear target state: the recovered authored
`character-skill-state-v4/cast-lifecycle-v46.md` records unconditional
`ClearTarget` for BashDown and GroundSlam, while Charge clears only when
`TargetInMeleeRange` is false. ClearTarget is `SetTarget(NULL,false)` followed
by `SyncLastTarget` on the same source owner. A succeeding Space press must
therefore be tested with normal reacquisition from a cleared target; the test
must not restore a manually selected actor before sending Space.

## Focused regression definition

Use a fresh single-session Knight fixture with its actual BashDown source bank
and a living, valid in-range Swamp Lizard. Resolve PC key 1 through the current
saved-slot mapping, run the actual retained Skill animation through its authored
`do_skill` Use marker and Post completion, and assert the cast receipt is
completed and its generation is no longer active. Assert Post leaves target
cleared. Then send a single PC-adapted Space attack with no target and assert
normal target search reacquires the living Lizard, admits the static
AttackStatic bank, enters the retained attack pose, and completes back to Idle.
Check HP/RNG changes only at authored markers, no stale Skill generation is
reused, and no duplicate Use result occurs. A second case should exercise the
production Rogue dynamic Attack/AttackStatic selection path after JumpKick.

Keep the existing PC key mapping checks and source result/outcome assertions
unchanged. The previous experiment called Space probes inside the shared
Warrior/Rogue fixtures and contaminated their ordered-target/RNG expectations;
those probes were removed. The existing linked coordinator runner now passes
with the complete source outcome checks intact.

## Current execution status

Investigation is complete; the isolated post-cast Space regression is not yet
executable. The stable build has no reusable CombatSession/libfoundation archive
and no retained `runtime_skill_activation_session_v1_tests.exe`. The existing
runner compiles `port/windows-foundation/combat_session.cpp` directly. The
dispatcher explicitly requires root clearance before starting another
Session-core compilation, so no new linked test compile was started. Root owns
any Session fix; the integration lead owns the production input hookup.
