# Preview 13 Group C report: Skills page and skill slots (B018 priority, B020 and B021 stretch)

Status: IN PROGRESS (skeleton written early; sections completed as work proceeds).

## 1. Evidence

### Visual (observed)
- Preview 12 candidate EXE: `.local-inputs/windows-source-clock-v19-preview-12-candidate/dh-foundation.exe`
  (not modified; run from an isolated folder `.local-inputs/claude-preview13/c-skills/`).
- Knight, fresh player, HUD (`knight/out/hud-knight.png`, frame 300): PC skill circles labelled 1, 2, 3 left to right.
  Circle 2 shows the Headsplitter icon; circles 1 and 3 are empty, with no label or icon. `4 Faery` and `5 Potion: 0` present.
- Knight Skills page, 0 skill points (`knight/out/skills-knight.png`, frame 120): Class skill row 0 position 0 is Headsplitter
  at rank 1; all other class cells rank 0; Skill Mapping shows empty left, Headsplitter middle, empty right; no Upgrade Skill
  button and no label (correct for 0 points).
- Reference video stills (existing, `.local-inputs/fidelity-video-v18/`):
  - `reference-396.png` (Headsplitter selected, 1 point): Skill Mapping left empty, Headsplitter middle, right empty; Upgrade Skill
    frame and label visible; Skill points left 1.
  - `reference-399.png` (Inner Strength selected, below level 3): Skill Mapping unchanged; Current Level "Unlocked at level 3";
    Next Level empty; Upgrade Skill absent; points left 1.

### Logic (IDA)
- `NativeHUDSkill` (`pseudocode-all.c` line ~228110, FUNCTION 0043e734): reads the slot index from the ActionScript argument
  (`to_int`), calls `Character::CTRLIsAllowed`, then `Character::SG_GetSkillInSlot(player, slot)`. If the result is -1 the
  function returns with no `BeginSkill`/`EndSkill` call. An empty slot therefore rejects before AI and mana, and does not shift later slots.
- `features/generic_skills/pc_skill_hud_projection_v1.cpp`: physical key position to source slot is `{2, 0, 1}`
  (key 1 -> source slot 2, key 2 -> slot 0, key 3 -> slot 1); empty assignments leave the cell unassigned.

## 2. Expected behaviour
(to be completed)

## 3. Changes
(none yet)

## 4. Tests
(to be completed)

## 5. Uncertainties / not verified
(to be completed)

## Package files required
(to be completed)

## Verifier script
(to be completed)
