# v21.8 subclass wave 2b: Illusionist (w2-illusionist)

Branch `v21.8-w2-illusionist`, worktree `.claude/worktrees/w2-illusionist`, subst `I:`, build `C:/dh218/w2-illusionist-build`
(Release, llvm-mingw). Base `windows-foundation` @ 99d4d1c5. Real-run cases and logs: `C:/dh218/w2-ill/cases/<case>/` (outside the repo).
Evidence labels: UT = unit test, RR = real hidden-window run (log lines and frames), IDA/Lua = original source.

## 0. Blocker: "Source actor property row unavailable: MagePlayerBase_Illusionist" (FIXED, verified RR)

Root cause is the run input, not the lookup code:
- `combat_session.cpp:1330` throws only when the bindings lack the actor or `propertyRow` is outside `database.characters.names`.
- `MagePlayerBase_Illusionist` is at name index 291 of the 448-entry `character_properties_pyarraynames.bin` (checked by parsing the table), so the row is valid.
- The run's `--combat-bindings original-melee-bindings.xml` is resolved inside `--assets`. The old packages carried the pre-subclass bindings (25 actors, 186,212 bytes, sha fcea6355...): `C:/dh218/overlay-mage-assets` and `windows-source-clock-v21.8-staged2` before the lead's patch. Those lack all six subclass actors.
- The lead patched `windows-source-clock-v21.8-staged2` (31 actors, 228,404 bytes, the regenerated export) and created `windows-source-clock-v21.8-staged3` (patched bindings, plus 75 Act-1 files). Both verified by size and actor count.

RR evidence: baseline Figments case with the unmodified build and `staged2`: `base-fig30b/run.log` shows `SPAWN ok template=Swamp_LizadMan_Type1` and no `Foundation error`. All later cases use `staged3`. Earlier `base-fig30` (player relocation via `--player-to`, no intro skip) also passed combat init; its cast did not happen because the player was moved away from the lizard. Cases `base-fig30b`, `figments30new`, `eclipse30`, `timecontrol30`, `passives-stat` run with the flags listed in §5.

Diagnostic note: the error message conflates "not in bindings" with "row out of range". Not changed here (hot file); recorded for the lead.

## 1. Per-skill results

