# B144 + B147 report (v21.5-ui)

Branch: `v21.5-ui` (worktree `C:\Users\adamc\Desktop\workspace\DH_sc\.claude\worktrees\v215-ui`), base `422d8f89`.
Commits: `746b9fe8` (B147), `90c305bb` (B144). Not pushed. Tracker not edited.
Build: llvm-mingw clang++ 23.1.3, CMake + Ninja Release, build dir `.local-inputs/v21.5-ui/build` (gitignored).

## Status summary

| Bug | Investigation | Implementation | Unit/isolated tests | Runtime (hidden window) | Gaps |
|---|---|---|---|---|---|
| B147 | done (recovered source + SWF strings + video reference from B140) | done, reviewed | PASS | PASS for rank-0 and locked, screenshots below | Charge/Paladin energy balance still blank; next-level text for unaudited scripts still blank |
| B144 | done (B138 evidence reused, re-checked against code) | done (partial work from an earlier interrupted session re-verified and finished) | PASS (world_drop_rules, status_message_v1) | NOT done | status queue has no 4-deep cap; no hidden-window chest/pickup capture |

## B144: invented item label, chest scatter, original status line

### Visual evidence (reused from `coordination/v21.4/B138-report.md`, re-checked)
- User screenshot (Bogwitch Cavern, our build): white "Imbued Belt" text above the open chest. Our invention (`main.cpp` label block).
- Original video P1 (v1.0.3, z_Zky7qQdYs), frames 209.4-214.0 s at 5 fps and 210.6-213.6 s at 10 fps: nothing is drawn over the open chest. The thrown item is a small glow leaving the chest. A status line ("Imbued Armor", green; "Useless Mace", white) appears at about 46% screen x and 59% screen y, fades in, holds about 0.4 s, fades out, and the next message starts after a gap. Direct observation of positions across scrolling frames: the text stays at the same screen pixels.
- Inference: the thrown item's exact model and landing point are not resolvable at 640x360.

### Logic evidence (IDA export, per B138 section 2)
- `ItemObject::GetInteractionType` 0x3ebeb4 returns -1, so `ShowTooltip` 0x3ebd5c is unreachable: no floating name exists in the original.
- `ItemObject::Interact` 0x3ed144 enqueues `MenuMessageManager<StatusMsg,4>` with the rarity-coloured item name on successful pickup, and `GAMEPLAYMENUS_INVENTORY_FULL` when the bag refuses.
- `ItemObject::DropAndAwardLoot` 0x3ec8a0 scatters every drop (chest and enemy alike) through `_GetRandomDropPos` 0x3ec668 toward the killer/opener. Chest `DoOpen` calls `DropLootTable(table, same object, opener)`.
- The status line is the authored `HUDelements.itemname_text` clip (sprite 166, 48 frames, 30 fps, scroll label at frame 8), in `dqhud_droid.swf`.

### Expected behaviour
1. No text over any ground item or chest.
2. Pickup or refusal produces one queued status line in item-quality colour at the authored stage position, with the authored fade timeline.
3. Chest and barrel drops are thrown from the object toward the opener and land 150-349 units away, using the same loot RNG draws as enemy drops.

### Cause in our code (before the fix)
- `main.cpp` drew `itemDisplayName` text at each targeted item (invented, `ShowTooltip` never reached).
- `container_loot_v1.cpp:116` and `runtime_world_item_adapter_v1.cpp:103,131` published chest drops at the chest origin with `destination = position`.
- Pickup status was a single slot drawn with `drawScreenLabel` at `window.height()*.25`, 90 frames, with no queue and no authored timeline.

### Changes
- `features/containers/container_loot_v1.{hpp,cpp}`: opener position and scatter wiring for chest drops (`publish_source_object_drop_scattered`).
- `features/loot/runtime_world_item_adapter_v1.{hpp,cpp}`: `publish_source_object_drop_scattered` (same `scatter_destination_v1` placement as enemy drops; keeps the item at the object when no RNG or opener is supplied).
- `features/hud_panels/status_message_v1.{hpp,cpp}` + `_tests.cpp`: pure status-line queue (`StatusMessageQueueV1`) playing the authored timeline.
- `features/hud_panels/status_message_timeline_v1.inc` + `export_status_message_v1.py`: generated from `port/android-native/app/src/main/assets/original-cache/data/menus/dqhud_droid.swf` (not hand-written; the exporter fails on unexpected SWF structure).
- `main.cpp`: removed the invented label block; pickup and inventory-full now enqueue into `StatusMessageQueueV1`; drawn via `drawScreenLabel` with alpha and Y scale; wired opener position into `ContainerLootInputsV1`.
- `world_drop_rules_v1_tests.cpp`: chest-drop cases (lands 150-349 units toward opener, uses exactly 2 RNG draws, no-opener keeps item at object).
- `CMakeLists.txt`: adds `status_message_v1` test (ctest 178 total).

### Tests
- Full ctest in worktree build: **178/178 pass** (includes `status_message_v1`, `world_drop_rules_v1`).
- Runtime: not run (see gaps).

