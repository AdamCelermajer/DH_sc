# 3d/characters/statue/animations/child_dying.bdae

Stable asset ID: `8be65fdaf3c4c1868dfe`. Source: `.local-inputs/assets-extra/android/data/3d/characters/statue/animations/child_dying.bdae`.

| Fact | Value |
| --- | --- |
| Source corpus | android |
| Relative path | 3d/characters/statue/animations/child_dying.bdae |
| Size bytes | 8368 |
| SHA-256 | 79d570e5237b76ffb282089a817463cbd9466f5281db8b7786740cd630473492 |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_2 |
| Source-linked reference rows | 1 |
| Actor dependency trails | 1 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 5b88d853abbb15373cd3 | anim | 61 | pydata/animations_pyarray.bin[sequence=103,step=0].Anim | RESOLVED | DONE | PROVEN | AnimStep field schema in animation_tables.cpp | UNKNOWN |  |  |  |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| DW_Cherub_RE | act_2 | CharacterProperties[55].AnimTable -> character_animations[8 ] -> pydata/animations_pyarray.bin[character=8].Died[0] -> animation_sequences[103] -> pydata/animations_pyarray.bin[sequence=103,step=0].Anim | animation_sequences[103] ChildStatue_Died | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | 5b88d853abbb15373cd3 | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/statue/animations/child_dying.bdae |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
