# v21.8 wave 2b: Necromancer subclass report (branch v21.8-w2-necromancer)

Worktree `.claude/worktrees/w2-necromancer` (subst `N:`), base `windows-foundation` @ 99d4d1c5. Build dir `C:/dh218/w2-necromancer-build`.
Package for real runs: `windows-source-clock-v21.8-staged3` (lead's: staged2 + the patched subclass bindings XML + the Act-1 files; the earlier Illusionist/Necromancer "Source actor property row unavailable" failure was the old package XML, confirmed by the lead). Runner: `C:\dh218\necro\run-necro.ps1` (derived from `act1rogue-run-rogue2.ps1`). Cases: `C:\dh218\necro\cases\`. Seeds: `C:\dh218\seeds\necro-*` (made by `subclass_save_writer_v21_8`).

Evidence key: UT = unit test (`runtime_necromancer_skill_owner_v1_tests`, real CombatSession); RR = real hidden-window run (keys are real `2` and Space presses; frames viewed where stated). Labels: PROVEN = read in the original Lua/IDA; INFERENCE = my reading, marked.

## 0. Save and seed (first blocker)

- Necromancer save: `CharacterTable 292` (`MagePlayerBase_Necromancer`) was created with `subclass_save_writer_v21_8` from `seeds/mage/profile.save`. The writer sets the SkillTree rows at rank 0, the chosen skill at rank 1 on hotbar slot 0 (key 2) and the level-gated HP/MP sheet.
- Seeds: `necro-deathclaw` (DeathClaw, L21), `necro-fsr` (FireSkullRain, L15), `necro-pest` (Pestilence, L18), `necro-rise` (Rise, L12), `necro-carrion` (CarrionEater, L18), `necro-leecher`, `necro-wrath` (L15), `necro-putrid` (L21).
- Loads in RR: `Frontend launched ... class=MagePlayerBase_Necromancer`, `Original actor=MagePlayerBase_Necromancer HP=141/141`, `Character name=QA class=MagePlayerBase_Necromancer` (dc-3 run.log lines 7, 9, 548). Bindings resolve (`Source skill bank preloaded class=MagePlayerBase_Necromancer roots=16`).
- The Illusionist-style "Source actor property row unavailable" did not occur with staged3 (the Necromancer ran combat in RR).

## 1. Per-skill results

| Skill (id) | Cast / admission | Animation | FX | Sound (resolved file) | Effect / damage | Verdict |
|---|---|---|---|---|---|---|
| DeathClaw (26) | RR dc-3: key 2 at frame 2978, `Source skill key=2 ... skill=DeathClaw`; MP 128.5 -> 33.5 (95 = 85 + 10 x rank 1, class sheet) | RR dc-4 f3010.png: the claw hand rises under the Bogwomp (the skill's own clip) | Lua FX commented out: none dispatched by the Lua. Visible claw is the animation | none: the Lua has no PlaySound (the ledger cue `sfx_skill_mage_nec_harrowing_grasp.wav` exists, not dispatched, Q5) | RR dc-3: 2 Use hits at frame 3017: requested 41.23 and 37.09 (lizards 47.5 -> 6.27 and 10.41). Explicit knockback: `[push] KnockBack monster target=... source=DeathClaw great=1 direct=0 outcomes=0x80 mask=0x100000` x2 at frame 3018. f3020.png: damage number 39, lizard displaced; the knockback pose is not clearly distinguishable in frames | PARTIAL (knockback pose not confirmed by eye) |
| FireSkullRain (47) | RR fsr-1: key 2 frame 2978, MP 113.5 -> 39.5 (74 = 65 + 9 x 1); Use frame 3004; `loop started (1000 ms, max 3 rolls)` | not separately verified | RR f3040.png: fire around the caster and the first target, damage 8 shown (METEOR FX on the tick) | none (the Lua has no PlaySound; cue `sfx_skill_mage_nec_reaperstorm.wav` exists, not dispatched) | Tick 1 (~frame 3067): rolls on 2 targets (39.56 and a miss); tick 2: 1 roll (34.84); tick 3: empty list, `tick empty: loop stopped` | WORKED (loop, FX, damage). UT: 1000/2000/3000 ms, 3 targets per tick, no 4th roll |
| Pestilence (81) | RR pest-1: key 2 frame 2978, MP 121 -> 37 (84 = 75 + 9 x 1); `infected ... removeAfterMs=6000` (SnS_Dot_Duration 4000 + 1000 x rank, + 1000) | not verified in frames | `play_effect` on the explosion, no diagnostic; FX not captured | `sfx_skill_mage_doppelganger_statue_explode.wav` (the Lua PlaySound; the matrix's `sfx_skill_mage_nec_pestilence_breath.wav` is a second candidate, C8) | Initial roll requested 0 (miss). The victim died at frame 3266; the 500 ms death watch fired: `victim dead=... -> explosion`, `explosion victims=1`, roll requested 0. DoT NOT applied (no native DoT tail in the port, E4) | PARTIAL (cast, infection, death-watch explosion RR; no damage seen in RR; DoT missing) |
| Rise (93) | RR rise-2: key 2 frame 3030, MP 106 -> 51 (55 = 50 + 5 x 1); `TeleportTool launched id=2` | not verified | RR f3040.png: purple glow around the caster (not attributed to the Rise clip) | none (the Lua has no PlaySound; `sfx_skill_mage_nec_raise.wav` exists, not dispatched). Lua RISE_FX (`spell_dh2_prince_necromancer_rise`) is played on the risen zombie, which does not exist | Projectile flight and landing: `landing expired=1 at=-3578.85,139.92,255.0`; Use: `summon DEFERRED (RisenZombie would spawn at ...)` | PARTIAL (projectile + landing real; summon DEFERRED; HasPath probe not implemented) |
| CarrionEater (11, passive) | RR pas-1: DeathClaw kill at frame 3084 (player's skill hit, `died=1`) | n/a | n/a | none | HP 60/201 at frame 3060 -> 70/201 at frame 3090 (+10 = 5 + 5 x rank 1, class sheet TempProp1 (1280+1280L)/256) | WORKED (kill heal, RR). UT: heal clamped to max, own kills only |
| Leecher (60, passive) | stat passive, RR pas-1 frame 60 `Timed buff applied ... class=Skill_Necromancer_Leecher ... cells=133:0->384` (Leech_MP 1.5 = 1 + 0.5 x 1, class sheet) | n/a | n/a | n/a | The leech on hits is not verified (no applied hit on the player in RR; `apply_attacker_leech_v1` is the ES-PASSIVE path) | PARTIAL (buff applied RR; effect UT-only) |
| Wrath (125, passive) | RR pas-1: `Timed buff applied frame=3059 ... class=Skill_Necromancer_Wrath source=Wrath strength=1 ... cells=166..170: 2560->4096` (+1536 raw = +6 = 3 + 3 x 1, class sheet) at HP 60/201 = 30 % | n/a | n/a | `sfx_passive_skill_activate_3.wav` (the Lua PlaySound; exists; the dispatch is called, no diagnostic) | +6 elemental damage bonus via the existing Spell_Damage_Bonus consumer (ES-PASSIVE: `combat.cpp` reads props 166+element) | WORKED (apply RR). Removal at >= 40 % is UT-only |
| Putrid (89, passive) | — | n/a | n/a | n/a | UT: 24 procs in 400 defender-side hits (chance 5 + 2 x rank, percent from TempProp1), buff on the attacker with TempProp2 duration. RR pas-1: NO applied hit on the player occurred (4 observed hits involving the player, all misses or dodges), so the listener never fired in RR | NOT VERIFIED in RR (UT only). DoT ticks not applied (E4) |

Shared Mage passives and actives are not in this report (mage wave).

## 2. Engine changes (all on this branch)

- `features/generic_skills/runtime_skill_use_profile_v1.hpp`: two trailing fields with defaults: `pre_range_from_temp_prop1` (OnPre RANGE = TempProp1, Pestilence) and `knockback_great_on_character_hit` (DeathClaw).
- `features/generic_skills/runtime_skill_use_profile_necromancer_v1.hpp` (new, registered in `runtime_skill_use_profile_registry_v1.hpp`): FireSkullRain, Pestilence, Rise rows (external owner). DeathClaw row stays in `runtime_skill_use_profile_magesub_v1.hpp` with the new flag set.
- `features/generic_skills/runtime_skill_cast_coordinator_v1.{hpp,cpp}`:
  - OnPre search range now comes from `active.pre_range` (profile range or class TempProp1).
  - `RuntimeSkillKnockBackRequestV1` / `RuntimeSkillKnockBackSinkV1` and `RuntimeSkillCastRequestV1::knockback_great`. The Use loop calls it after each Character roll with dmg > 0 (`original_actor_state` is the admission state); sink failures are recorded in `receipt.knockback_diagnostics`, the Use still completes (the Lua does not check the return).
  - Receipt fields `knockback_targets` / `knockback_diagnostics`.
- `features/generic_skills/runtime_necromancer_skill_owner_v1.{hpp,cpp}` (new): external owner for FireSkullRain (1000 ms loop via `RuntimeSkillTimerServiceV1`, rolls with the Use-time formula sheet, 3 ticks), Pestilence (infection list, removal timer SnS_Dot_Duration + 1000, 500 ms `RuntimeSkillDeathWatchV1` poll, explosion 200/360 around the victim, PlayFX + PlaySound), Rise (TeleportTool projectile with a landing callback; summon deferred). `cleanup` for Rise. Projectile contact sensor not requested (`wants_contacts = false`, see section 4).
- `features/generic_skills/runtime_necromancer_passive_owner_v1.{hpp,cpp}` (new): Wrath (3000 ms loop, HP < 40 % buff, PlaySound on apply, unchanged rank does not restart the loop) and Putrid (defender-side OnTargetHit listener on the existing `CombatResultEventBusV1` bus, proc from TempProp1, buff on the attacker). `sync_necromancer_passive_ranks_v1` from the saved rows.
- `main.cpp` (small diff): `sourcePushSink` (the same-Session Push sink, now named), `sourceKnockBackSink` (explicit `SessionPushRequestV1` with outcome 0x80, mask 0x100000, great = true, not direct, pre-hit state), `request.knockback_great`, the necromancer owner's services (same shared projectile/effect/sound services as the mage owner) registered on `skillOwnerRegistry`, the passive owner constructed with the session, synced each frame, bound and advanced (Wrath clock in ms).
- `CMakeLists.txt`: owner and passive sources added to `foundation_runtime_skill_cast`; test `runtime_necromancer_skill_owner_v1`.
- Tests: `features/generic_skills/runtime_necromancer_skill_owner_v1_tests.cpp` (real session, 3 lizards; FireSkullRain timeline, Pestilence infect / death-watch explosion / removal, Rise error path and no-landing no-op, Wrath threshold and re-sync, Putrid defender-side procs and outgoing-hit exclusion).

## 3. CTest

- Full CTest on the branch (before the final one-line Rise change): **214/214 passed** (`C:/dh218/w2-necromancer-build/ctest1.out`, 100 % tests passed, 0 failed out of 214; the number includes the new test).
- After the Rise change (owner file only) the rebuilt test `runtime_necromancer_skill_owner_v1` passed: `necromancer_skill_owner_v1 passed`. The full suite was not re-run after that one change (the machine was heavily loaded; this is a known gap).

## 4. Problems found and fixed during the real runs

- Rise: the first RR (rise-1) aborted in the Pre (`No authored projectile AABB is enrolled for this source URI`) because the projectile's contact sensor registration failed for TeleportTool. The owner's check rejects every contact anyway (Lua `delayed_check` accepts only a non-Character target; the port's contacts are ActorIds, all Characters), so the projectile is launched with `wants_contacts = false`. rise-2 then ran through.
- DeathClaw first runs (dc-1, dc-2) did not reach the cast: the Necromancer's intro cinematic was still playing at frame 2400 and the taps were after the run ended. The schedule of dc-3 (Space taps, 3900 frames) fixed it.
- Unit test: the first FireSkullRain timeline failed because the test's damage killed the lizards at tick 2, so the owner's empty-list stop was correct; the test now uses 1-point rolls.

## 5. Remaining gaps (not claimed as working)

- Rise summon: DEFERRED (no runtime Summon in the port; `spawn_character_v1` admits pool slots only, E7). The DoRise `MoveTo`/`HasPath` four-direction probe (E8) is not implemented: only the front projectile (angle 0) is launched.
- DoT tail (E4): Pestilence's SnS_Dot_Min/Max/Duration and Putrid's DoT are not applied (`combat_session.cpp` applies direct health only). Q16 (`DotTickBorrow` wiring) not verified.
- Pestilence: the Lua matches victims by name; the port matches by ActorId (documented deviation). Objects in the OnPre list are not rolled. The death-watch loop starts with the first infection (the Lua starts it at learn time; phase differs, INFERENCE). Explosion victims are searched around the victim's position with the victim excluded (Q8 open).
- Putrid: `TargetInMeleeRange` is an INFERENCE (attacker target radius). The IDA gate on CreateBuff (state Idle, `Character::_CreateBuff` 0x3b86a8) is not modelled. Codex's answer (QUESTIONS.md Q1: attacker CharAI first, then defender, both only when their flag is set) matches the bus model, but no RR applied defender hit occurred to test it.
- Leecher: the leech effect on hits is not verified in RR; the elemental damage reduction (Q13) is not located.
- Wrath: removal at >= 40 % is UT-only; the activation sound is requested but not heard (nobody has listened to any sound; the resolved wav names are recorded above).
- FX: only the DeathClaw claw (f3010.png) and the FireSkullRain fire (f3040.png) were inspected by eye. Pestilence, Rise, Wrath and Putrid visuals were not captured. The Rise caster glow is not attributed to a named FX.
- Sounds: no dispatch path for the four data-bound cues (Q5). Only Pestilence's PlaySound and Wrath's PlaySound are called by the Lua, and their wav files exist; none has been listened to.
- Observed, not investigated: the observer and Damage log lines print the player's ActorId as `18446744073709551615` for player hits (CarrionEater still fired, so the attacker check matched on the same value). Also `Source death reward diagnostic: Live ActorState vital could not be applied to its source property` appears after skill kills (xp = 0 in those lines); pre-existing, not from this branch as far as I could see.
- Passive observer delivers only applied hits. Misses are not delivered to OnTargetHit listeners (no current necromancer passive needs them).
- DeathClaw: the Lua `plvl` global is undefined (C10, LIKELY no effect at cooldown 1500).

## 6. Branch

- Branch `v21.8-w2-necromancer`, commit: see the git log entry for this report (committed on the branch; not pushed, not merged).
- Subst `N:` and the `.local-inputs` junction are removed at the end of the session (`cmd /c rmdir` for the junction).
