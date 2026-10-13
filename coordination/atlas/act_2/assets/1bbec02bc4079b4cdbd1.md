# file001102.dat

Stable asset ID: `1bbec02bc4079b4cdbd1`. Source: `.local-inputs/claude-preview15/ipa/extracted/Payload/dungeonhunter2iPad.app/data/file001102.dat`.

| Fact | Value |
| --- | --- |
| Source corpus | ios-packed |
| Relative path | file001102.dat |
| Size bytes | 16068 |
| SHA-256 | 2e7510fa6e0e7828ea21fd3b859c81205ded74f050e9330cb65f3d9589e62108 |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_2 |
| Source-linked reference rows | 1 |
| Actor dependency trails | 1 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 81f2600bf086e8c9e0f7 | animated effect | 79 | pydata/effects_pyarray.bin[set=15,step=0].File | RESOLVED | DONE | PROVEN | EffectsTables AnimatedEffectTable row + EffectDict index; effects_tables.cpp | UNKNOWN |  |  | data/3D/interface/skill_dh2_monster_dark_queen1stform_attack_melee.bdae |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| DarkQueenForm1 | act_2 | CharacterProperties[62].AnimTable -> character_animations[14 ] -> pydata/animations_pyarray.bin[character=14].Attack[0] -> animation_sequences[479] -> pydata/animations_pyarray.bin[sequence=479,step=0].FX -> effect_sets[15] -> pydata/effects_pyarray.bin[set=15,step=0].File | effect_sets[15] DarkQueen1AttackMelee | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/effects_pyarraynames.bin | 81f2600bf086e8c9e0f7 | RESOLVED | DONE | EffectsTables AnimatedEffectTable row + EffectDict index; effects_tables.cpp | None | UNKNOWN |  | .local-inputs/claude-preview15/ipa/extracted/Payload/dungeonhunter2iPad.app/data/file001102.dat |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
