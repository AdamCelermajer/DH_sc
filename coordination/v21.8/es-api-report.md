# v21.8 ES-API report (branch v21.8-es-api, worktree .claude/worktrees/es-api, subst M:)

Base: windows-foundation (merged at b087f397; the branch carries the first ES-API commit 07b8bc65). Build: C:/dh218/es-api-build
(Release, llvm-mingw, CMake/Ninja 3.22.1).

## CTest

212/212 passed (C:/dh218/es-api/ctest3.out). The first follow-up CTest run reported 13 failures with exit code 0xc0000135
(a missing DLL): the background run had no llvm-mingw bin on PATH. Those are not code failures; the rerun with PATH set is green.

## Follow-up items

Evidence key: UT = synthetic-world unit test (runtime_skill_actor_api_v1_tests.cpp); SESS = asset-backed session test
(tests/combat_session_actor_transition_tests.cpp); ACT = asset-backed rogue activation fixture
(features/generic_skills/runtime_skill_activation_session_v1_tests.cpp, real coordinator and real CombatSession);
RR = real hidden-window EXE run.

| # | Item | Result | Evidence |
|---|---|---|---|
| 1 | Integration: one row through the arms (cue order, dispatch, HP write, named backup, Player filter) | WORKED (ACT) | ACT: JumpKick, through the real coordinator and session, with a test-only authored variant of the admitted row (`RuntimeSkillCastRequestV1::profile_override`; shipped table unchanged). Receipt cues in authored order: sound, self FX (caster), one target FX per rolled victim (4 cues). A duplicate do_skill marker adds no cue. Player filter over the live session finds nothing. Named backup of the victims returns only living members. SetProp(HP) on a victim reads back through GetPropHP and the ActorState mirror. `script_kill_actor(victim, caster)` is recorded once; the repeat records nothing. Output: C:/dh218/es-api/activation.out, line "PASS Rogue probe JumpKick: cues=4 victims=2 ...". Dispatch in main.cpp is exercised by RR item 4 (the FX diagnostic comes from that block). |
| 2 | Stop (Character::_Stop 0x3b56b4 = Cmd_Stop) through a generic seam, player and monster | PARTIAL | Seam: `CombatSession::set_script_stop_service` / `script_stop_actor`. main.cpp `bindEnemyAI` installs it: monsters call the navigation owner's `stop` (route erased, idle selected), the player is put to idle with `script_put_idle`. SESS: refused without an owner, dispatched to the owner with the right actor, refused for an actor outside the session. Not verified: the behaviour of a real monster or player Stop (no RR run of Stop). The player branch is an inference (the player walk is input-driven; idle is the closest source-state equivalent). |
| 3 | Half-angle question: how TargetListSearch consumes the angle, and whether Charge/ColdRay/JumpKick are off by 2x | WORKED (proven and fixed) | IDA `ObjectSearcher::TargetList::Search` (0x4a2f34) admits a target when `maxAngle >= 3.1416 || maxAngle >= angle`, with `angle` the target's angle from the look vector. TargetListSearch (0x38fcf0) computes the maxAngle for the two-argument, string-name and five-argument forms as `(ANGLE_deg * 0.017453) * 0.5`, so the cone is ±ANGLE/2 (360 gives pi, one-argument gives pi). The port passed the full cone, so Charge (ANGLE 120), JumpKick (270), ColdRay (90), ViciousStrike (180, previously unlimited) and DeathClaw (135) were about twice as wide (ViciousStrike unlimited). Fix: `native_search_max_angle_v1(deg)` helper, corrected rows and Use cones, activation and profile test pins updated, mage owner `2*pi` sentinels set to `pi` (no behaviour change). DeathClaw pre-cone was 135 degrees; the Lua pre search (prince_necromancer_deathclaw.luac:65) has no angle, so the pre cone is now the full circle. UT: `check_native_half_cone` (ANGLE 90: 40 degrees admitted, 50 rejected; ANGLE 120: 50 admitted, 70 rejected; 360 keeps all). |
| 4 | Real run of a sound+FX row: frames, log lines, sound resolves to an existing file | PARTIAL (escalate the sound part) | See the RR section below. |

### RR section (item 4)

Hidden window, staged v21.8 package, EXE built from this branch (C:/dh218/es-api-build/dh-foundation.exe, copied to the case folder).

- **DivineJudgement (Paladin, OnPre per-target FX): cast and dispatch WORKED; the FX itself is not visible.**
  Case C:/dh218/rr/dj (save from the BashDown Knight save with `subclass_save_writer_v21_8 ... KnightPlayerBase_Paladin DivineJudgement 30`; `--spawn-test Swamp_LizadMan_Type1@990,-212,258@30`; `--skill-key-frame 150:2`).
  Log (run.log): `Source skill key=2 slot=0 frame=150 ... phase=1 skill=DivineJudgement`; `Source skill lifecycle frame=150 ... phase=1`; the dispatch in main.cpp runs at phase 1 (OnPre) and logs `Skill FX diagnostic: Exact authored FX resource not found (basename fallback disabled): data/3D/interface/spell_dh2_prince_warrior_paladin_divine_judgement.bdae`; `Source skill lifecycle frame=178 phase=2 hits=1 ... Use applied`; `frame=220 phase=3 ... completed`.
  Frames: f156.png shows the cast pose (sword raised); f170.png shows the quest banner from the save over the scene. The aura FX is not visible because its bdae is missing from the staged package (known gap, phase-1 report section 3).
  Gate: the Paladin row is rejected at SnS level 9 and 20 ("character-level requirement rejected"), accepted at 30.
  Before the lizard spawn the OnPre list was empty ("Use completed with an empty target list"), which is the correct Lua behaviour and produced no cue.

