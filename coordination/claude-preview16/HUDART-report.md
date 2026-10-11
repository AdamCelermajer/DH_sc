# HUDART report (Preview 16): original HUD art for the action button, SKIP, caption box and quest banners

Branch `p16/hudart` (worktree `DH_wt/p16hudart`). Base: `p16/integrate`, merged at 0801db0e (includes P16 OPENING).
Never pushed. Commits: 228944b4 (WIP: panels, wiring, tests), 0801db0e (merge of p16/integrate), then the banner wrap and presenter-test fix (see git log).

## Status (short)
- DONE and verified in the EXE (quiet runs, frames looked at, contact sheet .local-inputs/p16-runs/hudart/sheet/contact.png):
  - Action button: original btn_interact ring and btimg icon (Attack sword out of range, Chest icon at the tutorial chest). Replaces the placeholder ring and label.
  - Cutscene SKIP: original X and SKIP plate and text (MENU_SKIP label).
  - Caption box: original band and text, two-line wrap, centred. Name plate art is drawn; the name text is not resolved (gap 1).
  - Quest banners: original ornate NEW QUEST and QUEST COMPLETED frames with the authored heading, sentence, Reward, EXP and GOLD slots. The NEW QUEST sentence word-wraps to two lines as in the reference.
- The counter banner ("QUEST UPDATED") is no longer shown (no source banner found; see gap 3).
- ctest (build-p16hudart, final build): 129/131. Failing: session_skill_binding (allowed) and character_menu (pre-existing integration conflict on p16/integrate, gap 5).

## 1. Investigation (evidence)
- Source dqhud_droid.swf: uncompressed FWS, 480x320 stage (9600x6400 twips), 30 fps, single root frame. Atlas MenusGraphics_droid.tga (bitmap 1), already loaded by the HUD.
- Action button: btn_interact = sprite 374, labels idle 0, pressed 2, released 7, disabled 16, activated 18. Placement chain from root menu_HUD_0 (sprite 470): HUDelements (469), controls (468), controls (467), btn_interact (scale 0.95). Icon sprite btimg = 373, frames Chest 0, Item 1, Lever 2, Character 3, Revive 4, Attack 5, Shrine 6, Inspect 7. icon_table_v1 in context_button_v1 already returns these frame indices (0..7), so the published icon is the btimg frame.
- SKIP: root menu_skipcutscene (sprite 737, label Idle) -> btn_MENU_SKIP (736, labels idle/pressed/release). Shapes 732 (plate) and 734 (X), bitmap-filled from the atlas. Text 735: EditText font 7 = Fontin SmallCaps (port font id 5), 16 px, colour (255,255,204). The string SKIP is the MENU_SKIP entry of the original label table (features/frontend/art/original_art_data.cpp); the SWF field itself is set by ActionScript.
- Caption box: root DialogBox (sprite 727), label ChestTuto = frame 11. Visible children: GraphBox (666: band shape 663, CharNameGraph 665: name plate shape 664), dialogBox 702 (TextBox 701 body text, NameBox 675 name text). TutoIcon (btn_interact) is in this frame but is not drawn in the reference caption frames, so it is excluded.
- Quest banners: labels QuestMsgDialog = frame 5 (dialogBox 684) and QuestCompletedMsgDialog = frame 20 (dialogBox 725). Rest pose = the frame before the child's hide label. Frame art: shape 498 (ornate bitmap frame); completed adds shape 723 (divider). Texts: new = Title (heading), Desc (sentence); completed = TitleReal (heading), Title (sentence, 12 px), RewardTitle, Desc (reward values, 12 px).
- Reference video: Part 1 in DH_sc/.local-inputs/reference-video/dh2-act1/ (640x360). Frames: .local-inputs/p16-runs/hudart/ref/{btn,skip,banner}/. Part 2 at dh2_video_research/video/part2_cIAW38IfLxY.mp4 (916 s), sampled at one frame per 20 s: .local-inputs/p16-runs/hudart/p2/sheet-0-1280.png.
- Mapping: the port letterboxes the 480x320 stage uniformly (scale = window height / 320, centred). The reference video is stretched to 640x360. Positions are compared after normalising the stage, not in raw pixels.

## 2. Expected behaviour
- Action button: the idle ring with the icon of the current OOI (Chest, Item, Lever, Character, Revive, Attack, Shrine, Inspect); Attack when no OOI. Pressed ring while Space is held (PC adaptation; see gap 2).
- SKIP: the X and SKIP text at the top left while SKIP is offered.
- Caption: the band and text at the bottom (ChestTuto variant); name plate above the band.
- Quest banner: the original frame with heading, sentence, and for completion the reward heading and values, on the presenter timing.

