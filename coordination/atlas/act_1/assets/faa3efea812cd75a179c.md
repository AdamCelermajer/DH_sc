# file000164.dat

Stable asset ID: `faa3efea812cd75a179c`. Source: `.local-inputs/claude-preview15/ipa/extracted/Payload/dungeonhunter2iPad.app/data/file000164.dat`.

| Fact | Value |
| --- | --- |
| Source corpus | ios-packed |
| Relative path | file000164.dat |
| Size bytes | 9336 |
| SHA-256 | a65f7f53f5b499f2d5cd9f4d5cdf8d28f26c430cf47694afadef07d66c9cb732 |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_1, act_2 |
| Source-linked reference rows | 1 |
| Actor dependency trails | 2 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 36a45d384875b922bcc0 | script | rene | .local-inputs/assets-extra/android/data/pydata/ai_pyarray.bin[section=0,row=50].Script | RESOLVED | DONE | PROVEN | AI.Script stem -> data/scripts/ai/<stem>.luac | PARTIAL | port/game-data/ai.cpp:22 |  | data/scripts/ai/rene.luac |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| WanderingEddie | act_2 | CharacterProperties[418].AI -> ai#group0[50 ] -> Script | ai#group0[50] Rene | RESOLVED | DONE | PROVEN | port/game-data/ai.cpp:22 | 36a45d384875b922bcc0 | RESOLVED | DONE | AI.Script stem -> data/scripts/ai/<stem>.luac | None | PARTIAL | port/game-data/ai.cpp:22 | .local-inputs/claude-preview15/ipa/extracted/Payload/dungeonhunter2iPad.app/data/file000164.dat |
| WanderingPriest | act_2 | CharacterProperties[419].AI -> ai#group0[50 ] -> Script | ai#group0[50] Rene | RESOLVED | DONE | PROVEN | port/game-data/ai.cpp:22 | 36a45d384875b922bcc0 | RESOLVED | DONE | AI.Script stem -> data/scripts/ai/<stem>.luac | None | PARTIAL | port/game-data/ai.cpp:22 | .local-inputs/claude-preview15/ipa/extracted/Payload/dungeonhunter2iPad.app/data/file000164.dat |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
