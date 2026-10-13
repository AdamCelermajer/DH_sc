# B155 (Mud Lord boss fight) and B156 (witch abilities) report

Branch `v21.7-mudlord`, from windows-foundation bc642f73. First code commit 497d9b3e, handoff b554b958, first report df0111eb; the follow-up
commit (A/B/review fixes below) is the branch head. Not pushed.

## B156: witch abilities (investigation only, no code change)
Nothing is missing on the witch.
- `witch.luac` uses only DoSkill(0) and a teleport every 10 s. SkillTree 35 is [122] only (`witch_ranged_00`, projectile 39 bogwitch, ImpactFX 76 `spell_dh2_hurt_bogwitch`).
- Video 966-1000 s: the green burst on the hero is WitchImpactFX and the white double flashes are the teleport FX. Both were ported in B154.
- The orange blast with the red crackle (about 984.1 s) and the blue lightning (about 985.5 s) land on the witch, and the hotbar icons grey out at the same moment. They are player-side effects (hero skill / faery), not witch abilities.
- `rene.luac` has no heal (only Attack() and the Buff_Rene_Haste catch-up). The hero's HP gains match potion use (counter 7 to 6).
- Rene's ranged attack is the separate known gap B077.

## B155: Mud Lord (SwampKing) fight
### Root causes found (first pass)
1. The intro cutscene aborted at PutCharacterInLimbus ("Unknown original lifecycle actor") after the arrival world restore, because the idle/Limbus lifecycle records were dropped by the restore.
2. The boss was never admitted as a gameplay actor (script-idled AI type 4), so it had no nameplate, targeting, damage or AI.
3. `swampking_core.luac` AI, its skills, the steam-cloud TimerTrap and the projectile FX had no port.
4. The empty authored clip `swampking_hurt_01` (INT32_MAX..INT32_MIN sentinel) was rejected, and the nested runtime transition in `select_actor_state_leaf` caused "unfinished reached prefix" when the outro posed the dead boss.

### Changes (first pass)
See section 3 of `B155-B156-HANDOFF.md` for the file-by-file list. In short: lifecycle idle baseline survives `detach_for_restore`; `campaign_limbus_v1` save component; boss admission, targeting and AI owner binding in `main.cpp`; new `features/bosses/runtime_swampking_core_ai_v1.*` and `runtime_timer_trap_v1.*`; boss plan additions (`CloseAttackRulesV1`, `ranged_attack`, `phase_after_update`); empty-clip admission; `saveLevelWorld` persists the campaign lifecycle before capture.

## Follow-up pass (items A, B, review fixes)

### A1. Dynamic level (WORKED, runtime-verified)
- Original: `swampking_OnInit` (swampking_core.luac, identical to `monster_OnInit`) runs the dynamic-level block: `m_lvl = GetHostPlayerLevel()` clamped to `GetCurrentLevelRange(difficulty)` when its second value is > -1 (LuaScript::_GetCurrentLevelRange 0x37f1f0: LevelList row `LevelRangeRow24`, row words 12..14 = maximum[difficulty], 15..17 = minimum[difficulty]; -1,-1 without a row), otherwise to the character row's own LevelMin/LevelMax; plus LevelOffset; then `SetLevel(ToFixed(m_lvl))` = Character::_SetLevel 0x3b73a4 (cap MaxLevelDVeryHard, property 19, RecalcProperties, RegenHP/MP).
- Finding: the real LevelList row of `001_swamp.mlx` (row 41) has range 1..4 on Normal (40..41 hard, 70..71 very hard), so the current-range branch wins over the CharacterTable row's LevelMin/Max 5-6 (which the first-pass note assumed). The hero in the test seed is level 2, so the Mud Lord becomes level 2 (HP 700 -> 900, attack rating 124 -> 148). A stronger hero clamps to level 4.
- Port: `spawn::current_level_range_v1` and a `spawn_level_raw_v1` overload taking the range (`features/spawn/spawn_character_v1.*`), applied to the boss policy in `main.cpp` (`applySourceDynamicLevel`, logs `[boss-ai] dynamic level ...`). Only the swampking_core boss uses it (see gaps for ordinary placed monsters).
- Runtime: `after-n.exe`, bossroom4 seed, log `[boss-ai] dynamic level profile=SwampKing host_level=2 current_range=1..4 level_raw=512`; the fight, dives, emerges, death reward and outro still complete.
- Damage figures: the skill rolls come from the SkillTable/ClassTable rows (SwampKingSkill30PCT etc.) and do not drop with this level: bites/spit stayed 47-68 against 169 HP at level 2 (54, 67, 61, 52). The video gives no figure for damage taken (the HP bar never moved in 19:14-19:28, and the bite at 18:56 shows a "Miss"), so no lower value can be claimed; these numbers are the formula output at the original level, not a calibration to a target.

