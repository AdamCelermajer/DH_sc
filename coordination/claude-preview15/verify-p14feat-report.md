# P15 rc1 verification: Preview 14 feature streams (verifier: p14feat)

## Summary
Ran the P14 feature checks on the P15 rc1 EXE (SHA256 993CBC94...196420, verified) with the quiet runner (hidden desktop, silent, -Parallel 12; small follow-up batches for dependent reloads). 92 generator jobs plus 5 DROPS-verify jobs (the superseded first-pass reload runs are not counted); all exit 0 except two P13 baseline runs that reject `--bag-item` (expected). Stats, Skill Upgrade, Faery, Drops, Equip and Menu metadata behave as the stream reports say. Under P15, Stats spends are staged and persist only after Yes (intended, POINTS B049). Blocking finding: casting a skill from a saved profile fails on P15 and P14 ("Current skill assignment belongs to a different CharacterState/class than the preloaded bank"), while P13 casts the same save.

## Method and scratch
- Scratch and captures: `C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/claude-preview15/verify-p14feat/` (one folder per job: `run.log`, `out/*.ppm`, `frames/`, `run.args`).
- PNG copies: `.../verify-p14feat/png/` (full frames) and `.../verify-p14feat/crops/` (HUD and panel crops).
- Generator: `.../verify-p14feat/gen/` (`lib.sh` is the P14 generator retargeted to the P15 EXE; `stage1.sh`..`stage7.sh`, `menumeta.sh`, `after.sh`). Job files and summaries: `gen/jobs-*.json`, `gen/summary-*.json`.
- Runner: `DH_wt/p15int/port/windows-foundation/tools/quiet_run.ps1` only. DROPS template check: `DH_wt/drops/coordination/claude-preview14/DROPS-verify.ps1` run against the P15 EXE (10/10 PASS).
- Dependency order: reloads and after-save runs read saves written by earlier runs, so they run in later batches. A first stage-2 generation copied pre-run inputs into the reload jobs (r-*, h-hud-cur1, mm-after-*). Those jobs were regenerated after their sources ran (stage5, stage6, after.sh). Only the regenerated results are reported here.
- Baselines: P14 rc1 logs in `.local-inputs/claude-preview14/verify-features/` (cast and Faery comparisons); P13 EXE with P13 assets.

## SKILLS (Stats, Skill Upgrade, starter)
| # | Item | Result | Evidence |
|---|------|--------|----------|
| S1 | Stat + spends a point and recalculates HP/MP | PASS | `s-click-s3`: `Source stat training staged stat=0 points=3->2 value=10->11`. `s-end-s3`: `maxHP=165->169`; `png/s-end-s3.png` shows 165/169, Endurance 9, Points left 2. `s-en-s3`: `maxMP=27->31` (Energy) |
| S2 | Stat spend refused at 0 points | PASS | `s-click-zero`: `Character menu action diagnostic: Source stat training refused: no Stat_Points remain` |
| S3 | Persists | PASS under P15 rules (staged; Yes saves) | `y-yes-s3`: `Stats confirmation opened staged=2`, `Stats confirmed frame=150 points=1`; `y-reload-yes`: `stat=1`. `y-no-s3`: `Stats cancelled frame=150 points=3`; `y-reload-no`: `stat=3`. Without Yes nothing is saved: `r-click3`, `r-end`, `r-en` reload `stat=3` |
| S4 | Stat minus (remove) | NOT-RUN | No decrement control in the jobs. The No (cancel) path exercises the refund instead |
| S5 | Skill Upgrade rank 1->2 | PASS | `s-skill-up`: `Source skill training row=0 rank=1->2 points=2 potionCapacity=12`; reload `r-skill-up`: `skill=2` |
| S6 | Skill refused at 0 points | PASS | `s-skill-zero`: `NativeSkillsTrainSkill source admission rejected training` |
| S7 | Fresh-character starter grant | PASS | `s-starter-fresh`, `c-fresh-*`: fresh profile `stat=0 skill=1`. An existing save loads unchanged (`s-cast-up-k2` loads `skill=3`) |
| S8 | (Found) cast from a saved profile | FAIL (regression vs P13) | `x-save-cast-k2`, `x-save-cast-k2-menu`, `s-cast-up-k2`, `x-reload-upg-cast-k1/k2`: `Source skill key=N ... diagnostic=Current skill assignment belongs to a different CharacterState/class than the preloaded bank`. P13 EXE with the same save: `x-p13-save-cast-k2-menu` and `p13-cast-save-k2` cast BashDown. Fresh-path casts work: `c-fresh-base-k2`, `c-fresh-up-k2` (`skill=BashDown`). P14 rc1 shows the same failure (`verify-features/x-save-cast-k2`). The stream report says the cast effect was not verified in the EXE. |

