# Registered PyData table payloads

## Scope and current result

`tools/atlas/table_payloads.py` enriches existing stable-ID rows in
`coordination/atlas/data/tables.json`; it does not write generated data or retain
serialized binary blocks. Against the read-only `.local-inputs` corpus, the
helper returns **1,316 payload updates** across 20 registered families and 367
source-backed reference edges. Of these, **1,313** were previously `UNRESOLVED`;
three `camera_animation_sets` aliases were already resolved and receive validated
payload enrichment. Applying the updates to the 8,863-row input inventory leaves
no rows marked `UNRESOLVED`. This is inventory coverage, not a claim that every
engine table has been behaviorally ported. Updates keep stable IDs and merge
without overwriting pre-existing fields. Each accepted stream is count/name
checked and consumed to its exact boundary.

| Family | Rows updated | Payload / reader evidence |
| --- | ---: | --- |
| CharacterClasses | 260 | `ClassTable [iiiii]`, `0x4b4484` / names `0x4a99b8`; `port/game-data/class_tables.cpp:39-103`; 13 schema sections; formula group links follow `:29-39,54-67`. |
| CharacterTemplates | 121 | `Charater_Templates [i]`, `0x4b3f5c` / `0x4a93b8`; `port/game-data/character_templates_v78.cpp:19-34,42-47`; ordered member IDs. |
| DialogActors / Dialogs | 55 | `iSi`, `0x4bc798` / `0x4a8c44`, and `[iiii]`, `0x4bc64c` / `0x4a8ac4`; `port/windows-foundation/features/cinematic_runner/dialog_tables.cpp:36-49`; raw caption/style IDs stay raw. |
| GameObjects | 156 | 11 exact registered groups and shapes; `port/level-loader/game_object_arrays_owner_v81.cpp:22-25,39-60`; field values remain positional unless schemas align. |
| v2Conditions | 118 | `[iii]i`, add `0x4b8dec` / names `0x4a4924`; `port/level-world/native_conditions_v69.hpp:25-26` and `native_conditions_table_v69.cpp:34-43`; count, per-row stub count, three signed words per stub, and type word. The 2,796-byte stream is fully consumed. |
| v2Quests | 64 | Full format starts `iiii[iii][iiiSSiii][iii][iii][iii]...`, add `0x4b8acc` / names `0x4a4624`; `port/game-data/quest_persistence_v51.cpp:8-27,43-67`. Variable objective/reward groups and all 14 strings are retained. |
| WorldMap / WorldMapLockers | 16 | `ii[i]`, `0x4b8980` / `0x4a44a4`; `ii`, `0x4b883c` / `0x4a4324`; `port/game-data/world_map_profile_table_v59.cpp:17-30`. |
| ItemBonusAttrMonopoly | 43 | `[i]`, add `0x4baa30` / names `0x4b6c94`; single-integer list entries are retained without guessed meanings. |
| TrophyTable | 69 | `iiiiiii`, `0x4b8f28` / `0x4b1130`; exact fields `Desc, GLIndex, GLLive, Grade, Label, Name, Type`; `port/level-world/trophy_manager_owner_v1.cpp:12-18`. |
| FontPalette | 7 | `ii`, `0x4bbe9c` / `0x4b2ad0`; fields `glowcolor,textcolor`; `port/engine-ui/character_menu_font_palette_v1.cpp:5-11`; consumer uses `textcolor` for colored item names. |
| HelpPages / HintPages | 25 | Two `ii` groups: HelpPages `0x4baf58` / names `0x4b72fc`, HintPages `0x4bae14` / `0x4b7160`; `port/engine-ui/loading_menu_v1.cpp:18-23`; schemas `HelpStringCategory,HelpStringText` and `HintAvatarID,HintStringID`. |
| v2Events | 66 | `iS[iiiSSiii]`, `0x4b8c9c` / names `0x4b1460`; `port/level-loader/game_event_manager_v50.cpp:19-23,50-66`; source names, scalar, script, and exact objective records. Unnamed members stay positional. |
| SpawnGroups | 1 | `bi[iii]`, `0x4b9060` / names `0x4b12c8`; schemas align with choice `{Char,Prob,Qty}` and row `{ActiveSpotOnly,Delay,Spawns}`; `port/android-native/app/src/main/cpp/source_campaign_spawn_groups_v108.cpp:76-78`. |
| DesignSettings / GameDifficulties / GameOptionTable | 20 | Registration entries at `0x4b3cd0` / `0x4a90b8` (43 words), `0x4af5e8` / `0x4a8f38` (5 words), and `0x4bc8e0` / `0x4a8dc4` (7 words). `port/game-data/design_settings.cpp:23-25,49-57` and `port/engine-ui/game_option_table_v1.cpp:25-38` validate counts and repeated schemas. Values are retained as raw 32-bit word bits because some settings are floats and the decoder does not normalize them. |
| MerchantTable / NumProbArray | 51 | Loot groups 6-7: `ii[ii]`, `0x4b9d40` / `0x4b0460` (12 rows); `[hh]`, `0x4b9bf4` / `0x4b02c4` (39 rows). `tools/atlas/table_sites.py` and `port/game-data/loot_tables_v2.cpp:42-53` establish the preceding groups' byte boundary. Merchant pairs and probability halfword pairs remain ordinal. The complete 284,104-byte loot stream is consumed. |
| CameraAnimationSet / `camera_animation_sets` | 3 | `CamAnimSetTable [i]iiii`, `0x4b37a4` / names `0x4b4a58`; stable inventory alias supplied by `tools/atlas/references.py:57,145-153`. These rows are read after 785 animation-sequence rows and before 80 character-animation rows; the entire animation stream is consumed. |
| RectTable / `common#group0` | 2 | `iiii`, add `0x4b3e18` / names `0x4b796c`; exact four-member `Rect` declaration in `source_process_compiled_members_v121.hpp:20,238`. Retained as `Height,PosX,PosY,Width`; coordinates are not reinterpreted. |
| StrID_Languages / `common_text#group0` | 9 | `[SS]`, add `0x4b53f8` / names `0x4b346c`; nested `filename,name` and outer `list` schemas. Localization notes at `port/engine-ui/reference/localization/NOTES.md:17-23` identify nine ordered language packs with 37 sheets each (333 bindings). This is sheet metadata, not the string-body table. |
| CharacterProperties suffix groups | 7 | The exact `CharacterTable` prefix is 448 rows × 224 signed words plus its count (401,412 bytes), matching `port/android-native/app/src/main/cpp/source_process_array_registration_v101.hpp:21-22` and compiled member count 224 at `:234`. Its remaining **196 bytes** are fully consumed by `StatAutoAssignSchemeTable [iii]` (3 rows, names at `0x4b7e3c`, add `0x4b41f4`) and `StatListTable ii` (4 rows, names at `0x4b7ca0`, add `0x4b40a8`). Tuples/words are retained without assigning meanings or namespaces; compiler metadata names their outer members `StatAutoAssignTargetList`/`StatListList` (`:235-236`). |
| Legacy sound tables / `sounds#group1..4` | 223 | Reader consumes the already-covered `CharSoundsTable` prefix, then exact registered formats: `Listeners iiiifi`, `0x4b9578` / names `0x4b0c68` (5); `SoundBankPlayback ii`, `0x4b9434` / names `0x4b0ad0` (27); `SoundGroupsRouting Si`, `0x4b92f8` / names `0x4b0f98` (8); `Sounds_bak iiSiiiiii`, `0x4b91ac` / names `0x4b0e00` (183). Exact pystructname schemas are required. `port/engine-audio/reports/audio-v38-handoff.md` documents the listener consumer and field use. Other numeric/string fields remain source values with no inferred edges. `Sounds_bak` is deliberately kept separate from the live 638-row `sdd_dungeon_hunter_2_iphone` sound space. The 11,295-byte sound stream is fully consumed with the existing CharSounds section. |