| Skill | Cast | Animation | FX | Sound (resolved file) | Effect / damage | Verdict | Evidence |
|---|---|---|---|---|---|---|---|
| Figments (45) | WORKED: `Source skill key=2 slot=0 frame=150 phase=1 skill=Figments MP=61`; Use applied at 198 (`hits=1`) | clip present; not visually verified | buff FX NOT created (see §3.1) | WORKED: `Plain sound submitted name=sfx_skill_mage_ilu_nightmare ordinal=531`, `Source skill sound frame=198` | Stun WORKED: `[status] actor ...: Idle -> Stunned (SetStunState immediate)`, `Enemy arms ... stunned=1`. FX-only buff applied 4250 ms (4000+250*L, SnS_Dot_Duration = prop 181). DoT NOT implemented. | PARTIAL (no DoT, no buff FX) | RR `figments30new/run.log` lines 1072-1147 |
| Eclipse (30) | WORKED: key 2 accepted at 150; Use at 184 | clip present; not visually verified | WORKED: `Source skill presentation FX frame=184 name=spell_dh2_prince_mage_illusionist_eclipse kind=target actor=<lizard>`; frame f200 viewed (burst on the target) | none (Eclipse has no PlayFX/PlaySound sound in the Lua) | Speed cells 46/47/48: 0 -> -6656 raw (-26 %, walk/rotation/attack) on the lizard, 4000 ms. Buff applied, rejected 0, skipped 0. **Side effect: cell 50 (Rating_Attack) 16128 -> 1698054144** (see §3.2). | PARTIAL (blocked on §3.2) | RR `eclipse30/run.log` line 1127-1129; `eclipse30/f200.png` |
| TimeControl (111) | WORKED: key 2 accepted at 150; Use at 171 | clip present; not visually verified | buff FX NOT created (see §3.1); f170 viewed, no bubble visible | WORKED: `Plain sound submitted name=sfx_skill_mage_ilu_hold_time_hit ordinal=530`, `Source skill sound frame=171` | Speed cells 46/47/48: 0 -> -8448 raw (-33 % = -30-3*L at L=1, walk/rotation/attack), 5000 ms (4000+1000*L). Applied to the lizard, boss and miniboss skipped by policy. | PARTIAL (no buff FX; walk-slow effect on movement not measured) | RR `timecontrol30/run.log` 1124-1126; `timecontrol30/f170.png` |
| Terror (110) | not implemented | - | - | - | - | DID NOT WORK (not implemented) | see §4 |
| Disciples (27) passive | RR: rank 3 via `--test-event-passive-rank disciples=3`; the Figments hits reached the listener (`Passive hit observed`), no Disciples application (the roll missed; chance 3.5 % at rank 3) | n/a | n/a | n/a | Listener + chance via TempProp2 + faction 8 via class row | PARTIAL: listener RR-reached, application not observed | RR `passives-event/run.log`; UT `event_passive_owner_v1` section E |
| Mesmerize (73) passive | WORKED (stat table) | n/a | n/a | n/a | Self class cells at rank 3: 50 Rating_Attack 41984 -> 45824 (+15 = +5*3), 79 Damage_Min +1, 80/81/82 damage/rating cells, 158 Spell_Rating +2 | WORKED (self); group buff NOT modelled | RR `passives-stat/run.log` line 974 |
| Mirror (74) passive | self armor WORKED | n/a | n/a | n/a | Physical_Armor (71) 1024 -> 8704 (+30 = +10*3) | self WORKED; stun part UT only (real run pending) | RR `passives-stat/run.log` line 975; UT section E |
| Petrify (82) passive | RR: `--test-event-passive-rank petrify=3`; the Figments hit on the lizard applied `Skill_Illusionist_Petrify` at frame 198 (`strength=3 durationMs=0`, cell 79 `Damage_Min_Main_Hand` 2610 -> 818 raw, i.e. 10.2 -> 3.2) | n/a | n/a | n/a | Threshold gate (rating > 10) passed in play; permanent debuff applied | WORKED (one stack; see §3.4) | RR `passives-event/run.log` line 1131; UT section E |

Notes on verdicts:
- Per the process rule, every sound name resolves to an existing file in the staged `sounds.xml`; nobody has listened to the sounds yet.
- Animation: the Figments/Eclipse/TimeControl clips are present in the package, but the frames were only viewed for FX and the cast pose; they are not compared with original footage (none exists in the repo for the Illusionist).

## 2. Engine changes (general, not per skill)

