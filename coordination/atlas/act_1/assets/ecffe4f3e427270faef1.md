# 3d/characters/swampking/animations/swampking_pre_attack_03.bdae

Stable asset ID: `ecffe4f3e427270faef1`. Source: `.local-inputs/assets-extra/android/data/3d/characters/swampking/animations/swampking_pre_attack_03.bdae`.

| Fact | Value |
| --- | --- |
| Source corpus | android |
| Relative path | 3d/characters/swampking/animations/swampking_pre_attack_03.bdae |
| Size bytes | 14604 |
| SHA-256 | 8eb4f6843d984d487a58f137d1b1635751344ea07a19704be9d8bb77031871b0 |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_1 |
| Source-linked reference rows | 1 |
| Actor dependency trails | 1 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 29dce45ee58d97014bc4 | anim | 1335 | pydata/animations_pyarray.bin[sequence=667,step=0].Anim | RESOLVED | DONE | PROVEN | AnimStep field schema in animation_tables.cpp | UNKNOWN |  |  |  |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| SwampKing | act_1 | CharacterProperties[358].AnimTable -> character_animations[70 ] -> pydata/animations_pyarray.bin[character=70].Attack[0] -> animation_sequences[669] -> pydata/animations_pyarray.bin[sequence=669,step=2].Anim -> animation_sequences[667] -> pydata/animations_pyarray.bin[sequence=667,step=0].Anim | animation_sequences[667] SwampKingAttack03 | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | 29dce45ee58d97014bc4 | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/swampking/animations/swampking_pre_attack_03.bdae |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
