# v21.8 w2-archer: Archer subclass, phase 2b (branch v21.8-w2-archer)

Status: **BLOCKED at the first deliverable.** The Archer bow basic attack did not reach a real attack. The bow stance and bank now resolve, and combat initialization then fails on an engine gap (Type2 attack group choice). No Archer active and no Archer passive was implemented. Stopped per the rule "same reason fails twice": the failure reproduced on two packages, and the next fix is an engine change for the lead to escalate.

Base: windows-foundation @ 99d4d1c5. Worktree `.claude/worktrees/w2-archer`, subst `A:`, build dir `C:/dh218/w2-archer-build` (Release, llvm-mingw). The build (all targets) is OK. Full CTest was not run on the final tree (see Tests).

Evidence key: **Lua** = `.local-inputs/assets-extra/android/data/scripts/skills/prince_archer_*.luac` (plain text, re-read this phase). **Code** = file:line on this branch. **Run** = hidden-window run, case folder `C:/dh218/w2-archer/cases/<case>/` (run.log, injector.txt, run.args).

## 1. Bow basic attack (deliverable 1)

| Item | Verdict | Evidence |
|---|---|---|
| Bow stance 4 | WORKED (log) | Run `bow4`: `Source player melee bank class=RoguePlayerBase_Archer stance=4 moving=252 static=247 main=94 off=-1`. Matches the phase-1 arithmetic (252 `Human_Attack_05_Bow`, 247 `Human_AttackStatic_05_Bow`). Clip names and frames not yet viewed. |
| Bow save | WORKED (tool) | `archer_bow_save_v21_8` (new, test-only, `tests/archer_bow_save_v21_8.cpp`) turns the saved Archer dagger save into one with `Bow01` on the main-hand binding (source_slot 1) and the off-hand binding removed. Output: `wrote ... class=RoguePlayerBase_Archer main=Bow01`. The source_slot 1 = main hand mapping is inferred from main.cpp:1148 and is confirmed by the stance=4 / off=-1 line. |
| Bank selection for a ranged main hand | CODE CHANGED, runtime not reached | `main.cpp` ~1802: `rangedStaff` is now `family == MagePlayerBase || main-hand ItemTable Type (words[22]) in {4,5}`. Before, only the Mage family took the ranged-marker branch (no melee markers, state-5 marker to the projectile executor). Control run `rogue-ctl1` (Rogue dagger seed, same new binary) still loads: stance=2, no error, so daggers are not caught by the new rule. |
| ProjectileTable row 10 BowProjectile, `Mage projectile launch ... row=10` | DID NOT WORK (not reached) | No attack marker was produced: combat init fails first (below). |
| Arrow impact / miss sounds 291 / 292, no swoosh | NOT VERIFIED | Data only (phase-1 section 4.2-4.3: Bow01 SwooshSoundFX -1, ImpactSFX 291, ObjectImpactSFX 292). No run reached an impact. |
| Real run with real keys | DID NOT WORK | Runs `bow2`, `bow3`, `bow4` (case folders above). No frame PNG was captured (combat init aborts before the first capture). |

### The failure, exactly

Run `bow4` (staged2, bindings 228,404 bytes with the subclass actors, `Source actor property row unavailable` is gone):

```
Source player melee bank class=RoguePlayerBase_Archer stance=4 moving=252 static=247 main=94 off=-1 ...
Foundation error: Combat initialization: Explicit choice required for original type2 group
```

Same error on `bow2` (staged, single-file seed), `bow3` (staged, seed folder with the swamp world save) and `bow4` (staged2, seed folder). Run `bow1` (staged2 before the lead patched its bindings file) failed earlier with `Source actor property row unavailable: RoguePlayerBase_Archer`, which is the stale-bindings problem the lead described; that error does not occur in `bow4`.

