# v21.8 magewire report (Mage base class: staff contact, skill external owner, ColdRay, package, Tab)

Branch `v21.8-magewire`, worktree `.claude/worktrees/magewire`, subst `W:`, build dir `C:/dh218/magewire-build`.
Base: `windows-foundation` @ 0eea5e65 (CTest 199). Runs: `C:/dh218/cases/mw-*` (exe `C:/dh218/magewire2.exe`, package `windows-source-clock-v21.8-staged-magewire`).

## Premise check (read first)

The brief said the staff bolt's enemy-query / owner-hit / hit-semantics providers return errors. That was true for the
pre-merge `v21.8-mage` WIP only. On `windows-foundation` they are bound in `main.cpp` (`services.source_is_enemy`,
`source_owner_on_projectile_hit`, `source_hit_semantics` in `launchPlayerRangedProjectile`) and the NativeWorld sensor
is registered per bolt (`RuntimeSessionProjectileContactBodiesV1`). Item A was therefore verified, not built.

## Per item

| Item | Result | Evidence |
|---|---|---|
| A. Staff basic attack contact, damage, impact FX, removal | **WORKED** (impact sound: none in original) | Real run `mw-staff1` (no `--projectile-source-assets`): `launch accepted id=1 row=20 model=elemental_bolt_fire.bdae`, `sensor registered radiusGameUnits=162.021`, `impact id=1 kind=4 target=<lizard> fx=241 damage=23.2031 applied=1`, `Mage projectile impact FX set=241 created=...`, `Damage ... marker=do_skill removed=23.2031`. Second bolt (id=2) `damage=19.84`. kind=4 = bolt consumed. Unit: `runtime_session_projectile_v1` PASS (same-Session launch, contact/hit parity), `combat_session_ranged_attack` PASS. |
| B. Skill external owner (ChainLightning, Cyclone, StoneEscape) wired in main.cpp | **WORKED for ChainLightning; PARTIAL for Cyclone and StoneEscape** (gaps below) | `RuntimeMageSkillOwnerV1` is constructed lazily at the PC skill-key cast site and passed as `request.external_owner`; `advance()` runs per unpaused frame; reset at session switches. Unit: `mage_skill_owner_v1` PASS (handles; explicit no-executor errors; StoneEscape Pre/Use/statue/blast on a real session). |
| C. ColdRay verified in a real run | **WORKED** on the final package | Real run `mw-cold1`: `Audio step source sequence=413 sound=228 status=1`, `Source player step FX ... dispatched=1 sets=150`, `Source skill lifecycle phase=2 hits=1`, `Damage ... removed=7.27344`, `Source skill target FX name=elem_dmg_02_water set=125 created=...`. Frame `mw-cold1/f176.png` shows the ice ray hitting the enemy. |
| Package gap (`--projectile-source-assets`) | **WORKED** | `coordination/v21.8/asset-selection-v21.8.json` (+ `skill_fx_extra`: `skill_dh2_prince_mage_stone_escape_idle.bdae`, which was missing from the staged package). Generated with `py -3 -I coordination/v21.8/stage_assets_v21_8.py --folder windows-source-clock-v21.8-staged-magewire` (base v21.7; 122 added files incl. the 39 PyData files, `elemental_bolt_fire.bdae`, both skill projectile BDAEs). All `mw-*` runs use it with no extra projectile flag. |
| I008 Tab target reliability | **WORKED** under the gameplay-anchored harness (harness fix, no code change) | `Source target command frame=78|79 ... selected=<lizard>` in 4 of 4 runs of this session (`mw-chain4`, `mw-stone2`, `mw-cyclone1`, `mw-cold1`); 3 of 3 earlier (`tab-b1..3`). Cause (per `mage-report.md`): the injector clock started at window creation; now anchored to the gameplay-frame sync file `S1.sync`. |

## Item B detail

