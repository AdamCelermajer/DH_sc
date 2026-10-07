# Complete script resources and current-spell source connection

The native world caller now obtains authored scripts from the complete original cache inside the APK. `CharacterScriptAssetsV1` retains all 219 `.luac` files (900,493 bytes), the actual 16-row Faery tables and the caller's existing 127-row SkillTables authority. Monster sessions receive the full available include set, with their separately owned common/external files excluded. The owner survives cache/table facade destruction and remains pinned through world restoration.

The native ZIP filesystem folds ASCII filename case. The resource owner retains real table request spellings over those actual cached bytes, including capitalized DarkQueen skill names. Five authored spell rows genuinely name absent files; those stay source LoadFile misses. No class-suffix spell replacement is made. Missing common scripts, malformed Faery tables, unsafe/duplicate paths, excessive payloads and missing cache providers fail atomically. Reload is denied while live character borrows exist.

`Character::_GetCurrentSpellInfo` (`0x003b6e30`) is now reconstructed and centrally compiled. Its ignored Arguments, two fresh selected-faery reads, intervening GetCharFaery validation and final saved-level query are preserved. The native callback requires real same-owner services; the V3 gameplay owner is connecting those to the pinned Faery tables, live difficulty and saved faery rows. The query alone does not establish full player initialization.

## Verification

- Main native library: 140 centrally linked suites PASS; ASan/UBSan/LSan zero. Receipt: `port/level-world/reports/native-script-resources-spell-main-linked-host-audit-v1.json`.
- Full original script bytes: 219 independent canonical ZIP length/SHA matches, 129 available authored script bindings, five genuine absent spell rows, 19 failure/ownership guards, 721 checks. The current central suite uses the single scene-material SHA implementation.
- Current-spell original versus optimized ARM64: 512 cases and 2,048 ordered services with no mismatch. SG/table helpers are declared services in this differential. Its separate real-VM/sanitizer replay passes 12,847 checks, ten guards and two Lua calls.
- Actual ARM64 and x86_64 Android compilation: complete resource owner, current-spell query and native caller selected; defined symbols and ELF64/16 KiB LOAD alignment verified in `port/android-native/reports/native-script-resources-spell-library-build-v1.json`.
- Frozen current-spell handoff: `port/level-world/reference/character-current-spell-v1/freeze-manifest.json`, SHA256 `39842222b73e201b43bf630cc91eb0e24d0bc55a3e56db2f0be5ad36848328b6`.

## Full-cache startup limitation

The complete-cache player bootstrap was attempted against the frozen V2 owner. It fails honestly when a real faery update reaches the unsupported GetCurrentSpellInfo registration. The V2 skill-only audit supplied fewer files and never loaded these spell scripts. The failed complete-cache attempt is preserved as **INCOMPLETE**, not PASS, in `port/level-world/reports/character-player-full-cache-bootstrap-v1-required-failure.json`. The newly recovered callback still needs the V3 owner's genuine provider and registration composition before that attempt can succeed. Nonzero starting grants additionally require the V3 Buff/property lifecycle.

The main 140-suite success does not include a successful complete-cache player bootstrap. Its existing V2 three-class/19-skill counters remain scoped to that audit's supplied file snapshot. No full gameplay, automatic enemy AI, campaign, new menu, audio or physical-device acceptance is inferred.

No new APK checkpoint was produced for this preparation. The visible emulator remains on the verified `dh2-native-source-player-ui-136e924a.apk`, living player/full HP and mana, developer panel closed and enemy attacks held for inspection. New menu screens remain unconnected. The whole game reconstruction goal remains active.
