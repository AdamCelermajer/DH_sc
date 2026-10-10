# Lane 13 — libDungeonHunter2.so, records 22394–24259

Audit date: 2026-10-08. Source marker: 75a7c2fe3261403e841ffbeb46734e9e4e84e75c, with the pre-existing dirty working tree read as requested. Only this lane's Markdown/CSV reports were written. No builds, tests, emulator, or gameplay runs were performed.

## Coverage and classification limits

There are **1,866 rows**, exactly the assigned inclusive range, with **1,866 unique IDs**, zero omissions and zero duplicates. Every assigned export has decompile_status=success; zero import-only inventory records and zero native decompilation failures occur in this range. The CSV preserves every record, address, code/pseudocode hashes, explicit direct callers/callees, source candidate locations/hashes or a scoped unresolved reason, and not_run runtime status.

| Status | Rows |
|---|---:|
| matched | 30 |
| partial | 29 |
| unclear | 1623 |
| clone | 156 |
| import/thunk | 28 |
| missing / disconnected / failed-body / out-of-scope | 0 |
| Total | 1,866 |

**Record enumeration is complete; semantic parity is not established for the whole lane.** The 1623 unclear records remain open. Complete native texts were read and their calls/control markers inventoried; a production-source definition/body parser supplied candidates for 1,600 records before manual alternate-path refinement. These mechanical comparisons are not deep branch-by-branch parity proof. The substantive manual comparisons appear below and in the CSV's MANUAL BEHAVIOR COMPARISON evidence.

Partitions: 22394–22412 (19 allocator/compression/mesh records), 22413–23297 (885 FreeType/runtime records), and 23298–24259 (962 GameSWF records). Eighteen ARM/Thumb veneers occur inside the FreeType partition; ten GameSWF this-adjustment thunks occur later. Compiler global initialization and compiler clone suffixes are distinguished. A clone suffix alone never yields clone.

Source candidates exclude report/reference/test/build/platform-build/integration/staged/prospective/verify-tree snapshots. FreeType uses freetype-2.3.7-hud source/include files. Overloads and inactive preprocessor branches remain candidates unless manually resolved. Actual selection follows [CMakeLists.txt](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/CMakeLists.txt:4), [freetype237-hud.cmake](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/freetype237-hud.cmake:2), and the GameSWF overlay CMake files: vendor root/character/object/player/dlist/text bodies are replaced. No semantic absence claim is based on a parser/text-search miss.

## Findings

### F13-01 — cold first display skips original initialization advance

**Partial; high static confidence; runtime not_run.** IDA [24106 / 0x775574](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0077/00775574.c) checks visibility and invokes advance(1.0,false) if LOAD has not fired, before beginning display, displaying the movie, flushing text, and ending display. Selected [gameswf_root.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/frame-v1/gameswf_root.cpp:599) starts rendering without that cold-root advance.

The selected facade [swf_movie.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/source-facade-v1/swf_movie.cpp:343) calls root display directly. Selected [gameswf_player.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/player-lifetime-v1/gameswf_player.cpp:772) load_file returns an instance without advancing it. The analogous native virtual call in [24022 / 0x7736f4](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0077/007736f4.c) at +76 is get_movie_version, confirmed by root vtable 0x990298, address point 0x9902a0, entry 0x9902ec → 0x773d48. It is not first-frame initialization.

Unverified path: load → visible display before explicit update. First-frame actions and LOAD can be skipped at that point. The explicit advance path does initialize through the frame adapter. No runtime trace established that a particular campaign screen takes this cold path.

### F13-02 — direct viewport mutation bypasses original bounds and AS publication

**Partial; high static confidence; runtime not_run.** IDA [24108 / 0x775d38](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0077/00775d38.c) returns for identical xywh; changes are stored and forwarded to [24107 / 0x7755f4](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0077/007755f4.c) with mode 0. That body computes logical bounds/pixel scale and publishes a live player's Viewport AS object containing xMin/yMin/xMax/yMax.

Selected [gameswf_root.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/frame-v1/gameswf_root.cpp:417) always writes xywh and computes pixel scale without bounds/publication. Selected facade display/display_clip call it directly. The replacement helper [viewport.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/viewport.cpp:62) reproduces the changed-only coordinator, and [swf_viewport_connection.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_viewport_connection.cpp:31) binds it to the retained graph. Selected facade set_source_bounds at current line 399 uses that adapter.

Unverified path: direct display viewport change followed by AS/input use of logical bounds. The adapter behavior is statically represented; the direct setter bypasses it.

Related [24059 / 0x774128](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0077/00774128.c) only assigns mouse button/x/y. Selected root mouse setter additionally emits MOUSE_MOVE on a change, whereas connected [swf_viewport_connection.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_viewport_connection.cpp:103) reproduces assignment-only behavior. Direct root API and connected cursor route have different behavior.

### F13-03 — native standard-member and transform getter closure differs

**Partial; runtime not_run.** The table in [24019 / 0x772030](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0077/00772030.c) includes htmlText, textHeight, Stage, transform, matrix, concatenatedMatrix, colorTransform, and concatenatedColorTransform, absent from selected [gameswf_player.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/player-lifetime-v1/gameswf_player.cpp:326) standard property map.