## 3. Implementation
- features/hud_panels/export_hud_panels_v1.py (new): reads the exact SWF display lists from the named root placements and writes hud_panels_art_v1.cpp (generated). Batches are in the 480x320 stage (twips/20); bitmap batches keep the atlas UVs; zero-alpha solids are dropped; clip masks, filters and colour-add terms are refused. Text slots carry container, name, rect, rgba, height and align.
- features/hud_panels/hud_panels_art_v1.hpp (new): accessors (action_base_v1, action_pressed_v1, action_icon_v1, skip_*, caption_*, quest_new_*, quest_done_*), HudPanelTextSlotV1, kSkipLabelV1.
- generic_skills/pc_gameplay_hud_v1.{hpp,cpp}: the action button is appended at its stage position (append_stage_batches, no fit). PcGameplayHudLayoutV1 has action_icon and action_pressed; the placeholder ring, label and action_label are removed.
- main.cpp: layout gets the published icon and actionButtonHeld (Space held, set in the frame loop); cinematic SKIP and caption batches are drawn with the HUD atlas and text items carry align and height; drawQuestBanner draws the original batches and places each line in its text slot, with greedy word wrap (wrapBannerTextV1, HudGlyphRun advance) to the slot width.
- cinematic_runner.{hpp,cpp}: frame is original panel sets plus text items (SKIP label, caption body). Placeholder rects and the SKIP label constant are removed. The caption body is the TextBox slot; the NameBox slot is not drawn (no speaker name).
- quests/quest_banner_presenter_v1.{hpp,cpp}: lines carry slot and stack; counter ("updated") banners are not queued.
- CMakeLists.txt: features/hud_panels/hud_panels_art_v1.cpp in foundation_data.
- Tests: cinematic_runner_tests.cpp and quest_banner_presenter_v1_tests.cpp updated (slots, reward stacks, counter not shown).

## 4. Integrated runtime verification (quiet runs, EXE DH_wt/build-p16hudart/dh-foundation.exe)
Jobs: .local-inputs/p16-runs/hudart/quiet/jobs.json and jobs2.json (generated by .local-inputs/p16-runs/hudart/scratch/gen_jobs.js; not committed). Frames: quiet/<job>/<job>.png.

| Job | Setup | Log evidence | Frame |
|---|---|---|---|
| action-outrange | Swamp, no OOI | Action icon frame=0 icon=5 | Attack sword |
| action-inrange | Swamp, chest in range | container lines; icon 0 Chest | Chest icon |
| action-held | Swamp, Space held 30..49 | - | same as idle (gap 2) |
| zone-inside | tutorial_treasure zone (CINE2 job) | trigger and tutorial lines | SKIP top left, caption band, two-line centred body, name plate with no text |
| banner-new | bind-time NEW QUEST | Quest banner kind=NEW QUEST ... text='Kill 8 Bog Moths.' | ornate frame, New Quest, sentence wrapped to two lines |
| banner-done | Moths kills to completion | Quest banner kind=QUEST COMPLETED ... xp=20 gold=150 | ornate frame, heading, sentence, Reward, 20 EXP, 150 GOLD |

Contact sheet .local-inputs/p16-runs/hudart/sheet/contact.png: rows action / caption / new quest / completed; columns video (Part 1 frame where one exists) | before (placeholder, from earlier runs) | after (this build). The completed row has no video frame.
Comparison notes: after normalising the stage, the action ring, SKIP, caption band, and banner title and body positions agree with the reference within about 0.02 of the frame. The SKIP X sits about 0.015 of the width right of the reference; not changed.

## 5. Gaps (explicit)
1. Caption speaker name plate: the name text (e.g. Celeste, Rene, Tutorial in the reference) is not drawn. The host does not map the actor to its display name. Part 2 shows the plate text is set per speaker. The plate art is drawn empty. This is a missing mapping, not placeholder art.
2. Pressed state: the source pressed frame (374 frame 2) has the same display list as idle at the top level, and the held capture is visually identical. The held ring is wired but not visibly different; the source pressed visual is not decoded beyond this.
3. Objective counter banner: QUEST UPDATED is no longer shown. The source has no call site for an updated banner (IDA, QUESTUI2 report). Part 2 was sampled at 20 s intervals over the first ~21 minutes: no counter banner was seen, and the Quest Journal pane has no objective counter line. A denser sample around a kill is needed to rule it out completely.
4. Caption and banner hold times remain placeholder timing (the original advance is not decoded): kCaption*Placeholder, banner 3.0 s / 4.5 s.
5. character_menu ctest failure (Tab overlap left part not answered by the nearer Quest Log, character_menu_tests.cpp:90): pre-existing on p16/integrate, not caused here (this branch does not change the character_menu tab zones). The MAPFIX assertion expects the nearest-tab rule at x=424, but QUESTUI2 moved the Quest Log zone's right edge to 420.95, so x=424 belongs to Map. The two assertions disagree; the integration lead should decide which rule holds. Not changed here.
6. session_skill_binding ctest failure: known, allowed.
7. The banner sentence baseline (0.85 of glyph height) is an estimate; within about 0.02 of the reference.

## 6. Placeholders
- Caption and banner hold timing (kCaptionBaseMsPlaceholder etc., banner 3.0 s / 4.5 s): placeholder timing, not art. The art is original.
- No placeholder art remains in the action button, SKIP, caption band or quest banner frames. The name plate text is a missing mapping (gap 1), not a placeholder.

## 7. Package files required
- original-cache/data/menus/dqhud_droid.swf and original-cache/data/3d/textures/MenusGraphics_droid.tga (both present in the rc3 package). No new package files.

## 8. Verifier script
1. Export (exact source, no system Python): py -3 -I port/windows-foundation/features/hud_panels/export_hud_panels_v1.py (writes hud_panels_art_v1.cpp).
2. Build: powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p16hudart -Jobs 6
3. ctest (toolchain on PATH): ctest --test-dir build-p16hudart -j 6 (expect only session_skill_binding and character_menu failing; gaps 5-6).
4. Quiet runs: tools/quiet_run.ps1 -JobsFile <jobs> -Parallel 6, then convert the PPM captures with ffmpeg and look at the frames.
