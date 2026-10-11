# B097 - skill rank up does not change the stats (shot-16: Headsplitter rank 2, Current = Next = MP 6, damage 14-21)

## Continuation investigation and verification plan (2026-10-11, before new tests)

Owner `/root/b097_rank_stats`; independent reviewer `/root/b097_review`. Preserve prior fix `ab95d356`; privately merge reviewed integration `10f8fa3f` for B096 class-list admission and B102 short-click input.

- Visual evidence inspected: user `shot-16.png` actually shows Headsplitter rank2 with both columns MP6/damage14-21. Original P2 `part2_cIAW38IfLxY.mp4` extracted at 394,394.5,...397.5 seconds (`.local-inputs/claude-v20.1/fix097b/original-skills394-3975.png`). At 394-395 the selected Headsplitter is rank1: Current MP9/damage14-21; Next MP12/damage15-22, with two points and Upgrade visible. At 395.5 selection changes to Inner Strength; 396-397 its rank changes1->2, points2->1 and next-level text becomes the character-level training cap. At397.5 a confirmation overlay opens. This sequence establishes the visible numeric distinction; it does not show a rank2 Headsplitter cast. The older507-510 reference was Map footage and is explicitly withdrawn.
- Logic evidence: recovered `skill-lua-v46.json::bashdown.source` actually calls SetTempProps(CLASS_ID,slvl) for info, CalcManaCost(CLASS_ID,slvl) for Update, HasMana for Check (no BashDown cooldown), frontal-first Enemy/AttackableOnly search at range160 for Pre, retaining one target, then UseMana even on empty search; Use writes rank to SnS_Level, applies Class215 and SkillCombatRoll to the retained target; Post clears target. Comments claiming mana5/+1 conflict with executable ClassTable data and are not authority. `_SetProp`0x3b7eac uses owner PROPS_Set when third arg is Boolean; `_ApplyPropClass`0x3babdc branches by arg type. `_LinearWithPropPlusBaseValue`0x3e2d4c mode1 reads owner+2708. NativeGetSkillDetails0x445a10 resolves class-list position, compares source required level, invokes AI_SkillInfo at saved rank and rank+1 and formats ordered display_props; cap/current-level messages replace numeric text on their source gates. `Character::UseMana`0x3bdef4 checks negative amount, online remote/GOD_MANA/debug exemption, HasMana, and debits property41 only on admission. B096 recovered gate mapping is reused; no passive rule is inferred.
- Expected implementation: preserve rank on the copied owner source passed to both shared formula evaluations, retaining actual same-character weapon/stat source. Do not mutate player progression while previewing Next. Reuse current source skill admission, marker/lifecycle and result calculation.
- Focused tests defined: normal level2 source profile -> real10ms select Headsplitter -> details rank1/current9,next12 -> real Upgrade rank2/current12,next15 -> duplicate no spend -> save/Continue/reselect same rank/values -> cast mana12. Deterministic same-Session paired rank1/rank2 BashDown uses identical weapon/target/RNG and reaches actual do_skill result; verify exact mana9/12, increasing source damage interval, exact source-gold damage outcomes, duplicate suppression, insufficient MP rejection and accepted empty-target mana spend. Existing generic mana and Rogue tests cover additional class/table formulas. Frozen EXE/run args, captures and complete ctest results must be retained.
- Uncertainty: APK recovered source v1.0.2 versus video v1.0.3. Ordinary gameplay leveling to2 is fixture setup rather than replayed leveling. Damage shown in UI is a range, actual outcome also depends on weapon, RNG, target defenses, source flags and lifecycle; a rank factor alone cannot prove hit damage. Other unaudited skill-info/passive consumers remain outside this bounded fix.

## Continuation verified results (2026-10-12)

### Investigation correction: individual damage need not grow monotonically

The initial paired test asserted each identical-seed higher-rank hit must grow. That assumption failed with rank1=2.5, rank2=2.34375. Investigation proved initial RNG1:0 and final12133000:9, target3 and outcomes0 are identical. Original `Random::GetRandom`0x33ff90 computes recurrence then `seed % bound`; `CF__CalcDamage`0x3b1fb8 uses `minimum + GetRandom(maximum-minimum)`. Therefore changing interval width changes the remainder, not a fixed percentile. Rank1 fixed bounds536/1608 and rank2 bounds560/1680 follow unarmed base512/1536 times the executable class factor268/280 divided256. At the ninth roll seed12133000, independent gold arithmetic yields536+104=640 fixed(2.5) and560+40=600 fixed(2.34375). The corrected test asserts increasing source bounds and these exact damage/HP outcomes; it does not invent monotonic individual rolls. Source code locations `port/game-data/combat.cpp::roll/dh2_combat_random/dh2_combat_damage` and `combat_result.cpp::dh2_combat_result` preserve those original arithmetic/call gates.

### Standalone production UI/train/save/Continue

