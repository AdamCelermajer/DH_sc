# v21.8 w2-assassin: Assassin skill wiring and real-run verdicts (2026-10-14)

Branch `v21.8-w2-assassin`, worktree `.claude/worktrees/w2-assassin`, subst `S:`, build dir `C:/dh218/w2-assassin-build`.
Package for runs: `.local-inputs/windows-source-clock-v21.8-staged3` (primary checkout). Runner: `coordination/v21.8/act1rogue-run-rogue2.ps1`
(hidden window, real keys, `--fixed-step .016`). Case folders: `C:/dh218/act1rogue/cases/w2a-*` (outside the repo).
Not pushed. Not merged into any other branch.

## 1. Verdicts (per skill)

Evidence key: "log" = a line in the case `run.log`; "frame" = a PNG from the same run (viewed); "sheet" = the class sheet cells printed by the timed-buff log line.
Visuals were NOT compared with the original: there is no original Assassin gameplay footage in the repo (phase-1 report, section 6). Sounds: the file name was resolved on disk; nobody listened to them.

| Skill (type, lvl) | Cast | Animation | FX | Sound file (resolved) | Effect / numbers | Verdict |
|---|---|---|---|---|---|---|
| Assassination (A, 21) | ACCEPTED: `Source skill key=2 ... phase=1->3 skill=Assassination`, MP 65.25 -> 11.25 (rank 10, cost 14+4L = 54) (`w2a-assassination-4`, frame 3338) | Use sequence ran to phase 3; hero pose not compared (no original footage) | frame `f3380.png`: purple beam over the struck lizard (set 142 assassinate FX) | kill cue `sfx_passive_skill_activate_4` -> `sfx_passive_skill_activate_4.wav` (staged3, exists); sequence cue `sfx_skill_rogue_ass_assassinate.wav` exists, playback not logged | Gate log: `Execution gate ... targetLevelRaw=256 casterLevelRaw=5376 killBelowRaw=5378` -> kill. `Source death End34 actor=7118... serial=3427` (lizard dead). No damage roll (by design of the Lua) | **WORKED for the kill branch.** Not run: the combat-roll branch (target at or above caster+2) and the BOSS branch (no boss in the run). Execution kill printed no `Source death reward` (XP/trophy/loot of the kill not checked) |
| Blindness (A, 15) | ACCEPTED: key 2 frame 3338, phase 1->3, "empty target list" (buff-only row, expected) | Sequence 518 phase 1->3 (no separate anim check) | BUFF_FX `spell_dh2_assassin_blindness_persist` (set 212, staged); not inspected in a frame | `sfx_skill_rogue_ass_concealing_cloud.wav` (staged3, exists); playback not logged | `Timed buff applied frame=3374 ... class=Skill_Assassin_Blindness durationMs=18000 cells=198:0->665600` (Special_Sneak). Caster's first own hit at frame 3441 -> `Timed buff removed frame=3441 ... cells=198:665600->0` (OnTargetHit break WORKED). Lizard attacks on the hero: none between 3374 and 3441 (before: hits at 3314, 3354, 3363), but this is one second and is not proof. SetIsTargetable(false) is not logged anywhere | **PARTIAL.** Buff, duration, sneak cells and own-hit break verified by log. Target-acquisition suppression not observed |
| PoisonSting (A, 18) | ACCEPTED: key 2 frame 3336 (`w2a-poison-5`); "empty target list" expected (buff row) | Sequence 520 phase 1->3 | BUFF_FX `skill_dh2_assassin_poison_sting_persist` (set 145); frame `f3420.png` shows a green glow on the struck target (not compared to the original) | `sfx_skill_rogue_ass_plague_edge.wav` (staged3, exists); playback not logged | `Timed buff applied frame=3362 ... class=Skill_Assassin_PoisonSting durationMs=10500 cells=123:0->16640,124:0->20480,125:0->2688000` (Dot_Min 65, Dot_Max 80, Dot_Duration 10500 ms at rank 10). After the buff, the caster's hits carry a DoT: `DoT applied ... tickHp=68.0 / 71.3 / 69.8 / 73.0` (`DoT applied` lines). Ticks: `DoT tick target=... tickHp=71.3477 applied=1 targetDied=1` (5 lizards died from DoT ticks, `Source death End34` serials 3556-3977). Ticks are 1000 ms apart (CharacterDesign DoT_Tick = 1000) | **PARTIAL.** DoT logic WORKED in a real run (apply on hit, per-second tick, kill). Not verified: the DoT tick visual (no frame captured during a tick), the Slow_Target tail (Slow_Target_Chance 145, Slow_Duration 146: not in this path), tick damage scale beyond the IDA reading (section 3) |
| Contaminate (P, 18) | n/a (passive) | n/a | Debuff_Slow FX `AUTO_DEBUFF_SLOW` requested (not inspected) | n/a | `Timed buff applied frame=3134 actor=<lizard> class=Debuff_Slow source=contaminate strength=18 durationMs=10000 cells=46:0->-12800,47:0->-12800,48:0->-12800` (-50 walk/rotation/attack speed, 10 s). Three procs on lizard hits in the run (frames 3134, 3354, 3666) out of the lizards' melee hits on the hero | **PARTIAL.** Proc logic and the Debuff_Slow sheet effect WORKED in a real run. Not verified: the visible slow (only Speed_Modifier_Walk is consumed by the port, ES-BUFF item 3) |
| TreasureHunter (P, 15) | n/a | n/a | n/a | n/a | `Timed buff applied frame=60 ... class=Skill_Assassin_TreasureHunter source=stat passive TreasureHunter strength=15 ... cells=195:0->19200` (Special_Loot_Gold_Multiplier 75). Consumer: `features/loot/runtime_death_rewards_v1.cpp:405-417` passes the killer's resolved[195] as the gold value bonus (IDA `ItemObject::GetInventoryToDrop` 0x3ecae4 reads 195 from the killer). No gold drop occurred in these runs (lizards drop nothing) | **PARTIAL.** Data applied and consumer wired (by code reading); gold amount not measured |
| Assassinate (P, 12) | n/a | n/a | n/a | n/a | `... class=Skill_Assassin_Assassinate ... cells=92:0->9216,93:0->9216` (+36 each). Consumers in `game-data/combat.cpp:50-51`: 92 when the defender state is 9, 93 when attacker Special_Sneak (198) > defender Special_Sneak_Detection (199). Blindness sets 198 in this same run, so the 93 bonus is reachable | **PARTIAL.** Data applied; damage bonus not measured against a seeded baseline |
| blade_defect (P, 21) | n/a | n/a | n/a | n/a | `... class=Skill_Assassin_BladeDeflect source=stat passive blade_defect strength=21 ... cells=59:32000->41375` (Rating_Defense 125.0 -> 161.6, formula type 6 as fixed by ES-PASSIVE) | **PARTIAL.** Sheet effect applied; defence on the character panel and hit chance not measured |
| GrapplingHook (A, 12) | NOT ADMITTED (no profile row; the coordinator rejects it with the generic no-Use-arm detail) | n/a | n/a | `sfx_skill_rogue_ass_throw_knife.wav` (uid 139) resolves to a staged file; the Lua PlaySound is the only sound | not run | **DID NOT WORK (not admitted).** Blockers in section 4 |

