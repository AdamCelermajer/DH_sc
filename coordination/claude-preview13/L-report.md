# Preview 13 Wave 2 report L: Equipment page (B042 art/tint, B016 text, B019 avatar)

Status: NO CODE CHANGE in this wave. B042 stays OPEN (tint layer not identified). B016 and B019 were not
reproduced as defects in the candidate, so nothing was changed for them. Nothing was built or committed.
Scratch and frames: `.local-inputs/claude-preview13/l/` (git-ignored). The EXEs there are copies; the
candidate and Preview 12 folders were not modified.

## 1. Evidence

### 1a. Captures (normal window 1201x720 and 4:3 640x480; EXE copies in `l/wip` and `l/p12`)
Args: `l/base.args` (the Knight swamp setup, from the group-B args) plus the lines given in section 6.
- Equipment grid, no Details: `l/wip/eq-n.png`, `l/p12/eq-n.png`, `l/wip/eq-43.png`, `l/p12/eq-43.png`.
  Observed: 9 slots, brass avatar frame, gold at bottom. WIP and P12 look the same here (no plates to restore).
- Right hand (Useless Blade equipped), Details open: `l/wip/weap-n.png`, `l/p12/weap-n.png`.
  Observed WIP: the carved corner ornaments and dark plates from the main sheet are back under the Details
  art (the wave-1 fix is visible). P12: flat brown fabric, no plates. Confirmed.
- Torso (Ceremonial Garb equipped): `l/wip/torso-n.png`, `l/p12/torso-n.png`, `-43` variants: same effect.
- Potion details (`l/*/pot-n.png`, `pot-43.png`): captured, NOT reviewed in this session.
- Skills/Stats were not re-captured.
- Knight only. Rogue and Mage were not captured (no class-switch args tested).

### 1b. Reference (`dh2_video_research/video/`)
- Part 1 (`part1_z_Zky7qQdYs.mp4`, 640x360) has NON-MERCHANT equipment pages:
  - t=336 s, Torso Details: `l/ref/p1-t336.png`. Ceremonial Garb equipped, Imbued Armor below, top-right grey block
    with UNEQUIP and item text, bottom-right orange-brown block with EQUIP, Total Gold 13, no Drop button shown, dark list
    block on the left, avatar with the longsword.
  - t=372 s, Feet Details (Vagrant Boots selected, unequipped): `l/ref/g-372.png` (320 px). Shows TRANSMUTE button,
    "Value 11", "Req: 4 dex", Drop. The same grey/orange block structure.
  - Side by side with the candidate: `l/side3-torso.png` (left reference, middle WIP, right P12).
- Part 2 (`part2_cIAW38IfLxY.mp4`): the equipment screens I found at 384-520 s and in the earlier group-B
  montages are all MERCHANT mode (BUY/SELL header). I did not find a non-merchant equipment frame in Part 2.
  The earlier claim that the tints are a merchant-mode effect is WRONG: the non-merchant Part 1 frames show
  the same grey/orange block structure.
- Not done: I did not extract a full-resolution Part 1 frame of the 9-slot "Inventory" grid. A 12 s montage
  suggested one near t~380-400 s, but my exact extraction at t=384 gave a tutorial frame. Treat it as unverified.
- Sampling: 1 frame per 6 s over Part 2 from 404 s (`l/ref/p2-01..05.png`) and 1 per 12 s over Part 1
  (`l/ref/p1-01..06.png`). Gaps shorter than the sampling step can be missed.

### 1c. Logic (SWF export, read-only probes in `l/scripts/`, run with `-I`)
- Probe `cxform_probe.py`: colour transforms on placements under Details (sprite 456), InvMain (439).
  Only alpha-0 hidden states and one 50% alpha on the Transmute background (`btn_GAMEPLAYMENUS_TRANSMUTE2/2`).
  No colour transform on the panel plates. So the tint is NOT a placement colour transform in frame 0.
- Probe `children.py 456`: direct children of Details. The relevant ones:
  - depth 8, shape 441 (bitmap fills, kind 0x42): bbox stage y 178-211, x 27-227. This is the orange selected
    row bar, not a panel.
  - depth 5, shape 440: DefineShape4 with one SOLID fill (217,64,0) alpha 6/255, full-stage bounds. Nearly
    invisible. Not the panel. The exporter rejects it ("hit-only shape must have solid fill").
  - depth 234, shape 453: DefineShape, one SOLID fill (0,0,0), full-stage bounds x 37..491, y -10..278, several
    contours. The exporter rejects it ("multiple contours need a hole-aware tessellator"), so it is NOT in
    `original_inventory_art.cpp`. Black would darken the area, not lighten it, so it does not explain the
    reference's light grey and orange blocks. It is still the one visible Details shape the exporter drops.
