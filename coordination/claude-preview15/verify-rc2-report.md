# Preview 15 rc2 verification (verifier rc2)

EXE under test: `.local-inputs/windows-source-clock-v19-preview-15-rc2/dh-foundation.exe`, SHA256 `5C5CFD8D...B9E88BC` (matches brief). A/B: `.local-inputs/windows-source-clock-v19-preview-15-rc1/` and, for the DROP check, `preview-14-rc1`. Startup flow excluded (in progress). No verifier code changes.

Method: all runs through `port/windows-foundation/tools/quiet_run.ps1` (hidden desktop, silent, `DH_AUDIO_SILENT`). Batch 1: 53 jobs, `-Parallel 12`. Batch 2 (reloads, dependent on batch 1): 4 jobs. DROP A/B: 3 jobs, `-Parallel 3`. HUD key-4 follow-up: 1 job. B039 real-time: 1 job, `-Parallel 1`, started hidden. Scratch and outputs: `C:/Users/adamc/Desktop/workspace/DH_wt/p15int/.local-inputs/claude-preview15/verify-rc2/` (per-job `run.args`, `run.log`, PPM captures, `png/`, `summary-*.json`). Generator: `verify-rc2/gen/build-rc2.js` (reuses the job templates of verify-final, p14feat, p15fixes, castfix, faerysound and hudbtn; saves are copied from pristine inputs into job folders). Converter: `gen/ppm2png.js`. Exit codes: all rc2 jobs exit 0; no timeouts. The only Foundation errors are the two rc1 Hotty baselines (expected).

Note: batch 2 was first generated before batch 1 had written its saves, so the first reload results (stat 2) read a fresh default save. Those results were discarded; batch 2 was regenerated and re-run (reported values below are from the re-run).

## Results

