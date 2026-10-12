# B154: the Bogwitch cannot be targeted or killed (Opus session)

Branch `v21.6-b154` (worktree `.claude/worktrees/v216-b154`), base `windows-foundation` `c331cd4c`. Not pushed. Tracker and `docs/RESOLVED-BUGS.md` not edited (integration lead/root own them).

**Status in one line:** fixed and runtime-verified with real Tab/Space input. With the current package assets the witch is targetable, damageable (with her AnimTable hurt reaction), killable (dying clip, despawn) and the encounter continues (Return_Cinematic dialogs, QUEST COMPLETED, `IsAfter_Swamp_KillWitch2`). Her ranged attack, her 10 s teleport, the projectile, the impact FX and the boss music were also implemented and verified, but they need nine original files that the v21.6 package does not ship yet (see section 5).

Built EXE: `C:/dh216/b154/after-f.exe`, SHA256 `9df6ec24c42bf42cdef2a32fd6b388be5709c41e68bfc794146ce883cb2dc312`. "Before" EXE: `C:/dh216/b154/before.exe` (base `c331cd4c`), SHA256 `279c70e518ce2a13436c82ea1c9ac9e705e4feb4244f5575dea972dd7deb7771`.

## 1. Investigation

### 1.1 Visual evidence (original)

Source: `.local-inputs/reference-video/dh2-act1/Dungeon Hunter 2 (v1.0.3) Part 1 [720p]` (decodes at 640x360). I extracted contact sheets at 1 fps (940-1140 s), 4 fps (966-1006 s), 8 fps (997.5-1000.5 s), 10 fps (975-977.5 s) and 15 fps (978.2-979.8 s). Selected sheets are in `coordination/v21.6/B154-evidence/original-*.jpg`.

Direct observations:

