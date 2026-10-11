# B111: Statistics sub-pages (Defense / Offense / Magic / Recovery) and icon navigation

Status: Implemented and verified in the normal private production executable at two aspects; pending fresh independent review and root integration. Prior handoff is retained below as historical context. Branch fix/b111 (based on fix/b103, reviewed integration 389c8806 merged).

## HANDOFF

### Evidence (observed / from IDA-SWF)
- SWF dqcharmenu_droid.swf: the five icons are sprite 278 clips btnStatisticsTab/Defence/Offense/Magic/Recovery inside sprite 279 (`buttons`, placed in menu_CharacterSheetNew 328). Their on_clicked handlers (authored-actions.txt 0xf87d-0xf9a8): NativePopMenu, NativePushMenu(menu_CharacterSheetStats|Defense|Offense|Magic|Recovery), NativePlaySoundFX("MenuSelect"). Sub-page sprites: Stats 386, Offense 375, Defense 359, Magic 352, Recovery 339 (root-menu-catalog.json).
- Correction from resume inspection: sprite 278 alone has one frame, but that does not establish absence of selected art. Each authored page sheet includes its selected orange icon artwork; exporting the full sheet preserves it. Normal runtime captures show Defense/Offense/Recovery/Magic/Statistics selected art. No new highlight was invented.
- Video P2 (part2_cIAW38IfLxY.mp4) t=386-398 s and 752-764 s show only the default Stats page (5 icon row top right, Stats icon orange). The four sub-pages are NOT visible in the P2 video (P1 not scanned). Not verified against video.
- Page contents (UpdateCharInfos, authored-actions.txt 0x13caa-0x1490f; values from NativeGetPlayerStats, property indices in engine-ui/character_menu_queries_owner_v1.cpp tail table):
  - Defense: Rating_Defense(59), Physical_Armor(71), Rating_Block(61)+"%", Rating_Dodge(60)+"%", Menu_Melee_Damage_Reduction(209), Resistances Fire/Water/Lightning/Earth/Air (74..78).
  - Offense: Rating_Attack(attack formula), Rating_Critical + "%", Menu_Average_Melee_To_Hit(207)+"%", RH/LH/TWOH weapon damage blocks (same visibility rules as Stats).
  - Magic: Spell_Rating_Critical(165)+"%", Menu_Average_Spell_To_Hit(208)+"%", Spell_Damage_Bonus Fire/Water/Lightning/Earth/Air(166,167,168,169,170).
  - Recovery: Regen_HP(39), Regen_MP(44), Leech_HP(132)+"%", Leech_MP(133)+"%".
  - Static captions: labels added in menu_text.cpp original_menu_label_symbol (GAMEPLAYMENUS_DEFENCE_TAB, OFFENSE_TAB, MAGIC_TAB, RECOVERY_TAB, etc.).

### Done (commit 881bdd1c plus the final fix commit)
- features/character_menu/export_stat_pages.py (new): reuses export_art.py walk; `--check` proves it reproduces the committed art0 / art0_no_points byte for byte (verified: True/True). Generates stat_pages_art.cpp (8 art blocks: 4 pages x with/without stat points, + 5 icon hit rectangles + original_stat_page_art()).
- character_menu.hpp/.cpp: StatPage enum, Action::stat_page_* (appended), Presenter::stat_page()/select_stat_page(), hit test of icon rectangles on the stats tab, frame() uses the sub-page art; re-selecting the Stats tab resets to the Stats page.
- menu_stats.hpp/.cpp: NativeMenuStats extended with dodge/block/melee_hit/melee_reduction/spell_crit/spell_hit/spell_bonus/regen/leech; original_stats_field handles all five sheets with % suffixes; hand-visibility for Offense.
- source_composition.hpp release(): stat_page_* actions call select_stat_page.
- audio: RuntimeSessionAudioV1::play_named_plain(name) (MenuSelect); main.cpp click loop plays it and logs "Character menu Statistics sub-page=N".
- CMakeLists.txt: stat_pages_art.cpp added to foundation_data.

### NOT verified at the historical cutoff
- NOTHING was compiled after the last edits (the first build hit a main.cpp string-literal error, now fixed with the Edit tool: both `'\n'` literals in the B111 block of main.cpp were rewritten as "\n"; re-check that block, ~line 5604, and that the file still has consistent line endings).
- No EXE run, no injected clicks, no screenshots, no video comparison, no ctest.

