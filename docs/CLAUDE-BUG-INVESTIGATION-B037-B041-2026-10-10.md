# Read-only investigation: B037, B004/B029, B038, B039, B040, B041/B005

Date 2026-10-10, from commit `1db50a30`. Written by Claude at the user's request ("investigate and hand over").
**No source, tracker or save was modified. Nothing was built or run. No original video frames were inspected.**
AGENTS.md requires visual evidence before implementing; every item below has logic/source evidence only
and its visual-evidence line is still owed. Addresses are from
`.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`.

Confidence: **High** = original code and port code both read and they disagree. **Medium** = port cause read,
effect not reproduced. **Low** = hypothesis.

---

## B037 — Skills cannot activate while Space is held (High)

**Port cause.** `features/generic_skills/runtime_skill_cast_coordinator_v1.cpp:381`:

```cpp
if (actor->action != CharacterAction::idle && actor->action != CharacterAction::moving)
    return fail(error, "Current source lifecycle action rejects a new skill cast");
```

While Space is held the player is in `attacking` almost continuously, so every skill press is rejected.
`main.cpp` (~2585) reads `uiInput.skills[i].pressed`, a one-frame edge, calls `begin_skill_cast_v1` once and
drops the request. Nothing queues or retries it.

**Original behaviour (this gate is not in the source).**
- `CharAI::AI_BeginSkill` (0x3d86bc) → `AI_IsSkillUsable` (0x3d8358). Its only state tests are:
  reject if `SM_IsUsingSkill` and not `(Character+1312 & 0x8000)`; reject if `SM_IsCasting`; reject if the
  script process is not loaded; then `OnSkillCheck_Usable`. **No attack-state test.**
- Then `CharStateMachine::SM_SetSkillState` (0x3c6670) raises state event **50005** and success is
  `SM_IsUsingSkill()`.
- `CSAttack::OnInit` (0x3c8284) registers `50005 → state 6` with **no guard** (`SM_RegisterEvent(..., 50005, 6, 0, 0)`).
  Idle (0x3c7e60), Move (0x3c80ac), Injured (0x3c8850) and Anim (0x3c8b28) also accept it unguarded.
  `CSSkill` accepts a new skill only through `CSM_StopSkill` = `(Character+1312 >> 15) & 1`. `CSInteract` uses
  `CSM_CanStopInteraction`. So a skill **interrupts an attack** in the original.

**Fix direction.** Replace the idle/moving pre-gate with the source predicate: reject only when using a skill
without flag `0x8000`, or casting; otherwise admit and let the retained attack pose depart through the existing
accepted-departure/OnBlur path (cancel the attack sequence, clear combo state, keep generation guards). Keep the
dead check. Hurt/injured is accepted by the source table too, but decide that separately because the port's hurt
action may carry a pose the source treats differently.

**Also in B037: "attack stops on release".** Releasing Space raises event 50001 (Attack → state 4) guarded by
`Character::CSM_StoppedAttacking` (0x3ad280): `return (current_state == 5) ? *(byte*)(this+1090) : 0`. So release
only leaves Attack when the byte at `Character+1090` is set. Find who writes +1090 (likely the combo/recovery
window) and compare with the port's behaviour. Do not stop mid-swing unless that flag is set.

**Test to write first.** Same-Session: hold attack through a swing, press skill key in the middle of the swing,
expect `begin_skill_cast_v1` accepted, attack retired once, skill state 6 entered, no second attack from the
still-held key until after skill Post. Keep the existing rejection cases (already using a skill, casting, dead).

**Uncertain.** What the original does with Space still held when the skill ends (re-attack immediately, or wait for
a fresh press). B003's regression suggests the port currently resumes; the HUD `Update` at 0x41a780 sends
`Cmd_Attack(null)` while held, so resuming is probably right. Needs the video.

---

## B004 / B029 — Target ring/selection disappears after Space release or skills (Medium-High)

