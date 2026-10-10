# Verify assets3: Preview 15 rc3 (iPad 1.0.0 assets) vs rc2

Verifier: `assets3`. Date: 2026-10-10. No code changed. Scratch: `.local-inputs/claude-preview15/verify-assets3/` (args, logs, captures, PNG crops, summaries).
Runner: `DH_wt/p15int/port/windows-foundation/tools/quiet_run.ps1` only (hidden desktop, `DH_AUDIO_SILENT=1`). Main batch `-Parallel 12`; timing and audio-cadence jobs `-Parallel 1`.
Packages (read only): `.local-inputs/windows-source-clock-v19-preview-15-rc3` (candidate) and `...-rc2` (baseline).

## Package checks

| Item | Result | Evidence |
|---|---|---|
| EXE identity | PASS | rc2 and rc3 `dh-foundation.exe` both SHA256 `5C5CFD8D…9E88BC` (matches the brief). |
| rc3 = rc2 + 256 files | PASS | 256 new paths only (253 under `audio-assets/data/sounds`, `assets/data/3d/interface/level_up.bdae`, `assets/data/3d/camera/minimapcameras.bdae`, `minimapcamerashud.bdae`). No rc2 path removed; common files keep their sizes. `manifest.json` and `release-notes.json` identical. |
| Audio count | 501 sound files | `audio-assets`: 483 WAV + 17 VXN + `sounds.xml` (+ 3 `.bin`), rc2 had 251 files in total. |
| SHA256 vs `assets-extra/ios/MANIFEST.json` | PASS | 10 random picks (seeded): 10/10 match source and rc3 copy. Then all 256 rc3 copies: 256/256 match sha256 and size. |
| Package metadata | NOTE | `package-receipt.json` in rc2 and rc3 is still the preview-13 receipt (`exe_sha256 418FDB…`, not the rc3 EXE). Stale, not a code issue. |

## Item results