- **Figments (Illusionist, PlaySound at Use): static sound resolution WORKED; the real run is BLOCKED.**
  Sound label `sfx_skill_mage_ilu_nightmare` resolves in the staged `audio-assets/data/sounds/sounds.xml` (line 127: uid 123, filename `sfx_skill_mage_ilu_nightmare.wav`), and that file exists in `audio-assets/data/sounds/`.
  Blocked: the Illusionist case (C:/dh218/rr/fig, save from the a9-coldray mage case with `MagePlayerBase_Illusionist Figments 30`) fails in combat initialization with `Foundation error: Combat initialization: Source actor property row unavailable: MagePlayerBase_Illusionist` (combat_session.cpp:1324). The binding exists (original-melee-bindings.xml line 2486, propertyRow=291), and the save writer resolves the row name, but the combat path rejects it. The cause is not identified. At SnS level 15 the same case fails the Figments level gate. Two failures on different steps; escalating to Sonnet for the propertyRow / name-index check.

## Open items for the lead

1. Behaviour change in shipped rows (item 3): Charge, JumpKick, ViciousStrike, ColdRay and DeathClaw cones are now narrower (about half the previous width; ViciousStrike was unlimited). This follows the IDA semantics, and the activation test now pins the native values. A real-run check of one of them would confirm the width in-game; none was done.
2. Stop (item 2) has no real-run evidence; monsters' route erase and the player's idle are unverified in-game.
3. Presentation: the FX set names for DivineJudgement and the Figments sound are present in the authored tables, but the DivineJudgement FX asset is not staged. The Figments row needs a working Illusionist combat start before its sound can be heard in the EXE (item 4).
4. Atlas: TargetListBackup, SetTargetListCharacterFilter (player-side query only), GetPropHP/SetProp (HP cell path), and the cone semantics should be updated.
5. Presentation dispatch counter in main.cpp is not reset when the coordinator is recreated (generations assumed monotonic).

## Files (this follow-up)

- port/windows-foundation/features/generic_skills/runtime_skill_target_query_v1.{hpp,cpp}: `native_search_max_angle_v1` (constexpr), cone comments.
- port/windows-foundation/features/generic_skills/runtime_skill_use_profile_v1.hpp: Charge, JumpKick, ViciousStrike, ColdRay Use cones via the helper.
- port/windows-foundation/features/generic_skills/runtime_skill_use_profile_magesub_v1.hpp: DeathClaw pre cone (no angle) and Use cone (135 via helper); Figments `use_sound_name = "sfx_skill_mage_ilu_nightmare"`.
- port/windows-foundation/features/generic_skills/runtime_skill_use_profile_knight_v1.hpp: DivineJudgement `pre_target_fx_name = "spell_dh2_prince_warrior_paladin_divine_judgement"`.
- port/windows-foundation/features/generic_skills/runtime_skill_cast_coordinator_v1.{hpp,cpp}: `RuntimeSkillCastRequestV1::profile_override` (test seam, null in play); admission applies it only to the same (name, script) row.
- port/windows-foundation/features/generic_skills/runtime_mage_skill_owner_v1.cpp: `2.0f * kPi` sentinels set to `kPi` (unchanged behaviour).
- port/windows-foundation/combat_session.{hpp,cpp}: `ScriptStopServiceV1`, `set_script_stop_service`, `script_stop_actor`.
- port/windows-foundation/main.cpp: `bindEnemyAI` installs the Stop service.
- port/windows-foundation/features/generic_skills/runtime_skill_activation_session_v1_tests.cpp: probe fields, JumpKick probe run, pin updates.
- port/windows-foundation/features/generic_skills/runtime_skill_use_profile_v1_tests.cpp, runtime_skill_use_profile_magesub_v1_tests.cpp: pin updates.
- port/windows-foundation/features/generic_skills/runtime_skill_actor_api_v1_tests.cpp: `check_native_half_cone`.
- port/windows-foundation/tests/combat_session_actor_transition_tests.cpp: Stop service assertions.

## Files (first ES-API commit, unchanged)

- runtime_skill_target_query_v1.{hpp,cpp} (backups, Player filter, backup query, relative origin, SkillPresentationCueV1); runtime_skill_use_profile_v1.hpp (presentation fields); runtime_skill_cast_coordinator_v1.{hpp,cpp} (`presentation_cues`); main.cpp (cue dispatch block); playable_actor_world.{hpp,cpp} (set_current_health, current_health); combat_session.{hpp,cpp} (script_kill_actor with killer, script_kill_records); faery_menu/faery_ability_registry_v1.{hpp,cpp} (equipped_faery_element_v1); runtime_skill_actor_api_v1_tests.cpp.