Why (code, PROVEN by reading):
- `main.cpp:1839` sets `policy.sequenceAction = attack.static_selection` (`runtime_player_profile_attack_bank_v1.cpp:141`).
- `runtime_player_combo_chain_v1.cpp:169-195` builds that selection with `selection.choices` **empty**, and sets `requires_original_rng_start = true` when any sequence policy is type 2. The flag is never consumed (grep: no reader).
- `original_attack_sequence.cpp:55` requires an explicit choice for every type-2 group, so `prepare()` fails at `combat_session.cpp` ~1498.
- The Rogue dagger sequence (stance 2) has no type-2 group, which is why the dagger control run passes. The Mage staff path also has none.
- `features/combo_chain/source_combo_chain.hpp` (comment, ~line 52) says the original Type2 choice consumes the existing AnimationRandom stream through `select_start(...)`. That selection is not called on the player's `sequenceAction` path.

Not verified: which type-2 group(s) the bow sequences contain, and whether the original draws them per attack (most likely, since the flag name says "rng start"). IDA search for the sequence-type code path was started and not finished.

### Suggested next step (for the lead / Sonnet)
Draw the Type2 choice per attack for the player's static/moving selection through `select_start` with the AnimationRandom stream (the existing source_combo_chain path), instead of passing an empty `choices` map. This is a combat-session change in `combat_session.cpp` (sourceCombo/sequenceAction branch) and the planner, not a bow-specific change. Then re-run `bow4`'s case (`run-archer.ps1`, staged2, `archer-bow.save` seed folder `seed-bow1`, `--player-to -2900,-100,255@700`).

## 2. Archer actives (deliverable 2): NOT STARTED

Re-read the Lua this phase (confirms phase-1 with these details):
- **ExplodingArrows** (`prince_archer_exploding_arrows.luac`): Pre TargetListSearch(RANGE 1200, ANGLE 220) + LookAt + UseMana + SetSkillCooldown(cooldown); check = HasBow and not HasSkillCooldown and HasMana. Use: pop the top target (or none) and `SpawnSkillProjectile(ArrowExplode)`. Delayed(target): SetProp(SnS_Level), ApplyPropClass, SkillCombatRoll(target), then `StartSkillTimerCB(500, one-shot, DoExplosion, target)`. DoExplosion: TargetListSearch(RANGE_EXPL = TempProp1, 360, target) sorted ClosestFirst, PlayFX `spell_dh2_prince_rogue_burst_shot` on the target, PlaySound `sfx_impact_burst_shot`, roll up to MAX_COUNT=3 victims (the first is the target itself).
- **MultiHitArrow** (`prince_archer_multihit.luac`): Use runs on each of three `do_skill` markers (OnSkill_ pops the target, `SpawnSkillProjectile(ArrowDefault)`). Delayed: `hit++`; `hit==3 and pot_targets==1` → SetKnockBackState(true); `hit<3 and pot_targets==2` → SetKnockBackState(false); then SkillCombatRoll. Needs the per-marker re-entry (N11) and KB (N5), neither verified.
- **PinShot** (`prince_archer_pin_shot.luac`): RANGE 600, no angle. Use: first projectile at top target; if count>1 and level>4, `StartSkillTimerCB(750)` spawns a second projectile at the next target; if count>2 and level>9, a third at 1500 ms. Delayed: dedupe by name; on a hit `target:SetStunState(duration, true)` with duration = SnS_Duration/1000 s (ES-STATUS stun owner needed; not checked this phase).
- **PoisonArrows** (`prince_archer_poison_arrows.luac`): one ArrowPoison projectile per Use; Delayed = SetProp/ApplyPropClass/SkillCombatRoll. The author note says the DOT does not work (open, phase-1 Q4).
- **Volley** (`prince_archer_volley.luac`): one ArrowDefault per Use; Delayed rolls the target; `do_rebound`: `GetRand(0,100) < chances` (TempProp1 from class), then TargetListSearch(900, 360, victim, NoSort), pops until a name differs from `oldTarget`, spawns a second ArrowDefault at the victim. The Lua's `SetProjectileTarget` is ignored (it "doesn't seem to work"), and the Lua fakes the target with `newTarget`.