### A2. Steam-cloud TimerTrap damage (investigated; NOT applied, on purpose)
- Logic (IDA): `TriggerTrap::Update` 0x39eedc damages each queued victim through the trap script `OnDamageRoll -> DealDamages(victim, GameObjectDamager Level01Damage)` (`_DealDamages` 0x3906fc -> `Character::F_GOAttack` 0x3b061c: rand(min..max)<<8 = 5..10 HP, no mitigation, immunity slot -1) once per activation per victim (set +992 cleared on Activate and at the end of the activate clip, `TriggerTrap::__Callback` 0x39e738); victims are the occupants of the zone (`TransferVictims` 0x39f168, `CanTrigger` -> `OnTrigger` returns true).
- Zone: `Zone::InitPost` 0x39771c takes the model node `_colzone` (here `_colzone_GL_pcloud`) and builds a CTriangleSelector from its mesh when exactly one mesh node is found; `Zone::IsInside` 0x3970c8 tests a vertical +-100 segment through the victim against that selector. Decoded from the BRES: the node is a glitch particle emitter (SEmitter: type 0 box 5 x 350 x 350, particle type 3 = billboards, 30/s) and `createParticleSystem` 0x634c64 attaches a mesh only for types 1/2/7 and geometry references, so this emitter has no mesh. Inference (not executed in the original): the selector is empty and IsInside never reports a victim.
- Visual evidence (direct): ffmpeg crops of the HP bar over video 19:14-19:28 (4 fps, 56 frames) and a 1 fps pass over 16:40-19:10, red bar pixel extent 71 in every frame, never lower; contact sheet `B155-evidence/video-mist-sheet-19-16.jpg` and strip `B155-evidence/video-mist-hp-bar-19-16-to-19-26.png`. The hero stands inside the mist for all six activations with an unchanged HP bar; a 5..10 HP roll per activation would have been visible.
- Result: the mist stays visual-only; the header of `runtime_timer_trap_v1.hpp` records the decode. New checks (`runtime_swampking_core_ai_v1`): real TimerTraps/GameObjectDamager rows (Delay 1000, Level01Damage 5..10), `swampking_trap.luac` facts, model clips and the 6-activation lifetime.

### A3. SetIsTargetable no longer blocks damage (WORKED)
- Original: Character::_SetIsTargetable 0x3b7a64 only clears Character+1045, read by IsInteractive (target selection).
- Port: `PlayableActorWorld::eligible_target` (selection) honours a new per-actor `selection_suppressed` set; `damage_eligible` (used by `resolve_source_result`) does not. The boss AI sets it every update (`RuntimeSwampkingCoreAiV1::update`); the Session permission provider no longer hides a submerged boss. New CTest `playable_world_selection_suppression`.
- Runtime: run k3 (after-n.exe): `SetIsTargetable(false)` at each dive and `true` after the emerge, repeated dive/emerge cycles, no error.

### A4. AOE pushback (NOT applied; evidence recorded)
`swamp_king_attack_4.luac` rolls class `SwampKingSkillAOEPushback`; its SkillTable flags add bit 0x100000 (great knockback, read by `Character::F_ApplyResult` 0x3b10b4 for `SM_SetKnockBackState`) and the class sheet has SnS_Push_Target_Chance 158 (bites 108). The combat result already rolls the push chance, but the Session does not turn outcome bit 0x80 into the knockback state for any actor (a generic gap, not boss-specific), and the Attack4 rolls in k2/k3 gave outcome 0. Not implemented, to avoid a boss-only special case; the flag/class values are asserted in the test.

