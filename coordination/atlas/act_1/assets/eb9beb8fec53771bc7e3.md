# 3d/characters/lizardman/animations/lizardman_great_pushback_standup.bdae

Stable asset ID: `eb9beb8fec53771bc7e3`. Source: `.local-inputs/assets-extra/android/data/3d/characters/lizardman/animations/lizardman_great_pushback_standup.bdae`.

| Fact | Value |
| --- | --- |
| Source corpus | android |
| Relative path | 3d/characters/lizardman/animations/lizardman_great_pushback_standup.bdae |
| Size bytes | 21152 |
| SHA-256 | 9d9d07a66f0cd03e3d9e3b28f1f221e993effca5e5a0db0ac33baa204ea73133 |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_1 |
| Source-linked reference rows | 2 |
| Actor dependency trails | 4 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 22609d3c13a3f4abb119 | anim | 815 | pydata/animations_pyarray.bin[sequence=375,step=1].Anim | RESOLVED | DONE | PROVEN | AnimStep field schema in animation_tables.cpp | UNKNOWN |  |  |  |
| cd627c6e40dc9ff50f31 | anim | 815 | pydata/animations_pyarray.bin[sequence=86,step=0].Anim | RESOLVED | DONE | PROVEN | AnimStep field schema in animation_tables.cpp | UNKNOWN |  |  |  |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Swamp_LizadMan_Type1 | act_1 | CharacterProperties[360].AnimTable -> character_animations[34 ] -> pydata/animations_pyarray.bin[character=34].GreatKnockedBack[0] -> animation_sequences[375] -> pydata/animations_pyarray.bin[sequence=375,step=1].Anim | animation_sequences[375] LizardMan_GKnockedBack | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | 22609d3c13a3f4abb119 | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/lizardman/animations/lizardman_great_pushback_standup.bdae |
| Swamp_LizadMan_Type2 | act_1 | CharacterProperties[362].AnimTable -> character_animations[7 ] -> pydata/animations_pyarray.bin[character=7].GreatKnockedBack[0] -> animation_sequences[86] -> pydata/animations_pyarray.bin[sequence=86,step=0].Anim | animation_sequences[86] BigLizardMan_GKnockedBack | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | cd627c6e40dc9ff50f31 | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/lizardman/animations/lizardman_great_pushback_standup.bdae |
| Swamp_LizadMan_Type3 | act_1 | CharacterProperties[363].AnimTable -> character_animations[7 ] -> pydata/animations_pyarray.bin[character=7].GreatKnockedBack[0] -> animation_sequences[86] -> pydata/animations_pyarray.bin[sequence=86,step=0].Anim | animation_sequences[86] BigLizardMan_GKnockedBack | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | cd627c6e40dc9ff50f31 | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/lizardman/animations/lizardman_great_pushback_standup.bdae |
| Swamp_SnackMan | act_1 | CharacterProperties[374].AnimTable -> character_animations[7 ] -> pydata/animations_pyarray.bin[character=7].GreatKnockedBack[0] -> animation_sequences[86] -> pydata/animations_pyarray.bin[sequence=86,step=0].Anim | animation_sequences[86] BigLizardMan_GKnockedBack | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | cd627c6e40dc9ff50f31 | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/lizardman/animations/lizardman_great_pushback_standup.bdae |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
