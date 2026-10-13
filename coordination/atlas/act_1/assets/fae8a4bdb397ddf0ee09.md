# sounds/sfx_lizardman_die.wav

Stable asset ID: `fae8a4bdb397ddf0ee09`. Source: `.local-inputs/assets-extra/ios/data/sounds/sfx_lizardman_die.wav`.

| Fact | Value |
| --- | --- |
| Source corpus | ios |
| Relative path | sounds/sfx_lizardman_die.wav |
| Size bytes | 64780 |
| SHA-256 | db36c0823a01ad3a185fe66ba6a57b7c779158377eefe0372aa8c457a2bb1047 |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_1 |
| Source-linked reference rows | 1 |
| Actor dependency trails | 8 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 905c59f73dd249133065 | sound | sfx_lizardman_die | pydata/sdd_dungeon_hunter_2_iphone_pyarray.bin[row=476] | RESOLVED | DONE | PROVEN | row index in the 638-entry source SDD; event expands target bank members | UNKNOWN |  |  | sfx_lizardman_die.wav |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Swamp_LizadMan_Type1 | act_1 | CharacterProperties[360].AnimTable -> character_animations[34 ] -> pydata/animations_pyarray.bin[character=34].DeadlyGreatKB[0] -> animation_sequences[371] -> pydata/animations_pyarray.bin[sequence=371,step=1].Sound -> exact SDD row -> audio resolver | animation_sequences[371] LizardMan_DeadlyGreatKB | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | 905c59f73dd249133065 | RESOLVED | DONE | row index in the 638-entry source SDD; event expands target bank members | None | UNKNOWN |  | .local-inputs/assets-extra/ios/data/sounds/sfx_lizardman_die.wav |
| Swamp_LizadMan_Type1 | act_1 | CharacterProperties[360].AnimTable -> character_animations[34 ] -> pydata/animations_pyarray.bin[character=34].Died[0] -> animation_sequences[374] -> pydata/animations_pyarray.bin[sequence=374,step=0].Sound -> exact SDD row -> audio resolver | animation_sequences[374] LizardMan_Died | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | 905c59f73dd249133065 | RESOLVED | DONE | row index in the 638-entry source SDD; event expands target bank members | None | UNKNOWN |  | .local-inputs/assets-extra/ios/data/sounds/sfx_lizardman_die.wav |
| Swamp_LizadMan_Type2 | act_1 | CharacterProperties[362].AnimTable -> character_animations[7 ] -> pydata/animations_pyarray.bin[character=7].DeadlyGreatKB[0] -> animation_sequences[82] -> pydata/animations_pyarray.bin[sequence=82,step=0].Sound -> exact SDD row -> audio resolver | animation_sequences[82] BigLizardMan_DeadlyGreatKB | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | 905c59f73dd249133065 | RESOLVED | DONE | row index in the 638-entry source SDD; event expands target bank members | None | UNKNOWN |  | .local-inputs/assets-extra/ios/data/sounds/sfx_lizardman_die.wav |
| Swamp_LizadMan_Type2 | act_1 | CharacterProperties[362].AnimTable -> character_animations[7 ] -> pydata/animations_pyarray.bin[character=7].Died[0] -> animation_sequences[85] -> pydata/animations_pyarray.bin[sequence=85,step=0].Sound -> exact SDD row -> audio resolver | animation_sequences[85] BigLizardMan_Died | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | 905c59f73dd249133065 | RESOLVED | DONE | row index in the 638-entry source SDD; event expands target bank members | None | UNKNOWN |  | .local-inputs/assets-extra/ios/data/sounds/sfx_lizardman_die.wav |
| Swamp_LizadMan_Type3 | act_1 | CharacterProperties[363].AnimTable -> character_animations[7 ] -> pydata/animations_pyarray.bin[character=7].DeadlyGreatKB[0] -> animation_sequences[82] -> pydata/animations_pyarray.bin[sequence=82,step=0].Sound -> exact SDD row -> audio resolver | animation_sequences[82] BigLizardMan_DeadlyGreatKB | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | 905c59f73dd249133065 | RESOLVED | DONE | row index in the 638-entry source SDD; event expands target bank members | None | UNKNOWN |  | .local-inputs/assets-extra/ios/data/sounds/sfx_lizardman_die.wav |
| Swamp_LizadMan_Type3 | act_1 | CharacterProperties[363].AnimTable -> character_animations[7 ] -> pydata/animations_pyarray.bin[character=7].Died[0] -> animation_sequences[85] -> pydata/animations_pyarray.bin[sequence=85,step=0].Sound -> exact SDD row -> audio resolver | animation_sequences[85] BigLizardMan_Died | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | 905c59f73dd249133065 | RESOLVED | DONE | row index in the 638-entry source SDD; event expands target bank members | None | UNKNOWN |  | .local-inputs/assets-extra/ios/data/sounds/sfx_lizardman_die.wav |
| Swamp_SnackMan | act_1 | CharacterProperties[374].AnimTable -> character_animations[7 ] -> pydata/animations_pyarray.bin[character=7].DeadlyGreatKB[0] -> animation_sequences[82] -> pydata/animations_pyarray.bin[sequence=82,step=0].Sound -> exact SDD row -> audio resolver | animation_sequences[82] BigLizardMan_DeadlyGreatKB | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | 905c59f73dd249133065 | RESOLVED | DONE | row index in the 638-entry source SDD; event expands target bank members | None | UNKNOWN |  | .local-inputs/assets-extra/ios/data/sounds/sfx_lizardman_die.wav |
| Swamp_SnackMan | act_1 | CharacterProperties[374].AnimTable -> character_animations[7 ] -> pydata/animations_pyarray.bin[character=7].Died[0] -> animation_sequences[85] -> pydata/animations_pyarray.bin[sequence=85,step=0].Sound -> exact SDD row -> audio resolver | animation_sequences[85] BigLizardMan_Died | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | 905c59f73dd249133065 | RESOLVED | DONE | row index in the 638-entry source SDD; event expands target bank members | None | UNKNOWN |  | .local-inputs/assets-extra/ios/data/sounds/sfx_lizardman_die.wav |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
