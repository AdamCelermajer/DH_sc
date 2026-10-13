# 3d/characters/lizardman/animations/big_lizardman_spawn_jump.bdae

Stable asset ID: `ab19b5e71ae4da9e59ce`. Source: `.local-inputs/assets-extra/android/data/3d/characters/lizardman/animations/big_lizardman_spawn_jump.bdae`.

| Fact | Value |
| --- | --- |
| Source corpus | android |
| Relative path | 3d/characters/lizardman/animations/big_lizardman_spawn_jump.bdae |
| Size bytes | 13248 |
| SHA-256 | 9b9d7ea59d4318983282eef945ca6e9c1658e674b85f46f83513fa0f7edfb7ae |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_1 |
| Source-linked reference rows | 1 |
| Actor dependency trails | 3 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| d62baaeed4d9d344fb4b | anim | 40 | pydata/animations_pyarray.bin[sequence=91,step=0].Anim | RESOLVED | DONE | PROVEN | AnimStep field schema in animation_tables.cpp | UNKNOWN |  |  |  |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Swamp_LizadMan_Type2 | act_1 | CharacterProperties[362].AnimTable -> character_animations[7 ] -> pydata/animations_pyarray.bin[character=7].Spawn[0] -> animation_sequences[91] -> pydata/animations_pyarray.bin[sequence=91,step=0].Anim | animation_sequences[91] BigLizardMan_Spawn | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | d62baaeed4d9d344fb4b | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/lizardman/animations/big_lizardman_spawn_jump.bdae |
| Swamp_LizadMan_Type3 | act_1 | CharacterProperties[363].AnimTable -> character_animations[7 ] -> pydata/animations_pyarray.bin[character=7].Spawn[0] -> animation_sequences[91] -> pydata/animations_pyarray.bin[sequence=91,step=0].Anim | animation_sequences[91] BigLizardMan_Spawn | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | d62baaeed4d9d344fb4b | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/lizardman/animations/big_lizardman_spawn_jump.bdae |
| Swamp_SnackMan | act_1 | CharacterProperties[374].AnimTable -> character_animations[7 ] -> pydata/animations_pyarray.bin[character=7].Spawn[0] -> animation_sequences[91] -> pydata/animations_pyarray.bin[sequence=91,step=0].Anim | animation_sequences[91] BigLizardMan_Spawn | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | d62baaeed4d9d344fb4b | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/lizardman/animations/big_lizardman_spawn_jump.bdae |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
