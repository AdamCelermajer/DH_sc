# 3d/interface/spell_dh2_hurt_lightning.bdae

Stable asset ID: `1a5b26e9aed8da1ec2a0`. Source: `.local-inputs/assets-extra/android/data/3d/interface/spell_dh2_hurt_lightning.bdae`.

| Fact | Value |
| --- | --- |
| Source corpus | android |
| Relative path | 3d/interface/spell_dh2_hurt_lightning.bdae |
| Size bytes | 6436 |
| SHA-256 | 5599f7f6e9ad117285eb315c575799b60abb26ac6918789f172c0cb127e0eea4 |
| Inventory status | ORPHAN |
| Work status | DONE |
| Act uses | act_1, act_2 |
| Source-linked reference rows | 1 |
| Actor dependency trails | 2 |
| All inventory referrer IDs | 0 |


## Source-linked reference and resolution rows

| ID | Kind | Name | Site | Status | Work status | Confidence | Rule | Port status | Implementation | Reason | Resolved source path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| e10b8b4fa7179fa9e7b4 | animated effect | 209 | pydata/effects_pyarray.bin[set=243,step=0].File | RESOLVED | DONE | PROVEN | EffectsTables AnimatedEffectTable row + EffectDict index; effects_tables.cpp | UNKNOWN |  |  | data/3D/interface/spell_dh2_hurt_lightning.bdae |


Resolver metadata above is copied from the reference ledger. A resolved source path records a data link; it does not assert runtime use.

## Actor table dependency trails

| Actor | Act | Dependency path | Exact source row | Row status | Work status | Confidence | Schema evidence | Reference ID | Reference status | Reference work status | Resolver rule | Reason | Port status | Implementation | Resolved asset path |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| WanderingEddie | act_2 | CharacterProperties[418].RangeProjectileID -> projectiles#group0[22 ] -> pydata/projectiles_pyarray.bin[row=22].ImpactFX -> effect_sets[243] -> pydata/effects_pyarray.bin[set=243,step=0].File | effect_sets[243] spell_lightning_hurt | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/effects_pyarraynames.bin | e10b8b4fa7179fa9e7b4 | RESOLVED | DONE | EffectsTables AnimatedEffectTable row + EffectDict index; effects_tables.cpp | None | UNKNOWN |  | .local-inputs/assets-extra/android/data/3d/interface/spell_dh2_hurt_lightning.bdae |
| WanderingEddie | act_2 | CharacterProperties[418].RangeProjectileID -> projectiles#group0[22 ] -> pydata/projectiles_pyarray.bin[row=22].ImpactFX -> effect_sets[243] -> pydata/effects_pyarray.bin[set=243,step=0].File | effect_sets[243] spell_lightning_hurt | RESOLVED | DONE | PROVEN | .local-inputs/assets-extra/android/data/pydata/effects_pyarraynames.bin | e10b8b4fa7179fa9e7b4 | RESOLVED | DONE | EffectsTables AnimatedEffectTable row + EffectDict index; effects_tables.cpp | None | UNKNOWN |  | data/3D/interface/spell_dh2_hurt_lightning.bdae |


These trails follow exact actor field links and decoded table cells. Unresolved sound rows remain in the act gap ledger; no sound payload is inferred.