Shared rogue skills (JumpKick, ViciousStrike, Roundhouse, Quickness) were not in this wave's scope; the act1rogue report covers them.

## 2. Engine changes (general, not per skill)

1. **DoT owner** (new, header-only): `features/status_effects/dot_tick_owner_v1.hpp` + `dot_tick_owner_v1_tests.cpp` (ctest `dot_tick_owner_v1`, 5 scenario groups). PROPS_AddDot capacity 1 per element, DoT_Tick boundaries (1000 ms, read from `original-cache/data/pydata/design_pycst.bin`), expiry, dead-target drop. Previously missing (ES-BUFF item 5 DID NOT WORK).
2. **CombatSession DoT hook** (`combat_session.cpp`): every applied hit reaches the DoT queue through the runtime's applied-hit observer (covers melee markers, skill hits and projectiles). A DoT is created when the ATTACKER's live sheet has Dot_Duration (125) > 0, the victim is not the player, and the hit applied. Tick = Dot_Min (123) + rand(Dot_Max (124) - Dot_Min). Ticks run in `CombatSession::update` as ordinary applied hits (no roll, no outcome bits) and are added to the session event list. Log lines: `DoT applied`, `DoT tick`.
3. **Blindness targetability** (earlier commit ffea7331): `TimedBuffSpecV1::suppress_selection` (acquisition only; attacks unchanged; restored on expiry/removal). Real-run effect not observed (section 1).
4. **Assassination execution gate** (ffea7331): profile fields `top_character_only`, `execute_below_level`, `boss_class_token`, `kill_sound_name`; the coordinator kills the top Character with the caster as killer (`session.script_kill_actor(target, caster)`), plays the kill cue, and logs `Execution gate`.
5. **Event passive listeners** (ffea7331, merged): Contaminate (defender-side percent proc -> Debuff_Slow on the attacker) and Blindness break (caster's own hit removes the buff).
6. **Merge fixes** (this round, windows-foundation merged into the branch): the four Illusionist rows (Figments, Eclipse, TimeControl, Terror) in `runtime_skill_use_profile_magesub_v1.hpp` were positional against a struct without `pre_range_from_temp_prop1` and `knockback_great_on_character_hit`; they now pass `false, false` before their enemy buff arm. `runtime_necromancer_skill_owner_v1_tests.cpp` `RecordingBuffSink` lacked the new pure virtual `apply_timed_buff_to_targets` (did not compile after the merge: its `RecordingBuffSink` lacked the pure virtual added by the illusionist branch). Both came in with windows-foundation, not from this branch. Struct tail order kept as the brief says; my Assassination row appends after `enemy_stun`.
7. **Diagnostics**: `DoT applied`, `DoT tick`, `Execution gate` lines (stdout). Timed-buff and damage logs were already present.

## 3. Original evidence (IDA, `libDungeonHunter2.so/pseudocode-all.c`; Lua in `assets-extra/.../prince_assassin_*.luac`)

- DoT compute: `CF_CalcDotDamage` 0x3b09c4 (line 129334): normal branch reads attacker Dot_Duration (125), tick = Dot_Min (123) + GetRandom(Dot_Max (124) - Dot_Min).
- DoT apply: `F_ApplyResult` 0x3b10b4 tail (129868-129883): non-player target, duration > 0 and tick > 0 -> `PROPS_AddDot` 0x3e2720 (buff `dot_normal`, capacity 1; property 126 = tick).
- DoT tick: `CharProperties::HandleDots` 0x3df3f0 (161399); driven by CharAI command 52 on a repeating timer with period `CharacterDesign.DoT_Tick` (CharAI OnInit, lines 150274-150459). DoT_Tick = 1000 ms (read from design_pycst.bin).
- DoT damage scale: `F_DotAttack` 0x3b2e68 calls `_F_CalculateResult(..., flags 0x20080000, element, tick)`; in `_F_CalculateResult` flag 0x80000 calls `CF__CalcDamage(..., a4 = tick, a5 = 3)`, and the `a5 == 3` branch sets the result damage to `a4` directly (line 130410-130416). So the tick is absolute damage (raw x256), no defence roll. The port follows this. Magnitude at rank 10 (65-80 HP per second) is therefore what the sheet says; its balance is not checked against the original.
- Assassination: `prince_assassin_assassination.luac` OnSkill_: `en_level < level + 2` on raw GetProp values; kill (`Character:Kill`) for non-boss, otherwise `SkillCombatRoll` (BOSS_CLASS for bosses). Atlas/Lua header text says "+2 levels" but the code compares raw x256 values, so it is effectively `<= level` (phase-1 E7). The port follows the code.
- Blindness: `prince_assassin_blindness.luac`: `SetIsTargetable(false)` on the caster for the buff life; `OnTargetHit_Skill` removes the buff when the caster hits (confirmed by the run, frame 3441).
- Sounds: `sounds.xml` labels resolved to staged files: `sfx_skill_rogue_ass_concealing_cloud.wav`, `sfx_skill_rogue_ass_plague_edge.wav`, `sfx_skill_rogue_ass_throw_knife.wav`, `sfx_skill_rogue_ass_assassinate.wav`, `sfx_passive_skill_activate_4.wav`. All exist in `windows-source-clock-v21.8-staged3/audio-assets/data/sounds/`. Not listened to.

## 4. Gaps and blockers

- **GrapplingHook (not admitted)**: (a) `HeadTo` for characters (phase-1 E9; `Character::_HeadTo` 0x3ba71c) is missing, so the pull cannot run; (b) the Lua delayed callback (`OnDelayedSkill_`, stun via `SetStunState(duration, true)` when not boss and not mini-boss) needs a rogue external owner for `SpawnSkillProjectile` (ProjectileTable 21) that does not exist yet (E10: the shared executor exists; the owner does not); (c) the stun itself exists for monsters (ES-STATUS) but a player-side admission has not been wired. Phase-1 E6 (IsBoss / IsMiniBoss) is present in the port via `actor_is_boss_v1`.
- **Assassination**: roll and boss branches not run; execution kill prints no death reward (XP/trophy/loot not verified; the original `Character::Kill` runs trophy and event code, phase-1 Q2).
- **Blindness**: target-acquisition suppression not observed; the "cannot target MC" behaviour is inferred from the code, not from a run.
- **PoisonSting**: Slow tail (Slow_Target_Chance 145 / Slow_Duration 146) not wired on the skill hit path; elemental DoTs (props 178-181, fire/water/...) not implemented (not used by any admitted skill here).
- **Contaminate**: the slow speed effect is only consumed for walking (Speed_Modifier_Walk, ES-BUFF item 3).
- **TreasureHunter / Assassinate / blade_defect**: data verified; the effect (gold amount, damage bonus, defence) was not measured against a baseline.
- **Visuals**: no original Assassin footage is in the repo; nothing here is compared to the original video. FX and animation are "seen in the port", not "matched to the original".
- **Sounds**: resolved to files, not heard.
- **Runs**: each run used a hand-built seed (skill on key 2, `--test-skill-rank` for the rank) and the lizard fight from `Swamp_LizadMan_Type1` with `--follow-actor ...!`. The Tab/Space plan is the same as the act1rogue chain.

## 5. Tests

- New unit test: `dot_tick_owner_v1` (ctest), 3 scenarios (single DoT over 3 boundaries, replacement / multi-element / forget, long frame).
- Earlier: `event_passive_owner_v1` section F (Blindness break) added to the merged test file.
- CTest after the merge with windows-foundation: see the final line of this report (section 6).

## 6. CTest and post-merge runs

- Full CTest on the merged branch (windows-foundation merged, DoT owner, all fixes): **100% passed, 215 of 215** (`C:/dh218/w2-assassin-ctest4.out`, 790 s).
- Post-merge real runs on the same build: `w2a-poison-merged` (DoT ticks still kill: `DoT tick ... tickHp=70.8 targetDied=1`, `66.6 targetDied=1`) and `w2a-assassination-merged` (`Execution gate ... targetLevelRaw=256 casterLevelRaw=5376` -> kill, phase 1->3).


## 7. Commits on the branch

- `ffea7331` Assassination execute gate, Blindness targetability, Contaminate/Blindness event listeners (verified before this round: 213/213)
- `59254b74` DoT owner, DoT diagnostics
- merge of `windows-foundation` (26 commits), with the fixes in section 2.6
