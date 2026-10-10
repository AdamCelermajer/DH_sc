# Generic source sequence departure

Original visual evidence: inspected the existing 20-frame original-video contact
sheet `.local-inputs/fidelity-hit-reaction-audit/fight-374-376375.png` and its
timestamped report `reports/fidelity-player-hit-reaction-20261009.json`. Warrior
poses continue changing across a small HUD health decrement at375.25–376.25.
This is bounded ordinary melee evidence, not footage proving a faery cast exit.
The video is v1.0.3; recovered logic/assets are v1.0.2.

Logic evidence: original exported IDA pseudocode is
`.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`.
`_SetState`0x3c1938 runs outgoing Blur before incoming Focus. Cast Blur0x3c3934
raises33; Skill Blur0x3c434c raises31; both route to original Post without requiring
Use. Cast OnInit0x3c85b8 registers death50008 but no injury50010. Skill OnInit
0x3c8438 registers injury50010 via CSM_Interrupted0x3ad244, which reads state6
flags bit16. Skill Focus0x3c4480 writes0x6341; only ordinary non-boss monsters
receive bit16. Cast Focus0x3c39d0 writes0x6301. SM_SetInjureState0x3c5d84 writes
the3000ms gate before source clip lookup and either raises50010 or transitions
directly to11. F_ApplyResult0x3b10b4 derives that direct flag from result mask
at+28 &0x18000000. Recovered `character_skill_application_v116.cpp` and
`player_injure_state_v7.cpp` confirm the result layout and caller ordering.

Expected behavior: preserve normal damage, cast cursor and authored event timing
when no state exit is admitted. An accepted injury/death/explicit caller exit
cancels the existing retained sequence occurrence and calls source Post once
before incoming pose focus, even before Use. Stale events cannot resume after
recovery. The source program and its timers have a distinct checkpoint policy:
active playback and unpersisted cooldown reject; quiescent generic completion
does not permanently register campaign state or clear audio/permission services.
Legacy campaign/controller save guards remain intact.

Focused verification plan: use actual Knight and Lizard profiles/poses and the
original skill bank. Compare source result seeds for ordinary Injury50010 vs
direct-mask Injury; verify player Skill/Cast rejection, flagged Skill acceptance,
death acceptance, Post-before-step ordering, no late Use/completion, duplicate/
foreign/stale lease rejection, callback failure prefix, same retained owner and
RNG, active/cooldown/quiescent checkpoint gates and post-restore operation.
Also rerun existing direct/retained injury/death and source-hit regressions.

Limit: exact original faery interruption pixels, complete result statuses and
production coordinator/main acceptance are separate work. This change does not
reconstruct the original callback graph or infer a stun from HP loss.

Focused actual-Session runner now passes:
`tools/run_combat_session_source_departure_tests.ps1`, output
`.local-inputs/session-source-departure-test/run.log`. Real source Knight/Lizard
profiles, skill347, injury/death resources and original result calculation are
used. The test supplies controlled source formula Injury ratings135/182 to
exercise outcome branches; Lizard's authored ordinary rating is zero, so no seed
can produce its Injury branch without that explicit test control. This is not
production Lizard balance evidence. Ordinary/no-injury, playerSkill/Cast normal
Injury rejection, direct-mask injury, flaggedSkill admission, lethal exit,
pre/post-Use cancellation, callback exception prefix, foreign/stale/duplicate
occurrences and restore lease checks pass. Callback Post precedes incoming
pose-step observer;240 later frames do not replay Use or change RNG.

Active playback and outstanding transient cooldown reject checkpoints. A
quiescent pre-detach witness is captured before invalidating the feature lease;
rebind validates that witness and discards only generic callbacks, retaining
unrelated services. Active cast detach cannot forge the witness. Existing
campaign state17 still rejects saves. Exact production coordinator and GUI
acceptance remain lead-owned.

Independent review found a legacy cleanup bypass. `clear_lifecycle_services`
now checks generic quiescence before clearing anything; active playback,
outstanding cooldown and unadmitted detached clear/rebind regressions pass.
Same-state `_SetState` Blur/Focus is tested as an admitted restart. An
animation-only class-host fixture exposed Idle being rejected as combat; only
its exact authored Idle is now admitted by the retained fallback. Completion
returns World facts to state3 while preserving receiver traits and vital/gear
sheets. Latest focused EXE SHA256:
`C84EB3288A172467DAC7A448C6AE58D18A838B8FDE16D628582DF1A733BB89A8`.
The independent reviewer inspected the final code and reran that frozen test:
PASS. Production main acceptance remains separate.

Production key4 investigation exposed stale Faery timer admission at Use: the
main clock was updated after Session.update, while the retained `do_spell`
consumer runs within that update after its serial increments. A separate generic
`set_frame_begin_provider` now exposes the actual current Session/dt before
commands/animation. It preserves the AI provider and diagnostic fallback; no
predicted serial or second clock is introduced. Main moves its two existing
clock owners there. This modern frame-phase seam preserves the source Use
consumer's requirement for current cooldown facts; it does not claim the exact
original global engine traversal order. The focused test verifies240 frame ticks,
independent AI decisions, current timer serial at the real skill marker, exact
elapsed, zero-dt/no elapsed, invalid-dt/no tick and explicit clearing. Latest
expanded runner EXE SHA256:
`830D0F6BF5E4D6AAF483D7399A6FD26F4885EAD9F06B2CC507A88ADA5B41FDDC`.
Root writer terminal; production key4 rerun remains lead-owned.
