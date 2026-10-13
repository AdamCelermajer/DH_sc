# 3d/characters/moth/animations/moth_template_anim.bdae

Stable asset ID: `c5fbbdce83ac909de869`. Source: `.local-inputs/assets-extra/android/data/3d/characters/moth/animations/moth_template_anim.bdae`.

| Fact | Value |
| --- | --- |
| Source corpus | android |
| Relative path | 3d/characters/moth/animations/moth_template_anim.bdae |
| Size bytes | 17748 |
| SHA-256 | 50aaaa2727c11bd28322d907b8e961f3ede6e0d9ae79520256c59278d181e48c |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_1 |
| Source-linked reference rows | 1 |
| Actor dependency trails | 1 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| cd5e1534f748b5879b64 | anim | 898 | pydata/animations_pyarray.bin[sequence=453,step=0].Anim | RESOLVED | DONE | PROVEN | AnimStep field schema in animation_tables.cpp | UNKNOWN |  |  |  |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Swamp_MovieMoth | act_1 | CharacterProperties[373].AnimTable -> character_animations[42 ] -> pydata/animations_pyarray.bin[character=42].Template[0] -> animation_sequences[453] -> pydata/animations_pyarray.bin[sequence=453,step=0].Anim | animation_sequences[453] Moth_Template | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | cd5e1534f748b5879b64 | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/moth/animations/moth_template_anim.bdae |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
