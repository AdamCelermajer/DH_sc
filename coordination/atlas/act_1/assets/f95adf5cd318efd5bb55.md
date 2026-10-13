# 3d/characters/witch/animations/witch_great_pushback_despawn.bdae

Stable asset ID: `f95adf5cd318efd5bb55`. Source: `.local-inputs/assets-extra/android/data/3d/characters/witch/animations/witch_great_pushback_despawn.bdae`.

| Fact | Value |
| --- | --- |
| Source corpus | android |
| Relative path | 3d/characters/witch/animations/witch_great_pushback_despawn.bdae |
| Size bytes | 6544 |
| SHA-256 | befef933efa08ceca7d865ec4db29b27559529a4c9f322f8f08912cbf5ec2722 |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_1 |
| Source-linked reference rows | 1 |
| Actor dependency trails | 1 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 85970e694d2d363b19ec | anim | 1411 | pydata/animations_pyarray.bin[sequence=759,step=0].Anim | RESOLVED | DONE | PROVEN | AnimStep field schema in animation_tables.cpp | UNKNOWN |  |  |  |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Witch | act_1 | CharacterProperties[435].AnimTable -> character_animations[76 ] -> pydata/animations_pyarray.bin[character=76].DespawnGreatKB[0] -> animation_sequences[759] -> pydata/animations_pyarray.bin[sequence=759,step=0].Anim | animation_sequences[759] Witch_DespawnGreatKB | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | 85970e694d2d363b19ec | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/witch/animations/witch_great_pushback_despawn.bdae |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
