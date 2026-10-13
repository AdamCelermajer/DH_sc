# 3d/characters/lizardman/animations/lizardman_pre_attack_01.bdae

Stable asset ID: `fbed3d4e318ff8f1fddc`. Source: `.local-inputs/assets-extra/android/data/3d/characters/lizardman/animations/lizardman_pre_attack_01.bdae`.

| Fact | Value |
| --- | --- |
| Source corpus | android |
| Relative path | 3d/characters/lizardman/animations/lizardman_pre_attack_01.bdae |
| Size bytes | 6696 |
| SHA-256 | 25b0cd37c2156298affcd337036ed987a60737b0de5b6a2e5cde1b3d0e9e61b1 |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_1 |
| Source-linked reference rows | 4 |
| Actor dependency trails | 4 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 2ae17021372fde4a1f87 | anim | 822 | pydata/animations_pyarray.bin[sequence=369,step=0].Anim | RESOLVED | DONE | PROVEN | AnimStep field schema in animation_tables.cpp | UNKNOWN |  |  |  |
| 6c362e010d89a8379ebf | anim | 822 | pydata/animations_pyarray.bin[sequence=367,step=0].Anim | RESOLVED | DONE | PROVEN | AnimStep field schema in animation_tables.cpp | UNKNOWN |  |  |  |
| c02bb1dccc100dbe32d9 | anim | 822 | pydata/animations_pyarray.bin[sequence=366,step=0].Anim | RESOLVED | DONE | PROVEN | AnimStep field schema in animation_tables.cpp | UNKNOWN |  |  |  |
| c7ae8714444c650e58e9 | anim | 822 | pydata/animations_pyarray.bin[sequence=368,step=0].Anim | RESOLVED | DONE | PROVEN | AnimStep field schema in animation_tables.cpp | UNKNOWN |  |  |  |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Swamp_LizadMan_Type1 | act_1 | CharacterProperties[360].AnimTable -> character_animations[34 ] -> pydata/animations_pyarray.bin[character=34].Attack[0] -> animation_sequences[370] -> pydata/animations_pyarray.bin[sequence=370,step=0].Anim -> animation_sequences[366] -> pydata/animations_pyarray.bin[sequence=366,step=0].Anim | animation_sequences[366] LizardManOneHandedAttack01 | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | c02bb1dccc100dbe32d9 | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/lizardman/animations/lizardman_pre_attack_01.bdae |
| Swamp_LizadMan_Type1 | act_1 | CharacterProperties[360].AnimTable -> character_animations[34 ] -> pydata/animations_pyarray.bin[character=34].Attack[0] -> animation_sequences[370] -> pydata/animations_pyarray.bin[sequence=370,step=1].Anim -> animation_sequences[367] -> pydata/animations_pyarray.bin[sequence=367,step=0].Anim | animation_sequences[367] LizardManOneHandedAttack01NoSound | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | 6c362e010d89a8379ebf | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/lizardman/animations/lizardman_pre_attack_01.bdae |
| Swamp_LizadMan_Type1 | act_1 | CharacterProperties[360].AnimTable -> character_animations[34 ] -> pydata/animations_pyarray.bin[character=34].Attack[0] -> animation_sequences[370] -> pydata/animations_pyarray.bin[sequence=370,step=3].Anim -> animation_sequences[368] -> pydata/animations_pyarray.bin[sequence=368,step=0].Anim | animation_sequences[368] LizardManOneHandedAttack02 | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | c7ae8714444c650e58e9 | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/lizardman/animations/lizardman_pre_attack_01.bdae |
| Swamp_LizadMan_Type1 | act_1 | CharacterProperties[360].AnimTable -> character_animations[34 ] -> pydata/animations_pyarray.bin[character=34].Attack[0] -> animation_sequences[370] -> pydata/animations_pyarray.bin[sequence=370,step=4].Anim -> animation_sequences[369] -> pydata/animations_pyarray.bin[sequence=369,step=0].Anim | animation_sequences[369] LizardManOneHandedAttack02NoSound | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | 2ae17021372fde4a1f87 | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/lizardman/animations/lizardman_pre_attack_01.bdae |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
