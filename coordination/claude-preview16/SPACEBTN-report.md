# SPACEBTN report (Preview 16, branch p16/spacebtn, from p16/integrate 2d703432)

Status: SKELETON. Sections are filled as each step lands.

## 1. Investigation (evidence)
- Reference video (Part 1, v1.0.3): frames extracted to `.local-inputs/p16-runs/spacebtn/ref/` (at-206.5, at-208.5, crop strip 205.0-213.0 s of the bottom-right button).
  - 205.0-205.5 s: chest tutorial caption ("Keep an eye out for Epic Chests...") with SKIP, HUD hidden.
  - 206.5 s: caption still up, HUD hidden.
  - 207-208 s: HUD back; bottom-right orange ring button shows a sword icon (attack) with no chest icon in the crops.
  - 208.5 s: full HUD, sword button, chest visible ahead of the player.
  - Open point: the chest icon mentioned in the brief (205.5-208 s) is NOT yet seen in these crops. Needs a finer pass.
- Source logic: CONTEXT-report sections 1.1-1.5 (OOI query, type dispatch, icon table).

## 2. Implementation
(pending)

## 3. Isolated tests
(pending)

## 4. Integrated runtime verification
(pending)

## 5. Gaps
(pending)

## Placeholders
(pending)

## Package files required
- None yet.

## Verifier script
- Build: `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name p16spacebtn -Jobs 6 [-Test]`