| # | Item | Result | Evidence | Verdict |
|---|---|---|---|---|
| 1a | B052 keyed cast from saved profile (x-save-cast-k2 style, 230 frames, `--skill-key-frame 170:2`) | PASS | rc2: `Source skill key=2 slot=0 frame=170 ... skill=BashDown MP=21.25 diagnostic=` (empty); lifecycle phase 1 (170) -> 2 (184) -> 3 (197); `Rendered frames=230; clean shutdown`. MP 27.25 -> 21.25 | APPROVE |
| 1b | B052 A/B vs rc1 (same save) | rc1 FAIL (baseline, reproduces bug) | rc1: `diagnostic=Current skill assignment belongs to a different CharacterState/class than the preloaded bank`, no BashDown, MP stays 27.25 | n/a (baseline) |
| 1c | B052 legacy P13-era save (`claude-preview13/p/b016/profiles/b016-knight-bag-longsword02.save`) | PASS | Same as 1a: empty diagnostic, BashDown phases 1-3, MP 27.25 -> 21.25 | APPROVE |
| 1d | B052 fresh, no save (absent paths) | PASS | Empty diagnostic, BashDown, MP 27.25 -> 21.25 | APPROVE |
| 2a | Hotty slot 4, key 4, far (targets=0) and near (targets=3) | PASS | rc2 far: `Source Faery key=4 frame=70 slot=4 ... MP=30.25 diagnostic=`, `Faery cast sound uid=479 ... targets=0 ... status=dispatched`, `Hotty source state7 do_spell applied its ordered same-session result loop` (frame 93); near: `targets=3 ... dispatched`, hits=3. Exit 0, no Foundation error. rc1 far and near: exit 1, `Foundation error: Live combat: Source Post cannot complete before its authored retained Use event` | APPROVE |
| 2b | Celest key 4 unchanged | PASS | Knight and Rogue Celest far/near on rc2 exit 0 and dispatch the cast sound (targets 0 / 3, do_spell completes). Rogue rc1 A/B shows the same dispatch and do_spell lines (checked for far and near). `StaticBallKilled` logs `failed` (asset `data/sounds/sfx_static_ball_killed.wav` missing), as before | APPROVE WITH CAVEATS (missing asset is pre-existing) |
| 2c | Cast sound lines with and without an enemy | PASS | `Faery cast sound ... targets=0` (far) and `targets=3` (near), both `status=dispatched` (rc2) | APPROVE |
| 3a | Smoke: Swamp 130 frames | PASS | `Rendered frames=130; clean shutdown`, exit 0. `png/smoke130.png`: portrait, HP/MP bars, pause, keys 1-5, Faery icon, Potion | APPROVE |
| 3b | B037 skill mid-swing (Space held) | PASS | `b037-hold50`: `Source skill key=2 slot=0 frame=50 ... skill=BashDown MP=21.25 diagnostic=`; `b037-hold36`: same at frame 36. Exit 0 | APPROVE |
| 3c | B004 target marker after skill | PASS | `png/b004-mid.png` (frame 200): target ring, "Bogwomp" name and HP bar on the enemy | APPROVE |
| 3d | B038 XP bar grows on kills | PASS | Kill 1: `Source death reward ... xpRecipients=1 xp=8` (frame 143); kill 2: `xp=16` (frame 240). `png/xp200-crop.png` vs `xp300-crop.png`: green fill grows from a sliver to about 15 px. No pixel measurement against the formula | APPROVE WITH CAVEATS (visual only) |
| 3e | B043 Rogue Celest (Faery key 4) then potion | PASS | `Source Faery key=4 frame=30 slot=0 ... MP=30.25`; `Source potion key=5 frame=60 result=1 consumed=1 quantity=5->4 MPraw=7744->10304` | APPROVE |
| 3f | B044 Rogue Swamp kill | PASS | `Source death reward frame=175 ... xp=8`, `Damage frame=175 ... dead=1`; second kill frame 289 `xp=16`. Exit 0 | APPROVE |
| 3g | B040 return-to-menu stop | PASS | `Level music transition: kind=start ... ordinal=211`, `Pause menu confirmed main menu frame=80`, `Level music transition: kind=return-stop track=SwampHubAmbientMusic fadeMs=1000`, `Rendered frames=80; clean shutdown`; 0 `Audio host is unavailable` lines. Clicks injected with `--menu-release` (no OS clicks) | APPROVE |
| 3h | B039 35 s real-time, 0 underruns | PASS | `summary-audio.json` exit 0. `WinMM pump: underruns=0 refills=3651 renderedSeconds=38.944 wallSeconds=38.877 maxGapMs=32.37 gapsOver40ms=0`; 2100 frames; clean shutdown | APPROVE WITH CAVEATS (audibility needs a listener) |
| 3i | No hit pushes the hero | PASS | Clean lizard hit at frame 14 (`Damage ... removed=8.35 dead=0`). `png/hit13-hero.png` vs `hit17-hero.png`: hero feet, sword and target ring in the same screen position | APPROVE WITH CAVEATS (screen-space, 4-frame gap, clean hits only, no 0x98 push hit) |
| 3j | B042 DROP on an unequipped bag sword (P13 finding 9b) | FAIL (pre-existing, not a P15 regression) | `png/eq-bag-mid.png`: unequipped Useless Blade shows EQUIP, no DROP. Same as P13 rc2. P12 (`png/p13-eq-bag-P.png`) shows DROP. P14 rc1 on the same job (`png/drop-sword-p14rc1.png`): no DROP. Unequipped Imbued Boots on rc2 (`png/drop-boots-rc2.png`): Transmute and DROP shown | REJECT for this item (open since P13; root must fix or waive) |
| 4a | B051 avatar idle after attack-then-equip (Knight) | PASS | `png/b051-attack-equip.png` and `b051-idle-equip.png`: Knight upright, sword at idle, Auto-equip All page; `Character menu Equipment selected frame=36` in both | APPROVE WITH CAVEATS (Rogue/Mage not run) |
| 4b | B046 rail click, arrows, wrap | PASS | `png/b046-after-icon1.png`: rail icon 1 -> title "Right hand". `png/b046-after-up.png`: Up from Torso wraps -> title "Potions", stack shows "5 Potion" (quantity digit kept) | APPROVE WITH CAVEATS (3 of 9 icons run; dim art gap known) |
| 4c | B049 two points, confirm box, Yes/No, Escape, reload | PASS | `two-f105` staged `points=2->1`; `refuse-3`: `Source stat training refused: no Stat_Points remain`; `yes-open`: `Stats confirmation opened staged=2`; `yes-final`: `Stats confirmed frame=150 points=0`; `no-final`: `Stats cancelled frame=150 points=2`; `esc-dismiss`: `dismissed frame=125 via Escape`; `nostage-close`: no box. Reload: yes `stat=0`, no `stat=2`, fixture `stat=2` | APPROVE WITH CAVEATS (skill-side confirm not implemented, as before) |
| 4d | B047 drop motion stop | PARTIAL / NOT-RUN for speed and stop | `World item target frame=148 item=1 id=ClothGloves01 position=-6957.32,935.157,255` (matches the branch line). Captures at 149, 165, 180, 215 differ across the whole frame (scene animates), so item speed and stop point cannot be isolated; no per-frame trace in rc2 | APPROVE WITH CAVEATS (speed and stop unverified) |
| 4e | B048 dropsound (asset_missing lines) | NOT-RUN | Not in the spot-check list; rc1 verified it earlier (p15fixes) | APPROVE WITH CAVEATS (not re-run) |
| 4f | I025 HUD art (keys 1-5, Faery, Potion) | PASS (art); cooldown overlay NOT-RUN | `png/i025-k2-f120-hud.png`: key 2 shows the BashDown art, key 4 Faery, key 5 Potion, original-style buttons. Cooldown overlay needs a code edit, not done | APPROVE WITH CAVEATS (cooldown not run) |
| 4g | I026 level-up presentation, art missing | PASS | b200: `Level up presentation frame=148 ... fx=135:level_up:failed(Exact authored FX resource not found ... level_up.bdae)`, exit 0, no crash. ctrl: no presentation line. b144 run ends at frame 144 (before the level-up frame) | APPROVE |
| 4h | B042 eqtext: no digit on qty-1 rows | PASS | `png/eq-bag-mid.png`: "Useless Blade" row without digit; `png/b046-after-up.png`: "5 Potion" keeps the digit | APPROVE (DROP gate in 3j is separate) |
| 4i | B050 Hotty slot 4 (see 2a) | PASS | See 2a | APPROVE |
| 5a | P14: stat spend persists after Yes | PASS | P14 chain: `y-yes-s3` staged 3->2->1, `Stats confirmed frame=150 points=1`; `y-reload-yes` `stat=1`. P15 chain (4c) persists the same way | APPROVE |
| 5b | P14: Faery select changes key-4 icon | PASS | `f-select-hotty`: `Faery selection committed current=1 HUD key-4 refresh`; reload `h-hud-cur1`: key-4 icon green (Primula); before: gold (Celeste). `png/h-hud-cur1-hud.png` | APPROVE |
| 5c | P14: drop pickup | PASS | `d-gold-potion`: `World item pickup frame=190 id=GoldStack01 reason=scripted outcome=0 picked=1 gold=0->9` | APPROVE |
| 5d | P14: Auto-Equip ALL | PASS | `e-auto-all`: `Equipment AutoEquip ALL -> slot0=StartingSuit/equipped ... slot1=Longsword01/equipped ...`; "Auto-equip All" button on screen (`png/b051-*.png`) | APPROVE WITH CAVEATS (choice rule not checked against the original) |

