# Decompiled-function reconstruction: summary

Branch: `feature/decomp-reconstruction`. This is the single integration branch for the decomp work, separate from
`development`. The game session should sync from it. Current status is in `port/decomp-reconstruction/STATUS.md`.
Each category has its own `NOTES.md` and `ledger.csv`.

Date: 2026-10-12. Five category branches are merged, and each head is an ancestor of this branch: glitch-scene
`aff5bf5d`, net `74326091`, pydata-core `b4aa7a8b`, vox-audio `7283788d` and core-video `8408c960`.

## Where things stand

- 15,164 functions are in scope. They are the nine category queues in `port/decomp-reconstruction/<category>/queue.csv`.
- Every in-scope function has a status, except 161 that are not yet attempted.

| status | functions | meaning in this repo |
|---|---|---|
| reconstructed | 7,472 | a port exists and a test was written. Not checked against the binary (see below). |
| upstream | 1,820 | matched by name to a library. FreeType and libjpeg matches were checked against vendored sources. The rest are name matches. |
| partial | 3,046 | a port exists, but some branch, table, layout or helper is unknown. The no-guessing rule keeps these partial. |
| skipped | 2,665 | deliberately not rebuilt. The note names the blocker: JNI, a missing layout, a thunk, or an out-of-scope helper. |
| not attempted | 161 | queued, no ledger row yet (net-session-online 146, net-transport 15). |

- Host build: 380 of 380 tests pass (`tools/decomp_build.sh host`).
- Windows build (MinGW cross-build, tests under Wine): 380 of 380 pass (`tools/decomp_build.sh windows`).
- Nothing from this tree is linked into `dh2_native` or `dh2_desktop_runtime`. No game CMake or Gradle file refers
  to these libraries. The game session has to do that wiring.

## Per category

Counts come from `python3 tools/decomp_progress.py`, which counts each ledger row once against its category's queue.
Layout-blocked counts the partial and skipped rows whose notes say a struct or class layout is needed. It comes from a
note pattern, so it is an estimate. Host tests are the test executables that CTest runs for the category.

| category | queued | reconstructed | upstream | partial | skipped | not attempted | layout-blocked | host tests |
|---|---|---|---|---|---|---|---|---|
| core-platform | 653 | 296 | 0 | 67 | 290 | 0 | 13 | 24 |
| glitch-core-io | 1,255 | 848 | 17 | 165 | 225 | 0 | 25 | 55 |
| glitch-scene-collada-gui | 4,354 | 1,365 | 3 | 1,982 | 1,004 | 0 | 859 | 76 |
| glitch-video | 1,686 | 630 | 0 | 318 | 738 | 0 | 95 | 41 |
| net-session-online | 1,235 | 781 | 0 | 183 | 125 | 146 | 3 | 76 |
| net-transport | 794 | 528 | 0 | 185 | 66 | 15 | 0 | 31 |
| pydata-accessors | 2,201 | 2,109 | 0 | 72 | 20 | 0 | 0 | 5 |
| tinyxml-and-c-libs | 1,846 | 7 | 1,755 | 8 | 76 | 0 | 3 | 3 |
| vox-audio | 1,140 | 908 | 45 | 66 | 121 | 0 | 2 | 69 |
| **total** | **15,164** | **7,472** | **1,820** | **3,046** | **2,665** | **161** | **1,000** | **380** |

Figures from the category threads differ from this table, for two reasons:

- Ledger rows outside a category's own queue are not counted here. Net-session-online's ledger had 232 such rows.
  198 of them are functions the queue routes to net-transport. 185 of those already have a row in net-transport's
  ledger, and the other 13 are among net-transport's 15 not-attempted rows. The remaining 34 are in no queue. The 232 rows moved to `port/decomp-reconstruction/net-session-online/ledger.ignored.csv`
  so nothing is lost. Core-platform's ledger on the pydata branch had 26 rows outside its queue. Those were not kept.
- Test counts. CTest counts executables. The threads quoted assertion counts, for example pydata 201 and glitch-scene
  331. I have not reconciled those with the 5 and 76 executables here.

The glitch-scene NOTES.md still shows counts from before the final merge. Use the table above.

## What was done, by category

