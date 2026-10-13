# scripts/skills/witch_ranged_00.luac

Stable asset ID: `d92cdc0b7d50b7b65cfc`. Source: `.local-inputs/assets-extra/android/data/scripts/skills/witch_ranged_00.luac`.

| Fact | Value |
| --- | --- |
| Source corpus | android |
| Relative path | scripts/skills/witch_ranged_00.luac |
| Size bytes | 2866 |
| SHA-256 | b92a73f95d28a21ed9fd08c904051a8f6fbf15062f917b6f05ee680c6e0faf2e |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_1 |
| Source-linked reference rows | 1 |
| Actor dependency trails | 3 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 5b7553e79400b41004c8 | script | witch_ranged_00 | pydata/skills_pyarray.bin[skill=122].Script | RESOLVED | DONE | PROVEN | SkillTable named string field; game-data/skill_tables.cpp | UNKNOWN |  |  | data/scripts/skills/witch_ranged_00.luac |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Witch | act_1 | CharacterProperties[435].SkillTree -> skills#group0[35 ] -> skills#group1[122] -> pydata/skills_pyarray.bin[skill=122].Script | skills#group1[122] Witch_Projectile | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/skills_pyarraynames.bin | 5b7553e79400b41004c8 | RESOLVED | DONE | SkillTable named string field; game-data/skill_tables.cpp | None | UNKNOWN |  | .local-inputs/assets-extra/android/data/scripts/skills/witch_ranged_00.luac |
| Witch | act_1 | CharacterProperties[435].SkillTree -> skills#group0[35 ] -> skills#group1[122] -> pydata/skills_pyarray.bin[skill=122].Script | skills#group1[122] Witch_Projectile | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/skills_pyarraynames.bin | 5b7553e79400b41004c8 | RESOLVED | DONE | SkillTable named string field; game-data/skill_tables.cpp | None | UNKNOWN |  | data/scripts/skills/witch_ranged_00.luac |
| Witch | act_1 | CharacterProperties[435].SkillTree -> skills#group0[35 ] -> skills#group1[122] -> exact skills#group1 Script witch_ranged_00 | skills#group1[122] Witch_Projectile | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/skills_pyarraynames.bin | 1ecf36c4a51dbd458ca6 | UNRESOLVED | UNRESOLVED | exact skills#group1.Script to script inventory name | CALL_MAPPING_INCOMPLETE | None |  | .local-inputs/assets-extra/android/data/scripts/skills/witch_ranged_00.luac |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
