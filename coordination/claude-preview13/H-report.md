# Preview 13 Group H report: PC HUD keys, Faery key 4 path, character page navigation

Bugs: B002 (PC circle/key order, visual + cast proof), B024 (Faery key 4 path and visuals), B009 (page navigation).
Status: implementer report. Not closed. One source fix is applied in `main.cpp` but I have NOT built it into an EXE (root builds). One defect is open (key 5 potion after a skill cast, under B002/B024).

## Setup and provenance

- Candidate EXE copied to `.local-inputs/claude-preview13/h/pkg/` (copy of `.local-inputs/windows-source-clock-v19-preview-12-candidate`, `*.save` removed). The candidate folder was not modified.
- Candidate EXE SHA256 `1d43942ecd52df7b86bdf77ba2e5cb306afc76944813f38ca7ce9e6ccb252672` (15114752 bytes). The folder's `README.txt`, `manifest.json` and `package-receipt.json` still describe Preview 11 (`FD850478...`, 15079936 bytes), so they are stale for this EXE.
- Profiles (copies only, no user save touched): Rogue `h-rogue.save` (from `.local-inputs/v19-frontend-hotfix/profile-projection/profile-rogue.save`, Celest selected, slot 0); Mage `h-mage.save` (from `incoming-profile-mage.save`); Knight fresh (`swamp.args`).
- Scripted input uses the EXE's own options: `--skill-key-frame FRAME:KEY` (keys 1..5), `--profile-click-frame` (portrait), `--menu-release FRAME:X:Y` (authored 480x320 clicks), `--frames`, `--capture`.
- Helpers: `.local-inputs/claude-preview13/h/scripts/make_args.sh`, `ppm2png.py`, `live_keys.ps1` (attempted real Win32 keys; see Not verified).
- Frames: `.local-inputs/claude-preview13/h/frames/`. Logs: `.local-inputs/claude-preview13/h/pkg/*.log`.

## Evidence

### Keys 1/2/3 (B002): observed, consistent with [2,0,1]
- Knight fresh (`key-run-end.png`, frame 200): log `Source skill key=1 slot=2 ... no valid saved skill row`, `key=2 slot=0 skill=BashDown`, `key=3 slot=1 ... no valid saved skill row`. Screen: only the MIDDLE circle (labelled 2) carries the BashDown icon; left (1) and right (3) are empty. Labels 1/2/3 are centred under their circles.
- Rogue (`rogue-f125.png`, `rogue-f160.png`): key 2 casts JumpKick from slot 0 (MP 40.25 to 30.25) and the middle circle shows the JumpKick icon. Keys 1 and 3 reject empty slots 2 and 1.
- Mage (`mage-keys-160.png`): key 2 casts ColdRay from slot 0; the middle circle shows the ColdRay icon; keys 1 and 3 reject.
- Saved rows and slot numbers are unchanged by the key path.

### Key 4 (B024): cast path works; visible effect partial; Faery icon blank
- Rogue (Celest, slot 0): log `Source Faery key=4 frame=120 slot=0 sequence=348 ... MP=20.25`. MP 30.25 to 20.25 (10 MP). A re-press during the lifecycle is rejected (`Current source lifecycle action rejects a new skill cast`). Then `Celest OnPre completed; waiting for retained state7 do_spell Use`, and `Celest source state7 do_spell completed its ordered two-roll result loop` at frame 145. FX: `Source Celest FX bound ... particleInput=source-white`; `Source FX frame=120..122 packets=1`.
- Mage: same pattern, MP 78.5 to 68.5.
- Visible effect (direct observation): `rogue-f148.png` and `mage-keys-160.png` show a bright blue-white glow at the caster's head/hand during the cast. `rogue-f125.png` and `rogue-f138.png` show no sphere. The Android reference (`.local-inputs/v19-frontend-hotfix/profile-projection/reference-video-celest-skill.png`, a 640x360 frame, not PC) shows a translucent sphere around the hero plus a red ground ring. The PC run does not reproduce that sphere/ring. Whether the asset or the dispatch is missing is not established by this run; no cause is claimed.
- Faery HUD circle (key 4): EMPTY on Rogue and Mage (`rogue-f125.png`, `mage-keys-160.png`), although Celest is the saved current Faery and casts. The label reads `4 Faery`. This is a real defect (fixed in source, below).
- Knight fresh has no Faery state (`Source Faery key=4 ... slot=-1 ... no initialized source tables`). An empty key-4 circle is correct for that save.

