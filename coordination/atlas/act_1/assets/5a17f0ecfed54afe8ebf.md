# 3d/characters/witch/witch.bdae

Stable asset ID: `5a17f0ecfed54afe8ebf`. Source: `.local-inputs/assets-extra/android/data/3d/characters/witch/witch.bdae`.

| Fact | Value |
| --- | --- |
| Source corpus | android |
| Relative path | 3d/characters/witch/witch.bdae |
| Size bytes | 81980 |
| SHA-256 | dc8b27e421e96b18d89962367459511f30cd01c8f3d6593cf6c0df3a1c462501 |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_1 |
| Source-linked reference rows | 1 |
| Actor dependency trails | 2 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 55f7c7c6c8d90425922e | model | data/3D/characters/witch/witch.bdae | .local-inputs/assets-extra/android/data/pydata/character_models_dictionary_pyarray.bin[section=0,row=113].Path | RESOLVED | DONE | PROVEN | port/game-data/data.cpp:35; field Path | PARTIAL | port/game-data/data.cpp:35 |  | data/3D/characters/witch/witch.bdae |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Witch | act_1 | CharacterProperties[435].ModelFile -> character_models_dictionary#group0[113 ] | character_models_dictionary#group0[113] witch | RESOLVED | DONE | PROVEN | port/game-data/data.cpp:35 | b71882daa62e768acfdd | RESOLVED | DONE | actor field link from exact source cell | None | None |  | data/3D/characters/witch/witch.bdae |
| Witch | act_1 | CharacterProperties[435].ModelFile -> character_models_dictionary#group0[113 ] -> Path | character_models_dictionary#group0[113] witch | RESOLVED | DONE | PROVEN | port/game-data/data.cpp:35 | 55f7c7c6c8d90425922e | RESOLVED | DONE | port/game-data/data.cpp:35; field Path | None | PARTIAL | port/game-data/data.cpp:35 | .local-inputs/assets-extra/android/data/3d/characters/witch/witch.bdae |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
