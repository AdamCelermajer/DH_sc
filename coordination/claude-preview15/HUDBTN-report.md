# HUDBTN (I025): PC HUD skill buttons use original button art and real cooldown

Branch `p15/hudbtn`, worktree `DH_wt/hudbtn`. Status: implemented, unit-tested, and verified in isolated quiet runs.
Gaps are listed at the end. The original gameplay cooldown was not observed on screen: no skill with a nonzero source cooldown is in the supplied saves.

## Evidence (investigation)

Visual:
- User screenshot `.local-inputs/claude-preview15/user-shots/hud-skill-circles.png`: plain gold-ringed circles, no art, no cooldown.
- Android/native reference `.local-inputs/publication/checkpoint/port/android-native/reports/native-skill-world-final/gameplay.png`: original bottom row of five rings centred horizontally (pitch about 56 stage px, radius about 24), captions Lv 1 / Empty / Locked / x 5. Used for placement.
- Reference video (actually 640x360, not 720p): `.local-inputs/claude-preview15/hudbtn/ref/full420.png` shows the phone HUD with Faery and skill buttons on the right and attack bottom-right, a different HUD style from the bottom row. Not used for placement (see gaps). The sampled frames (t=60..420 s) show no skill cooldown.

SWF `dqhud_droid.swf` (decoded with `.local-inputs/claude-preview15/hudbtn/dump_buttons.py` and `probe2.py`):
- btn_skill sprite 353: ring base (shapes 167, 168, atlas bitmap), btimg icon holder (245), Grey (249 -> shape 248, solid), hitzone (352 -> 351), CoolDown sprite 350 at (-4,3) twips.
- CoolDown sprite 350: 101 frames; frame 0 empty; frame f >= 1 is one solid shape 249+f, a rim sweep starting at 12 o'clock and running clockwise (frame 10 spans about -97..-48 deg, frame 49 about -97..90, frame 99 nearly full).
- btn_spell sprite 398: ring shape 381 (alpha 0.8), btimg 397, Grey 249, CoolDown 350 at depth 7.
- btn_potion sprite 121: ring shape 109 (bitmap), btframe 105 icon. No CoolDown child.
- Shape 349 (frame 100) is unreachable because FastUpdate clamps to 99; it is not exported.

IDA `InfoHUDManager::FastUpdate` 0x41e064 (pseudocode-all.c around lines 206031-206300):
- Per skill button (v26 0..2), the button's AS slot selects `v125[slot]`, where `v125[slot] = CharAISkillScript::GetCooldown(skill in SG_GetSkillInSlot(slot))`.
- Frame = `(int)(float)(v125*100.0) - 1`, clamped at 0; the spell/other paths clamp at 99.
- `CharAISkillScript::GetCooldown` (line 157558) = `1 - elapsed/total`, 0 when there is no timer. `CharTimers::TMR_Start` (line 158724) sets elapsed=0 at start, so the value is the REMAINING fraction: 1 right after a cast, 0 when ready.
- btn_spell CoolDown is `this+300` (`engine-ui/reference/hud-player-infos/manager/NOTES.md`), driven by the spell script cooldown. Inference: the PC port's 5000 ms Faery/Celest spell clock (`hotty_cast_v1.cpp:424`, `celest_cast_v1.cpp:236`) is used as the same remaining fraction. Not verified in the EXE.
- Potion has no CoolDown child, so no potion cooldown is drawn.

## Expected behaviour (implemented)
- Skill rings are the original btn_skill ring/base art. Empty (unassigned) cells draw the original Grey overlay (shape 248) and no icon.
- Cooldown frame = clamp((int)(remaining*100)-1, 0, 99) of the cell's live skill timer; frame 0 draws nothing. Overlay order matches the original depth order: base, icon, Grey, CoolDown.
- Faery: ring 381 (alpha 0.8), icon as before, CoolDown from the 5000 ms spell clock.
- Potion: ring 109, icon 105, no cooldown.
- The 1-5 key legends stay as the explicit PC adaptation. Hit regions stay as circles; the [2,0,1] mapping is unchanged.

