# Preview 15 report EQTEXT: Equipment Details text and art (B042, update)

Branch p15/eqtext. Build: `p14_build.ps1 -Name eqtext -Test`: 111 ctests, 110 pass. The only failure is
`session_skill_binding`, the known worktree junction issue (ignored as instructed).

Status: B042 is NOT closed. One proven divergence is fixed (row digit). The panel fills, the damask extent, the
equipped-row glyph, the non-equippable row style, the rail brightness and the sword material are open, with evidence below.

## 1. Evidence (AGENTS.md note)

Frames: reference Part 1 t=336 (Torso, equipped) and t=372 (Feet, unequipped). Full-resolution frames extracted with
the Windows ffmpeg: `DH_wt/eqtext-scratch/t336-full.png`, `t372-full.png` (640x360).
Our captures: `DH_wt/eqtext-scratch/out/torso-base.png` (before), `torso-after.png`, `feet-base.png`, `feet-after.png` (1201x720).

Geometry (measured, not assumed): the reference is letterboxed. The game area is video x 20..620, y 0..360, so
stage = ((video x - 20) / 1.25, video y / 1.25) on a 480x288 stage. Our capture is stage x 2.5. The divider lines,
the button positions and the rail icons agree with ours to about 1 stage px under this mapping.

Observed in the reference (direct):
- Selected equipped row (t=336, Ceremonial Garb): a sword glyph at stage x ~40..50, y ~170..180. No digit.
- Unselected, not equippable row (t=336, Imbued Armor): red X at the left and green name text. No digit.
- Unselected equipped row (t=372, Ceremonial Boots): sword glyph. Selected unequipped row (Vagrant Boots): no glyph, no digit.
- No row shows a digit in either frame. Our rows showed "1" (the Number field was pushed for every row).
- Left list panel: damask continues to the divider at stage x ~218 (video 292). Ours stops at stage x ~172.
- Right side: grey top panel (video x 296..620, y 45..200, sampled (72,63,48) at y 70 rising to (107,101,85) at
  y 190) and an orange-brown bottom panel (y 200..320, sampled (135,97,62) at y 230 falling to (108,69,37) at y 310), with a
  soft dark shadow under the Knight's feet. Ours is dark damask there.
- Dividers: vertical at video x 292..296 (stage ~218) over the full height on both halves, horizontal at video y ~200
  (stage 160) across both halves. Ours matches in position (1201-px capture x 545, y 400). No change needed.

Logic (decoded from the authored script, `port/engine-ui/reference/character-menu-flow-v1/authored-actions.txt`):
- `GenerateInventoryListItems` (0000b8a7) clears `list/btn_0/Number.htmlText` for every row (0000ba41, 0000ba4e).
- The row function at 0000c475 then sets `Number.htmlText = $data.ItemQuantity` (0000c4c2). The native ItemQuantity
  for single items is not decoded; the reference shows no digit, so single items show nothing.
- `displaySelectedItemInfos` (0001c865) also writes `htmlText` "" to the Number field (0001ccfd).
- `ItemEquipped` (0000c4da) and `ItemEquippable` (0000c4cd) are set on the row data. The row art that these toggle (the
  glyph, the X and the green text) is not in the exported Details art (`features/inventory/original_inventory_art.cpp`
  has only `list/btn_0/{4,6,8,Host,Number}` and `list/btn_pre*/...`). The glyph has no small child shape in the exported art.

Art source checks:
- `DH_sc/.local-inputs/claude-preview14/equip/decode/menugraphics02_512.png` and `menusgraphics_droid_512.png` (512 px
  downsamples of the decoded PVRTC atlases): the damask, the carved frame, the button bars and the icons are present.
  Neither atlas shows a grey or orange gradient block, so the panel colours are not a plain atlas region.
- `original-cache/data/menus/dqcharmenu_droid.swf` Details display list: no grey or orange fill (Q-report 1.3, not re-probed).
  `avatarpane` (shape 217, stage x 219..322, y 66..261) is hit-only art. That is where the 3D Knight is drawn.

## 2. Changes

Commit `2279da01` "Details list rows: no count for single items (B042)":
- `features/inventory/inventory_details.hpp`: `inline bool details_row_shows_count(std::uint32_t quantity)` returns `quantity > 1`.
- `features/inventory/inventory_details.cpp`: the row Number field is pushed only when `details_row_shows_count` is true.
  Stacks keep their count (stack display is not verified in the reference).
