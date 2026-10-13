# Shared assets and actors

> Shared rows require the same stable source target to be referenced from at least two inferred acts. These memberships remain INFERENCE until act boundaries are confirmed.

## Actors named by spawn commands in multiple acts

| Actor | CharacterProperties row | Acts | Status |
| --- | --- | --- | --- |
| DW_Cherub_RE | 55 | ["act_2", "act_4"] | INFERENCE |
| DefaultFairy | 97 | ["act_1", "act_2", "act_3", "act_4", "act_5"] | INFERENCE |
| DummyEarthFaery | 112 | ["act_2", "act_3"] | INFERENCE |
| DummyLightningFaery | 114 | ["act_1", "act_2", "act_3"] | INFERENCE |
| EarthTemple_SmallPlant | 133 | ["act_2", "act_5"] | INFERENCE |
| Edward | 138 | ["act_2", "act_5"] | INFERENCE |
| Epic_merchant_notherlands | 149 | ["act_1", "act_4"] | INFERENCE |
| IcyHub_StillStatue | 248 | ["act_2", "act_4"] | INFERENCE |
| WanderingEddie | 418 | ["act_1", "act_2", "act_3", "act_4"] | INFERENCE |
| WanderingPriest | 419 | ["act_1", "act_2", "act_4"] | INFERENCE |

Resolved cross-act asset references: see `../data/act-gaps.json`; shared asset join status remains TODO pending reference review.
