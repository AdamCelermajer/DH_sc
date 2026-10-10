# Preview 13 Group C report: Skills page and skill slots (B018 priority; B020, B021 stretch)

Status: B018 NOT reproduced in the reachable candidate states and NO source change made. Remains open for the
states I could not reach (skill points > 0 with a selected row; Rogue). B020 and B021: investigation and
evidence review only; no runtime verification in this session.

## 1. Evidence

### Candidate build used
- EXE: `.local-inputs/windows-source-clock-v19-preview-12-candidate/dh-foundation.exe`, SHA256 `1d43942ecd52...` (prefix shown).
- Run from isolated folders `.local-inputs/claude-preview13/c-skills/{knight,knight-p1,rogue}/`. Args and saves were copied;
  assets/audio paths point at the candidate folder. Candidate folder not modified.
- Working tree at start: HEAD `f6b7f134`; my source files untouched.

### Visual (observed, I looked at the images)
- `knight/out/hud-knight.png` (fresh Knight, HUD, frame 300, `--hud`): PC circles labelled `1`, `2`, `3` left to right.
  Circle 2 shows the red Headsplitter-style icon; circles 1 and 3 are empty (no icon, no stale label). `4 Faery` and
  `5 Potion: 0` present.
- `knight/out/keys-knight.png` (same run, after key presses 1 at f100, 2 at f200, 3 at f300, frame 420): circle 2 still has the
  icon, 1 and 3 still empty, no circles shifted. Mana bar visibly lower after the key-2 cast.
- `knight/out/skills-knight.png` (Skills page, `--skills-page-frame 120`, 0 points): Class skill row 0 cell 0 has the icon and
  rank `1`; other class cells rank `0`; Skill Mapping shows empty left, icon middle, empty right (matches the HUD circle 2
  icon); Skill points left `0`; no Upgrade Skill button and no button label. Specialization cells show `0`.
- Reference stills (existing, `.local-inputs/fidelity-video-v18/`), viewed:
  - `reference-396.png` (~396 s): Skill Mapping left empty, icon middle, right empty; bottom Upgrade Skill frame and label visible;
    Skill points left `1`.
  - `reference-399.png` (~399 s): Inner Strength selected; Current Level `Unlocked at level 3`; Next Level empty; Upgrade Skill
    absent; points left `1`.
  - Interpretation: the candidate's Skill Mapping layout (left/middle/right) matches the original's layout in both stills.
    The 0-point candidate state has no Upgrade button, which is consistent with the original's point gate.

### Logic (IDA)
- `NativeHUDSkill` (`.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode-all.c`, function
  `0043e734`, around line 228110): reads the slot argument (`to_int`), checks `Character::CTRLIsAllowed`, then
  `Character::SG_GetSkillInSlot(player, slot)`. Result `-1` returns before `v2Controller::Cmd_BeginSkill`, so an empty slot
  is rejected before AI and mana and does not alter later slots.
- Candidate source mapping `port/windows-foundation/features/generic_skills/pc_skill_hud_projection_v1.cpp`: physical
  key position to source slot `{2, 0, 1}` (key1->slot2, key2->slot0, key3->slot1). Matches the observed behaviour below.

### Runtime key behaviour (candidate, Knight, `knight/out/keys-knight.log`)
- `Source skill key=1 slot=2 frame=100 ... skill= diagnostic=Selected source hotbar slot has no valid saved skill row`:
  empty slot rejected.
- `Source skill key=2 slot=0 frame=200 generation=1 phase=1 skill=BashDown MP=21.25`: the learned slot cast.
- `Source skill key=3 slot=1 frame=300 ... skill= diagnostic=Selected source hotbar slot has no valid saved skill row`:
  empty slot rejected; MP unchanged (21.25 before and after).
- So: an empty slot rejects, no later slot shifts, and no mana is spent on empty-slot presses.
- Note: the learned skill's name is `BashDown` (the Knight's rank-1 row). The Skills-page and HUD icons agree with each other.

## 2. Expected behaviour
- Empty skill slot: no label, no action, rejected before AI and mana; never shifts other slots.
- Learned slot: its icon in the HUD circle and in Skill Mapping; cast on its key.
- Skills page at 0 points: no Upgrade Skill frame or label; with points > 0 and a selected row, one localized Upgrade Skill
  label, and it must disappear when the same-state training probe rejects (already in HEAD per
  `features/generic_skills/README-runtime_skills_text_v1.md` "B018 action label cleanup").

## 3. Changes
- None to source. My scratch files live under `.local-inputs/claude-preview13/c-skills/` (git-ignored):
  - `tools/set_points.cpp` and `tools/set_points.exe`: fixture helper, clones a profile and sets `source_skill_points`.
    It compiles against `save_store.cpp` and `character_state.cpp`. It works only with the llvm-mingw `bin` on PATH.
  - `knight-p1/character-p1.save`: a 1-point Knight profile. It did NOT change the Skills page points (see Uncertainties).