- **core-platform** (653): memory, string, file and config helpers. The pydata thread upgraded 12 rows from partial to
  reconstructed and 3 to skipped. Thunks that were reimplemented rather than forwarded still need review.
  `ConfigFile::XOR` and `Decode` are skipped because their key and obfuscation tables are not in the repo.
- **glitch-core-io** (1,255): core value types and io helpers. Routing was fixed so a return type such as
  `boost::enable_if<glitch::video::...>::type` no longer sends a video function here. 311 rows moved to video.
- **glitch-scene-collada-gui** (4,354): scene graph, animation, COLLADA and GUI classes. Layouts are recovered from
  Hex-Rays usage, field by field, into evidence headers with a source address per field. 1,365 rows are reconstructed.
  1,982 are partial, mostly because their layouts are not yet recovered.
- **glitch-video** (1,686): video driver and material code. The material-parameter tables are bound through
  `IMaterialParametersTyped` (a CRTP mixin) with owner backends for G, M and R. The part 1b hooks are defined.
  `textureAddRef` and `textureStateWord` are complete. `textureDrop` and `destroyLight` are partial.
- **net-transport** (794) and **net-session-online** (1,235): sockets, connections, messages and sessions.
  `ConnectionLobby` now derives from the real `Connection` (see the fix below).
- **pydata-accessors** (2,201): 2,109 reconstructed. Its Arrays tables use the 32-bit stride from the listing, which
  overruns on a 64-bit host (see Limits).
- **tinyxml-and-c-libs** (1,846): 1,755 upstream rows. Of these, 13 are FreeType 2.3.7 and 4 are libjpeg, all matched
  against vendored sources. Four stb_vorbis helpers are name matches only, since stb_vorbis is not vendored here.
  The 7 reconstructed rows are the SSEncDec key table, SSEncDec ByteArray2Blob, NVThreadInit and NVThreadSleep.
- **vox-audio** (1,140): audio decoding and the stream and data-object layer. The Musepack rows are name matches.
  Four of them (`mpc_set_replay_level`, `mpc_demux_ST`, `mpc_bits_can_dec`, `mpc_bits_enum_dec`) are open: the older
  slice ledger marks them skipped as unconfirmed, but the consolidated `ledger.csv` has them upstream.
- Shared headers, one definition each, in `port/decomp-reconstruction/shared/include/glitch-shared/`:
  `glitch_core.hpp`, `sid_collection.hpp`, `gl_function_table.hpp`, `reference_counted.hpp`, `kd_tree.hpp` and
  `shared_string_heap.hpp`. Category `deps/` headers forward to them.

Infrastructure:

- `tools/decomp_categories.py` assigns each function to one category. Ghidra `FUN_` entries are moved to the address
  in their name, and `head()` strips the return type before routing.
- `tools/decomp_progress.py` produces the table above. `tools/decomp_build.sh host|windows [category]` builds and tests.
- Hex-Rays pseudocode is the primary source: `recovered/native/hexrays-all/` (31,724 functions in 64 chunk files).
  The IDA assembly listing is in `recovered/native/ida-asm/`. Ghidra is the fallback.
- IDA type library, `recovered/native/ida-types/layouts/`: 2,467 local types, of which 526 have members. None of the
  437 `glitch::` names has members. **IDA has no glitch layouts.** The glitch layouts come from usage in the decompiled
  code, which is what the scene evidence headers record.

## Fixed in the final merge

- **net-transport, `Connection::disconnect` overflow.** `ConnectionLobby` derived from a stand-in class named
  `Connection` in `deps/connection_base.hpp`. The stand-in has no buffers. The real `Connection::disconnect` clears
  two 0x1001-byte buffers, so it wrote past the stand-in-sized object. ASan reported a stack-buffer-overflow at
  `connection_base.cpp:499`. `ConnectionLobby` now uses the real `Connection`, `ConnectionSocket` moved into
  `connection_lobby.hpp`, and the stand-in header is removed. ASan on the net-transport tests: 31 of 31 pass with no
  reports. Commit `0c53e26f`.

## Testing

- **Host** (Linux): `tools/decomp_build.sh host`, build directory `build/decomp-host`. 380 of 380 pass. The test run
  takes about 2 seconds, and a full build takes several minutes.
- **Windows**: MinGW-w64 cross-build, tests under Wine (`tools/decomp_build.sh windows`, prefix `build/wineprefix`).
  380 of 380 pass, on the first run on this machine.