## Changes (commit 93a3fdf1 on p15/hudbtn)
- `tools/export_hud_geometry.py`: additive solid-fill support (fill kind 0, RGB or RGBA), `solid_triangles` output, `ROLES.get` for unknown shapes. Existing bitmap output unchanged; `pc_gameplay_hud_source_art_v1.cpp` regenerates identically.
- `features/generic_skills/export_pc_gameplay_button_art_v1.py` (new): generator, called from `export_pc_gameplay_hud_source_art_v1.py`.
- `features/generic_skills/pc_gameplay_hud_button_art_v1.{hpp,cpp}` (new, GENERATED, about 800 KB): base rings (skill/spell/potion), grey overlay, 100 cooldown frames. Do not hand-edit; regenerate with the exporter.
- `features/generic_skills/pc_cooldown_frame_v1.{hpp,cpp}` (new): remaining-fraction and frame math (pure).
- `runtime_skill_cast_coordinator_v1.{hpp,cpp}`: `skill_cooldown_total_ms_` stored with the ready time; `skill_cooldown_remaining_fraction_v1(actor, skill_table_id)` read accessor.
- `pc_gameplay_hud_v1.{hpp,cpp}`: fitted original button family (ring extent = radius, centre = placement), Grey and CoolDown layers, `faery_cooldown_frame` in the layout, cell cooldown validation (0..99), failure leaves the prior packet intact.
- `main.cpp` `updatePcHud`: original bottom-row placement (centres 128/184/240/296/352, y 270, radius 24, labels under each ring); per-cell cooldown from the coordinator; Faery cooldown from `faeryCooldownClock` (5000 ms). `updatePcHud` already runs every draw frame.
- CMake: new sources in `foundation_runtime_skill_cast` and `dh-foundation`; test scripts list the new files.
- Tests: `pc_gameplay_hud_v1_tests.cpp` moved to role-based checks (ring fit, icon, grey, failure atomicity) plus new cooldown tests (frame math 1.0->99, 0.999->98, 0.5->49, clamps; remaining fraction; skill and Faery wedges by shape 249+f; frame 0 no overlay; potion no overlay; grey on empty cell; out-of-domain frames rejected).

## Tests run
- `run_pc_gameplay_hud_v1_tests.ps1`: PASS.
- `p14_build.ps1 -Name hudbtn -Test` on the committed state: build exit 0; CTest 110/111 pass; only `session_skill_binding` fails (the known worktree `.local-inputs` junction issue).

## Verification in the EXE (quiet hidden-desktop batches via `port/windows-foundation/tools/quiet_run.ps1`)
- Matrix generator: `.local-inputs/claude-preview15/hudbtn/make_matrix.py` (copies the Preview 14 skill saves into `hudbtn/matrix/<save>/`).
- Observed, committed build, no forced cooldown: `hudbtn/cast/out/stack-k2.png` and `hudbtn/matrix/s1/out/stack-force.png` show original gold rings with icons for assigned skill 2 (BashDown), empty skill 1/3 rings, the Faery ring with icon, the potion ring with "5 Potion: 0", and labels 1, 2, 3, "4 Faery", "5 Potion: N".
- Cast evidence: key 2 (slot 0, BashDown) is accepted (`Source skill key=2 slot=0 ... skill=BashDown MP=21.25`, down from 27.25). Source `SnS_Cooldown` for BashDown is 0 (debug print `HUDBTN_DEBUG use cooldown_ms=0.000000 ... row=7`, identical across all six saves), so no overlay is correct for it.
- Keys 1 and 3 are rejected (`Selected source hotbar slot has no valid saved skill row`), so those cells stay empty. Key 4 (Faery) is rejected (`NativeHUDSpell requires the same source-known current FaeryList and difficulty`).
- Overlay progression (verification only, NOT committed): a temporary `cooldown_ms = 3000` force in the coordinator, built and run, then reverted with `git checkout`. Captures `matrix/s1/out/zoom-cooldown.png` (frames 70, 90, 120) show the teal rim sweep shrinking to nothing by frame 120. Quiet runs advance by real dt, so frame numbers are not a clock.

## Package files required
- None new at runtime: the button art is compiled in. The existing `MenusGraphics_droid` atlas (bitmap1) is already packaged for the HUD and character menu. `dqhud_droid.swf` is needed only to regenerate the art.

## Verifier script (quiet, hidden desktop)
1. `hudbtn/make_matrix.py s1 2 70,90,120,160,220,300`, then `quiet_run.ps1 -JobsFile hudbtn/jobs-matrix.json -Parallel 12`. Expected: BashDown ring 2 shows its icon; no cooldown overlay (source cooldown 0); labels as above; no crash.
2. Forced-cooldown check (needs a temporary local edit, do not commit): set `active.cooldown_ms = 3000.0` at the coordinator timer store. Expected: teal rim arc on ring 2 at 70 frames after the cast, shrinking by 120 frames.
3. `run_pc_gameplay_hud_v1_tests.ps1`: expect PASS.

## Open risks and gaps (not verified)
- The original's on-screen cooldown look (colour, rim) comes from the SWF shape colour (teal, alpha 0.8). It was not compared with footage, because no cooldown appears in the sampled reference frames.
- Empty cells look light and pinkish rather than the Android dark "Empty" look. The grey overlay (alpha 0.6) may need comparison with the original Grey state.
- Placement uses the Android bottom-row layout. The reference video shows a different phone HUD (Faery and skills on the right). Which HUDStyle the user's "original iPhone positions" mean is unresolved.
- Faery cooldown mapping (btn_spell CoolDown = spell script vs the 5000 ms clock) is an inference. The Faery cast cannot run in the available saves, so it is not verified in the EXE.
- Potion label bounds (321..383) can clip labels longer than "5 Potion: 99".
- Hit regions remain the 48-segment circles, not the original hitzone shape 351.
- Skill timers use wall-clock dt, so cooldown progression is not frame-deterministic in parallel quiet runs.
