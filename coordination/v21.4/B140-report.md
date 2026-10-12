# B140 Skills page: rank-0 button label and click path

Owner: B140 session. Patch only; no tracked file other than this report was edited, `main.cpp` untouched.
Scratch/evidence root: `.local-inputs/v21.4/b140/` (called `$B` below).

## Deliverables

- Patch: `$B/B140-skills-learn-label.patch` (SHA256 `A182279AE9B4C76F8425D6BB27215162AA0675883F9C22BFEE4AF49165BC6641`). Verified with `git apply --check` (plain and `--ignore-whitespace`) against HEAD `45a0c602` working tree. Files: `features/generic_skills/runtime_skills_menu_v1.cpp`, `runtime_skills_menu_v1_tests.cpp`, `run_runtime_tests.ps1` (stale source list, missing `stat_pages_art.cpp`).
- Patched private EXE: `$B/dh-foundation-b140.exe`, SHA256 `5A569AAD339178866C1787767C61698C3F1B71C8100F769B2533CA381ED294FD` (fresh build dir `$B/build`, Ninja Release, llvm-mingw clang 23.1.3, sources = private copy of `port/` at HEAD + the patch; `$B/src/port`).
- Unpatched baseline EXE built identically from the same private copy: `$B/dh-foundation-baseline.exe`, SHA256 `EFEDCCB923710050AB3C8C4EDFD71B807184408A290CD083064D10D47879B70E`.
- No file in `main.cpp` needs to change. The fix is entirely inside `RuntimeSkillsMenuV1::append`.

## 1. Investigation (original behaviour)

Visual evidence (observed by viewing, v1.0.3 Part 2 video stills in `.local-inputs/fidelity-video-v18/`; second-resolution stills, so no timing claims):
- `reference-398.png` (~398 s): Battle Hardened ([Passive]) selected, its cell is the greyed rank-0 cell (row 2, col 1), Skill points left 1, Current level "Add points to use this skill", Next level "+5 to Def. Rating / +1 to resistances", bottom button reads **Upgrade Skill**. This is exactly the user's screenshot state, and the original shows "Upgrade Skill" for rank 0. The contact sheet `menus-390-477.jpg` shows the same (3rd tile).
- `reference-395/396.png`: Headsplitter (rank 1) selected, button "Upgrade Skill".
- `reference-399.png`: Inner Strength (locked, "Unlocked at level 3", Next empty), points left 1: **button absent**.
- Sheet tile 5 of `menus-390-477.jpg`: after leaving with a staged spend a "Confirm character point allocation?" Yes/No modal appears (already implemented by B103; reproduced below).
- Direct observation: the original has no distinct Learn state. Rank 0, rank >= 1 both show "Upgrade Skill"; locked/max/no-points hide the button.

Logic evidence (existing recovered source, re-read, not re-derived with IDA):
- `port/engine-ui/reference/character-menu-flow-v1/authored-actions.txt`: sprite496 `onShow` (0x21a3b-0x21a91) writes the single string `GAMEPLAYMENUS_ADD_SKILL` to `btn_add/AddText/text`. Selected-cell `onUp` (0x20e9a-0x210a1) sets `btn_add._visible = NativeSkillsTrainSkill(true, skillId, 0)`. `btn_add.onRelease` (0x21b79, per `coordination/v21.3/B122-SKILL-FAERY-CUES.md`) saves once per visit, calls `NativeSkillsTrainSkill` (`Character::IncSkill` 0x3bcc58), `presetAllSkills`, then `MenuSpending`. There is no label branch or rank test anywhere in the SWF.
- Shipped string table (`features/frontend/art/original_art_data.cpp` labels, plus the localization corpus): `GAMEPLAYMENUS_ADD_SKILL = "Upgrade Skill"`; there is no Learn string/symbol anywhere in the corpus.
- Gates (unchanged by this patch): IncSkill = saved rows present, Skill_Points > 0, `RequiredLevel <= level`, rank < CharacterDesign cap, `CanIncrementSkill`; ours: `probe_skill_training_in_session_v1` / `train_skill_in_session_v1` (`runtime_skill_session_training_v1.cpp`).