## Evidence and interpretation limits

This is internal atlas infrastructure with no direct visual counterpart, so no
original gameplay frames were consulted. Investigation used the original
registered reader formats, their exact name/schema tables, and recovered source
consumers cited above. The invariant is preserving every row's registered
payload, names, order, and byte boundaries so later consumer-backed behavior
can use the data. This is source/data verification, not gameplay, UI, or runtime
verification.

For `v2Conditions`, the source is `NativeConditionTableV69::decode`, not the
older fixed-row `quest_conditions_v1` projection. Condition stubs are
`{Op,Argument,State}` in the native record. The recovered evaluator in
`port/windows-foundation/features/conditions/authored_conditions_v1.cpp:90-139`
uses op 0-2 for quest-state row arguments, op 3 for a current-level row, op 4-5
as literal true, and op 6 for a GameEvent lookup. The decoder emits quest and
level edges only for the former proven row-index spaces. Event arguments are
retained, but no edge is emitted because event identifiers are not proven to
equal v2Events row indices.

For legacy audio, `Listeners` is the only added table with a source-backed
consumer mapping: recovered `Level::UpdateListener3f0ae0` selects a row from the
level-owned listener index, and audio-v38 evidence identifies maximum distance,
reference distance, rolloff, and orientation vectors. The code stores the exact
registered members but emits no sound-table reference edges. `CharSoundsTable`
remains in the separate `mob_sounds` decoder; its 98-byte prefix is walked only
to reach `Listeners`. Neither it nor `Sounds_bak` is joined to the live sound
ledger by numeric value or name.

All readers bound counts and input spans. A truncated stream, wrong count,
unsupported schema, conflicting existing field, or trailing byte rolls back the
entire affected family. Original assets are opened through `common.context` as
read-only inputs; no generated table file is rewritten by this helper.

## Verification and remaining gaps

Focused checks ran against the actual `.local-inputs` corpus. The 20 decoder
families returned 1,316 unique stable IDs; the 1,313 input rows previously
marked unresolved were all represented, the three pre-resolved camera aliases
retained their original fields, and no field already present was overwritten.
Every decoder returned with an empty `gaps` list, every family required exact
record/name counts, and every record stream ended at its validated boundary.
The CP suffix ended exactly after the declared rows; common-text produced nine
ordered packs; sound output totalled 223 rows and ended at byte 11,295. Truncated
`i32` reads and over-limit counts were rejected. Applying these updates to the
8,863-row source inventory leaves zero `UNRESOLVED` rows.

This helper's tests do not perform a full atlas regeneration, visual check, or
real-game runtime test. `RESOLVED` means that the registered source payload was
recovered; it does not mean that the game behavior using every row is ported or
runtime-verified.