- **Wine first-run quirk**: the glitch-scene thread found that on a fresh Wine prefix, the first run of each binary
  fails and the second run passes. This machine's prefix was already initialised, so the quirk did not appear. On a
  new machine, run the Windows build twice before reading a failure as real.
- **ASan**: `cmake -S port/decomp-reconstruction` with `-fsanitize=address` for the net-transport tests. 31 of 31 pass.
  Only net-transport was run under ASan. The other categories have not been.

## What the tests do not prove

- Nothing is checked against the original binary. The tests were written from the Hex-Rays pseudocode by the category
  workers. Nothing runs the ARM code or compares its output with ours. "Reconstructed" is the workers' own label for
  "a port exists and its test passes".
- Upstream rows are name matches, except the FreeType and libjpeg rows, which were checked against vendored sources.
  None was byte-diffed.
- Test coverage is thin in places. pydata-accessors has 5 test executables for 2,109 reconstructed functions, and
  core-platform has 24 for 296.
- Stand-ins are used in several places. Some `deps/` headers are stand-ins, and `NetBitStream` is byte-aligned only.
- The `LC_API_*` wrappers have no definition in the repo. The LC copies that use them are partial, and the rest assume
  libc. `XP_API_*` was confirmed as libc in net-session-online's `xp_api.cpp`.
- Nothing is linked into the game. The Android build, `dh2_native` and `dh2_desktop_runtime` do not see any of it.

## Warnings

- **pydata-accessors, Arrays**: the 27 rows of the arrays slice (`w7_arrays27.cpp`) use the 32-bit stride from the
  listing, as does `structs_5`. On a 64-bit host, `sizeof(element)` is larger than the stride for 25 of the 27 rows.
  For example, `Structs::Projectile` is 80 bytes where the stride is 72, and `DestructibleContainer` is 80 where the
  stride is 68. Reads overrun their slot. Do not call these Arrays reads on a 64-bit host until the stride is decided.
- **pydata-accessors, Item family**: the Item-family destructors (ArmorRef, BeltRef, Valuable, Potion, GoldItem, Item)
  and the ItemPower destructors in `structs_4` free the same word twice, as the listing does. Do not call them on live
  objects yet. `ItemList` and `ItemBonusAttrMonopoly` are matched to their element classes by name only.
- **pydata-accessors, generators**: `gen_structs.py` and `gen_script_cmd.py` still parse Ghidra text, so they cannot
  be rerun against the Hex-Rays export.
- **glitch-scene, header clashes**: several new evidence headers have the same class names as older accessor headers.
  Each translation unit may include only one of each pair. The merged build respects this.
- **glitch-scene, evidence**: some classes have no usable evidence. These are `ISceneNode`, `CMeshSceneNode`,
  `CTerrainSceneNode`, the `CSceneNodeAnimatorSet` base and the PD domains.
- **net-transport, layouts**: `tMemberInfo`'s layout is unsettled, because `t_member_info.hpp` was overwritten.
  `CMsgRaisedEvent` and `CMsgAttackResult` are declared twice. The Bluetooth STL string layouts are garbled.
- **net, send paths**: `transport_manager_calls::Send` and the `DataPacket` vtable are stubs, so the send paths are
  blocked. About 64 blocked rows are named only in worker reports, not in the ledger.
- **core-platform, ledger**: its ledger has no rows outside its queue now. The pydata branch's 26 extra rows were
  dropped.
- **Stale NOTES**: net-session-online's NOTES give the old queue count (1,268). glitch-scene's NOTES give the pre-merge
  counts. STATUS and this summary are the current record.

## Limits and defects

- Thunks reimplemented rather than forwarded (core-platform 5, glitch-video 1, net-session-online 2) need review.
- glitch-video material parameters: the pointer slot is `sizeof(void*)` on the host, where the original stores 4 bytes.
  This is a documented port deviation. The owners size their buffers to match.
- glitch-video, the dispatcher declarations in `material_parameters_typed.hpp` still name the 4th argument `count`.
  For codes 12 to 15 and 18 it is a byte stride. The impl passes it through unchanged, so this is a naming fix only.
- `CTerrainSceneNode` at `+0x178` and `+0x17c` has conflicting evidence (see Open items).
- `pydata-accessors/0x4f2750` (`Structs::CharacterProperties::read`) is ledgered but not queued. The progress tool
  reports one ignored row.

