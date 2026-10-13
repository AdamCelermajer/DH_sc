# sounds/sfx_dark_queen_die.wav

Stable asset ID: `05beb05dadb4fef0420e`. Source: `.local-inputs/assets-extra/ios/data/sounds/sfx_dark_queen_die.wav`.

| Fact | Value |
| --- | --- |
| Source corpus | ios |
| Relative path | sounds/sfx_dark_queen_die.wav |
| Size bytes | 412884 |
| SHA-256 | 74b2aa0b4a10d07a549699d77e7bce1117ce5c9f28d4dd610d76fac071990f52 |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_2 |
| Source-linked reference rows | 1 |
| Actor dependency trails | 1 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| ef118e138c7a528814dd | sound | QueenKilled | pydata/sdd_dungeon_hunter_2_iphone_pyarray.bin[row=164] | RESOLVED | DONE | PROVEN | row index in the 638-entry source SDD; event expands target bank members | UNKNOWN |  |  | sfx_dark_queen_die.wav |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| DarkQueenForm1 | act_2 | CharacterProperties[62].AnimTable -> character_animations[14 ] -> pydata/animations_pyarray.bin[character=14].DeadlyGreatKB[0] -> animation_sequences[482] -> pydata/animations_pyarray.bin[sequence=482,step=0].Sound -> exact SDD row -> audio resolver | animation_sequences[482] Queen1_Died | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | ef118e138c7a528814dd | RESOLVED | DONE | row index in the 638-entry source SDD; event expands target bank members | None | UNKNOWN |  | .local-inputs/assets-extra/ios/data/sounds/sfx_dark_queen_die.wav |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
