# B097 - skill rank up does not change the stats (shot-16: Headsplitter rank 2, Current = Next = MP 6, damage 14-21)

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
