# B140 independent review: Skills page rank-0 "Learn Skill" label

Reviewer: independent B140 reviewer. Repo `C:/Users/adamc/Desktop/workspace/DH_sc`, branch windows-foundation, HEAD `45a0c602`.
No tracked file was edited and the patch was not applied to the primary checkout (`git status` shows only the pre-existing docs/BUGS-AND-IMPLEMENTATION.md change and untracked coordination/.claude files). Scratch evidence: `.local-inputs/v21.4/b140-review/` (test binaries, build logs, outputs, patched/original source copies; the 1.3 GB scratch source tree was removed after use).

## Verdict: ACCEPT WITH CAVEATS (no required changes to the patch)

The label change is correct, narrowly scoped, and verified. Caveats are limited to scope/localization notes below; none block integration.

## Inputs checked

- Patch SHA256 = `A182279AE9B4C76F8425D6BB27215162AA0675883F9C22BFEE4AF49165BC6641` (matches). `git apply --check` against HEAD: OK.
- `dh-foundation-b140.exe` SHA256 = `5A569AAD339178866C1787767C61698C3F1B71C8100F769B2533CA381ED294FD` (matches report).
- `dh-foundation-baseline.exe` SHA256 = `EFEDCCB923710050AB3C8C4EDFD71B807184408A290CD083064D10D47879B70E` (matches report).
- The report's private source copy `b140/src/port` equals a fresh `git archive HEAD` plus the patch (compared with CRLF normalised; no content differences in windows-foundation, game-data, engine-ui, level-world, script-runtime; the only extras are build/pycache directories). So the patched exe is plausibly built from exactly this patch (not independently rebuilt; behaviour in screenshots is consistent).

## (1) Scope of behavioural change: PASS

Read in the patched `RuntimeSkillsMenuV1::append`:
- `show_train_button` is computed and the admission probe (`train_available_`, i.e. `probe_skill_training_in_session_v1`) runs exactly as before; the new `learn_label` block is inserted after that and only reads `show_train_button`, `page_.selected_position()` and `view.rows[sel].saved_rank`. It writes nothing except the local bool `learn_label`.
- `learn_label` is true only if `show_train_button` is already true AND a row is selected AND saved rank is known and == 0. Hence: locked, no-points, rejected-probe, max-rank, no-selection (initial preset state) and unknown-rank cases are unchanged (button hidden / "Upgrade Skill" respectively). There is no active/passive branch anywhere in the diff, so it cannot change active-vs-passive behaviour.
- The only effect of `learn_label` is the value pushed for the single field path `menu_SkillTreeSheetNew/btn_add/AddText/text`, in both the symbol-provider path and the base-frame (no provider) path. Art, hit zones, `release`, `train`, saves, sounds, ranks and points are untouched (the diff touches only `append`; `release`/`original_skill_hit`/`services.train`/`main.cpp` are unchanged). No click admission can change because label text is not an input to any gate (hit regions are vector shapes, see (2)).

## (2) Localization fallback and other consumers of the text: PASS (one caveat)

- `learn_skill_label` asks the symbol provider for `GAMEPLAYMENUS_LEARN_SKILL` first. In production (`main.cpp` ~4243) the provider is `MenuLocalization::symbol`; for an absent symbol `Localization::native_string` returns true with `found=false`, and `MenuLocalization::symbol` converts that to `false` ("symbol absent"), so the fallback "Learn Skill" is used; a missing/empty/unbound provider also falls back. A localized symbol, if one is ever added, wins automatically. Absent-symbol lookup has no side effects beyond the same sheet scan the other per-frame symbol lookups do; no "symbol absent" diagnostic appears in any run log.
- I confirmed no `LEARN_SKILL` symbol or "Learn Skill" string exists in the shipped assets (android assets/windows assets). Existing `GAMEPLAYMENUS_ADD_SKILL` projection (`skill_page_text_projection_v1.cpp`) is unchanged; that file and its test still expect "Upgrade Skill" for the projection itself, which stays correct.
- Consumers of the text "Upgrade Skill" / `btn_add/AddText`: only `runtime_skills_menu_v1.cpp` (this class), `runtime_skills_menu_v1_tests.cpp`, `skill_page_text_projection_v1(.cpp/_tests)`, `original_art*.cpp` data, and report/log artefacts. `skill_ui.cpp` only hides `btn_add` art; `main.cpp` has no reference to `btn_add` or the text; Faery menu code references neither; hit tests are vector-geometry zones (`export_skill_art.py` `HitKind::train`) with no text dependence; no tool, script or ps1 matches on the string. Android `CharacterPanel.java` already uses "Learn skill"/"Train skill" on level 0, a separate code path, unaffected.
- Caveat A (localization, minor): for non-English language packs the rank-0 button would read English "Learn Skill" while rank >= 1 stays localized, until a `GAMEPLAYMENUS_LEARN_SKILL` string is supplied. Acceptable as a user-requested adaptation for the current English-only shipped build; keep the code comment as the tracking note.