### Remaining steps
1. `powershell -NoProfile -File C:/Users/adamc/Desktop/workspace/DH_wt/p14_build.ps1 -Name fix111 -Jobs 2` (build dir DH_wt/build-fix111 already configured); fix compile errors (possible: <vector>/<array> includes in character_menu.hpp, Presenter::hit_test is noexcept).
2. Run with tools/quiet_click.ps1 like fix103/run1.ps1 (args file .local-inputs/claude-v20.1/fix103/a.args with --test-stat-points 5; keys: "5:-67:0:80" = C opens the menu). Icon centres in 480x320 px: Defence 241,127; Offense 293,127; Recovery 345,127; Magic 398,127; Stats 450,127 (scale by window 960x640 = x2). Capture each page; check labels resolve (a missing symbol in MenuLocalization::label would error) and the Recovery/Magic values.
3. Check the log line "Character menu Statistics sub-page=" and the MenuSelect sound line.
4. Add unit tests (character_menu_tests.cpp: hit_test on icons, select_stat_page only on stats tab, stats field strings).
5. Gaps: TWOH staff/bow caption variants (HasStaff/HasBow) and ElementDamageType frame selection are not implemented (also missing on the Stats page before); compare pages with the video if one shows them.

### Traps
- original_art.cpp must not be regenerated by export_art.py (disabled; solids 311/315 spliced by hand); export_stat_pages.py handles that.
- main.cpp is CRLF; avoid Python text-mode rewrites. Build dirs are ~440 MB each.

## Resume investigation / test plan (2026-10-11)
- Preserved committed prior work c267a04c; merged reviewed integration 389c8806 as d6d22074, retaining B102 short-click latch and B096 skill-point projection. No package or tracker writes.
- Directly inspected P2 386..397 at 1 fps (resume/ref-p2-386-397.png): default Statistics appears at 391..393, points 2 then 0, orange Statistics icon; transition to Skills then confirmation. Four subpages absent in this sequence. Directly viewed supplied shot-21: default Statistics, level 2, points 0, circled five icons. It proves appearance, not navigation. Existing SWF export is authored visual evidence for other sheets, not video comparison.
- Re-read recovered NativeGetPlayerStats consumer CharacterMenuQueriesOwnerV1::stats (port/engine-ui/character_menu_queries_owner_v1.cpp:58..91): player/save identity and genuine text/combat/inventory gates; class label source, fixed-point ASR8 fields, equipment HasStaff/HasBow queries, elemental 95..100 and tail properties. Authored on_clicked handlers 0xf87d..0xf9a8 perform Pop/Push/MenuSelect; page calls remain gated by open Stats tab. Existing actions 0x13caa..0x1490f describe exact subpage fields. Two-hand caption/element icon cases need completion against these actions before claiming fidelity.
- Expected: five icons navigate immediately to authored sheets, keep shared character/points, project live combat values and original localized labels, preserve hand visibility and source weapon categories; one source MenuSelect submission per accepted release. No invented active highlight.
- Focused plan: first compile source (currently pointer-return compile error). Then hidden normal EXE with fresh isolated fixture/save: C opens, 10 ms icon clicks visit all five pages, duplicate click stays consistent, wrong tab rejects, no-point plus refuses. Capture labels/values at 960x640 and 1200x640; source unit tests exercise page admission, navigation, percent/range fields, category/element visibility. Synthetic level-2 fixture may prove UI only, not natural level-up. Full ctest only known session_skill_binding exemption. Freeze/hash tested EXE and record evidence.

## Completed resume implementation
- Generated MenuArt pointer-return compile error fixed in exporter/art (3d0ea1ca).
- Offense/Stats elemental min-max fields project properties 95/96 and 98/99. ElementDamageType calls gotoAndStop(type+1), authored actions 0x141be..0x141ed and equivalent LH/RH cases; exporter emits exact original sprite347 frames 0..4 at each sheet/hand transform. Negative type hides caption/range/icon. Whole static Stats art remains byte-identical (`--check` True/True), including manually preserved deactivated solids.
- Two-hand captions use original category word37: HasStaff=5, HasBow=4 (player_equipment_queries_v1.cpp:13, same fields as original_combat_equipment_facts). Authored Offense 0x141ef..0x14310 chooses STAFF / BOW / 2_HAND and elemental variants; localization borrows the same live properties. RH/LH elemental captions are restored as well. No item-name/class-name heuristics.
- Top Stats release resets its default sheet, consistent with ChangeToStats. Added unit coverage for five icons/two aspects, duplicate release, closed/wrong-tab/invalid-page rejection, no-point artwork, source field arithmetic/formats, staff/bow/ordinary two-hand captions, five element frames, negative sentinel, dual offhand range, and unarmed hand suppression.
- Named plain sound method records successful host submission of actual source MenuSelect ordinal143; missing authentic asset retains explicit silent diagnostic. Silence mode affects output only, preserving mixer submission proof.