Evidence scratch root: `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/claude-v20.1/fix097b/`. Frozen executable `B097-frozen.exe`, SHA256 **A0D3391B90881FC3ECEC52D15601594771493BA9939FC33D2039AF53995DFC55**, built after privately merging reviewed integration10f8fa3f into a46a65b1. Only added production delta in this continuation is `[[maybe_unused]]` on the faery sound helper so the strict Warrior-only test configuration compiles; game behavior is unchanged after freezing. The original B097 rank fix remains ab95d356.

The fixture generator loads the real B096 source-generated level2 seed with BashDown1/points1 and saves it through SaveStore, adding Longsword01 as source main-hand slot1. It does not edit bytes. Profiles, configurations, world saves and captures are private; assets/audio/UI junctions only read acceptedv20 content. Failed fixture attempts (wrong main-hand slot8 and command-line array quoting) are not success evidence.

- **Failure reproduced:** acceptedv20 EXE, same level2 Longsword profile already rank2. `baseline.args`, quiet_run exit0; `baseline/cap.png` visibly shows rank2 Current=Next MP6/damage14-21, matching shot16. Baseline uses the existing frame-scheduled pointer adapter because acceptedv20 lacks the newer Win32 client-coordinate support.
- **Before:** `rank1-cast.args`, real10ms pointer select. `rank1-cast/f540.png` shows selected rank1, points1, Upgrade visible, Current MP9/damage14-21, Next MP12/damage15-22. These exact numbers match original P2 Headsplitter394-395s.
- **Upgrade/duplicate:** `train.args`, quiet_click clicks8s90:184,10s916:650,12s916:650, all10ms. `train/f540.png` shows rank2/points0, Current MP12/damage15-22, Next MP15/damage15-23. Log commits exactly once `Source skill training row=0 rank=1->2 points=0`, then refuses duplicate. Saved profile inspection confirms level2/rank2/points0/Longsword01.
- **Reload/no-point refusal:** copy the normally written trained profile into a fresh `reload/` save owner, Continue through the actual frontend, reselect with10ms click and repeat Upgrade position. `reload/f540.png` preserves the rank2/current-next values and points0; log has refusal with no training commit; saved inspection retains rank2/points0.
- **Normal cast dependency discovered:** both rank1/reload scheduled source key2 at frame1100 reach the production skill dispatch but return generation0/phase0/MP28.5 unchanged. Main sets `non_character_attackable_objects_absent = world.objects().empty()`; coordinator rejects the unresolved AttackableOnly object-candidate scope despite no target nearby. This is B083's owned target-admission repair; no bypass was introduced. A10ms key20s in initial train run occurred while the menu was open and is explicitly not counted as a cast proof. B083 must be merged and the normal cast rerun before B097 is called fully integrated.

### Isolated source consumer and regression verification

`mana-test.log`: PASS BashDown ranks0/1/2/7/65535 costs, owner nonmutation, unknown class and overflow. `text-test.log`: PASS four exact info/current-next cases plus73 static localized rows,42 projected script-info rows and48 Rogue tree-cell audits; only BashDown/JumpKick have actual audited dynamic detail providers. The unarmed BashDown integer display can remain2-6 across neighboring ranks despite growing fixed bounds; Longsword production captures above show damage-range growth.

The paired actual coordinator regression restores the same GameSave/live sheets, target3 and source RNG, then reaches one authored `do_skill` marker and one completed result at each rank, MP9/12, exact gold damage/HP, duplicate no-spend/no-RNG/no-second-hit. Rank2 at9MP refuses without mutation; exact12MP with empty target search spends12MP and completes with no fabricated result. It retains existing Warrior GroundSlam/Charge and Rogue JumpKick production-consumer checks. The runner accepts an optional scratch root; retained Session callback owners are detached before their local coordinator dies. The initial test lifetime hang was confined to its own PID and corrected in the fixture; it does not establish a production bug.

Build wrapper exit0. Full172 CTests:170pass; known worktree session_skill_binding52 failed and campaign_host82 hit timeout123.63s during parallel load. Focused82 rerun without source change PASS0.84s (`ctest-campaign-rerun.log`), giving171/172 aggregate with only the documented52 failure remaining. Preserve the original full log rather than hiding the timeout. `git diff --check` passes.

Independent reviewer `/root/b097_review` strictly compiled and ran the final source consumer in its own `fix097-review/cast-output` scratch: PASS exact gold/current-rank bounds, mana/empty/duplicate branches and existing Warrior/Rogue matrix. It independently inspected original394-397.5, accepted baseline, rank1, trained rank2 and Continue/reload captures and verified the frozen executable SHA. Review of this standalone portion passes; full normal cast verdict remains conditional on B083.