## What is left

Not attempted: 161 (net-session-online 146, net-transport 15).

Partial: 3,046. Most are in glitch-scene (1,982) and glitch-video (318), where a layout or table is missing.

Skipped: 2,665. Most are in glitch-scene (1,004), glitch-video (738), vox-audio (121) and net-session-online (125).

Layout-blocked: 1,000 (glitch-scene 859, glitch-video 95, glitch-core-io 25, core-platform 13, tinyxml 3,
net-session-online 3, vox-audio 2). Of these, the 859 scene rows should be retried against the evidence headers.

Open items from the glitch-scene thread:

- Rerun the blocked partial rows against the evidence headers.
- Resolve the `CTerrainSceneNode` `+0x178` and `+0x17c` conflict.
- Settle the `ISceneNode` `+0xf4` question.
- Find evidence for the classes listed under Warnings.

Other open items:

- glitch-video: `textureDrop` and `destroyLight` are partial (onDelete and the Matrix4Pool push). The 311 rows moved
  from core-io are in glitch-video's queue and ledger. `glitch-core-io/include/glitch-core-io/video/` no longer exists,
  so do not look there for them.
- glitch-video: blockers on `IVideoDriver +0xd4`, `IAttributes` and `ITexture` remain. Their layouts come from usage,
  not from IDA, so they need evidence headers built from usage.
- glitch-core-io: `deps/glitch_core.hpp` now forwards to the shared header, so `rect` is the shared one, with
  `UpperLeftCorner` and `LowerRightCorner`. Core-io's own `X1..Y2` naming is gone.
- glitch-core-io: `core/sided_collection.hpp` defines a second `SIDedCollection`, separate from the shared one in
  `sid_collection.hpp`. Core-io's owner decides which stays.
- tinyxml-and-c-libs: VZPurchase (10), the NVThread JNI and pthread functions (3), the Java `nativeInit` thunk and the
  three `II_Item` comparators are skipped. They need JNIEnv, pthread, `ADevice` or the `Item` layout. The stb_vorbis
  helpers are name matches. vox-audio should confirm them, and their routing should move from tinyxml to vox-audio.
- vox-audio: `ledger.parts/` holds the older slice ledgers. 306 of its rows differ from `ledger.csv`, which is the
  consolidated record and the file the progress tool reads. Vox should retire `ledger.parts/`.

## Open decisions

1. `intrusive_ptr`: there are three forms. Core-io's counts references. Video's and scene's are non-counting stubs.
   Pick one for the game. The owners decide.
2. Pointer slot width: keep `sizeof(void*)` (a port deviation) or match the 4-byte original.
3. Arrays stride (pydata): widen each table to the host `sizeof`, or build the Arrays code for a 32-bit layout.
4. `LC_API_*` wrappers: confirm them as libc, or define them in the repo.
5. Thunks: forward them to their targets, or keep reimplementing them.
6. Routing of the stb_vorbis helpers: move them from tinyxml to vox-audio.
   Likewise the four Musepack rows above: keep them upstream by name, or mark them skipped until libmusepack is compared.
7. Which of each clashing header pair stays (scene evidence headers against older accessor headers).
8. Whether and when the decomp libraries are linked into `dh2_native` and `dh2_desktop_runtime`. This is the game
   session's decision.

## Integration (for the game session)

- Sync from `feature/decomp-reconstruction`. Do not merge it into `development`.
- The code is a set of static libraries under `port/decomp-reconstruction/`, one per category. `CMakeLists.txt` builds
  them as `dh2_<category>` (with `-` replaced by `_`), plus their tests. Each library's public include path is its own
  `include/` and `shared/include/`.
- To use a category, link its library and add its include path. Do not link `glitch-core-io` and `glitch-video` into
  one target until the `intrusive_ptr` decision is made. Both use the shared `rect` now.
- Live gameplay (characters and AI, quests, items, levels and scripts, menus and HUD, camera, animation, input,
  renderer) belongs to the other "Dungeon Hunter II native reconstruction" session. Do not overlap with it.
- Read `port/decomp-reconstruction/STATUS.md` first. It lists the decisions in force and the open items. Each category's
  `NOTES.md` has its own detail.
