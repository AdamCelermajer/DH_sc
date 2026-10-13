# 3d/characters/swampking/animations/swampking_swim_small.bdae

Stable asset ID: `a1ad1ef4ac1439e4c9af`. Source: `.local-inputs/assets-extra/android/data/3d/characters/swampking/animations/swampking_swim_small.bdae`.

| Fact | Value |
| --- | --- |
| Source corpus | android |
| Relative path | 3d/characters/swampking/animations/swampking_swim_small.bdae |
| Size bytes | 11896 |
| SHA-256 | ab78e49d9c2e97f9e610321ab33bbb7a4e26650424c31e3439851736beae6c88 |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_1 |
| Source-linked reference rows | 1 |
| Actor dependency trails | 1 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| ba7359b9b4a95ee0ae03 | anim | 1338 | pydata/animations_pyarray.bin[sequence=678,step=1].Anim | RESOLVED | DONE | PROVEN | AnimStep field schema in animation_tables.cpp | UNKNOWN |  |  |  |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| SwampKing | act_1 | CharacterProperties[358].AnimTable -> character_animations[70 ] -> pydata/animations_pyarray.bin[character=70].DespawnGreatKB[0] -> animation_sequences[678] -> pydata/animations_pyarray.bin[sequence=678,step=1].Anim | animation_sequences[678] SwampKing_Walk | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | ba7359b9b4a95ee0ae03 | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/swampking/animations/swampking_swim_small.bdae |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