## (3) `run_runtime_tests.ps1` extra `stat_pages_art.cpp`: PASS

- Without it the Skills-menu test fails to link at HEAD (I reproduced: `undefined symbol ... original_stat_page_hit_zones / original_stat_page_art / original_stat_element_art`). `CMakeLists.txt` line 840 already lists `features/character_menu/stat_pages_art.cpp` in `foundation_data`, so adding it to the test script is consistent with production linking and harmless (only adds a translation unit; no behaviour change).
- I ran the patched `run_runtime_tests.ps1` itself from a scratch tree (assets junctioned to the real data dir): compile with `-Wall -Wextra -Werror` OK, output `{"validation":"PASS",...}`, exit 0.

## (4) Screenshots (`.local-inputs/v21.4/b140/runs/`): PASS, all match the report

- `s2-pts1/f700.png`: Battle Hardened (passive, rank 0, 1 point) selected: button reads LEARN SKILL, centred, fits.
- `s2-pts1/f860.png`: after click: cell shows 1, grey removed, points 0, button gone. Log: `Source skill training row=4 rank=0->1 points=0`, then duplicate click `admission rejected training`.
- `s5-wide-pts1/f700.png` (1800x1090): same LEARN SKILL at the wide shape.
- `s1-pts2/f560.png`: Headsplitter (active, rank 1, 2 points): UPGRADE SKILL with Current/Next level text. `f640.png`: Battle Hardened after first point (rank 1, points 1): UPGRADE SKILL. `f730.png`: rank 2, points 0: no button. `f480.png`: nothing selected, 2 points: UPGRADE SKILL (preset gate, unchanged).
- `s3-pts0/f740.png`: rank-0 skill with 0 points: no button/label. `s4-pts1-locked/f740.png`: locked Earthstrike with 1 point: no button/label.
- Run logs (s1/s2/s3/s4/s5/s9, base1) show the training/rejection counts stated in the report; baseline exe reproduces UPGRADE at rank 0 with the identical click result, so the click path is not changed by the patch.

## (5) Unit test re-run: PASS (reproduced independently)

Built in a scratch copy of HEAD with the exact command from the report (clang++ llvm-mingw, `-std=c++17 -Wall -Wextra -Werror`):
- original tests + original source: PASS (`validation":"PASS"`).
- new tests + patched source: PASS.
- new tests + UNPATCHED source (negative control): FAIL at test line 359 (`add_label_count(learn_frame,"Learn Skill")==1`), i.e. exactly the new Learn assertion.

## Caveats / notes for the integration lead (non-blocking)

1. Deliberate deviation from the original: the original uses "Upgrade Skill" at rank 0 (video ~398 s; SWF has no rank branch). The code comment and report label this a user-requested adaptation; keep it recorded as such so it can be reverted by removing one branch.
2. This patch resolves the label only. The "click does nothing" part of the user's report is NOT reproduced and NOT addressed (baseline and patched trained correctly in a normal frontend session, two window sizes). The post-Swamp->Bogwitch-transition case was not exercised; B140 should not be closed for the click symptom until that is re-run after B137 (shared Session binding) lands, reading stderr for `Character menu action diagnostic` / `Allocation baseline requires the current same character/Session`.
3. Integrated coverage is Knight level 2, one passive and one active skill. Max-rank / other classes rely on the unchanged admission probe plus the unit test (button and label hidden when admission false); code inspection confirms the label cannot appear unless the button is visible.
4. Pre-existing and unrelated: blank Current/Next level text for rank-0 Battle Hardened and locked skills (B020).

## Required changes

None. Safe to integrate the patch as-is (files: `generic_skills/runtime_skills_menu_v1.cpp`, `runtime_skills_menu_v1_tests.cpp`, `run_runtime_tests.ps1`).
