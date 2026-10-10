# B056 report: Equipment (Details) page vs the original (B042 update 2)

Branch `fix/b056` (from windows-foundation = Preview 15). Build `build-fix056`, quiet EXE captures (hidden desktop, silent).

## 1. Evidence
Visual: user reference `user-shots/b056-REFERENCE-original-equipment-video-0832.png` (video 8:32, v1.0.3) and video frames
t=336/342/372/500/505/512/520 of Part 1. Direct observations (reference vs Preview 15 EXE `user-shots/b056-equipment-righthand.png`
and my reproduction `.local-inputs/claude-preview15/fix056/runs/before-rh.png`):
- Right side: upper panel is a vertical grey gradient (sampled ~(46,40,31) at the top to ~(108,101,82) at the divider), lower panel an orange
  gradient (~(143,100,62) to ~(94,59,26)); ours showed the dark damask there.
- Left list: one continuous crisp leaf damask (mean ~(44,39,27), darker toward the rail) to the vertical divider; ours showed the stretched
  streak plate with a seam at stage x 172.85 and a flat gap.
- A soft dark ground shadow under the avatar, clipped to the avatar pane (stage x 219..322, bottom 260.8); ours had none.
- Unselected rail icons are dark; ours showed icons 0,1,2,5,6 bright.
- The thin black dividers exist in the original too (same positions as ours, EQTEXT/Q-report); they are not removed. The avatar is drawn over them already.
Logic: no IDA needed for this fix (art only). Key finding: the panel art is NOT missing, it is in textures that the SWF export did not wire:
`MenusGraphics_droid.tga` (hudTexture) has the grey block at px x 361..421, y 262..391 and the orange block at x 428..488, y 262..403;
`MenuGraphics02.tga` (present in the assets, never loaded by the engine) holds the crisp damask picture (motif pitch ~85 texels).

## 2. Root cause
The v1.0.2 SWF Details display list has no grey/orange layers and plate 34 samples a one-texel-high strip of the damask fill, so
the exported page lacked the panels and damask. Nothing drew them.

## 3. Change (all in `features/inventory/inventory_details.cpp/.hpp`, 4 clearly commented hunks in `main.cpp`)
- `details_panel_art`: grey and orange panels as textured quads from MenusGraphics_droid, drawn above the main plates and below all authored Details art (dividers, buttons, text stay on top).
- `mirrored_damask`: the list panel is mirror-tiled from MenuGraphics02 (`details_list_damask_role()`), plus `list_damask_lift` (8 warm translucent strips; the fixed-function overlay cannot tint above 1.0 and the picture is darker than the reference).
- `avatar_shadow`: 12 concentric translucent ellipses clipped to the avatar pane, drawn after the orange panel and before the avatar.
- Rail: a translucent dark plate over each unselected icon that has only Highlight art (stand-in for the missing normal-state art; EQRAIL open gap).
- `main.cpp`: loads `MenuGraphics02.tga` (`menuDamaskTexture`) and draws the damask batch with it; skipped if the texture is missing.
- Test: `inventory_source_tests.cpp` checks the panel/damask batches exist, are below the authored Details art, shadow/lift solids exist, then strips them so the authored-solid order check is unchanged (compiles with -Wall -Wextra; NOT run, needs the staged shared assets closure, not in ctest).

## 4. Verification
- Full ctest in `build-fix056`: 117/119. Failures: `session_skill_binding` (known worktree junction issue) and `winmm_pump_priority_v1` (exit 0xc0000135, missing DLL in this shell; unrelated to this change, not investigated).
- Quiet EXE captures: before `runs/before-{rh,feet,torso}.png`, after `runs/a6-{rh,feet,torso}.png`; contact sheet `.local-inputs/claude-preview15/fix056/sheet-ref-before-after.png` (reference | before / after | after Ring 1). Recipe: `fix056/gen.ps1` + `run.sh` (b016 `--equipment-page-frame 60 --menu-release 80:112:Y`, bagA save).
- After: grey and orange panels, continuous damask list, avatar shadow, dimmed rail icons visible.

## 5. Remaining gaps (not done, not guessed)
- Red X + green name for unusable rows, and the equipped sword glyph in list rows: row art not exported; usability logic belongs to B057.
- Row separator lines, rounded list frame border, VALUE box content (icon + value, needs the transmute value provider), disabled EQUIP button look.
- Rail normal-state art for icons 0,1,2,5,6 (dim plate is a stand-in; its rectangle is faintly visible).
- Damask brightness/contrast is approximate (lift strips); shadow shape approximates the reference. Avatar render/pose differs from the video (different build).
- Only checked at 1201x720 on the Windows build; Android must ship `MenuGraphics02.tga` and load it for the damask (the page degrades to no list damask if absent).

## Package files required
`assets/data/3d/textures/MenuGraphics02.tga` (already in the Preview 15 assets; now actually used). EXE from this branch.