This does not make all those properties missing: selected [gameswf_text.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/edit-text-v1/gameswf_text.cpp:892) directly handles htmlText/textHeight. Native sprite getter 24331 / 0x77ffbc (outside this lane), case 41, initializes and returns the same sprite transform object. Selected [gameswf_sprite.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/frame-v1/gameswf_sprite.cpp:726) has no corresponding transform case. The project includes as_transform, but that constructor's presence does not establish sprite.transform lookup or native identity/lifecycle. Asset consumers and Stage/transform closure remain unresolved.

### F13-04 — direct display-list advancement pins/runs different children

**Partial; runtime not_run.** IDA [23451 / 0x755908](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0075/00755908.c) snapshots strong children in reverse into a player work stack, processes forward only need9d children, rereads the need byte after each callback, and drops each pin immediately. Native construct [23452 / 0x755a10](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0075/00755a10.c) uses corresponding per-child pin release.

Selected [gameswf_dlist.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/edit-text-v1/gameswf_dlist.cpp:456) copies all display-object rows, advances every non-null child, and retains the whole copy until return. [swf_frame_connection.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_frame_connection.cpp:32) children_construct/children_advance reproduce native scheduling on the connected sprite-frame route. Direct API parity and callback-triggered removal/lifetime behavior remain unverified.

### F13-05 — callback registration is implemented with different scope/owner

**Partial; runtime not_run.** IDA [24008 / 0x76ef5c](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0076/0076ef5c.c), called by MenuManager::Init 6736 / 0x42f304, installs C functions into process builtin method map index 0. Selected facade [swf_movie.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/source-facade-v1/swf_movie.cpp:569) delegates to [swf_actionscript_connection.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_actionscript_connection.cpp:102), which installs owned as_function objects into the exact player's global, verifies stored identity, and requires active movie scope.

This is real callback delivery. Process-wide visibility, existing builtin shadowing, repeated registration and shared-player behavior differ and need acceptance; the old symbol's absence cannot classify registration as missing.

### F13-06 — get_character has different successful results

**Partial; runtime not_run.** IDA [24259 / 0x77de28](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0077/0077de28.c) unconditionally returns null. Current [gameswf_sprite.h](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/vendor/gameswf1714/gameswf/gameswf_sprite.h:99) checks display-list size and returns an indexed child. Root get_character forwards to it. This callable API differs observably; actual game use is unverified.

## Subsystem evidence and remaining work

**FreeType:** real source-built 2.3.7-hud is used by [freetype_font.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/freetype_font.cpp:16) and [hud_freetype_font_v2.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/hud_freetype_font_v2.cpp:17) for memory-face/raster calls with real face/library teardown. Validator, stream, glyph-loader, outline, version/debug-hook, initialization and LZW bodies were manually compared with per-row logical field/callback/error notes. Matched applies to the stated valid-owner behavior, not ARM32 struct layout or all numeric domains.

Native fixed math uses split 32-bit arithmetic. Source [ftcalc.c](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/vendor/freetype-2.3.7-hud/src/base/ftcalc.c:337) includes that algorithm under !FT_LONG64. ftconfig.h:282 and ftoption.h:115 select that branch under ordinary C compilation, but native64 FT_Long width, extreme values and overflow behavior remain unresolved; arithmetic and dependent matrix rows are partial. Original injected allocator globals in 22646 and allocator rows 22648–22651 differ from [ftsystem.c](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/vendor/freetype-2.3.7-hud/src/base/ftsystem.c:69) default allocator calls. The original hook setter has no direct exported caller; indirect/API reachability is unproved.

The larger TrueType interpreter, CFF/Type1/Type42/CID/PFR parsers, SFNT/cmap/bitmap loaders, autotuning/hinting, raster/smooth, BDF/PCF/WinFNT and gzip/LZW rollback paths retain individual unclear CSV rows. Matching release names/source/calls are not exhaustive branch parity.

**Mesh connectivity:** [22412 / 0x703acc](C:/Users/adamc/Desktop/workspace/DH_sc/.local-inputs/ida-apk-export-2026-10-07/libraries/libDungeonHunter2.so/pseudocode/0070/00703acc.c) is called by CShadowVolumeSceneNode::createSilhouetteVolume 22385 (outside lane). It maps index/position streams, optionally welds exact coincident positions, creates ordered undirected edges, stores at most two adjacent faces, and frees/unmaps temporary data. Save writes counts, 16-byte edges, 6-byte face triples and sentinel 0xc0ffe808. Load's assembly has anomalous zero-base loads: MOV R5,#0 at 0x703868, LDR R3,[R5,#0x14] at 0x70389c, and another zero-base load at 0x703ab0. Bad-looking pseudocode cannot be transplanted as recovered source. No direct load/save caller or equivalent production renderer/floor/mesh silhouette owner was established. All relevant records/helpers retain explicit unresolved source/reachability dispositions.

