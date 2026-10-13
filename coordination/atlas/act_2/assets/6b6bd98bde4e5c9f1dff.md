# file000461.dat

Stable asset ID: `6b6bd98bde4e5c9f1dff`. Source: `.local-inputs/claude-preview15/ipa/extracted/Payload/dungeonhunter2iPad.app/data/file000461.dat`.

| Fact | Value |
| --- | --- |
| Source corpus | ios-packed |
| Relative path | file000461.dat |
| Size bytes | 2396 |
| SHA-256 | 06a762715bae1b3e04819c709fe92979c0960649dfadd6a6d5497c710ed99aca |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_2 |
| Source-linked reference rows | 1 |
| Actor dependency trails | 1 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| a5049e61901c6f4f7913 | script | DarkQueen2_melee_00 | pydata/skills_pyarray.bin[skill=20].Script | RESOLVED | DONE | PROVEN | SkillTable named string field; game-data/skill_tables.cpp | UNKNOWN |  |  | data/scripts/skills/DarkQueen2_melee_00.luac |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| DarkQueenForm1 | act_2 | CharacterProperties[62].SkillTree -> skills#group0[4 ] -> skills#group1[20] -> pydata/skills_pyarray.bin[skill=20].Script | skills#group1[20] DarkQueen1_Attack | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/skills_pyarraynames.bin | a5049e61901c6f4f7913 | RESOLVED | DONE | SkillTable named string field; game-data/skill_tables.cpp | None | UNKNOWN |  | .local-inputs/claude-preview15/ipa/extracted/Payload/dungeonhunter2iPad.app/data/file000461.dat |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