### Key 5 (potion): OPEN defect
- Mage at frame 30 (vitals full): `Source potion key=5 ... result=3` (vitals_full). Correct gate.
- Rogue after the Celest cast (MP 20.25 of 40.25; HP full) and Mage after the Celest cast (MP 68.5): `Source potion key=5 frame=150 result=0 consumed=0 quantity=0->0 ... diagnostic=Source PropertyAdd could not synchronize live actor vital`. The HUD in the same frame shows `5 Potion: 5`. Nothing is consumed.
- Localisation: `features/inventory/runtime_session_potion_use_v1.cpp`, `synchronize_live_vital` (about lines 33-47). The health pass (36/38) succeeds with zero delta. The resource pass (41/43) fails, either at `dh2_property_add` or at the `state.resolved[current_id] != current_raw` check after it. `quantity=0->0` is the default receipt value, since the sync fails before `quantity_before` is set. It is NOT a zero count.
- `dh2_property_add` (`port/game-data/properties.cpp:34`) returns 1 only for an invalid property; otherwise it adds to `saved` (type bit 32) or `resolved` (type bit 8) and then resolves. The exact failing branch is NOT isolated.
- Hypothesis to test first (not verified): the Celest/UseMana debit writes `candidate.sheets.resolved[41]` and `actor->resource`, while the potion path reads a different `live` copy, so `resolved[41]` after add is not `current_raw`.

### Page navigation (B009): observed by scripted clicks
- `--profile-click-frame 30` opens the character menu on Stats (`nav-a-stats.png`).
- Clicks at authored (245,12) Equipment (`nav-tabs-50.png`: Equipment page, "Potions: 5"); (297,12) Skills (`nav-tabs-70.png`: Skills page, JumpKick in the middle Skill Mapping circle); (349,12) Faery (log `Character menu Faery page requires live content provider; current page retained`; the page stays on Skills); (193,12) Stats (`nav-tabs-110.png`: Stats page). Stats, Equipment and Skills navigate correctly and no stale page remains. The Faery tab is the documented stub.
- Hit geometry source: `port/windows-foundation/features/character_menu/original_art.cpp`, `original_menu_hit_zones` (tab zones at authored x 161-225, 213-277, 265-329, 317-381, y -13..38). Tab centres used: 193, 245, 297, 349 at y 12.
- Not verified: Escape (pause) and the C key by real input (see Not verified).

## Changes

1. `port/windows-foundation/main.cpp`: ONE hunk. Anchor: the line `layout.faery.key_label_bounds={267,313,295,311};layout.potion.key_label_bounds={319,381,295,311};` (about line 2058, PC HUD layout block). Added after it:
   ```
   // B002/B024: exact NativeHUDGetActiveFaery result = Character::SG_GetCurrentFaerieId(-1), the saved current_faery of difficulty 0 (difficulty used by this build's Faery cast arm).
   if(state.source_faery_state_known)layout.active_faery_id=state.faery_by_difficulty[0].current_faery;
   ```
   Why: `compose_pc_gameplay_hud_v1` already supports `layout.active_faery_id` (`pc_gameplay_hud_v1.cpp` lines 232-245 and 280-288), but the caller never set it, so the Faery circle was always empty.
   Source: `port/engine-ui/authored_gameplay_hud_v1_handoff.md` (`NativeHUDGetActiveFaery(0)` returns the active ID; `onPush` selects `btimg` frameID+1). IDA: `NativeHUDGetActiveFaery` at `0x44a820` (`pseudocode-all.c:234786`) calls `Character::SG_GetCurrentFaerieId(v6,-1)` (`0x3bb98c`, `pseudocode-all.c:136570`), which returns the save's `current_faery` for the current difficulty. The build's Faery cast arm uses `difficulty=0`. The value 0..4 maps to authored frames 0..4 (the packet accepts -1..12). The icon is absent when `source_faery_state_known` is false (fresh Knight), so no icon is invented.
   Syntax: `clang++ -fsyntax-only` with the exact `main.cpp` compile line from `ninja -t commands` (`.local-inputs/claude-preview13/h/main-compile-line.txt`) returned exit 0 with no diagnostics.
   Visual effect NOT yet seen. Root must build the integrated EXE to confirm an icon in the key-4 circle.
2. No other files changed. No saves, no candidate folder, no tracker, no commits.

## Tests

