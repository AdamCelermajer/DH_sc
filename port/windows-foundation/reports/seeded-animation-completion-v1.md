# Seeded Injury and death animation completion

Investigation before edit. The user's report says a dying mob stops in midair.
The supplied reference is https://www.youtube.com/watch?v=z_Zky7qQdYs&t=377s.
Root inspected the existing original-footage contact sheet
`.local-inputs/fidelity-hit-reaction-audit/overview-365-385.png`: it shows a
combat encounter, an enemy death/reward and subsequent tutorial frames. Those
coarse frames do not establish the exact death clip end or physical detachment
frame, and the visible enemy is not the Lizard used by the regression. We must
preserve continuous authored death playback rather than infer an end at HP zero.

Recovered logic is stronger for this internal notification: source
`actor_blended_playback.cpp::animator_phase` consumes genuine applicator
completion through the original animation scheduler. Its `scheduler_event`
closes the sequence before delivering decimal34/0x22. The actual CSDead
OnEvent at0x3c4c3c handles End34 by SetPhysicalObject(null,false), reflected in
`character_state.cpp::event_body`: only state12 removes the body, then starts
the NPC despawn timer and changes flags. HP loss/entry is not completion.
CSInjured OnEvent0x3c0044 is empty, but source CharacterStateOwner registration
routes End34 from state11 to Idle3 afterward; see the same-FSM proof in
`tests/player_injury_same_fsm_v7.cpp`. Injury completion must not perform dead
body removal. Numeric animation routing belongs to the current Session's
existing notification services, not a second native actor graph.

Current defect: `RetainedSequencePlayback::advance_seeded` delivers frames and
takes actual completion but only handles repeats. It never delivers the final
sequence event. Full `advance` already invokes its separate `closed` callback.
The Session retained binding uses seeded playback for accepted incoming poses,
so runtime can observe `current_ended` while the notification consumer gets no
End34. Restored corpse playback is terminal-held without advancing and must
remain silent.

Minimal reusable fix: append an optional seeded-completion callback, separate
from full action `closed`. A finite source `seed_sequence` invokes it exactly once
on genuine final completion, after events/motion and after consuming the end
latch. Positive source repeats close only after their last selection; infinite
repeats and raw `seed` compatibility paths do not close. Consume before callback so failure
or reentrant selection cannot replay the old end. Session installs the callback
to its existing numeric event34 router only, preserving full-action cleanup and
the prior action's finished flag. The actual consumer owns FSM/body effects.

Verification defined before coding: actual same-Session Lizard Injury/Died
assets, lethal/nonlethal original result, continuous16ms playback and the real
pose completion latch. No End34 at HP loss/entry or before actual end; exactly
one end at final authored completion; no subsequent replay or save/rebind
replay. Injury end has no dead removal. Low-level finite repeats, raw/infinite
loops, callback reentry and callback failure need focused coverage. Existing
attack/source-departure/incoming and retained animation checks must still pass.
Production body detach/GPU timing requires its normal main consumer and a fresh
runtime check; the event regression alone does not prove complete death parity.

Implementation and independent verification now pass. Root inspected the new
low-level test, PASS log and executable C2062CE03EF5C05D4EA00D77498D317D499BBD8EF7250ADB3C3EA69BD1899C61.
It verifies separate full-action close, finite and positive-repeat completion,
infinite/raw compatibility suppression, frame-before-close, completion reentry
and failure without end replay. The actual Session regression failed before
the core edit (both Injury and Died ended without any numeric/state End34),
then passed with executable A4D0B7744255332BEACBAF34CA638C07E47C6B26A8CC28A90B4F9DF2E3FEAB24.
Root inspected its source and log/hash: Injury1033ms emits one event/no Dead
state observation; Died800ms adds one at genuine current_ended, retains authored
root movement through completion and does not replay after terminal/rebind.
The test's Dead-handler count is an observation, not actual physical removal.
Production state consumer/body detach and normal gameplay remain integration
gates. Lead owns the next coherent build and focused shared regressions.

Production consumer now verified on frozen executable
3F31F063DA7692B57FC291E4B9EB58FC837DD0BB38241E1CC7D83F8C4BBDE161:
`.local-inputs/v19-frontend-hotfix/death-end-main/rogue-kill.args/.log/.png`.
The normal menu-selected Rogue attacks and kills Lizard7118915781085668844,
then End34 at Session serial134 changes physicalBefore1 to physicalAfter0 while
retained pose remains1. Root checked the exact executable hash/log and inspected
the rendered Rogue/two daggers, surviving target ring and visible corpse. The
180-frame run is integrated death completion/body-detach evidence, not original
death-image parity, collision Step, complete despawn or persisted body presence.
Saved mid-death/terminal body reconstruction and original-video frame comparison
remain open. CTest91/92 pass alongside the eight other focused coherent checks.
The subsequent complete coherent suite passes92/92 (25.02s); root inspected
the final LastTest.log, including both new tests. Preview9 remains the accepted
release; this executable is frozen development evidence. Next root-owned
durability gap: after a genuine death end removed the body, current R/F9 rebuild
recreates a body for every authored actor. Persist the actual completion/body
fact through the current world/save owner; HP0 alone does not distinguish an
unfinished death from a terminal corpse, and restore must not replay End34.
