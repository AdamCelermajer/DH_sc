# Post-pause source-change impact sweep

Cutoff: **2026-10-08 17:32:11 Asia/Jerusalem**. Read-only sweep snapshot ended at approximately **18:11 local / 15:11 UTC**. This report is the only file written for this sweep. No lane report, game source, build, test, emulator, or external service was modified or run.

**Initial snapshot obligations (superseded by the final reconciliation below): recheck lanes 02, 03, 04, 05, 06, 08, 09, 12, 13, 19, 22 and 23.** Lane 21 has refreshed callback hashes but still needs the new process-design and movie/reset edges reviewed. Lane 14 refreshed the three font implementation files; its new application reset edges and changed header contracts remain separate. Lanes 01 and 11 already captured their affected current source. For lanes 07, 10, 15, 16, 17, 18 and 20, no concrete impact from the listed edits was established in this scoped sweep.

## What was measured

I inspected all **23 existing CSVs (38,415 rows including cross-cutting edges/findings)** and **22 available Markdown reports**. `lane-21.md` was absent at both initial and final checks; its 575-row CSV was inspected. All report file hashes were unchanged during this sweep. This row count is a report scan, not semantic coverage or a new function-completeness claim.

Exact changed-source path matches appear in **477 unique CSV rows**, across 11 lanes. Markdown adds five lanes with fingerprint/caller-only citations, giving **16 lanes with direct changed-file references**. Per-file counts overlap; for example, lane 14's 53 GFNT, 55 platform and 3 render-owner references refer to 55 unique rows.

The production `port/level-world/character_script_session.cpp` and fixture `port/level-world/tests/character_script_session.cpp` were treated as separate paths. **No existing lane file cited the exact changed fixture path.** Bare basename mentions in production-source paragraphs were not counted as fixture citations.

Current hashes were present in the 228 affected lane 21 rows and 55 lane 14 rows, and in the relevant Markdown fingerprints of lanes 01 and 11. A current hash authenticates the compared bytes; it does not demonstrate every edge or argument branch was semantically reviewed. All existing runtime acceptance remains outside this source-impact sweep.

## Initial all-lane impact matrix

“Recheck” means the cited source version or a concrete dependency changed. It is not an automatic defect/status reclassification. “No identified impact” is bounded to these files, report citations and the dependency edges inspected here.

| Lane | Impact disposition | Unique exact CSV rows | Changed citations | Required scope |
|---|---|---:|---|---|
| 01 | Refreshed | 0 | native_character_menu_v4.inc | Current d6d00fb8 fingerprint and post-pause F3 queue search recorded. No additional drift found; retain F3 partial. The font-reset hook in the same class is a separate edge. |
| 02 | Recheck | 21 | character_script_session.cpp; original_ui_session.cpp; model_renderer.cpp | 20 callback rows plus #1960 filename caller cite old hashes. Recheck required-failure propagation and current line anchors; also review pending #2418 AnimDict and #2996 GetPyOID on the new process delegate. |
| 03 | Recheck | 0 | character_script_session.cpp (Markdown hash only) | Old f1604fe5 fingerprint. #4329 RegisterAnim and #4764 ANIM_AddAnimDictToSet were already unclear/unfinished; update source snapshot and review their new lookup/failure neighbors without promoting prior coverage. |
| 04 | Recheck | 6 | character_game_design.cpp; original_ui_session.cpp/.hpp; native_menu_process_bootstrap_v104.inc; model_renderer.cpp | IDs 5720,6151,6655,6673,6910,7455. Five are candidate/clone/unclear; #7455 matched caller still has correct low-byte write at current renderer:3127-3129. Refresh provenance and menu/setup/teardown edges. |
| 05 | Recheck | 0 | original_ui_session.cpp (Markdown) | PyDataArrays.Load missing-file finding and process startup connection use old caller hash. Current array failure forwarding remains at :680-693, but process owner is now shared and loaned to design consumers. Keep local branch finding bounded. |
| 06 | Recheck | 8 | original_ui_session.cpp | IDs 9517-9520,9523-9525,9539 cite old owner/hash. Constants sticky failure remains at current :660-678; array GetOID closure now gains a character-process consumer. Revisit #9350/#9444 AnimDict and #9508 GetOID dependency closure. |
| 07 | No identified impact | 0 | none | No exact changed-file citation or concrete changed dependency identified in the existing report. This is not a guarantee about all transitive consumers. |
| 08 | Recheck anchors | 5 | model_renderer.cpp | IDs 14779,14780,14784-14786 are unclear source candidates. Preserve unclear; refresh shifted renderer anchors/hash before using candidates as evidence. |
| 09 | Recheck anchors/closure | 115 | model_renderer.cpp | All 115 affected CSV rows are unclear/clone, not established matches. Markdown cleanup/material includes now occur at renderer:1282/:2312; old bb979002 snapshot is superseded. Recheck wrapper selection and ownership only where claims use it. |
| 10 | No identified impact | 0 | none | Changed files do not directly occur in this report; no concrete changed animation/scene leaf was identified from the listed edits. |
| 11 | Refreshed | 0 | model_renderer.cpp (Markdown) | Current de089594 hash recorded; post-pause factory/billboard selection was explicitly reread. No further renderer drift occurred in this sweep. This does not complete unreviewed particle/scene bodies. |
| 12 | Recheck anchors | 1 | model_renderer.cpp | #21438 is matched for the standalone projection kernel but its caller hash is old. Current dh2_camera_gpu_projection_v12 call is renderer:3080; related includes are :2312/:2313. Recheck caller provenance; hash drift alone does not disprove arithmetic. |
| 13 | Recheck | 35 | swf_text_font_platform_v1.cpp | IDs 23473-23475,23477-23508 use old 1faa3f2e hash. New image-release/reset state and headers affect filter/glyph/capture ownership discussions; dispositions were unclear and should remain so pending comparison. |
| 14 | Refreshed helpers; new edges remain | 55 | gfnt_text_backend_v1.cpp; swf_text_font_platform_v1.cpp; text_render_owner_v2.cpp | All 55 unique changed-citation rows carry current cpp hashes (53/55/3 per-file hits overlap). Existing F5 remains partial. Changed headers and application process-slot reset binding were not cited, so do not extend helper reread into full Language reset closure. |
| 15 | No identified impact | 0 | none | No exact changed-file citation or concrete changed dependency found in the scoped existing report. |
| 16 | No identified impact | 0 | none | GLX unknown front-dispatcher and Lua random findings are separate from new achievement-message interception. The interception handles Next/SkipAchievementMessage, not GLX login/sign-in; no status change supported. |
| 17 | No identified impact | 0 | none | Audio/codec and conditional DRM findings are outside the listed changed source leaves. No concrete affected path established. |
| 18 | No identified impact | 0 | none | Storm shader/dimension/PVRTC comparison leaves are unchanged by the supplied list. Common renderer dependence alone does not invalidate these bounded comparisons. |
| 19 | Recheck anchors | 0 | model_renderer.cpp (Markdown) | F2 shader/GPU include connection cites old renderer snapshots (text c35716fa, table bb979002). Current includes :2313/:2976 are present; source binding still exists, but captured caller provenance is stale. |
| 20 | No identified impact; keep platform limits | 0 | none | Current locale mapping, browser/IGP queues, Java captions and GLive platform consumer claims cite other files. Achievement-message interception supplies no nativeOpenGCLive bridge, so it does not close F20-02. |
| 21 | Partly refreshed; recheck new closure | 228 | character_script_session.cpp; native_character_menu_v4.inc | 166 session and 62 AS rows carry current hashes. NPC-020,125,126 remain partial. New CharacterGameDesign -> OriginalUiSession -> renderer delegate is not in those recorded sources; new movie interception/font-reset path also needs a bounded edge pass. lane-21.md was absent throughout sweep. |
| 22 | Recheck | 1 | character_game_design.cpp/.hpp | L22-U16 still correctly calls reload policy unresolved, but old snapshot-only hashes omit the new pinned process provider and initialization/lender lifetime. Follow owner publication, ready groups, teardown and live borrower behavior. |
| 23 | Recheck/update narrative | 2 | character_script_session.cpp; model_renderer.cpp; delivery tracker | 23-06 callback source fingerprints/line anchors are stale. 23-05/06 must add GetPyOID -> AnimDict upstream closure; 23-11 must record newer planning/artifact snapshots. Hash-family and six stale negative claims are not invalidated by these edits. |

