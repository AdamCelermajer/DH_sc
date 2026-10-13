# v21.8 magefix report (Mage act-1 blockers B161-B162)

Branch `v21.8-magefix`, worktree `.claude/worktrees/magefix`, base `windows-foundation` 9fd592b1. Subst `G:` (released). Build dir `C:/dh218/magefix-build` (Release, llvm-mingw). Harness: `C:/dh218/tools/run-act1.ps1` with `-Pkg` pointing at the new package. Cases: `C:/dh218/cases/magefix-pkg-boss1`, `magefix-pkg-boss2`.

## Verdicts

| Item | Verdict | One-line reason |
|---|---|---|
| 1. B161 Mud Lord package gap | **WORKED** | 10 files added to the selection; packaged run binds the boss AI with no overlay and casts spits. |
| 2. Staff bolt contact at caster, untargeted bolts expire in place | **DID NOT WORK** (root causes found, no fix) | Spawn origin and aim are not the original's; velocity units unresolved. See item 2. |
| 3. Potion heals full HP | **WORKED** (port is right, no change) | Original `Ctrl_UsePotion` also restores to max (`RegenHP(-1)`). |
| 4. Hero does not move after Mud Lord outro | **DID NOT WORK** (not isolated) | Lock/unlock counts balance; the lock that stays set is not identified. |

## Item 1: Mud Lord package (WORKED)

- Investigated: the 10 files listed in `coordination/v21.7/B155-B156-HANDOFF.md` section 8 exist in `.local-inputs/assets-extra/android/data`. `swampking_trap.luac` and `go_trap_swampking_steam_cloud.bdae` are already in the v21.7 base, so they are not added. The 10 are: `scripts/ai/swampking_core.luac`, `scripts/skills/swamp_king_attack_1..4.luac`, `scripts/skills/swampking_atk_spit.luac`, `3d/projectiles/elemental_bolt_water.bdae`, `3d/interface/fx_swampking_blue_shockwave.bdae`, `3d/interface/fx_swampking_attack_03_poisonspit.bdae`, `3d/interface/spell_dh2_hurt_steam.bdae`.
- Change: `coordination/v21.8/asset-selection-v21.8.json` gets group `mud_lord_boss_ai_fx` (commit e3aae476).
- Staging: python is not on PATH in Git Bash; run as `/c/Windows/py.exe -3 -I coordination/v21.8/stage_assets_v21_8.py --inputs <primary>/.local-inputs --base windows-source-clock-v21.7 --selection coordination/v21.8/asset-selection-v21.8.json --folder windows-source-clock-v21.8-staged-magefix`. Result: 4561 manifest files, 132 added (10 of them Mud Lord, `mud_lord_boss_ai_fx`). Folder: `.local-inputs/windows-source-clock-v21.8-staged-magefix`.
- Real-run evidence (magefix build, packaged assets, no overlay, Mage save from seed `mage-witch-done`):
  - `magefix-pkg-boss2/run.log`: `[boss-ai] profile=SwampKing swampking_core owner bound ai=swampking_core ... skills=0:SwampKingAttack1/swamp_king_attack_1 ... 2:SwampKingAttack3/swampking_atk_spit`, then `DoSkill(2) ... SwampKingAttack3` (3 times) and `projectile spawn ... row=SwampkingSpit` (3 times). `Original resource not found` count: 0.
  - Frame `magefix-pkg-boss2/f3000.png` (viewed): Mud Lord head at the top, a white-blue AOE on the Mage, HUD with 3 potions.
  - `magefix-pkg-boss1` (1600 frames, same package): `swampking_core owner bound` at the serpent intro; the run ends before any attack, so the attack proof is `boss2`. The old package (`windows-source-clock-v21.8-staged-magewire`) printed `swampking_core owner unavailable` in the earlier act-1 runs.

## Item 2: staff bolt contact and untargeted bolts (DID NOT WORK, no fix)

Original (IDA export `.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/003e/003e58cc.c`, `Projectile::SetInfo`):
- Spawn point: if the visual has `projectile_node`, the bolt starts at that node's absolute position; otherwise at the owner's target position (`GetTargetPosition(owner)`).
- Aim: if an explicit target GameObject is given, destination = target position. Otherwise the look vector of the owner, rotated by the angle argument; with a node and the bool flag, the direction is node minus owner target position (normalized). Destination = origin + dir*1000.
- Physics body: `PhysicalObject(world, this, 0, 1,1,1, 0, 0x20, 0x51F)`; its shape comes from the projectile GameObject bounds (`GameObject` fields 75-79 times 0.01). Category 0x20 and mask 0x51F match our port.

