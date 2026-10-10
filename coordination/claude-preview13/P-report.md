# Preview 13 wave 3 report P: Equipment Details art (B042, B016, B019)

Status: PARTIAL. Shape 453 (the black divider lines of the Details panel) is now exported and drawn, with focused
tests passing. The grey top-right and orange-brown bottom-right panel tints are NOT fixed; the layer that paints them
is not identified. B042 stays OPEN. B016 and B019 unequipped/Rogue/Mage captures are reported in section 7 (helper run).
Nothing was committed. HEAD at start of this wave: `5db15994` (the brief's `f6b7f134` is older).

## 1. Evidence

### Visual (observed, reference video Part 1 `part1_z_Zky7qQdYs.mp4`, 640x360)
- t=336 s, Torso Details (`.local-inputs/claude-preview13/l/ref/p1-t336.png`). Crop and 2x zoom:
  `.local-inputs/claude-preview13/p/tex/ref-right.png`. Observed: a grey top-right panel (UNEQUIP button, "Ceremonial
  Garb / Armor: 2"), an orange-brown bottom-right panel (EQUIP button, same text), thin black lines between the
  panels, and the 3D Knight standing over the boundary with a floor shadow.
- Black lines measured on the full frame (`p/scripts/dividers.py`, luminance < 6): a vertical line at x=293..294
  from y=43 to y=201, and the horizontal panel boundary at y~200..201 from x~296 to x~620 (seen in the crop).
- Candidate (WIP EXE capture `l/wip/torso-n.png`, scaled to 640x360) samples: (450,120) = (43,30,13) vs reference
  (86,80,66); (450,260) = (34,27,15) vs reference (130,89,53). So the candidate is dark brown where the reference is
  grey and orange. Confirmed by sampling, not only by eye.
- Inference (labelled): the 1.333x / 1.125x scale from the 480x320 stage to the 640x360 video, derived from the
  divider positions below. Two independent lines agree (section 1 Logic), so the mapping is treated as reliable.

### Logic (SWF, read-only probes in `.local-inputs/claude-preview13/p/scripts/`)
- Details sprite 456 has ONE frame and no labels (`frames.py`, `dl.py 456`): 41 placements; the Torso and Feet
  states are not separate frames of this sprite (the labelled variants are inside sprite 449, handled by existing code).
- Shape 453 (depth 234, plain SOLID black, no colour transform, no blend flag) is two separate thin polygons, not
  a full cover (`contours.py 453`):
  - A: x 220.9..223.7 from y -10.6 to 129.1, then y 129.1..131.8 from x 37.3 to 490.9.
  - B: y 162.2..164.9 from x 37.3 to 221.9, then x 220.9..223.7 from y 162.2 to 278.1.
  - Both are non-nested (bounding boxes disjoint in y), so they are separate solid pieces.
- Details placement in the InvMain root is (-69, 966) twips = (-3.45, +48.3) px. With that offset the lines land at
  stage x 217.5..220.2 and y 177.4..180.1 (top horizontal) and y 210.5..213.2 (lower-left). Times the scale above these
  are x 290..293 and y 199.6..202.6 in the video: they match the observed x=293 and y=200 lines.
- The selected-row bar (shape 441, stage y 178..211) sits on the same line in both candidate and reference.
- Why 453 was dropped (`export_hud_geometry.py`): `read_bitmap_styles` rejects fill kind 0 (solid) with
  "unsupported fill 0"; the hit-only solid path then rejects the two-contour fill ("multiple contours need a
  hole-aware tessellator"). So the lines were never exported, and the candidate shows no gutters.
- Grey/orange layer search (none of these produce a large coloured area under the Details frame 0 tree):
  - avatarpane 217 (both InvMain and Details) is an alpha-0 hit shape (`[1,1,1,0]`), so it is invisible in the original too.
  - InvMain inv_anim (435, 13 frames) contains only the 10 slot buttons (frames 0..12 checked).
  - Details list sprite 111 (116 frames): no shape with bounding box area >= 15000 px in any frame.
  - Button sprite 179 (Equip/Unequip): child 2 glow alpha ramps 0..0.86 only in the press/focus frames (6..17); frame 0 is idle.
  - The only large fills under the panel area are plates 396 (depth 34) and 436 (depth 177). Their triangles covering
    (400,60) are 396/tri 3 (atlas uv 0.55..0.67, 0.083..0.099) and 396/tri 15 (uv 0.35..0.50, 0.21..0.24); (400,250)
    is 396/tri 18 (uv 0.35..0.50, 0.24..0.25). The atlas is `assets/data/3d/textures/MenusGraphics_droid.tga`
    (BTEX/PVRTC, 1024 px; the SWF defines bitmap 1 as 1024x1024). Its decoded colours could not be checked here.
- Action scripts: the `ItemColor` push in authored-actions.txt (~line 7101) drives the main slot `btfill` frame, which
  the exporter already handles (5 fill states). No Details panel colour change was found in the authored actions.

### Remaining hypotheses for the grey/orange tints (not resolved)
1. Version difference: video v1.0.3 vs recovered v1.0.2 (the Details art may differ).
2. Plate 396 sub-regions (tri 3/15/18) sample a different atlas area than the original (needs a decoded atlas to check).
3. A state of a sprite that the exporter walks only at frame 0 (checked for the main list, buttons and inv_anim; not exhaustive).

## 2. Expected behaviour and minimal implementation
- Expected: the Details panel shows the thin black gutter lines of the original (stage x~218-220 vertical, y~178-180
  and y~211-213 horizontal, drawn above the buttons it crosses) over the main sheet plates. The grey and orange
  panel fills are expected too, but their source is not yet known; they are not faked.
- Implementation (reusable, no item/class/map hacks):
  - `tools/export_hud_geometry.py` `parse_shape`: several contours in one fill are triangulated one by one when none
    of them lies inside another (`contour_inside` helper). A nested (hole) contour still raises the hole-aware error.
  - Regenerated `features/inventory/original_inventory_art.cpp` (`export_inventory_art.py`): exports 453 as one more
    panel solid, anchored after `menu_InventorySheetDetails/btn_AutoEquip/1`, colour (0,0,0,1).

## 3. Changes
1. `port/windows-foundation/tools/export_hud_geometry.py`: new helper `contour_inside` before `def parse_shape`;
   in `parse_shape`, the `len(fill_contours) != 1` branch now tessellates each non-nested contour (the old single-contour
   path is unchanged).
2. `port/windows-foundation/features/inventory/original_inventory_art.cpp` (GENERATED, not hand-edited): `git diff`
   against HEAD is one inserted solid on line 16 (`menu_InventorySheetDetails/234`, 453) and nothing else (checked with a
   SequenceMatcher over the two 19-line files: one insert opcode).
3. `port/windows-foundation/features/inventory/inventory_source_tests.cpp`: the solid order check now expects 4 solids
   (453 first, black, anchor `btn_AutoEquip/1`, then avatarpane, list/btn_0/8, list/btn_post0/8); new geometry check
   `covered(...)` (point-in-triangle): (219,100) and (300,178.5) are covered, (300,100) is not.
- Not changed: `main.cpp`, `CMakeLists.txt`, `hud_geometry.cpp`, `character_menu/*` (generated, unchanged), any feature
  logic. `source_inventory_layout.json` is unchanged.

Regeneration proofs (unrelated output byte-identical):
- `python -I tools/export_hud_geometry.py`: `hud_geometry.cpp` and `reports/hud-source.json` `cmp` identical to the
  copies taken before the run (`p/backup/`). Log: `p/hud_export.log` (exit 0).
- `python -I features/character_menu/export_art.py`: `git status` shows no change under `features/character_menu/`.

## 4. Tests (real output)
Private builds only (`p/build/`); the shared `windows-foundation-build` was only read. Compiler:
`llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe -std=c++17 -Wall -Wextra -Werror -O1`, linked with the existing archives.
Scripts: `p/scripts/build_inv.sh`, `build_eq.sh`, `build_pane.sh`. Runs use `PATH` with the toolchain `bin` (DLLs).
- `build_inv.sh` -> `BUILD_OK`.
- Before the test update (after the exporter change): `inventory_source_tests.exe <assets>` -> exit 1,
  `Original detail panel/list source solid order or mask roles changed` (expected: 4 solids now, not 3).
- After: `inventory_source_tests.exe <assets>` -> exit 0,
  `original inventory source tests PASS name=Useless Blade stats=Damage 12 - 15 potions=Potions: 3` (`p/build/src-test.log`).
- `equipment_main_page_tests.exe <assets>` -> exit 0,
  `equipment_main_page_tests PASS: shared source slot/instance, Details, native Gear routing and compatibility equip/unequip`.
- `pane_tests.exe` (`inventory_character_pane_tests.cpp` + `original_inventory_art.cpp`) -> exit 0,
  `inventory character pane API tests PASS`.
- Not run: `features/inventory/run_inventory_character_pane_tests.ps1` and `features/equipment/run_source_equipment_renderer_smoke.ps1`
  (they write into shared build folders); the Python source test `inventory_character_pane_source_tests.py` was not run
  in this wave (no change to the character-pane source it reads).
- Rendered pixels: not tested. No offscreen renderer exists for this frame. The geometry check above is the only
  automated check of the new layer. The integrated screenshot is the root's job.

## 5. Package files required
- No new runtime file. The 453 solid is a colour fill (no texture), and the art is compiled into the EXE.
- Source SWF (generator input, not a runtime file): `port/android-native/app/src/main/assets/original-cache/data/menus/dqcharmenu_droid.swf`,
  SHA-256 `43227075f407626b52eca4c345ac6f568023fcb39c269588201026c0fc6760e0` (matches `character_menu/source_layout.json`).
- Runtime atlas used by the Details plates (already packaged): `assets/data/3d/textures/MenusGraphics_droid.tga`,
  SHA-256 `c75e8f4df8a9b28fcca7aa819a05cd69e6206d27a391b4364fd67790aeea86c7`. Present in
  `.local-inputs/windows-source-clock-v19-preview-12/assets/data/3d/textures/` (identical to the android-native copy by `cmp`).

## 6. Verifier script (integrated EXE, after the root rebuilds)
Use a copy of the integrated package and fresh saves, never the live saves. Base args: `.local-inputs/claude-preview13/l/base.args`.
- Torso Details: `--equipment-page-frame 60 --menu-release 80:112:84 --frames 95 --capture torso.ppm`, then `ffmpeg -i torso.ppm torso.png`.
- Expected: a black vertical line at about 45.5% of the width (stage x 218..220 of 480), running from the top down to about
  55.6% of the height, and a black horizontal line at about 55.6% of the height (stage y 178..180) from the same x to
  the right edge. Plus a black short horizontal line at stage y 211..213 on the left. The panel tints remain different
  from the reference (see section 7).
- Compare with `l/ref/p1-t336.png` (reference) at the same panel positions.

## 7. What still differs from the reference (after this wave)
1. Panel tints (B042, OPEN): reference top-right grey (86,80,66 at ref (450,120)) and bottom-right orange-brown
   (130,89,53 at ref (450,260)); candidate dark brown (43,30,13) and (34,27,15). The layer is not identified (section 1).
2. Divider lines (shape 453): now exported and drawn in the code; not yet visually verified in an integrated build.
3. B016 (unequipped Details with Transmute and Value) and B019 (unequipped weapon, Rogue/Mage): see the helper result
   below. Summary of helper result: PENDING at the time of writing (see the update line).

## Uncertainties
- The scale mapping (x 1.333, y 1.125) is inferred from the video and the stage size. It agrees with two line positions,
  but a crop offset of a few pixels is possible.
- The grey and orange panels could be part of the v1.0.3 art rather than the v1.0.2 recovered code.
- Line drawing order: 453 is drawn after `btn_AutoEquip/1`, so it is above the Transmute button and the list row bar.
  Whether the original draws the line above the Transmute button (it crosses x 209..300 at y 230) needs the Feet
  reference frame (t=372) to confirm.
