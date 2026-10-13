# 3d/characters/slime/animations/slime_idle_02.bdae

Stable asset ID: `7a242c05f3fcfacbda6b`. Source: `.local-inputs/assets-extra/android/data/3d/characters/slime/animations/slime_idle_02.bdae`.

| Fact | Value |
| --- | --- |
| Source corpus | android |
| Relative path | 3d/characters/slime/animations/slime_idle_02.bdae |
| Size bytes | 5728 |
| SHA-256 | fdf12aecf3b38dc20de496e2fcb2d90db88512b794b2ee05ad95842c9cd41baf |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_1 |
| Source-linked reference rows | 1 |
| Actor dependency trails | 1 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 0a6413313ea90fa21db1 | anim | 1259 | pydata/animations_pyarray.bin[sequence=613,step=5].Anim | RESOLVED | DONE | PROVEN | AnimStep field schema in animation_tables.cpp | UNKNOWN |  |  |  |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Slime | act_1 | CharacterProperties[349].AnimTable -> character_animations[64 ] -> pydata/animations_pyarray.bin[character=64].Idle[0] -> animation_sequences[613] -> pydata/animations_pyarray.bin[sequence=613,step=5].Anim | animation_sequences[613] Slime_Idle | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | 0a6413313ea90fa21db1 | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/slime/animations/slime_idle_02.bdae |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
