# P15 fixes verification (verifier p15fixes)

EXE under test: `.local-inputs/windows-source-clock-v19-preview-15-rc1/dh-foundation.exe` (SHA256 993CBC94...196420, matches brief). A/B baseline: `.local-inputs/windows-source-clock-v19-preview-14-rc1/dh-foundation.exe`. All runs through `quiet_run.ps1` (hidden desktop, silent, `-Parallel 12`; B049 batches and B037 k2 run at lower parallelism). Own folder: `.local-inputs/claude-preview15/verify-p15fixes/` (job builder `build-jobs.ps1`, `jobs-all.json`, summaries, captures, PNGs). Package and fixture folders were checked by mtime: nothing written after 18:50.

## Summary

Verified on rc1: B051 (Knight), B046 (all six icons, arrows, wrap), B042 (no digit on qty-1 rows), B048 (asset_missing lines, outcome unchanged), B049 (full two-point flow plus reload), I026 (failure path logged, no crash), Swamp 130-frame smoke (exit 0, HUD intact), B037 mid-swing skill accepted. B050 Celest cast sound logs correctly, but Rogue Hotty slot 4 ends with a Foundation error (exit 1): REJECT for slot 4. B047 target line matches but speed and stop are not measured. I025 art and casting OK; cooldown overlay not run. Rogue/Mage B051 not run.

## Results

| Item | Check | Result | Verdict |
|---|---|---|---|
| B051 avatarpose | attack, then Equipment: idle stance (Knight) | PASS. P14: mid-swing (raised arm, no sword). P15: upright idle sword. Attack vs idle-equip PSNR 43 dB (same pose). Late-open also idle. | APPROVE WITH CAVEATS (Rogue/Mage NOT-RUN) |
| B046 eqrail | rail icon clicks, arrows with wrap; Torso, Right hand, Feet, Ring 1, Waist, Potions | PASS. icon1 Right hand, icon3 Feet, icon5 Ring 1, icon7 Waist, icon9 Potions; up from Torso -> Potions (wrap); down x3 -> Feet; 9 then down -> Torso. P14: title stays Torso for all. | APPROVE WITH CAVEATS (dim normal art gap, known) |
| B042 eqtext | no '1' on Details rows; compare p1-t336 | PASS. Torso and Feet rows have no digit. P14 Torso shows "1 Ceremonial Garb". Stack count "5 Potion" shows the digit (quantity > 1 rule). | APPROVE WITH CAVEATS (reference layout differs; stack display not checked against original) |
| B047 dropanim | ~9.6 units/frame, stops 80 short, Z constant | PARTIAL. Frame 148 `World item target ... position=-6957.32,935.157,255` matches the branch's expected line. Speed, stop and Z per frame NOT-RUN: no trace line and item not isolable in captures. | APPROVE WITH CAVEATS (speed/stop unverified) |
| B048 dropsound | `World item sound ... status=asset_missing`; no crash; outcome unchanged | PASS. seed2: drop uid=151 and pickup uid=155 asset_missing; pickup gold 0->2 (P14 same). seed8: uid 148, 149 drop, 152 pickup asset_missing; PlateHelm stacks 4->5 (P14 same). All exit 0. | APPROVE (submitted branch not run: no sample) |
| B049 points | 2-point fixture: spend two, third refused, confirm box; Yes saves, No reverts; reload | PASS. two-f105/125 staged 2->1->0; refuse-3 logs "no Stat_Points remain"; yes-open "Stats confirmation opened staged=2"; yes-final "Stats confirmed points=0"; no-final "Stats cancelled points=2"; esc-dismiss; nostage-close shows no box. Reload: yes stat=0, no stat=2, fixture stat=2. | APPROVE WITH CAVEATS (skill-side confirm not implemented; box art differs) |
| B050 faerysound | key-4 logs `Faery cast sound uid= label= targets=N`; Knight/Mage/Rogue; Hotty slot 4 | Celest PASS: far (targets=0) and near (targets=3) dispatched for Knight, Mage, Rogue; StaticBallKilled failed (missing asset, logged). Hotty slot 4 casts (MP 30.25, cast sound dispatched) but then `Foundation error: Live combat: Source Post cannot complete before its authored retained Use event`, exit 1, far and near. The branch's own out logs show the same error, so it is not an rc1 packaging artifact. | REJECT for Hotty slot 4 (Celest cast sound: APPROVE WITH CAVEATS, audibility not checked) |
| I025 hudbtn | original button art, cooldown overlay during cooldown, keys still cast | Art and casting PASS: key 2 BashDown accepted in all cast runs. Cooldown overlay NOT-RUN: needs the temporary `cooldown_ms` edit, which a verifier may not make. The f300 matrix capture is the Skills page by design (`--skills-page-frame 120`), not a HUD test. | APPROVE WITH CAVEATS (cooldown NOT-RUN; empty-cell colour unchecked against original) |
| I026 levelup | `Level up presentation` fx=135 failed (level_up.bdae missing), no crash | PASS. Missing in rc1 (only sfx_level_up.wav exists). b144, b146, b150, b160, b200 log `fx=135:level_up:failed(Exact authored FX resource not found ... level_up.bdae)`. b130, b140, b143 and ctrl have no presentation line. 0 Foundation errors, exit 0. | APPROVE |
| Smoke: Swamp 130 | exit 0, HUD intact | PASS. exit 0, 466-line log; screenshot shows portrait, HP/MP, pause, keys 1-5, Faery, Potion. | PASS |
| B037 skill mid-swing | skill accepted during held-Space swing | PASS (via B048 seed2, same EXE): held Space from frame 30, combo hit 46, `Source skill key=2 slot=0 frame=50 generation=1 phase=1 skill=BashDown`, lifecycle to phase 2 at frame 84. The dedicated `s1-skill` fixture (preview-12 candidate save) is rejected by BOTH P14 and P15 ("different CharacterState/class than the preloaded bank"), so it is a fixture issue, not a regression. | PASS |

