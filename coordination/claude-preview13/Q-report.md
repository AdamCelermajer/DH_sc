# Preview 13 wave 4 report Q: Equipment Details conformance (B042)

Status: PARTIAL. One proven divergence is fixed in code and has focused tests (carved frame ornaments and pillars, and the
Drop button on an equipped selection). B042 stays OPEN: the grey/orange panel tints and the damask extent are still
different from the reference, and no integrated EXE was captured after the change (the root builds it).
Nothing was committed. No tracker or shared build folder was touched. Scratch: `.local-inputs/claude-preview13/q/`.

## 0. Verdict per item (user screenshot vs reference)

| # | Item | Verdict | Cause | Action |
|---|------|---------|-------|--------|
| 1 | Grey top-right / orange bottom-right panels | PROVEN divergence (sampled). Cause NOT found. | No grey/orange fill exists in the Details display list (section 1.3). | Not fixed. Open. |
| 2 | Carved corner ornaments on the panels | PROVEN divergence. | Wave-1 plate restore kept plate 34 and 177 whole; their carved-frame triangles are not drawn in the reference Details. | FIXED: `drop_carved_frame` in `inventory_details.cpp`. |
| 3 | Flat white sword on the avatar | PROVEN divergence (visual). Cause NOT isolated. | Leads only (section 1.4). Texture is packaged. | Not fixed. |
| 4 | Black dividers (453) | NOT divergent in extent. The user's "right panel only" claim is not supported. Vertical line about 2 to 3 video px left in the user shot. | The horizontal line spans both halves in the reference (measured). The vertical offset is within the unknown stretch of the user's window. | Not changed. |
| 5 | Left slot-icon column | PROVEN brightness divergence. Also selected row differs (glyph vs quantity "1"). Cause NOT found. | Not isolated. | Not fixed. Open. |
| + | DROP button shown on an equipped selection | PROVEN divergence (reference Torso t=336 and Hands t=342 show no DROP; Feet t=372 unequipped shows DROP). | Original hides `btn_Drop` on the ItemEquipped path; our Details never gated it. | FIXED: `drop_available` gate, batch and label text. |

## 1. Evidence

### 1.1 Visual (observed, all frames looked at)
- User shot `.local-inputs/claude-preview13/user-shot/user-equipment-torso.png` (Torso, Ceremonial Garb equipped, Knight).
  Client area is the 480x320 stage stretched to 1780x1087 (normalised coordinates match the 640x360 reference).
- Reference Part 1 (`dh2-act1/Dungeon Hunter 2 (v1.0.3) Part 1 ...mp4`), extracted with ffmpeg:
  - t=336 Torso Details (`l/ref/p1-t336.png`), t=342 Hands Details (`q/ref/p1-342.png`), t=372 Feet Details, unequipped
    (`l/ref/g-372.png`), t=390 9-slot Inventory grid (`q/ref/p1-390.png`). Contact sheet `q/sheet_p1.png`.
- Side by side: `q/side_ref_user.png`; right half at 2x: `q/right_zoom.png`; middle band 3x: `q/mid_cmp.png`;
  icon column 3x: `q/icons_cmp.png`.
- Observed in the reference Details frames (t=336, 342, 372):
  - Left list panel: damask, bounded by a black border on its right at video x ~292 (stage ~219) and by the
    selected-row bar below; no carved ornaments and no thin vertical pillars anywhere on the left.
  - Right side: grey top panel (stage y ~40 to 178) and orange-brown bottom panel (stage y ~178 to 320), both starting
    at x ~296 video (stage ~222). No damask and no ornaments on the right.
  - Horizontal black line at video y 200-201 across BOTH halves (dark fraction 0.86 left, 0.89 right, measured).
  - Grid frame t=390 (not Details): damask on the right with the carved frame around the avatar column.
- Observed in the user shot (ours): carved corner scroll ornaments at the four corners of the avatar column (stage
  x 117-178 and 304-365, y 43-112 and 254-322), thin pillars at stage x ~166-172 and ~310-317, and carved bits at the
  left and right outer edges. These are the main-sheet frame. The reference Details frames show none of them.
- Our right side shows damask and the frame, not grey/orange (sampled: ours (43,30,13) and (34,27,15) at reference
  points (450,120) and (450,260) video; reference (86,80,66) and (130,89,53)).

