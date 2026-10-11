# B082 death screen ("black screen, not try again / return to checkpoint")

Branch fix/b082 (from windows-foundation 2a8faf0f), build exit 0, ctest only session_skill_binding fails (known). Scratch/evidence: `.local-inputs/claude-v20.1/fix082/run1/` (run.args, run.log, f*.png). Fixture: `--test-player-hp 10` next to a lizard (deathwire runF args, own isolated save dir).

## Evidence
- Video: NONE. No death screen was found in the inspected sparse grids and blackdetect candidates in P1/P2 (v20 DEATH-FLOW report); this does not prove absence throughout both videos. No video match can be claimed.
- SWF (dqhud_droid.swf, sprite742 `menu_FadeToBlackScreen`, re-read from dqhud_droid-tree.json): children are only flush_text, btn_Block (empty action), btn_Continue (full-stage hit rect, action `gotoAndPlay("Hide")`), black fade shape496, panel shape739 (skull aperture), FlashingText sprite741 = `GLOBAL_DEATH{1..5}_01`. No button graphic, no "try again", no "return to checkpoint" text. `RespawnText`/`TimerNumber_txt` (sprite747) belong to the ONLINE death timer. A search of tree/inventory for retry/try again/checkpoint/revive/game over found nothing else offline.
- IDA (APK 1.0.2): `_HandleLocalDeaths` 0x375fe8 pushes menu_FadeToBlackScreen once offline; `NativeReviveAllPlayers` 0x43b1e4 (SWF frame 88) -> `ReviveLocalPlayers` 0x37565c -> `Level::LoadCheckpoint` 0x3f0428 + `Character::Revive`. So the original's single offline action IS "return to checkpoint"; there is no choice.
- Match: our flow matches the SWF/IDA logic (fade+skull, quote, tap = Continue, checkpoint reload, revive, fade-from-black). Visual layout vs original device: not verifiable (no footage).

## Findings in our render (run1, window 1201x720, not 3:2)
1. BUG: the black fade/skull surround cover only the letterboxed 480x320 stage; the side bars stayed fully lit with the live world (old frames deathwire runF f200/f300 show the world at both edges, even while the quote is up). Fixed.
2. No hint at all that a tap/click/Space is needed (original relied on touch; first-time PC user sees only a quote on black = the reported "black screen"). Added hint (below).
3. Quote is rendered and visible (Fontin SmallCaps, white, 2 lines, `$player` substituted; f350). Hero death pose: hero lies on the floor in the Died clip's last pose (f200 skull frame); consistent with shot-6. Not an animation bug.

## Decision on "try again / return to checkpoint"
The original offline death screen has no choice buttons, so none are invented. Instead a clearly labelled PC ADAPTATION hint "Click or press Space to return to the checkpoint" (dim grey, below the quote) appears once the quote is fully visible (timeline frame >= 74) while Continue is armed, and disappears when Continue is taken. It tells the user exactly what the original's only action does (checkpoint reload + revive) without changing flow or timing. If the user wants real two-button choice it must be a new design decision (no source basis).

## Change
- `features/persistence/death_letterbox_black_v1.hpp` (new): bar rects tiling window-minus-stage + draw helper.
- `death_screen_renderer_v1.cpp`: bars drawn with max(alpha of panel, fade) before the stage layers. `death_fade_from_black_renderer_v1.cpp`: bars follow sprite743 alpha.
- `main.cpp`: hint draw after the death screen (marked B082 PC ADAPTATION).
- Test: `death_host_v1_tests` new `letterbox bars tile window minus stage` (4 window sizes + exact 3:2); PASS.

## Verification
Real EXE (quiet_run, hidden, silent): death at frame 132, f200 skull frame fully black outside skull incl. side bars, f350 quote + hint, f420 hint gone while fading, f480 revived at checkpoint with HUD. Before-frames: `.local-inputs/v20/deathwire/runF/f200.png,f300.png` (bright side bars).

## Not verified
Original look on device (no footage); gameplay behaviour in the user's exact window size/run (shot-6 shows the hero lying with HUD and no overlay, i.e. captured before the fade built up or before admission; cannot tell which); the hint wording is not original text and is English-only; mouse click path still only unit-tested/untouched (not exercised live).