- **Intro and start of the fight.** The intro ends with the witch close-up at about 966-967 s. Gameplay resumes at about 968 s. The witch is visible a short distance from the hero, and the priest (Celeste's companion) stands at the lower right.
- **Targeting.** From about 970 s the target plate "...arslickt, Ravenous Bogwitch" is shown, with a level icon and a red HP bar, and a red target ring is drawn under her. The hero's hits show white numbers on her (3-29).
- **Her attack.**
  - About 0.2-0.3 s after each cast, a green burst appears centred on the hero, with either a red number (17-23) or an orange "Miss". At 15 fps a faint orb can be seen travelling from her hands toward the hero.
  - Casts come about every 1-1.5 s.
  - She does not melee.
- **Teleport.** White flash bursts appear at two places at once (her old and new position) at about 978.0, 988.0 and 998.0 s, so about every 10 s. After a teleport she is on the other side of the hero or near the priest.
- **Hurt reaction.** At 4-15 fps I see no clear flinch on most hits, only blood and hit sparks. This is consistent with her high Hurt_Resist (data below). I treat "rare flinch" as an inference, not an observation.
- **Death and what follows.**
  - At about 1000.0 s the last hit (12) lands with a red blood burst and "+49 EXP", and she collapses. A green burst still lands on the hero about 0.1-0.4 s after her death: an in-flight projectile.
  - SKIP appears and the return cinematic plays:
    - the camera moves to the portal (1001-1004 s);
    - Celeste: "crisr82! Her heart... it turned into an orb!";
    - Celeste: "I see... this is her crystallized power. We should be able to break the seal with this.";
    - "Come on, let's head back and open that door";
    - QUEST COMPLETED "Defeat the Bogwitch in the Northeast Cavern", reward 50 EXP / 50 gold (about 1019 s).
- **User screenshot** `35094ffd-...png`: the hero is adjacent to the witch with no target plate and no ring. This is the failure.

### 1.2 Logic evidence

Sources: the IDA export `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode`, the authored scripts (plain Lua text in `.luac`) under `.local-inputs/assets-extra/android/data/scripts`, and the pydata tables. The pydata tables are byte-identical in the v21.6 package for Character/AI/Anim/Skill/Class. The ProjectileTable exists only in assets-extra.

**Corrections to the earlier Haiku report (`B153-B154-report.md`):**
- **AnimTable 76 does have `Injured`** (sequence 763, `witch_hurt.bdae`). The audit file it relied on lists only the seven profile states (`kProfileStates`), and Injured is not one of them.
- **Her attack clips carry a named event.** `witch_attack_01/02.bdae` carry `do_skill` (at 466.7/366.7 ms), not `attack_mainhand`.
- **WitchTeleport is never called.** The "WitchTeleport idles her" lead points at a script that no trigger, quest slot or Lua calls.

**Why she is untargetable (port).**
1. `collect_script_gameplay_actors` (`features/campaign_host/actor_clip_manifest.cpp`) drops every spawned actor that any script retires (kind 29). The only retire of `_prim_Monster_04_56` is in `SkipCutscene_Bugfix` (console text "THIS IS WHAT YOU GET FOR QUITTING DURING A CUTSCENE", run by the 70x70 `_prim_TriggerZone_BugFix`). Normal play never reaches it.
2. `main.cpp` therefore admits her as an animation-only "script visual actor" (`animationOnly`, no `receiveDamage`). That makes `traits.targetable=false`, so `PlayableActorWorld::eligible_target` rejects her for Tab, melee and skills.
3. Making her a normal derived monster instead fails `CombatSession` initialisation ("no damage marker attack_mainhand"), because her Attack clips carry `do_skill`.

**The original rules that make her a combat actor and drive her fight:**

| Topic | Original rule / data | Source |
|---|---|---|
| Lifecycle | Intro_Witch spawns her (kind 30) and SetActorPositions her. EndScriptedCutScene `PutCharacterInIdle("All")` hands her to the AI. Return_Cinematic opens with `KillActor` on her by name. WitchTeleport (uncalled) puts her in Idle by name. Quest `Swamp_KillWitch` objective KillXEnemies oid 435 (Witch row) -> state 8 PreCompleted -> slot 13 Return_Cinematic. | compiled bank `002_swamp_witchcave`, `Quest::SetState 0x480c78`, `swamp_quest_event_route_v1.cpp` |
| Character row Witch (435) | AI 74 `witch` (Type 4 Monster, Flags 5 Boss+Default), RangeMinDistance 600, RangeMaxDistance 5000, RangeProjectileID 39, SkillTree 35 (`WitchSkills=[122]`), ClassID 254 (Max_HP 250+150*Level; Hurt_Resist 45+4.3L), Visual_BlinkWhenHit 1 | pydata |
| Range events | `CanRangeAttack` 0x3a4d3c is true (RangeProjectileID != -1). `_UpdateTarget` 0x3cb908 raises 16 OnTargetInCloseRange when d < RangeMin (`AI_IsInCloseRange` 0x3d63d8), 15 OnTargetInRangedRange when RangeMin <= d <= RangeMax (`AI_IsInRange` 0x3d6604), else 14 OutOfRange. The melee event 17 is never raised for such actors. Distances use `GetTargetPosition`. | IDA |
| AI script `witch.luac` | Close range: `Stop(); LookAtTarget(); DoSkill(0)`. Ranged range: `MoveTo(GetTarget())` while Idle without a path. Out of range: Stop, ClearAggro, ClearTarget. Enemy spotted: `PlayMusic("SwampKingMusic",500)`, Do_SearchVictims (Player filter), `StartTimerCB(MOVE_DELAY=10000, true, DoTeleport)`. Target died: Stop, ClearTarget, stop timer. Died: `PlayMusic("SwampHubAmbientMusic",1000)`, stop timer. `MarkAsFlying(true)`. | script text |
| Teleport | DoTeleport: Stop; PlayFX(`spell_dh2_prince_mage_stone_escape_magefx`) at the old position; PlaySound `sfx_teleport`. Then MoveTo/HasPath/WarpTo relative to the look vector: forward 900, else back 900, else right 900, else left 900. Then PlayFX at the new position. `Character::_MoveTo/_WarpTo` relative branch = position + x*cross(Z,look) + y*look + z*Z. | script, IDA 0x3bada8/0x3bb268 |
| DoSkill | `_DoSkill` 0x3b8bd8 -> `AI_UseSkill` -> `AI_BeginSkill` 0x3d86bc. Usable when not already using a skill and the script check passes (witch_ranged_00: always true, no cooldown). `SM_SetSkillState` 0x3c6670 enters state 6 with SkillTable Anim 756 (= her Attack root Witch_Attack1H, type 2 over 754/755 pre/attack/post, speed 1.3). | IDA, pydata |
| Skill script `witch_ranged_00` | pre: Enemy filter, FrontalFirst, `TargetListSearch(RangeMaxDistance)`, LookAt. At `do_skill`: LookAt + `SpawnSkillProjectile(RangeProjectileID)`. Hit: delayed_check (target is a Character), delayed: `SnS_Level = skill_level`, `ApplyPropClass(Swamp_WitchRangedAttack)`, `SkillCombatRoll(target)`. | script text |
| Skill level | `GetCurrentSpellInfo` 0x3b6e30 is only reached from OnSkillUpdate. `UpdateAllSkills` runs for the local player only (`Character::InitFinal`, `PlayerManager::_ManageCharacters`), so a monster keeps the script's initial `skill_level = 0`. | IDA |
| Projectile | `SpawnProjectile` 0x3da510 -> `ProjectileManager::Spawn` 0x3e701c -> `Projectile::SetInfo` 0x3e58cc: origin = owner target position, destination = origin + look*1000, no target lock, model from ProjectileDict. Row 39 `bogwitch`: Velocity 20 (Box2D, x100 game units/s = 2000 u/s), Timer 4000 ms, DisappearOnHit 1, FriendlyFire 0, ImpactFX 76 `WitchImpactFX` (`spell_dh2_hurt_bogwitch.bdae`), ImpactSFX 613. `HandleImpactFX` 0x3e4eec plays a Character peer's impact FX at its `GetTargetPosition`. | IDA, ProjectileTable |
| Damage | Class 242 `Swamp_WitchRangedAttack`: SnS_CleanUp, SnS_MinDamage 15+1.6L, SnS_MaxDamage 18+1.8L, SnS_Push_Target_Chance 50. | ClassTables |
| Injury | `SM_SetInjureState` 0x3c5d84 (3000 ms gate) is reached only on the Injure bit of the result, so with her Hurt_Resist most hits do not flinch her. | IDA (agent audit), existing `ActorCombatRuntime::start_injure_reaction` |
| Music | `LuaScript::_PlayMusic` 0x37e420 = `GetMemberIDByString<Sounds>(name)` then `VoxSoundManager::PlayMusic(id, change=1, 0, fade)`. | IDA |

### 1.3 Expected behaviour and chosen implementation

She is a Monster that the player can target and kill. She takes the shared combat damage with her own Injured/Died poses. Her outgoing attack is DoSkill(0): the Attack1H sequence in state 6, a `do_skill` projectile, and a class-242 SkillCombatRoll on contact. Her AI is the `witch.luac` range-event shape plus a 10 s relative teleport.

The minimal reusable implementation:
- **Generic rule, no witch name.** It applies to any CanRangeAttack Monster whose authored AI script has this shape. The facts are read from the authored script text, the same pattern the port uses for `rene.luac` buffs. The same tables/script also describe `El_Lightning` (ai `witch`).
- **Incoming-only session profile.** The existing `animationOnly + receiveDamage` capability with her own AnimTable Injured/Died, so no fake melee marker is needed.
- **Outgoing skill through existing owners.** `CombatSession::play_actor_source_sequence` (Skill6) and `CombatSession::apply_source_result`, the same owners the player skills use.

## 2. Root cause

The witch's only Limbus retire sits in a skip-recovery zone script, but the B080 classifier treats any retire as "cutscene prop". That gives her an animation-only, untargetable profile with no combat binding. The normal derived monster policy could not replace it, because her outgoing attack is a projectile skill, not a melee marker.

## 3. Changes (commit on `v21.6-b154`)

- **`features/campaign_host/actor_clip_manifest.{hpp,cpp}`.** A spawned actor that some script retires stays a gameplay actor if a script also names it in a gameplay lifecycle step: kind 31 PutCharacterInIdle or kind 44 KillActor (both @12). Probe over all 14 package level banks: only `_prim_Monster_04_56` changes. The opening troll (spawned and retired by Swamp_Intro), the Darkwood lost merchant and the Swamp_Serpent fans stay props.
- **New `features/enemy_ai/runtime_ranged_skill_monster_v1.{hpp,cpp}`, in library `foundation_runtime_ranged_skill`:**
  - **Data:**
    - strict extractors for the AI script and the skill script shape;
    - a ProjectileTable + ProjectileDict decoder;
    - `derive_ranged_skill_monster_v1`, which chains Character row -> AI script -> SkillTree -> SkillTable row (Anim/ElementalType/Flags words 1/5/7, as the player skill owner) -> skill script -> projectile row -> ClassTables id;
    - `derive_ranged_skill_monster_policy_v1`, the incoming-only Idle/Injured/Died profile, which validates that the AnimTable has those states.
  - **Pure helpers:**
    - range events 14/15/16;
    - the source look vector and relative MoveTo/WarpTo point;
    - the skill formula sheet (defaults, `SnS_Level << 8`, class row with mode-1 reads from the live sheet; the same construction as the player coordinator);
    - projectile motion/contact.
  - **`RuntimeRangedSkillMonsterV1`**, the decision owner:
    - enemy spotted: music, closest Player, start the teleport timer;
    - close range: LookAtTarget plus DoSkill, i.e. FrontalFirst pre-search over the player and population (`query_source_character_targets_v1`), LookAt, then `play_actor_source_sequence` Skill6 (flags 0x6341) with event/finished/departed/checkpoint services;
    - ranged range: approach through the shared enemy navigation;
    - out of range: clear target;
    - target died and died handling;
    - `do_skill`: LookAt + spawn the projectile;
    - projectile flight; contact with an enemy body circle; impact FX at the victim's target position plus ImpactSFX; `apply_source_result` with mask `Flags|0x08000000`, element, `skill_name`, `do_skill`;
    - the relative teleport with FX, sound and path check (first candidate with a path).
- **`combat_session.{hpp,cpp}`.** Read-only `actor_source_plan(id)` and `source_sequence_policies()`, so an external source program can select from the plan the Session prepared.
- **`features/enemy_ai/runtime_enemy_navigation_v1.{hpp,cpp}`.** `path_exists`: the same sewn-floor FindPath with the actor's PFObject, without caching a route or selecting locomotion (source `MoveTo(point)` + `HasPath()`).
- **`features/audio/runtime_session_audio_v1.{hpp,cpp}`.** `play_named_music` (LuaScript::_PlayMusic through `script_play_music`).
- **`main.cpp`.** Localised blocks:
  - the ranged-skill row predicate and definition cache next to the derivation tables;
  - in `initializeCombat`, the ranged-skill profile is used instead of `derive_enemy_combat_policy_v1` for such rows (diagnostic AI is not set for animation-only profiles);
  - in `bindEnemyAI`, the owner and its services are built and run on the decision provider; while the controller is blocked only the projectiles advance;
  - the physical Skill Focus hook reads the monster's own SkillTable row (AnimIsMoving);
  - in-flight projectiles are drawn as their ProjectileDict model's embedded FX (`EmbeddedSceneFxV1`, own ID domain).
- **`CMakeLists.txt`.** The new library and the test `runtime_ranged_skill_monster_v1` (argv: v21.6 package assets, `assets-extra/android`).

## 4. Tests

- **New `runtime_ranged_skill_monster_v1` (66 checks).** It covers:
  - classification on the real banks: witch gameplay; cave ambush lizards gameplay; troll, lost merchant and Swamp_Serpent props; moths gameplay;
  - `witch.luac` facts (DoSkill 0, teleport 10000 ms repeating, FX/sound names, candidate order forward/back/right/left at 900, music);
  - rejection of `monster.luac` and of a script without range callbacks;
  - the `witch_ranged_00` facts;
  - ProjectileTable row 39 values, and rejection of a truncated table;
  - the Witch definition chain (Witch_Projectile 122, Anim 756, range 600..5000, projectile 39, class id), plus negative rows (a melee lizard; a `monster.luac` ranged bandit keeps its owner);
  - the incoming-only policy;
  - the formula sheet (SnS_MinDamage 15, SnS_MaxDamage 18, Push chance 50 at level 0; a bad class is rejected);
  - range boundaries, the look vector, relative points, and projectile speed/expiry/contact.
- **Full CTest, worktree build (Release, Ninja, llvm-mingw via `subst T:`).** 182 tests: the 181 existing tests plus this one. The run on the main changes passed 100% (`C:/dh216/b154/ctest1.log`, 426 s). On the final code, the first rerun was launched without the toolchain DLL directory on PATH, and 7 tests failed with 0xc0000135 (DLL not found); with PATH set they pass (`-I 1,9`). The full rerun with PATH on the final code passed **100%, 182 of 182** (`C:/dh216/b154/ctest3.log`, 219 s).

## 5. Runtime verification (hidden window, real input, isolated seed copies)

Harness: `C:/dh216/b154/tools/run-b154.ps1`. It is `run-case.ps1` with the v21.6 package and an `-Assets` override, and uses `quiet_click_sync.ps1` on a private desktop. Each case starts from a copy of the `slot1` seed, which resumes in the witch cave.

Recipe:
1. Menu Continue clicks.
2. `--player-to 6346.94,-10602.9,60.779@300`: the test aid puts the hero in the intro trigger zone.
3. Space taps at 2 s intervals to advance the intro captions.
4. Tab, then Space held, to fight.
5. Space taps for the return-cinematic captions.

Frames are full-size 1201x720 `--capture-at` PNGs, kept in `C:/dh216/b154/cases/<case>/`.

| Case | EXE / assets | Result |
|---|---|---|
| `before6` | before.exe / v21.6 package | Reproduced. Witch next to the hero, Tab selects another actor, STATE_TRACE `name=_prim_Monster_04_56 ... targetable=0`, no damage (`f1420.png`, same composition as the user screenshot). |
| `after1` | after-a / **v21.6 package as shipped** | Log: `[ranged-skill] ... outgoing skill owner unavailable: ... witch.luac not found`. Combat binding: "Incoming-only source actor ... targetable". Plate "Krslickt, Ravenous Bogwitch", red ring, 14 hits. One hurt (sequence 763, frame 1606). Death at 1785 (sequence 760 witch_dying), XP reward. Return_Cinematic: KillActor, portal camera, the three Celeste captions. QUEST COMPLETED. `IsAfter_Swamp_KillWitch2 false->true`. |
| `hurt1` | after-d / package | The same fight with 4-frame captures. A critical hit (yellow 69) is followed by the visible witch_hurt recoil (frames 1608-1640). Dying clip frames 1780-1864. |
| `full2`, `full3`, `final2` | after-b/c/f / package + 9 original files (overlay copy `C:/dh216/b154/assets-overlay`) | The hero stays at the intro end position (about 1250 away). She spots him (`SwampKingMusic` ordinal 213, the same track the intro ends on) and approaches to RangeMin 600. Then she casts: 12 casts, Attack1H with sound 612. Each `do_skill` launches a green orb with a smoke trail that flies to the hero in about 11 frames (`full3` f1321-f1336). Impacts show the green WitchImpactFX burst centred on the hero, with red 16-19.7 damage or "Miss". Two teleports (10 s apart) with white flashes at both ends, moving her 900 forward past the hero, plus `sfx_teleport`. Then Tab/Space: the hero kills her in about 7 s; her cast is interrupted by death (`from=6 to=12`); `SwampHubAmbientMusic`; Return_Cinematic; quest complete. No error or "not played" diagnostics. The hero ends at 57/169 HP. |
| `full4` | after-d / overlay | Hero adjacent from the start. She casts point-blank and teleports behind him; impact bursts at the hero's target position. |
| `skip1` | after-f / overlay | SKIP pressed during the first intro caption. The intro fast-forwards, the fight works the same (9 projectile hits, a teleport, death), quest complete. |
| `swamp1` | after-f / overlay | E-travel back to 001_swamp. The level loads unchanged (troll, SnackMan and SwampKing still "script visual actor" props), no ranged-skill profile, no errors. |

Evidence images (`coordination/v21.6/B154-evidence/`):
- before/after targeting, and original 976 s vs port f1820;
- original fight, teleport+cast, death and return-cinematic sheets;
- port: projectile flight, cast/impact/teleport (two runs), fight+death, hurt reaction, death, and return cinematic + quest banner.

**Runtime-verified:**
- classification;
- the incoming-only profile (Tab, hits, Injured, Died, despawn);
- Return_Cinematic, quest completion and condition flip with the shipped package;
- with the overlay:
  - AI target/spot/approach/close-range casting;
  - the Skill6 sequence and its physical Focus;
  - projectile motion, render and contact;
  - SkillCombatRoll damage on the hero, impact FX and sound submission;
  - the teleport (path, warp, FX, sound);
  - named music;
  - interruption by death;
  - the skip branch.

**Unit-only:** the script/table parsers' negative cases, the formula-sheet values and the projectile helper boundaries.

**Packaging requirement (needed before the attack/teleport work in a shipped build).** The v21.6 package has the Effects/Skill/Class tables and the FX textures, but lacks these files. They are copied byte-identically from `.local-inputs/assets-extra/android/data/`:
- `data/scripts/ai/witch.luac`
- `data/scripts/skills/witch_ranged_00.luac`
- `data/pydata/projectiles_pyarray.bin`, `projectiles_pyarraynames.bin`, `projectiles_pystructnames.bin`, `projectiles_dictionary_pyarray.bin` (the overlay put them at `data/`; `data/pydata/` also resolves)
- `data/3D/interface/spell_dh2_hurt_bogwitch.bdae`
- `data/3D/interface/spell_dh2_prince_mage_stone_escape_magefx.bdae`
- `data/3D/projectiles/projectile_dh2_bogwitch.bdae`

Without them the fix degrades as verified in `after1`: she is targetable, hurt-capable, killable and the encounter completes, but she does not attack or teleport, and a log line names the missing source.

## 6. Remaining gaps and uncertainties (honest)

- **Attack variant.** The Attack1H root is type 2. The source picks child 754 or 755 at random; the port pins the first group, the same convention as `derive_enemy_combat_policy_v1`. So she always plays `*_attack_01`.
- **Projectile contact.** XY contact of the projectile point with the victim's PFObject radius. The source uses Box2D sensors sized from the projectile model's XY AABB, so contact may happen slightly later than in the original. FriendlyFire 0 is honoured. TimeBetweenHit is irrelevant (DisappearOnHit).
- **Projectile hit after her death.** The original still damages the hero (21 at about 1000.3 s). Here the impact FX plays, but no damage is applied, because `apply_source_result`/`eligible_target` require a live attacker.
- **LookAtTarget during a cast.** The source calls `LookAtTarget()` on every close-range update even while casting. The port turns her only before the cast and at `do_skill` (LookAt). Whether CSSkill ignores mid-cast LookAt is unknown.
- **Pre-search scope.** FrontalFirst pre-search order is the player first, then the population. The source's object arm (AttackableOnly objects) is not modelled for this Enemy/NONE filter.
- **`MarkAsFlying(true)`** (PFObject flying) is not modelled. She walks with the shared floor navigation. Teleport goals keep her current Z through the SetActorPosition owner.
- **Level.** The dynamic level block in `Witch_OnInit` (LevelMax 4, LevelOffset 1, `GetCurrentLevelRange`) is not applied. Like every placed population monster in the port she uses table Level 1 (400 HP, level icon 1). The reference fight lasts about 30 s and shows a higher-level hero (+49 XP; the port gives 101), so her original level and HP are probably higher. This is a general population-level gap, not specific to B154.
- **Hurt frequency.** It follows the shared injure-bit result (her Hurt_Resist). Two of the logged runs contain one hurt and the others none. The reference video shows no clear flinch at 4-15 fps, so I cannot compare the frequency precisely.
- **Other actors with the witch AI.** `El_Lightning` (ai `witch`, later acts) gets the same owner only when it is admitted through the campaign/script path. An ordinary placed non-`monster` AI actor is still skipped by the existing ordinary-monster rule (`main.cpp` "ordinarySourceMonster"). Not changed.
- **Witch-specific script features not modelled.** `monster_OnTargetHit` (retarget the attacker when she has no target) and Lua timer granularity.
- **Teleport audio.** `sfx_teleport` is submitted as a plain non-positional sound; the source uses Character PlaySound.