**GameSWF filters/3D attachment:** 23473–23508 implement fork framebuffer texture-cache/filter behavior and glyph caller 25747 → 23494. Current [edit_text_display_v1.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/edit_text_display_v1.cpp:22) text-filter fallback and [swf_text_font_platform_v1.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_text_font_platform_v1.hpp:6) renderer/cache providers do not establish the original capture/region/raster/cache owner graph or non-default binding.

24111–24136 implement exact scene registration, typed UV collection, collision ray/triangle intersection and attached-movie mouse conversion. Attach 23412 constructs 24135; init_corners 24134 → collect_uvs 24133 → typed collect 24126–24132. Current [swf_input_geometry.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_input_geometry.cpp:1) and viewport/input connections implement screen-space geometry. Equivalent 3D attachment or source consumer remains unresolved, not declared missing. Empty native render/glow bodies retain original identity classifications where proven.

**Strings/sound/loading/ownership:** UTF-8 23314–23315 map to differently namespaced [utf8.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/vendor/gameswf1714/base/utf8.cpp:14) with corresponding invalid/overlong masks, pointer advancement and 1–6-byte encode thresholds. Custom format_utf_text 23316 remains unclear. StartSound parsing/dispatch gates and sound table were compared against [gameswf_sound.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/vendor/gameswf1714/gameswf/gameswf_sound.cpp:157). Real non-null SWF sound-handler binding was not established; native handler ABI/player transport is partial. DefineSound/ADPCM remains unclear. Complex movie definition/tag imports/cache serialization, fork player_context, generic/bitmap characters and ref/GC ownership retain individual open rows.

The connected AVM1 root frame path 24105 → selected root wrapper → swf_frame_root_advance → dh2_ui_swf_root_frame preserves listeners-before-remainder, first flash_vars/init/construct, one/catch-up delta selection, loaded-before-LOAD, GC mark/listeners/movie/clear, 2-second reset and fmodf. Exact source paths are [swf_frame_connection.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_frame_connection.cpp:145) and [swf_frame_schedule.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_frame_schedule.cpp:47). Receiver binding/failures and invalid/nonfinite/catch-up progress guards extend native behavior; reentrant/full-domain parity remains partial.

## Cross-lane edges

The CSV includes all resolved direct incoming/outgoing function references for each assigned row, with calls, tail transfers and address-taking distinguished by xref type. Prominent paths and their dispositions:

| Original edge | Disposition |
|---|---|
| 21875/21876 JPEG allocators → 22394 → 412 GlitchAlloc | allocator replacement unclear |
| 22385 shadow-volume silhouette → 22412 → 22410 | mesh/shadow owner closure unclear |
| 6736 MenuManager Init → 24008 | real per-player replacement; scope/visibility partial |
| 25111 RenderFX Load → 24010/24022 | native load traced; cold display/context partial |
| 25131 RenderFX Update → 24105 | connected adapter; reentry/invalid domains partial |
| 25077 RenderFX Render → 24106 | cold-display initialization differs |
| 25080/25081 RenderFX bounds/viewport → 24107/24108 | retained adapter versus direct setter distinction |
| 7099 NativeGetCursorState / 25130 RenderFX cursor → 24057/24059 | mapping connected; direct mouse setter differs |
| 6710 FlushTextCallback → 24096 | queue flush matches stated logical behavior |
| 25747 glyph cache → 23494 filter apply | raster/cache owner unresolved |
| 23412 → 24135; 24134 → 24133 → 24126–24132 | 3D attachment/UV/collision graph unresolved |
| 23602 loader registration → 24246/24251 | StartSound parsing matched; DefineSound/ADPCM/handler open |

## Clone, thunk and hash discipline

45 duplicate code-hash groups have 196 extra equal-byte records. Only **156 rows** retain clone: code hash, normalized complete body and resolved outgoing function/data targets agree. Equal bytes with distinct PC-relative targets are classified independently. Identity does not transfer source parity between symbols; each clone row cites its representative. Record 24259 was separately compared and is partial despite original return-zero body identity.

Eighteen STLport veneers were inspected in assembly: LDR/ADD/BX resolves a named Thumb runtime target; ten GameSWF thunks adjust this by vtable offset -0x18/-0xC or fixed -0x20 then transfer to the named method. They are import/thunk, not import-only failures or parity passes. Indirect virtual/interface calls and unresolved non-function/data endpoints remain explicit; live ownership closure is unproved.

The CSV records SHA-256 for all cited source definitions. Drift checking from the final source scan/refinement through report close found no changed cited C/C++ files. Earliest exploratory un-hashed reads cannot establish a longer immutable interval. Original-pass fingerprints follow; post-pause rechecks below supersede the two text-platform fingerprints and the selected movie-facade fingerprint, and add their bounded dependency fingerprints. Other original-pass source hashes/anchors are historical unless separately rechecked.

