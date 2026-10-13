# sounds/sfx_serpent_spit_water_explosion.wav

Stable asset ID: `e07fd7adcbbf7f8dade8`. Source: `.local-inputs/assets-extra/ios/data/sounds/sfx_serpent_spit_water_explosion.wav`.

| Fact | Value |
| --- | --- |
| Source corpus | ios |
| Relative path | sounds/sfx_serpent_spit_water_explosion.wav |
| Size bytes | 382508 |
| SHA-256 | 231bf843f1128a4def21cedd1bdf89f39974219736858edfb9716bdb79b38ccd |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_1 |
| Source-linked reference rows | 1 |
| Actor dependency trails | 1 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 9436f5976fe3d82bbaa3 | sound | sfx_serpent_spit_water_explosion | pydata/sdd_dungeon_hunter_2_iphone_pyarray.bin[row=524] | RESOLVED | DONE | PROVEN | row index in the 638-entry source SDD; event expands target bank members | UNKNOWN |  |  | sfx_serpent_spit_water_explosion.wav |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| SwampKing | act_1 | CharacterProperties[358].RangeProjectileID -> projectiles#group0[34 ] -> pydata/projectiles_pyarray.bin[row=34].ImpactSFX -> exact SDD row -> audio resolver | projectiles#group0[34] SwampkingSpit | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/projectiles_pyarraynames.bin | 9436f5976fe3d82bbaa3 | RESOLVED | DONE | row index in the 638-entry source SDD; event expands target bank members | None | UNKNOWN |  | .local-inputs/assets-extra/ios/data/sounds/sfx_serpent_spit_water_explosion.wav |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
