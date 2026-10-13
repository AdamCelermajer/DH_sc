# 3d/characters/lizardman/animations/big_lizardman_pre_stunned.bdae

Stable asset ID: `fddffcbe16a885f5e0dc`. Source: `.local-inputs/assets-extra/android/data/3d/characters/lizardman/animations/big_lizardman_pre_stunned.bdae`.

| Fact | Value |
| --- | --- |
| Source corpus | android |
| Relative path | 3d/characters/lizardman/animations/big_lizardman_pre_stunned.bdae |
| Size bytes | 7144 |
| SHA-256 | f2544a81de2f3b51c9de94fbc6dfc06dc074912569585cb1b33693289085e8fb |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_1 |
| Source-linked reference rows | 1 |
| Actor dependency trails | 3 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 91af6957831b01e47535 | anim | 38 | pydata/animations_pyarray.bin[sequence=92,step=0].Anim | RESOLVED | DONE | PROVEN | AnimStep field schema in animation_tables.cpp | UNKNOWN |  |  |  |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Swamp_LizadMan_Type2 | act_1 | CharacterProperties[362].AnimTable -> character_animations[7 ] -> pydata/animations_pyarray.bin[character=7].Stunned[0] -> animation_sequences[92] -> pydata/animations_pyarray.bin[sequence=92,step=0].Anim | animation_sequences[92] BigLizardMan_Stunned | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | 91af6957831b01e47535 | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/lizardman/animations/big_lizardman_pre_stunned.bdae |
| Swamp_LizadMan_Type3 | act_1 | CharacterProperties[363].AnimTable -> character_animations[7 ] -> pydata/animations_pyarray.bin[character=7].Stunned[0] -> animation_sequences[92] -> pydata/animations_pyarray.bin[sequence=92,step=0].Anim | animation_sequences[92] BigLizardMan_Stunned | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | 91af6957831b01e47535 | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/lizardman/animations/big_lizardman_pre_stunned.bdae |
| Swamp_SnackMan | act_1 | CharacterProperties[374].AnimTable -> character_animations[7 ] -> pydata/animations_pyarray.bin[character=7].Stunned[0] -> animation_sequences[92] -> pydata/animations_pyarray.bin[sequence=92,step=0].Anim | animation_sequences[92] BigLizardMan_Stunned | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | 91af6957831b01e47535 | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/lizardman/animations/big_lizardman_pre_stunned.bdae |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