Caveat: P14 expected Stats to persist after every click. P15 (branch p15/points, B049) stages spends per visit: Yes saves, No refunds, closing with staged spends asks for confirmation, and the one-per-visit gate is removed. The P15 behaviour is what was verified.

Verdict SKILLS: APPROVE WITH CAVEATS for the training items (S1-S3, S5-S7). S8 is a regression vs P13 and should be filed as a bug before the preview gate.

## FAERY
| # | Item | Result | Evidence |
|---|------|--------|----------|
| F1 | Faery tab renders | PASS | `f-view-zero`, `f-view-celest`: `Character menu Faery selected frame=60 via CharacterState provider`; `png/f-view-celest.png` (Celeste unlocked, slots 2-5 dark) |
| F2 | Swamp_Intro unlocks Celeste | PASS | `f-script-zero`: `Source command frame=40 script=Swamp_Intro index=156 kind=27`, `Source SetFaeryState slot=0 state=1 committed to CharacterState`; save byte 954: 0 -> 1 |
| F3 | Locked slot rejected | PASS | `f-locked-click`: `Character menu action diagnostic: Faery slot 2 is locked; selection rejected`; save byte-identical to the input (`cmp`) |
| F4 | Selection changes the key-4 icon | PASS | Gold = Celeste (`crops/h-hud-cur0-hud.png`). Primula selected (`f-select-hotty`: `Faery selection committed current=1 HUD key-4 refresh`; byte 950: 0 -> 1; `png/f-select-hotty.png`). After reload the icon is green (`crops/h-hud-cur1-hud2.png`). Rocky = blue (`crops/f-rocky-key4b-hud.png`) |
| F5 | Legacy save gets zero rows | PASS | `f-script-legacy`: `Legacy save Faery rows initialized to creation zeros`, then the script unlock commits. The P14 report expected `skipped ... (limitation)`; P14 rc1 logs the same as P15, so that expectation is stale |
| F6 | Rocky has no spell (key-4 log) | PASS | `f-rocky-key4b`: `Source Faery key=4 frame=100 slot=2 ... diagnostic=no spell implemented for Faery slot 2 (key 4 does nothing)`. `f-rocky-key4` (page still open) logs nothing; a harness state, not a defect |
| F7 | Celeste key 4 | NOTE | `f-celest-key4b`: `Current source Faery script is unsupported; no Hotty/Celest substitution is permitted`. Same as P14; not a P15 change |

Verdict FAERY: APPROVE WITH CAVEATS (F7 is a known unsupported path).

## DROPS
| # | Item | Result | Evidence |
|---|------|--------|----------|
| D1 | Seeded kill spawns a drawn drop | PASS | `d-kill` (seed 1234): `Source death reward frame=148 ... spawned=1 store=1`; `World item draws frame=148 count=1 store=1` |
| D2 | Target label | PASS | `d-kill`: `World item target frame=148 item=1 id=ClothGloves01`; `png/d-kill.png` shows the "Imbued Bracers" label on the target |
| D3 | Pickup by scripted `--pickup-frame` | PASS | `d-gold-potion`: `pickup frame=190 id=GoldStack01 ... gold=0->9`; `frame=200 id=Potion0 ... stacks=4->5`. `d-staff`: `Staff01 ... stacks=4->5` |
| D4 | Potion capacity | PASS | 12 bag potions (`d-potion-cap-12b`): `pickup frame=200 id=Potion0 outcome=5 picked=0 ... store=1` (outcome 5 = potion_capacity; the item stays). 11 bag potions (`d-potion-cap-11b`): `outcome=0 picked=1`, accepted up to 12 |
| D5 | Reload clears ground items | PASS | `d-reload`: `Content unloaded and reloaded at frame=300`; 0 `World item draws` after frame 300 |
| D6 | Inventory shows 9 gold and Potions: 1 | PASS | `d-inventory`, `png/d-inventory.png` (Gold 9, Potions: 1, "Auto-equip All" button) |
| D7 | DROPS-verify.ps1 (template check) | PASS 10/10 | Run against the P15 EXE; all 10 lines PASS (`verify-p14feat/drops-verify/`) |
| D8 | Automatic pickup (`reason=automatic`) | NOT-RUN | No automatic pickup line in any run (same open risk as P14) |