- `features/generic_skills/runtime_skill_use_profile_v1.hpp`: `RuntimeSkillEnemyBuffProfileV1` (per-target enemy buff: class, FX, duration cell or formula, skip boss/miniboss) and `RuntimeSkillEnemyStunProfileV1` (per-target stun range and skip rule). Two defaulted fields appended to the row struct (`enemy_buff`, `enemy_stun`); existing rows unchanged.
- `runtime_timed_buff_v1.hpp/.cpp`: `TimedBuffSinkV1::apply_timed_buff_to_targets` (7-argument form, the flags read from the live session by `target_buff_flags_v1`: player flag from traits, boss/miniboss from the AI row bits 2/1, same as status_states); `RuntimeTimedBuffOwnerV1` override.
- `runtime_skill_cast_coordinator_v1.hpp/.cpp`: admission rejects a row with `enemy_buff`/`enemy_stun` before any mana commit when the buff owner or stun hook is unbound; Use runs the stun pass (`stun_rng_` drawn 1500..4000, inclusive, fixed seed) and then the enemy buff loop after the rolls; receipt gains `enemy_buff_outcomes` and `enemy_stun_targets`; `RuntimeSkillCastRequestV1::stun_target` (std::function).
- `runtime_skill_use_profile_magesub_v1.hpp`: rows Eclipse and TimeControl added; Figments gains the stun and FX-buff arms (DoT tail stays a documented gap).
- `passive_skill_owner_v1.cpp`: stat table gains Mesmerize and Mirror (plain `CreateBuff(CLASS, 0, false, L)` self pattern).
- `event_passive_owner_v1.hpp/.cpp`: Disciples (attacker, `GetRand(1,100) < TempProp2`, not boss/miniboss, 10 s faction buff), Petrify (attacker, `Damage_Min_Main_Hand > 10` or `> 30` for boss/miniboss, permanent buff capacity 5), Mirror (defender = self, melee-range attacker, `GetRand(0,100) < TempProp1`, stun `TempProp2` ms via a `set_stun_target` hook). Projection of the class row at SnS_Level = rank.
- `main.cpp`: `request.stun_target` and `passiveEvents->set_stun_target` bind to `statusStates->owner().set_stun(target, ms, true, true, boss_flags, true)`; enemy-arm log line `Enemy arms frame=... buffApplied=... stunned=...`; `--test-event-passive-rank` accepts disciples/petrify/mirror.
- Tests: `runtime_skill_use_profile_magesub_v1_tests` (rows, admission count 4), `runtime_skill_activation_session_v1_tests` (fixture stun stub), `event_passive_owner_v1_tests` section E.

Mirror's defender-side delivery: Codex's answer (attacker CharAI first, then defender CharAI, both vtable +180; `coordination/atlas/r2/natives/QUESTIONS.md`) is what the bus models: the listener is registered on the player and receives events where the player is the target.

## 3. Open issues