Runners (all PASS, run from `port/windows-foundation/features/generic_skills`):
- `run_pc_gameplay_hud_v1_tests.ps1`: `pc_gameplay_hud_v1_tests PASS: original SkillIcon UV/pixels, 48-segment PC circle geometry, centered labels across 800/1201/1920 viewport widths, non-overlap rejection, physical hit order1/2/3, Faery4, same-state potion count, strict identity and deduplicated edges`.
- `run_pc_skill_hud_projection_v1_tests.ps1`: `pc_skill_hud_projection_v1_tests PASS: actual Rogue SkillTables row/icon; left/middle/right labels1/2/3 -> source slots2/0/1; middle key2 JumpKick; shape hits emit keys once; saved rows unchanged`.
- `run_pc_skill_input_binding_v1_tests.ps1`: `pc_skill_input_binding_v1_tests PASS: physical keys1/2/3 -> source slots2/0/1 once; saved rows and native slots unchanged`.
- `main.cpp` `-fsyntax-only` with the build's compile line: exit 0.
Not run: `features/faery_menu/run_hotty_*` and `features/inventory/run_runtime_session_potion_use_v1_tests.ps1` (no changes in those files; the potion defect needs a focused test first).

## Package files required

Checked in `.local-inputs/claude-preview13/h/pkg` (copy of the Preview 12 candidate):
- `assets/original-cache/data/menus/dqhud_droid.swf`, SHA256 `a4ffacd1abdf7c9b2ba19c46ebb81c60c100458731a4cdba5880391b9c11b238` (present; matches the B002 handoff value).
- `assets/original-cache/data/menus/dqcharmenu_droid.swf`, SHA256 `43227075f407626b52eca4c345ac6f568023fcb39c269588201026c0fc6760e0` (present; matches the B009 handoff value).
- `assets/original-cache/data/3d/textures/MenusGraphics_droid.tga`, SHA256 `c75e8f4df8a9b28fcca7aa819a05cd69e6206d27a391b4364fd67790aeea86c7` (present; HUD atlas, bitmap1).
- `assets/original-cache/data/3d/characters/faeries/animations/faeries_celeste_spell_cast.bdae` (present; Celest cast animation).
- Celest sphere/ring effect asset: not identified in this pass; neither claimed present nor missing.

## Verifier script

Faery icon (after root builds the integrated EXE), run from a copy of the package with a fresh copy of the Rogue profile:
```
dh-foundation.exe --startup-config <args>
```
where `<args>` = `startup.args` plus `--save <rogue-copy>.save --menu-actions menu_MainMenu.btn_MENU_SINGLE_PLAYER|menu_StartGame.StartMenuButtons.btn_MENU_SINGLE_PLAYER --fixed-step .016 --skill-key-frame 30:2 --skill-key-frame 60:4 --frames 90 --capture faery.ppm`.
Expected: log `Source skill key=2 slot=0 ... JumpKick`, `Source Faery key=4 frame=60 slot=0 ...` with MP debit 10. The capture shows the middle circle with the JumpKick icon and the key-4 circle with an original Faery icon (not empty), labelled `4 Faery`. A fresh Knight profile keeps the key-4 circle empty.

Potion (open defect): `--skill-key-frame 30:4 --skill-key-frame 60:5`. Expected after a fix: `Source potion key=5 ... result=1 consumed=1 quantity=5->4`. Current candidate: `result=0 ... PropertyAdd could not synchronize`.

Navigation: `--profile-click-frame 30 --menu-release 40:245:12 --menu-release 60:297:12 --menu-release 80:349:12 --menu-release 100:193:12 --frames 110 --capture nav.ppm`. Expected logs: `Character menu opened frame=30 via profile input`; `Character menu Faery page requires live content provider; current page retained` at frame 80. Visual checks: Equipment at frame 50, Skills at 70, Skills still shown at 90 (Faery click retained), Stats at 110.

## Not verified / open

1. The `main.cpp` Faery icon hunk is not built or seen in an EXE. Root must build and capture (see Verifier script).
2. Key 5 potion fails after any MP-spending skill (reproduced on Rogue and Mage). Root cause not isolated. It needs a focused probe of `synchronize_live_vital` on property 41 after a UseMana debit, then a test. Not fixed here; it lives in `features/inventory`, outside my assigned files, and is flagged to its owner.
3. Celest visual is partial: a glow appears; the reference sphere and ground ring do not. Not established whether the asset or the dispatch is missing. No fix claimed.
4. Cooldown not measured in these runs.
5. Escape and the C key by real Win32 input were NOT verified. My attempts (`scripts/live_keys.ps1`) did not focus the game reliably: first runs sent keys to another window, and a redirected-stdout run hung as Not Responding. The portrait path is verified through `--profile-click-frame`, which triggers the same `profile` control as the C key (`semantic_input.cpp` lines 61 and 91; default binding `profile{'C'}`). Escape maps to `back{0x1b}` (pause) and was not exercised.
6. The Faery tab click is the documented stub (`Character menu Faery page requires live content provider`). Per the brief this is implementation, not a bug; the Faery page was not built.
7. The HUD Faery icon is composed once (`pcHudReady`); it may not refresh after an in-session Faery change. Not checked.
8. Group C (Skills content) and Group B (Equipment art) pages were only observed, not changed.
