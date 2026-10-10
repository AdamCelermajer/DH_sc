# P15 FAERYSOUND report (B050: Faery key-4 cast sound)

Branch `p15/faerysound`, worktree `DH_wt/faerysound`, build `DH_wt/build-faerysound`.
Code commit: `973efff0`. `p14_build.ps1 -Name faerysound -Test`: build exit 0; ctest 111/112 passed. The only failure is `session_skill_binding` (known .local-inputs junction issue, pre-existing).

## Answer to the user's question
The original plays the Faery cast sound on EVERY cast, including with no enemy in range. It is not tied to a hit. Our build played nothing for key-4 casts: no code path requested a Faery sound. Fixed below.

## (a) Original: what plays and when (evidence)
Source: `.local-inputs/character-skill-session-v2/cache/data/scripts/skills/faerie_celest.luac` and `faerie_hotty.luac`. Both are plain-text Lua source despite the `.luac` extension (the inspector reported no Lua bytecode header).

- Celest `OnPreSkill_` (after the UseMana/SetSpellCooldown prefix): `PlayFX(Celest_Level_1_Player)` unconditionally; then the tier label `sfx_spell_lightning_small` if target_count is 1..2, `sfx_spell_lightning_medium` if target_count < 6 (this includes 0), `sfx_spell_lightning_big` otherwise. If the list is empty, also `PlaySound3D("StaticBallKilled")`. So with NO target: `sfx_spell_lightning_medium` plus `StaticBallKilled`.
- Celest `OnSkill_` (do_spell use step): per target `SpellCombatRoll` and `PlayFX(Celest_Level_1_Main)` (when damage > 0 for characters). The PlaySound block at the end of OnSkill_ is COMMENTED OUT in the source. No Faery hit sound exists in the script.
- Hotty `OnPreSkill_`: with targets, `sfx_spell_fire_small/medium/big` by count and `PlayFX(Hotty_Level_1_Player_Pre)`; with NO target, only `PlaySound3D("sfx_mage_staff_elemental_fire")`.
- Hotty `OnSkill_`: `PlayFX(Hotty_Level_1_Target)` per target, no sound.
- AnimatedEffectTable rows have no sound field (`publication/.../effects-tables/original-reader-projection.json` schema: File, ForceCancel, Loop, ...). The FX are silent.
- Timing: `features/faery_menu/celest_source_evidence_v1.md` (prior worker) says focus event 32 invokes OnPreSkill and do_spell event 40 invokes OnSkill_. Not re-verified this session. Our Pre sound is requested at the same point as the Pre FX (after the UseMana/cooldown commit in `begin_skill_cast_v1`).
- Conclusion: the cast sound is in OnPreSkill_ and plays for every cast. No Faery-specific impact sound exists in the original scripts, so none was added. The target's own injured reactions come from the existing combat audio and were not changed.

## (b) Our build before the fix (quiet batches, `run1/`)
Key 4 pressed at frame 70 (`--skill-key-frame 70:4`). "Near" = player at (-6800,-250,259), next to `_prim_LizTemplate_03`. "Far" = default spawn, no enemy in range.
- Knight/Mage/Rogue Celest, far and near: `Source Faery key=4 frame=70 slot=0 ... generation=1 phase=1 MP=17.25 / 68.5 / 30.25`. The cast was admitted and mana was debited, with no Faery cue. In the near runs the lizard took 14 to 19 damage from the cast. `Audio final dispatched=2` came from lizard combat cues (`Audio source frame=14 ... event=attack_mainhand`), not from the Faery cast. The cast's `Audio step source update ... sequence=348 step=0 sound=-1` is an animation step with no sound.
- Rogue save set to current slot 4 (Hotty): `Source Faery key=4 frame=70 slot=4 sequence=-1 ... diagnostic=no spell implemented for Faery slot 4 (key 4 does nothing)`. Slot 1: `diagnostic=Current source Faery script is unsupported`.
- `view-celest.save` (run14): slot 0 `unsupported`. Its saved FaeryList is not the class list, so this is a test artifact, not a fresh class save.