| # | Item | Result | Evidence |
|---|---|---|---|
| 1 | I026 level-up: logs and light column | PASS (logs), CAVEAT (look) | rc2 kill at frame 148: `Level up presentation frame=148 ... fx=135:level_up:failed(... level_up.bdae)` in b150, b160, b200. rc3: `fx=135:level_up:played` in the same runs, no `failed`. Pixel diff vs rc2 starts at frame 150 (bbox at the hero's feet, 4.7k px), grows to about 38k px by frame 160 and persists to frame 200 (about 0.8 s at 62.5 fps). Frames 149 and 146 identical to rc2. Look: a gold, diagonal beam leaves the hero's feet and a glow sits at the feet. The reference (`levelup/ref/burst_768_8_770.jpg`) shows a white-blue burst at the feet and a tall vertical white column. Colour and orientation do not match; the FX owner must review. `LEVEL UP!` text: `drawn=0 (no HUD status-message owner)`. |
| 2 | B048 drop/pickup sounds | PASS (armor, potion, gold, weapon drop); NOT-RUN (weapon pickup) | rc3 logs `World item sound uid=... event=drop|pickup ... status=submitted` for 148 drop armor, 149 drop potion, 151 drop gold, 150 drop weapon (1x), 152 pickup armor, 153 pickup potion, 155 pickup gold. rc2 gives `status=asset_missing` for the same uids. Weapon pickup (uid 154) never triggered in 26 seeds. Counters: WinMM `underruns=0` in all 99 runs. Voices count rises (seed8: 26 rc2, 37 rc3; seed2: 24 → 33). Silent mode counts as expected. Audibility not verified. |
| 3 | B050 Celest zero-target cast | PASS | Far casts, knight/mage/rogue: `Faery cast sound uid=209 label=StaticBallKilled targets=0 frame=71 status=dispatched` (rc2: `status=failed`, `Unavailable original audio asset: sfx_static_ball_killed.wav`). The success token in the Faery line is `dispatched`, the brief's `submitted`. Near casts and Hotty casts unchanged. Audibility not verified. |
| 4 | B028 lizard cues | attack PASS, death PASS, hurt FAIL | Lizard attack `sound=474` (`sfx_lizardman_attack_1`, uid 282): rc2 `status=4` with `Unavailable original audio asset` (131 lines), rc3 `status=1` (132 dispatches). Lizard death `sound=476` (`sfx_lizardman_die`, uid 285, at victim death): rc2 35 failures, rc3 35 dispatches. uid 283 (`attack_2`) not seen in any log. uid 284 (hurt): never submitted in either package, although a lizard was hit to HP 31/47.5 in `B048/seed2`. Cause: `RuntimeAudioHostV1::make_combat_audio` (`features/audio/runtime_audio_host_v1.cpp:304`) and `RuntimeCombatAudioV1::dispatch_hit` have no caller in `p15int/port` (grep). The CharSounds hit list (injury row → uid 284) is never reached. Missing hook: call `make_combat_audio` from the session/main melee-hit path and feed each hit to its observer. |
| 5 | B040 other music | NOTE | 37 runs: only `Level music transition: kind=start track=SwampHubAmbientMusic` appears. The other staged level banks are not requested by any scenario. Unreachable, as the brief expected. |
| 6 | Regression | PASS | SMOKE 130 frames: exit 0 in rc2 and rc3 (`renderedSeconds` 2.12 and 2.17). No `error`, `exception` or `assert` line in any rc3 run (all batches). Load time (sequential, `Parallel 1`, swamp + 130 frames): rc2 6.6 s and 7.3 s, rc3 6.2 s and 7.1 s, so no slowdown. Menu-start `startup.args` jobs timed out at 300 s in both packages, so menu startup timing is NOT-RUN (harness). |
| 7 | SHA256 of copied files | PASS | See package table: 10 random picks, then all 256. |

## Affected bugs

| Bug | Statement |
|---|---|
| B048 | **Fixed for the sound path.** Drop and pickup cues for armor, potion, gold and the weapon drop are submitted (not asset_missing) in rc3, with voices started and no underruns. Weapon pickup (uid 154) is not yet exercised. Audibility is not verified. |
| B050 | **Fixed.** Celest zero-target casts submit `StaticBallKilled` (`dispatched`, rc2 failed). Audibility not verified. |
| B028 | **Partly fixed.** Attack (uid 282) and death (uid 285) cues now dispatch in combat. The hurt cue (uid 284) is still never submitted: the combat-hit hook is not wired. Keep B028 open for hurt. |
| I026 | **Fixed for asset and event path, visual mismatch.** `level_up.bdae` loads, the presentation plays, and a light column/burst shows at the feet for about 0.8 s. Colour and orientation differ from the reference; the `LEVEL UP!` text is not drawn (no HUD owner). |
| B023 | **Assets present, behaviour not tested.** `minimapcameras.bdae` (2,600 B) and `minimapcamerashud.bdae` (1,688 B) are in rc3 `assets/data/3d/camera` and match the manifest. `camera_minimap_idle.bdae` (Android-staged) is not in rc3. The bug (markers, map page) is not verified by these runs. |

## Verdicts

| Item | Verdict |
|---|---|
| Package and SHA | APPROVE |
| I026 level-up | APPROVE WITH CAVEATS (colour/orientation vs reference; text not drawn) |
| B048 drop/pickup | APPROVE WITH CAVEATS (weapon pickup not run; listen test) |
| B050 Celest | APPROVE (listen test) |
| B028 lizard | REJECT as a whole (hurt cue unwired); attack and death parts APPROVE |
| B040 music | NOTE |
| B023 map cameras | APPROVE WITH CAVEATS (presence only) |
| Regression and audio cadence | APPROVE (smoke exit 0, no new errors, underruns 0, load time equal) |

## Notes and limits

- Cues are identified by the WAV name in the rc2 failure line and the same `sound=` ordinal in rc3. The logs carry ordinals, not uids.
- `dispatched` and `submitted` show that the output submit succeeded under silent mode. They do not show that anything is audible.
- Process hygiene: the quiet runner left two SMOKE EXEs running after their timeout (PIDs 84052 and 208268, both from this verification). I stopped those two PIDs only.
- Images: `.local-inputs/claude-preview15/verify-assets3/png/` (I026 frames b146, b150, b160, b200, strip crops f149–f172).