### Remaining gaps (B144)
- No hidden-window capture of a chest opening, the thrown item, or the status line. Walking to a chest in the Act 1 start level was not scripted.
- The status queue has no 4-deep cap. The original is `MenuMessageManager<StatusMsg,4>`; its overflow behaviour was not verified, so none was invented.
- The status line's exact on-screen size and position come from the SWF timeline and were not checked against the 59% reference at runtime.
- Enemy-drop scatter already existed; the chest path is now consistent with it, but the thrown travel animation (80-unit stop) was not separately re-verified.

## B147: Skills Current/Next text for rank-0 and locked skills

### Visual evidence
- B140 reference (`reference-398.png`, Battle Hardened rank 0, 1 point): Current "Add points to use this skill", Next "+5 to Def. Rating / +1 resistances".
- Our build, hidden-window capture (Battle Hardened, rank 0): Current "Add points to use this skill", Next "+ 5 to Def. Rating / + 1 to resistances", bottom button "Learn Skill" (B140 user-requested adaptation). Screenshot: `.local-inputs/v21.5-ui/runs/b147-r1/f600.png`.
- Locked skill (Earthstrike, required level 3, character level 2): Current "Unlocked at level 3", Next empty. Screenshot: `.local-inputs/v21.5-ui/runs/b147-r1/f1000.png`.

### Logic evidence (`port/engine-ui/hud_initialization_v1.cpp`, recovered NativeGetSkillDetails)
1. Level < `RequiredLevel`: Current = `GAMEPLAYMENUS_SKILL_UNLOCK_AT_LEVEL` with the required level; Next empty. No script is consulted.
2. Otherwise rank 0: Current = `GAMEPLAYMENUS_NEEDS_SKILL_POINTS`. Rank > 0: Current = the row's current-level OID parsed with `AI_SkillInfo(rank)` display properties.
3. Next: `GAMEPLAYMENUS_MAX_SKILL_LEVEL` at the cap; `GAMEPLAYMENUS_SKILL_MAXIMUM_LEVEL_TRAINING` when `CanIncrementSkill` is false; otherwise the next-level OID parsed with `AI_SkillInfo(rank+1)`.
- The heading keys exist in the shipped original symbol table (`gameplaymenus.symbols` / common text).

### Cause in our code (before the fix)
- `runtime_skills_text_v1.cpp` returned unavailable for every SkillTable script not in a two-entry audit list (`prince_rogue_jump_kick`, `prince_warrior_bashdown`), before the rank-0 and locked branches. Those branches need no script for their current text, so Battle Hardened, Earthstrike and every other unaudited skill showed blank Current/Next.

### Changes
- Audited SetTempProps OnSkillInfo bodies for the prince_* scripts (generated table; the test verifies each entry against the original `.luac` text in `.local-inputs/hud-formatting-v1/scripts`). Unverifiable ones stay unavailable: `prince_warrior_charge` (HasShield branch) and `prince_paladin_energy_balance` (live faery buff). Magi's Shielding is audited only for rank <= 1000 (its script substitutes a cached level above that).
- Static guard branches are evaluated first; script-dependent display arguments are loaded lazily and only when a string consumes them.
- Parameter-free `GAMEPLAYMENUS_*` symbols resolved through `StrID` constants (matches the source `named("StrID", key)` path).
- `run_runtime_skills_text_v1_tests.ps1` takes `-InputsRoot`, `-Libs`, and passes the scripts directory to the test.
- No change to admission, rank/points mutation, hit regions, saves or sounds.

### Tests
- `run_runtime_skills_text_v1_tests.ps1` (InputsRoot = primary `.local-inputs`, Libs = worktree build): **PASS**. 120 player SkillList rows: 69 simple + 1 guarded scripts audited, 2 unaudited; locked=102, rank0=120, ranked=232, capped=120, blocked=120; 73 localized names, 42 source SkillInfo current/next rows, 4 exact SetTempProps rows, 48 Rogue specialization states.
- `run_runtime_tests.ps1` (Skills menu, B140): **PASS**.
- These two scripts are not registered in CTest (pre-existing); ctest 178/178 still passes.

### Remaining gaps (B147)
- Next-level text for unaudited scripts stays blank (honest gap). Charge rank 0 shows "Add points..." but no next text.
- Faery-dependent `FairieDependantText` rows: not exercised in the runtime capture; the unit matrix reports 0 such rows.
- Not checked against a shipped build with the packaged saves; only the isolated seed (`L2p1`: Knight level 2, 1 point) was used.

## Environment notes (for reproduction)
- The worktree has no `.local-inputs`. I created `.local-inputs/` junctions (gitignored) for the top-level inputs the tests reference, pointing to the primary checkout's `.local-inputs` (read-only use; tests may write scratch into linked dirs).
- Some test binaries import `libc++.dll` and `libunwind.dll` dynamically. They need `windows-toolchain/llvm-mingw-20261006-ucrt-x86_64/bin` on PATH in Git Bash form (`/c/...`); with `C:/` entries they fail with exit 0xc0000135.
- Hidden-window runs used `quiet_click_safe.ps1` (copied from the old B147 scratch) because `|` in `--menu-actions` is interpreted by `cmd` in `quiet_click.ps1`.

## Runtime artefacts
- `.local-inputs/v21.5-ui/runs/b147-r1/` (isolated copy of the Knight level-2 seed; run args; logs; `f400..f1400.png`, `final.png`).
