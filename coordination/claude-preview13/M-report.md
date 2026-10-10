# Preview 13 Group M report: Skills page level locks (B020), subclass chains (B021), B018 probe

Status: **B020 draw-order defect found and fixed in source (focused test PASS). Not yet verified in the integrated EXE.**
B021 not fixed: no source defect proven, and the specialized-class runtime evidence is missing. B018 still not reproduced
(the points > 0 state was not reached).

## 1. Evidence

### Source data (extracted this session)
Tool: `.local-inputs/claude-preview13/m-skills/tools/dump_skill_trees.cpp` (links `port/game-data/skill_tables.cpp`,
`data.cpp`, `class_tables.cpp`, `properties.cpp`), input `.local-inputs/windows-source-clock-v19-preview-12/assets/original-cache/data/pydata/`.
Output: `.local-inputs/claude-preview13/m-skills/dump.txt`.

- The SkillTable Skill record has no prerequisite-skill field (fields: Anim, AnimIsMoving, DisplayProps, ElementalType,
  FairieDependantText, Flags, Level, Script, SkillAssignable, SkillCurrLevel, SkillDescription, SkillIcon, SkillName,
  SkillNextLevel, Type). The lock/availability gate is therefore only `character level >= Level` (RequiredLevel = word 8).
  There is no data-driven prerequisite chain on the page; the "chain" the page shows is the Lock glyph.
- CharacterTable SkillTree (column 28): KnightPlayerBase 14 (Berserker 15, Paladin 16); MagePlayerBase 21 (Illusionist 22,
  Necromancer 23); RoguePlayerBase 27 (Archer 28, Assassin 29).
- Base lists, positions 0-7 (RequiredLevel / SkillAssignable / icon):
  - Knight (14): p0 BashDown 0/1 sword_explosion; p1 GroundSlam 3/1; p2 Charge 6/1; p3 PotionAddict 9/0;
    p4 Hardiness 0/0 battle_hardened; p5 FastMetabolism 3/0 inner_strength; p6 Leadership 6/0; p7 CombatMaster 9/0.
  - Rogue (27): p0 JumpKick 0/1; p1 ViciousStrike 3/1; p2 Quickness 6/1; p3 Roundhouse 9/1; p4 Ambidextrous 0/0;
    p5 Acrobat 3/0; p6 ComboMaster 6/0; p7 FindWeakness 9/0.
  - Mage (21): p0 ColdRay 0/1; p1 StoneEscape 3/1; p2 Cyclone 6/1; p3 ChainLightning 9/1; p4 StaffMaster 0/0;
    p5 MagisShielding 3/0; p6 ElementalMastery 6/0; p7 FaeryBlood 9/0.
- Positions 8-15 (subclass cells):
  - Base class lists (14, 21, 27) have RequiredLevel 12/15/18/21 but icon `blank` (SpecializationSkill placeholders).
  - Knight Berserker (15): WhirlingDrift 12, Intimidation 15, Berserk 18, Ravager 21 (assignable), Survival 12, KillingStreak 15,
    Toughness 18, KillingFrenzy 21 (passive). Paladin (16): OffensiveAura 12, PurifyingAura 15, GuardingAura 18, DivineJudgement 21,
    Shield 12, EnergyBalance 15, Conversion 18, HolyWeapon 21.
  - Rogue Archer (28): Volley 12, PinShot 15, MultiHitArrow 18, ExplodingArrows 21, BowMastery 12, PoisonArrows 15, SideStep 18,
    Precision 21. Assassin (29): GrapplingHook 12, Blindness 15, PoisonSting 18, Assassination 21, Assassinate 12,
    TreasureHunter 15, Contaminate 18, blade_defect 21. Mage Illusionist (22) and Necromancer (23) follow the same pattern.

### Source rule (IDA-recovered authored actions, `port/engine-ui/reference/character-menu-flow-v1/authored-actions.txt`)
`presetAllSkills` (sprite496, about 0x20c38-0x20dc1):
- `Lock._visible = !SkillUnlocked` for every cell (0x20c9a-0x20cab), where SkillUnlocked comes from NativeGetSkillDetails
  (`level >= RequiredLevel`).
- Only when `SkillLevel < 1`: if the SkillIcon is `blank`, Grey and Lock are both hidden (0x20d53-0x20d8f); otherwise Grey is
  shown (0x20d94).
- `setDisabled(!SkillAssignable)` and `setDisabled(true)` at rank 0 (0x20c79-0x20c92, 0x20d3f).
- The current code (`runtime_skills_menu_v1.cpp` and `skill_page_text_projection_v1.cpp::project_skill_page_cells_v1`) matches
  this rule. The only defect is the draw order below.

