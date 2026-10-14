# w4-hud: original mobile (touch) gameplay HUD layout, selectable next to the desktop layout

Branch `v21.8-w4-hud` (worktree `.claude/worktrees/w4-hud`, from `windows-foundation` 7f0d250a). Package: `.local-inputs/windows-source-clock-v21.8-staged3`.
Build `C:/dh218/w4-hud-build` (subst R:), runner `C:/dh218/w4-hud/run-hud.ps1` (hidden desktop, real Win32 mouse/keyboard messages), evidence in
`coordination/v21.8/w4-hud-evidence/`.

Usage: `dh-foundation.exe ... --hud --hud-layout original-touch` (or `desktop`; the DEFAULT stays `desktop` until the owner decides).

## 1. Investigation: where the original layout lives (INVESTIGATION)

Reference observed (visual): the supplied screenshot (1616x940). Direct observations: portrait + HP/MP bars top-left, pause button under it on the left, music
note and potion (with a small round count badge "6") top right, faery button (round, rune icon) upper right, three skill buttons in an arc below it, a large
round attack button bottom right, bottom-left area empty (virtual joystick, not drawn in a screenshot).

### Where the numbers come from (all PROVEN unless marked)

* **PROVEN - the HUD SWF.** `port/android-native/app/src/main/assets/original-cache/data/menus/dqhud_droid.swf` (stage 480x320). The root timeline places four whole
  HUD variants `menu_HUD_0..3` (sprites 470/462/466/453). Each contains `HUDelements` with `HealthBars` (portrait, bars, `btn_potion`), `btn_mainmenu` (pause),
  `btn_charactermenu` (portrait), `itunes_controls.btn_itunes` (music note) and `controls.controls` with the buttons. All matrices were composed from the exact
  display lists (`features/hud_panels/probe_touch_hud_layout_v1.py`, outputs `w4-hud-evidence/probe-dqhud_droid-all-styles.json`):

  | style | skills | joystick | buttons present (HUDelements.controls.controls) |
  |---|---|---|---|
  | menu_HUD_0 | rolling `list` (btn_pre0/btn_0/btn_post0) | left (72,245) | list, btn_interact, btn_spell, btn_swap, Joystick |
  | menu_HUD_1 | rolling `list` | right (420,247) | same, mirrored |
  | **menu_HUD_2** | **fixed btn_skill1/2/3** | **left (72,247)** | btn_skill1..3, btn_interact, btn_spell, Joystick (labels `Hud1`, `Righty`) |
  | menu_HUD_3 | fixed btn_skill1..3 | right (410,247) + btn_dpad | mirrored, with btn_dpad |

  **The reference screenshot is menu_HUD_2** (PROVEN by geometry, fractions of the stage: reference skill-left .72/.80, skill-middle .77/.61, skill-right .90/.57,
  faery .91/.37, attack .91/.80; menu_HUD_2 .723/.807, .769/.618, .904/.582, .909/.387, .907/.803; the iPad HUD_0 .714/.859, .743/.69, .846/.637, .923/.505, .846/.839
  does not match). The earlier engine-ui handoff (`port/engine-ui/authored_gameplay_hud_v1_handoff.md`) reached the same conclusion ("HUDStyle2 matches the supplied
  left-joystick/fixed-buttons reference").
* **PROVEN - authored stage positions used (menu_HUD_2, 480x320 stage px; sprite origin and uniform scale 0.95 unless noted):**
  skill3 (347.35,259.0), skill1 (369.7,198.45), skill2 (434.65,187.0), btn_interact (435.2,255.5), btn_spell (faery, 435.55,123.8), btn_potion (444.95,34.5)
  with the counter badge shape 109 and the number in `cnt.value` rect (452.79,47.85)-(472.83,64.09) height 11, btn_itunes (348.45,9.25, scale 1.0),
  btn_mainmenu (pause, 28.3,93.25), btn_charactermenu (portrait, 6.05,8.45), Joystick (72.1,247.2; stick hit zone (20.6,197.5)-(121.9,293.9)).
  Generated into `features/hud_panels/touch_hud_art_v1.cpp` by `export_touch_hud_v1.py`. Physical left/middle/right = source slots 2/0/1 = btn_skill3/btn_skill1/btn_skill2
  (IDA `InfoHUDManager::FastUpdate` 0x41e064 reads `btn_skill%d` for source slot i; matches the desktop key order 1/2/3).
* **PROVEN - IDA.** `MenuManager::SwitchMenusToIngame` 0x42d30c loads `dqhud_droid.swf` for screen width 854, `dqhud_i9000.swf` for 800, `dqhud.swf` (iPad/iPhone variant)
  otherwise. The style is the saved option `HUDStyle` (design table row: Default 0, Min 0, Max 2, Step 1 - atlas `HUDStyle`); `menu_HUD_%d` is resolved in
  `HUDControls::initCachedChars` 0x419b4c, `InfoHUDManager::initCachedChars` 0x41d880, `VoxSoundManager::IPodControl` 0x36cda8. `HUDControls` also lets the player drag the
  controls and stores the positions per style (`SaveHUDPos` 0x418360/`SetHUDPos` 0x4187a0); defaults are the SWF positions used here. The joystick is the `Joystick` clip driven by
  touch (`HUDControls::onEvent` 0x418258, ignored when option `DPad` is set); the movement joystick is deliberately NOT part of this work.
* **PROVEN - iPad/iPhone difference.** `.local-inputs/ui-layout-discovery/dqhud.swf` has a 1024x768 stage (output `probe-dqhud-ipad-1024x768-all-styles.json`); there HUD_0/1 are the
  fixed three-skill styles (0 left joystick, 1 right) and HUD_2/3 the rolling list, i.e. the opposite order to droid, with iPad positions e.g. skill3 (731,660), skill1 (761,530), skill2 (866,489),
  attack (866,645), faery (945,388) (centre of art, from shape rectangles). `dqhud_i9000.swf` has the same 480x320 geometry as droid. Not used.
* **UNRESOLVED (documented, not guessed).** IDA's `FastUpdate`/`initCachedChars` choose the fixed `btn_skillN` cooldown path for `HUDStyle <= 1` and the `list` path above 1, which
  is the iPad order, while the droid SWF has the fixed clips in menu_HUD_2/3. Which saved `HUDStyle` value a droid user had for the reference is therefore not derivable from the code;
  the layout is chosen by the SWF clip (menu_HUD_2), not by a style number.
* **PROVEN - music note.** `btn_itunes` belongs to `itunes_controls` (iTunes bar: btn_play/pause/next/previous/playlist, song_title). It is only shown when `NativeUseIpodPlayer` 0x43a974
  (`nativeIsSupportMM()==1`) is true; the controls call `NativeIPodControl` 0x43a930 -> `VoxSoundManager::IPodControl` ("Play","Pause","Stop","Next","Previous","Open" ->
  `nativePlayBGMusic/PauseMusicBG/StopMusicBG/ChangeStep`) which drive the DEVICE's own music player. It does not toggle the game's music.
* **PROVEN - action natives** (existing behaviours reused unchanged): `NativeHUDSkill` 0x43e734 (slots 0/1/2), `NativeHUDSpell` 0x43cabc, `NativeHUDInteract` 0x43afb8, `NativeUsePotion` 0x43d2c8; potion
  count `ItemInventory::GetNumPotions` written to `btn_potion.cnt.value` (FastUpdate 0x41e064).
* **INFERENCE.** Screen scaling: the original stretches the 480x320 stage over the whole screen (reference attack at x=.91 of the width); the port keeps circles round (existing rule: uniform scale
  height/320), left cluster at the left edge, right cluster anchored to the right edge (`stage_shift_x = W/scale - 480`, negative for narrow windows). Hit regions are circles inscribing each
  button art (the SWF `hitzone` rectangles exist only for skill/potion; the desktop layout also used circles) - an explicit PC adaptation. The old comment in main.cpp that called the bottom row
  "original Android" was wrong: the bottom row is the PC adaptation.

## 2. Implementation (IMPLEMENTATION)

* `features/hud_panels/export_touch_hud_v1.py` -> `touch_hud_art_v1.{hpp,cpp}`: authored transforms, potion counter slot, joystick zone, music-note art, HUD_0->HUD_2 translations for the
  existing btn_interact and pause art. `probe_touch_hud_layout_v1.py`: the evidence table of all styles/files.
* `features/generic_skills/pc_gameplay_hud_v1.{hpp,cpp}`: `PcTouchGameplayHudLayoutV1` + `compose_pc_touch_gameplay_hud_v1` (same ring/icon/Grey/CoolDown/faery/potion/action art and the same
  five hit controls as the desktop layout; no key legends; potion count in the authored badge; attack and music as `extra_hits`; `joystick_zone` is the seam: not drawn, not hit-tested, add it
  via `platform_input::Surface::joystick` in a mobile build). The desktop composer is untouched apart from a shared helper refactor.
* `features/platform_input/semantic_input.{hpp,cpp}`: `Control::music` + `Frame::music_pressed` (click on release, ignored while a menu is open). Attack/skills/faery/potion by pointer already behave like
  their keys (held = pressed ring, `Control::attack` = Space).
* `features/audio/runtime_session_audio_v1.{hpp,cpp}`: `toggle_level_music()` (stop / restart the level track; restarts from level start / safezone / revive are held back while off) - **PC ADAPTATION**: the original
  note opens the iTunes bar for the device's own player, which does not exist on PC. Scripted PlayMusic is unaffected.
* `main.cpp`: option `--hud-layout original-touch|desktop` (default desktop), `hudStyleIndex` (base HUD `compose_original_hud(2)`/portrait aperture for touch, 0 for desktop), touch composition in
  `updatePcHud`, edge-anchored mapping for draw and hit test, pause art moved to its menu_HUD_2 origin, music handler.
* Tests: `pc_touch_gameplay_hud_v1` (CTest, new): hit test at all 7 authored centres incl. physical->source mapping, empty desktop row / joystick centre, non-overlap, right-edge shift,
  pressed ring/icon/cooldown/grey, failure preservation, semantic pointer behaviour (attack hold, skill click, music click, off-target release, modal menu).

## 3. Verification (REAL RUNS, hidden window, frames viewed)

Seeds: archer `w2-archer-PoisonArrows` (skill in the middle slot), knight `knight-groundslam` (starts damaged). Intro skipped with Enter twice.

| element | result | evidence |
|---|---|---|
| Layout vs reference | WORKED | `w4-hud-evidence/side-by-side-reference-vs-original-touch.png` (1616x940 each; positions match within ~1% of width) |
| Skill (middle, PoisonArrows) by mouse | WORKED | click (951,445): `Source skill key=2 slot=0 ... skill=PoisonArrows`, `Archer skill projectile launched` (frame 828); same at 1600x900 and 1024x768 |
| Skill left/right (empty slots) by mouse | WORKED | `Source skill key=1 slot=2 ... no valid saved skill row` / key=3 slot=1: the click reaches the right slot (refused because the slot is empty, same as the key) |
| Faery by mouse | WORKED | `Faery cast sound ... frame=709` after the click at (1102,278); also by key 4 |
| Potion by mouse | WORKED | knight run: `Source potion key=5 frame=650 result=1 consumed=1 quantity=5->4 HPraw=16318->50657`, badge shows 4 (`touch-1201x720-knight-potion-count-4.jpg`); at full HP `result=3` like the key |
| Attack by mouse (hold) | WORKED | semantic attack pressed/held at frame 1175, released 1216 (700 ms hold), archer attacks (`touch-1201x720-attack-held.jpg`) |
| Pause by mouse | WORKED | click (71,210): `Pause menu opened frame=1596`, Escape resumes (`touch-pause-open-after-click.jpg`) |
| Portrait by mouse | WORKED | click (81,85): `Character menu opened frame=1359 via profile input` (`touch-portrait-click-character-menu.jpg`) |
| Music note by mouse | WORKED (PC adaptation) | `HUD music button pressed frame=414 level music now off`, frame 504 `on`; icon drawn at the authored position. Audio output itself not listened to (harness is silent) |
| Keyboard parity in touch layout | WORKED | keys 5, 2, 4, 1, 3, Space, C, Esc all act as before (`touch-keys1`): skill/faery/attack/profile/pause logs identical to the clicks |
| Scaling / anchoring | WORKED | 1201x720, 1600x900 (shift 88.9) and 1024x768 (shift -53.3): drawn positions and click positions agree; no overlap at 4:3 |
| Desktop layout unchanged | WORKED | default run: bottom/top-left HUD regions pixel-identical (0 differing pixels) to a pre-change build; desktop circle click still casts; touch positions inert in desktop mode |
| Joystick | NOT DRAWN by design | seam documented (`joystick_zone`, `Surface::joystick`) |

Not verified: sound audibility; a real touch device; the original's stretched (non-uniform) mapping is not reproduced (circles stay round).

CTest: 219/219 passed (`ctest -j4`, llvm-mingw bin on PATH; the 218 existing + the new `pc_touch_gameplay_hud_v1`); `windows-foundation` had not moved, so no merge was needed.

## 4. Remaining gaps / owner decisions

* Default stays `desktop`; the owner can flip it (one line in `main.cpp` options).
* Music-note behaviour is a PC adaptation (toggle level music); the original iTunes bar needs a device music library.
* HUDStyle number mapping (see UNRESOLVED), the left-handed layouts (menu_HUD_1/3, `btn_swap`, d-pad) and the user drag-customisation of positions are not implemented; their authored data is in the probe JSON.
* Enemy/target bars and other HUD pieces are unchanged.
