# v21.8 w4-grapple: Assassin GrapplingHook on the general engine (2026-10-14)

Branch `v21.8-w4-grapple`, worktree `.claude/worktrees/w4-grapple`, subst `X:`, build dir `C:/dh218/w4-grapple-build`.
Package for runs: `.local-inputs/windows-source-clock-v21.8-staged3`. Save: `subclass_save_writer_v21_8` (RoguePlayerBase_Assassin,
GrapplingHook on slot 0, level 12) -> `C:/dh218/seeds/w4-grapple-assassin/character.save`. Never pushed, not merged.

## Verdict: PARTIAL

The cast, the sound resolution, the projectile launch and contact, the roll, the stun and the pull to arrival all happen in a
real hidden-window run with the real keys. Three things are not shown: the knife's visible flight, a clean pull (the target is
displaced mid-pull, cause not isolated), and anything about sound or visuals against the original (there is no original Assassin
footage in the repo, and nobody has listened to the sound).

## 1. Per-item verdicts

| # | Item | Verdict | Evidence |
|---|------|---------|----------|
| 1 | Cast accepted (key 2, slot 0, rank 10) | WORKED | run `w4-gh-6` log: `Source skill key=2 slot=0 frame=3338 ... skill=GrapplingHook` (no admission refusal) |
| 2 | Use reached at the do_skill marker | WORKED | `Source skill Use geometry ... frame=3367 sourceEvent=do_skill` in the same run; the Use launches the knife there |
| 3 | Sound request resolves to an existing file | WORKED for resolution, NOT heard | Lua label `sfx_skill_rogue_ass_throwing_knife` -> `audio-assets/data/sounds/sounds.xml` uid 139 -> `sfx_skill_rogue_ass_throw_knife.wav` (44338 bytes, present in staged3). Run log: `Plain sound submitted name=sfx_skill_rogue_ass_throwing_knife ordinal=550`. Nobody has listened |
| 4 | Projectile launched via the shared executor (row 21 `GrapplingHook`) | WORKED | `Rogue skill GrapplingHook projectile launched id=1 row=21`; native sensor registered `radiusGameUnits=28.66 category=32 mask=1311` |
| 5 | Projectile visible in flight | NOT VERIFIED (PARTIAL) | The knife's first contact came one frame after the launch (launch 3367, hit 3368, lizard 175 units away). No flight frames were captured. The frames at f3358 and f3372 show a small blade-like object near the hero's hand; not confirmed as the knife |
| 6 | Hit delivers the delayed body (roll) | WORKED | `Rogue skill GrapplingHook hit victim=7118915781085668844 damage=9.140625`; `Damage frame=3369 ... marker=do_skill removed=9.14 outcomes=0x0` |
| 7 | Stun: SetStunState(SnS_Duration, true) on the hit target | WORKED (log); pose NOT verified | `stun ... ms=4500.000000 applied=1` (= 2000 + 250 x 10 from the Lua formula); `[status] enemy decisions blocked ticks=60, 120, 180, 240` follow. No frame of the stun pose was checked |
| 8 | Pull: HeadTo(caster) while Stunned | PARTIAL | `pull start ... goal=-7071.76,986.40 arrive=100 distanceToGoal=175.63`; the run ends with `pull end ... end=arrived distanceToGoal=99.40 frame=117`. Mid-pull the target is displaced: distance 143.7 -> 215 -> 254.6, flat at 255.4 for ~20 advances, then approach resumes at about 2 units per advance. See section 4 |
| 9 | Boss / miniboss: roll only, no stun, no pull | WORKED in unit test only | `runtime_rogue_skill_owner_v1` boss case. No boss in any run |
| 10 | Delayed(nil) on expiry does nothing | WORKED in code only | `if (event.expired ...) return true` (not exercised in a run) |
| 11 | Animation of the cast (AnimationSequence 519, anim 908 npc_assassin_throw_knife) | NOT VERIFIED | The clip is staged; no frame compared with the authored throw |
| 12 | Visuals against the original | NOT COMPARED | No original Assassin footage in the repo |

## 2. Original evidence (IDA + Lua, plain text; atlas read-only)

