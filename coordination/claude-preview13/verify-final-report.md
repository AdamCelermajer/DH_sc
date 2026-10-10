# Preview 13 FINAL verification (verifier `final`)

EXE under test: `.local-inputs/windows-source-clock-v19-preview-13-rc2/dh-foundation.exe`, SHA256 `418FDB56...B7ECE` (matches brief), git HEAD `e685ed13`.
Baseline: `.local-inputs/windows-source-clock-v19-preview-12/dh-foundation.exe`, SHA256 `1D43942E...2672`.
Method: all runs through `port/windows-foundation/tools/quiet_run.ps1` (hidden desktop, `DH_AUDIO_SILENT=1` for rc2). Main batch = 41 jobs, `-Parallel 12` (`verify-final/jobs-main.json`, `summary-main.json`); B039 alone with `-Parallel 1` (`jobs-b039.json`, `summary-b039.json`). Each job in `verify-final/runs/<name>-<R|P>/` with `run.args`, `run.log`, own saves and PPM/PNG captures. Montages in `verify-final/montage/`. Generator: `verify-final/scripts/gen_jobs.py`. All 41 main jobs exited 0 except `b044-rogue-swamp-P` (exit 1, expected P12 failure). No timeouts. No user process touched.

## Results

| # | Check | Verdict | Evidence |
|---|---|---|---|
| 1 | Smoke (Swamp 130 frames, HUD) | PASS | `smoke-R` and `smoke-P` exit 0. HUD in `montage/smoke-R-P.png`: portrait, HP red, MP blue, key circles 1-5 with potion, same in both. |
| 2 | B037 Space + skill mid-swing, cut, resume, release | PASS (no regression); brief's "rejected in P12" NOT reproduced | `b037-hold50` (Space 30:400, BashDown key 2 at 50): both builds log `Source skill key=2 ... skill=BashDown MP=21.25 generation=1` (R and P identical). Combo boundaries identical in R and P: gen1 30/43, hit 46 `continued=1`, gen2 at 132. `b037-hold36`, `b037-rel8`, `b037-rel20`: same combo frames in both. P12 accepts mid-swing skill with Space held here, so the "rejected in P12" premise is not reproduced. |
| 3 | B004/B029 target marker after Post and release | PASS for rc2; NOT A/B-discriminating | `montage/b004-R-P.png` (frame 200, after BashDown Post ~131 and Space release at 44): rc2 and P12 both show target ring, Bogwomp name and enemy HP bar. The brief's "gone in P12" is not reproduced. |
| 4 | B038 XP bar grows after kills 1 and 2 | PASS for rc2 (visual); P12 also grows in this capture | Kill logs rc2: `kxp-200-R` `dead=1 xp=8` at frame 143; `kxp-300-R` second kill `xp=16` at 240. `montage/xpbar-R130-200-300-P130-300.png`: rc2 empty at 130, partial fill at 200, larger fill at 300; P12 empty at 130 and visibly filled at 300 (eye estimate, no pixel measurement). |
| 5 | B041 three combo swings + BashDown dispatch + blue trail | PASS for rc2; P12 identical | `b041-f260-R`: `sequence=470 step=1 frame=43 dispatched=1`, `471` at 74, `472` at 106 (`dispatched=1` each). BashDown: `sequence=347 step=0 frame=36/50 dispatched=1` next to `Source skill ... BashDown`. Blue trail visible at frames 50 and 112 in `montage/b041-R-50-112.png`; P12 shows the same trail in `montage/b041-P-50-112.png`. |
| 6 | B040 return-to-menu stop | PASS | `b040-return-R`: `Level music transition: kind=start track=SwampHubAmbientMusic ... ordinal=211` (exactly 1), `Pause menu confirmed main menu frame=80`, `Level music transition: kind=return-stop track=SwampHubAmbientMusic fadeMs=1000`, `Rendered frames=80; clean shutdown`, exit 0. `Audio host is unavailable` count 0. One diagnostic line `Level music diagnostic: ... (retrying)` appears before the start. P12 not run for this path. |
| 7 | B039 real-time underruns (~35 s) | PASS | `b039-rt-R` (alone, `-Parallel 1`): `WinMM pump: underruns=0 refills=3437 renderedSeconds=36.6613 wallSeconds=36.6019 maxGapMs=32.67 gapsOver40ms=0`, `Rendered frames=2100; clean shutdown`, exit 0. |
| 8 | B043 Rogue Faery/Celest then potion; B044 Rogue Swamp kill | PASS (rc2); P12 fails both as expected | B043 rc2: `Source Faery key=4 frame=30 MP=30.25`, `Source potion key=5 frame=60 result=1 consumed=1 quantity=5->4 MPraw=7744->10304`. P12: `result=0 ... PropertyAdd could not synchronize live actor vital`. B044 rc2: `Damage frame=175 ... dead=1`, `Source death reward ... xp=8`, second kill frame 289 `xp=16`; P12 exit 1 `Selected source phase has no damage marker: RoguePlayerBase/attack_offhand`. (The brief says Celest; the log labels key 4 as Faery.) |
| 9a | B042 Details: torso (equipped) DROP hidden, plates restored | PASS | `montage/eq-torso-R-P.png`: rc2 shows plate panels and no DROP; P12 shows DROP and flat brown fill. Known open differences remain (grey/orange tints, see L-report) and were not re-judged. |
| 9b | B042 Details: unequipped item DROP shown | **FAIL** | `montage/eq-bag-R-P.png` (Useless Blade, EQUIP button visible, so unequipped): rc2 has NO DROP button; P12 shows DROP. The fix hides DROP for unequipped items too, contrary to the authored/reference rule (Feet t=372 unequipped shows DROP). |
| 9c | B020 skill lock glyphs | PASS | `montage/skills-R-P.png`: rc2 locks on cells 1,2,3,5,6,7, none on 0 or 4; P12 none. Matches reference pattern. |
| 10 | No hit pushes the hero | PASS (screen-space, clean hits only) | `hit-13-R` vs `hit-17-R` hero crop (`montage/hit-R-13-17-crop.png`): hero feet, ring and sword in the same screen position across the lizard hit at frame 14; damage "8" appears at 17. Limit: 4-frame gap, screen-space only, no push-bearing hit tested. |

## Verdict: REJECT rc2 as Preview 13 (until the DROP gate is fixed)

Blocking: check 9b. rc2 hides DROP on unequipped items, a regression against P12 that the B042 fix must not introduce. Re-run `eq-bag-R` after the fix (`eq-bag` template uses save `b016-knight-bag-longsword03.save`).

Non-blocking findings:
- B037: no P12-vs-rc2 combo regression seen. The brief's "rejected in P12" premise is not reproduced; P12 accepts the mid-swing skill too. Preserved behaviour matches P12 frame for frame.
- B004 and B041: P12 also shows the marker and trail in these runs, so these checks prove rc2 works but do not show a fix over P12.
- B038: rc2 grows after both kills; P12 also grows in this capture, so the bug is not reproduced by this run. Pixel measurement not done.
- Damage rolls differ between builds from the first player hit (frame 46: rc2 17.5 vs P12 14.5, identical in release runs too), and the lizard hit at frame 44 differs (rc2 0 vs P12 7.7). Not explained by the skill; worth a look by the implementer.
- B040 P12 A/B not run.

## Not run / limits
- B043 HP restore not observable (HP full). Mage path not rerun here.
- B044 dual-dagger path not run.
- B042 unequipped Details also not checked against the reference grey/orange fills.
- B038 pixel widths not measured.
