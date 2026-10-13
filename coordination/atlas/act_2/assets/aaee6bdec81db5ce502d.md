# 3d/characters/lizardman/old_man.bdae

Stable asset ID: `aaee6bdec81db5ce502d`. Source: `.local-inputs/assets-extra/android/data/3d/characters/lizardman/old_man.bdae`.

| Fact | Value |
| --- | --- |
| Source corpus | android |
| Relative path | 3d/characters/lizardman/old_man.bdae |
| Size bytes | 59648 |
| SHA-256 | 9833409e724dc076a94ae0e9a4b09d4c108b07b018447f6228737af754bd3786 |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_2 |
| Source-linked reference rows | 1 |
| Actor dependency trails | 4 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 78825685ac5f849b0612 | model | data/3D/characters/lizardman/old_man.bdae | .local-inputs/assets-extra/android/data/pydata/character_models_dictionary_pyarray.bin[section=0,row=72].Path | RESOLVED | DONE | PROVEN | port/game-data/data.cpp:35; field Path | PARTIAL | port/game-data/data.cpp:35 |  | data/3D/characters/lizardman/old_man.bdae |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| HellMan_DW_final | act_2 | CharacterProperties[209].ModelFile -> character_models_dictionary#group0[72 ] | character_models_dictionary#group0[72] old_man | RESOLVED | DONE | PROVEN | port/game-data/data.cpp:35 | 0850c79be16fc5848865 | RESOLVED | DONE | actor field link from exact source cell | None | None |  | data/3D/characters/lizardman/old_man.bdae |
| HellMan_DW_final | act_2 | CharacterProperties[209].ModelFile -> character_models_dictionary#group0[72 ] -> Path | character_models_dictionary#group0[72] old_man | RESOLVED | DONE | PROVEN | port/game-data/data.cpp:35 | 78825685ac5f849b0612 | RESOLVED | DONE | port/game-data/data.cpp:35; field Path | None | PARTIAL | port/game-data/data.cpp:35 | .local-inputs/assets-extra/android/data/3d/characters/lizardman/old_man.bdae |
| HellMan_quest_1 | act_2 | CharacterProperties[211].ModelFile -> character_models_dictionary#group0[72 ] | character_models_dictionary#group0[72] old_man | RESOLVED | DONE | PROVEN | port/game-data/data.cpp:35 | 03a9c616197289f66961 | RESOLVED | DONE | actor field link from exact source cell | None | None |  | data/3D/characters/lizardman/old_man.bdae |
| HellMan_quest_1 | act_2 | CharacterProperties[211].ModelFile -> character_models_dictionary#group0[72 ] -> Path | character_models_dictionary#group0[72] old_man | RESOLVED | DONE | PROVEN | port/game-data/data.cpp:35 | 78825685ac5f849b0612 | RESOLVED | DONE | port/game-data/data.cpp:35; field Path | None | PARTIAL | port/game-data/data.cpp:35 | .local-inputs/assets-extra/android/data/3d/characters/lizardman/old_man.bdae |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