### 1.2 Logic: which triangles are the carved frame (generated art, read-only)
- `features/character_menu/original_art.cpp` (generated, NOT edited) holds the main-sheet plates.
  Parsed with `q/scripts_plates.py` into `q/plate_tris.json`:
  - Plate 34 (shape 396, 16 fill records, 40 triangles):
    - tris 0-3: top button band (stage y 43-69, atlas u 0.55-0.67, v 0.08-0.10).
    - tris 4-19: damask base (stage x -5..173 and 310..488, atlas u 0.35-0.50, v 0.21-0.25).
    - tris 20-23: thin pillars (x 166-172 and 311-317, atlas u 0.32-0.33, v 0.21-0.40).
    - tris 24-39: 8 scroll-ornament quads (atlas u 0.26-0.32, v 0.36-0.43).
  - Plate 177 (shape 436, 4 fill records, 16 triangles): only outer scroll-ornament quads (same atlas rect).
- Overlay (`q/overlay_plates.png`, red = plate 34 and blue = plate 177 outlines on the user shot) shows the
  red quads sit exactly on the ornaments of the user shot.
- Rule used (data-based, no geometry hacks): a triangle whose three vertices all lie in atlas u 0.25..0.33,
  v 0.20..0.43 is carved frame (pillars and quads). Result: 20 kept (base and band), 36 dropped (`q/after_geometry_overlay.png`).

### 1.3 Logic: why no grey/orange layer is found (layer search, read-only probes)
- `q/scripts_bigfail.py` walks the Details sprite 456 (frame 0, recursively) and INCLUDES shapes the exporter fails on
  (the earlier scans skipped them). Large items: shape 440 (depth 5, full-stage solid 217,64,0 alpha 6/255: invisible),
  shape 453 (black dividers), shape 109 (list drag handle, hit only), shape 217 (avatar pane, hit only).
  No other large fill. The list sprite 111 (116 frames) has no shape with bbox area >= 15000 px in any frame (P-report).
- CharacterMenu root: the only full-stage base is shape 90 (plain brown fabric; this is the flat brown the P12 Details
  capture showed where the plates were erased).
- Conclusion: the grey top and orange bottom panels are not in the Details display list of the SWF we have
  (`dqcharmenu_droid.swf`, the v1.0.2 recovered art). Either the v1.0.3 video art differs, or the panels are drawn by
  a runtime path. This is a hypothesis, not proven. No fill was invented.

### 1.4 Logic: DROP gate (authored actions)
- `port/engine-ui/reference/character-menu-flow-v1/authored-actions.txt`, `displaySelectedItemInfos` (~0x1c927-0x1cc1e):
  the `btn_Drop._visible=false` store at 0x1cc09 sits on the path taken when the list row is ItemEquipped
  (`branch_if_true` to 0x1cbed, 0x1cbbe-0x1cbed). The unequipped path leaves Drop visible. The full branch was not
  decoded; the rule used is the three reference frames above.

### 1.5 Sword (item 3) leads, not resolved
- The longsword model `assets/original-cache/data/3d/characters/prince/weapons/mc_rweapon_longsword_01.bdae` references
  `atlas_weapons_dh2.tga` and `envmap_swamp.tga`. Both are packaged (`assets/data/3d/textures/`, and
  `original-cache/data/3d/textures/`).
- Both textures are BTEX/PVRTC containers with a TGA name (PIL cannot read them, and there is no CPU decoder here),
  so the texel content and the material path (envmap or specular on the pane render) were NOT checked.

## 2. Expected behaviour and implementation
- Expected (reference Details frames): the main sheet's carved frame (corner scrolls, pillars) is not drawn under the
  Details panel; the damask base and the top button band remain. Drop is not offered for an equipped selection.
- Implemented (`port/windows-foundation/features/inventory/inventory_details.cpp`):
  1. `carved_frame_vertex` / `carved_frame_triangle` / `drop_carved_frame`: keeps only triangles of plates 34 and 177 that
     are not all inside the carved atlas rect; a batch left empty is removed. Called once in `DetailsPresenter::frame`
     right after the existing erase lines.
  2. `drop_available = rows.empty() || !rows[current].equipped`. When false, btn_Drop batches (`menu_InventorySheetDetails/btn_Drop/`)
     are removed and the Drop label text (`menu_InventorySheetDetails/btn_Drop/`) is skipped.
