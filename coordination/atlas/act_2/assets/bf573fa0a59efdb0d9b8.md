# 3d/projectiles/projectile_dh2_monster_darkqueen_form01.bdae

Stable asset ID: `bf573fa0a59efdb0d9b8`. Source: `.local-inputs/assets-extra/android/data/3d/projectiles/projectile_dh2_monster_darkqueen_form01.bdae`.

| Fact | Value |
| --- | --- |
| Source corpus | android |
| Relative path | 3d/projectiles/projectile_dh2_monster_darkqueen_form01.bdae |
| Size bytes | 16964 |
| SHA-256 | 540e0f726c7c9a81a52c74b84f7173a177720bb66969fabe758a3d538131579a |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_2 |
| Source-linked reference rows | 2 |
| Actor dependency trails | 4 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 1bb6ee411938473057f2 | projectile | 19 | pydata/projectiles_pyarray.bin[row=40].Model | RESOLVED | DONE | PROVEN | ProjectileTable compiled member order + source process row descriptor; runtime-source-projectile-v1-report.json | UNKNOWN |  |  | data/3D/projectiles/projectile_dh2_monster_darkqueen_form01.bdae |
| 7bd780aafa1af027f3b5 | projectile | data/3D/projectiles/projectile_dh2_monster_darkqueen_form01.bdae | .local-inputs/assets-extra/android/data/pydata/projectiles_dictionary_pyarray.bin[section=0,row=19].Path | RESOLVED | DONE | PROVEN | port/game-data/data.cpp:35; field Path | PARTIAL | port/game-data/data.cpp:35 |  | data/3D/projectiles/projectile_dh2_monster_darkqueen_form01.bdae |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| DarkQueenForm1 | act_2 | CharacterProperties[62].RangeProjectileID -> projectiles#group0[40 ] -> pydata/projectiles_pyarray.bin[row=40].Model | projectiles#group0[40] darkqueen_form01 | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/projectiles_pyarraynames.bin | 1bb6ee411938473057f2 | RESOLVED | DONE | ProjectileTable compiled member order + source process row descriptor; runtime-source-projectile-v1-report.json | None | UNKNOWN |  | .local-inputs/assets-extra/android/data/3d/projectiles/projectile_dh2_monster_darkqueen_form01.bdae |
| DarkQueenForm1 | act_2 | CharacterProperties[62].RangeProjectileID -> projectiles#group0[40 ] -> pydata/projectiles_pyarray.bin[row=40].Model | projectiles#group0[40] darkqueen_form01 | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/projectiles_pyarraynames.bin | 1bb6ee411938473057f2 | RESOLVED | DONE | ProjectileTable compiled member order + source process row descriptor; runtime-source-projectile-v1-report.json | None | UNKNOWN |  | data/3D/projectiles/projectile_dh2_monster_darkqueen_form01.bdae |
| DarkQueenForm1 | act_2 | CharacterProperties[62].RangeProjectileID -> projectiles#group0[40 ] -> pydata/projectiles_pyarray.bin[row=40].Model -> projectiles_dictionary#group0[19] | projectiles_dictionary#group0[19] projectile_dh2_monster_darkqueen_form01 | RESOLVED | DONE | PROVEN | port/game-data/data.cpp:35 | 1bb6ee411938473057f2 | RESOLVED | DONE | ProjectileTable compiled member order + source process row descriptor; runtime-source-projectile-v1-report.json | None | UNKNOWN |  | data/3D/projectiles/projectile_dh2_monster_darkqueen_form01.bdae |
| DarkQueenForm1 | act_2 | CharacterProperties[62].RangeProjectileID -> projectiles#group0[40 ] -> pydata/projectiles_pyarray.bin[row=40].Model -> projectiles_dictionary#group0[19] -> Path | projectiles_dictionary#group0[19] projectile_dh2_monster_darkqueen_form01 | RESOLVED | DONE | PROVEN | port/game-data/data.cpp:35 | 7bd780aafa1af027f3b5 | RESOLVED | DONE | port/game-data/data.cpp:35; field Path | None | PARTIAL | port/game-data/data.cpp:35 | .local-inputs/assets-extra/android/data/3d/projectiles/projectile_dh2_monster_darkqueen_form01.bdae |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
