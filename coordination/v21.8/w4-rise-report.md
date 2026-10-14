# w4-rise: Putrid DoT feed and Rise summon (v21.8 wave 3, 2026-10-14)

Branch `v21.8-w4-rise`, worktree `.claude/worktrees/w4-rise`, subst `V:`, build dir `C:/dh218/w4-rise-build`. Base `windows-foundation` 7f0d250a (unchanged at the end, so no merge was needed). Not pushed, not merged.

Evidence key: UT = unit test (`runtime_necromancer_skill_owner_v1_tests`, real CombatSession); RR = real hidden-window run (real keys or the skip aid; frames PNG viewed where stated). Run logs: `C:/dh218/necro/cases/<case>/run.log`. Labels: PROVEN = read in the original Lua/IDA or atlas; INFERENCE = my reading.

## Verdicts

| Item | Verdict | Evidence |
|---|---|---|
| (1) Putrid DoT feed into the general DoT owner | **WORKED** (real run) | RR `w4put-12`: 47 hits by lizards landed on the player. Three Putrid procs (`DoT added (buff) ... durationMs=5500 tickHp=40`). Ticks `Damage ... marker=dot_tick removed=40` at frames 1288 and 1525 (about 62 frames = 1 s apart), then `removed=7.5 dead=1` at 1350 and 1587. `Source death reward ... xpRecipients=1` for both lizards. Frame `w4put-13/f1340.png` (between the first tick and the kill): lizard at low HP bar with a damage number; no DoT-specific FX visible. |
| (2a) Rise: runtime summon of RisenZombie into the session | **WORKED** (real run, through the spawn-pool owner via `--spawn-test`) | RR `w4spawn-1`: `SPAWN ok template=RisenZombie profile=RisenZombie row=323 ... pos=-2850,-100,255 ... clip=source_spawn_state`; lifecycle `state=3 enabled=1 physical=1 collisions=1`. Frame `w4spawn-1/f2000.png`: an extra armed figure beside the Necromancer (INFERENCE that it is the summon, from its position and the spawn line). |
| (2b) Rise: the Rise skill summon (landing -> Summon -> slots -> lifetime) | **DID NOT WORK in real runs (cast not accepted in the harness); UT only** | RR `w4rise-1` and `w4rise-2`: no `Source skill key` line in either, so no cast, no landing, no Use. UT (`runtime_necromancer_skill_owner_v1_tests`, section B4): three slots; a 4th Use force-kills the oldest (despawn list `{100}`) and summons into its slot; a dead summon's slot is reused without a Kill; summons expire at SnS_Duration = 9000 ms (none at 8999 ms); cleanup kills every live summon and stops its timer. |
| (2c) Rise: ally behaviour (AI faction) | **PARTIAL** (data read; not seen in play) | PROVEN from the atlas: `ai_factions` row 13 is named `RisenZombie` (+1000 to faction 11, -1000 to factions 6, 7, 8). The Necromancer's AIFaction is 11 (`Players`). So the faction is authored data and the Lua comment "no solution atm" does not apply to the faction. In RR `w4spawn-1` (3900 frames, lizards attacking the player) the summon dealt and took no logged damage: ally combat NOT verified. |
| (2d) Rise: the original free-spot probe (MoveTo(dir, 650, true) + HasPath, four directions) | **DID NOT WORK (not built)** | Only the front TeleportTool projectile is launched (as in w2). HasPath (IDA 0x38e98c) reads the actor path list at +512 after MoveTo. The port has no path query for an arbitrary point; the mage `teleport_path` service is never assigned in main.cpp. |
| (2e) Rise: SetLevel(plvl), LOCK/UNLOCK, PlayFX on the risen | **PARTIAL** | PlayFX(spell_dh2_prince_necromancer_rise) at the landing is wired (not seen in a real cast). SetLevel is not applied: the spawn line shows `level=-1` (profile base level). LOCK/UNLOCK are not modelled (INFERENCE: they gate targeting). |

## What the original does