### Reference video (observed)
- `.local-inputs/fidelity-video-v18/reference-399.png` (Part 1 timestamp ~399 s, Inner Strength selected, `Unlocked at level 3`).
  Enlarged crop: `.local-inputs/claude-preview13/m-skills/frames/ref399-grid.png`. Observed:
  - Row 0: p0 learned (bright, no overlay). p1, p2, p3 rank 0: greyed icon with a **dark chain/cross glyph drawn on top**.
  - Row 1: p4 (Hardiness, L0) greyed with **no** glyph. p5, p6, p7 greyed with the glyph.
  - Specialization cells (blank): no glyph, rank `0`.
  - This is exactly the source rule at a player level of 1 or 2 (the stated "level 3" lock).
- `reference-396.png` shows the same pattern (`.local-inputs/claude-preview13/m-skills/frames/ref396-grid.png`).
- Observed: no arrows or connector lines between cells in these stills.
- Part 2 (`dh2_video_research/sheets2`, 15:16 long): the character is a Warrior (stats page at 13:20 shows `WARRIOR Lvl 6`).
  The skill_ui code names the three base classes Warrior/Rogue/Mage, so this is the Knight tree (inference). I looked at
  sheets s020, s034, s035, s036. They show gameplay, the Stats page (13:20), the Equipment page (13:24-13:36) and the Quest
  Journal (14:08). **No Skills page was found in those sheets. Sheets s001-s033 and s037-s039 were not scanned.**

### Candidate runtime (observed, `.local-inputs/claude-preview13/c-skills/knight/out/skills-knight.png`, looked at)
- Fresh Knight at level 1 (`--start-mode swamp --fresh-player --actor-row KnightPlayerBase --skills-page-frame 120`): all
  rank-0 nonblank cells are greyed, but **no Lock glyph is visible on p1, p2, p3, p5, p6, p7**. The expected Lock cells are
  listed above. Level-1 Knight: p0 and p4 have no Lock, the rest do (6 Lock cells).
- Enlarged: `.local-inputs/claude-preview13/m-skills/frames/cand-skills-grid.png`.

### Root cause (proven by a failing test, then fixed)
- The model does produce Lock batches (`menu_SkillTreeSheetNew/buttons/skillN/Lock/2`, shape 488, `original_art.cpp`). Its
  extent (x 71-105, y 67-105 in page space) matches the icon cell (Grey x 70-107, y 64-105). Its atlas UVs point into
  `MenusGraphics_droid.tga` at texels 973-991 x 149-183, where alpha > 0 for 400 of 612 texels. The crop shows the chain glyph
  (`.local-inputs/claude-preview13/m-skills/frames/lock1-uv-crop.png`).
- The authored batch order is `btimg/1`, `Lock/2`, `cnt/value`, `Grey/1`. The generic icon presenter (`skill_ui/original_skill_presenter.cpp`
  `icon()`) erases the authored btimg batch and **appends** the source icon batch at the end, so the icon is drawn on top of the
  Lock. The result is a Lock hidden under the opaque icon.
- Evidence: after adding the draw-order check to `runtime_skills_menu_v1_tests.cpp`, `run_runtime_tests.ps1` FAILED at the
  new check (line 263) before the fix. The same test passes after the fix.

## 2. Expected behaviour
- Source rule above. Lock is visible when `character level < RequiredLevel`, only on nonblank cells, and is drawn over the
  icon (and over the Grey tint), as in reference-399.
- Expected at level 1 (Knight): Lock on p1 GroundSlam, p2 Charge, p3 PotionAddict, p5 FastMetabolism, p6 Leadership,
  p7 CombatMaster. No Lock on p0 BashDown, p4 Hardiness, or the blank Specialization cells.
- Expected at level 3: Lock on p2, p3, p6, p7 (p1 and p5 unlock). At level 6: p3, p7. At level 9: none on the base list.
  The same pattern holds for Rogue (ViciousStrike boundary at 3) and Mage.
- Blank specialization cells (level < 1, icon blank): no Lock, no Grey. Above level 12, a Berserker/Archer/Assassin
  cell is a nonblank icon, so Lock follows `level >= RequiredLevel`.

## 3. Changes
- `port/windows-foundation/features/generic_skills/runtime_skills_menu_v1.cpp`, inside `RuntimeSkillsMenuV1::append`, in the
  `if (source_cells_known)` block, right after the per-cell `for (...) states` loop that ends with the `lock_visible` erase, and
  before the `// presetAllSkills initially exposes Add Skill` comment. Adds one `std::stable_partition` that moves visible
  `skillN/Lock/` batches to the end of `next.art.batches` (order preserved). +11 lines.
- `port/windows-foundation/features/generic_skills/runtime_skills_menu_v1_tests.cpp`, the per-row loop after
  `CHECK(lock_rendered == !source_unlocked);` (about line 250). Adds the draw-order check: a rendered Lock must come after every
  `skillN/btimg/` batch. +15 lines.
