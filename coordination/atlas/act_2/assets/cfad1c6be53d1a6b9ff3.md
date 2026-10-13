# 3d/characters/statue/animations/statue_idle.bdae

Stable asset ID: `cfad1c6be53d1a6b9ff3`. Source: `.local-inputs/assets-extra/android/data/3d/characters/statue/animations/statue_idle.bdae`.

| Fact | Value |
| --- | --- |
| Source corpus | android |
| Relative path | 3d/characters/statue/animations/statue_idle.bdae |
| Size bytes | 17672 |
| SHA-256 | fadeba11a37e967b0e74818976ed79aa8bd220f30e1a792a4e65eb53599baba1 |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_2 |
| Source-linked reference rows | 4 |
| Actor dependency trails | 4 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 2c7a162a3e88d9e56f78 | anim | 1312 | pydata/animations_pyarray.bin[sequence=664,step=0].Anim | RESOLVED | DONE | PROVEN | AnimStep field schema in animation_tables.cpp | UNKNOWN |  |  |  |
| 39a38b42cd4c0ca8b586 | anim | 1312 | pydata/animations_pyarray.bin[sequence=661,step=1].Anim | RESOLVED | DONE | PROVEN | AnimStep field schema in animation_tables.cpp | UNKNOWN |  |  |  |
| 6627eac940c2136c694f | anim | 1312 | pydata/animations_pyarray.bin[sequence=661,step=0].Anim | RESOLVED | DONE | PROVEN | AnimStep field schema in animation_tables.cpp | UNKNOWN |  |  |  |
| 7bd1fc162e689bdd3b3c | anim | 1312 | pydata/animations_pyarray.bin[sequence=661,step=2].Anim | RESOLVED | DONE | PROVEN | AnimStep field schema in animation_tables.cpp | UNKNOWN |  |  |  |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| DW_Statue | act_2 | CharacterProperties[57].AnimTable -> character_animations[68 ] -> pydata/animations_pyarray.bin[character=68].GreatKnockedBack[0] -> animation_sequences[661] -> pydata/animations_pyarray.bin[sequence=661,step=0].Anim | animation_sequences[661] Statue_Idle | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | 6627eac940c2136c694f | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/statue/animations/statue_idle.bdae |
| DW_Statue | act_2 | CharacterProperties[57].AnimTable -> character_animations[68 ] -> pydata/animations_pyarray.bin[character=68].GreatKnockedBack[0] -> animation_sequences[661] -> pydata/animations_pyarray.bin[sequence=661,step=1].Anim | animation_sequences[661] Statue_Idle | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | 39a38b42cd4c0ca8b586 | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/statue/animations/statue_idle.bdae |
| DW_Statue | act_2 | CharacterProperties[57].AnimTable -> character_animations[68 ] -> pydata/animations_pyarray.bin[character=68].GreatKnockedBack[0] -> animation_sequences[661] -> pydata/animations_pyarray.bin[sequence=661,step=2].Anim | animation_sequences[661] Statue_Idle | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | 7bd1fc162e689bdd3b3c | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/statue/animations/statue_idle.bdae |
| DW_Statue | act_2 | CharacterProperties[57].AnimTable -> character_animations[68 ] -> pydata/animations_pyarray.bin[character=68].Walk[0] -> animation_sequences[664] -> pydata/animations_pyarray.bin[sequence=664,step=0].Anim | animation_sequences[664] Statue_Walk | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | 2c7a162a3e88d9e56f78 | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/statue/animations/statue_idle.bdae |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
