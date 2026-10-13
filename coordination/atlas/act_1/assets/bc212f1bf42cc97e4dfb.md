# 3d/characters/lizardman/animations/lizardman_great_pushback_dying.bdae

Stable asset ID: `bc212f1bf42cc97e4dfb`. Source: `.local-inputs/assets-extra/android/data/3d/characters/lizardman/animations/lizardman_great_pushback_dying.bdae`.

| Fact | Value |
| --- | --- |
| Source corpus | android |
| Relative path | 3d/characters/lizardman/animations/lizardman_great_pushback_dying.bdae |
| Size bytes | 11376 |
| SHA-256 | 528850c2d96b704dbc14dfc55a7983b5a54643c3fc063b4bad1b6d065a8e95f2 |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_1 |
| Source-linked reference rows | 1 |
| Actor dependency trails | 1 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 76324f0da42e4616b277 | anim | 814 | pydata/animations_pyarray.bin[sequence=371,step=1].Anim | RESOLVED | DONE | PROVEN | AnimStep field schema in animation_tables.cpp | UNKNOWN |  |  |  |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Swamp_LizadMan_Type1 | act_1 | CharacterProperties[360].AnimTable -> character_animations[34 ] -> pydata/animations_pyarray.bin[character=34].DeadlyGreatKB[0] -> animation_sequences[371] -> pydata/animations_pyarray.bin[sequence=371,step=1].Anim | animation_sequences[371] LizardMan_DeadlyGreatKB | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | 76324f0da42e4616b277 | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/lizardman/animations/lizardman_great_pushback_dying.bdae |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
