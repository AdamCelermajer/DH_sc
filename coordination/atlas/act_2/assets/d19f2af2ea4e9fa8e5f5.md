# 3d/characters/faeries/animations/faeries_celeste_idle.bdae

Stable asset ID: `d19f2af2ea4e9fa8e5f5`. Source: `.local-inputs/assets-extra/android/data/3d/characters/faeries/animations/faeries_celeste_idle.bdae`.

| Fact | Value |
| --- | --- |
| Source corpus | android |
| Relative path | 3d/characters/faeries/animations/faeries_celeste_idle.bdae |
| Size bytes | 27104 |
| SHA-256 | f993bfb319bf50c2166773f70abd27dc834e6792bd2ad9dda90efaf16311ec10 |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_1, act_2 |
| Source-linked reference rows | 1 |
| Actor dependency trails | 2 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 9bcf673b3f976df97646 | anim | 686 | pydata/animations_pyarray.bin[sequence=196,step=0].Anim | RESOLVED | DONE | PROVEN | AnimStep field schema in animation_tables.cpp | UNKNOWN |  |  |  |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| DefaultFairy | act_2 | CharacterProperties[97].AnimTable -> character_animations[23 ] -> pydata/animations_pyarray.bin[character=23].Idle[0] -> animation_sequences[196] -> pydata/animations_pyarray.bin[sequence=196,step=0].Anim | animation_sequences[196] Faeries_Idle | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | 9bcf673b3f976df97646 | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/faeries/animations/faeries_celeste_idle.bdae |
| DummyLightningFaery | act_2 | CharacterProperties[114].AnimTable -> character_animations[23 ] -> pydata/animations_pyarray.bin[character=23].Idle[0] -> animation_sequences[196] -> pydata/animations_pyarray.bin[sequence=196,step=0].Anim | animation_sequences[196] Faeries_Idle | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | 9bcf673b3f976df97646 | RESOLVED | DONE | AnimStep field schema in animation_tables.cpp | None | UNKNOWN |  | data/3D/characters/faeries/animations/faeries_celeste_idle.bdae |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
