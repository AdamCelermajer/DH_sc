# 3d/characters/witch/animations/witch_pre_attack_01.bdae

Stable asset ID: `a8bcb70cd6c362c0edfc`. Source: `.local-inputs/assets-extra/android/data/3d/characters/witch/animations/witch_pre_attack_01.bdae`.

| Fact | Value |
| --- | --- |
| Source corpus | android |
| Relative path | 3d/characters/witch/animations/witch_pre_attack_01.bdae |
| Size bytes | 10940 |
| SHA-256 | c9fa4d208f150d14eb11e58cbcb45624afb2101cfd6e420237eb31b4cabedac9 |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_1 |
| Source-linked reference rows | 1 |
| Actor dependency trails | 1 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 044ca654d97f5c5c95bd | anim | 1419 | pydata/animations_pyarray.bin[sequence=754,step=0].Anim | RESOLVED | DONE | PROVEN | AnimStep field schema in animation_tables.cpp | UNKNOWN |  |  |  |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Witch | act_1 | CharacterProperties[435].AnimTable -> character_animations[76 ] -> pydata/animations_pyarray.bin[character=76].Attack[0] -> animation_sequences[756] -> pydata/animations_pyarray.bin[sequence=756,step=0].Anim -> animation_sequences[754] -> pydata/animations_pyarray.bin[sequence=754,step=0].Anim | animation_sequences[754] WitchOneHandedAttack01 | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | 044ca654d97f5c5c95bd | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/witch/animations/witch_pre_attack_01.bdae |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