## Second finding, fixed here: the Hotty slot is 4, not 1
- Class FaeryList rows (list `[1,13,14,15,7]`): row 7 = `Hotty` / `faerie_hotty`, with type field `words[8]` = 4. Row 1 = `Celest` (type 0). Slot 1 is row 13, not Hotty. The Hotty connected test already uses `current_faery = 4`.
- The gate `faery_slot_has_spell_v1` allowed slots 0 and 1 (inferred from SWF frame order, survey G7), so Hotty could never cast. It now allows slots 0 and 4. Slots 1 to 3 give "no spell implemented" through the gate.
- Open: the page names for slots 1 to 3 (Primula, Rocky, Wetty, Windy) are not verified against the table.

## Changes (files)
- NEW `port/windows-foundation/features/faery_menu/faery_cast_sound_v1.{hpp,cpp}`: `faery_pre_sound_labels_v1(hotty, target_count)` returns the original labels in play order.
- NEW `features/faery_menu/tests/faery_cast_sound_v1_tests.cpp` (ctest `faery_cast_sound_v1`).
- `features/generic_skills/runtime_skill_cast_coordinator_v1.hpp`: `RuntimeSkillFaeryPreSoundV1`, `RuntimeSkillFaeryPreSoundSinkV1`, `set_faery_pre_sound_sink(...)`, member `faery_pre_sound_sink_`, includes.
- `features/generic_skills/runtime_skill_cast_coordinator_v1.cpp`: anonymous `request_faery_pre_sound(...)` after `fail()`. Called after the Celest UseMana commit (before `faery_menu::CelestEffectReceiptV1 pre_fx;`) and after the Hotty UseMana commit (before `if (active.hotty_effect_dispatch) {`). Target count = prepared `character_targets.size()`.
- `features/audio/runtime_session_audio_v1.{hpp,cpp}`: `queue_faery_pre_sounds(...)` and `flush_faery_pre_sounds(const RetainedFrameAudioClock*, error)`. Labels resolve through `host_->source_ordinal` and submit through `submit_source_sound` on the frame's device clock. Log: `Faery cast sound uid=<ordinal> label=<label> targets=<n> frame=<f> status=dispatched|failed|dropped detail=...`.
- `main.cpp` (3 hunks): (1) key-4 block: `skillCastCoordinator->set_faery_pre_sound_sink(...)` before the Faery `begin_skill_cast_v1` (anchor `request.active_faery_spell=&arm;`); (2) after `before_update(...)`: `runtimeAudio->flush_faery_pre_sounds(audioClock, ...)` (anchor `if(settingsKnown)audioClock=runtimeAudio->before_update(`).
- `features/faery_menu/character_state_faery_v1.hpp`: gate slot 0 or 4, comment corrected.
- `features/faery_menu/tests/character_state_page_tests.cpp`: gate expectations (0 and 4 have spells; 1, 2, 3 do not).
- `CMakeLists.txt`: `faery_cast_sound_v1.cpp` in `foundation_runtime_skill_cast`; test target `faery_cast_sound_v1_tests`.

## Tests run (real output)
- `ctest --test-dir build-faerysound -j 6`: `99% tests passed, 1 tests failed out of 112`; `29 - session_skill_binding (Failed)` (junction path, pre-existing). `78 character_state_page Passed`, `79 faery_cast_sound_v1 Passed`.
- `faery_cast_sound_v1_tests` covers: Celest 0 gives {medium, StaticBallKilled}; Celest 1 and 2 give small; 3 and 5 give medium; 6 gives big; Hotty 0 gives {mage staff fire}; Hotty 1 gives fire small; 4 gives fire medium; 6 gives fire big; every count gives a non-empty label list.
- Not unit-tested: the sink call inside the connected Celest/Hotty session tests (they need the connected runner). Covered by the EXE runs below.

