# HUDART report (Preview 16): original HUD art for the action button, SKIP, caption box and quest banners

Branch `p16/hudart` (worktree `DH_wt/p16hudart`), from `p16/integrate` (261f2e49). Never pushed.
Status: SKELETON (work in progress; sections filled as verified).

## 1. Investigation (evidence)
- Source: `dqhud_droid.swf` (uncompressed FWS, 1 root frame, 480x320 stage, 30 fps). Atlas: `MenusGraphics_droid.tga` (bitmap 1), already loaded by the HUD.
- Action button: `btn_interact` = sprite 374 (labels idle 0, pressed 2, released 7, disabled 16, activated 18). Chain from the PC HUD root placement `menu_HUD_0` (sprite 470): HUDelements -> controls -> controls -> btn_interact. Icon sprite `btimg` = 373, frames Chest, Item, Lever, Character, Revive, Attack, Shrine, Inspect (0..7). Icon table `icon_table_v1` (context_button_v1) already returns these frame indices.
- SKIP: root `menu_skipcutscene` (sprite 737, label Idle) -> `btn_MENU_SKIP` (736, labels idle/pressed/release). Art: shapes 732 (plate) and 734 (X), bitmap-filled. Text 735 = EditText font 7 (Fontin SmallCaps, 16 px, pale yellow), label MENU_SKIP = "SKIP".
- Caption / tutorial box: root `DialogBox` (sprite 727), label `ChestTuto` = frame 11: GraphBox (shape 663 band, CharNameGraph 665 name plate, bitmap), dialogBox 702 (TextBox 701 body, NameBox 675 name), TutoIcon (btn_interact, not drawn: not visible in the reference caption frames).
- Quest banners: labels `QuestMsgDialog` = frame 5 (dialogBox 684, rest pose = frame before `hide` = 57), `QuestCompletedMsgDialog` = frame 20 (dialogBox 725). Frame: shape 498 (bitmap ornate frame), completed adds shape 723 (divider). Texts: Title/Desc (new) and TitleReal/Title/RewardTitle/Desc (completed), font 7.
- Reference video: `DH_sc/.local-inputs/reference-video/dh2-act1/Dungeon Hunter 2 (v1.0.3) Part 1 [720p] [z_Zky7qQdYs].mp4` (640x360 frames). Extracted frames: `.local-inputs/p16-runs/hudart/ref/{btn,skip,banner}/`.
  - btn: sword (Attack) button bottom right, ring orange (b-019 at 205 s: HUD hidden during the tutorial caption).
  - skip 109 s: X + SKIP top left; caption band with name plate ("Celeste") and two-line body centred near x 240.
  - banner 659 s: NEW QUEST ornate frame, gold heading "New Quest", pale two-line body, centred.
- Calibration (video vs chain, stage px): potion centre chain (444,34) vs video (578,38)/1.333,1.125 consistent with a stretched 640x360 mapping; action ring chain centre (416,257) vs video ~(424,252). Exact check is done on EXE frames (section 4).

## 2. Expected behaviour
(to be completed)

## 3. Implementation
(to be completed)

## 4. Integrated runtime verification
(to be completed)

## 5. Gaps
- Caption name plate text: the speaker name is not resolved by the host (no actor-to-name mapping decoded), so the plate is drawn without text.
- Caption body text: the chain puts the body box at x 4..370, the video centres it near x 240; the centring rule is applied in the drawing path (to be confirmed in frames).
- Caption and banner hold times remain placeholder timing (the original advance is not decoded).

## 6. Placeholders
(to be completed; expected: none for the art, timing placeholders only)

## 7. Package files required
- `original-cache/data/menus/dqhud_droid.swf`, `original-cache/data/3d/textures/MenusGraphics_droid.tga` (both present in rc3).

## 8. Verifier script
(to be completed)
