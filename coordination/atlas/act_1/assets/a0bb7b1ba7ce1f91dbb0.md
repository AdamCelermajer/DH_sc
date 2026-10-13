# file004834.dat

Stable asset ID: `a0bb7b1ba7ce1f91dbb0`. Source: `.local-inputs/claude-preview15/ipa/extracted/Payload/dungeonhunter2iPad.app/data/file004834.dat`.

| Fact | Value |
| --- | --- |
| Source corpus | ios-packed |
| Relative path | file004834.dat |
| Size bytes | 18856 |
| SHA-256 | 50dce9ec47104315374fbf6c9441eabc6b403f1b4e5cf43eeb920f8ddc59775d |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_1 |
| Source-linked reference rows | 2 |
| Actor dependency trails | 2 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 7abdc9e79019f71da961 | projectile | data/3D/projectiles/projectile_dh2_bogwitch.bdae | .local-inputs/assets-extra/android/data/pydata/projectiles_dictionary_pyarray.bin[section=0,row=15].Path | RESOLVED | DONE | PROVEN | port/game-data/data.cpp:35; field Path | PARTIAL | port/game-data/data.cpp:35 |  | data/3D/projectiles/projectile_dh2_bogwitch.bdae |
| e910ea97de01a8842e01 | projectile | 15 | pydata/projectiles_pyarray.bin[row=39].Model | RESOLVED | DONE | PROVEN | ProjectileTable compiled member order + source process row descriptor; runtime-source-projectile-v1-report.json | UNKNOWN |  |  | data/3D/projectiles/projectile_dh2_bogwitch.bdae |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Witch | act_1 | CharacterProperties[435].RangeProjectileID -> projectiles#group0[39 ] -> pydata/projectiles_pyarray.bin[row=39].Model | projectiles#group0[39] bogwitch | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/projectiles_pyarraynames.bin | e910ea97de01a8842e01 | RESOLVED | DONE | ProjectileTable compiled member order + source process row descriptor; runtime-source-projectile-v1-report.json | None | UNKNOWN |  | .local-inputs/claude-preview15/ipa/extracted/Payload/dungeonhunter2iPad.app/data/file004834.dat |
| Witch | act_1 | CharacterProperties[435].RangeProjectileID -> projectiles#group0[39 ] -> pydata/projectiles_pyarray.bin[row=39].Model -> projectiles_dictionary#group0[15] -> Path | projectiles_dictionary#group0[15] projectile_dh2_bogwitch | RESOLVED | DONE | PROVEN | port/game-data/data.cpp:35 | 7abdc9e79019f71da961 | RESOLVED | DONE | port/game-data/data.cpp:35; field Path | None | PARTIAL | port/game-data/data.cpp:35 | .local-inputs/claude-preview15/ipa/extracted/Payload/dungeonhunter2iPad.app/data/file004834.dat |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