| Source | SHA-256 |
|---|---|
| [port/engine-ui/CMakeLists.txt](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/CMakeLists.txt:4) | 92f5bd733e492cdd71f72b6d8f3e91bf565e9b453cc60d023ccf43cd8cc9f625 |
| [port/engine-ui/freetype237-hud.cmake](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/freetype237-hud.cmake:2) | ac8afc6d3cd246d9159e0ab67b5b2340f3b4c4afc84aed7497078a4c707d5804 |
| [port/engine-ui/gameswf_sources.cmake](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/gameswf_sources.cmake:1) | 39eb92ae4f89a9ce64401dcfa10c72a86234858f491f57fb52af13a3da4f9dcc |
| [port/engine-ui/gameswf_frame_overlay_v1.cmake](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/gameswf_frame_overlay_v1.cmake:1) | 296ec22372bb43e1deef407817673aea835495a310db3081b0c9807fd73fd380 |
| [port/engine-ui/gameswf_edit_text_overlay_v1.cmake](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/gameswf_edit_text_overlay_v1.cmake:1) | 406284f2020472b60cbc4e2fe9eaffc3725cb5b2f4b95bd61e9321bd3cc1df23 |
| [port/engine-ui/gameswf_source_facade_v1.cmake](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/gameswf_source_facade_v1.cmake:1) | e7608376d2f022c333580ee93edc531c583cc5cd4483979a44d0e550962c3946 |
| [port/engine-ui/overlays/frame-v1/gameswf_root.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/frame-v1/gameswf_root.cpp:599) | adc7ee0426f469a87d778417d0d4e0f44e0cb2007754b5c76b0116653535e71c |
| [port/engine-ui/overlays/frame-v1/gameswf_sprite.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/frame-v1/gameswf_sprite.cpp:726) | 5ba3c07c2f106a685ebf43294f17036e885ed96b3cd7f4956ba838c59104250a |
| [port/engine-ui/overlays/edit-text-v1/gameswf_dlist.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/edit-text-v1/gameswf_dlist.cpp:456) | 77be78637f6ba88e4284812fa51e171ceaedc915dd6532743035bb5d119ce0af |
| [port/engine-ui/overlays/edit-text-v1/gameswf_text.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/edit-text-v1/gameswf_text.cpp:892) | 5c0c14bfaf8504e36284316294f53d2badf444a30d804fbc13951620f2fc55f0 |
| [port/engine-ui/overlays/player-lifetime-v1/gameswf_player.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/player-lifetime-v1/gameswf_player.cpp:326) | a7236e70f0f5dfe97059b94e9db7906591c7b63272f052bdc552f3368fdf258d |
| [port/engine-ui/overlays/source-facade-v1/swf_movie.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/source-facade-v1/swf_movie.cpp:343) | 0370235d606a9eaaba191cc7e4facdf64a779d4fd3ba2686cb23f4d7d5d06ce8 |
| [port/engine-ui/swf_frame_connection.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_frame_connection.cpp:145) | 9f3ddea3d3321f0dfc3718d3f1e670558c84a6bb99875adb2f6f9556c7664837 |
| [port/engine-ui/swf_frame_schedule.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_frame_schedule.cpp:47) | 2401099b82e8ba5e5f093d5a33690cbe3258363081b74730dce41c13f3d6bc59 |
| [port/engine-ui/swf_viewport_connection.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_viewport_connection.cpp:31) | 7f5278e000a91a79088be113a3b6f79354a22bc2845be2cbeab922ad17a2b813 |
| [port/engine-ui/viewport.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/viewport.cpp:42) | ba2ccdba830d992463f1f61bc02501fb5d7fff65b2c4b246ca5df490e3758ba9 |
| [port/engine-ui/swf_actionscript_connection.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_actionscript_connection.cpp:102) | c3e873f984d9033fd9950a8963738110b6cd07e0cc6db34cbc6c39aeea70c9ee |
| [port/engine-ui/swf_input_geometry.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_input_geometry.cpp:1) | 83cedb6d4f0f13f27c58e8de8e22e84388f850b1113c42eed985b5583770cc81 |
| [port/engine-ui/swf_text_font_platform_v1.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_text_font_platform_v1.hpp:6) | 611c1327217e41f618b78b7aed57b7603109fbe751a4a9f0e897a9a89c8637f0 |
| [port/engine-ui/swf_text_font_platform_v1.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_text_font_platform_v1.cpp:1) | 30807835dafb798999781a0b7088bba6619fbd3e17f034292ddcd4066f5ebad8 |
| [port/engine-ui/edit_text_display_v1.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/edit_text_display_v1.cpp:22) | 54910b78bc37334ba18c1c27f236c1b2d5653ba9bd31bbd055199c3a4c994f70 |
| [port/engine-ui/freetype_font.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/freetype_font.cpp:16) | efbf41c933e87bc915b0ea5f73d7b261d6ca2273b5f3bdbf568d93618c844aeb |
| [port/engine-ui/hud_freetype_font_v2.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/hud_freetype_font_v2.cpp:17) | d16accfab2c1b96ca5fbf8f3cb04dfff5737937dec02f21d05463a729f64fd3d |
| [port/engine-ui/vendor/freetype-2.3.7-hud/src/base/ftcalc.c](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/vendor/freetype-2.3.7-hud/src/base/ftcalc.c:337) | 67ad7fa43861b96a281bd66edd4ee698c0fc64bc48446c8d40e026555ee9876b |
| [port/engine-ui/vendor/freetype-2.3.7-hud/src/base/ftsystem.c](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/vendor/freetype-2.3.7-hud/src/base/ftsystem.c:69) | 13200847a1f67005b7af0f4bd96a57f1fdc124917d642edd3e41d3efcc11cca7 |
| [port/engine-ui/vendor/freetype-2.3.7-hud/include/freetype/config/ftconfig.h](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/vendor/freetype-2.3.7-hud/include/freetype/config/ftconfig.h:282) | 47194ddd400b6d397c8b3276f612e31e3261c6ee575c5125ce2dc5af924404fc |
| [port/engine-ui/vendor/freetype-2.3.7-hud/include/freetype/config/ftoption.h](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/vendor/freetype-2.3.7-hud/include/freetype/config/ftoption.h:115) | 81ba2615c809687c55628c889b880399995964b10f1dbcf4dada26fd727bfc7a |
| [port/engine-ui/vendor/gameswf1714/base/utf8.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/vendor/gameswf1714/base/utf8.cpp:14) | 4765317b6a9f6859152234856ae43b1fe3ce419efe35c1b13a0c63c93bf3905d |
| [port/engine-ui/vendor/gameswf1714/gameswf/gameswf_sound.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/vendor/gameswf1714/gameswf/gameswf_sound.cpp:157) | ea1bd5d8530c514567a933499f6fd2604a2d4c2ccf5ea10be35a41ddbf384848 |
| [port/engine-ui/vendor/gameswf1714/gameswf/gameswf_sprite.h](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/vendor/gameswf1714/gameswf/gameswf_sprite.h:99) | 9dcd137626d10bf56c202f2a4c9efa59d52d01ddc5186cae480ab5e3230c8053 |