Conclusion on the label: the user's request is a **deliberate deviation from the original** (not a regression: our current "Upgrade Skill" for rank 0 matches the video). It is implemented as a user-requested PC adaptation and labelled as such in the code comment, so it can be reverted by removing one branch.

Our code (why the label is "Upgrade Skill"): `RuntimeSkillsMenuV1::append` (`runtime_skills_menu_v1.cpp`) projects the authored `GAMEPLAYMENUS_ADD_SKILL` text for every visible button; there is no rank awareness. Click path: `SourceCompositionV1::release` -> `RuntimeSkillsMenuV1::release` -> `original_skill_hit` train zone -> `train_available_` probe -> `PageV1::train` -> `main.cpp` `services.train` (`begin_allocation_visit_v1` baseline save, `train_skill_in_session_v1`, staged until the shared Yes/No dialog) -> `MenuSpending`. Hit zones are vector-shape based on 480x320 authored coordinates (`x*480/width`), independent of window aspect.

## 2. Implementation

In `append`, after the source admission gate decides button visibility: if the button is visible, a row is selected, and its saved rank is 0, the `AddText` value is `GAMEPLAYMENUS_LEARN_SKILL` if the symbol resolver knows it (it does not today), otherwise the English fallback "Learn Skill". Rank >= 1 keeps the source label. Hidden button (locked, no points, max rank, rejected probe) still emits no label or art. No selection (initial `presetAllSkills` state) keeps "Upgrade Skill". Both the symbol-provider path and the no-provider/base-frame path are handled. No change to admission, rank/points mutation, hit regions, saves or sounds.

Isolated test (`runtime_skills_menu_v1_tests.cpp`, production `RuntimeSkillsMenuV1` + `SourceCompositionV1` composition on the real SkillTables): selected rank-1 row -> exactly one "Upgrade Skill", no Learn; same row at rank 0 with admission accepting -> exactly one "Learn Skill", no Upgrade, art present; rank 0 with admission rejected -> no label and no art; rank restored -> Upgrade again. Compiled with `-Wall -Wextra -Werror`.
- Original tests + original source: PASS. New tests + patched source: PASS (`{"validation":"PASS",...}`). New tests against the **unpatched** source: FAIL at the new Learn assertion (negative control). Binaries/scripts in `$B/tests`. The shipped `run_runtime_tests.ps1` did not link at HEAD (missing `stat_pages_art.cpp`); the patch adds it; I ran an equivalent command line from `$B/tests/build_test.sh`, not the .ps1 itself.

## 3. Integrated runtime/visual verification (production `dh-foundation-b140.exe`)

All runs: hidden private desktop via `quiet_click` (copy `$B/tools/quiet_click_safe.ps1` from the B122 fixture, quotes `|` in args), `DH_AUDIO_SILENT=1`, real posted Win32 mouse messages, normal frontend route (`--start-mode menu`, single player/Continue-style with `--save`), isolated copies of the accepted-package `character-slot-1.save` (Knight, level 2, Skill points 1, BashDown rank 1, Hardiness rank 0; `$B/seeds/pts{0,1,2}.save` differ only in `source_skill_points`). User saves, accepted packages and live windows untouched. Window 1200x640 unless noted. Each run dir under `$B/runs/` holds `run.args`, `run.log`, `injector.log` and PNG captures; all PNGs were looked at.