## Production verification (frozen executable)
- EXE: C:/Users/adamc/Desktop/workspace/DH_wt/build-fix111/dh-foundation.exe. SHA256 E1A205D0CF53F2798E4359950E6C7B0B4FC051A646DB118804270920EFC7BB97; final sound implementation included. Build Jobs2 exit0. No accepted-package files were written.
- Evidence directory: .local-inputs/claude-v20.1/fix111/resume. quiet_click with DH_AUDIO_SILENT=1 owns each private desktop; real WM_MOUSEMOVE/DOWN/UP, 10ms clicks; isolated saves. nav.args/nav.log at960x640; wide.args/wide.log at1200x640. Both exit0. nav sequence accepted page1@452,2@571,4@689,3@807,0@927, duplicate0@987, later1@1395; exactly seven corresponding `Plain sound submitted name=MenuSelect ordinal=143`, audio diagnostics0, no underruns. Wrong-tab icon at20s emits no page action, top Stats at22s returns default, no-point plus at26s logs source refusal; no point mutation.
- Captures nav-480/600/720/840/960.ppm and wide-480/600/720/840/960.ppm inspected; montages nav-pages.png/wide-pages.png retained. All five labels readable and live values populated: initial Defense36/Armor4/Block1%/Dodge5%, Offense attack69/critical5%/hit84%/R-Dam13-17, Magic critical5%/hit74%/bonuses3, Recovery HP6/MP2/leech0%. Legitimate zero values remain visible; no blank value captions or placeholder strings. At both aspects original selected icons are visible. Wide fixture has2 points and source attribute12; direct startup forcibly resets its requested level2 to1, so that run cannot prove level2 (reported honestly).
- Full ctest167/168 pass; only allowed worktree session_skill_binding fails (ctest.log, 11.09s). Expanded final dual/unarmed focused character_menu_tests PASS after that suite. Source/export checks pass, git diff --check clean.

## Fidelity and remaining limits
- Matches directly observed default Statistics footprint/row/zero-point appearance from P2 386..397 and shot21, allowing different real character values. Four subpage layouts and navigation/captions are established by authentic SWF authored art/actions and recovered/native logic; no four-subpage video sequence has been found/claimed. P1 s001 viewed only, not a full P1 audit. Original v1.0.3 footage versus supplied Android SWF asset version difference remains possible.
- Element/category outcome branches are covered in genuine per-frame projection tests; real runtime gear-switching across bow/staff/dual/elemental variants has not been shown in this B111 scope. Physical mouse, audible listening (silent policy), natural level-up and B103 stat-plus allocation/confirmation are not claimed. Source sound submission is proven, audible quality is not.
- B103 remains a separate owner/review request; B111 does not resolve all stat-plus reports. B110 shared multiline layout is untouched.
- No extra package assets required; the subpage/element geometry is compiled and existing original UI/font/localization/audio assets suffice. Root owns fresh review, integration/reproduction, tracker and release gates.

## Additional source-level-2 check
- A source-format private save was produced by normal frame120 snapshot, then a scratch save_store-based helper changed only the private fixture name/level/XP/points/Strength. The direct launcher resets level1 (as already recorded); normal frontend load via real Navigator/creation services preserves level2.
- level2-final.log exit0 with the same frozen EXE: menu frontend script selected occupied slot0 and invoked real Single Player/Start source services; --skip-boot bypassed only intro/logo/title for the test. --profile-click-frame120 delivered source portrait pointer down/up through SemanticInput, opening the real Statistics menu. Visually inspected level2-source-180.png: Warrior Lvl2, HP165/169, MP27/28, XP41/200, points2, Strength12, R-Dam14-18, attack59/defense25. Natural gameplay level-up remains unproven. Fixture Dexterity0 comes from the copied direct bootstrap snapshot and is not a claim about original level2 balance.
- Earlier exploratory frontend runs did not open the menu because click times were in boot/before gameplay or after the fast unfocused gameplay pass; one private boot run timed out and its own PID was terminated by the harness. Those are failed harness attempts, not production B111 passes. No user's window/process/save was controlled. Scratch refinding injector was not committed; final source-portrait run uses existing production SemanticInput path.
- Recovered IDA native entry explicitly identified as NativeGetPlayerStats 0x00446afc; registration/caller is in pseudocode-all.c at line217649, native function at233117. Authored sheet consumer/gates and original equipment property word37 were checked as described above.

- Additional source-level2 icon clicks: level2-nav.log exit0; page2@162,4@283,3@402,0@521, each one real10ms release and one MenuSelect ordinal143 successful submission. First Defense click at4s preceded profile opening and is not counted as a success; level2-defense.log separately proves Defense1@207 with real10ms click at6s and one MenuSelect submission, exit0. Inspected level2nav-210/330/450/570 montage level2-four-pages.png and level2-defense-540.png: all five source-level2 page titles/values/selected icons. Recovery7/3, Offense attack59/hit79%/damage14-18, Defense25/Armor4. Natural level-up is still not claimed. Original quiet_click used for these final passes; source portrait admission hook120 opens through normal SemanticInput. No code changes were needed for the frontend proof.