## Acceptance limit

All 1,866 records have a disposition and **integrated_runtime_acceptance=not_run**. No source was edited; no acceptance is inferred from unrelated historic fixtures. There are 1623 unclear and 29 partial rows. Complex alternate implementation/asset paths, virtual dispatch, source ownership/rollback and width edges remain open. This lane does not satisfy a claim that full semantic/runtime completion passed.

## Post-pause bounded recheck — 2026-10-08

Scope: exactly **35 existing rows**, **23473–23475 and 23477–23508**. Record 23476 (original empty glow-body identity clone) was not in the source-impact set and remains untouched. The 35 complete IDA pseudocode files were reread; filter/cache geometry, raster helpers, collection/capture, hash/container state and teardown were compared boundedly with the current text platform, changed headers, image release and reset callers. No new game-source edits, build, test or emulator work was performed.

**All 35 rows remain unclear; all remain not_run.** This refresh replaces the earlier cpp hash 1faa3f2ed49d7493f30557acccd76be895dd34bef8c9703ea4817bf8c5897cc1 and header hash 9c3c1981e71a0a15da3541281513f44fb482502abc20cc1e329b075e81e9dd31 with current cpp **30807835dafb798999781a0b7088bba6619fbd3e17f034292ddcd4066f5ebad8** and header **611c1327217e41f618b78b7aed57b7603109fbe751a4a9f0e897a9a89c8637f0**. Source context is not an exact original filter-owner mapping. Other 1,831 CSV rows and comparison counts are unchanged.

### What the changed source now establishes

[port/engine-ui/swf_text_font_platform_v1.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_text_font_platform_v1.cpp:217) rejects a populated captured-pixel registry without an image releaser, then calls the selected releaser once per captured alpha texture. It invokes [port/engine-ui/text_render_owner_v2.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/text_render_owner_v2.cpp:126) clear_fonts, whose new optional backend hook executes before face/image/clone maps clear. The GFNT implementation at [port/engine-ui/gfnt_text_backend_v1.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/gfnt_text_backend_v1.cpp:80) clears descriptor-face and URI-resource indexes. After every step succeeds, the platform clears device/font projections, font pins/clones, captured pixels/core bitmaps, glyph/image identity maps and last-upload/failure state.

The changed contracts are explicit: SwfServices.release_image_v119 in [port/engine-ui/swf_movie.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_movie.hpp:72), TextFontBackendsV2.clear_fonts in [port/engine-ui/text_render_owner_v2.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/text_render_owner_v2.hpp:10), platform reset in [port/engine-ui/swf_text_font_platform_v1.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_text_font_platform_v1.hpp:53), and GFNT resource/cache ownership in [port/engine-ui/gfnt_text_backend_v1.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/gfnt_text_backend_v1.hpp:27).

The actual application movie lease supplies the releaser at [port/android-native/app/src/main/cpp/original_ui_movie_services_v1.inc](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/original_ui_movie_services_v1.inc:20) and line 44, tied to that lease's retained OriginalUiSession resource owner. [port/android-native/app/src/main/cpp/original_ui_session.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/original_ui_session.cpp:1323) creates the lease and platform. [port/android-native/app/src/main/cpp/front_ui_session_v87.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/front_ui_session_v87.cpp:1290) wires front/shared platforms to the same front GPU. [port/android-native/app/src/main/cpp/swf_gpu.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/swf_gpu.cpp:378) removes only a non-white positive identity with matching dimensions, rejects removal during an actual display frame, checks GL ownership at a barrier, and releases a texture only against its recorded/current context generation.

