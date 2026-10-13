# 3d/characters/slime/animations/slime_template_anim.bdae

Stable asset ID: `ac4bbef57fe70c914c5e`. Source: `.local-inputs/assets-extra/android/data/3d/characters/slime/animations/slime_template_anim.bdae`.

| Fact | Value |
| --- | --- |
| Source corpus | android |
| Relative path | 3d/characters/slime/animations/slime_template_anim.bdae |
| Size bytes | 5744 |
| SHA-256 | c39e568ebb138f16cbbae64abf9cb1b5f0a71f9f2deaefc38b24ce4734f9b503 |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_1 |
| Source-linked reference rows | 1 |
| Actor dependency trails | 1 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| ab9616fe51ac3a9cb4a0 | anim | 1267 | pydata/animations_pyarray.bin[sequence=619,step=0].Anim | RESOLVED | DONE | PROVEN | AnimStep field schema in animation_tables.cpp | UNKNOWN |  |  |  |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Slime | act_1 | CharacterProperties[349].AnimTable -> character_animations[64 ] -> pydata/animations_pyarray.bin[character=64].Template[0] -> animation_sequences[619] -> pydata/animations_pyarray.bin[sequence=619,step=0].Anim | animation_sequences[619] Slime_Template | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | ab9616fe51ac3a9cb4a0 | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/slime/animations/slime_template_anim.bdae |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
