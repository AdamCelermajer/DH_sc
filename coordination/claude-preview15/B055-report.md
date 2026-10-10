# B055 - campaign loading screen was a placeholder (fix session fix055, branch fix/b055)

## Evidence
- Bug shot: `user-shots/b055-loading-placeholder.png` (black panel, LOADING, plain tip box, red bar).
- Video (Part 1, 66-80 s, frames in `.local-inputs/claude-preview15/fix055/vid/`): ornate gothic frame with pillars and top arch, "LOADING" plate, tip in an ornamented panel with four filigree corners, red glossy progress bar with a bright spark, "TOUCH THE SCREEN TO CONTINUE" on the bar at 100 %, then the level-transition cinematic. Direct observation.
- Earlier Android reconstruction: `port/android-native/tests/test_authored_loading_screen_assets_v1.py`, `front_loading_render_policy_v1.hpp`, `front_ui_session_v87.cpp` (`show_game_loading` pushes `menu_Loading`). The real loading menu is the `menu_Loading` clip of `data/menus/dqshared.swf` (sprite 141: bg sprite 126 -> shape 8, frame shape 127, `LoadingText`, `loading` tip field, `loading_anim` (101 frames: track 131, Mask 133, red fill 134, spark 136), `continue_text`), bitmaps MenuGraphics01..05 + MenusGraphics. `loadanims*.swf` is NOT the loading menu (the STARTUP report guessed wrong).
- Variants: `dqshared_droid.swf` (480x320, brown MenusGraphics_droid art) does NOT match the video; the HD `dqshared.swf` art does (dark gothic pattern, silver filigree). IDA not needed (pure art/layout; progress and tip logic unchanged: `NativeGetLoadingTipStrID` 0043cb24).

## Change
- `features/startup/tools/export_loading_art.py` (python + shapely) exports the clip into `loading_art_data.cpp` (stage px, UVs per bitmap, 101 per-frame mask rects and spark matrices). `loading_art_v1.hpp` is the data model.
- `loading_screen_v1.cpp`: draws bg, frame, track, red fill (clipped by the per-frame Mask rect, CPU triangle clip), spark, then authored-font text (heading 42 px, tip 19 px, #ffffcc, authored field rectangles). Real progress (0/40/80/100 %) selects the frame; tip text/selection untouched. Falls back to the old fills if the textures are missing.
- Test `startup_loading_art_v1` (layers, UV range, mask monotonic, spark hidden at 100 %). Full ctest: only `session_skill_binding` fails (expected in worktrees).

## Verification (quiet run, Preview 15 assets, seeded save, `--loading-capture`)
before = Preview 15 exe, after = this build: `.local-inputs/claude-preview15/fix055/{before-040,after-sheet}.png` (after-sheet = 0/40/80/100 %). After frames show frame, tip panel, glossy bar with spark; layout of heading/panel/bar matches the video within a few pixels.

## Remaining gaps
- Placement: the stage is stretched to the window and zoomed 1.12x about the centre (fitted to heading/panel/bar landmarks in the video). With that the side pillars of the video are cropped; with zoom 1.0 they show but the panel is 25 % narrower than the video. The video (v1.0.3) probably uses a re-laid-out clip; the cached v1.0.2 movie and scaling code are not recovered.
- "TOUCH THE SCREEN TO CONTINUE" wait (clip `continue_text`, frames 72) is not implemented: the PC loading still auto-continues after the 1.5 s minimum (flow change, not art).
- Spark/bar use frames at the 4 real stage fractions only (finer stages not wired).

## Package files required
All already in the Preview 15 assets: `assets/data/3d/textures/{menugraphics01,MenuGraphics02,menugraphics03,MenuGraphics04,menugraphics05,MenusGraphics}.tga` (BTEX) and `data/Fontin SmallCaps.ttf`. No new files (the SWF is only used offline by the exporter).