- Not done (open, see section 6): grey/orange panel fills; the damask extent (reference damask to stage x ~219,
  ours to 173, with plain brown in between); the icon dimming; the selected-row glyph and the quantity "1".

## 3. Changes (every file)
1. `port/windows-foundation/features/inventory/inventory_details.cpp` (my hunks only; wave-1 `details_replaces_main` untouched):
   - `#include <cstddef>` after `#include <cmath>`.
   - New helpers after `details_replaces_main` (anchor: its closing brace, then `// Plates 34 and 177 also carry`).
   - In `frame`: `drop_carved_frame(next.art.batches);` after the `next.solids.erase(...)` line.
   - In `frame`: the `drop_available` flag and batch erase, placed before `std::string selected_name;` (after the panel-art block).
   - In `frame`: one `if(!drop_available&&prefix(path,"menu_InventorySheetDetails/btn_Drop/"))continue;` in the text-field loop.
2. `port/windows-foundation/features/inventory/inventory_source_tests.cpp`:
   - Replaced the "plates 34 and 177 must both survive" check with: plate 34 has exactly 20 triangles, plate 177 is absent,
     and no carved-rect vertex remains (anchor: `Original full-stage InventorySheetMain background plates` line).
   - Added "Unequipped item Details lost its Drop button" after the `EquipedSwordIcon/1` leak check.
   - Added "Equipped selection still shows the Drop button" after the "Original equipped marker did not follow" check.
- No generated file changed (`original_art.cpp`, `original_inventory_art.cpp`, `hud_geometry.cpp`, exporters): `git status` shows only the two files above (plus the pre-existing modified reports that are not mine).
- `main.cpp`, `CMakeLists.txt`, the shared build dir: untouched.

## 4. Tests (commands and real output)
Private build folders: `.local-inputs/claude-preview13/q/build/` and `q/build_old/` (negative check). Compiler
`llvm-mingw-20261006-ucrt-x86_64/bin/clang++.exe -std=c++17 -Wall -Wextra -Werror -O1`, linked against the existing
archives in `windows-foundation-build/*.a` (read only). Scripts: `q/build_inv_q.sh`, `q/build_eq_q.sh`, `q/build_pane_q.sh`
(copies of the P-wave scripts with the output folder changed).
- `build_inv_q.sh` -> `BUILD_OK`. `build_eq_q.sh` -> `BUILD_OK`. `build_pane_q.sh` -> `BUILD_OK`.
- `inventory_source_tests.exe .local-inputs/windows-source-clock-v19-preview-12/assets` -> exit 0,
  `original inventory source tests PASS name=Useless Blade stats=Damage 12 - 15 potions=Potions: 3` (`q/build/src-test.log`).
- Negative check (same new test file, COMMITTED `inventory_details.cpp` from HEAD): exit 1,
  `Original Details kept carved frame quads/pillars of the main plates or dropped the damask base` (`q/build_old/old.log`).
  So the new carved-frame assertion catches the bug. The Drop assertions were not reached in this run (the first failing check stops the run).
- `equipment_main_page_tests.exe <assets>` -> exit 0,
  `equipment_main_page_tests PASS: shared source slot/instance, Details, native Gear routing and compatibility equip/unequip`.
- `pane_tests.exe` (inventory_character_pane_tests + original_inventory_art) -> exit 0, `inventory character pane API tests PASS`.
- Runtime equipment runners, private copies (`q/run_q_binding_v1.ps1`, `q/run_q_avatar_matrix_v1.ps1`; the originals write into
  the shared `windows-foundation-build` tree, so only the output folder and repo root were changed):
  - binding: exit 0, `PASS same CombatSession player/world/CharacterState, original MAINPAGE/Details composition, typed drop/auto-equip/transmute ...`
  - avatar matrix: exit 0, `PASS B019 actual Knight/Rogue/Mage starter row and same-Session avatar matrix`.
- Not run: `features/equipment/run_source_equipment_renderer_smoke.ps1` and `features/inventory/run_inventory_character_pane_tests.ps1`
  (they write into shared build folders; my brief forbids that).