### B. Gothicus -> Darkwood (WORKED, runtime-verified)
- Pre-existing: `before.exe` started directly on `data/scene/003_darkwood.mlx` fails with the same `Level::_LoadFromXML service: required original LoadTemplate assertion(9) provider` (case dwB). The boss seed cannot be used with before.exe (it holds a boss admitted as a gameplay actor, which the old build rejects on restore).
- Original: `PropertyMap::LoadTemplate` 0x51419c is only `ASSERT(false)` (PropertyMapTemplates.cpp:9): `gAssertLevel == 2` -> NULL store, `== 1` -> fprintf; `gAssertLevel` (0x99f914, .bss) is never written in the library (no store in the export), so the call returns without loading any file. Fix: `source_root_scopes.cpp` installs `load_template_assertion` as that no-op.
- Second failure behind it: module `_module_08_001` -> "Source Module requires explicit unique declaration occurrence association ... matches 0 declaration hosts". Cause: `Darkwood_08_01.mgp/.mvp` are an empty `<Module/>`, so no child GameObject carries the host key. Fix: `module_child_occurrences()` (`actor_definitions.*`) gives the deterministic key `<parent>/<name>#<module ordinal>` for every Module declaration; used as the fallback host in `source_root_scopes.cpp` and registered in `SourceWorldObjects::load` so `bind_module` accepts it.
- Runtime: direct level start (dwA, after-m.exe): exit code 0, hero and HUD in the Darkwood forest. Real transition (bdC, bossroom4 seed, `--condition-active IsAfter_Swamp_Escape`, hero put at the exit, real key E): `Transition confirmed key=E ... DARKWOOD frame=378`, `Level world saved reason=transition`, `Session start ... level=data/scene/003_darkwood.mlx`, 1000+ frames in Darkwood without a Foundation error; screenshot `B155-evidence/darkwood-after-transition-f1450.png` (hero with Celeste and Rene, "The Boglands [E]" return prompt). CTest `source_root_scopes` now also builds the Darkwood root scopes.

### Review fixes (independent review of v21.7-mudlord)
1. Boss AI fatal-error risk: `play_anim` asked the Session for a CSAnim14 departure, which the Runtime handler rejects (only Idle3/Dead12). It now departs to Idle3 and `play_actor_clip`'s own source_program transition enters CSAnim14; `dive_state` drops the call for a dead boss. New checks in `session_actor_transition_v1` (direct 14 rejected, PlayAnim before departure rejected, Idle3 departure then PlayAnim -> state 14). Fixed.
2. `scriptIdledMonster` is restricted to rows that have a recovered AI owner (`bossDefinition(id)`, the swampking_core shape); other placed Monster rows named by PutCharacterInIdle/KillActor (Darkwood Dark Queen) keep the prop admission; a missing derivation table is logged and not fatal. Fixed.
3. Restore re-registration: `OriginalActorLifecycle::idle_preset_restorable` limits it to Limbus records, live Idle3 actors whose body is present, or bodiless presentation props (the intro lizards; needed so PutCharacterInLimbus finds them). Dead/Spawn/PreSpawn actors and bodiless targetable actors keep no record; skipped actors are logged. Unit-tested in `original_actor_lifecycle`. Verified in the afterwitch -> lever -> intro run (fulln): 4 Idle records restored (3 lizards + boss), 0 "Unknown original lifecycle actor". Fixed. (My first narrowing used `alive()` and broke the intro; the real run caught it, it now uses `is_dead()`.)
5. `RuntimeSwampkingCoreAiV1::update` sets `targetability_changed_` on a lease change. Fixed (the targetability itself no longer goes through the permission).
6. The Limbus guard on activate_cond now applies only to Idle-preset lifecycle actors that are in Limbus (the boss-room lizards), not to preset PreSpawn/Limbus actors; CSLimbus::OnFocus 0x3c2e58 / TestEnableCondition 0x33e6d4 are cited in the comment. Fixed (the original-behaviour justification is partly inferential: SetEnable and flags328 are different owners).
9. `campaign_limbus_v1` is written only when a live Limbus actor exists (or to overwrite a stale component), and logged only when the set changes. New test in `campaign_host`. Fixed.
4, 7, 8, 10 (documentation-only findings): their text was not part of the message that reached this session; the corresponding facts that I know of are listed under Known gaps.

