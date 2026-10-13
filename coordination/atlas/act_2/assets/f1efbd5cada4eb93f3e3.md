# file001322.dat

Stable asset ID: `f1efbd5cada4eb93f3e3`. Source: `.local-inputs/claude-preview15/ipa/extracted/Payload/dungeonhunter2iPad.app/data/file001322.dat`.

| Fact | Value |
| --- | --- |
| Source corpus | ios-packed |
| Relative path | file001322.dat |
| Size bytes | 54264 |
| SHA-256 | 0abe54a300f2a8f35a6dda2b86186183e637b8dc841f9ceea805dc0c18caa849 |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_2 |
| Source-linked reference rows | 1 |
| Actor dependency trails | 2 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| d5ebd3b697e305160825 | model | data/3D/characters/npcs/castle_maid.bdae | .local-inputs/assets-extra/android/data/pydata/character_models_dictionary_pyarray.bin[section=0,row=10].Path | RESOLVED | DONE | PROVEN | port/game-data/data.cpp:35; field Path | PARTIAL | port/game-data/data.cpp:35 |  | data/3D/characters/npcs/castle_maid.bdae |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| GCV_P3_003 | act_2 | CharacterProperties[197].ModelFile -> character_models_dictionary#group0[10 ] -> Path | character_models_dictionary#group0[10] castle_maid | RESOLVED | DONE | PROVEN | port/game-data/data.cpp:35 | d5ebd3b697e305160825 | RESOLVED | DONE | port/game-data/data.cpp:35; field Path | None | PARTIAL | port/game-data/data.cpp:35 | .local-inputs/claude-preview15/ipa/extracted/Payload/dungeonhunter2iPad.app/data/file001322.dat |
| GCV_P4_002 | act_2 | CharacterProperties[201].ModelFile -> character_models_dictionary#group0[10 ] -> Path | character_models_dictionary#group0[10] castle_maid | RESOLVED | DONE | PROVEN | port/game-data/data.cpp:35 | d5ebd3b697e305160825 | RESOLVED | DONE | port/game-data/data.cpp:35; field Path | None | PARTIAL | port/game-data/data.cpp:35 | .local-inputs/claude-preview15/ipa/extracted/Payload/dungeonhunter2iPad.app/data/file001322.dat |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