| Run | Seed | Sequence | Observed |
|---|---|---|---|
| `s2-pts1` | 1 point | C, Skills tab, select Battle Hardened (rank 0), click button, click again | `f700.png`: label **LEARN SKILL**, text fits centered. Click: log `Source skill training row=4 rank=0->1 points=0`, `MenuSpending` cue; `f860.png`: cell shows 1, Grey gone, Skill points left 0, button hidden. Second click: `NativeSkillsTrainSkill source admission rejected training`, no change. |
| `s5-wide-pts1` | 1 point | same at 1800x1090 (the user's window shape) | `f700.png` LEARN SKILL; click trains once (rank 1, points 0, button gone), duplicate rejected. |
| `s1-pts2` | 2 points | select Headsplitter (rank 1), then Battle Hardened, 3 clicks | Headsplitter "Upgrade Skill" with 2 points; Hardiness: first click 0->1 (points 1), label then "Upgrade Skill" at rank 1; second click 1->2 (points 0); third click rejected. |
| `s3-pts0` | 0 points | select rank-0 unlocked and a locked cell, click button zone | No button/label with 0 points; locked cell shows Earthstrike, no button; both zone clicks rejected, ranks/points unchanged. |
| `s4-pts1-locked` | 1 point | select chained (level-locked) Earthstrike, click zone twice | No button, no label, points stay 1, both clicks rejected. |
| `s9-rapid-dup` | 1 point | three clicks 90 ms apart | One training, two rejections. |
| `s6-mapping` | 1 point | learn Hardiness, click mapping slot with it selected, then select Headsplitter and click left slot | Passive refused (`Original SkillTable marks this skill non-assignable`); Headsplitter appears in the left slot; middle slot unchanged. |
| `s7/s8` | 1 point | learn, Back, Yes | "Confirm character point allocation?" modal (screenshot), `Stats confirmed`, saved file shows Hardiness rank=1, points 0. |

Baseline (unpatched) EXE, `runs/base1` and `base2` (1200x640 and 1800x1090): same sequence shows "UPGRADE SKILL" for the rank-0 skill (reproduces the report's label) **and the click worked** (`rank=0->1 points=0`, points shown 0, duplicate rejected).

## 4. The "click does nothing" report: honest status

- Not reproduced. In a normal single-player/frontend session (not post-transition), on a fresh load, at two window sizes including the user's shape, with an unlearned passive skill and one available point, the baseline click trains the skill, spends the point, updates the cell and plays `MenuSpending`; the patched EXE behaves identically. Static review of the click path finds no hit-region, enable-state or selection defect.
- The page rendered with the button visible in the user's screenshot, which means `train_available_` (the same `probe_skill_training_in_session_v1` the click uses) accepted the row. The click differs from the render in exactly two further steps that can fail without any on-screen result: `begin_allocation_visit_v1` (a `save_character` to `options.save`, baseline snapshot, `visit.session_owner == &same_session` check) and `train_skill_in_session_v1`'s `world->update_combat_properties`. Failures surface only as `Character menu action diagnostic: ...` on stderr. This is a hypothesis for the post-transition case (it matches B137's "shared post-transition session binding" suspicion), not a finding. I could not test it.
- Post-transition was not exercised: `--player-to _prim_ExitLevelZone@N` with `--transition-confirm/-key` in the frontend route logged `no_armed_exit` (the exit prompt never armed), and the direct `--start-mode swamp` route spends its time in the opening cutscene, so the Skills page after a real Swamp -> Bogwitch transition was **not** reached. Therefore nothing here proves or disproves the click after a transition. If B137 fixes the shared Session binding, re-run `runs/s2-pts1`-style steps after a real transition: if the click still fails, read stderr for the `Character menu action diagnostic` / `Allocation baseline requires the current same character/Session` text.

## 5. Remaining gaps / unverified

- Learn label wording is a user-requested adaptation with an English fallback; no localized symbol exists. The original uses "Upgrade Skill" at rank 0 (video ~398 s).
- Only Knight, level 2 saves; one passive and one active skill were exercised. Max-rank, per-difficulty cap and other classes are covered only by the unchanged admission probe and the unit test (button hidden when admission is false), not by an integrated max-rank capture.
- With nothing selected and points > 0 the button shows "Upgrade Skill" (original `presetAllSkills` gate); a click there is rejected (`requires the source-selected skill row`). Unchanged.
- Observed, pre-existing, not changed: Current/Next level text is blank for rank-0 Battle Hardened and for locked skills (original frame 398/399 shows "Add points to use this skill ..." and "Unlocked at level 3"). `runtime_skills_text_v1.cpp` leaves it blank when the skill script has not been source-audited (`OnSkillInfo implementation has not been source-audited`). This relates to B020 and should be tracked separately.
- Audio was silent (routing cue logged only). Visual timing/animation of the click not evaluated.
- Full ctest was not run (only the Skills-menu unit test and the production runs above).
