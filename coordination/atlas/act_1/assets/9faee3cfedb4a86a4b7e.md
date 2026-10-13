# sounds/sfx_cave_troll_mace_attack.wav

Stable asset ID: `9faee3cfedb4a86a4b7e`. Source: `.local-inputs/assets-extra/ios/data/sounds/sfx_cave_troll_mace_attack.wav`.

| Fact | Value |
| --- | --- |
| Source corpus | ios |
| Relative path | sounds/sfx_cave_troll_mace_attack.wav |
| Size bytes | 134672 |
| SHA-256 | 06d58a9d4780b008258e4ef925f5af99e2c04f46505405199deecd7d85d57cf2 |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_1 |
| Source-linked reference rows | 1 |
| Actor dependency trails | 4 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| c54a58a90c636255a7bf | sound | sfx_cave_troll_mace_attack | pydata/sdd_dungeon_hunter_2_iphone_pyarray.bin[row=327] | RESOLVED | DONE | PROVEN | row index in the 638-entry source SDD; event expands target bank members | UNKNOWN |  |  | sfx_cave_troll_mace_attack.wav |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Swamp_ActorTroll | act_1 | CharacterProperties[359].AnimTable -> character_animations[73 ] -> pydata/animations_pyarray.bin[character=73].Attack[0] -> animation_sequences[707] -> pydata/animations_pyarray.bin[sequence=707,step=0].Anim -> animation_sequences[704] -> pydata/animations_pyarray.bin[sequence=704,step=1].Sound -> exact SDD row -> audio resolver | animation_sequences[704] TrollOneHandedAttack01 | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | c54a58a90c636255a7bf | RESOLVED | DONE | row index in the 638-entry source SDD; event expands target bank members | None | UNKNOWN |  | .local-inputs/assets-extra/ios/data/sounds/sfx_cave_troll_mace_attack.wav |
| Swamp_ActorTroll | act_1 | CharacterProperties[359].AnimTable -> character_animations[73 ] -> pydata/animations_pyarray.bin[character=73].Attack[0] -> animation_sequences[707] -> pydata/animations_pyarray.bin[sequence=707,step=2].Anim -> animation_sequences[706] -> pydata/animations_pyarray.bin[sequence=706,step=1].Sound -> exact SDD row -> audio resolver | animation_sequences[706] TrollOneHandedAttack03 | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | c54a58a90c636255a7bf | RESOLVED | DONE | row index in the 638-entry source SDD; event expands target bank members | None | UNKNOWN |  | .local-inputs/assets-extra/ios/data/sounds/sfx_cave_troll_mace_attack.wav |
| Troll | act_1 | CharacterProperties[383].AnimTable -> character_animations[73 ] -> pydata/animations_pyarray.bin[character=73].Attack[0] -> animation_sequences[707] -> pydata/animations_pyarray.bin[sequence=707,step=0].Anim -> animation_sequences[704] -> pydata/animations_pyarray.bin[sequence=704,step=1].Sound -> exact SDD row -> audio resolver | animation_sequences[704] TrollOneHandedAttack01 | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | c54a58a90c636255a7bf | RESOLVED | DONE | row index in the 638-entry source SDD; event expands target bank members | None | UNKNOWN |  | .local-inputs/assets-extra/ios/data/sounds/sfx_cave_troll_mace_attack.wav |
| Troll | act_1 | CharacterProperties[383].AnimTable -> character_animations[73 ] -> pydata/animations_pyarray.bin[character=73].Attack[0] -> animation_sequences[707] -> pydata/animations_pyarray.bin[sequence=707,step=2].Anim -> animation_sequences[706] -> pydata/animations_pyarray.bin[sequence=706,step=1].Sound -> exact SDD row -> audio resolver | animation_sequences[706] TrollOneHandedAttack03 | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/animations_pyarraynames.bin | c54a58a90c636255a7bf | RESOLVED | DONE | row index in the 638-entry source SDD; event expands target bank members | None | UNKNOWN |  | .local-inputs/assets-extra/ios/data/sounds/sfx_cave_troll_mace_attack.wav |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