## 4. Tests
- Existing generic_skills runners on the current tree (run in background, PowerShell `-File`):
  - `run_runtime_tests.ps1`: PASS; JSON `{"validation":"PASS","authored_rows":16,"authored_tree_icons":16,"same_owner_callback":true,"source_composition_e2e":true,...}`
  - `run_runtime_skills_text_v1_tests.ps1`: PASS; `runtime skills text PASS: 73 actual localized names, 73 actual base descriptions, 0 FaeryDependantText rows; 42 source SkillInfo current/next rows...`
  - `run_pc_skill_hud_projection_v1_tests.ps1`: PASS; `left/middle/right labels1/2/3 -> source slots2/0/1; middle key2 JumpKick; shape hits emit keys once; saved rows unchanged`
  - `run_pc_skill_input_binding_v1_tests.ps1`: PASS; `physical keys1/2/3 -> source slots2/0/1 once; saved rows and native slots unchanged`
  - `run_pc_gameplay_hud_v1_tests.ps1`: PASS; `physical hit order1/2/3, Faery4, same-state potion count...`
  - `run_runtime_skills_menu_v1_tests.ps1`: no such file; the menu composition is covered by `run_runtime_tests.ps1`
    (`runtime_skills_menu_v1_tests.cpp`), which passed.
- Candidate EXE runs: `knight/out/hud-knight.log`, `knight/out/skills-knight.log`, `knight/out/keys-knight.log` (see section 1).
- Rogue runs failed before reaching the HUD (see Uncertainties): `rogue/out/keys-rogue.log` and `keys-rogue2.log`
  report `Combat initialization: Selected source phase has no damage marker: RoguePlayerBase/attack_offhand`.
  The Knight-based swamp args route does not carry a valid Rogue attack-marker set.
- Not run: any new focused B018 test. No defect was found to fix.

## 5. Uncertainties / not verified
- **B018 Upgrade label with points > 0 is not reproduced.** Fresh-player bootstrap takes `source_skill_points` from the source
  stat sheet (`main.cpp` ~1259), which is 0 at level 1. My 1-point profile did not reach the page (`--save` did not take effect in
  this combat/fresh route, inferred from the Skills log still saying `points=0`; not confirmed in code). The selected-row admission path and the Upgrade label are therefore
  unverified in the candidate. The tracker already lists this as "Normal HUD label/circle capture still open".
- **Rogue not captured.** The Knight swamp args route cannot start a Rogue combat setup (see Tests). The Rogue HUD and
  Skills page need a profile route (the B020 level-matrix notes: menu start with a saved profile).
- **After reload / after training not verified.** Only the 0-point state and the in-session key presses were run.
- **Possible B020 observation (unverified):** at level 1 the Knight Skills page (`skills-knight.png`) shows no Lock icon on any
  class cell. Whether this is correct depends on the Knight SkillList `RequiredLevel` values, which I did not extract. Do not
  treat it as a bug or as correct until the RequiredLevel values are checked.
- **B021 (subclass art) not attempted.** No specialization save or runtime was run this session. The page shows only the base
  Knight/Rogue placeholders in the blank specialization cells.
- Package files required: this session changed no assets. Icons used by the candidate page and HUD came from the candidate folder
  `assets/` (`ui-assets/` also present). No new package file is required by an unchanged fix; if a future B018 patch changes
  the Upgrade label, it reuses the existing `menu_SkillTreeSheetNew/btn_add/AddText/text` field.

## Verifier script (for the integrated EXE; this session changed nothing)
- Knight HUD and empty-slot check (uses the candidate args file pattern; copy `swamp.args` and point saves at a fresh folder):
  `dh-foundation.exe --startup-config keys.args` with `--skill-key-frame 100:1 --skill-key-frame 200:2 --skill-key-frame 300:3
  --frames 420 --capture <out>/keys.ppm`.
  - Expected log: `Source skill key=1 ... skill= diagnostic=Selected source hotbar slot has no valid saved skill row`;
    `Source skill key=2 slot=0 ... skill=BashDown`; `Source skill key=3 ... diagnostic=...no valid saved skill row`.
  - Expected frame: circles 1 and 3 empty, circle 2 with the icon, no shifted circle.
- Skills page: add `--skills-page-frame 120 --frames 200 --capture <out>/skills.ppm`.
  Expected: Skill points left 0, no Upgrade Skill frame/label, Skill Mapping middle icon matches HUD circle 2.
- Still needed on the integrated EXE: a profile with points > 0 and a selected row (accepted and rejected branches), plus a
  Rogue profile through the menu route.