The selected [port/engine-ui/overlays/source-facade-v1/swf_movie.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/source-facade-v1/swf_movie.cpp:369) reset holds the loaded movie owner, permits synchronous same-owner Scope reentry, blanks each currently traversed root edit field, then looks up the exact player's platform. [port/android-native/app/src/main/cpp/native_menu_process_bootstrap_v104.inc](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/native_menu_process_bootstrap_v104.inc:113) binds a weak same-menu callback and validates populated process slot 0/2/3 movie/receipt identity before reset; slot 1 is expressly level-owned. The separate [port/android-native/app/src/main/cpp/native_menu_update_prefix_v62.inc](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/native_menu_update_prefix_v62.inc:42) level/prefix loop can reset all populated slots including 1. The weak owner is backed by [port/android-native/app/src/main/cpp/native_character_menu_v4.inc](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/native_character_menu_v4.inc:5) enable_shared_from_this. This is observed source closure, not integrated Language/reset acceptance.

### Original behavior still not established by that closure

| Records | Native obligations retained as unclear |
|---|---|
| 23473–23475, 23502 | Same atlas bitmap, 16-cell bounds, signed rounding/minimum16, character-key region reuse/subdivision, RGBA alpha/stride blend |
| 23477–23482, 23484, 23496, 23498–23501 | Flat hash sentinel/chaining state, same character/key/region identities, borrowed array storage, cache-entry dirty fields and allocation/free ownership |
| 23479, 23485–23486 | Process effect identity initializer and one process filter-engine singleton owning 256x256 RGBA cache |
| 23483, 23487–23495, 23497 | Native framebuffer channel/alpha conversion, padding/trig offsets, Gaussian convolution/channel paths, raster swaps, recursive filtered-character collection and cached world-transform draw |
| 23503–23504, 23506–23507 | Effect/container destruction, mapped bitmap unlock/ref release, cache base teardown and process singleton retirement |
| 23505 | Native stream parser and failed-read prefix; current text_filter parser exists but image-reset work does not prove this path |
| 23508 | PreRender→collect/cache→allocation-failure reset/reallocation→temporary character transform/parent→capture/framebuffer read→locked atlas row copy→dirty clear |

Native dependency **24623 / 0x793ed8 texture_cache::reset**, reached by 23508 / 0x75918c on allocation failure, clears the key map, increments its 64-bit generation, clears mapped raster bytes if available, and rebuilds a full-size 16-cell region/free list while **keeping the same bitmap**. Region allocation is 24628 / 0x794668. Native **25099 / 0x7aac54 RenderFX::ClearFonts(player_context*)** traverses all players registered in the context, blanks their edit fields, clears face/bitmap-font maps and calls reset on existing provider atlases. Native 23504 destructor separately unlocks/drops the bitmap and frees key/region/free-list storage. Source per-image retirement/per-player map clear does not itself prove those native reset-versus-destruction distinctions or shared context/player identity.

### Open reset and ownership questions

- **Partial release and retry:** platform release calls precede backend/local-map clearing. If one image removal succeeds and a later removal or backend clear fails, earlier GPU identities have been removed while pixels/core registries remain populated. GPU removal rejects an already absent identity. The code does not record an erased-image prefix or rollback here; retry/draw behavior remains open. This is a static failure-path question, not a runtime observation or an asserted original-parity defect.
- **Retained borrowers:** TextRenderOwnerV2::Image pins platform/texture_owner/face, and core bitmaps retain source image pins. Clearing maps does not revoke external shared image owners or glyph records. Current-root blanking removes the visited fields' records, but removed/shared/imported characters, active callback pins, queued root text and reentrant bound-variable setters were not exhaustively closed. Platform reset does not clear its buffered weak-field vector, and the root's queued text array is a separate owner.
- **Scope and caller completeness:** process slots 0/2/3 and level-owned slot 1 have separate reset callers. Complete original player_context sharing, every live movie/borrower, missing-platform handling, Language callback ordering and caller failure propagation still need closure. Selected SwfMovie reset skips platform clear when for_player returns null, then returns owner->finish; this branch is not proved equivalent to an original absent-provider case.
- **Reset during dispatch/display and teardown:** same-owner movie reentry is allowed, while GPU removal rejects an active frame. A GL/ledger failure can arise after prefix mutation; context-generation checks avoid deleting recycled names but do not establish native cache-generation/region-pointer equivalence. Successful reset followed by unload, pending buffered draw or image repopulation was not executed.
- **Full filter engine:** the source text fallback/alpha-glyph registries do not establish native full-character framebuffer cache, process filter singleton or all non-default render_cache/filter_engine providers. Actual providers reread at OriginalUiSession/front setup enable grid-fit only and return a required-cache error for cache commands.

### Bounded recheck fingerprints

These files were read for the stated reset/image/owner path. Source bytes were checked before report mutation and again at close; unchanged hashes authenticate this bounded read only.

