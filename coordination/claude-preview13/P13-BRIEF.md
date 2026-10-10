# Preview 13 worker brief (read AFTER ../claude-preview12/COMMON-BRIEF.md)

COMMON-BRIEF.md still applies in full (rules, evidence sources, build restrictions, budget, report format), with these changes:
- Repo HEAD is `f6b7f134` (Preview 12 candidate, committed). Reports go to `coordination/claude-preview13/<GROUP>-report.md`; your scratch/frames go to `.local-inputs/claude-preview13/<group>/`.
- Your bug rows are in `docs/BUGS-AND-IMPLEMENTATION.md` section 2 (read ONLY your rows, they are long). Do not edit the tracker; the root does.
- Reference for what Preview 12 changed: `docs/CLAUDE-BUG-INVESTIGATION-B037-B041-2026-10-10.md` and `coordination/claude-preview12/*-report.md` (skim only what touches your area).
- The latest Preview 12 candidate EXE is `.local-inputs/windows-source-clock-v19-preview-12-candidate/dh-foundation.exe` (a built copy to run for A/B; never modify that folder). To see the CURRENT source behaviour you cannot rebuild; the root builds the integrated EXE after all workers finish, so rely on focused tests plus the candidate EXE for baseline behaviour.

## Lessons from Preview 12 (apply them)
1. **Package completeness.** Twice a fix "worked" in tests but the packaged game lacked a file (swoosh .bdae files, music .vxn). In your report add a section **"Package files required"** listing every asset file (path relative to `assets/` or `audio-assets/`, source location, SHA256) the fix needs at runtime, and confirm each is present in the Preview 12 candidate package; if not, say so explicitly.
2. **Regenerated/generated files.** If you run an exporter that regenerates source (e.g. tools/export_hud_geometry.py -> hud_geometry.cpp), diff against HEAD afterwards: unrelated output must be byte-identical. A previous change silently shifted a layer index and broke the portrait.
3. **Regression runners.** Before you finish run every standalone runner that exercises code you touched (e.g. features/combat/run_*_tests.ps1, features/generic_skills/run_*.ps1, run_target_retention_regression_tests.ps1, run_b0xx_*). A previous change broke `combat_session_combo` and the target-retention test; both only showed up in the integrated build. If you change combat_session.cpp, also reason about `tests/combat_session_combo_tests.cpp`.
4. **Never trust a threshold-based visual check alone.** Look at the actual images; use more than one colour/contrast measure.
5. **Say plainly what is not verified.** The verifier and the root rely on it.

## Verification handoff
Your report must end with a **"Verifier script"**: exact EXE arguments (the option names from main.cpp's parser), what frames/log lines to capture, and the expected observable result, so a verifier can reproduce it on the integrated EXE without reading your code.

## Wave 2 update
- A WIP integrated EXE containing wave-1 changes (arrow ordering fix, equipment Details backgrounds, Faery HUD icon, music logging) is at `.local-inputs/windows-source-clock-v19-preview-13-candidate/dh-foundation.exe` (SHA256 AA4442AD…; manifest/receipt there still say Preview 12). Use it to A/B your area against `.local-inputs/windows-source-clock-v19-preview-12/dh-foundation.exe`. Never modify either folder; run from your own copy with your own saves. The older `...-preview-12-candidate` path no longer exists.
- The root rebuilds the main EXE; you still must not run ninja/cmake on the shared build dir.
- Wave 1 changes that may interact: group A edited features/frontend/input/screen_interaction.cpp; B edited features/inventory/inventory_details.cpp; D edited features/audio/*; H added two lines in main.cpp near the PC HUD (`active_faery_id`).
- **Hit/knockback history (user, 2026-10-10):** an earlier version knocked the hero back on EVERY hit; the user had that fixed. Clean hits (result flag 0x0) must never push. Only push-bearing hit results (e.g. Lizard 0x98, controlled 0x88) may push. Bit 0x8 means critical, not "clean".
