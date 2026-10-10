# B056b report: Equipment (Details) page, remaining differences vs the original (follow-up of B056)

Branch `fix/b056b` (from fix/b056). Build `build-fix056b`. Reference: user frame `user-shots/b056-REFERENCE-original-equipment-video-0832.png`
(video 8:32) plus Part 1 frames t=336/342/372/500/506 (extracted to `.local-inputs/claude-preview15/fix056b/vid/`).

## 1. Evidence (direct observation, video frames; no IDA needed: art/tone only)
- Rail (cropped, zoomed): unselected icons are dim grey-brown silhouettes, only the selected icon is bright. The previous dim plate was a stand-in.
  The dark art IS in the atlas: `MenusGraphics_droid.tga` top right (px x704..880, y0..75) holds the full row of dim silhouettes (torso, sword, shield,
  one ring, potion, boots, belt, gauntlet, helm). Exported dark icons (Type3/4/7/8/9) use the same cell size as their Highlight cell, so the five missing
  ones (slots 0,1,2,5,6) are centred on their silhouette with the Highlight cell size. Side-by-side crop: matches the video rail.
- Black dividers (item 6): in the reference there IS a black vertical band right of the list, a thin black horizontal line between the grey and orange
  panels, and black lines above/below the orange selected row (the frame of the two list halves). These are original and stay. What the original does NOT
  have is a band below the Auto-equip row: that grey metal lip across the left column (stage y 46..60) was drawn by main-sheet plate 34 (found by hiding
  roles one at a time with a temporary debug switch, removed again). The area around Auto-equip is dark brown in t=372/t=512. Plate 34 is now dropped under Details.
- List rows: separators are thin light lines under every non-selected row (centre rows 289..291 of the 720p frame, peak ~(122,116,97), fading to both ends,
  x about 72..188 stage); the equipped row shows the small sword glyph left of its name (t=372 'Ceremonial Boots', 8:32 'Useless Blade'); the list frame has a
  rounded top-left corner (upper part) and rounded bottom-left corner (lower part).
- VALUE box (8:32, t=372): dark box, 'VALUE' label, number, and at the right end the sword-over-gold-coins icon (~30 x 27 units), above the Transmute button.
  The icon is atlas cell x450..482, y480..511 (found on the atlas contact crop).
- Tone measurements (pixel means of the text-free list area, 8 columns): reference smooth horizontal ramp ~(12,9,4) at the rail side to ~(52,48,37) near
  the divider, greyer to the right; B056 output was ~(36,27,16) flat to ~(56,46,34).

## 2. Changes (`features/inventory/inventory_details.cpp/.hpp`, tests; `main.cpp` untouched in the final diff)
- `details_replaces_main`: plate 34 is no longer kept under Details (grey lip gone). Plate 177 (dark brown frame) stays.
- `rail_dark_art` / public `details_rail_dark_art`: dark atlas quads for slots 0,1,2,5,6 on the Highlight quad rectangle (clipped at the atlas edge); those slots
  now count as having normal art, so their Highlight is hidden unless selected. B056 dim plate removed.
- `list_damask_lift`: 24 strips solved from the reference ramp (alpha .55) instead of the 8 warm strips.
- `list_frame_outline`: thin lighter outline with the rounded outer corners.
- `row_separator_glow`: light glowing separator over each non-selected row's exported separator.
- `equipped_row_glyph`: header EquipedSwordIcon art at 78% on the equipped row (position from the row Host field matrix).
- `value_box_icon`: sword+coins icon in the idle Transmute variant.
- Tests: `details_row_geometry_tests.cpp` (runs in ctest) checks dark rail art exists exactly for slots 0,1,2,5,6, uv in the atlas silhouette row, quad inside the icon box;
  `equipment_main_page_tests.cpp` updated (every unselected icon now hides its Highlight); `inventory_source_tests.cpp` updated (plate 34 gone, dark rail batches, frame outline;
  compiles with -Wall -Wextra, not run: needs the staged shared assets closure, as in B056).

## 3. Verification
- ctest in `build-fix056b`: 117/119; failures only `session_skill_binding` (known) and `winmm_pump_priority_v1` (0xc0000135, same as B056, unrelated). `schema_v4` failed once, passed on rerun.
- Quiet EXE captures (bag items via `--bag-item` so the list has several rows and an unequipped selection): before `runs/before-*.png` (B056 EXE), after `runs/after-*.png`.
  Contact sheets: `.local-inputs/claude-preview15/fix056b/sheet-rh2.png` (reference | before | after, Right hand, unequipped Flawed Moon selected) and `sheet-ft2.png`
  (before | after | after Right hand equipped). Rail crop comparison `runs/rail-cmp.png`, list crop `runs/list-cmp2.png`. Scripts: `gen4.ps1`, `run4.sh`.

## 4. Remaining gaps (not done, not guessed)
- Red X + green names, disabled EQUIP look: B057. Icon mapping B060, helm B061 (not touched).
- Avatar render differs from the video build; avatar-pane shadow is still the B056 ellipse approximation (the reference has a darker pane gradient).
- Orange panel tone: reference is slightly less saturated at the bottom ((112,87,64) vs ours (105,55,23) in a text-free strip); atlas gradient used as is. Not corrected.
- The VALUE icon is not dimmed like the video's bluish-grey look; its size/position were measured from one frame (8:32). The glyph and separator offsets are approximations.
- Selected row glow art is the exported one (more ragged than the video's smooth gradient).
- Windows 1201x720 only.

## Package files required
None new. `MenusGraphics_droid.tga` and `MenuGraphics02.tga` (already in the Preview 15 assets). EXE from this branch.