Additional original caller/gate audit: `_SkillCombatRoll`0x3b9fbc bounds-checks actual class SkillList position and nonnull target user data, resolves GetCharSkill, and reads flags+28/element+20. The dual-weapon flag0x800000 admits separate main/off weapon attacks only when the corresponding weapon exists; BashDown's actual0x00335535 lacks that flag, so the unarmed source fixture legitimately reaches its single result. Character targets call F_SkillAttack then F_ApplyResult; compatible non-character type8 uses its virtual destructible consumer. `AI_IsSkillUsable`0x3d8358 requires loaded script process, in-range nonnull script, not casting and no noninterruptible active skill before OnSkillCheck_Usable. The rank patch adds no bypass to these target/class/weapon/owner gates; B083 owns the unresolved host object/hurt admission changes.

## Historical implementation report before continuation (superseded verification gaps)

The following preserves the earlier session's investigation and gaps. Its "not verified" list describes that earlier session; the continuation above supplies original footage, normal details/training/Continue and actual source-consumer proof. Normal executable cast remains explicitly gated on B083.

Branch fix/b097 (from v20.1/integrate). Headsplitter = SkillTable row 7 `BashDown`, script `prince_warrior_bashdown`, class `Skill_Warrior_BashDown` (class row 215).

## Root cause (observed by running the real class tables)
Both the Skills page text (`runtime_skill_mana_v1.cpp::evaluate_skill_temp_properties_v1`) and the cast projection
(`runtime_skill_cast_coordinator_v1.cpp::source_skill_class_projection`) set `SnS_Level` (prop 172) only on the temporary sheet, but passed the
player's live resolved sheet (SnS_Level = 0) as the type-1 "buff" source. Class 215 is `dst173 = 1536 + 768*prop172` (MP cost) and
`dst190 = 256 + 12*prop172` (damage scale), both type-1 reads of prop 172, so every rank evaluated at rank 0.
Measured with the real tables (scratch dump): buff-as-before gives MP 6 for ranks 0..5; with rank on the buff sheet: 6, 9, 12, 15, 18, 21.
The same wrong sheet fed the real cast (mana cost and `formula_sheet` damage), so the bug was not only visual.

## Original logic (IDA libDungeonHunter2.so, pseudocode-all.c)
- `Character::_ApplyPropClass` 0x3babdc -> `PROPS_ApplyClass(id, true)` 0x3df3b8: target `s_temp`, flag a4 = 1 -> `_LoadClass` 0x3e2e20.
- `_LinearWithPropPlusBaseValue` 0x3e2d4c: with a4 set, the type-1 source property is read from the OWNER sheet (`this+2708`), not from s_temp.
- Therefore `SetTempProps(CLASS_ID, slvl)` must put slvl on that owner sheet. Existing recovered note
  `port/engine-ui/reference/hud-formatting-v1/NOTES.md`: "SetProp's third Boolean is ignored; SnS_Level updates owner.resolved;
  ApplyPropClass(class,true) evaluates mode1 using live owner.resolved". Our port ignored the first half.

## Fix (smallest, generic: every class-driven player skill that uses the shared evaluators)
- `runtime_skill_mana_v1.cpp`: copy the resolved snapshot, set [172] = fixed rank, pass that as buff (feeds page current/next text AND `CalcManaCost`).
- `runtime_skill_cast_coordinator_v1.cpp`: same copy for the Use-time `formula_sheet` (damage/effects of the actual cast).
- Not touched: faery casts (`celest_*`, `hotty_*`) use their own projections; not audited here.

## Tests
- `runtime_skill_mana_v1_tests.cpp`: expected sheet now built with rank on the buff, plus hard check BashDown MP = 6 + 3*rank for ranks 0,1,2,7 (fails before: 6 constant). PASS.
- `runtime_skills_text_v1_tests.cpp`: current_level != next_level for BashDown and Rogue JumpKick ranks 1,2 (real tables, real localization). Output after fix:
  BashDown r1 MP 9 -> 12, r2 MP 12 -> 15; JumpKick r1 12/1-7 -> 14/3-10, r2 14/3-10 -> 16/5-13. PASS (test is outside ctest; compiled by hand with the same command as run_runtime_skills_text_v1_tests.ps1 against build-fix097 libs).
- Build exit 0 (-Jobs 2); ctest 167/168, only `session_skill_binding` fails (allowed in worktrees).

## Observed / inferred / not verified
Observed: shot-16 symptom; real class-table numbers before/after (scratch dump); unit outputs above; IDA code of 0x3e2d4c / 0x3df3b8.
Inferred: the live in-game damage (with weapon) scales by the class-190 factor (256+12*rank)/256 (+4.7%/rank) - values come from data, not hand-tuned; integer-rounded display may still look similar between neighbouring ranks at low weapon damage.
Not verified: reference video (no skills-page footage checked, so "matches the video" is NOT claimed; match is to IDA + recovered tables only); the real EXE skills page at rank 2 (a click injection via `--campaign-click` did not select the skill cell in my run, so the details pane was never shown in a capture; rank-1 fresh-player page captured without selection); a real cast damage comparison rank 1 vs 2; B096 learn button (separate branch) and other skill scripts, whose OnSkillInfo is still "not source-audited" (only BashDown/JumpKick show current/next text).