Our port:
- `main.cpp` `launchPlayerRangedProjectile` (around line 3740): origin = owner `transform.position` (caster), never the `projectile_node`. The header `runtime_session_projectile_v1.hpp` says origin "is the live projectile_node when present", so this is a documented but unimplemented gap.
- `runtime_session_projectile_v1.cpp` `launch_state5` (around line 290-360): direction = owner heading only; no target steering. (The skill launch has target steering.)
- Sensor radius 162.021 game units is the AABB half-extent of `elemental_bolt_fire.bdae` (logged as `radiusGameUnits`). The original also sizes from GameObject bounds, so 162 may be faithful. The 176-unit contact equals 162 plus about 14 for a target body (inference, not measured).
- Velocity units (not resolved): `projectile.physics_velocity = speed * forward` (around line 637) has no 0.01 factor, but the no-body path (around line 618) multiplies `physics_velocity` by 100, which treats it as Box2D units. The two paths disagree. The original velocity scale lives in GameObject speed code that is not decoded here. Bolts that "expire in place" at one point fit a velocity or position-publication problem, but this is not confirmed.

Why no fix: the origin fix needs the live `projectile_node` absolute position from the player's visual at launch, and the velocity question needs the original GameObject speed code. Neither was completed. Next steps: (1) read the player's visual `projectile_node` at the state-5 marker and pass it as origin; (2) decode the original velocity scale and make the Box2D unit consistent; (3) re-run `a1-s1-swamp`-style case and check `Mage projectile impact pos` against the target distance.

## Item 3: potion heal (WORKED, port is right)

- Original: `Character::Ctrl_UsePotion` (`pseudocode/003a/003ad6f0.c`) calls `Character::RegenHP(this, -1)` then `RegenMP(this, -1)`. `Character::RegenHP(int)` (`pseudocode/003b/003bdca4.c`): when `a2 < 0` it sets `a2 = max HP`, so the heal is to full. No item-table heal amount is read.
- Port: `gameplay_hud.cpp` `gameplay_use_potion` calls `dh2_character_skill_regen_v6(props, 0, -1)` and `(props, 1, -1)`, the same -1 (full) convention used by the port's death-reward path.
- Conclusion: full restore for every character (same code path for Knight and Mage). No code change, so Knight behaviour is unchanged. The four observed full restores match the original.

## Item 4: post-outro movement lock (DID NOT WORK, not isolated)

- Evidence from `C:/dh218/cases/b10-darkwood/run.log` (Mage, packaged-era run with the overlay): `Script_LockCharacter` 5 times, `Script_UnlockCharacter` 5 times (balanced). `EndScriptedCutScene` unlock and `Script_ExitCutSceneMode` at frame 5967. Then `[campaign] frame=5970 player position=-18462.8,11536.2,-112.5` and no movement after `--player-to` (mage report blocker 3).
- STATE_TRACE after the outro shows `cinematic=0`, `focus=1`, `modal=0`, `menu=0` and recorded move input (`semantic=1,...` at frame 6874). So the lock is not the global cinematic or modal flag. The `hero=` field in the trace is the invalid id on every line (including frame 0), so it is not evidence.
- Not identified: which per-actor or controller flag stays set. Candidates to check next: `Swamp_Serpent_Defeated` commands `Script_UnEquipHands` and `Script_SetActorPosition` (frame 4500 onward) and whether the hero's controllable binding or the `Limbus` state is restored after the outro.

## Tests

- Full CTest on `C:/dh218/magefix-build`: run 1 had one failure (`schema_v4`, passed alone on rerun); run 2 `100% tests passed, 0 tests failed out of 202` (log `C:/dh218/magefix-ctest2.log`). The build has 202 tests; the lead's baseline said 203, so the count does not match and this is noted.
- The worktree needed junctions to the primary `.local-inputs` directories for tests that read `G:/../../.local-inputs/...`. They were created with PowerShell `New-Item -ItemType Junction` and removed with `cmd /c rmdir`; the primary folder was not touched.
- No code changed in this branch except the JSON selection; the unit suite is unchanged.

## Commits

- e3aae476: item 1 selection change (`mud_lord_boss_ai_fx`).
- Report and tracker commit: see the final message of the agent.

## Tracker note

The rows B161-B163 exist in the primary checkout's `docs/BUGS-AND-IMPLEMENTATION.md` (lines 341-343, uncommitted there) but not in the committed file on this branch, so the worktree tracker was not edited. Suggested status for the lead: B161 FIXED (e3aae476, item 1); B162 PARTIAL (item 2 DID NOT WORK, item 3 WORKED as correct, item 4 DID NOT WORK); B163 unchanged.
