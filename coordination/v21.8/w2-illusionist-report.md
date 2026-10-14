# v21.8 subclass wave 2b: Illusionist (w2-illusionist)

Branch `v21.8-w2-illusionist`, worktree `.claude/worktrees/w2-illusionist`, subst `I:`, build `C:/dh218/w2-illusionist-build`
(Release, llvm-mingw). Real-run cases and logs: `C:/dh218/w2-ill/cases/<case>/` (outside the repo; `run.log`, `f<N>.ppm`, and PNG
conversions made with `C:/dh218/w2-ill/ppm2png.js`).
Evidence labels: UT = unit test, RR = real hidden-window run (log lines and frames), IDA = original decompilation, Lua = original script.

Commits on this branch: `6bcaf8df` (merge of windows-foundation, conflicts resolved, CTest 213/213), `e1b9082a` (monster buff FX
anchor, receiver SnS_Level for enemy buffs, Terror conversion).

## 0. Blocker "Source actor property row unavailable: MagePlayerBase_Illusionist" (FIXED, RR)

Root cause is the run input, not the lookup (unchanged from wave 2a): the harness package carried pre-subclass bindings. Fixed by the
regenerated bindings in `windows-source-clock-v21.8-staged3` (see the lead's patch). Every case below uses staged3. The
save-menu preview fix for subclass saves is in windows-foundation and was not needed by these runs (they start from saves written by
`subclass_save_writer_v21_8.exe`).

## 1. Per-skill results (Illusionist subclass, level-30 saves, key 2 at frame 150, target `Swamp_LizadMan_Type1`)

Sounds: nobody has listened to them; only the resolved wav name is recorded. Original footage for Illusionist skills is not in the
repo (AGENTS.md visual rule): visuals are not compared with the original; they are the port's own frames.

| Skill (id) | Cast | Animation | FX | Sound (resolved file) | Effect / damage | Verdict | Evidence |
|---|---|---|---|---|---|---|---|
| Figments (45) | WORKED: `Source skill key=2 slot=0 frame=150 ... skill=Figments MP=61` | clip present; not visually compared | Use-time FX-only buff; grab accepted (no identity error). Persistent FX not isolated in a frame | WORKED: `Plain sound submitted name=sfx_skill_mage_ilu_nightmare ordinal=531` at frame 198 (`Source skill sound frame=198`) | Stun WORKED: `[status] actor 6003394295803936768: Idle -> Stunned (SetStunState immediate)` at frame 198, `enemy decisions blocked ticks=60`. Buff applied 4250 ms (`SnS_Dot_Duration`, prop 181). DoT tail NOT implemented | PARTIAL (DoT not implemented; buff FX not visually isolated) | RR `cases/figments31/run.log` 1072, 1127-1132, 1195 |
| Eclipse (30) | WORKED: key 2 at 150, `Source skill key=2 ... skill=Eclipse MP=93`; Use at 184 | clip present; not visually compared | WORKED for the presentation burst (`Source skill presentation FX frame=184 name=spell_dh2_prince_mage_illusionist_eclipse kind=target`), frames f184/f200 show it on the target. Persistent buff FX grab accepted, not visually isolated | none (the Lua has no PlaySound/PlayFX sound) | Speed cells 46/47/48: 0 -> -6400 raw (-25 %, flat). **Cell 50 (Rating_Attack) and 190 unchanged** (was 16128 -> 1698054144 before this session; see §3.1) | PARTIAL (persistent FX not isolated) | RR `cases/eclipse31/run.log` 1072, 1127-1130; `f200.png`, `final.png` |
| TimeControl (111) | WORKED: key 2 at 150; Use at 171 | clip present; not visually compared | buff FX grab accepted; a pink/violet FX is visible at f200 between the player and the lizard (`f200.png`), not isolated to the lizard | WORKED: `Plain sound submitted name=sfx_skill_mage_ilu_hold_time_hit ordinal=530` at frame 171 | Speed cells 46/47/48: 0 -> -7680 raw (-30 %, flat), 5000 ms (the Lua formula 4000+1000L at L=1; the row has no duration cell). Lizard applied; boss/miniboss skip rule not run (no boss target) | PARTIAL (buff FX not isolated; boss gate not run; walk slow not measured on the mover) | RR `cases/timecontrol31/run.log` 1072, 1124-1128 |
| Terror (110) | WORKED: key 2 at 150, `Source skill key=2 ... skill=Terror MP=106` | clip present; not visually compared | buff FX grab accepted (no detail) | not in the Lua as a sound (no PlaySound) | Use at 203: `Timed buff applied frame=203 actor=4141719441445850732 class=FactionBuff source=Terror strength=1 durationMs=5000 cells=0:7->8` (monster AIFaction 7 -> 8, duration = SnS_Fear_Duration 4250+750L). One conversion per Use. Monster afterwards: no logged attack on another monster | PARTIAL: cast, conversion and faction write WORKED; the "enemies turn against each other" behaviour is NOT observed (faction 8 relations unread, Q4) | RR `cases/terror31/run.log` 1107-1108, 1189-1191; `f200.png`, `f260.png`, `final.png` |
| Disciples (27) passive | listener reached: `Passive hit observed frame=114/144` | n/a | application: roll-dependent (chance 3.5 % at rank 3), not applied in these runs | n/a | Listener + chance + faction flip logic: UT (death branch) | PARTIAL (roll missed in RR; application not observed) | RR `cases/passives31/run.log`; UT `event_passive_owner_v1_tests` E |
| Petrify (82) passive | WORKED: `Timed buff applied frame=198 ... class=Skill_Illusionist_Petrify source=petrify strength=3 durationMs=0 cells=79:2610->818,...` (re-run after the merge) | n/a | n/a | n/a | Damage_Min_Main_Hand 10.2 -> 3.2 on the struck lizard (permanent debuff, one stack, §3.4) | WORKED (one stack) | RR `cases/passives31/run.log` 1075-1132 |
| Mesmerize (73) passive | WORKED (self class cells) | n/a | n/a | n/a | Rating_Attack +15 at rank 3 (= +5*3); group buff NOT modelled (single-player session) | WORKED (self) | RR `cases/passives-stat/run.log` 974 (wave 2b, before the merge; not re-run) |
| Mirror (74) passive | self armour WORKED (Physical_Armor 1024 -> 8704, +30) | n/a | n/a | n/a | Stun on a melee-range attacker: UT only; no monster attacked the player in a run | PARTIAL (stun real run pending) | RR `cases/passives-stat/run.log` 975; UT section E |

Notes on the verdicts:
- "PARTIAL" in every buff row means the persistent FX was accepted by the engine (no identity error) but was not isolated in a frame. The stills show the presentation FX (Eclipse burst) and the cast pose only.
- Monster buff FX identity error (`detail=fx=Buff actor has no opaque FX manager identity`) is gone in all four runs (0 occurrences in `eclipse31`, `timecontrol31`, `figments31`, `terror31`).

## 2. Engine changes (general, not per skill)

Earlier in wave 2b (commit `69b00232`, unchanged):
- `RuntimeSkillEnemyBuffProfileV1` and `RuntimeSkillEnemyStunProfileV1` (appended row fields `enemy_buff`, `enemy_stun`); per-target loop in the cast coordinator; `RuntimeSkillCastRequestV1::stun_target`; `event_passive_owner_v1` gains Disciples, Petrify, Mirror.

This session:
1. **Merge `windows-foundation` into the branch** (`6bcaf8df`). The four conflicted files were resolved keeping both sides: `EventPassiveKindV1` has disciples/petrify/mirror AND killing_streak/survival; `on_hit`, the script table and the listeners carry both; `RuntimeSkillUseProfileV1` tail order is volleys, volley_count, self_hp_cost, status_arm, enemy_buff, enemy_stun. The magesub rows set `self_hp_cost`/`status_arm` to nullptr before the enemy buff pointers. (The brief listed two more tail fields, `pre_range_from_temp_prop1` and `knockback_great_on_character_hit`; neither exists in either side of the merge, so they were not added.)
2. **Combat-spawned monsters get an FX anchor** (`main.cpp`, buff `fx.play`): a monster spawned by the CombatSession (not a level Character definition) is registered with `SourceFxAnchorRegistryV1::register_actor` on its first buff FX. Before this the level-only registration left them without an opaque manager identity (`features/effects/source_fx_anchor_registry_v1.cpp`, `main.cpp:3613` registers only level Characters).
3. **Enemy-target buffs read the receiver's own SnS_Level** (`TimedBuffSpecV1::sns_level_from_receiver`, set by the enemy loop). The port previously wrote the caster rank into the receiver's SnS_Level. IDA: `CharProperties::_LinearWithPropPlusBaseValue` (0x3e2d4c) computes `dest = base + P3 * (RecalcProperty(receiver, P2) >> 8)` on the a4 = 0 path. The caster's CreateBuff level is only stored at BuffInst+900 for capacity ordering (`CharProperties::PROPS_AddBuff` 0x3e232c, `Character::_CreateBuff` 0x3b86a8 passes it as `a5`); nothing writes it to SnS_Level. Self-buffs keep the strength write (the Lua `SetProp(SnS_Level)` on the caster).
4. **Terror conversion** (`RuntimeSkillEnemyBuffProfileV1::conversion_slots`, coordinator `conversion_slots_`): one target per Use (first popped Character that holds no live slot), at most 3 slots, FIFO (the oldest slot is freed when full), each slot released at its expiry (Lua `StartSkillTimerCB(BUFF_DURATION, delete_slot)`). Duration `SnS_Fear_Duration` (property 187, verified with the compiled member list `android-native/.../source_process_compiled_members_v121.hpp`). Row `Terror` in `runtime_skill_use_profile_magesub_v1.hpp`, admission test updated.

## 3. Open issues

### 3.1 Eclipse Rating_Attack anomaly (D6): resolved by the IDA reading (inference confirmed by the run)
The class row `Skill_Illusionist_Eclipse` has type-1 entries `dest 50 / 190, P1 = 0, P2 = SnS_Level (172), P3 = 1698038016`. Before this session the port multiplied that term by the caster's rank, which gave 16128 -> 1698054144 on the monster. Per IDA §2.3 the term multiplies the RECEIVER's own SnS_Level (0 for the lizard), so the term is zero. RR `eclipse31`: cells 50/190 are no longer listed as changed, and the speeds read exactly -6400 (flat -25 %), which is only possible when the lizard's SnS_Level is 0 (the same `>> 8` term would otherwise change the value). Remaining caveat: if a receiver has SnS_Level > 0 (for example a monster that cast its own skill), the 1698038016 term applies with that receiver's level. That case is not observed. The raw constant itself still looks like a data artifact (a 32-bit float pattern is not a plausible rating); not changed, since the native would evaluate it the same way.

### 3.2 Persistent buff FX not visually isolated
The buff FX grab is accepted for monsters now (no identity error). A frame that proves the persistent loop on the lizard was not produced; the Eclipse and Terror frames show the presentation burst and the fight, not the loop. Needs a no-buff vs buff frame pair on the same position.

### 3.3 Terror: the faction effect on other monsters
The conversion and the FactionBuff write (cell 0, 7 -> 8) are observed. Whether converted monsters now attack other monsters or only the player depends on the faction-8 relations (Q4, `ai_factions` table row 8, not yet read) and on monster-vs-monster targeting, which the port does not log. Not observed in the runs; the monster converted in `terror31` (`_prim_LizTemplate_03`, fixture log line 642) shows no further AI log after frame 203.

### 3.4 Petrify stacks
Unchanged from wave 2b: the row writes Damage_Min_Main_Hand absolutely, so after the first stack the target is below the Lua threshold and no second stack is added. Capacity 5 is not reached through this path.

### 3.5 Walk slow after cast on the mover
The speed cells change for the lizard (RR), but the locomotion effect was not measured on movement.

## 4. Not done / gaps

- **Terror mana**: the Lua charges `UseMana` once per conversion inside `add_slot`. The port charges the generic OnPre prefix on every cast, so a cast with no eligible target costs mana here and nothing in the Lua. (The coordinator has no Use-time mana commit; adding one is a hot-file change, deferred.)
- **Terror SkillCombatRoll**: the Lua rolls the converted target once. Not modelled: the Terror class has no damage rows (MinDamage/MaxDamage = 0), so the roll has no effect in the data.
- **Terror LookAt**: presentation only, not modelled.
- **Terror slot eviction** (D13): the Lua frees the evicted slot with `RemoveBuff(BUFF_ID, handle)` on the caster; the port follows the INFERENCE that this does not remove the monster's buff (the monster keeps FactionBuff until its own duration ends). Not confirmed by a native trace.
- **DoT tail (Figments)**: native `F_ApplyResult` tail `PROPS_AddDot` is not implemented (ES-BUFF item 5).
- **Mirror stun**: unit-tested only (listener, melee-range gate, hook call, duration). No monster attacked the player in a run.
- **Disciples application**: not observed in the real run (3.5 % roll missed); branch logic is unit-tested.
- **Mesmerize group buff**: not modelled (single-player session has no other Player).
- **Boss / miniboss exclusions** (TimeControl, Figments stun, Disciples, Petrify threshold): covered by UT; no real boss target was run.
- **Figments stun expiry** (1500..4000 ms): not measured in time; only the start.
- **Sounds**: resolved to existing staged files; not listened to.

## 5. Real-run recipe (reproduction)

Generator: `C:/dh218/w2-ill/make-case.ps1` (template `C:/dh218/rr/fig/run.args`; extra lines through `-Extra 'a|b'`, which must include `--campaign-skip-frame|20,40,60,80,100`, otherwise the skill does not cast). Saves: `subclass_save_writer_v21_8.exe <worktree> C:/dh218/seeds/mage/profile.save <out> MagePlayerBase_Illusionist <Skill> 30`. Cast key 2 at frame 150 onto `Swamp_LizadMan_Type1@990,-212,258@30`. Terror second target: `--spawn-test Swamp_LizadMan_Type1@1010,-230,258@30`. Frames: `node C:/dh218/w2-ill/ppm2png.js f200.ppm f200.png`.

Cases this session: `eclipse31`, `timecontrol31`, `figments31`, `terror31`, `passives31` (all with the post-merge build, `C:/dh218/w2-illusionist-build/dh-foundation.exe`).

## 6. Test counts and commit

- Full CTest at the merge commit `6bcaf8df` (after conflict resolution, before the wave-2b follow-up changes): **213/213 passed** (`C:/dh218/w2-ill/ctest-merge.log`, 729 s).
- After `e1b9082a` (monster FX anchor, receiver SnS, Terror): targeted tests 8/8, then **full CTest 213/213 passed** (`C:/dh218/w2-ill/ctest-merge.log`, 722 s).

## 7. Summary line for the lead

Merge done (6bcaf8df) and wave-2b follow-up (e1b9082a); full CTest 213/213 on the post-merge tree. Monster buff FX anchor fixed (no identity errors in four runs). Eclipse D6 resolved by IDA: the class term multiplies the receiver's SnS_Level, which is 0 for the lizard, so Rating_Attack is unchanged and the speed is flat -25 %. Terror implemented as a 3-slot FactionBuff conversion (faction 7 -> 8 observed, 5000 ms); monster-vs-monster behaviour not observed (Q4 open). Per-skill verdicts: Figments PARTIAL (stun + sound WORKED, DoT missing), Eclipse PARTIAL (presentation WORKED, persistent FX not isolated), TimeControl PARTIAL (speed/sound WORKED, FX not isolated), Terror PARTIAL, Petrify WORKED (one stack), Mesmerize WORKED (self), Mirror PARTIAL, Disciples PARTIAL.