- `solids.py` on `original_inventory_art.cpp`: the only non-list solid in the Details art is the alpha-0
  EquipedSwordIcon. The grey and orange blocks are not emitted as solids.
- `batches.py`: Details/8 UV range (0.25-0.43, 0.68-0.70) is the highlight bar. Details/186/1 and 237/1 are
  small icons (bboxes stage x 188-216 and 248-271). None of them is a large block.
- The main-sheet plates (shapes 396 and 436) are the full-stage plates restored in wave 1.
- Conclusion: the light blocks in the reference are not identified. The candidate has no matching layer in
  the Details art. Possible sources not yet checked: the avatar pane backdrop (`avatarpane` sprite 218 at
  depth 235, scale 0.62 x), the InvMain avatar plate, or a bitmap-filled shape that the exporter emits with the
  wrong UV. Needs an IDA or SWF display-list dump of the Details sprite at the Torso frame, with per-depth
  colour and fill, to decide.

## 2. Expected behaviour and implementation
- B042 expected: the Details state shows the same grey top-right and orange-brown bottom-right blocks as the
  reference (Part 1 t=336 and t=372), with the main sheet's plates underneath.
- Implementation: none yet. Adding a hole-aware tessellator for shape 453, or drawing the blocks from a guess,
  would be a guess and was not done.

## 3. Changes
None. No file in `port/` or `coordination/` was modified by this wave (only the new report file).

## 4. Tests
- No code changed, so no focused test was added. The existing runners were NOT run for this wave: no touched code
  to exercise. (Wave-1 tests from group B still apply.)
- Read-only probes (all with the bundled Python, `-I`):
  `C:/Users/adamc/.cache/codex-runtimes/codex-primary-runtime/dependencies/python/python.exe -I cxform_probe.py 456 Details`
  (CX lines printed: alpha-0 and 50% only); `children.py 456`; `solid440.py` (parse errors for 440, 453, 106, 94, 441);
  `styles.py 440 453 106 441` (fill dumps as in 1c).
- Captures: EXE runs returned exit 0 (`--startup-config`), logs show `Character menu Equipment selected frame=60`.

## 5. Package files required
No new runtime asset. The captures used the candidate's `assets/`, `audio-assets/`, `ui-assets/` through junctions.
Source SWF: `port/android-native/app/src/main/assets/original-cache/data/menus/dqcharmenu_droid.swf`
(sha256 43227075... as in group B). Not hashed again.

## 6. Verifier script (for B042 Details)
Copy the EXE and its assets to a fresh folder, add `base.args`-style startup lines, and use these extra lines:
- Equipment grid, normal window: `--equipment-page-frame 60 --frames 75 --capture eq-n.ppm`. Expected: 9 slots,
  brass avatar frame, log line `Character menu Equipment selected frame=60`.
- Equipment grid, 4:3: add `--window-size 640,480` (the form is `W,H`).
- Torso Details: `--equipment-page-frame 60 --menu-release 80:112:84 --frames 95 --capture torso-n.ppm`.
  Expected WIP: carved plates visible under the Details art. Expected (not yet met): grey top-right and
  orange-brown bottom-right blocks as in the Part 1 t=336 frame.
- Right-hand Details: `--menu-release 80:112:132`. Potions: `--menu-release 80:382:284`.
- Convert PPM to PNG with ffmpeg (`-i x.ppm x.png`).

## 7. What still differs from the reference (after this wave)
1. Tint: the reference's top-right block is light grey and the bottom-right block is orange-brown. The candidate
   shows dark brown fabric in those areas. Not fixed. Layer not identified.
2. Shape 453 (black, multi-contour) is dropped by the exporter. It is not the cause of the light blocks, but it is
   a real gap in the Details art. Needs a hole-aware tessellator or a documented exclusion.
3. Merchant mode (BUY/SELL, Value/SELL button) is not reachable here. The candidate's header and buttons differ from
   merchant frames by design.
4. Unequipped-item Details (Transmute button, "Value N", "Req" for an unequipped item) was not captured: the Knight's
   starting inventory has only equipped items. The reference (Part 1 t=372) shows this state. B016's normal-build
   check needs a save with an unequipped item.
5. B019: the Knight holds the longsword in all captured states, as in the reference Part 1 frames. No defect seen; not
   verified against unequipped weapons or the Rogue/Mage.
6. The 9-slot "Inventory" grid in Part 1 was not extracted at full resolution (see 1b).

## Uncertainties
- The Part 1 and Part 2 sampling can miss short screens.
- Video version (v1.0.3) differs from recovered code (v1.0.2); a tint change could be a version difference.
- The black-solid claim for shape 453 comes from the SWF fill record (0,0,0). Its actual on-screen effect (blend
  mode, layer order) was not checked.
