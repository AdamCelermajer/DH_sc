# Portrait regression after B038 (HUD portrait top-left, aperture black)

Status: root cause found and fixed in the exporter. `hud_geometry.cpp` and `reports/hud-source.json` regenerated. Isolated proof done. NOT verified in the integrated EXE: the root must rebuild `dh-foundation.exe` and check the portrait in a fresh window (see section 5).

## 1. Exact cause

The portrait is positioned by an index into the per-style static layer array, not by shape id:

- `port/windows-foundation/hud_geometry.cpp` (generated), `portrait_aperture_centre(style)` and `original_hud_portrait_bounds(style)` both read `static_layers[style][3].matrix`.
- At HEAD, `static_0..3 = {86,143,153,155}`, so index 3 is shape **155** (the dark aperture, `player_overlay_b`). The portrait is translated so its paint centre lands on the aperture of 155.
- B038 added the XP background shape **148** to the static list by inserting it in draw order (between 143 and 153). The list became `{86,143,148,153,155}`, so `static_layers[style][3]` became shape **153** (`player_overlay_a`).
- The portrait was therefore centred on 153's matrix (about 41 SWF px left and 16 px up of the true aperture). The portrait lands in the top-left, and the dark aperture of 155, which was drawn correctly, is exposed as the black empty round frame.

The generated `order_*` arrays also changed (`{0,-1,1,-2,2,3,-3}` to `{0,-1,1,-2,2,-4,3,4,-3}`), but the order itself is not the bug. Only the index-3 anchor is.

Not the cause: compose logic (textually unchanged), `PORTRAIT_BOUNDS`, `portrait_paint_centres`, the portrait layer matrix/`found` selection, and the `FRAME_OVERRIDES` handling of 152 (the `found` selection for portrait is unchanged).

## 2. Diagnostic evidence (diff program)

Program: `.local-inputs/claude-preview12/portrait-fix/dump.cpp`. It prints per-batch index, role, shape id, triangle count, bounding box, and an FNV-1a hash of the exact float bits. It is built twice (HEAD sources in `head/`, current in `cur/`) with `clang++ -std=c++17 -Wall -Wextra -Werror -static`. Output is in `portrait-fix/build/`.

Before the fix, against HEAD (6-argument overload, all 4 styles, HP=MP in {0,49,99}, portrait 0..2): 72 differing batch lines out of 252. Every differing line is a portrait batch (shape 38/39/40). For example, `s0 p0` warrior:

- HEAD: box `[17.752,14.585]-[59.477,55.177]` fnv `0abc96ac25f5ff41`
- broken: box `[-23.664,-0.996]-[18.061,39.595]` fnv `daebdda9d80386d8`

HP, MP, and all static batches (86, 88, 143, 145, 153, 155) were identical to HEAD in the broken build. The 155 aperture was identical, which explains the black frame.

## 3. Fix (exporter, not the generated file)

File: `port/windows-foundation/tools/export_hud_geometry.py`, `main()` layout loop.

```python
static = [(char,m) for char,m in initial if char not in (38,39,40,88,145,148,150)]
static += [(char,m) for char,m in initial if char == 148]
```

The XP background 148 is now appended after the HEAD static layers, so static indices 0..3 are unchanged (index 3 is 155 again). Draw order is unchanged, because the `order_*` generator maps 148 to its new index 4. The generated order is now `{0,-1,1,-2,4,-4,2,3,-3}`: 86, HP, 143, MP, XP background, XP fill, 153, 155, portrait. The XP fill (150) stays at order code -4, as in B038.

The exporter was run with `python.exe -I` and printed `{"shapes": 13, "source_triangles": 40, "layouts": 4, "hp_frames": 100, "mp_frames": 100, "portraits": 3}`.

Also no hand edits to the generated file. `hud_geometry.hpp` and `main.cpp` are unchanged by this fix.

## 4. Proof

- Legacy 6-argument overload: current output vs HEAD for all 4 styles, HP=MP in {0,49,99}, portrait 0..2, is **0 differing lines** (the full dump is identical, including fnv hashes). Portrait fnv `0abc96ac25f5ff41` matches HEAD.
- 7-argument overload: for 108 sampled frames (4 styles x HP=MP in {0,49,99} x XP in {0,49,99} x portrait 0..2), removing the XP batches gives a batch list identical to HEAD's for that frame (`compare.py`: 108 identical, 0 mismatches). Every 7-argument frame contains exactly `xp_background` (148) and `xp_fill` (150).
- XP fill moves with the XP frame (s0, box x-range): frame 0 `[49.8,105.1]`, frame 49 `[76.9,105.4]`, frame 99 `[104.5,105.6]`. This is the expected left-to-right fill, with the cover shrinking to the right.
- `hud-source.json`: top-level keys equal HEAD, non-geometry top-level values equal HEAD, `geometry_export` keys equal HEAD. `native_verification` and `source_font_metrics` under `geometry_export.target.plain_text_layout` are equal to HEAD.

Tests (compiled with the same flags as the runners, outputs in `portrait-fix/build/`, not in `.local-inputs/b038-build`):

- `hud_xp_bar_tests` (equivalent to `run_hud_xp_bar_tests.ps1`, which writes to the B038 folder): `XP bar rendered fill: 0=0.00361011 50=0.498195 99=0.98556; monotonic frames 0..99, carry, cap, wrap and transactional guards passed`. Exit 0.
- `hud_geometry_tests`: `12 original layout/class portraits aligned to ring155; ... passed`. Exit 0.
- `hud_target_tests`: `100 original targetHPframes validated; ... pass`. Exit 0.

Negative check: `hud_geometry_tests` also **passes against the pre-fix broken `hud_geometry.cpp`** (header unchanged). The existing test does not guard the portrait anchor, so it did not catch this regression.

Not run: ninja/cmake on the shared build dir (as instructed). No `main.cpp` syntax check was needed, because `main.cpp` and the header did not change in this step.

## 5. For the root and verifier

- Rebuild `dh-foundation.exe` and launch an isolated window with a fresh save. Check the portrait inside the round frame at the top-left HUD, for each class (warrior, rogue, mage). It should match the preview 11 image (`.local-inputs/claude-preview12/verify-combat/p11/smoke-hud.png`).
- Check the XP strip between the XP background and the HP bar. It should still fill left to right.
- I did not re-open the two evidence PNGs in this pass. The numbers above are the proof for the geometry. The screenshot check is still required.

## 6. Risks and follow-ups

- The index-3 anchor (`static_layers[style][3]`) remains fragile. Any future change in static order silently moves the portrait again. Recommended follow-up: key the anchor by shape 155 in the exporter, or add a regression assertion in `hud_geometry_tests` (portrait batch box equals the 155-anchored value). Not done here, to keep the diff minimal.
- The B038 report's claim that "non-XP HUD geometry is unchanged" was not true for the portrait. The B038 hunk list is otherwise unchanged. `main.cpp`, `hud_geometry.hpp`, and the B038 tests are untouched.

## Files touched

- `port/windows-foundation/tools/export_hud_geometry.py` (static-list line, 3 lines plus comment)
- `port/windows-foundation/hud_geometry.cpp` (regenerated by the exporter)
- `port/windows-foundation/reports/hud-source.json` (regenerated by the exporter; evidence keys preserved)
- `coordination/claude-preview12/portrait-fix-report.md` (this report)

Scratch and evidence: `.local-inputs/claude-preview12/portrait-fix/` (git-ignored).