### 3.1 Buff FX on monsters (affects Eclipse, TimeControl, Figments)
The persistent buff FX (`grab_marker_v28` on the actor's opaque FX manager identity, `main.cpp` buffBindings.fx.play) fails for monsters: `detail=fx=Buff actor has no opaque FX manager identity` (`resolve_actor_identity` finds no identity for the spawned lizard). The per-target presentation PlayFX (Eclipse) uses a world-position play and does work. Not fixed: needs the monster's FX anchor registration. The buff itself, its speed/stun effect and the sound are unaffected.

### 3.2 Eclipse writes Rating_Attack (cell 50) and TempProp1 (190) from the class row (phase-1 D6, UNRESOLVED)
The row `Skill_Illusionist_Eclipse` has type 1 entries with `a = -666` (keep) and `c = 1698038016` for cells 50 and 190 (the ClassTable fields). The port formula `dest = sheet[d] + c * rank` therefore adds about 6.6e6 to the monster's Rating_Attack per rank (observed: 16128 -> 1698054144 raw). The Lua does not use these cells, and I could not decide from the data whether the native applies them differently (the value is not a plausible fixed-point number; as float32 it is about 5e22). The port currently applies the authored row faithfully, which means the lizard's attack rating is wrecked for the Eclipse duration. **This must be decided before Eclipse is called WORKED.** I did not drop the cells: that would be invented behaviour. Owner's decision needed (IDA check of the CharacterProperties type for dest 50/190 is the open question Q2 of phase 1).

### 3.3 Walk slow after cast not measured
The `Timed buff speeds` log line did not print after the cast (it prints only in some states); the cells changed (`Timed buff applied` line), and the locomotion consumer exists in `runtime_enemy_navigation_v1.cpp:144`, but the monster's movement speed was not measured.

### 3.4 Petrify stacks
The Petrify row writes `Damage_Min_Main_Hand` absolutely (`a = -1`, rank term -2 per rank: -7 at rank 3). After the first stack the target is at -7, below the Lua threshold (> 10), so no further stacks are added in the Lua either. The capacity of 5 is never reached through this path; the unit test pins the gate and the value, not a stack count. Note for the lead: the resolver with type&2 set-priority overrides rather than adds.

## 4. Not done / gaps

- **Terror: DID NOT WORK (not implemented).** Requirements found in the Lua: (a) mana is spent per conversion inside `add_slot` (no OnPre cost), which the cast coordinator's OnPre mana prefix does not model; (b) three conversion slots keyed by target name, the oldest evicted when full; the eviction's `RemoveBuff(BUFF_ID, SLOTn_BUFF)` acts on the caster and is a no-op for the monster's FactionBuff (ES-BUFF D-ES-5), so converted monsters can exceed three while their buffs last; (c) slot release is `StartSkillTimerCB(BUFF_DURATION, delete_slot)`, a timer the coordinator does not provide; (d) Q4: the faction-8 relations to the monster factions are still unread (`ai_factions` table), which decides whether converted monsters attack each other or the player. The FactionBuff class and the owner's faction flip exist (ES-BUFF UT), so the remaining work is (a), (c) and (d).
- **DoT tail (Figments): DID NOT WORK (not implemented).** Native `F_ApplyResult` tail `PROPS_AddDot(target, duration, tick, element)` creates `dot_<element>` (capacity 1) and writes the tick to property `element+127`; the per-tick consumer on the combat path is not located (ES-BUFF item 5).
- Mirror stun: unit-tested only (listener, melee-range gate, hook call, duration). No monster attacked the player in the event-passive window, so no real stun was observed.
- Disciples application: not observed in the real run (3.5 % roll missed); its branch logic is unit-tested (death branch) and the faction flip itself is the class row already verified by ES-BUFF UT.
- Mesmerize group buff (other `Player` characters): not modelled; the single-player session has no other Player, so the group loop is a no-op in the observable game.
- Boss / miniboss exclusions (TimeControl, Figments stun, Disciples, Petrify threshold): covered by the owner's UT (ES-BUFF) and by the flags function; no real boss target was run.
- Figments stun expiry and the 1500..4000 range were not measured in time (only the stun start).
- Sounds: resolved to existing files; not listened to.

## 5. Real-run recipe (for reproduction)

Generator: `C:/dh218/w2-ill/make-case.ps1` (copies the Figments template `C:/dh218/rr/fig/run.args` with the staged package paths, removes `--player-to`, adds `--campaign-skip-frame 20,40,60,80,100`). Saves: `subclass_save_writer_v21_8.exe <worktree> C:/dh218/seeds/mage/profile.save <out> MagePlayerBase_Illusionist <Skill> 30`. Cast key 2 at frame 150 onto `Swamp_LizadMan_Type1@990,-212,258@30`. Logs: `C:/dh218/w2-ill/cases/<case>/run.log`, frames `f<N>.png`.

## 6. Test counts and commit

- Full CTest on the tree before the final `main.cpp` flag edit: **213/213 passed** (`C:/dh218/w2-ill/ctest-full.log`, 504 s).
- After that, one more build for the `--test-event-passive-rank` flag (main.cpp only; no CTest entry covers it). Event-passive real check: `C:/dh218/w2-ill/cases/passives-event/run.log` (Petrify applied; Disciples roll missed; Mirror not triggered).

## 7. Summary line for the lead

Blocker fixed (stale bindings in the harness package, not the lookup; `staged2`/`staged3` patched). Figments: cast, stun, FX-only buff, sound WORKED in play; DoT and buff FX open. Eclipse and TimeControl: cast, per-target enemy buff with the correct speed cells and durations WORKED in play; buff FX blocked on monster FX identity; **Eclipse also writes Rating_Attack (cell 50) by about 6.6e6 raw per rank from its class row (D6), unresolved, blocks calling Eclipse WORKED**. Terror: not implemented (reasons in §4). Passives: Mesmerize and Mirror self-buffs and Petrify WORKED in play; Disciples listener reached; Mirror stun unit-only. CTest 213/213 before the final flag-only change.