**What the port does.**
- The target HUD (name, level, health) is drawn from `combatSession->selectedactor()`
  (`combat_session.cpp:1790`: the player's `target_id`), `main.cpp` ~3008.
- `target_id` is cleared by: every skill Post (`ClearTarget` is unconditional in BashDown, GroundSlam and Rogue
  JumpKick, conditional in Charge); the combo `attack_anim_clear_nonsticky` operation at combo boundaries
  (`combat_session.cpp:~436`) unless `stickyPlayerTarget` is set, which only Tab (`targetSelect`) sets; and the
  housekeeping at `combat_session.cpp:1441` when the target is no longer eligible.

**What the original does (from `features/combat/auto-target-marker-v1-independent-review.md`).**
`CharacterTargetMarkerV28::update` draws the marker from `last_target` (`Character+0x40c`) and **falls back to the
object of interest** (`Character+0x14a4`) when last-target is null or the player. OOI is refreshed by
`UpdateObjectOfInterest` (0x3ac0b4 / 0x3abb9c) on a 500 ms timer candidate query. So after a skill's
`ClearTarget` (which also does SyncLastTarget → null) the marker stays on the OOI candidate. The port's HUD has no
OOI and no fallback, so the ring vanishes. The helper `features/combat/auto_target_marker_v1.cpp` implements the
precedence but, per its own review, **is not wired to any caller**.

**Fix direction.** Do not change target/skill-clearing rules (they match the source). Add an OOI owner (500 ms
nearest-candidate query per 0x3abb9c) and drive the marker/HUD from `auto_target_marker_v1` (last_target → OOI).
Keep four values separate: combat target, last/preferred target, sticky, rendered marker.

**Test.** After BashDown Post: `target_id == 0`, marker still non-null while an eligible enemy is in OOI range; after
Space release: same; after the OOI candidate dies/leaves range: marker hides within the 500 ms refresh; Tab sticky
survives combo boundary.

**Uncertain.** Whether the name/health frame (not just the ring) follows the marker in the original. Confirm in the
video at several post-skill timestamps. The 377 s incoming-hit clip is unrelated to this.

---

## B038 — XP bar does not fill (High)

**Port cause.** `hud_geometry.hpp:61` says the HUD composer "Omits XP timeline". `compose_original_hud(style, hp,
mp, portrait, out, error)` has no XP input, and `main.cpp:~2981` never supplies one. `hud_art.cpp:19-20` has
`xp_background`/`xp_fill` shapes (chars 148/150) that are never driven. Numeric XP is correct; nothing binds it.

**Original formula** (`InfoHUDManager::FastUpdate`, 0x41e064, same block that sets HP/MP):
```
xp_frame = min(99, 100 * *(int*)(char+4220) / *(int*)(char+4224))   // GotoFrame(bar_xp, xp_frame)
```
HP uses `+4232/+4240` and MP the next pair, which the port already maps to property indices 36/38 and 41/43, so
`+4220/+4224` are property indices **33 (XP) and 34 (XP for this level)**. That matches the port's own level-up code
(`runtime_death_rewards_v1.cpp:165-178` carries `resolved[33] − resolved[34]`). Note HP/MP use `100*cur/max − 1`
(clamped 0..99) but XP does **not** subtract 1 and has no lower clamp. The source report says XP152 has 101 frames;
the original only ever goes to frame 99 here.

**Fix direction.** Add an XP frame parameter to `compose_original_hud`, draw `xp_fill` (the same shrinking-cover
technique the report describes for HP/MP), and feed it from the live sheet:
`combatSession->world()->combat_properties(player_id)->sheets.resolved[33]` and `[34]`. Do not use the stale
`actorProperties` sheet. Reuse the existing 32-bit-wrapping `frame()` lambda semantics.

**Test.** Rendered-pixel test at 0%, ~50%, 99% and just after a level-up (fill resets to the carry); not a numeric
test. Confirm the bar position/orientation against source artwork.

---

## B039 — Audio chops / stutters (Medium)

**Port cause candidate.** `features/audio/winmm_output.cpp`: 4 `WAVEHDR` buffers × 512 stereo frames at 48 kHz =
**10.7 ms each, 42.7 ms total queued**. `update()` runs once per game frame from `after_update` (`main.cpp:~2819`)
and only refills buffers already marked `WHDR_DONE`, with `CALLBACK_NULL`. Any frame longer than ~43 ms (below
~23 fps, or one load/upload hitch) drains the queue and WinMM plays a gap. This build uses CPU skinning and
compat-profile GL, so >43 ms frames are plausible. The main loop also ends each frame with `Sleep(1)`.

**Test before fixing.** Log frame times and an underrun counter in `update()` (all four headers `WHDR_DONE` at
entry). If underruns line up with the chopping, the fix is to decouple: a dedicated audio thread (or larger ring,
e.g. 8–16 buffers of 1024–2048 frames ≈ 170–340 ms) so output no longer depends on frame cadence. Keep the mixer
and cue-submission code single-owner (render on the audio thread, or hand over a lock-free command queue).

**Other causes still to rule out** (from the handoff): duplicate animation-event dispatch (check the cue log for the
same uid within one frame), cue restarts at state transitions, and the missing Lizard WAVs (B028) being re-requested.

---

## B040 — Missing map ambience (High that nothing starts it)

- `sounds.xml` defines `uid 467 SwampHubAmbientMusic` = `m_level_swamp_sfx_swamp.vxn`, `uid 468
  SwampWitchCaveAmbientMusic` = `m_level_swamp_sfx_witch.vxn`, loop = yes, group 2 (music). There is also a water layer.
- The authentic files exist locally: `.local-inputs/audio-v34/cache/m_level_swamp_sfx_{swamp,water,witch}.vxn`,
  and the VXN decoder/mixer exists (`port/engine-audio`).
- `main.cpp` contains **no** music/ambience/PlayMusic call, so gameplay never starts level audio. The packaged audio
  assets (`features/audio/assets/data/sounds`) hold only three WAVs plus `sounds.xml`, so a package could not play
  them even if started.
- Source lifecycle: level pyscripts call `ResumeLevelMusic`; `enterLocation_*` scripts change areas. Check
  `port/engine-audio/audio_level_gameplay_v67` and the `PlayMusic(id,bool13,int8,2000)` (2000 ms fade) path in
  `features/audio/README.md` before adding anything.

**Fix direction.** Stage the real VXN files into the packaged asset set (manifest + hash), start the level music
through the existing source `PlayMusic` owner on level load, stop/transition on focus loss and area change as the
README describes. Do not substitute samples. The `m_world_map.wav` file is the one the truncated cache cannot supply.

---

## B041 / B005 — Missing sword swing / skill trace (Medium-High on the lead)

- The "white/bluish swing" is the **item swoosh FX**, not the ground impact. The engine has
  `CharAnimator::_PlayItemSwooshFX(ItemInstance*, bool)` and `_PlayItemSwooshSFX`, and item fields `SwooshFX`,
  `SwooshEffect`, `SwooshSoundFX`. There is no "Trail"/"Ribbon" class in the engine.
- The tracker row for B005 says production `main` has not registered `RuntimeSwingFxObserverV1`. **That is stale:**
  `main.cpp:1765` constructs it and `1778-1786` registers it as a step-entry observer (`"Source player step FX
  observer bound"`). The pipeline runs in production; the question is what it draws.
- `features/effects/runtime-swing-fx-step-wgl-attempt-report.json` shows the pipeline works up to the framebuffer:
  sequence 470 step 1 → set 253 `swoosh_prince_1hand_combo_01`, 38 vertices, additive blend (1,1), depth write off,
  texture 64×64 uploaded, **but only 7 pixels changed (needs >10) and the run is recorded FAILED**. Its own CPU
  projection finds 7,524 of 7,534 sampled texels are black. Additive blending of black = nothing.
- The packet timeline has phases (initial 0..333 ms). The test appears to sample the **first instant**, where the
  baked UV window sits on black. A UV-scrolling swoosh only becomes visible later in its 333 ms life.

**Next step.** Re-run the WGL smoke capturing several times across 0–333 ms (and with the real in-game camera, not
the bounds-derived one) before changing any renderer code. If it is visible mid-life, the "bug" is the test and
possibly camera/draw ordering in the live path; then compare the live draw order with the packet's phase clock.
Only if it stays black at all phases suspect the texture matrix bake or material colour (0.584 grey × additive).

---

## Suggested order

1. B038 (smallest, certain, self-contained, visible).
2. B037 (certain root cause; fix is in one coordinator function plus a test).
3. B004/B029 (wire the already-written marker helper to an OOI owner).
4. B040 (assets + one call; needs staging decisions).
5. B039 (instrument first, then fix).
6. B041/B005 (needs the multi-phase capture first).

## Tracker corrections to make

- B005 row: the `RuntimeSwingFxObserverV1` registration claim is stale (main.cpp:1765).
- B037 row: add the CSAttack/AI_IsSkillUsable evidence above.
- B038 row: add "HUD composer has no XP input" and the formula above.