- Lua `prince_assassin_grapling_hook` (the SkillTable script name is `prince_assassin_grapling_hook`, the file has the same stem):
  - OnPre: `TargetListSearch(RANGE 1000, ANGLE 220)` FrontalFirst + `LookAt(top)` + `UseMana`. No SetSkillCooldown. Post `ClearTarget`.
  - OnSkill_: `PlaySound("sfx_skill_rogue_ass_throwing_knife")`, `SpawnSkillProjectile(PROJECTILE_ID)` with no target (the caster's heading).
  - OnDelayedSkill_(target): a Character: `SetProp(SnS_Level)` + `ApplyPropClass(CLASS)` + `SkillCombatRoll(target)`; `SetStunState(duration, true)` unless IsBoss or IsMiniBoss; `if GetState() == Stunned` and not boss/miniboss: `HeadTo(GetPosition())`. An object: roll only. `nil` (expiry): nothing. The Lua has a TODO: "need to speed up the pulling".
- IDA `Character::_HeadTo` 0x3ba71c (`pseudocode/003b/003ba71c.c`): one userdata -> `Cmd_MoveTo(object)`; three numbers (or `(x,y,z,true)`) -> `v2Controller::Cmd_HeadTo(point)`. The Lua call passes three numbers (GetPosition), so it is the point form. Atlas `natives/HeadTo.md` agrees.
- IDA `SM_SetStunState` 0x3c5ffc: boss refused first; stun clip from CharAnimTable +140 (atlas `target_SetStunState.md`); the port's existing `script_status_actor` owns this.
- ProjectileTable `GrapplingHook` (row 21): model `projectile_returning_knives.bdae` (staged3 has it), DisappearOnHit 1, Timer 2000 ms, TimeBetweenHit 1000 ms, Velocity 30.0. Not used for the pull.
- Duration: class `Skill_Assassin_GrapplingHook` SnS_Duration = 2000 + 250 x rank ms (rank 10 -> 4500 ms, matches the log).

## 3. Engine changes (general unless stated)

1. **Projectile model enrollment (general fix, a real gap).** `RuntimeSessionProjectileRenderV1::load` enrolled a hard-coded list of six skill
   BDAE. Any other launched ProjectileDict model failed with `No authored projectile AABB is enrolled for this source URI`
   (run `w4-gh-1`, the first attempt, stops there). Now it also enrolls every ProjectileDict model of the original tables (the same
   tables the executor launches from; 12 BDAE, 272 KB). File: `features/effects/runtime_session_projectile_render_v1.cpp`.
2. **General forced move: `ForcedHeadToV1`** (new, `features/generic_skills/forced_head_to_v1.{hpp,cpp}`). Cmd_HeadTo as one
   per-actor command stepped each frame: XY step of speed x dt toward the goal, Z kept, arrival radius, refusal predicate, cancel,
   gone, move error. Any skill that drags an actor can use it.
3. **Rogue external owner: `RuntimeRogueSkillOwnerV1`** (new, `features/generic_skills/runtime_rogue_skill_owner_v1.{hpp,cpp}`).
   The GrapplingHook body is a `delayed` projectile callback, which the coordinator's data rows cannot express; the mage and archer
   owners exist for the same reason. The owner does: PlaySound, `launch_skill` (the existing executor), the delayed roll
   (`apply_source_result` with the Use-time formula sheet), `script_status_actor` for the stun (the status owner refuses boss and
   miniboss inside), `StatusStateOwnerV1::stunned` for the Stunned check, and `ForcedHeadToV1` for the pull.
4. **Profile row** `GrapplingHook` in `runtime_skill_use_profile_rogue_v1.hpp` (external owner, Pre query 1000 / half-angle 110,
   LookAt, Post ClearTarget, no cooldown). No new struct field was needed, so no other row changed. The fields after
   `kill_sound_name` take their defaults.
5. `main.cpp`: declaration and wiring of the rogue owner next to the archer owner (registered in `skillOwnerRegistry`, timer-free
   `advance` for the pull).
6. Tests updated for the admitted row: `runtime_skill_use_profile_v1_tests.cpp` (row count 3 -> 4; the "GrapplingHook is
   BLOCKED-PROJECTILE" assertion becomes "external owner row").

Not changed: the shared stun/knockback owner, the coordinator, the timer service (the GrapplingHook body has no StartSkillTimerCB;
its expiry is the executor's own 2000 ms timer).

## 4. Gaps and open questions

- **Pull speed and arrival radius are not in the Lua.** The port uses 250 units/s (`kPullSpeedPerSecond`) and arrival 100
  (`kPullArriveRadius`). Both are constants of the owner, not recovered from the original (Cmd_HeadTo's walk-locomotion speed is
  not decoded). The Lua TODO ("need to speed up the pulling") says the authored pull is slow.
- **Measured step versus command.** The owner steps 4 units per 16 ms command; the trace shows 2 units per advance call
  (`pull step` lines every 2 advances, distance drops 4). Not isolated: the advance cadence or the position write.
- **Mid-pull displacement (not isolated).** In `w4-gh-6` the target sits at 143.7 to 175 units, is displaced to 255.4 at about
  advance 22 to 24 and stays there for about 20 advances, then approaches again. Candidates, none verified: the stunned loop's
  root motion (`features/combat/session_status_pose_v1.cpp:169` sets `loop_root_motion = stun`, consumed at
  `combat_session.cpp:708`), collisions with a neighbouring actor or terrain, a reaction clip. The pull writes positions through the
  teleport verb, so each write is a position set, not a walk.
- **Knife flight not measured** (see item 5 above). The launch happens at the do_skill marker, one frame before the contact.
- **Object targets** (non-character actors) get the roll only. "IsCharacter" is approximated as "has combat properties".
- **Formula sheet** of the delayed roll is the Use-time sheet (the Lua re-applies the class at callback time; no difference expected).
- **Flake (not established):** run `w4-gh-3` ended at advance ~3028 with the injector `exit=-1`, before the cast. The same inputs passed
  in `w4-gh-4`, `w4-gh-5` and `w4-gh-6`. Not investigated further.
- **Harness note for other agents:** piping the runner into `Select-Object -First 1` stops it before the PPM-to-PNG conversion
  (the case keeps `.ppm` files). Convert with `.local-inputs/v21.4/verify/tools/ppm2png.ps1` by hand, or capture the output first.

## 5. Real runs (hidden window, real keys, `act1rogue-run-rogue2.ps1`, `--fixed-step .016`)

Clicks: the `w2a-assassination-4` plan (Tab/Space/key 2). Extra: `--player-to -2900,-100,255@700`, `--test-skill-rank GrapplingHook=10`,
`--follow-actor Swamp_LizadMan_Type1@1500-4900:60!`. Cases in `C:\dh218\act1rogue\cases\`:

- `w4-gh-1`: before the AABB fix: `Foundation error: ... No authored projectile AABB is enrolled` right after the launch.
- `w4-gh-4`: after the fix; full cast, roll, stun, pull, arrival at 175.6 -> `arrived` (arrival radius was the melee reach, 200).
- `w4-gh-6` (current build, exe sha256 `3B3D7470...55BA`): knife launch 3367, hit on the next frame (damage line 3369), `damage=9.14`, `stun 4500 ms applied=1`,
  pull trace per advance, pull end `arrived distanceToGoal=99.40`. Frames `f3340 ... f3392` and `final.png` are in the case folder
  (viewed: f3342, f3358, f3372, f3390: the hero in melee, "Miss/Block" texts, both lizards in ordinary poses; no stun pose confirmed).

## 6. Tests and CTest

- New: `runtime_rogue_skill_owner_v1` (ctest, real CombatSession + shared executor): profile row flags; ForcedHeadTo alone (step,
  Z kept, arrival, refusal, cancel, gone, move error); hit -> roll damage, stun 2000 ms requested with SnS_Duration, pull start, pull
  step to arrival within 1 unit of the radius, no pull for a target that is not Stunned after the stun, boss: roll only.
- Updated: `runtime_skill_use_profile_v1` (rogue row count 4; GrapplingHook is an external-owner row).
- CTest on this branch (`ctest -j4`, 219 tests: the base's 218 plus the new `runtime_rogue_skill_owner_v1`): one full run had `runtime_skill_use_profile_v1` failing on the old "BLOCKED-PROJECTILE" assertions, fixed here; the
  final full run has 218/219 passing. The one failure, `runtime_skill_session_training`, is a wall-clock test under `-j4` load: it passed
  when run alone (9.5 s, `ctest -R`). Not related to this change by inspection, but not proven unrelated either.