- Putrid (`prince_necromancer_putrid.luac`, PROVEN): OnTargetHit on the player's AI; defender == self and attacker in melee range; GetRand(0,100) < TempProp1 (percent) gives the attacker `CreateBuff(Skill_Necromancer_Putrid, TempProp2 ms, false, rank, AUTO_DOT_00_NORMAL)` and `ApplyBuff`. The class sheet carries Dot_Tick_Damage_Normal (prop 126) = 30 + 2 L per tick. The DoT path (`PROPS_AddDot` 0x3e2720, `HandleDots` 0x3df3f0, see w3-dot-report) takes the tick from prop 126 and the duration from the buff; expiry clears it.
- Melee gate (PROVEN, IDA `CharAI::AI_IsInMeleeRange` 0x3d6188 via `Character::_TargetInMeleeRange` 0x3b7020): 3D distance between the attacker and its target <= (attacker MeleeRadius + target MeleeRadius). The port used the attacker's radius alone, which rejected every landed hit in the first real run.
- Rise (`prince_necromancer_rise.luac`, PROVEN): `count = checkslot()` (empty slot; else a dead summon's slot; else -1 -> ForceKillAndSpawnNew on the oldest). Spawn = `Summon(CharacterTable RisenZombie, false, x, y, z, false)` + LOCK + PlayFX(RISE_FX) + SetLevel(plvl). One-shot timer `StartSkillTimerCB(bufftime, false, killed, count)`, bufftime = SnS_Duration (9000 + 1000 L ms; 10000 when 0). OnSkillCleanUp = KillAllSummoned. RisenZombie is CharacterTable index 323 (answers phase-1 open item Q11; the spawn line prints `row=323`); its AnimTable is 62 and AIFaction 13.
- The Lua header says "Time to respawn after death: 2 sec" but the code has no such delay (Q10 stays open; the port follows the code).

## Engine changes

1. `combat_session.{hpp,cpp}`: `CombatSession::add_dot_v1(attacker, target, duration_ms, tick_hp)` = PROPS_AddDot for the normal element, into the same DoT owner the applied-hit path uses (capacity 1, ticks, killer = attacker). Diagnostic `DoT added (buff)`.
2. `runtime_necromancer_passive_owner_v1.{hpp,cpp}` (Putrid): after the buff is applied, the buff class sheet's prop 126 (x256) and the buff duration feed `add_dot_v1` with the player as killer (INFERENCE: the Necromancer owns the passive). `putrid_dots()` counter. Melee gate changed to the IDA form (sum of both melee radii, 3D distance).
3. `combat_result_event_bus_v1.{hpp,cpp}` (general fix): the Session's player is `invalid_actor_id` (18446744073709551615). The bus refused every invalid receiver, so every player listener was silently dropped in real runs (`subscribe` returned 0). `set_player_identity(player)` declares the player ID; `accepts_receiver` accepts an invalid ID only after that. `main.cpp` declares it on the passive bus. **Lead: this changes behaviour for every existing passive that subscribes the player through this bus (event passives, archer passives). They now receive real hits. Their real-run behaviour was not re-checked in this wave.**
4. `runtime_necromancer_skill_owner_v1.{hpp,cpp}` (Rise): `RuntimeNecromancerSkillServicesV1` gained `summon`, `despawn`, `is_dead`. `use_rise` implements checkslot / ForceKillAndSpawnNew / one-shot lifetime timer (SnS_Duration, sheet prop 176) / PlayFX on the landing. `rise_release` = killed() / KillAllSummoned (stop timer; Kill + UNLOCK when alive). `cleanup` kills all live summons; `reset` drops them. Test seams `set_rise_landing_for_test`, `rise_live_summons`.
5. `main.cpp`: (a) a Necromancer reserves 3 pool slots of RisenZombie (`spawnSlotsFor`), through the existing spawn pool and derived profile (`SPAWN profile derived from tables: RisenZombie animationTable=62`); (b) the owner's services call `spawn_character_v1` (clip none, i.e. Summon spawn=false), `despawn_character_v1`, and an IsDead query (absent = dead).
6. `runtime_skill_activation_v1.cpp`: the hard error "Original SkillTable has no supported authored Anim sequence root" now names the skill and the root (diagnostic only).

## Tests

- `runtime_necromancer_skill_owner_v1`: passes. New checks: Putrid queues one DoT per applied buff (`putrid_dots()`); Rise summon slots (B4); bus player identity (declared invalid ID delivered; undeclared refused).
- `dot_tick_owner_v1`: passes (unchanged).
- Full CTest (`ctest -j4` from `C:/dh218/w4-rise-build`, toolchain on PATH): **218/218 passed** (`C:/dh218/w4-rise-ctest2.out`). No merge needed: `windows-foundation` had not moved since the branch point.

## Real-run notes and harness findings

- Seed `necro-putrid` fails at start: the seed writer puts the passive Putrid on hotbar slot 0, and the skill-animation preload throws for a passive with no Anim root (`skill=Putrid anim=-1`). Passives must not sit on the hotbar; `subclass_save_writer_v21_8` should be fixed. The Putrid runs use `necro-deathclaw` with `--test-skill-rank Putrid=5`.
- The tutorial/cinematic ("CombatTuto" Script_WaitDialog) does not advance with the Space schedule copied from dc-3 in this base. The same schedule also stalls the w2 exe (control run `w4put-9`), so this is harness timing, not a regression from this branch. `--campaign-skip-frame 1000,...,2000` ends the cinematic (`EndScriptedCutScene ... skip=1 frame=1001`) and lets lizards engage.
- Rise casts were not accepted after the skip aid (no `Source skill key` line in `w4rise-1` / `w4rise-2`). Not investigated further in this wave.
- Sounds: no summon or DoT path plays a sound in this port (Rise and Putrid have none in the Lua). Nobody has listened to any sound.
- Evidence: `C:/dh218/necro/cases/w4put-12/run.log`; `w4put-13/f1300.png f1340.png f1400.png f1530.png`; `w4spawn-1/run.log`, `w4spawn-1/f2000.png`; `w4rise-1`, `w4rise-2` (no cast).

## Gaps (not claimed)

- Rise free-spot probe (MoveTo / HasPath, four directions) not built; only the front projectile.
- Rise cast not reached in real runs; the summon owner path is UT only.
- Risen ally combat not observed; SetLevel and LOCK/UNLOCK not modelled.
- Putrid DoT FX/tint: none located (the Lua passes AUTO_DOT_00_NORMAL as the buff FX; not dispatched by the port).
- Monster DoT killer = player is an INFERENCE.