- No other file touched. No `main.cpp` edit (none needed). No asset, no CMake change.

## 4. Tests
- `powershell -NoProfile -ExecutionPolicy Bypass -File port/windows-foundation/features/generic_skills/run_runtime_tests.ps1`
  - Before fix (test edited, source not): FAIL at `runtime_skills_menu_v1_tests.cpp` line 263 (the new draw-order check); the
    runner exits 1.
  - After fix: `{"validation":"PASS","authored_rows":16,"authored_tree_icons":16,"same_owner_callback":true,"source_composition_e2e":true,"native_vm_required":false,"localized_provider_fixture":true,"live_source_localization_claimed":false}`
- `run_tests.ps1` (generic_skills_page_v1, not changed): PASS `{"validation":"PASS","actual_class_rows":55,"actual_skill_positions":267,"same_state_assignments":1,"same_state_training_provider_calls":1,"native_progression_policy_claimed":false}`
- Other generic_skills runners: `run_runtime_skills_text_v1_tests.ps1`, `run_pc_skill_hud_projection_v1_tests.ps1`,
  `run_pc_skill_input_binding_v1_tests.ps1`, `run_pc_gameplay_hud_v1_tests.ps1` (run in the previous session on this tree, PASS;
  not re-run after this change because none of them compile `runtime_skills_menu_v1.cpp`, verified with grep).
- Not run: an integrated EXE capture (the root builds it). Not run: the points > 0 state (see Uncertainties).

## 5. Uncertainties / not verified
- **Integrated visual not verified.** The fix is verified at the model/test level only. The root must build the EXE and
  capture the level-1 Knight page (see Verifier script).
- **Icon-vs-Lock overlap at level 2+:** the Lock is drawn over the icon, as in reference-399, but I have not compared the glyph
  size or alpha pixel by pixel against the reference. The Grey tint is drawn after the icon batch; the Lock is now after the
  Grey tint too, which matches the reference (the glyph is visible, not tinted). Fidelity should confirm.
- **Lock action is not a click target.** Not investigated.
- **B020 level matrix (L2-L4):** not run in this session. The earlier `make_profile` matrix (in `B020-evidence-20261010.md`) did not
  launch within 30 s. The Rogue boundary at ViciousStrike (3) follows the same rule, but no Rogue capture exists.
- **B018 (Upgrade label, points > 0):** not reached. The fresh route sets `source_skill_points` from the source stat sheet
  (`main.cpp` ~1259), which is 0 at level 1. The earlier 1-point profile did not change the page. Still open.
- **B021 (subclass chain art/icons):** not fixed, no defect proven. The data is in place (Berserker/Paladin/Archer/Assassin
  icons and RequiredLevel 12-21). The page only shows blank Specialization placeholders at level 1. No specialized-class
  capture was made, and the Part 2 video was not searched for the Warrior's (Knight's) Specialization page at level 12+.
- **Prerequisite chain:** the source has no prerequisite field. I did not find any arrow or connector art on the reference
  stills. If the user expects connector arrows, that art needs a source (not found here).
- **Candidate folder moved:** `windows-source-clock-v19-preview-12-candidate` no longer exists, so the earlier capture used a
  stale path. The Preview 12 package (`...-preview-12/assets`) and the Preview 13 candidate share the same
  `MenusGraphics_droid.tga` hash.

## Package files required
- `assets/data/3d/textures/MenusGraphics_droid.tga` (menu atlas holding the Lock texels). SHA256 prefix `c75e8f4df8a9b28f`.
  Present in `windows-source-clock-v19-preview-12/assets` and `windows-source-clock-v19-preview-13-candidate/assets` (same hash).
  The fix adds no new file.
- `assets/original-cache/data/pydata/skills_pyarray.bin`, `skills_pyarraynames.bin`, `skills_pystructnames.bin`,
  `character_properties_*` (SkillTable and CharacterTable data), already present.

## Verifier script (integrated EXE)
- Launch with a fresh isolated save folder (copy `c-skills/knight/skills.args`, point `--assets` at the integrated candidate `assets`,
  and `--capture` at a new folder). Keep `--fresh-player --actor-row KnightPlayerBase --start-mode swamp --skills-page-frame 120 --frames 200`.
- Expected frame: Class skill grid shows Lock glyphs over greyed cells p1, p2, p3, p5, p6, p7 (row 0 cells 2-4 and row 1 cells 2-4),
  no glyph over p0 (learned) or p4 (Hardiness), no glyph over the blank Specialization cells. Skill points left 0, no Upgrade Skill.
- Compare to the Preview 12 candidate capture: the same frame has no glyphs (the bug).
- Log (`--frames` run): no new error lines. Test: `run_runtime_tests.ps1` PASS.
