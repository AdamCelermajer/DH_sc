# ABCHECK: Preview 16 rc3 vs 15.2 combat A/B, moth hypothesis

Scope: read-only analysis of `.local-inputs/claude-preview16/verify-p16c/jobs/ab-{152,rc3}-s{1234,99,1,2,7,42}/run.log`, the Swamp level and MGP data in `windows-source-clock-v19-preview-16-rc3/assets/original-cache`, the reference video `dh2_video_research/video/part1_z_Zky7qQdYs.mp4`, and the p16 branch reports. No code changed. Scratch scripts were in /tmp; frame grids are in `.local-inputs/claude-preview16/abcheck/`.

## Verdict: EXPECTED CHANGE (moth hypothesis refuted)

The extra rc3 moths are not authored moths and they never attack the player. The player HP gap in seeds 1234, 99 and 2 is fully explained by one respawned Lizardman (`_prim_LizTemplate_03`) hitting the player from about frame 1740. That respawn is a Preview 16 despawn/respawn feature that 15.2 does not have. Open item: the respawn gate is not verified against the original (see section 5).

## 1. Actors spawned, 15.2 vs rc3 (all seeds have identical args)

Authored MothTemplate actors are the same in both builds. Only the rc3 spawn-pool actors are extra.

| Actor (log `Actor final` lines) | 15.2 | rc3 |
|---|---|---|
| `_prim_MothTemplate_01/02/05/06/08/10` (6 authored moths, identical ids and positions) | yes | yes |
| `_prim_MothTemplate_13` | "Population notice ... MothTemplate_13: Condition policy returned unknown; actor not instantiated" | same line |
| `_prim_Monster_FakeMoth_01/02` | "Condition policy returned unknown; actor not instantiated" | same |
| `spawn-pool/Swamp_Moth_Minions/0` and `/1` | absent | present, `position=0,0,0`, `Lifecycle final ... state=17 enabled=0 physical=0 collisions=0` |

Total `Actor final` lines: 25 in 15.2, 27 in rc3, so the two extra are the pool slots. The same two-slot pattern appears in all six rc3 seeds.

The 15.2 log quotes no "not admitted" or "skipped" line about Swamp_Moth_Minions. Its only spawn-related lines are the authored `Spawn point requires caller-owned spawning policy` notices, which both builds print. The "not admitted" wording exists in the p16/containers2 report (`CONTAINERS2-report.md`: "Summon of Swamp_Moth_Minions is refused in live (no actor profile)"), not in the 15.2 log. The rc3 lines that add the minions are:
- `SPAWN profile derived from tables: Swamp_Moth_Minions animationTable=44 states=8`
- `SPAWN pool profile=Swamp_Moth_Minions slots=2 level_raw=256`
- `SPAWN melee bindings derived from tables: Swamp_Moth_Minions`

These run without `--spawn-test` (the rc3 run.args does not contain it), so the PROFILES report's claim that derivation is inert without that flag does not hold for rc3. The trigger is the container declarations (14 containers, 8 with script `moth_spawn_container`), which 15.2 never logs at all.

The pool slots never become active. They stay disabled, and no `Container opened` or `Summon` line appears in any combat run.

## 2. Moth damage on the player

- Moth-attributed `Damage` events on the player: **0** in all 12 runs (6 seeds x 2 builds). No moth has a non-zero final target in any run.
- HP reconciliation (165.098 start minus the sum of `removed` on the player):

| Seed | 15.2 sum / final HP | rc3 sum / final HP | Extra rc3 hits | Extra sum | HP diff |
|---|---|---|---|---|---|
| 1234 | 41.4258 / 123.672 | 72.7070 / 92.391 | 7 (LizTemplate_03 at 1740..2086) | 31.281 | 31.281 |
| 99 | 24.3320 / 140.766 | 63.6523 / 101.445 | 7 (LizTemplate_03 at 1741..2087) | 39.321 | 39.321 |
| 2 | 15.2383 / 149.859 | 64.1523 / 100.945 | 7 (LizTemplate_03 at 1740..2086) | 48.914 | 48.914 |
| 1 | 25.0039 / 140.094 | 25.0039 / 140.094 | 0 | 0 | 0 |
| 7 | 25.6602 / 139.438 | 25.6602 / 139.438 | 0 | 0 | 0 |
| 42 | 42.1211 / 122.977 | 42.1211 / 122.977 | 0 | 0 | 0 |