## Concrete changes affecting the reasoning

### Stage 17 now requires a process-owned design lookup review

The fixed original path is `LuaScript::_GetPyOID` (#2996, `0x37f5fc`) -> original process `PyDataArrays::GetOID` (#9508, `0x4bd640`), with AnimDict getter #2418 at `0x364b80` and AnimDict name/data loaders #9350 / #9444 at `0x4b1f94` / `0x4b85a4`. Original `0037f5fc.c` captures the process arrays global and passes both authored strings before pushing the result. SwampKing's `RegisterAnims` therefore depends on GetPyOID before the RegisterAnim adapter is reached.

Current `character_game_design.cpp:49-68` adds a retained `process_design` provider/owner and delegates known original groups outside the six local registrations. Its overload at :89-108 pins that provider in the snapshot. `character_game_design.hpp:73-80` declares this broader ownership contract; the no-provider overload still exists.

The actual current construction path is `model_renderer.cpp:944-950,967-972`: `process_array_design_lookup_v101` invokes `SourceProcessArraysV101::get_member_id`; `load_game_design` borrows current process arrays and passes that provider plus the shared owner to initialize. `original_ui_session.cpp:94-95,699-703,1144-1145,1165-1168` publishes, borrows and conditionally retires the process owner under a mutex. The loan itself establishes an owner, not that every names/data group is ready at every initialization call. Failed borrow leaves the no-provider design path, whose known extra groups still fail. Recheck initialization timing, selected owner identity, readiness, reload and VM-finalizer lifetime.

Current `character_script_session.cpp:19,208,225,228-237,332-334` propagates motion/animation provider failure and design query delivery failure as `DH2_SCRIPT_REQUIRED_SERVICE_FAILURE`, including when authored Lua catches a diagnostic. This changes the failure contract compared with the f1604fe5 source cited by lanes 02, 03 and 23. Lane 21's current hash/handler anchors include this wrapper, but NPC-020's recorded sources omit the changed design snapshot, lender and renderer constructor.

Affected obligations: lanes **02, 03, 04, 05, 06, 21, 22, 23**. Pending/unclear original records do not become matched because the new provider is visible. Lanes 05/06's local missing-file differences remain visible: current `original_ui_session.cpp:660-678` still latches constants read/debug/load failure, and :680-693 still propagates array loading failure. Their broader process-consumer closure has changed.

### Font reset spans headers, helper owners, movie slots and renderer images

Current GFNT source shares faces by resource URI (`gfnt_text_backend_v1.cpp:28-39`), returns a delivered bitmap-image miss when the original bitmap-cache dimensions are disabled (:67-72), and exposes `clear_fonts` through `TextFontBackendsV2` (:80-87). The changed `gfnt_text_backend_v1.hpp` and `text_render_owner_v2.hpp` are interface dependencies even where a report cites only their cpp consumers.

`text_render_owner_v2.cpp:126-129` invokes the optional backend clear hook before clearing faces/images/clones. `swf_text_font_platform_v1.cpp:217-227` releases exact renderer image owners before clearing font/core/image registries. Current movie services wire `release_image_v119` to `gpu.source_remove_image_v119` (`original_ui_movie_services_v1.inc:17-18,41-43`).

The new outer path is `native_menu_process_bootstrap_v104.inc:113-135`: a weak same-owner callback binds process font reset, borrows populated process slots 0/2/3, verifies actual movie/receipt identity, and calls `SwfMovie::source_reset_fonts_v119`. Slot 1 is expressly level-owned. `native_character_menu_v4.inc:5` now derives from `enable_shared_from_this`, which supplies the weak owner used by that callback.

Lane **14** reread current cpp helpers and retained partial F5; this is good evidence for those reviewed differences. It does not establish full Language/reset caller closure, updated header ABI, all live movie owners, failed release rollback or teardown ordering. Lane **13**'s 35 affected filter/cache rows retain the old platform hash and need refresh. Lane **04**'s bootstrap/menu lifetime candidates and lane **21**'s movie callback/owner edges need the new reset path considered. Lane **01** already refreshed its native-menu fingerprint and F3 queue search; that does not turn its separate input-queue question into font-reset acceptance.

### Renderer edits mostly require caller provenance refresh, with one new semantic provider edge

Current renderer includes are:

| Connection | Current line |
|---|---:|
| `renderer_native_cleanup_v50.inc` | 1282 |
| `renderer_authored_effect_scene_v5.inc` | 2312 |
| `renderer_native_batch_shader_v113.inc` | 2313 |
| `renderer_native_batch_gpu_v111.inc` | 2976 |
| `dh2_camera_gpu_projection_v12` call | 3080 |
| `source_store_menu_cut_screen_v62` body | 3127–3129 |

I reread those selected current anchors. The cut-screen body still replaces only the low byte of `controller_global_blocked`; lane 04's #7455 matched local effect remains supportable at the new anchor. Cleanup/material/GPU includes and the camera call remain present, so a stale whole-renderer hash alone does not establish disconnection or an arithmetic regression.

Lanes **08, 09, 12, 19** need their cited caller locations/hash provenance refreshed. Lane 09's 115 direct-citation rows are unclear/clone, and lane 08's five are unclear candidates; preserve those qualifications. Lane **11** already recorded current de089594 and reread its factory selection. The semantic new renderer obligation is the process-design construction edge above, rather than every renderer-dependent standalone kernel.

### New movie interception adds a route not named in the previous source lists

`original_ui_movie_services_v1.inc:28-34,41-48` now checks `achievement_native_v119` before delegating to the application's native action handler, and ensures Next/SkipAchievementMessage are registered on the separate movie service packet. Current included `original_ui_messages_v97.inc:76-106` handles those two names against the retained achievement-message queue.

This calls for a bounded **lane 21** recheck of the actual selected movie's registration -> intercept -> exact queue owner -> synchronous AS effects. Its SWF census lists these names on HUD assets but does not itself prove this new selected owner path. The intercept does **not** handle GLX login/sign-in or supply `nativeOpenGCLive`; lanes **16/20**'s separate unsupported online/platform endpoints are not contradicted by this change.

## Planning documents, fixtures and receipts

| Evidence | Current effect on prior audit narrative |
|---|---|
| `docs/ACT1-DELIVERY-TRACKER-2026-10-06.md` | 17:37 “Live refresh” describes an AnimDict source blocker and later focused host suites. Its 535/465/21/133/2 checklist totals agree with lane 23's later snapshot; no new aggregate status change was observed here. Its six-table-rejects-provider description predates the present process-provider source and must be version-qualified. It still reports no integrated Act 1 acceptance. |
| `coordination/luna-act1/ledger.md` | Same 17:37 reconciliation; distinguishes older Stage 10 and 15:44 Stage 17 evidence, and records the separate 15:04 creation-confirm crash. These add historical runtime cases for lane 23's adversarial narrative and lane 21's creation/menu closure; they do not establish current-source runtime behavior. |
| `port/level-world/tests/character_script_session.cpp` | The changed fixture separately exercises no-provider failure and the real decoded process-array provider (:130-143). The negative no-provider case is not evidence that the newly connected production initializer must fail. No existing lane directly cited this fixture; reported check totals remain external host evidence, and this sweep did not run them. |
| `.local-inputs/dh2-loading-progress-logcat.txt` | Observed mtime 15:44:33, predating the pause and new source. Historical MarkAsFlying stall; cannot be attributed to the current delegate/failure-wrapper source without replay. |
| `.local-inputs/codex-v176-swamp-logcat.txt` | Observed mtime 09:03:04. Older Stage 10 receipt, already superseded by the later Stage 17 log in the tracker. |
| Current debug APK | Replaced outside this audit at **18:04:19 local**, size **643,008,166 bytes**, SHA-256 **`6a26a65247205077d17152867b2f2dcac783610b537ac689636a845b68e99e85`**. This differs from the tracker/ledger's 16:51 artifact (`748c25ee…`, 632,616,710 bytes). Only file metadata/hash was read; this sweep did not build, install, launch or validate it. The matching external build receipt and whether it was replayed were not established. |

The planning docs' test/syntax/build statements are assertions in those documents and must be matched to their source/build receipts by the integration owner. Their arrival after a pause does not retroactively upgrade any audit lane's runtime field. The observed current APK also makes “latest APK is 16:51” a historical statement even though its gameplay status remains unproved.

## Source and planning-document fingerprints

All 17 supplied source/fixture/planning paths were hashed at initial and final read; **none changed during this sweep**. Their mtimes below establish that their present files are later than the requested cutoff, but mtimes do not reconstruct an exact patch. Several pre-pause baselines are hashes only, so this report does not claim to identify every post-pause edit within each file.

| Path | Local mtime | Current SHA-256 |
|---|---|---|
| `port/android-native/app/src/main/cpp/original_ui_movie_services_v1.inc` | 2026-10-08 17:32:12.1243392 | `d6fa672f4b1b8de09dc8df6519bb8e3c5bf170cea82e0ca4eb47698b3fb2fa2f` |
| `port/level-world/character_script_session.cpp` | 2026-10-08 17:35:32.9322742 | `8ab0110f18e758f766e38213a411b0e978f5f5f2fef4f3702f4b148a763f630e` |
| `port/level-world/character_game_design.cpp` | 2026-10-08 17:42:49.8697150 | `51444c7524394eba045744a2c7ddca90c5b2b8bb2ccbb8650aef99ef418bb8c8` |
| `port/level-world/character_game_design.hpp` | 2026-10-08 17:40:36.3514614 | `7c640ec5c3abd50117d6fd33d43322516819f8462d77126bef13b91f181e705e` |
| `port/android-native/app/src/main/cpp/original_ui_session.cpp` | 2026-10-08 17:56:19.8081088 | `d49dacbfd28ad4246604199608d4c129d25b8a34838a53c863e8bd4c7f61ca18` |
| `port/android-native/app/src/main/cpp/original_ui_session.hpp` | 2026-10-08 17:40:48.8688988 | `4efa3ebdd7cb260492ad7be7817737a59fdd0c742b3149812f5141617b16388f` |
| `port/android-native/app/src/main/cpp/native_character_menu_v4.inc` | 2026-10-08 17:53:16.6793376 | `d6d00fb8ecce412ff73db91f6b0bd42f5d0b7a9cb61c8177c682cbfe64eee557` |
| `port/android-native/app/src/main/cpp/native_menu_process_bootstrap_v104.inc` | 2026-10-08 17:53:22.7099444 | `21bf7754bf4b4d99f09ee9436227e87d2dea62dbfd7b36a6312129b4666c7a1d` |
| `port/android-native/app/src/main/cpp/model_renderer.cpp` | 2026-10-08 17:41:02.8595350 | `de0895946c87a3a09df48581b1d69db8f5676a870f4683951886b78b93ab20ca` |
| `port/engine-ui/gfnt_text_backend_v1.cpp` | 2026-10-08 17:35:36.7889744 | `87da563d2b7a36d7afff3461e973b20059b0250cc369f4cbf99647ae381e3e9a` |
| `port/engine-ui/gfnt_text_backend_v1.hpp` | 2026-10-08 17:35:36.7873850 | `4e9425ce9e4133c8f7c4a3df9d023692ad25b4232b08438f296ee4b164e6034e` |
| `port/engine-ui/swf_text_font_platform_v1.cpp` | 2026-10-08 17:35:36.7900668 | `30807835dafb798999781a0b7088bba6619fbd3e17f034292ddcd4066f5ebad8` |
| `port/engine-ui/text_render_owner_v2.cpp` | 2026-10-08 17:35:36.7879257 | `6ee8e338fafcc60b94fee38d3fcd39d445847bda6de4451a51bca383da32e140` |
| `port/engine-ui/text_render_owner_v2.hpp` | 2026-10-08 17:35:36.7863135 | `4e72399c7fe8fee28f60216060a3ef6549b0b85cc41a04945caca93fcfb3db8a` |
| `port/level-world/tests/character_script_session.cpp` | 2026-10-08 17:46:10.4827761 | `52e3ffd3c1956984ee8ebc908eaf6acd6234e9c95245c025aeaeebbff1fefebd` |
| `docs/ACT1-DELIVERY-TRACKER-2026-10-06.md` | 2026-10-08 17:42:20.6336151 | `2400bdb23a39af3341c6a1385fa78b1a0da7f8bbd5d016db3ca0d628dab74307` |
| `coordination/luna-act1/ledger.md` | 2026-10-08 17:42:20.6204216 | `c671311f142166977665478efd124fbc32a6240b0f8723b9e3bad93c671df748` |

The inspected additional helper `port/android-native/app/src/main/cpp/original_ui_messages_v97.inc` (SHA-256 `f908c7dca6927261760c015f6c76bb21b56719bcaf3f1560f2e96bdc3495e043`) was read only to identify the names handled by the changed movie wrapper; it is not asserted to be a post-pause changed file.

## Exact affected-row sets

The following are citation sets, not completed-comparison sets. They retain their prior partial/unclear/clone dispositions until the appropriate bounded recheck.

- Lane 02: 21 rows — 1960, 3434, 3438–3440, 3446–3447, 3450, 3452, 3465–3466, 3468, 3470–3472, 3474–3475, 3477–3478, 3489–3490.
- Lane 04: 6 rows — 5720, 6151, 6655, 6673, 6910, 7455.
- Lane 06: 8 rows — 9517–9520, 9523–9525, 9539.
- Lane 08: 5 rows — 14779–14780, 14784–14786.
- Lane 09: 115 rows — 14957–14968, 14970, 14972–14989, 14991–15012, 15014–15050, 15052–15053, 16632–16646, 16648–16655.
- Lane 12: 1 rows — 21438.
- Lane 13: 35 rows — 23473–23475, 23477–23508.
- Lane 14: 55 rows — 24556, 24608–24610, 24622–24628, 25066–25068, 25075, 25099, 25556–25557, 25559–25561, 25566–25569, 25574–25575, 25579, 25586–25593, 25719, 25721–25725, 25735–25738, 25740–25748.
- Lane 21: 228 unique rows; all 62 AS rows, 164 NPC rows selected by the exact current session path, plus CORE-01/CORE-02. NPC-020 and NPC-125/126 are the specific new design/dictionary edges. The complete selection is reproduced by exact changed-path matching in `lane-21.csv`.
- Lane 22: 1 rows — L22-U16.
- Lane 23: 2 rows — 23-06, 23-11.

## Version of reports inspected

No lane artifact changed between the scan and final rehash. The hashes below pin this sweep's report evidence; `lane-21.md` remained absent. A later resumed lane may supersede these snapshots.

| Lane | CSV SHA-256 | Markdown SHA-256 |
|---|---|---|
| 01 | `63f90e2b82515bbc7bc29ff618320eb48ac2b92fa6dff3424cce7d9a7aa7ab12` | `958859b3a90a956fada1edb3759c80ace4b2f79336bd9d4383f271f3423fe202` |
| 02 | `77bb2fd546e52ba84e87afbbfaa8792dd53665bc52c9c9fb66d0584b821b3f15` | `28d6de5ba2b2df3c0f5a52deef44803da00d48c2108c3ebb27fc195017aca144` |
| 03 | `df7bbb2291800f84748b75103ce0fe37ef26886510e198880ce1bc90bea424ae` | `89275fcce0b53330a072dad3cffe6bae481e63157e8ba0e24a87f4d3d64f1c25` |
| 04 | `b7c89ba0b61f89f948462d7315376310bb1cbb1da371493e9398d13704b520a8` | `9724aa8f5ebe94990b831a9f9ac5cb7fd46e327a4edc3e1f238865663e45e65e` |
| 05 | `c805f9ae5255f1a0ef75e30795488592815052b18061888b663ed1101113c0a5` | `6a964db1e6f90792cd1dfe613172e1f330043db8089a504e6b8364faf4b0ec2d` |
| 06 | `ae54d694b2f1fca876aa4056aa0732c72fa7cfc44e1eb4a272a0997e893c6ee8` | `ad555f04658ad718c7706e5169f26d8ff6c64db5c1c767a34c32db668e50b31a` |
| 07 | `7ddf1fbac26aac7f0bd6bdaf4ca1cb4b9981a9c89525f9466f823e2d26d8f310` | `5ff155e757cf45261fd4b5803ad1399c061e009219033e9aed1f1fcb048112f2` |
| 08 | `de4a33046b768ef601a9305f5248169c47c67f43b55e05c86ae874135a26724d` | `4ef9b00da499f11424e916353c5f1dd2dfa717ab7f78f18b77201b7b3086d7f3` |
| 09 | `09a65865ff6832f864de0afdc60df5597e5cb8e9f4b12a250f2f51801e2f38a3` | `6eacd626dee5c20dc0e97ced60e4cbec33599dc50678b1baa8a053e4d7f261c4` |
| 10 | `e09bf7c97db775bd2a4a4f84dbefd9440261531400c5030ff4775b019a887719` | `6d3a1432cc88810cc2730ba3ff8201afbf7703fd58d48bcf9865400cef24805f` |
| 11 | `e77ceb8362a4006e6d8b2120c8438862f4efe2692e6ed07373ce2050f426c4f2` | `157abad7c6cfa07a1d5809405d58f6a8238ae0d1237f212d63e25660834556af` |
| 12 | `b065c0942958fa4f36eeea4abc41c20384791f35f38e617f35725e4c594ccd6b` | `d2d2e7b2f588a208ddd8a84405442ef2201b5d6a3ddb5609d70107492740100e` |
| 13 | `7edeeec14f0c3c0030818810edef1101d699aa499aa13d108b8766fae1c244bb` | `240f7ba0dd204e3e780287f8f460fb1a19c54e4589aa14129f7de84887e3a9dd` |
| 14 | `c395b35e6e07d3938bff9af4ce8f219e16618a107bd13c6059400f403ff7664e` | `0c53fb2ba52e1281f8db72a17f06d4c3437c9b3be3fd316a58602853f94996bc` |
| 15 | `0cdbf3faea16c1360d42d7bf957a93a88f76072e5658cf051939fe0b21ff77ac` | `a18ab9b289c0458bf41539a4943e19d38ab67f8e637fe53cca2aa2d2c60c0ce8` |
| 16 | `fe7e2dff2f073a524cddbd7e098c7a9c63d284a57dceeb48636829d5da4144ce` | `38b009269fa229304e3b1cffdd2cd820ae8358eaee85c09f5f0c039cd0e03732` |
| 17 | `0bda7c0b60a0d306819c31fea37ca59173d71b302f2a721dc8ebc56c1ac6281d` | `843a6465c2e229ede5ec7958e20af5d1d42920c5105a46f76068dbca7764bb15` |
| 18 | `57443394c9f4a8b2df59bb8cdc5051e1e88471757972bbe0872c4d013195f494` | `f83f27624ad1d391d557cc2ac1a1251d3d846656ccada08afe47743cf0b6e44f` |
| 19 | `4b782263b56f3453b0036e214c94fd4e3b5b929905edd145044e09ed7eec829b` | `552d9535b14b1c9f9f5c76f84157bf732c63fa3eff01614452e36a7949adc33c` |
| 20 | `0b3e0985d940abe39ba4cdcb3c0bc80fb0c9bd30a1ba984bec7acf990822bac3` | `c789fea64a7f28d6eeb86ebc6ed4fb87b9df012715555fdf11f38012c0cfeec2` |
| 21 | `059d73048851bb3187814f958fa02609cd8746b057d1fab29ed49e4ce619af6a` | absent |
| 22 | `84d33cc7b14812a47b610ada67768bdc6ad69ee407b6dfaf6d02e0c6e5849e2c` | `8c183da542e706506d8d6c19d8f5dbbe2b98e4fc9857af8d7947f4012cef1aef` |
| 23 | `b957fd2a1b512b2341b0de9ddfef68a882df3f2b34e3d7075374ebfd218e18ea` | `cfea42aecde76399cf3f1592b0d72acf0f27094acdc2ad63a5f1d0cfa26bad03` |

The initial impact sweep concluded for the supplied changed-file set and the report versions listed here; the later reconciliation below supersedes its open-refresh dispositions. It does not complete the underlying 37,813-record semantic audit, prove all transitive source dependencies, or provide gameplay acceptance.

## Final post-pause reconciliation — snapshot ending 18:33:34 local

This section supersedes the initial **recheck obligations**, while preserving the initial measured counts, source fingerprints and report versions above. The source sweep ran **2026-10-08 18:33:27.9331913–18:33:34.4663657 Asia/Jerusalem (+03:00)**, ending **15:33:34.4663657 UTC**. It enumerated **18,950 files under port** with `rg --files`, excluded path components `build`, `.cxx` and `.gradle`, and examined modification times for C/C++ headers/sources, includes, Python helpers, CMake scripts and CMakeLists.txt. This is a bounded filesystem snapshot, not a guarantee against writes after the endpoint.

All **12 requested lane refreshes** (02,03,04,05,06,08,09,12,13,14,19,22) are now written. The extra source-facade provenance refreshes in lanes **04/13/14** are written too. Lane 21 now has a finalized Markdown report and **579 edges**, including CORE-14/15/16; its owner and normal-startup readiness notes were read. Citation refresh completion does not complete the semantic comparisons or their runtime acceptance. The following table is the current disposition; the initial table above remains historical.

### Current all-lane disposition

| Lane | Completed refresh / retained evidence | Open semantic or runtime work |
|---|---|---|
| 01 | Initial current native-menu fingerprint and F3 reread retained; artifact unchanged. | F3 input queue and separate reset closure remain bounded/partial. |
| 02 | 21 affected rows refreshed; #2418/#2996 bodies/ASM newly inspected but remain unclear. Neighboring Front/FSM hashes corrected for #1960 and #3440/#3446/#3447/#3474/#3475. | Ordinary HasPath failure still returns 1; all wrappers were not converted to required failures. TargetList/self/position, method/VM lifetime and full lookup domains remain unresolved. |
| 03 | RegisterAnim #4329, AddAnimDict #4764 and neighboring #4077 reread; current session hash and design failure/close/lifetime dependencies recorded. | These records remain unclear. Float conversion/finite-domain differences, manager #8006 guards, frozen-library failure prefixes and aliases remain open. |
| 04 | Six affected citations and ResetFonts #6843 reviewed; #7455 low-byte effect still matched. Extra #6308 selected overlay glyph/preload path and CMake selection refreshed. | Five original candidates retain clone/unclear; #6843/#6308 unclear. Native four-slot reset vs process 0/2/3, selected level slot1 route and nullable/filter/reentry domains remain open. |
| 05 | PyDataArrays.Load #9176 caller/provider provenance refreshed; partial retained. 79 cited source fingerprints checked without broad semantic reread. | Local missing-read failure forwarding remains a difference; provider reload/global selection/finalizer/readiness parity remains outside whole-function completion. |
| 06 | 11 rows #9350/#9444/#9508/#9517–9520/#9523–9525/#9539 reread/current anchors. AnimDict phases29/64 and normal startup sequencing reviewed. | Constants sticky failure and names-count questions remain; reload/multiple-publisher identity and runtime boss execution unresolved. Early publisher alone does not prove an early Stage17 query. |
| 07 | No identified impact in the original set; artifact unchanged. | No claim that all transitive consumers were inspected. |
| 08 | Five renderer candidates refreshed; unclear/low/not_run preserved. | Generic mapped stream mutation/unmap ownership and remaining buffer semantics unproved. |
| 09 | 115 renderer citations refreshed (105 unclear,10 clone); selected cleanup/effect/shader/GPU/projection connections present. | Driver/callback closure, dirty masks, argument domains and material/render parity remain open. |
| 10 | No identified impact in the original set; artifact unchanged. | Remaining animation/scene semantic work unchanged. |
| 11 | Initial current renderer/factory selection evidence retained; artifact unchanged. | Does not close all particle/scene branches. |
| 12 | #21438 standalone arithmetic match retained; caller at renderer:3080 refreshed. | Driver/producer identity, orientation/flip/target domains and integrated rendering remain open. |
| 13 | 35 text-platform references refreshed, still unclear. Extra six overlay rows #24008/#24057/#24105–24108 reread/current hashes: two scoped matches and four partials preserved. | Reset/release prefix, retry rollback, retained borrowers, reentry/shared contexts and slot1 selection remain open. Cold main-root display still lacks the original initial advance on the inspected route. |
| 14 | Current GFNT/platform/render helpers plus changed interfaces and actual process/movie/image-release path reviewed in S8. Overlay current hash replaces prior citation version; 99 originally stale overlay rows are corrected and seven additional dependency rows also carry current overlay references. | F5/#25099 partial; #25064 unclear. Default-context player union, callback snapshot/order, atlas/glyph equivalence, release rollback, retained pins and full Language/reset selection unresolved. |
| 15 | No identified impact in the original set; artifact unchanged. | Existing semantic limits retained. |
| 16 | No identified impact in the original set; artifact unchanged. | Achievement interception does not close GLX login/sign-in. |
| 17 | No identified impact in the original set; artifact unchanged. | Existing audio/codec/conditional-DRM limits retained. |
| 18 | No identified impact in the original set; artifact unchanged. | Existing Storm shader/dimension/PVRTC bounded comparisons retained. |
| 19 | F2 renderer shader/GPU anchors refreshed in Markdown; CSV unchanged. | Broader optimizer/parameter/render parity remains partial, not_run. |
| 20 | No identified impact in the original set; artifact unchanged. | NativeOpenGCLive/platform/JNI limits remain; no new online bridge acceptance. |
| 21 | Finalized 579-edge report, retained Character/generic-owner dependency notes and CORE-14 readiness reconciled. CORE-14/15/16 source hashes checked independently below. | All 579 edges not_run; full Stage17 init/first update, dynamic assets, reload/global identity and achievement producer/skip-string domains remain open. Selected-facade provenance qualification appears below. |
| 22 | U16 retained provider/publication/readiness/lifetime reread and C11 late actor/base owner reread written, both partial. Fixture/helper excluded from production comparison. | Generic reload, multiple publishers, exact virtual teardown and whole Character/GameObject lifecycle parity remain open. |
| 23 | The original adversarial report is preserved. This reconciliation version-qualifies its stale callback/design/tracker claims and records later receipt drift without editing lane-23.md/csv. | Hash-family assumptions, six stale negative claims and semantic coverage gaps remain; current static provider connection supersedes a universal six-table-only blocker claim. No gameplay acceptance. |

### Missed initial inventory entries and late source dependencies

**The source-facade overlay was omitted from the initial changed-file inventory.** `port/engine-ui/overlays/source-facade-v1/swf_movie.cpp` was already modified at **18:01:48.3333352**, before the initial 18:11 sweep. Its current hash `0370235d…` supersedes lane citations `6ba7aa32…`. This is a missed pre-snapshot citation, not a new post-snapshot source edit. The extra bounded checks cover lane04 #6308, lane13 #24008/#24057/#24105–24108 and lane14's 99 initially stale overlay rows. Current lane14 CSV includes 106 overlay-citing rows because S8 added seven concrete reset/interface dependency citations. Historical prior hashes remain in its evidence fields; these do not mean the active source fingerprint stayed stale.

Two neighboring lane02 files were also absent from the supplied post-17:32 source list: `front_ui_session_v87.cpp` (17:28:33.5500124, current `3df55182…`, prior `f88ceb1e…`) and `source_campaign_character_fsm_v101.cpp` (17:31:33.2028563, current `e4a9925f…`, prior `3c4ceb5d…`). Both mtimes predate the pause cutoff. Their current versions already existed before the initial snapshot; this followup corrects neighboring stale lane02 citations, not a new source edit during reconciliation.

The genuinely later production dependencies are `retained_character_actor_v1.cpp` and `canonical_gameobject_base_owner_v1.cpp` at **18:15:29**. No old lane report directly cited these paths when first identified; that did not exclude their call/data dependencies. Lane21 follows CandidateActorV60 -> RetainedCharacterActorV1 -> same property/life backing -> CharacterScriptSession and strict script publication/final/first-update gates. It separately follows the generic base pointer-cell receiver for CORE-11; it does not substitute that generic receiver for the Character seed. Lane22 follows the same retained actor through VM close before member destruction and the generic Handle/source-release owner, preserving C11 partial.

The current actor constructor explicitly initializes inherited byte **0x83** at `retained_character_actor_v1.cpp:13`; the base constructor includes 0x83 in its zero initialized field map at `canonical_gameobject_base_owner_v1.cpp:6`. The inspected batching producer reads this exact cell through the Character inherited-field view or generic base (`renderer_source_batching_v96.inc:59,66`, stored in its legacy `deleted83` field). This supplies a concrete property/construction -> batching data dependency, even though a filename search originally had no direct lane citation. It does not establish every property override, serialized producer, batching branch, or successful Stage22 execution. Lane04's batching candidate and lane01's property leads retain their original scope; the late lane21/22 owner rereads do not promote those whole-function comparisons.

The actor fixture remains distinct from production. Its witnessed hash sequence was:

| Observation | Fixture SHA-256 | Meaning |
|---|---|---|
| First late report, about18:15–18:17 | `9a01cb9f1b70201938eb46e3ee76e209438a9b0f1a3b13540a56ab02603e717b` | Parent-reported first late version. |
|18:20:10.8769086 mtime | `2dd061a481d7714f95645c848803ff8ddc353556d6c96c179cd9a1e53fbae90b` | Later observed fixture version. |
|18:21:14.3684381 mtime; still current at18:33:34 | `042a4b8f94a7d1d0f49120538753cd8189fa2fc0969c95669f7f3fa824f63761` | Final pinned fixture, 4837 bytes. |

`tools/run_retained_character_actor_v1_host.py` changed at18:18:52.3647901 and describes building/running O1/O2 sanitizer commands and emitting a receipt. It was only read. The 18:28 source scan additionally found `port/android-native/tools/launch_guarded_emulator_v36.py` (18:28:21.2676395) and `port/android-native/tests/test_authored_loading_screen_assets_v1.py` (18:27:57.5880732). No existing lane MD/CSV directly cited either path in an exact-path search. Their file headers describe an emulator launcher and a loading-artwork asset fixture; they were read, never executed. These are tool/fixture drift, not new evidence that the game or audit ran.

The closing scan also includes the two files added after18:28:31: `port/android-native/tests/test_menu_preview_selection_connector_live_v1.py` (mtime18:29:13.9199374) and `port/android-native/tests/menu_preview_selection_connector_live_v1.cpp` (mtime18:29:32.2134295). The Python helper explicitly describes a host compiler fixture, with no application target/APK/ADB/emulator; the C++ fixture supplies mocked Character creation/scene/camera and Load4 boundaries while linking/extracting selected production connector bodies. I read both files and found no exact-path lane citation; I did not execute them or inspect a receipt accepting this new fixture. Their creation is test evidence pending any separate bounded result, not production or in-game acceptance.

### Independent CORE-14/15/16 checks

The finalized rows remain **partial / not_run**. At the final source capture, **53 recorded source-hash entries across 32 unique files matched current bytes, with zero mismatches**. This authenticates those cited versions; it does not complete every branch.

- **CORE-14:** IDA GetPyOID #2996 `0x37f5fc` reads the process arrays global and passes both authored strings to GetOID #9508 `0x4bd640`. Current session design binding -> CharacterGameDesign known-group delegation -> renderer's borrowed process arrays is present and pinned. Normal GSInit waits for arrays completion before main entry/ready and frame processing. AnimDict record/name phases29/64 lie in that completed sequence. A loan lacking its own ready check therefore does not establish an early-query defect on this path. Zero rows returning -1, nonempty missing names failing delivery, later reload/multiple publishers and actual Stage17 execution remain distinct unverified domains.
- **CORE-15:** IDA reader #7044 `0x440278` copies front message and writes Name/Desc/TrophyType/Grade/Label; current `original_ui_messages_v97.inc:76–95` copies the retained queue front before synchronous SetMember writes and returns false for empty. `original_ui_movie_services_v1.inc:28–34,43–49` supplies interception/registration before the application delegate. Producer lifecycle, invalid object/coercion/reentry domains and runtime effects are not fully established.
- **CORE-16:** IDA skip #7062 `0x442994` pops then conditionally calls SkipFuncName/StartFuncName. Current source pops and starts the remaining achievement through `onAchievementMessage`. The original global-initializer proof for the claimed BSS-null skip callback remains incomplete; keep partial.

**Selected-facade qualification:** the first CORE-15/16 source manifests named `port/engine-ui/swf_movie.cpp`, while `gameswf_source_facade_v1.cmake:26–27` removes that TU and selects `overlays/source-facade-v1/swf_movie.cpp`. A matching inactive-file hash alone cannot establish the selected Android route. I separately read selected overlay `:281–313` (real player, owned graph/provider, install_native and root attachment), `:569–580` (idempotent later native registration), and its Scope/AsContext owner checks, together with `swf_actionscript_connection.cpp:102–115`. This supplements the static route evidence, with current overlay `0370235d…`. Lane21 completed its own selected-facade followup at18:32:28.8991536/18:32:28.9116954 (CSV/MD mtimes): CORE-15/16 now cite the active overlay and declared CMake/provider path; the prior inactive hashes are retained as historical witnesses. This bounded provenance obligation is closed. Both edges remain partial/not_run; full callback ABI, synchronous reentry and producer acceptance remain open.

### Receipt replacement is separate evidence

The **old Oct5** `retained-character-actor-v1-host.json` observed during the earlier reconciliation was **5924 bytes**, mtime **2026-10-05 18:16:04.4463159**, hash `48020c7421f2a53735fb52bf754ef4faf683841ca3992cb694c82311ce447e8a`. It reported PASS but had four source-hash mismatches:

| Source | Old recorded SHA-256 | Final current SHA-256 |
|---|---|---|
|retained_character_actor_v1.cpp|34507168f193c2d7e58a803284115402714be9182be7ed034937e85ac056c8e4|b88abef1cf3116bc321e98ed643fde226a914f76a8fb16968a3a09b12f1b0d60|
|canonical_property_map_v1.cpp|7733cd95b5c8cfd4a7507498c2203e94f3e4621cba81635eed2021f3078377f1|d2f32d25e001815237b74e1203a5927d912c8f13e744e41db177c0c9e26d4a72|
|character_world_npc_state_owner_v1.cpp|b63faf89e423df34c46dbf929a7945d940c436eb94b0d32096315c8cded5b018|58cf9343ba7ba546238b809d14beb2ef16d121c996a98faf996aaee65bb9b823|
|tests/retained_character_actor_v1.cpp|3e0f0a6f8bc7cf96bc93a58d45dcd1d0a2978a542d9d330f1eb9320d7f518097|042a4b8f94a7d1d0f49120538753cd8189fa2fc0969c95669f7f3fa824f63761|

That old receipt cannot accept the current source. During this audit the on-disk receipt was **replaced externally**: mtime **2026-10-08 18:23:07.3268716**, **7150 bytes**, current hash **43df298366ed51ff42eabd5144ee9a4ab1b21de8ea30a14b782f09bed9dd40ac**. The replacement reports PASS and four command exit codes 0 (O1/O2 compile and host invocation). At 18:33:34 all **14 recorded source hashes** match the current files: retained actor, canonical factory/property map/base owner, kill/set-position fields, world NPC state/initialization, state extensions/behavior/frame, world AI queue, idle events, and the distinct actor fixture. The eight recorded dependency-library/archive hashes belong to the helper's frozen host snapshot; they were not revalidated as the current Android binary closure by this sweep.

Its own scope is constructor fields, typed defaults/overrides, unavailable-producer failures and shared Handle/World lifetime. It explicitly excludes **graph resource/session loading and complete InitPost**. The audit **only read the replacement receipt and its helper**. It did not run those commands, refresh that receipt or infer in-game acceptance, Stage17/22 success, constructor-complete parity or integrated first-frame acceptance. Every lane CSV acceptance field remains not_run.

### Final counts and fingerprints

The final report scan reads **23 CSVs and23 Markdown files, 38,419 CSV rows**: primary lanes01–20 total **37,813**, lane21 **579**, lane22 **16**, lane23 **11**. The four-row increase from the initial38,415 scan is lane21's575 ->579 finalized edges. This row count is not a new complete semantic coverage claim or an independent proof of unique inventory assignments. All **38,419 runtime acceptance values** are not_run; lane23's separate historical `runtime_evidence` prose is not an acceptance field.

The initial **477 rows** metric remains scoped to the original supplied changed-source list. The missed overlay and additional explicit reset/readiness/owner dependencies mean it is not the final total of all changed-source citations. No global final affected-row completeness is claimed.

All **17 initially pinned source/fixture/planning files** retain the same SHA-256 at18:33:34. Tracker and ledger still have the earlier17:37 refresh, totals535/465/21/133/2, and no integrated Act1 acceptance; their six-local-table blocker description is historical relative to the current delegated provider. Build/APK and older emulator receipts remain external evidence; neither the initial nor final impact sweep ran them. The final new/missed-file fingerprints and all lane report fingerprints follow.

### Final changed/missed/additional file inventory

The first17 fingerprints remain identical to the initial table. The supplemental fingerprints below include late source activity, omitted citation versions, and separately read helper/receipt dependencies. Only the eight source-like paths returned by the post-18:11:30 mtime filter are new activity in that interval; the overlay/Front/FSM and unchanged lookup/selection leaves have the temporal qualifications stated above.

| File | Local mtime | Bytes | SHA-256 |
|---|---|---:|---|
| `port/level-world/retained_character_actor_v1.cpp` | 2026-10-08 18:15:29.8240515 | 23031 | `b88abef1cf3116bc321e98ed643fde226a914f76a8fb16968a3a09b12f1b0d60` |
| `port/level-world/canonical_gameobject_base_owner_v1.cpp` | 2026-10-08 18:15:29.8220509 | 7752 | `74d275672d4ff20af3fdd15eaadfb92c75638caf7d5fd9ffd3e3889c16f1439f` |
| `port/level-world/tests/retained_character_actor_v1.cpp` | 2026-10-08 18:21:14.3684381 | 4837 | `042a4b8f94a7d1d0f49120538753cd8189fa2fc0969c95669f7f3fa824f63761` |
| `port/level-world/tools/run_retained_character_actor_v1_host.py` | 2026-10-08 18:18:52.3647901 | 2425 | `185704e4c6f171100bcc2a7d7bbb37ea5c403f21269decb8caf05a537fab3783` |
| `port/engine-ui/overlays/source-facade-v1/swf_movie.cpp` | 2026-10-08 18:01:48.3333352 | 49430 | `0370235d606a9eaaba191cc7e4facdf64a779d4fd3ba2686cb23f4d7d5d06ce8` |
| `port/android-native/app/src/main/cpp/front_ui_session_v87.cpp` | 2026-10-08 17:28:33.5500124 | 172077 | `3df5518200fff40f4948821663627817411f3f5e5a1fae53362dde2735842638` |
| `port/android-native/app/src/main/cpp/source_campaign_character_fsm_v101.cpp` | 2026-10-08 17:31:33.2028563 | 145854 | `e4a9925f8f1673b83302165503ebe1ddfd4dc5dc6109015a0604110bbaad0391` |
| `port/level-world/reports/retained-character-actor-v1-host.json` | 2026-10-08 18:23:07.3268716 | 7150 | `43df298366ed51ff42eabd5144ee9a4ab1b21de8ea30a14b782f09bed9dd40ac` |
| `port/level-world/canonical_property_map_v1.cpp` | 2026-10-07 06:51:14.6038279 | 13476 | `d2f32d25e001815237b74e1203a5927d912c8f13e744e41db177c0c9e26d4a72` |
| `port/level-world/character_world_npc_state_owner_v1.cpp` | 2026-10-08 13:04:43.7273123 | 4331 | `58cf9343ba7ba546238b809d14beb2ef16d121c996a98faf996aaee65bb9b823` |
| `port/engine-ui/gameswf_source_facade_v1.cmake` | 2026-10-04 21:34:27.8621063 | 1689 | `e7608376d2f022c333580ee93edc531c583cc5cd4483979a44d0e550962c3946` |
| `port/engine-ui/CMakeLists.txt` | 2026-10-08 11:14:59.8652660 | 17309 | `92f5bd733e492cdd71f72b6d8f3e91bf565e9b453cc60d023ccf43cd8cc9f625` |
| `port/android-native/app/src/main/cpp/renderer_source_batching_v96.inc` | 2026-10-08 13:04:58.3093051 | 21214 | `c5fac3c846c4af98e6df1d259c3088aaec64bd0b71171fa93cc7e5f8ea43d576` |
| `port/android-native/tools/launch_guarded_emulator_v36.py` | 2026-10-08 18:28:21.2676395 | 10243 | `67f48b62b11619f33e485e002f88b789a2f1e9e3e96a9816529f4fbef91ff027` |
| `port/android-native/tests/test_menu_preview_selection_connector_live_v1.py` | 2026-10-08 18:29:13.9199374 | 3125 | `bb6e0801978be5ad58e5d400e0c2577604b4eab7e710521affeb2ab2cceecf27` |
| `port/android-native/tests/test_authored_loading_screen_assets_v1.py` | 2026-10-08 18:27:57.5880732 | 7397 | `13a3285bade3010c5fb60ca5f91b000a4a9124b3770d83959509110e344d82fa` |
| `port/android-native/tests/menu_preview_selection_connector_live_v1.cpp` | 2026-10-08 18:29:32.2134295 | 8178 | `63d8bf6732717ca94ac45895c568421767bd81f9dd32c5d9af50b7526d246a53` |

### Replacement receipt source coverage at the final snapshot

Every recorded source below matched current bytes at18:33:34; these are fingerprint comparisons only. No dependency binary, compiler command or executable was run by this audit.

| File | Recorded and current SHA-256 |
|---|---|
| `port/level-world/retained_character_actor_v1.cpp` | `b88abef1cf3116bc321e98ed643fde226a914f76a8fb16968a3a09b12f1b0d60` |
| `port/level-world/canonical_object_factory_v1.cpp` | `84eb8a2eb9aa8909f8d3f9918ec0fde83d92e434d69a4ce64978a8259bb08463` |
| `port/level-world/canonical_property_map_v1.cpp` | `d2f32d25e001815237b74e1203a5927d912c8f13e744e41db177c0c9e26d4a72` |
| `port/level-world/canonical_gameobject_base_owner_v1.cpp` | `74d275672d4ff20af3fdd15eaadfb92c75638caf7d5fd9ffd3e3889c16f1439f` |
| `port/level-world/character_kill_fields_v21.cpp` | `53b525fc680377ceb535f1c06ac671202bb9eee0190e09e9e5f6473b6154649f` |
| `port/level-world/character_set_position_v7.cpp` | `98d0c388457c5cf49c4a5eda90bc285dd83ca8ca9bb2c03e9cee7d7cff260d7f` |
| `port/level-world/character_world_npc_state_owner_v1.cpp` | `58cf9343ba7ba546238b809d14beb2ef16d121c996a98faf996aaee65bb9b823` |
| `port/level-world/character_world_npc_initialization_v1.cpp` | `d33f5d01d920799057375133c27138164e4cab4b698e851868f775db092a5d3c` |
| `port/level-world/character_state_owner_extensions.cpp` | `bf0ef776260ea12c7eac7e41dbcc9474ddf0b4c94e4f423e27ad869971588058` |
| `port/level-world/character_state_owner_behavior.cpp` | `36da18ca632a39ea032e1ff4fd70e152ba37cace03557a1a22045808cfc78ce6` |
| `port/level-world/character_state_owner_frame.cpp` | `d506ba5eac38e2e9991957e586365eccfbe9eeff439b07ed8f4b015fe9eca851` |
| `port/level-world/character_world_ai_queue_v1.cpp` | `6d30766081da4cc1515d1e4556116a905a24cb3fee941d3eac1f93bd3b36f140` |
| `port/level-world/character_idle_events.cpp` | `3cd0d864c93c62129c421bbfe7c1e4e14120330de66e3a1e7b6499d5c327c1a0` |
| `port/level-world/tests/retained_character_actor_v1.cpp` | `042a4b8f94a7d1d0f49120538753cd8189fa2fc0969c95669f7f3fa824f63761` |

### Final lane artifact fingerprints

These are the artifacts read by the reconciliation at18:33:34. “Changed” compares bytes to the initial18:11 snapshot; it does not imply all rows in that file were reexamined. Unchanged rows/files retain their earlier semantic scope.

| Lane | Rows | CSV SHA-256 | Markdown SHA-256 | Change from initial snapshot |
|---|---:|---|---|---|
| 01 | 1866 | `63f90e2b82515bbc7bc29ff618320eb48ac2b92fa6dff3424cce7d9a7aa7ab12` | `958859b3a90a956fada1edb3759c80ace4b2f79336bd9d4383f271f3423fe202` | CSV unchanged; Markdown unchanged |
| 02 | 1866 | `49156cd328bcb5f6fe2c6c08da0c2a978339b54c14eddd1961ad388c762b5856` | `593c14e4bb336426ae0ab9d96676360a36f5db3a1da0d0ac0c9ef8bba63d7d8d` | CSV changed; Markdown changed |
| 03 | 1866 | `da8ff448d79f7256d4b61791b0ab07599a02f22889d70ed6381115406cab8e1e` | `0e48ea80d920709ff3670973d185e6c09574dc0dc6ac996e655b7db024669046` | CSV changed; Markdown changed |
| 04 | 1866 | `dc2abcde52e2dc66ef9d0b2c7aabdfd0fc17b3facd7fe275a298e8e473cb4e48` | `fa23c7b820fc4c41f24ca5af4b3b86d8b8439f74aca3552e8985b95a9053ed18` | CSV changed; Markdown changed |
| 05 | 1866 | `612b39623e932a9be5f61b8481e2194debcb24cce2d3b85b19bcb47abee53d58` | `659d7f976df35fbc98d2f921ecf8752872cfc94f8921ff6ee54632da6d9fdfd0` | CSV changed; Markdown changed |
| 06 | 1867 | `d8f8e4052673016166859801872222cb62a51ba9745d68c00db17336c9535adc` | `177dbe06c8f811cd2af251b78289e9ed44aaa9fe5746a8c80297acf3d776d56b` | CSV changed; Markdown changed |
| 07 | 1866 | `7ddf1fbac26aac7f0bd6bdaf4ca1cb4b9981a9c89525f9466f823e2d26d8f310` | `5ff155e757cf45261fd4b5803ad1399c061e009219033e9aed1f1fcb048112f2` | CSV unchanged; Markdown unchanged |
| 08 | 1866 | `be92469b6ca649dfcaabb6a26bac9b4580070b117771836d55ae0959c30171c8` | `8f7b5739c18d65c74ee9d19972ed83292f9bc037f52c654be757201613a41099` | CSV changed; Markdown changed |
| 09 | 1866 | `078a6fcf705c100c9da4dd6aca0c7c7be773f26cbe83892c1bf083fd15f2bb55` | `c21c3ae3f40633a7a9d61169506c4a6606284a74581c7ccb2e647d863650277f` | CSV changed; Markdown changed |
| 10 | 1866 | `e09bf7c97db775bd2a4a4f84dbefd9440261531400c5030ff4775b019a887719` | `6d3a1432cc88810cc2730ba3ff8201afbf7703fd58d48bcf9865400cef24805f` | CSV unchanged; Markdown unchanged |
| 11 | 1866 | `e77ceb8362a4006e6d8b2120c8438862f4efe2692e6ed07373ce2050f426c4f2` | `157abad7c6cfa07a1d5809405d58f6a8238ae0d1237f212d63e25660834556af` | CSV unchanged; Markdown unchanged |
| 12 | 1867 | `13cbf559a0c786b319949bbb985dc5fe0ea5f9989a0cb95a4b1d6c11cc50b971` | `2372eb063044b970c233e62bf2a196a211c09c81e731df05d0f18d714d400a1c` | CSV changed; Markdown changed |
| 13 | 1866 | `524d5b8f948dd6508f3e2c9f0f0089e54d9358563f834d4f191bec02f2ac7109` | `44aaa95035ec8fca976dd1f2cc6ea7f8909413bbdbf1c6138799995e521f6183` | CSV changed; Markdown changed |
| 14 | 1866 | `92dfd2cb40f5839aba8b59da6731c95ed267c7b67ce2fae7c1f55074eec26eca` | `ea53e76506a4753be2483c7c4b6698a19a5c1bb8b38632376d3e9b9e322a829d` | CSV changed; Markdown changed |
| 15 | 1866 | `0cdbf3faea16c1360d42d7bf957a93a88f76072e5658cf051939fe0b21ff77ac` | `a18ab9b289c0458bf41539a4943e19d38ab67f8e637fe53cca2aa2d2c60c0ce8` | CSV unchanged; Markdown unchanged |
| 16 | 1866 | `fe7e2dff2f073a524cddbd7e098c7a9c63d284a57dceeb48636829d5da4144ce` | `38b009269fa229304e3b1cffdd2cd820ae8358eaee85c09f5f0c039cd0e03732` | CSV unchanged; Markdown unchanged |
| 17 | 1867 | `0bda7c0b60a0d306819c31fea37ca59173d71b302f2a721dc8ebc56c1ac6281d` | `843a6465c2e229ede5ec7958e20af5d1d42920c5105a46f76068dbca7764bb15` | CSV unchanged; Markdown unchanged |
| 18 | 1864 | `57443394c9f4a8b2df59bb8cdc5051e1e88471757972bbe0872c4d013195f494` | `f83f27624ad1d391d557cc2ac1a1251d3d846656ccada08afe47743cf0b6e44f` | CSV unchanged; Markdown unchanged |
| 19 | 1864 | `4b782263b56f3453b0036e214c94fd4e3b5b929905edd145044e09ed7eec829b` | `d0c55f9d7d1d0f7237faae14e2acd895a62b57cd8b66121106b91f06990250c9` | CSV unchanged; Markdown changed |
| 20 | 2360 | `0b3e0985d940abe39ba4cdcb3c0bc80fb0c9bd30a1ba984bec7acf990822bac3` | `c789fea64a7f28d6eeb86ebc6ed4fb87b9df012715555fdf11f38012c0cfeec2` | CSV unchanged; Markdown unchanged |
| 21 | 579 | `cc85d4b52f705a9abf92bb652037567dabe9846187087b47eda4164927c5d586` | `175490995e559aea3e2ee54ebecea5aebcad1103e58fccc1914cd1cb5f9759b3` | CSV changed; Markdown newly present |
| 22 | 16 | `1f0c7a4edf54ca00f0e331feb432c0b883e544b9da5cd8fdb3835465621901d3` | `0b701edeb3a74507d51678ede402c34148769f36ce9bc7aa475f065b523cf646` | CSV changed; Markdown changed |
| 23 | 11 | `b957fd2a1b512b2341b0de9ddfef68a882df3f2b34e3d7075374ebfd218e18ea` | `cfea42aecde76399cf3f1592b0d72acf0f27094acdc2ad63a5f1d0cfa26bad03` | CSV unchanged; Markdown unchanged |

**Disposition:** the requested post-pause citation/owner refreshes and selected-facade correction are reconciled for these pinned artifacts. Open semantic/runtime work is the work named in the current disposition table and the lane reports, not an unfinished hash-refresh checklist. No game source or other report was edited by this reconciliation. No build, test, host command or emulator was run by this read-only audit. Later writes require a new version-qualified review; this report stops at the stated source snapshot rather than following ongoing fixture/tool edits indefinitely.