- `features/inventory/details_row_geometry_tests.cpp`: check `!shows(1) && shows(2) && shows(32767)`.
- `features/inventory/inventory_source_tests.cpp`: the expectation "1" is replaced by "no Number for a single item" and a
  stacked (quantity 3) case. NOTE: this file is not compiled by CMake (no target references it). Its assertions are
  documentation only until someone adds it to a target. The compiled test is the one above.

## 3. Verification

- ctest: `inventory_details_row_geometry`, `equipment_adapter`, `equipment_main_page`, `equipment_inventory_actions`,
  `equipment_visual`, `combat_session_equipment_multiplicity`: all pass (6/6 in the equipment and inventory subset).
- EXE (quiet, hidden, silent): `quiet_run.ps1`, two jobs, exit 0 each.
  - `torso-after.png`: selected "Ceremonial Garb" row shows no digit (before: "1").
  - `feet-after.png`: "Ceremonial Boots" (selected) and "Imbued Boots" (second row): no digits (before: "1").
  - The equipment log shows the page bound (`Character menu Equipment selected frame=300`).
- Not verified: a stacked item's count in the EXE; the equipped glyph; the X and green style; any panel fill.

## 4. Remaining B042 divergences (open) and why

| # | Divergence (proven by side-by-side) | Why not fixed |
|---|---|---|
| 1 | Right side: reference grey top / orange-brown bottom panels (sampled above), ours dark damask | Not in the Details display list of `dqcharmenu_droid.swf`, not in the decoded atlases. Source unknown (3D backdrop behind the avatar pane, a runtime path, or a version difference). Painting sampled colours would be an invented fill. |
| 2 | Left list damask ends at stage ~172 instead of ~218 | The damask tris are plate 34 only (x -5..173). The reference region 173..218 has no plate in our set; the other main-sheet layers are removed in Details. Adding a stretched UV quad would be a guess about the atlas mapping. |
| 3 | Equipped row: sword glyph in the reference, nothing in ours | The glyph is not in the exported list-row art (no small child in `list/btn_0`). Needs the list sprite (111) frames from the SWF. |
| 4 | Non-equippable row (Imbued Armor): red X and green name in the reference | The `ItemEquippable` frame art is not exported; the green colour is not decoded from the SWF text style. |
| 5 | Rail slot icons: reference dimmed (luminance 24..33 vs ours 69..75, Q-report) | No alpha or colour change in the authored script for `inv_anim`. Mechanism (frame state or tint in the tab sprite) not found. |
| 6 | Sword on the avatar: reference grey/silver with dark hilt, ours bright | Material path not compared in this round. |
| 7 | Selected-row glyph vs digit | Fixed for the digit. The glyph (row 3 above) stays open. |

Text (names, ARMOR lines, buttons, TOTAL GOLD, TORSO/FEET title) was compared at the same stage scale. Positions match and the
sizes agree within about 5% (both buttons are the same width in both images). This is not a proven divergence at this
resolution, so nothing was changed there.

## Package files required
None new. The fix uses the existing Preview 13 Details art and the package in `.local-inputs/claude-preview13/p/b016/pkg`.

## Verifier script
Quiet jobs file: `C:/Users/adamc/Desktop/workspace/DH_wt/eqtext-scratch/jobs-after.json` (EXE `DH_wt/build-eqtext/dh-foundation.exe`,
cwd `.local-inputs/claude-preview13/p/b016/pkg`, same args as `claude-preview14/equip/r3/jobs-final.json` up to
`--equipment-page-frame 300`, then `--menu-release 305:112:84` for Torso or `305:400:228` for Feet, `--frames 340`).
Run: `quiet_run.ps1 -JobsFile <that file> -Parallel 2`. Expect exit 0 and no digit on the selected row.
Convert PPM with `DH_wt/eqtext-scratch/ppm2png.js`.

## Open risks
- The stacked-count display is an assumption (count shown when quantity > 1). The original stack display was not checked.
- The `inventory_source_tests.cpp` edit is not compiled; the compiled test covers the rule.
- The scratch tools (`crop.ps1`, `grid.ps1`, `ppm2png.js`) live in `DH_wt/eqtext-scratch`, outside the worktree.