- ChainLightning (`mw-chain4`): cast admitted (`phase=1`, MP 78.5 -> 24.5); Use: `Plain sound submitted name=sfx_skill_mage_thunder_braid`, `projectile launched id=1 row=30`, sensor registered; contact: `hit victim=<lizard> n=1 damage=47.5 died=1`; jump: `sfx_skill_mage_thunder_braid_add_1`, second projectile `id=2`, `hit victim n=2 scale=0.5 damage=32.03`; impact FX `set=222 created`. Frame `mw-chain4/f176.png` shows the bolt on the enemy.
- Cyclone (`mw-cyclone1`): tornado `id=1 areaRange=300`, sensor registered, animation sound `sound=537` dispatched (sequence 414), FX `sets=151 dispatched=1`; victims rolled. **Gap:** our class projection has `proximityDirectDamage=0`, so the rolled damage is 0. The stun/slow tails are not reproduced (the owner's own comment; inference from the class contents, not re-read in the Lua this session). Victim carry uses the actor teleport verb (inference).
- StoneEscape (`mw-stone2`): Pre FX `spell_dh2_prince_mage_stone_escape_magefx`; animation sound `sound=526` (sequence 423); warp `860 -> 210` = 650 along facing, teleported through `hostServices.actor_verbs.teleport`; statue idle FX (`skill_dh2_prince_mage_stone_escape_idle`, now staged) played at the old position; after 2200 ms `explosion targets=1` and `blast hit target=<lizard> damage=9.72`. The death FX call returned success (no diagnostic line; visual not checked). Frame `mw-stone2/f200.png` shows the glow FX at the caster and at the old position.
- Open defect candidate (not fixed): a second `do_skill` marker in the same cast (frames 171 and 195 in both `mw-stone2` and `mw-chain4`) re-runs the owner's Use: StoneEscape warped twice (860 -> 210 -> -440) and created two statues. The coordinator's "volley" rule (`apply_retained_use_event_v1`, written for ViciousStrike) allows it. Whether the original StoneEscape/ChainLightning clip carries two `do_skill` markers that both call OnSkill_ is NOT verified (needs the clip event list). ColdRay shows the same double marker in `mw-cold1` (two rolls: 7.27 and 7.13) while the earlier `cold8` run showed one roll; not explained.
- Gaps (explicit): no floor-clamped TeleportTool trace (StoneEscape uses the full 650 units); no Dummy statue actor (FX stand-in); ChainLightning victim LOCK and Cyclone LOCK/UNLOCK not reproduced; chain victims compared by identity (the Lua compares names).

## Item A detail (sound and original evidence)

- ProjectileTable row 20 (FireWandProjectile), probed with `C:/dh218/probe/prow.exe`: `model=data/3D/projectiles/elemental_bolt_fire.bdae speed=20 timer=1000 impact fx=241 impact sound=-1 expire fx/snd=-1/-1 magic=1`. So the original authors no impact sound for the staff bolt; the only sound is the animation's. Impact FX 241 = `spell_dh2_hurt_fire`.
- Original contact logic (inherited notes, `port/windows-foundation/features/effects/runtime-session-projectile-native-contact-evidence.md`): `_HandleProjectile` 0x3cc280 (owner `OnProjectileHit`, then `F_MeleeAttack`/`F_ApplyResult`), `Projectile::OnCollision` 0x3e55b4 (AI_IsEnemy gate), `Projectile::SetInfo` 0x3e58cc (bullet sensor, category 32). I did not re-run live IDA/rea in this session; these addresses come from the existing note.
- Gaps: the shot origin is the actor position, not the live projectile node; the first bolt of `mw-staff1` had a 64.8 degree aim error and still hit by sensor overlap (radius 162).

## Tests

- New unit test: `mage_skill_owner_v1` (`features/generic_skills/runtime_mage_skill_owner_v1_tests.cpp`, registered in CMake). Covers handles(), explicit no-executor errors, and StoneEscape on a real session. Inputs the test sets (damage range, mask 0x20000, category/element 0) are test inputs, not claims about original values.
- Changed: `runtime_session_projectile_render_v1` now enrolls one source per model URI (elemental_bolt_fire mandatory; chain lightning and tornado optional). New check: skill AABBs resolve from the staged package (`runtime_session_projectile_render_v1_skill_models`, registered only when the staged package exists).
- CTest (full, `C:/dh218/magewire-ctest.log`): 199 of 201 pass. Registered tests are 201 (199 baseline + `mage_skill_owner_v1` + `runtime_session_projectile_render_v1_skill_models`). Two FAIL, both caused by an input file in the primary checkout, not by this branch (verified below).
  - `original_melee_bindings`: `Original binding table counts changed`. The test expects 25 actors / 16 factions / 95 clips from `.local-inputs/windows-melee-bindings/original-melee-bindings.xml`, which was regenerated in the primary at 16:23:52 today (another agent's bindings export; backup of the previous file `original-melee-bindings.base4-backup.xml`, 16:23:11). With the backup content the test PASSES (checked in a temp input root, since removed); with the regenerated file it fails.
  - `runtime_skill_activation_session_v1`: `Skill request CharacterState does not identify this retained session actor ... actor-definition=RoguePlayerBase, state-class=RoguePlayerBase_Assassin`. The same regenerated bindings file adds a combat binding for the Assassin subclass, so the fixture no longer SKIPs and reaches the prepare step. With the backup content the test PASSES (exit 0; checked). The lead must decide whether the regenerated file is the new input (then update the expected counts and the Assassin fixture) or the backup must be restored.

## Files changed (branch v21.8-magewire)

- `port/windows-foundation/main.cpp`: projectile executor/sensor lambdas split out (`ensurePlayerProjectiles`, `registerPlayerProjectileSensor`); `mageSkillOwner` with services at the PC skill-key cast site; `advance` per frame; reset at session switches.
- `port/windows-foundation/features/effects/runtime_session_projectile_render_v1.{hpp,cpp}` and its test.
- `port/windows-foundation/features/generic_skills/runtime_mage_skill_owner_v1_tests.cpp` (new), `CMakeLists.txt` (2 test registrations).
- `coordination/v21.8/asset-selection-v21.8.json` (+ stone escape idle FX).
- `coordination/v21.8/magewire-report.md` (this file).

Not done: `SKILL-VERIFICATION-MATRIX.md` runtime-verified column for the Mage rows (left for the lead; the evidence above is per-run).

## Recipes used

- Build: `bash C:/dh218/build-magewire.sh` (TARGET optional). Tests: `bash C:/dh218/ct-mw.sh`.
- Real runs: `powershell -File C:/dh218/tools/run-mage-mw.ps1 -Case <name> -Exe C:/dh218/magewire2.exe -Save C:/dh218/seeds/mage/{chain,cyclone,stone,coldray,profile}.save -Clicks "W|{CASE}\S1.sync,0.5:-9:0:80,2:-50:0:80" -Frames 480 -Captures "170,176" -Timeout 300 -Extra "--skip-boot;--test-player-hp;5000;--capture-at;40:{CASE}\S1.sync;--original-native-bodies;--original-floor-motion;--original-heading-rotation;--original-target-position;--combat-retained-phase;MagePlayerBase;--original-actor-anchor;forward"` (`-50` = key 2, which maps to skill slot 0; `-9` = Tab; `-32` = Space, the staff attack).
- Saves: `C:/dh218/tools/skills-a/make_rogue_skill_save.exe seeds/mage/profile.save <out> <Skill> 0 1 9`.
