# Character / skill / faery coverage matrix (read-only inventory, 2026-10-13)

Base: `windows-foundation` @ bbdf182f (v21.7 line), package `.local-inputs/windows-source-clock-v21.7`. The unmerged `v21.7-mudlord` branch (B155) is not reflected except where stated. Nothing was built or run; statuses come from code, tests and tracker/coordination reports. Paths are relative to `port/windows-foundation/` unless noted.

Status vocabulary: **working-verified** = observed in a normal EXE run per a report; **implemented-unverified** = code exists, only unit/feature tests or a silent/log-only run; **missing** = no code path found; **unknown** = not determinable from the evidence. Audio: no cue has ever been verified by ear, so "wired" means "submitted to the output in a (silent) run" at best.

Data caveats: skill/faery/character tables decoded from the v1.0.2 pyarray files (identical to the package copies); the reference video is v1.0.3 (tracker B022 notes version differences). Decoders are scratch scripts outside the repo (`%TEMP%\inv\`).

## 0. Headline findings

1. Only 4 of 72 player skills can be cast in the EXE (Knight BashDown, GroundSlam, Charge; Rogue JumpKick). `generic_skills/runtime_skill_cast_coordinator_v1.cpp:115-145` whitelists these by name; every other skill returns `phase=rejected` (cpp:706-717). Verified in a normal EXE: BashDown, JumpKick (GroundSlam only on the Preview 10 EXE).
2. No passive skill has a gameplay effect (37 passives). Rank, training and Current/Next text exist; nothing applies a `Skill_*` class row to the live player sheet.
3. Subclass selection is not implemented (no `IsSpecTime` at level 12, no class change). The 48 subclass skills (24 active, 24 passive; levels 12-21) cannot be obtained at all. All 9 player rows share one model, AnimTable family and CharSounds row; a subclass differs only in SkillTree and name.
4. Mage has no basic attack in the EXE (animation-only profile, attack and Tab target disabled: `main.cpp:1117`, `:7699`). No player projectile exists in production (Mage staff, Rogue bow); the projectile code is feature-test only and not in CMake. Stance-switched attack banks (2H, dual, staff, bow) are only wired for Rogue at startup gear (`main.cpp:1708`).
5. Faeries: Celest is working-verified; Hotty casts but has no FX and is unverified; Rocky/Wetty/Windy have no spell. Only Celest has an unlock path (Swamp_Intro cmd 156); unlock sources for the other four are unknown. Faery passives (resistance buffs) are missing. Only the Celest model is staged.
6. Skill assets: all 35 active-skill clips are staged, but only 1 of 24 skill-FX models (BashDown FX164) is staged. 32 of 35 active skills resolve to a sound file that exists in the package. The package carries no player skill Lua (`assets/data/scripts/skills` has only `witch_ranged_00.luac`).
7. Documentation drift: tracker B028/I018/I019 and `features/audio/audio_source_asset_provenance_v1.json` still say Lizard WAVs are absent; the classes audit found them packaged (recovered from the iOS `sounds.zip`, `coordination/claude-preview15/IPA-report.md`). Tracker B157 row says OPEN while branch `v21.7-levelup` was merged (commit 82e50d8a).
8. Evidence gap: Knight is the only class with normal-EXE runs after Preview 15. No Rogue or Mage EXE run in `coordination/v20*..v21.7`.

## 1. Classes and subclasses

Original CharacterTable rows: Knight 263/264/265, Mage 290/291/292, Rogue 325/326/327 (plus KingPlayer 261, NetworkPlayer 295, unused). All nine: ModelFile `prince_modular`, ModularVisual -1, CharSounds row 1, FaeryList 1/2/3 (lists identical in content). AnimTable 48 (Knight) / 49 (Mage) / 50 (Rogue); the tables differ only in the class-select MenuIdle/MenuOnSelect poses. ClassID 77/94/120 shared by base and subclasses. `prince_low_poly_*` models are unreferenced and not packaged.

| Class / row | class data | model | anim set | weapon / attack anims | sounds | tested by | EXE evidence | only past Act 1 / notes |
|---|---|---|---|---|---|---|---|---|
| Knight (263) | working-verified (row, ClassID 77, recalc, level-up grants) | working-verified (prince_modular + saved gear; B008) | working-verified (table 48; locomotion switches on equipment, B102) | 1H sword bank working-verified; 2H/dual attack banks missing (bank not rebuilt on equip); axe/mace swing sounds unknown | swoosh + hit cue UID44 submitted; BashDown snd 190; not heard | 105 test files name it; many CTest | every EXE run v19-v21.7 | skill tiers at levels 0/3/6/9 |
| Berserker (264) | implemented-unverified (row, SkillTree 15, skills-page resolver) | same as Knight | same | clips staged (whirlwind, bersek, intimidate); no skill cast | `sfx_skill_warrior_bsk_*` present, wiring unknown | UI/skill-tree tests only (5 files) | none | all level 12+; class change missing |
| Paladin (265) | implemented-unverified (SkillTree 16) | same | same | aura/judgement clips staged; no cast | `sfx_skill_warrior_pld_*` present, wiring unknown | UI tests only | none | level 12+ |
| Rogue (325) | working-verified (ClassID 120) | working-verified (tests + Preview 9/11/13); B019 cross-class capture never confirmed | working-verified | dual-dagger melee working-verified (stance 2, roots 245/250) at startup gear only; later gear swaps not rebuilt; bow attack missing | dagger swoosh, dual 293/598/599 present; JumpKick 125; not heard | 32 test files; CTest `combat_session_source_attack_bank` | Preview 9/11/13 only | base tiers 0/3/6/9 |
| Archer (326) | implemented-unverified (SkillTree 28) | same | same | volley/multi/pin/poison/exploding clips staged; no bow attack, no projectile, no cast | `sfx_skill_rogue_arc_*`, `sfx_mc_shoot_arrow` present; no consumer | UI tests only | none | level 12+ |
| Assassin (327) | implemented-unverified (SkillTree 29) | same | same | clips staged (GrapplingHook uses NPC clip `npc_assassin_throw_knife`); no cast | `sfx_skill_rogue_ass_*` present | UI tests only | none | level 12+ |
| Mage (290) | working-verified (ClassID 94) | working-verified in tests and menus; EXE avatar capture unknown | movement/idle/hurt/death working-verified | staff attack bank (stance 3, roots 251/246) feature-test only; basic attack and target select missing in EXE (I008 open) | staff swoosh + `sfx_mage_staff_elemental_*` present; no consumer | 22 test files; projectile suites are runner-only | Preview 5/13/14 only; B026 overlap fixed | base tiers 0/3/6/9 |
| Illusionist (291) | implemented-unverified (SkillTree 22) | same | same | clips staged; no cast | `sfx_skill_mage_ilu_*` present | UI tests only | none | level 12+ |
| Necromancer (292) | implemented-unverified (SkillTree 23) | same | same | clips staged; no cast | `sfx_skill_mage_nec_*` present | UI tests only | none | level 12+ |

Weapon matrix (stance from `GetAnimStance`): sword 1H = working-verified; two-hander = locomotion verified (B102), attacks unknown/likely missing; single/dual dagger = working-verified (Rogue startup only); staff = locomotion verified, attack missing; bow = stance resolves in tests only, attack and projectile missing; wand, claw, axe/mace swing specifics = unknown; shield block input = unknown.

Subclass mechanics: `IsSpecTime` (fires at exactly level 12), `NativeSelectClassSpec` 0x43d178 and `NativeGetPossibleClassSpec` 0x449b10 exist only in older `engine-ui`/`level-world` libraries; zero references in `windows-foundation`. B021 (subclass art/chains) still open in the tracker.

Act 1 reachability (no Act-1-specific level cap found; max level 50/80/100 by difficulty): XP to next level = 100 x level. Footage pace: level 3 at Part 1 12:58, level 6 at Part 2 13:20. Playable content: Swamp + Bogwitch Cavern + Witch fight; Mud Lord (B155) unmerged; Darkwood load fails (LoadTemplate assertion 9). Skill points = 1 per level (level 1's point auto-spent on slot 0). By level: 0 = BashDown/JumpKick/ColdRay + 1 passive; 3 = GroundSlam/ViciousStrike/StoneEscape; 6 = Charge/Quickness/Cyclone; 9 = Roundhouse/ChainLightning (above footage pace, reachable by grinding; lizard = 8 XP). Levels 12-21 (all subclass content): not reachable in Act 1 as documented, and not obtainable in the port in any case.

## 2. Player skills (72)

Table: 35 active skills, one row each. "Cast" = castable in the EXE. Anim root and clip are staged for all 35. FX "N" = FX model BDAE not staged in the package (24 skills have an AnimTable FX set; 11 have FX -1, i.e. none in the AnimTable). Sound = AnimTable step sound ordinal (m = found by string scan of a clip marker; none = no linked cue found); the WAV exists in `audio-assets` in every listed case. Tests: "clip" = clip decode/preload only; "text" = skills-page text/UI only. No skill has a sound-specific test.

| Family / list | Skill (req lvl) | Anim root | Cast | FX | Sound | Test | Status |
|---|---|---|---|---|---|---|---|
| Knight | BashDown (0) | 347 | yes | 164 staged | 190 | strong (clip, FX164, exact-URI, activation, B097) | **working-verified**; ground-impact FX still open (B005/I015) |
| Knight | GroundSlam (3) | 350 | yes | 173 N | 185 | activation, training | working-verified on Preview 10 EXE only |
| Knight | Charge (6) | 349 | yes | 172 N | 48 | activation (heading) | implemented-unverified (cast and mana ok, no hit seen) |
| Berserker | WhirlingDrift (12) | 341 | no | 171 N | 553 | text | missing (cast); no spec flow |
| Berserker | Intimidation (15) | 340 | no | 168 N | 554 | text | missing |
| Berserker | Berserk (18) | 339 | no | 165 N | 557 | text | missing |
| Paladin | OffensiveAura (12) | 344 | no | none | 560 | text | missing |
| Paladin | PurifyingAura (15) | 345 | no | none | 558 | text | missing |
| Paladin | GuardingAura (18) | 343 | no | none | 561 | text | missing |
| Paladin | DivineJudgement (21) | 342 | no | 174 N | 559 | text | missing |
| Rogue | JumpKick (0) | 521 | yes | 160 N | 125 | clip, activation, HUD projection | **working-verified** (P13 EXE; hit damage in EXE not located) |
| Rogue | ViciousStrike (3) | 528 | no | 163 N | 225 | training only | missing |
| Rogue | Quickness (6) | 524 | no | 161 N | 552 | clip decode | missing |
| Rogue | Roundhouse (9) | 525 | no | 162 N | m 551 | clip decode | missing |
| Archer | Volley (12) | 516 | no | none | m 544 | text | missing (needs projectile) |
| Archer | PinShot (15) | 514 | no | none | m 543 | text | missing |
| Archer | MultiHitArrow (18) | 513 | no | none | m 545 | text | missing |
| Archer | ExplodingArrows (21) | 512 | no | none | m 542 | text | missing |
| Archer | PoisonArrows (15) | 515 | no | none | m 546 | text | missing |
| Assassin | GrapplingHook (12) | 519 (NPC clip) | no | none | none | text | missing |
| Assassin | Blindness (15) | 518 | no | 143 N | 548 | text | missing |
| Assassin | PoisonSting (18) | 520 | no | 144 N | 549 | text | missing |
| Assassin | Assassination (21) | 517 | no | 142 N | 547 | text | missing |
| Mage | ColdRay (0) | 413 | no (rejected) | 150 N | 228 | staging receipt + clip/do_skill test | missing (P13 reports are contradictory; current whitelist excludes it) |
| Mage | StoneEscape (3) | 423 | no | none (script FX `stone_escape_magefx` staged) | 526 | training | missing |
| Mage | Cyclone (6) | 414 | no | 151 N | 537 | text | missing |
| Mage | ChainLightning (9) | 412 | no | none | none | text | missing |
| Illusionist | Eclipse (12) | 416 | no | 152 N | 528 | text | missing |
| Illusionist | Terror (15) | 424 | no | 148 N | 532 | text | missing |
| Illusionist | Figments (18) | 417 | no | 147 N | none | text | missing |
| Illusionist | TimeControl (21) | 425 | no | 149 N | 529 | text | missing |
| Necromancer | Rise (12) | 422 | no | 159 N | 535 | text | missing |
| Necromancer | FireSkullRain (15) | 418 | no | 157 N | 536 | text | missing |
| Necromancer | Pestilence (18) | 421 | no | 158 N | 534 | text | missing |
| Necromancer | DeathClaw (21) | 415 | no | 156 N | 533 | text | missing |

Passive skills (37): no anim/FX/sound in the tables. For every row: rank/training and Current/Next text are implemented-unverified (rank+text working-verified in the UI only for Hardiness, B096/B122/B140); **gameplay effect missing**. Charge and Paladin EnergyBalance text bodies are unaudited (blank next-level text).
- Knight (req): Hardiness 0, FastMetabolism 3, Leadership 6, PotionAddict 9, CombatMaster 9.
- Berserker: Survival 12, KillingStreak 15, Toughness 18, Ravager 21, KillingFrenzy 21 (Ravager is passive in the table; one report calls it assignable).
- Paladin: Shield 12, EnergyBalance 15, Conversion 18, HolyWeapon 21.
- Rogue: Ambidextrous 0 (dual-wield works via items, the passive has no effect), Acrobat 3, ComboMaster 6, FindWeakness 9.
- Archer: BowMastery 12, SideStep 18, Precision 21. Assassin: Assassinate 12, TreasureHunter 15, Contaminate 18, blade_deflect 21.
- Mage: StaffMaster 0, MagisShielding 3, ElementalMastery 6, FaeryBlood 9.
- Illusionist: Mesmerize 12, Mirror 15, Disciples 18, Petrify 21. Necromancer: Leecher 12, Wrath 15, CarrionEater 18, Putrid 21.

Execution architecture: the table-driven layer (skills page, training, text, animation bank with `preload_current_class_roots`) covers all 72. The cast layer is per-skill C++ (4 whitelisted rows). The generic Lua/VM skill libraries (`features/skills_animation/native_skill_lua_services*`, `session_skill_animation`, `level-world/character_skill_*`) are compiled only into test target `session_skill_binding_tests`, not into `main.cpp`. The applied result covers HP/injury/death only; knockback, stun, DOT, leech, FX and audio tails of `F_ApplyResult` are not reproduced. Unreachable-in-Act-1 summary: the 48 subclass skills (levels 12-21); level-9 skills are above footage pace.

## 3. Faeries

Table (Knight/Mage/Rogue lists are all `[Celest, Rocky, Wetty, Windy, Hotty]`; slot 0..4; `*_mage` script rows are in no list and their Lua is absent). UI names: Celest = Celeste (lightning), Rocky = Primula (earth), Wetty = Maria (water), Windy = Sylph (wind), Hotty = Avalon (fire). The story NPC Celeste is the same character as the Celest faery.

| Faery (slot, element, model) | model | animations | ability | passive buff | unlock / act | page + HUD key 4 | sound | tests | status |
|---|---|---|---|---|---|---|---|---|---|
| Celest (0, lightning, `faeries_02_celeste`) | staged; rendered only as the story NPC, not as the player's own faery | staged (idle, walk, cast, cutscene clips); player cast uses prince Spells[0] sequence 348 | implemented (cast, FX dispatch bound at `main.cpp:3443-3458`, 10/20 MP, range 600/800, 5000 ms cd) | missing (`CheckFaeries` resistance buffs) | Swamp_Intro cmd 156 (Act 1); no IncFaeryLevel, level always 0 | working-verified (P14/P15) | pre-cast lightning sounds dispatched (silent run); `static_ball_killed` WAV now packaged | CTest `character_state_page`, `faery_cast_sound_v1`; session cast runner is PowerShell-only | **working-verified** (cast, page, HUD, unlock) |
| Rocky (1, earth, `faeries_03_rocky`) | not staged (63 KB in cache zip) | none (no faery body clips beyond celeste); `prince_spell_rocky` clips staged, unused | missing (script exists; key 4 logs "no spell") | missing | unknown | page and HUD working-verified for slot 1 | WAVs present, no consumer | CTest `character_state_page` | page/HUD working-verified; ability missing |
| Wetty (2, water, `faeries_01_wetty`) | not staged | as Rocky | missing | missing | unknown | locked-click rejection verified EXE; HUD blue icon seen | `sfx_faerie_ice` catalog name has no file (water variants exist) | as Rocky | page/HUD working-verified; ability missing |
| Windy (3, wind, `faeries_04_windy`) | not staged | as Rocky | missing | missing | unknown | slot 3 never captured in an EXE | WAV present, no consumer | as Rocky | page/HUD implemented-unverified; ability missing |
| Hotty (4, fire, `faeries_00_hotty`) | not staged | `prince_spell_hotty` + pre clip staged and used (sequence 351) | implemented; pre-cast and target FX never dispatched (`hotty_effects_v1` not in CMake); MP/cooldown not EXE-verified | missing | unknown | page working-verified; HUD slot 4 icon never captured | fire pre-cast sounds dispatched in P15; `fire` no-target label maps to thunder_22 (open question) | cast tests runner-only; session runner failed at P15, current state unknown | **implemented-unverified** |

Companions: Celeste NPC and Rene follow the hero (B077, working-verified); neither attacks or casts; Celeste stays at her off-floor placement after loading a post-opening save (tracker note); Rene ranged attack/heal not implemented. The player's own faery has no visual placement or follow. Later-act faery FX models (`faeries_fx_darkqueen/fogdot/madruk01/02`) are not staged. Difficulty is hard-wired to 0 (`faery_by_difficulty[0]`); cooldown is a 5000 ms literal (`main.cpp:5391`). Character rows `*FaeryPassive`, `Dummy*Faery` have no consumer (role unknown).

## 4. Sound coverage gaps

- Catalog `sounds.xml`: 578 rows, 497 resolve to a package file, 81 do not. Absent or misnamed rows: ImpactMetal1/2 (typo names), CriticalHit1, `sfx_faerie_ice`, `sfx_skill_mage_thunder_braid_add_2,wav` (comma typo), dark_queen_*, balrog_idle, wolf/demon/statue/assassin rows, checkpoint_activated, and all level combat-music VXNs (only swamp ambience VXNs are packaged).
- Characters: player hurt and death voices (CharSounds row 1) present but no EXE cue observed (unknown); level-up, menu and merchant cues submitted in silent runs only; footsteps intentionally silent (B058); Mage staff and bow cues have no consumer (no attack).
- Skills: 32/35 active skills resolve a cue; ChainLightning, Figments and GrapplingHook have no linked cue (candidate WAVs exist). 31 skills are not castable so none of their cues can fire; no skill-specific sound test exists. Audio provenance entries named "Quickness/Roundhouse" are animation BDAEs, not sounds.
- Faeries: Celest and Hotty pre-cast sounds dispatched but never heard; Rocky/Wetty/Windy none (no spell); no Hotty hit sound in the original.
- Enemies and bosses: Lizard attack/hurt/death are wired (tracker B028 text is stale); Witch, Serpent/Mud Lord, Moth combat cues unknown.
- Global: audibility of any cue has never been verified by ear; a listener pass is needed once.

## 5. Reusable-architecture observations (feeds the later audit)

Detail and file:line citations: working notes in `%TEMP%\inv\arch.md`, regenerate from these anchors.
1. `main.cpp` is 9,603 lines; one `main()` spans ~8,700 lines (per-frame body ~3,000 lines inline); 92 named lambdas, 531 `std::cout` trace lines, 187 includes. The B155 Mud Lord alone adds ~231 changed lines on its branch.
2. Boss owners are duplicated: `enemy_ai/runtime_ranged_skill_monster_v1` (Witch, 955 lines) and, on the branch, `bosses/runtime_swampking_core_ai_v1` (1,323 lines) each regex-parse authored Lua, keep their own projectile list and take near-identical service structs (about nine identical lambdas in `bindEnemyAI`, `main.cpp:5603-5663`). Each new owner adds ~6 fan-out sites (focus byte, projectile draw, permission, update, reset).
3. Three projectile implementations plus a fourth unlinked one (`effects/runtime_session_projectile_*`, ~2.8k lines, not in CMake); hero ranged skills have none. Draw uses ad hoc id domains (0xB154, 0xB155).
4. `bosses/runtime_boss_encounter_plan_v1` hard-codes Swamp King facts that the AI owner re-parses from Lua; the session helper is in no CMake target (the "not wired" report is confirmed).
5. Hero skill cast arm (`main.cpp:7736-7854`) and faery cast arm (`:7855-7905`) re-assemble the same target facts; the coordinator whitelists 4 skills and 2 faery scripts by name (`coordinator.cpp:115-180`); `faery_slot_has_spell_v1` is hard-coded `slot==0||slot==4`; Hotty range `rank==0?600:800` is inline.
6. Class gates in `main.cpp:1117` and `:1708` (Knight/Rogue ids); attack bank built once from startup gear.
7. Actor admission is script-name sniffing (`script=="monster"`, `scriptVisualOnlyProfiles`, `scriptGameplayActors`, branch-only `scriptIdledActors`); this produced the B080/B154/B155 "boss is an untargetable prop" bugs. Three different spawn-level policies (table level, `spawn_level_raw_v1` without range, branch-only dynamic range).
8. Two level-up code copies exist in older libraries (`level-world/player_progression_v1` vs the live `loot/runtime_death_rewards_v1`); the live path is single. Two companion stacks (four unbuilt generations) and a dead faery-menu stack remain; compiled `.o`/`.exe` files are checked into `features/enemy_ai`.
9. Faery table load duplicated (`main.cpp:1673-77` vs `:4433-40`); cooldown-clock reset copied four times (`:2245, 6299, 7375, 7486`); Celest FX dispatcher bound in the generic effects binder while Hotty uses a different style.
10. Most generic_skills, faery cast, attack-bank, combo, projectile and class tests are PowerShell runners, not CTest (174 `add_test` lines in `CMakeLists.txt`); v21.7 CTest total is unknown (v21.3 baseline 177/177; B157 report says 182/182 on its branch).

## 6. Suggested work order (rough effort, one engineer-day = 1d)

| # | Item | Effort | Why now |
|---|---|---|---|
| 1 | Hygiene: correct stale tracker rows (B028/I018/I019, B157, B155 note), register PowerShell-only tests in CTest, delete checked-in `.o`/`.exe`, record v21.7 CTest total | 1-2d | cheap, removes false gaps from every later audit |
| 2 | Stage assets: player skill Lua (`prince_*.luac`), the 23 missing skill-FX BDAEs, faery models 00/01/03/04, correct sound filename mismatches | 1-2d | pure packaging; unblocks everything below |
| 3 | Single level policy + actor admission classifier + targetability owner | 3-5d | prevents the recurring boss-as-prop bug before more bosses |
| 4 | One projectile/ranged executor (promote `runtime_session_projectile_*`), route Witch and hero through it | 4-6d | prerequisite for Mage attack, Archer, Rene, Act 2 casters |
| 5 | Skill use-profile table replacing the 4-name whitelist; add base actives: ViciousStrike, Quickness, Roundhouse, ColdRay, StoneEscape, Cyclone, ChainLightning (+ verify Charge, GroundSlam) | 5-10d + ~1d per skill | biggest player-facing gap in Act 1 (Rogue and Mage kits) |
| 6 | Mage basic staff attack + Tab target (I008), Rogue/Knight attack-bank rebuild on equip (2H, dual), bow attack | 5-8d | classes other than Knight are barely playable |
| 7 | Passive-effect application (reuse the `SetTempProps`/`ApplyPropClass` emulation already used for text) and audit unaudited text (Charge, EnergyBalance) | 3-5d | 37 passives do nothing; also affects Act 1 balance |
| 8 | Faery ability registry keyed by table script; Hotty FX; Rocky/Wetty/Windy spells; passive resistance buffs; own-faery follow visual | 6-10d | enables four faeries; first research the unlock/level scripts (1-2d) |
| 9 | Specialization flow (level-12 `IsSpecTime`, class change, SkillList reload, point rules) | 3-5d | gate for all 48 subclass skills (B021) |
| 10 | Subclass skills: 24 actives plus 24 passive effects | ~1d each, 30-45d total, after 4, 5, 7, 9 | Act 2+ content scale |
| 11 | Authored-AI runner (or BossDefinition table) and `EnemyAiHost` extraction from `main.cpp`; break up `main()` incrementally | 10-20d + 3-5d | per-boss cost is ~1-1.5k lines today |
| 12 | Verification campaign: normal-EXE runs for Rogue and Mage (avatar, attack, skills), Hotty MP/cooldown, GroundSlam/Charge on v21.x, one listener pass over audio | 3-4d | closes the "unverified" column; needs a human for audio |

Open questions that block estimates: Act 1 end level and total XP; unlock/level scripts for faeries 1-4 (later-act campaign scripts are not staged); exact `F_ApplyResult` tail needed per skill; whether `runtime_session_projectile_*` still compiles; ItemTable stance for wands/claws.