| Source | SHA-256 |
|---|---|
| [port/engine-ui/swf_text_font_platform_v1.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_text_font_platform_v1.cpp:217) | 30807835dafb798999781a0b7088bba6619fbd3e17f034292ddcd4066f5ebad8 |
| [port/engine-ui/swf_text_font_platform_v1.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_text_font_platform_v1.hpp:53) | 611c1327217e41f618b78b7aed57b7603109fbe751a4a9f0e897a9a89c8637f0 |
| [port/engine-ui/swf_movie.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_movie.hpp:72) | 5a1d3684ebb49e610118e5aba42c432e475cc6b9fe3b13888fdd49d3a3513c5a |
| [port/engine-ui/text_render_owner_v2.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/text_render_owner_v2.cpp:126) | 6ee8e338fafcc60b94fee38d3fcd39d445847bda6de4451a51bca383da32e140 |
| [port/engine-ui/text_render_owner_v2.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/text_render_owner_v2.hpp:10) | 4e72399c7fe8fee28f60216060a3ef6549b0b85cc41a04945caca93fcfb3db8a |
| [port/engine-ui/gfnt_text_backend_v1.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/gfnt_text_backend_v1.cpp:80) | 87da563d2b7a36d7afff3461e973b20059b0250cc369f4cbf99647ae381e3e9a |
| [port/engine-ui/gfnt_text_backend_v1.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/gfnt_text_backend_v1.hpp:27) | 4e9425ce9e4133c8f7c4a3df9d023692ad25b4232b08438f296ee4b164e6034e |
| [port/engine-ui/overlays/source-facade-v1/swf_movie.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/source-facade-v1/swf_movie.cpp:369) | 0370235d606a9eaaba191cc7e4facdf64a779d4fd3ba2686cb23f4d7d5d06ce8 |
| [port/engine-ui/swf_edit_text_connection_v1.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_edit_text_connection_v1.cpp:63) | a3f6b534c0cffa163407351c93c3250122060f2e7d65360e866f5ba2972253c5 |
| [port/engine-ui/edit_text_display_v1.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/edit_text_display_v1.cpp:30) | 54910b78bc37334ba18c1c27f236c1b2d5653ba9bd31bbd055199c3a4c994f70 |
| [port/android-native/app/src/main/cpp/original_ui_movie_services_v1.inc](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/original_ui_movie_services_v1.inc:20) | d6fa672f4b1b8de09dc8df6519bb8e3c5bf170cea82e0ca4eb47698b3fb2fa2f |
| [port/android-native/app/src/main/cpp/native_menu_process_bootstrap_v104.inc](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/native_menu_process_bootstrap_v104.inc:113) | 21bf7754bf4b4d99f09ee9436227e87d2dea62dbfd7b36a6312129b4666c7a1d |
| [port/android-native/app/src/main/cpp/native_menu_update_prefix_v62.inc](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/native_menu_update_prefix_v62.inc:42) | 55a921153c713f4ce640a1257307ad42fe240aea2adc7789a543a64fb70f1eed |
| [port/android-native/app/src/main/cpp/native_character_menu_v4.inc](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/native_character_menu_v4.inc:5) | d6d00fb8ecce412ff73db91f6b0bd42f5d0b7a9cb61c8177c682cbfe64eee557 |
| [port/android-native/app/src/main/cpp/swf_gpu.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/swf_gpu.cpp:378) | 140ef2708958cb5adb5a4f0d7e484c3fea742a9bcdb34c58078c603bd52013cd |
| [port/android-native/app/src/main/cpp/swf_gpu.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/swf_gpu.hpp:33) | ee1afe07ce0b08ab129accbe3013e34b1e93e69857ffb30faeedcb2532393dce |
| [port/android-native/app/src/main/cpp/front_ui_session_v87.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/front_ui_session_v87.cpp:1290) | 3df5518200fff40f4948821663627817411f3f5e5a1fae53362dde2735842638 |
| [port/android-native/app/src/main/cpp/front_ui_session_v87.hpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/front_ui_session_v87.hpp:50) | 11623552099bc57c48518dd7203cde649ff4b960a1b1b487cd9bb15e1fd93f03 |
| [port/android-native/app/src/main/cpp/original_ui_session.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/android-native/app/src/main/cpp/original_ui_session.cpp:1323) | d49dacbfd28ad4246604199608d4c129d25b8a34838a53c863e8bd4c7f61ca18 |
| [port/engine-ui/edit_text_field_v1.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/edit_text_field_v1.cpp:30) | 9cc385100bf673491aade151e2b35e99e428022d824c37be8c7f34403d831f9a |
| [port/engine-ui/text_filter_v1.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/text_filter_v1.cpp:21) | 6ffb1fb2a75abe79b75c6b9d5ca252b2568819a5be2323e6f3c829861154a332 |
| [port/engine-ui/overlays/edit-text-v1/gameswf_text.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/edit-text-v1/gameswf_text.cpp:30) | 5c0c14bfaf8504e36284316294f53d2badf444a30d804fbc13951620f2fc55f0 |
| [port/engine-ui/overlays/frame-v1/gameswf_root.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/frame-v1/gameswf_root.cpp:587) | adc7ee0426f469a87d778417d0d4e0f44e0cb2007754b5c76b0116653535e71c |

The bounded recheck changes no comparison status and no runtime acceptance. Exact lane coverage remains 1,866 unique IDs; all 35 affected rows remain explicitly unresolved.