## Open items for root and owners

1. **B042 DROP on an unequipped bag sword (3j), REJECT for this item.** The Drop button is missing for an unequipped Useless Blade in the bag. P12 shows it; P13 rc2, P14 rc1 and rc2 do not. Unequipped boots show Drop on rc2. This was P13 finding 9b and is not new in P15, but it was blocking for P13. Root must either fix it or explicitly waive it for Preview 15.
2. **B047 speed and stop unverified (4d).** Needs a per-frame trace or footage-based measurement.
3. **I025 cooldown overlay (4f) not run.** Needs the temporary `cooldown_ms` edit, which verifiers may not make.
4. **B048 dropsound (4e) not re-run in rc2.**
5. **Coverage limits:** B051 and B046 for the Knight only (Rogue/Mage not run). Hit push only for clean hits. B038 by eye, not measured.
6. **Audio:** cadence measured (0 underruns, 0 gaps over 40 ms). Audibility and the Hotty/Celest cast sound levels need a human listener.
7. **Fixture notes:** B049 batch 1 re-seeded from `points/prof/stat2.save` (the branch fixture was already overwritten by its own earlier Yes run). Missing asset `sfx_static_ball_killed.wav` (Celest) and `level_up.bdae` (I026) are pre-existing.

## Verdict

**APPROVE WITH CAVEATS** for packaging rc2 as Preview 15 (startup flow excluded). The B052 keyed-cast fix, the Hotty slot-4 Post/Use fix, the P14 flows, the regression smoke set and the nine P15 spot checks pass on rc2 and reproduce the earlier failures on rc1 where a baseline was run. Item 1 (DROP on the unequipped sword) is an open, pre-existing blocker from P13 that root needs to fix or explicitly waive before packaging. If the P13 gate still applies, the verdict is REJECT until it is fixed.