### Non-boss smoke test
Real hidden-window run, afterwitch seed Continue in `002_swamp_witchcave`, real key E at the exit: `Transition leave ... target=SWAMP`, `Session start ... 001_swamp.mlx`, `Transition character loaded`, no error. The same milestones appear for before.exe (bdB) and after-m.exe (bdD).

### Tests
- New CTests: `playable_world_selection_suppression`. Extended: `runtime_swampking_core_ai_v1` (trap tables/script/model/lifetime; level, knockback-flag, push-chance and HP facts), `spawn_character_v1` (Mud Lord level, level-range word order), `menu_metadata_v1` (real swamp range 1..4), `session_actor_transition_v1`, `original_actor_lifecycle`, `campaign_host`, `source_root_scopes` (Darkwood).
- Full CTest result: see the final section.

### Runtime-verified (real hidden-window runs, real keys)
First pass: sealed door and lever; the intro to its end; nameplate, Tab/Space targeting, damage and hurt reaction; spotting, bites, spit (AOE 250), steam-cloud mist (6 activations over about 7.6 s, matching video 19:17-19:24), phase 2 at about 73% HP, weak at about 23%, dive and emerge at spawn points 03/04/01 with the boss untargetable while submerged; death outro with the Swamp_Serpent_Defeated dialogue and SaveGame; quest Swamp_Escape completes with the banner "QUEST COMPLETED / Escape the swamp with Rene and Celeste / Reward 100 EXP" (matches video 20:48); the exit prompt "Gothicus Darkwood" appears and E starts the transition.
Follow-up (after-n.exe unless stated): dynamic level (k1/k2/k3), full dive/emerge cycles with the new targetability (k3), boss death reward (k2, frame 907), intro without abort (fulln), Darkwood direct load (dwA, after-m.exe) and real transition (bdC, after-m.exe), non-boss transition (bdD, after-m.exe).

### Unit-only / not shown
- A kill from one continuous run (afterwitch seed to lever to intro to fight): the scripted hero could not follow the relocated boss, so the boss was not killed in 28 s. The fight was killed from the bossroom4 seed instead.
- Darkwood was verified to load and run about 1000 frames with the hero; no Darkwood gameplay (Dark Queen intro etc.) was exercised.
- The `play_anim` cast-cancel path is covered by the Session unit test, not by a runtime case (no runtime sample overlapped a cast with a dive).

### Known gaps
- TimerTrap damage not applied (A2: the original appears to deal none for this emitter; the empty-selector step is an inference).
- AOE pushback / generic outcome-0x80 knockback not applied (A4).
- Ordinary placed monsters (monster.luac OnInit dynamic level) are not level-scaled in the port outside the spawn pool; only the boss now is. Applying the same block to every placed Monster would change other levels' difficulty and is left to a separate item.
- MarkAsFlying is not modelled (the boss motionRoot is cleared); the awake latch approximates culling; engine timers are not modelled.
- The projectile expiry delayed callback is not called; the shockwave sound name includes ".wav".
- Darkwood: modules with an empty mgp/mvp use the synthetic ordinal host key; the Dark Queen / HG2 boss intro AI has no owner (the actor keeps the prop admission).

### Package: original files to add (for v21.8)
From `.local-inputs/assets-extra/android/data/` (hashes printed by `C:/dh217/tools/make-overlay.ps1`):
- scripts/ai/swampking_core.luac
- scripts/skills/swamp_king_attack_1.luac, swamp_king_attack_2.luac, swamp_king_attack_4.luac, swampking_atk_spit.luac (and attack_3 if referenced)
- scripts/objects/swampking_trap.luac
- 3D/projectiles/elemental_bolt_water.bdae
- 3D/interface/fx_swampking_blue_shockwave.bdae, fx_swampking_attack_03_poisonspit.bdae, spell_dh2_hurt_steam.bdae
- 3D/GameObjects/go_trap_swampking_steam_cloud.bdae (if not already in the package)

## Full CTest result
184/184 passed (`ctest -j4`, Release, llvm-mingw; log `C:/dh217/ctest-final3.log`, build of commit parent + follow-up changes). Baseline was 183; the new test is `playable_world_selection_suppression`.
