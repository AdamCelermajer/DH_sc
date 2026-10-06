# Actual HurtCorners pulse

`dqhud_droid.swf` SHA a4ffacd1abdf7c9b2ba19c46ebb81c60c100458731a4cdba5880391b9c11b238.
Root HurtCorners is sprite31 (103frames), placement depth3, local identity with y=1twip.
Its sole unnamed depth1 sprite30 (22frames) contains sole unnamed depth1 sprite29 (1frame), containing shape28. Shape bounds are [-2,9606,-8,6397]twips. There are no four corner subclips and no ActionScript action tags in this subtree.

Source HudPlayerValues keeps sprite31 stopped at max(0,HP*100/MaxHP−1), with source signed32 multiplication/division behavior. It is an intensity lookup: full alpha throughframe11, fade throughframe49 (13/256), zero fromframe50. Thus integer percentage51+ is transparent; no additional critical-health bool is needed. Potion recovery updates the same source HP and lookup.

Sprite30 is independently PLAY. Its child alpha at frames0..18 descends 256,251,...166; frames19/20/21 are196/226/256. Actual movie30Hz gives22/30second loop. `AuthoredHurtPulseV7` invokes the actual linked source Sprite::advance body only on30 inside `SwfMovie::action_script` protected scope. It does not animate31, alter colors or traverse unrelated native menu callbacks.

The targeted cadence bridge is a modern integration adapter, preserving the recovered root catch_up=false one-frame/fmod schedule using actual root.m_frame_time. It does not claim the complete original Root::advance listener/GC/FlashVars lifecycle. Existing movie startup still owns source construction/providers. No application tick means no advance and no remainder accumulation; the caller must pass the existing application_dt/tick, never wall-clock catchup after menu pause.

Retain one AuthoredHurtPulseV7 with the HUD graph. After status.update, call update(movie,application_dt,application_tick,diagnostic,error), then the root-owned layout/display. Release it with HUD teardown. Diagnostics expose health frame, pulse frame and both actual alpha multipliers, actual cadence/remainder and whether a source frame advanced. Pulse graph mismatch and stale/absent services fail explicitly.

OriginalUiSession previously called movie.advance(0) only at load and never advanced nested HUD timelines during render. This explains a constant vignette despite a genuine authored pulse asset. Full-screen mapping is separately root-owned; it should preserve this one source shape and alpha.