In the three seeds with no extra hits, the HP is identical to 15.2. Only the lizard's hits differ, not the moths'.

## 3. Mechanism: lizard respawn (DESPAWN feature)

- 15.2 logs zero `DESPAWN` lines. rc3 logs `DESPAWN tracked`, `DESPAWN death end`, `delay expired` and, in seeds 1234/99/2, `DESPAWN respawn scheduled actor=...LizTemplate_03 after_ms=20000` followed by `DESPAWN respawned ... frame=1584/1585`. The respawned lizard then attacks from frame 1740.
- Death rewards are identical in both builds (xp 8 and 16, same victims, same frames within a few frames).
- rc3 `respawn_ms` for LizTemplate_03 is 20000 in seeds 1234, 99 and 2, and 0 in seeds 1, 7 and 42. The source is `1000 * (CharProperty 11 >> 8)` (`main.cpp` in p16/despawn2). Why this value differs across seeds is not isolated.

## 4. Reference video (original)

- Video: `part1_z_Zky7qQdYs.mp4`, 640x360, 1331.6 s, Android DH2 1.0.3.
- At 262-282 s (1 fps grid `p1-262-282-grid.png`), the Swamp fight shows "Bog Moth" nameplates with HP bars (for example at 274-277 s) on large winged moths engaging the hero. The hero kills them (EXP and gold popups). Moths fly over the hero around 270-273 s. I did not see a red damage number on the hero at this resolution, so moth hits on the hero are inferred from the engagement, not directly observed.
- At 408-417 s, the Hungry Prisoner says "Stranger, are you hungry?" then "Plenty of moths around if you want a bite", and a "New Quest: Kill 8 Bog Moths." card appears (`p1-408-418-quest.png`). `sidequests.english` also contains "Bogwalker's Delight 2 / Kill 8 Bog Moths."
- Level data (`obj_*_brdwalk_*.mgp`, `001_swamp.mlx`): the original places **7** `MothTemplate` declarations (01, 02, 05, 06, 08, 10 unconditional; 13 gated by `activate_cond="Invalid"`), **2** `Monster_FakeMoth` movie props (`auto_spawn=0`, used by the MothIntro cutscene), and **8** `Swamp_Normal_DestructibleBarrel` containers with a 25% summon of Swamp_Moth_Minions (`CONTAINERS2-report.md`, `moth_spawn_container`). The 9th barrel is `Swamp_NoMoth`.
- So the authored moths and the minion pool are both original content. The moths in the start area are real.

## 5. Open items

1. Respawn gate: the port's `GetRespawnDelay` port omits the original `CanRespawn` group permission (noted in the despawn2 header: "not modelled"). The respawn itself may therefore differ from the original, and the 20000 vs 0 split across seeds is unexplained. This must be checked before sign-off.
2. The rc3 `Context button` line at frame 30 (targeting the first monster) is new in rc3 and was not traced. Both builds still show the same first hits at frames 14, 43, 131.

## 6. Requested suppression runs

No flag suppresses authored moth declarations. The option list in `main.cpp` (p16/despawn2) has `--population-templates`, `--spawn-declared`, `--condition-active/inactive`, but no population filter. No runs were made. They would be moot anyway, because no moth engages the player in any run.

## Corrections to the REJECT report

- "Extra moth-minion actors ... unexplained": they are two inert pool slots (position 0,0,0, disabled), not moths in the fight.
- "Hit counts equal": the player-to-lizard hits are equal (7 each in seed 1234). The difference is in lizard-to-player hits (5 vs 12 in seed 1234).
- The HP difference cause is the respawned LizTemplate_03, not an unexplained combat change.
