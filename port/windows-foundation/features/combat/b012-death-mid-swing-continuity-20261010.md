# B012 lethal-in-flight death continuity probe

## Investigation before edit

The user report is that a dying enemy stops in midair or does not complete its
death animation. The nearby original reference is the v1.0.3 gameplay recording
`z_Zky7qQdYs`, sampled in
`.local-inputs/fidelity-hit-reaction-audit/` at 376.500, 376.750, 377.000,
377.250, 377.500, and 378.000 seconds. At 376.500 the player and a nearby
enemy are in combat with damage numbers and XP text; 376.750 shows a changed
player pose and additional damage/XP text; by 377.000–377.250 the player is in
the level-up effect with `+5 EXP`; at 377.500–378.000 the enemy is no longer
clearly visible in the sampled view. These are direct observations from spaced
frames, not a continuous death-clip sequence. The frames do not identify the
enemy's selected Died leaf, show its full fall, or locate the body-detach frame.
The recording therefore cannot establish the reported midair freeze. The
recovered gameplay assets are v1.0.2; the reference is v1.0.3 and no version
delta is established.

Recovered source evidence is in
`port/level-world/reference/character-state/NOTES.md` and its captured ARM
functions for original library SHA256
`36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80`.
The death caller chooses the Died sequence and stores the `State.animation_override`
before entering Dead12; Dead focus at `0x3c4d50` consumes that override through
`SM_SetAnim(-1)`. The exact visual leaf remains gated by the caller's lethal
result and source state transition; HP zero alone is not proof of animation
completion. The selected Lizard Died leaf is authored 0–800 ms, speed 1,
MoveGO=1, with root displacement measured as XY `[79.6437, 355.437]` and Z=0
in `reports/npc-root-motion-source.json`. Source `CSDead::OnEvent` at
`0x3c4c3c` handles numeric End34 (`0x22`) with `SetPhysicalObject(null,false)`;
the source body-removal action occurs at sequence completion, not at lethal HP.
Dead OnUpdate is empty. The source evidence does not define a gravity/floor
snap rule for this case.

Current host path: `ActorCombatRuntime::handle_applied_hit` synchronizes a
lethal result into `start_pose(Pose::death)`; the same visual binding selects
the configured Died clip and releases the prior attack pose. The retained
source binding advances its `RetainedSequencePlayback`; its finite seeded
completion calls `CombatSession::retained_services().seeded_closed`, which
routes numeric `0x22`. `RetainedSequencePlayback::advance_seeded` consumes the
completion once before that callback. Root motion reaches the same Session
motion handler during each authored sample. The Session continues to own the
same actor visual/pose data through terminal sampling; after End34, the source
body consumer may detach physical motion ownership. Rendered terminal-pose
continuity and body-detach appearance still require a normal executable run.

## Expected behavior and focused verification

The expected visible behavior is the selected authored Died clip progressing
continuously from its entry pose to its authored terminal pose, retaining that
pose until the source End34 consumer removes the physical body. Apply only the
selected leaf's authored MoveGO root motion through the current actor owner.
Do not add a floor snap, gravity, a fixed delay, or a repeat rule without source
evidence. If the normal renderer diverges, patch only the proven clock, pose
publication, or body-consumer seam.

The focused test was defined around the reported transition: start the actual
`Swamp_LizadMan_Type1` Attack leaf in a shared `CombatSession`; apply one lethal
source result while that attack owns the pose; verify it switches to the source
Died leaf; advance at 60 Hz to the exact 800 ms endpoint; verify MoveGO XY is
delivered without invented Z movement; then run four zero-time updates and
compare the published local skeletal pose and actor transform. The terminal
check expects a stable held image, not additional movement callbacks after the
source End34/body-detach boundary.

The new focused test and runner are
`features/combat/b012_death_mid_swing_v1_tests.cpp` and
`features/combat/run_b012_death_mid_swing_v1_tests.ps1`. Strict C++17 compile
and isolated run passed:

```text
PASS lethal mid-swing clip=actor-2/Died/sequence-0/phase-0 range=0..800
terminal=held deathMotionSamples=108 appliedXY=92.084,366.139 actorZ=17
```

This is focused same-Session clock/pose evidence. It does not run the ordinary
Windows gameplay renderer, bind the real Dead-state body-destruction provider,
or prove the reported midair visual is fixed. The earlier idle-target root-motion
test also passes, but is weaker for the lethal-in-flight handoff.

## Integrated capture request and disposition

On a fresh isolated test save and frozen normal executable, record a continuous
capture from before a Rogue JumpKick (`SkillList` position 0, source JumpKick
sequence 521, authored `do_skill` event) lethally hits a Lizard through at least
one second after its Died endpoint. Include frames at impact, Died entry,
roughly 100 ms, 400 ms, 700 ms, the exact source completion/End34 frame, and
one second later. Log the tested executable hash, source actor ID, selected
Died clip/slot/generation, current source timestamp and `current_ended`, authored
root delta and MoveGO, actor XYZ, physical-body presence before/after End34, and
whether the same terminal pose remains rendered. Repeat with a lethal hit while
the Lizard itself is in Attack if the normal AI can reach that branch. After
save/reload, verify the terminal corpse does not restart or repeat root motion.
Use a separate test window/save; do not control the user's live game.

No gameplay change is proposed because the focused lethal-in-flight continuity
probe passes and the original sampled frames do not establish a contradictory
death rule. If the normal capture shows timeline progression ending early,
inspect the active Session clock/pose owner first; if it reaches 800 ms but the
rendered model freezes at the wrong sample, inspect terminal pose publication;
if only physics/body state differs, inspect the normal End34 consumer. These
branches constrain a future fix without guessing gravity or changing the
authored death clip.