## Evidence

- Montages: `.../verify-p15fixes/B046/after-rail-montage.png` (after), `.../B046/before-rail-montage.png` (P14); `.../B042/b042-montage.png` (ref, after Torso, after Feet, P14 Torso); `.../I025/hud-montage.png` (after and P14 at f90, after f300 Skills page); `.../SMOKE/after/swamp130.png`; `.../B051/{before,after}/avatarpose/shots/attack-equip.png`, `.../B051/after/avatarpose/shots/idle-equip.png`.
- Logs: `.../verify-p15fixes/B047/{after,before}/B047-*-f*.log`, `.../B048/*/dropsound/runs/seed{2,8}/run.log`, `.../B049/after/points/runs/*/run.log`, `.../B050/*/faerysound/run1/out/*.log`, `.../I026/after/levelup/jobs/*/run.log`, `.../B037/after/B037-after-s1-skill.log`.
- Summaries: `.../verify-p15fixes/summary-all.json` (107 jobs: 78 exit 0), `summary-retry.json` (29 jobs: 27 exit 0; the 2 failures are Hotty), `summary-b049-b1.json` (9/9), `summary-b049-b2.json` (3/3), `summary-b037-k2.json` and `summary-b037-p14.json` (both exit 0; both rejected the skill).

## Open issues for root and owners

1. **B050 Hotty slot 4 Post error (blocking).** After the slot-4 cast passes Pre ("Hotty OnPre completed; waiting for its distinct source Cast-st..."), the run ends with exit 1 on `Source Post cannot complete before its authored retained Use event` (sequence 351). Reproduced in the branch logs and in two rc1 runs. Needs a fix or a deliberate gate.
2. **B047 speed and stop not verified.** No per-frame trace in the rc1 build. The branch's own report says the per-frame trace needs a temporary print. Needs a tested trace or footage-based measurement before approval.
3. **B049 branch fixture is not reproducible.** The branch's `points/runs/yes-final/character.save` had already been overwritten by its own earlier Yes run (points 0), so reruns that copy it from the branch folder start at 0 points and are refused. I re-seeded every batch-1 run from `points/prof/stat2.save`. The branch owner should reset those fixtures.
4. **B037 `s1-skill` fixture** (preview-12 candidate save) is class-mismatched and rejected by P14 too. Use the B048 fixture for the mid-swing check, or regenerate the candidate save.
5. **Not run:** I025 cooldown overlay (needs a code edit), B051 Rogue and Mage idle (no reachable actor rows in the branch jobs), B049 skill-side confirm (not implemented by the branch), B048 `submitted` branch (no sample).

## Method notes

- Branch job files were rewritten by `build-jobs.ps1`: EXE set to rc1 (or P14 for the A/B baseline), `--startup-config` converted to inline args with the same cwd, and all capture/save/log outputs redirected into this folder. Package and fixture folders were read only.
- Two runner issues found and fixed in my builder: `|` in `--menu-actions` must be escaped as `^|` for quiet_run's cmd.exe wrapper (startup-config files skip cmd, so the branch jobs did not hit this), and per-job `_out` folders to avoid shared-file races.
- Verifier made no code changes.