Verdict DROPS: APPROVE WITH CAVEATS (no automatic pickup evidence; drop motion not checked visually).

## EQUIP
| # | Item | Result | Evidence |
|---|------|--------|----------|
| E1 | Per-slot Auto-Equip | PASS (log), with caveat | `e-auto-slot`: `Equipment AutoEquip slot=3 -> slot0=StartingSuit/... slot3=StartingBoots/equipped/StartingBoots ...`. The template already had StartingBoots equipped, so the outcome is unchanged. The choice rule was not checked against the original |
| E2 | ALL button | PASS | `e-auto-all`: `Equipment AutoEquip ALL -> ...`; `png/d-inventory.png` shows the "Auto-equip All" button |
| E3 | Transmute gold | PASS | `e-transmute-feet`: `Equipment Transmute instance=bag-0-ClothBoots01 item=ClothBoots01 gold 0 -> 7 amount=7` (`png/e-pre-boots.png`: Value 7). `e-transmute-sword`: `bag-1-Longsword03 ... gold 0 -> 55 amount=55` |
| E4 | Drop publishes a real world item at the player's feet | PASS | `e-drop-feet-a`: `Equipment Drop instance=bag-0-ClothBoots01 item=ClothBoots01 -> world item`; `World item draws frame=330 count=1 store=1`. Stage 7 `e-drop-hud` (menu closed at 360): `World item target frame=360 item=1 id=ClothBoots01 position=1090.75,-212.202,258`, which is the player's start position from `--position`. `png/e-drop-hud.png` shows the "Imbued Boots" label at the player's feet. The bag entry is gone (`png/e-drop-feet-a.png`). Drop sword: `e-drop-sword` draws count=1 |
| E5 | B045 second-row click | PASS | `png/e-pre-boots.png`: one click on the second bag row (`315:112:220`) selects "Imbued Boots"; Transmute on that row works (`e-transmute-feet`) |
| E6 | Equipped hides DROP, unequipped shows it | PASS | `png/e-pre-boots.png`: unequipped Imbued Boots shows Transmute and Drop. `png/e-equipped-details.png`: equipped Useless Blade shows UNEQUIP/EQUIP and no Drop |
| E7 | Bag-item args | PASS | All P15 e-* runs accept `--bag-item`. P13 baseline rejects it: `Foundation error: Unknown argument: --bag-item` (`p13-transmute-feet`, `p13-drop-feet`: NOT-RUN baseline, expected) |

Verdict EQUIP: APPROVE WITH CAVEATS (E1 choice rule not checked against the original).

## MENUMETA
| # | Item | Result | Evidence |
|---|------|--------|----------|
| M1 | Legacy v3 save: Act/location/difficulty/last save blank | PASS | `png/mm-legacy-knight-f120.png` (QA Warrior, LEVEL 1, rows blank, red X); `crops/mm-legacy-mage-panel.png` (Mage, blank) |
| M2 | Save point stamps the save | PASS | `mm-k2play`: `Saved live checkpoint frame=40 HP=165.098 ...` (schema header version 4, 1008 bytes); `mm-m2play`: `Saved live checkpoint frame=40` (985 bytes) |
| M3 | Slot panel after save point | PASS | `png/mm-after-knight-f120.png`: Act 1, The Boglands, Difficulty Normal, Last Save 10/10 18:52. `png/mm-after-mage-f120.png`: same for Mage |
| M4 | Hard/Heroic difficulty text | NOT-RUN | Only Normal was exercised |

Verdict MENUMETA: APPROVE (Normal only; Hard/Heroic not run).

## Verdicts
- SKILLS: APPROVE WITH CAVEATS (training items). Blocking bug to file: S8, saved-profile skill cast regression vs P13.
- FAERY: APPROVE WITH CAVEATS (Celeste key-4 unsupported path, as before).
- DROPS: APPROVE WITH CAVEATS (automatic pickup not observed).
- EQUIP: APPROVE WITH CAVEATS (auto-equip rule not checked against the original).
- MENUMETA: APPROVE.

## Open items for the owners
1. S8: skill cast from a saved profile fails with "belongs to a different CharacterState/class than the preloaded bank"; P13 does not fail. Repro: `x-save-cast-k2` (input `.local-inputs/claude-preview14/skills/prof/zero.save` and `gameplay.save`, frames 230, `--skill-key-frame 170:2`).
2. Stats persistence is now staged (P15 B049). Anything that expected per-click persistence needs the Yes step.
3. The P14 FAERY report's legacy expectation is stale (legacy rows are initialized, not skipped).