## Evidence after the fix (EXE `build-faerysound`, quiet batch `run1/jobs3.json`, parallel 8)
Each log has `Source Faery key=4 frame=70`, then the cue lines at frame 71:
- Knight/Mage/Rogue Celest FAR (no enemy, targets=0): `Faery cast sound uid=585 label=sfx_spell_lightning_medium targets=0 frame=71 status=dispatched`, then `uid=209 label=StaticBallKilled targets=0 frame=71 status=failed detail=Unavailable original audio asset: data/sounds/sfx_static_ball_killed.wav`.
- Knight/Mage/Rogue Celest NEAR (targets=3): `uid=585 label=sfx_spell_lightning_medium targets=3 frame=71 status=dispatched`.
- Rogue Hotty FAR (slot 4, targets=0): `Source Faery key=4 frame=70 slot=4 sequence=351 ... MP=30.25`, then `Faery cast sound uid=479 label=sfx_mage_staff_elemental_fire targets=0 frame=71 status=dispatched`.
- Rogue Hotty NEAR (targets=3): `uid=582 label=sfx_spell_fire_medium targets=3 frame=71 status=dispatched`.
- The `uid` in the log is the runtime binding id, NOT the sounds.xml uid (XML uids: lightning medium 89, StaticBallKilled 233, fire medium 96, mage staff fire 29).
- Logs: `.local-inputs/claude-preview15/faerysound/run1/out/v2-*.log`. No screenshots (silent runs).

## Package files required
Present in `windows-source-clock-v19-preview-14-rc1/audio-assets` (sounds.xml, sounds_pyarray.bin, data/sounds):
- `sfx_faerie_lightning_small.wav` (label sfx_spell_lightning_small, XML uid 88), `sfx_faerie_lightning_medium.wav` (uid 89), `sfx_faerie_lightning_big.wav` (uid 90)
- `sfx_faerie_fire_small.wav` (sfx_spell_fire_small, uid 95), `sfx_faerie_fire_medium.wav` (uid 96), `sfx_faerie_fire_big.wav` (uid 97)
- `sfx_mage_staff_elemental_thunder_22.wav` for label `sfx_mage_staff_elemental_fire` (uid 29). sounds.xml maps this label to the thunder_22 file, and a file `sfx_mage_staff_elemental_fire.wav` also exists in the folder. The XML was followed. The user should confirm which one the original plays.
- MISSING: `sfx_static_ball_killed.wav` for label `StaticBallKilled` (XML uid 233, bank 5). No file with that name exists under `.local-inputs`. The whole bank-5 StaticBall group (uids 231 to 234: sfx_static_ball_spawn/hurt/attack/killed) is absent. The cue stays wired, the miss is logged (`status=failed`), and nothing substitutes. The original sample is needed.
- Side finding (not this bug): `data/sounds/sfx_lizardman_attack_1.wav` (uid 474) is also missing. Lizard attack cues log `Unavailable original audio asset`.

## Verifier script
Build the EXE, then from `port/windows-foundation/tools`:
`quiet_run.ps1 -JobsFile C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/claude-preview15/faerysound/run1/jobs3.json -Parallel 8 -Summary <dir>/summary.json`
The jobs point to `DH_wt/build-faerysound/dh-foundation.exe`. Repoint the EXE path to the packaged one. Expected:
- `*-celest-far` logs: `Faery cast sound ... label=sfx_spell_lightning_medium targets=0 frame=71 status=dispatched`, then the StaticBallKilled `status=failed` line.
- `*-celest-near` logs: `label=sfx_spell_lightning_medium targets=3 ... status=dispatched`.
- `v2-rogue-hotty-far`: `label=sfx_mage_staff_elemental_fire targets=0 ... status=dispatched`. `v2-rogue-hotty-near`: `label=sfx_spell_fire_medium targets=3 ... status=dispatched`.
- Every cast log has an empty `diagnostic=` on the Source Faery line, and MP drops by 10.

## Open risks / not verified
1. Audibility is not verified. Quiet runs use DH_AUDIO_SILENT (zeros), so `dispatched` means the Vox submit succeeded, not that a person heard it. A non-silent listen test is needed.
2. Timing: the cue is at frame 71, one frame after the key-4 log at frame 70 (the flush runs after `before_update` in the next loop pass). The original focus-event 32 timing comes from the prior note and is not re-verified against the animation data.
3. The target count is character-only (`character_targets`). The original Lua also counts GameObject targets (AttackableOnly objects), so breakable objects in range are not counted and the tier can be low. Limitation noted in the code.
4. The Hotty slot change (1 to 4) is data-backed (row type field, connected test). The page names for slots 1 to 3 are unverified; the root should check the Faery page against the SWF.
5. StaticBallKilled file missing (see Package files).
6. The sound is requested only when the Pre prefix commits (mana and cooldown). A rejected cast (no mana, cooldown, invalid slot) plays nothing, which matches the original OnSkillCheck gate.