Engine pieces they need and that exist on this base: owner registry (`runtime_skill_external_owner_registry_v1`), skill timers (`runtime_skill_timer_service_v1`), `RuntimeSessionProjectileV1::launch_skill` (rows 3/4/6/7), target query with origin. Missing: an Archer owner class and profile rows (none exist; phase-1 section 1 is still correct), HasBow predicate, stun state (ES-STATUS), KB state (N5).

## 3. Archer passives (deliverable 3): NOT STARTED

- **BowMastery**: its Lua is the exact pattern `CreateBuff(CLASS_ID, 0, false, slvl)` + `ApplyBuff` on level change, which `passive_skill_owner_v1` already implements for the stat table. Adding `Skill_Archer_BowMastery` to that table should be data-only. Not done this phase; the effect (props 55, 87) has not been checked against a damage reader.
- **Precision**: needs `StealAIEvent("OnTargetHit")` (player as attacker), HasBow, and a 1000 ms repeating check. The existing event passive owner (`event_passive_owner_v1`) covers the attacker-side OnTargetHit bus, so Precision could be a third event kind. Not done.
- **SideStep**: defender-side OnTargetHit, melee-range check, KB state (N5). Not done.
- Shared Rogue passives (Acrobat, Ambidextrous, ComboMaster, FindWeakness): Acrobat already WORKED in the ES-PASSIVE real run; the others are the Rogue owner's, not started here.

## 4. Engine changes made

1. `port/windows-foundation/main.cpp` (~line 1802): ranged main-hand rule (Type 4 or 5) joins the Mage family for the ranged-marker bank branch. Small diff (9 lines).
2. `port/windows-foundation/tests/archer_bow_save_v21_8.cpp` (new, test-only, not a CTest) and its `CMakeLists.txt` target (`archer_bow_save_v21_8`).

No change to the staged packages, bindings files, or saves in the repo.

## 5. Harness notes (for the lead)

- Package: `windows-source-clock-v21.8-staged2` (its `assets/original-melee-bindings.xml` is the 228,404-byte regenerated file, per the lead's note; it was the old file when run `bow1` was made). `windows-source-clock-v21.8-staged` has a 231,638-byte bindings file. Run `bow2`/`bow3` on that package reached the same type2 error, so the type2 blocker does not depend on the package.
- The runner is `coordination/v21.8/act1rogue-run-rogue2.ps1`, copied to `C:/dh218/w2-archer/run-archer.ps1` with the case root moved to `C:/dh218/w2-archer/cases`.
- Seeds: `C:/dh218/w2-archer/archer-bow.save` (single file), and `C:/dh218/w2-archer/seed-bow1/` (the bow save plus the swamp world save copied from `C:/dh218/cases/sub-archer-3`). The world save is not required for the type2 error (`bow2`, single file, fails the same way); it was added to match the Rogue runs.
- Clicks: `C:/dh218/w2-archer/clicks-bow1.txt` (Enter, then Space every 1.5 s, no Tab; the Mage auto-target path needs no Tab).

## 6. Tests

- Build: all targets built on the final tree (`C:/dh218/w2-archer-build`, `BUILD_OK`).
- CTest: **not run on the final tree.** The changes do not touch any unit-test source; `main.cpp` is linked only into `dh-foundation.exe`. The full CTest is still owed before the lead merges.
- Unit tests for the new owner: none (no owner written).

## 7. Cleanup

- Junction `.claude/worktrees/w2-archer/.local-inputs` points to the primary `.local-inputs` (one junction instead of 934; removed with `cmd /c rmdir`, not recursively).
- `subst A:` removed with `subst A: /D`.

## 8. Commit

Branch `v21.8-w2-archer`, see the final git log entry.