- Rendered pixels: NOT tested in this wave. The fix is geometry and gating; the integrated capture is the root's job (section 7).

## 5. Package files required
- No new runtime file. The fix uses the existing generated plates (`character_menu/original_art.cpp`, compiled into the EXE) and
  the packaged HUD atlas `assets/data/3d/textures/MenusGraphics_droid.tga` (SHA-256 `c75e8f4d…` per the P-report, present in
  `windows-source-clock-v19-preview-12/assets`; not re-hashed here).
- Generator input (not runtime): `port/android-native/app/src/main/assets/original-cache/data/menus/dqcharmenu_droid.swf`,
  SHA-256 `43227075f407626b52eca4c345ac6f568023fcb39c269588201026c0fc6760e0`, unchanged.

## 6. Uncertainties / not verified
- Item 1 (grey top-right and orange bottom-right panels) is open. The layer is not in the Details display list of the
  SWF we have (section 1.3). Possible causes: the v1.0.3 video art differs from the recovered v1.0.2 art; a runtime
  draw; a state of a child sprite not in frame 0 (list sprite 111 was checked in all frames). Not guessed.
- Right side after the fix: ours shows damask (base plate 34, x 310-488) where the reference shows grey or orange. Still wrong.
- Damask extent: the reference left damask ends at stage x ~219 (black border at video 292). Ours ends at stage x 173 with plain brown
  between 173 and 310. Cause not isolated (possibly a missing or non-triangulated part of shape 396, which has 16 fill records but only
  40 triangles). Not changed.
- Item 4 vertical divider: the user shot puts the line at video x 288-292 and the reference at 292-295 (measured). The offset
  (about 2 to 3 video px, about 1 to 2 stage px) is within the uncertainty of the stretched client area. The candidate capture
  `l/wip/torso-n.png` predates the 453 solid, so it cannot confirm the position. Needs an integrated capture.
- Item 5 icons: our icons average luminance 69-75 (upper and lower column) and the reference 24-33. The reference dims unselected rail icons;
  the mechanism (btn_Type frames or Highlight) was not identified. The selected row: the reference shows an equipped-item glyph at the left of
  the row and no number; ours shows the quantity "1" (the `Number` field is pushed for every row in `inventory_details.cpp`). Whether the
  original shows Number only for stackables (potions) was not checked. Not changed (this belongs with B016).
- Item 3 sword: texture content and material path not checked (section 1.5).
- Drop: the rule is inferred from the authored branch and three reference frames (equipped Torso and Hands, unequipped Feet). Other
  item types (rings, potions) were not checked. Drop text is hidden with the button.
- The reference list shows two rows (Ceremonial Garb, Imbued Armor) and ours one. That is a save-content difference, not a bug.
- No before/after EXE capture was produced. I was not allowed to rebuild the EXE, and the candidate EXE (`windows-source-clock-v19-preview-13-candidate`)
  does not contain this change. The only captures are the user shot and the WIP captures from wave 2 (`l/wip/`), so "before" is
  the user shot and "after" is the geometry overlay `q/after_geometry_overlay.png` (predicted, not rendered).
- B042 is NOT closed.

## 7. Verifier script (integrated EXE, after the root rebuilds)
Use a copy of the integrated package and fresh saves (never the live saves). Base args: `.local-inputs/claude-preview13/l/base.args`.
- Torso Details (equipped Ceremonial Garb): `--equipment-page-frame 60 --menu-release 80:112:84 --frames 95 --capture torso.ppm`,
  then `ffmpeg -i torso.ppm torso.png`. Expected:
  - no scroll ornaments at the four corners of the avatar column and no thin pillars (stage x ~166-172 and ~310-317);
  - the DROP button and its label are absent; EQUIP, UNEQUIP and Auto-equip remain;
  - left damask base present; right side still damask (known open: reference is grey/orange).
  Compare with `l/ref/p1-t336.png`.
- Unequipped item Details (e.g. right-hand Useless Blade, or any unequipped item in the list): the DROP button and label are present.
  The earlier wave-3 capture `.local-inputs/claude-preview13/p/b016/pkg/bag03-click.png` showed Drop for an unequipped Useless Blade.
- Known open after this fix (do not mark B042 closed): grey and orange panel fills, damask extent, icon dimming, selected-row glyph and
  quantity "1", sword model.
