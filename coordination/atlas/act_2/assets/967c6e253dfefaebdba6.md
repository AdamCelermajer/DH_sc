# 3d/characters/statue/animations/child_pre_stunned.bdae

Stable asset ID: `967c6e253dfefaebdba6`. Source: `.local-inputs/assets-extra/android/data/3d/characters/statue/animations/child_pre_stunned.bdae`.

| Fact | Value |
| --- | --- |
| Source corpus | android |
| Relative path | 3d/characters/statue/animations/child_pre_stunned.bdae |
| Size bytes | 6292 |
| SHA-256 | c19ca39c90ba4852842ffc58afd29dad1d745bc0ed269755f05884b8980966ef |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_2 |
| Source-linked reference rows | 1 |
| Actor dependency trails | 1 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| ad41dc2bf8c5877f581b | anim | 73 | pydata/animations_pyarray.bin[sequence=110,step=0].Anim | RESOLVED | DONE | PROVEN | AnimStep field schema in animation_tables.cpp | UNKNOWN |  |  |  |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| DW_Cherub_RE | act_2 | CharacterProperties[55].AnimTable -> character_animations[8 ] -> pydata/animations_pyarray.bin[character=8].Stunned[0] -> animation_sequences[110] -> pydata/animations_pyarray.bin[sequence=110,step=0].Anim | animation_sequences[110] ChildStatue_Stunned | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | ad41dc2bf8c5877f581b | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/statue/animations/child_pre_stunned.bdae |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
