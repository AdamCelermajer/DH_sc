# B155 (Mud Lord boss fight) and B156 (witch abilities) report

Branch `v21.7-mudlord`, from windows-foundation bc642f73. Code commit 497d9b3e, handoff b554b958. Not pushed.

## B156: witch abilities (investigation only, no code change)
Nothing is missing on the witch.
- `witch.luac` uses only DoSkill(0) and a teleport every 10 s. SkillTree 35 is [122] only (`witch_ranged_00`, projectile 39 bogwitch, ImpactFX 76 `spell_dh2_hurt_bogwitch`).
- Video 966-1000 s: the green burst on the hero is WitchImpactFX and the white double flashes are the teleport FX. Both were ported in B154.
- The orange blast with the red crackle (about 984.1 s) and the blue lightning (about 985.5 s) land on the witch, and the hotbar icons grey out at the same moment. They are player-side effects (hero skill / faery), not witch abilities.
- `rene.luac` has no heal (only Attack() and the Buff_Rene_Haste catch-up). The hero's HP gains match potion use (counter 7 to 6).
- Rene's ranged attack is the separate known gap B077.

## B155: Mud Lord (SwampKing) fight
### Root causes found
1. The intro cutscene aborted at PutCharacterInLimbus ("Unknown original lifecycle actor") after the arrival world restore, because the idle/Limbus lifecycle records were dropped by the restore.
2. The boss was never admitted as a gameplay actor (script-idled AI type 4), so it had no nameplate, targeting, damage or AI.
3. `swampking_core.luac` AI, its skills, the steam-cloud TimerTrap and the projectile FX had no port.
4. The empty authored clip `swampking_hurt_01` (INT32_MAX..INT32_MIN sentinel) was rejected, and the nested runtime transition in `select_actor_state_leaf` caused "unfinished reached prefix" when the outro posed the dead boss.

### Changes
See section 3 of `B155-B156-HANDOFF.md` for the file-by-file list. In short: lifecycle idle baseline survives `detach_for_restore`; `campaign_limbus_v1` save component; boss admission, targeting and AI owner binding in `main.cpp`; new `features/bosses/runtime_swampking_core_ai_v1.*` and `runtime_timer_trap_v1.*`; boss plan additions (`CloseAttackRulesV1`, `ranged_attack`, `phase_after_update`); empty-clip admission; `saveLevelWorld` persists the campaign lifecycle before capture.

### Tests
- New CTest `runtime_swampking_core_ai_v1` (parse, derive, timers, decisions); new checks in `original_actor_lifecycle_tests` and `animation_markers_tests`.
- Full CTest result: see the final section of this file.

### Runtime-verified (real hidden-window runs, real keys)
Sealed door and lever; the intro to its end; nameplate, Tab/Space targeting, damage and hurt reaction; spotting, bites, spit (AOE 250), steam-cloud mist (6 activations over about 7.6 s, matching video 19:17-19:24), phase 2 at about 73% HP, weak at about 23%, dive and emerge at spawn points 03/04/01 with the boss untargetable while submerged; death outro with the Swamp_Serpent_Defeated dialogue and SaveGame; quest Swamp_Escape completes with the banner "QUEST COMPLETED / Escape the swamp with Rene and Celeste / Reward 100 EXP" (matches video 20:48); the exit prompt "Gothicus Darkwood" appears and E starts the transition.
Evidence sheets: `B155-evidence/`.

### Unit-only / not shown
- A kill from one continuous run (afterwitch seed to lever to intro to fight): the scripted hero could not follow the relocated boss, so the boss was not killed in 28 s. The fight was killed from the bossroom4 seed instead.

### Known gaps
- Darkwood load fails after the exit: "Level::_LoadFromXML service: required original LoadTemplate assertion(9) provider". Not checked against the base build.
- TimerTrap damage is not applied (the `_colzone` point-cloud bounds are undecoded), so the mist is visual only.
- MarkAsFlying is not modelled; the boss dynamic level (5-6) is not applied, so damage is uncalibrated (bites/spit 46-67 against 169 HP); no AOE pushback; the awake latch approximates culling.
- `SetIsTargetable` also blocks damage in the port; in the original it only affects player targeting.
- The projectile expiry delayed callback is not called; the shockwave sound name includes ".wav".

### Package: original files to add (for v21.8)
From `.local-inputs/assets-extra/android/data/` (hashes printed by `C:/dh217/tools/make-overlay.ps1`):
- scripts/ai/swampking_core.luac
- scripts/skills/swamp_king_attack_1.luac, swamp_king_attack_2.luac, swamp_king_attack_4.luac, swampking_atk_spit.luac (and attack_3 if referenced)
- scripts/objects/swampking_trap.luac
- 3D/projectiles/elemental_bolt_water.bdae
- 3D/interface/fx_swampking_blue_shockwave.bdae, fx_swampking_attack_03_poisonspit.bdae, spell_dh2_hurt_steam.bdae
- 3D/GameObjects/go_trap_swampking_steam_cloud.bdae (if not already in the package)

## Full CTest result
183/183 passed (`ctest -j4`, Release, llvm-mingw; log `ctest-final.log`). The first run showed 14 failures that were environmental: the worktree's `.local-inputs` lacked 16 directories the tests read (e.g. `windows-melee-bindings`). After adding them as junctions, all 183 pass.
