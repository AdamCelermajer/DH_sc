# scripts/skills/swampking_atk_spit.luac

Stable asset ID: `1b1c3debd5faabe8dcc8`. Source: `.local-inputs/assets-extra/android/data/scripts/skills/swampking_atk_spit.luac`.

| Fact | Value |
| --- | --- |
| Source corpus | android |
| Relative path | scripts/skills/swampking_atk_spit.luac |
| Size bytes | 4009 |
| SHA-256 | 6254027d8aae8787a95adffa35ab41b769b5cf3ad77f51533b18d25e5541ca6f |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_1 |
| Source-linked reference rows | 1 |
| Actor dependency trails | 3 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 7f221c038af75cbdb8ed | script | swampking_atk_spit | pydata/skills_pyarray.bin[skill=108].Script | RESOLVED | DONE | PROVEN | SkillTable named string field; game-data/skill_tables.cpp | UNKNOWN |  |  | data/scripts/skills/swampking_atk_spit.luac |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| SwampKing | act_1 | CharacterProperties[358].SkillTree -> skills#group0[33 ] -> skills#group1[108] -> pydata/skills_pyarray.bin[skill=108].Script | skills#group1[108] SwampKingAttack3 | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/skills_pyarraynames.bin | 7f221c038af75cbdb8ed | RESOLVED | DONE | SkillTable named string field; game-data/skill_tables.cpp | None | UNKNOWN |  | .local-inputs/assets-extra/android/data/scripts/skills/swampking_atk_spit.luac |
| SwampKing | act_1 | CharacterProperties[358].SkillTree -> skills#group0[33 ] -> skills#group1[108] -> pydata/skills_pyarray.bin[skill=108].Script | skills#group1[108] SwampKingAttack3 | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/skills_pyarraynames.bin | 7f221c038af75cbdb8ed | RESOLVED | DONE | SkillTable named string field; game-data/skill_tables.cpp | None | UNKNOWN |  | data/scripts/skills/swampking_atk_spit.luac |
| SwampKing | act_1 | CharacterProperties[358].SkillTree -> skills#group0[33 ] -> skills#group1[108] -> exact skills#group1 Script swampking_atk_spit | skills#group1[108] SwampKingAttack3 | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/skills_pyarraynames.bin | 16d9803cce805b7bb594 | UNRESOLVED | UNRESOLVED | exact skills#group1.Script to script inventory name | CALL_MAPPING_INCOMPLETE | None |  | .local-inputs/assets-extra/android/data/scripts/skills/swampking_atk_spit.luac |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