## Additional selected-facade caller recheck — 2026-10-08

Exact scope: **six CSV rows 24008, 24057, 24105, 24106, 24107 and 24108**. Their selected movie-facade citation used the previous 6ba7aa327a5986c4a1266a4359ae3ad56317f5fbc114f9db0956aef888d8655d fingerprint. It is now **0370235d606a9eaaba191cc7e4facdf64a779d4fd3ba2686cb23f4d7d5d06ce8**, matching the file observed before the initial impact-sweep cutoff. These are additional caller/owner checks, separate from the 35 filter/reset rows. Only these six CSV rows changed in this refresh; the other 1,860 remain untouched by this refresh.

Current anchors and bounded conclusions:

| Record | Selected facade anchor | Existing disposition retained |
|---|---|---|
| 24008 native registration | [source_register_native_actions_v119](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/source-facade-v1/swf_movie.cpp:569) | partial: real player-global owned function registration remains; process builtin visibility/repeated-player semantics remain open |
| 24057 screen_to_logical | [screen_to_logical](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/source-facade-v1/swf_movie.cpp:414) | matched for stated valid retained-owner mapping: same viewport connection and failure gate |
| 24105 root advance | [advance](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/source-facade-v1/swf_movie.cpp:338) | partial: explicit root/frame adapter path remains; invalid/nonfinite/reentry/receiver limits remain open |
| 24106 root display | [display](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/source-facade-v1/swf_movie.cpp:343) | partial: main-root cold display still has no initialization advance |
| 24107 set_display_bounds | [set_source_bounds](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/source-facade-v1/swf_movie.cpp:399) | matched for stated valid retained-owner bounds/publication coordinator |
| 24108 set_display_viewport | [display](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/source-facade-v1/swf_movie.cpp:343) and line345 display_clip | partial: direct root setter still bypasses separate bounds/Viewport publication coordinator |

Each entry method keeps its local shared impl owner through the current Scope at [Impl::Scope](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/source-facade-v1/swf_movie.cpp:112) which stores the implementation pointer, installs its renderer/glyph handlers and restores prior active handlers when it exits. Initial loading at [graph registration/start](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/source-facade-v1/swf_movie.cpp:299) pins exact graph/player/provider context before shared/root loading. It advances shared roots at line307 but loads/attaches the main root at lines309–313 without main-root advance. Thus the earlier cold-main-display finding is still supported; no inference is made that every live gameplay caller takes this path.

The registration method pins impl/player/provider, filters the already recorded names, delegates to the same SwfAsGraph installer and checks owner failure before adding names. The mapping and bounds methods pin both impl and the retained viewport connection, enter Scope and delegate to that connection. The advance/display entry methods pin impl/root and invoke the same selected root methods. The newly inserted font-reset method does not alter these entry bodies or close their already recorded parity limits.

Comparison statuses and all runtime fields are unchanged: two rows remain matched within their stated valid-owner scope and four remain partial. There were no unclear rows in this six-row set, and no unclear rows were promoted. No test, build, source edit or emulator run was performed.

Refreshed facade/caller dependency fingerprints were checked before and after this bounded pass:

| Source | SHA-256 |
|---|---|
| [port/engine-ui/overlays/source-facade-v1/swf_movie.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/source-facade-v1/swf_movie.cpp:569) | 0370235d606a9eaaba191cc7e4facdf64a779d4fd3ba2686cb23f4d7d5d06ce8 |
| [port/engine-ui/swf_actionscript_connection.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_actionscript_connection.cpp:1) | c3e873f984d9033fd9950a8963738110b6cd07e0cc6db34cbc6c39aeea70c9ee |
| [port/engine-ui/overlays/player-lifetime-v1/gameswf_player.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/player-lifetime-v1/gameswf_player.cpp:1) | a7236e70f0f5dfe97059b94e9db7906591c7b63272f052bdc552f3368fdf258d |
| [port/engine-ui/viewport.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/viewport.cpp:1) | ba2ccdba830d992463f1f61bc02501fb5d7fff65b2c4b246ca5df490e3758ba9 |
| [port/engine-ui/swf_viewport_connection.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_viewport_connection.cpp:1) | 7f5278e000a91a79088be113a3b6f79354a22bc2845be2cbeab922ad17a2b813 |
| [port/engine-ui/overlays/frame-v1/gameswf_root.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/overlays/frame-v1/gameswf_root.cpp:1) | adc7ee0426f469a87d778417d0d4e0f44e0cb2007754b5c76b0116653535e71c |
| [port/engine-ui/swf_frame_connection.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_frame_connection.cpp:1) | 9f3ddea3d3321f0dfc3718d3f1e670558c84a6bb99875adb2f6f9556c7664837 |
| [port/engine-ui/swf_frame_schedule.cpp](C:/Users/adamc/Desktop/workspace/DH_sc/port/engine-ui/swf_frame_schedule.cpp:1) | 2401099b82e8ba5e5f093d5a33690cbe3258363081b74730dce41c13f3d6bc59 |

Exact lane coverage and all status counts remain unchanged; this provenance refresh adds no integrated runtime acceptance.
