# Track B status

Updated 2026-10-14 by the integrator. Rules and decisions: `TRACK-B.md`. Totals: `py -3 tools/decomp_progress.py`.

## Totals (whole binary in scope: 25,218 functions)

| status | functions | share |
|---|---:|---:|
| reconstructed | 17,504 | 69.4% |
| upstream (Lua 5.1.4, Box2D 2.0.1, gameswf r1714, tinyxml, libjpeg, FreeType, Musepack) | 3,984 | 15.8% |
| partial | 1,616 | 6.4% |
| skipped | 2,119 | 8.4% |

Every queued row now has a ledger row (the last G1b rows are in).

| category | queued | reconstructed | upstream | partial | skipped |
|---|---:|---:|---:|---:|---:|
| game-world | 1,527 | 1,259 | 0 | 176 | 92 |
| game-app | 1,409 | 1,208 | 2 | 118 | 81 |
| game-character | 1,143 | 1,095 | 0 | 2 | 46 |
| game-items-save | 831 | 765 | 0 | 0 | 66 |
| game-ui | 808 | 775 | 0 | 0 | 33 |
| game-anim | 365 | 332 | 0 | 3 | 30 |
| glitch-scene-collada-gui | 4,735 | 3,996 | 3 | 444 | 292 |
| glitch-video | 1,701 | 758 | 0 | 245 | 698 |
| glitch-core-io | 1,336 | 1,016 | 17 | 96 | 207 |
| net-session-online | 1,244 | 906 | 0 | 192 | 146 |
| net-transport | 1,048 | 849 | 0 | 96 | 103 |
| pydata-accessors | 2,273 | 2,208 | 0 | 56 | 9 |
| vox-audio | 1,140 | 943 | 72 | 74 | 51 |
| core-platform | 653 | 438 | 9 | 51 | 155 |
| tinyxml-and-c-libs | 1,846 | 26 | 1,791 | 5 | 24 |
| vendor-gameswf | 2,370 | 918 | 1,304 | 62 | 86 |
| vendor-box2d | 318 | 2 | 316 | 0 | 0 |
| vendor-lua | 471 | 1 | 470 | 0 | 0 |

At the start of Track B (`2482a90d`) the scope was 15,164 functions with 7,472 reconstructed.

## Build state (2026-10-14, `build/tb-int`, native MinGW)

`ninja -k 0`: 877 of 877 targets, exit 0. `ctest`: 779 of 813 pass. `link/library-duplicates` PASSES.

- 34 tests fail: 24 SEGFAULT, 7 assertion failures, 3 exit `0xc0000409` (a link stub was reached).
  The first crash fixed (2026-10-14, third commit) was `GSEndGame-20f7ca-0`: its `COnline::GetInstance()`
  stub returned null with the comment "the test never dereferences it", but the library's
  `GetOnlineGameFlag()` (game-app/src/online_flag.cpp) reads `COnline::IsOnlineGame()` through that pointer.
  The test now supplies a real `COnlineImpl`, as `LevelSavegame-0b011d-1_test` already did.
  Two more fixed the same way: `MenuManager::ResetFonts` (0x42d72c) forwards through `m_multiMenuManager`,
  and two MenuBase tests stubbed `MenuManager::GetInstance()` to null, so `FS_SetLanguage` and
  `FS_IncOption` crashed; the forward is stubbed in the test and `r2a-game-ui-MenuBase-8948cc-0`'s
  `FS_SetLanguage`/`FS_LoadOptions` assertions were stale (both handlers are reconstructed and return true
  on a nonzero `setLanguage`).
- Crash sites for the remaining segfaults, all INSIDE reconstructed bodies (so these are real defects or
  unreconstructed seams, not stub noise). Grouped by owner:
  - game-anim: `AnimSetManager::AddTemplateAnim`, `AnimatedFX::_HandleLoopEnd`,
    `AnimController::AnimController`, `CSceneNodeAnimatorSet::animateNode`, `GamepadTweaker::UpdateButtonMappings`,
    `v2Controller::Cmd_UseOOI`, `AnimationSet::FreeAll`, `EventManager::Raise`.
  - game-app: `nativeResumeMusicBG`, `RenderFX::Find`, `VoxSoundManager::Play`.
  - game-character: `PlayerManager::GetPlayerByCharacter` (2 tests), `Character::AddCharOIDToCache`.
  - game-ui: `MenuBase::FS_IsSaveSlotValid`, `OnScreenKeyboard::onEvent`, `Dragable::Dragable`,
    `MenuFlash2DCamera::Update`, `MenuFX::SetMenu`, `MenuManager::GetRoot` (reached through
    `ApplyLanguageAndShowName`; `GetRoot` is 0x42cb80 = `multi->m_menuFX[2]` and has NO definition in the
    tree yet, so `MenuBase-8948cc-1` needs that row reconstructed rather than another stub).
  - game-world: `LiftableObject::Release`, `SkyBoxMeshSceneNode::SkyBoxMeshSceneNode`.
  - other: `TiXmlAttribute::Next`.
  - `scene_mesh_functions_test` dies on a null call in the `createMeshWith2TCoords` path.
- The three `vox-audio` `0xc0000409` ones are explicit:
  `DH2 link stub called (not reconstructed yet): _ZN15VoxSoundManagerC1Ev`.
- Regenerating `link-stubs/link_stubs.csv` was required after the residue wave:
  180 tests no longer linked without stubs (121 stub symbols).
- Resolved (2026-10-14, second commit): all six duplicate global definitions.
  `AnimationSet::s_invalidAnim` was defined twice (the batch file default-initialised it; the
  canonical storage is `game-app/src/statics_AnimationSet.cpp`, which reproduces the binary's
  static initializer 0x364cf8). `Res_bit`/`__Cc`/`__Dc` collided because `requant.h` declared them
  without `extern` - a tentative definition in C, so every including TU emitted a copy; upstream
  marks them `extern`. `point_compare`/`dct_iv_slow` collided because stb_vorbis's file-local
  helpers lacked internal linkage and clashed with the reconstructed originals in
  `core-platform/src/vorbis_util.cpp` (0x8764f8, 0x8784f4).
- Remaining undefined symbols in the whole tree: 178 (84 with no ledger status, 81 skipped,
  10 partial, 3 upstream; the link stub set is 118 symbols).

  **Observed, not inferred, for the CImage pixel formats** (checked by building an image over a caller's
  buffer and reading the bytes back): format 12 writes component order a,b,c,r and 13 writes b,a,c,r and 14
  writes c,b,a,r, where a/b/c are SColor components 0/1/2 and r is component 3; formats 5 and 8 pack a single
  halfword and replicate it into both halves of the word; any other format writes nothing. The write loop is
  `m_dataSize >> 5` passes of eight words plus `(m_dataSize >> 2) & 7` words (0x5ffa38 and the UBFX at
  0x5ffa68). `CImage::fill` is now VERIFIED by `glitch-video/image_fill_test`, which is registered with ctest
  and passes: it builds real images over a caller-owned buffer and asserts the exact bytes, including the
  negative cases (a 1x1 16-bit image has a 2-byte block, smaller than the loop's word, so fill writes nothing;
  a format outside the five writes nothing).

  **Stale blockers cleared this pass (read these before skipping a row again).** Two large clusters are
  recorded as blocked on infrastructure that is now in the tree:
  - `CMaterialRendererManager` (`createMaterialRenderer` 0x5d8b28, `createMaterialInstance` 0x5d9af0,
    `getMaterialRenderer`, `getMaterialInstance` 0x5dd0e4, and the rest of its 40-odd rows): the ledger says
    the "technique/pass/instance tables are not in material_renderer_manager.hpp". `SIDedCollection` now
    exists (`glitch-shared/sid_collection.hpp`) and
    `glitch-video/include/glitch-video/material_renderer_manager_layout.hpp` gives every offset with the
    function that fixes it. The four lookups above also share one already-written shape: entry = entries[id]
    with the collection's Invalid sentinel when id is past the end. The real work is the header refactor, and
    the layout doc flags one genuine conflict to settle first (open question 1: the original puts the id at
    node +0x22 where the shared template puts it at SIdValue +0x1c). Worth a dedicated pass - those four
    symbols are the largest link fan-out in the tree.
  - `CImage::fill` (done) recorded "needs the pixel_format PFD table"; `PFDTable` has since landed in
    `glitch-video/src/pixel_format_table.cpp`, and its rows name the packing each format case uses.

  The largest single blocker is still `CMaterialRendererManager`
  (`createMaterialRenderer` 0x5d8b28, `createMaterialInstance` 0x5d9af0, `getMaterialRenderer`,
  `getMaterialInstance` 0x5dd0e4): all `skipped` because the technique/pass/instance tables are
  not in `material_renderer_manager.hpp`. Those four are referenced from hundreds of objects
  (the largest link fan-out in the tree) and unblock the glitch-video partials with them.



## What "reconstructed" means today

The body follows the Hex-Rays control flow and side effects. It compiles in the unified tree. A Sonnet
verifier checked the three largest functions of each worker batch against Hex-Rays and the assembly, and a
batch that failed got one Haiku fix pass. Nothing has been executed against the original machine code yet.
`tools/trackb/armdiff` (in progress) will run original and reconstructed functions side by side under Unicorn.

## Waves done

| wave | what | result |
|---|---|---|
| setup | queue the whole game, one CMake program, tools | `6d4d454c` |
| L0, keystone | 44 foundation game headers (Sonnet) | all compile |
| E1, E2 | newly queued engine, net and pydata functions | 741 + 240 rows |
| L1, L2, L3 | 459 + 29 game class headers, header fixes | 482 of 488 compile alone |
| decisions | 12 engine decisions (binary wins), static C++ runtime | engine tests pass |
| vendor | Lua, Box2D and gameswf vendored, 2,130 rows upstream | `8124d46c` |
| R1, R1n | engine and net partial redo | 107 + 157 upgraded |
| G1a | 181 gameplay batches | 1,600 reconstructed |
| L3b | 867 layout requests applied (add and retype only) | `b70ac95e` |
| unify | net classes, game types (Point3D, streams, Structs): one definition each | `5fdf7cb3`, `3c1011ab` |
| V1 | Gameloft's gameswf, Box2D and Lua additions | 315 reconstructed |
| R2a | gameplay redo of unblocked partial rows | 464 upgraded |
| U1, U2 | canonical engine types (core-io, video, scene), one definition per class | engine tests pass |
| residue | engine residue passes 1 and 2 (animation tracks, CTerrainSceneNode, ps domains, particles, CMaterial, GL driver, animated mesh, shadow volume) | `aec5a38c`, `552f9847` |
| residue-int | ledger merge, build repair to 934/934, stub regeneration, test triage | `9f5d5e6e` |
| residue-fix | six duplicate globals resolved, first three tests fixed, crash-site map | `803c39d5`, `5a433c6f` |
| partial-redo | `CDynamicAnimationSet::setDefaultAnimationLibrary` (both overloads, 0x62fc90 / 0x62fcf8) | `40253893` |
| partial-redo2 | `CDynamicAnimationSet::addAnimationLibraryBindings` (0x62ee9c) | `1be2fe6e` |
| partial-redo3 | `CImage::fill` (0x5ff9b4); PFDTable rows named the packing | `6f71e24c` |
| partial-redo4 | `CImage::initData` (0x601988) and the 5-argument constructor (0x602670) | `2caf0176` |
| verify | `CImage::fill` measured and asserted; image_fill_test registered | `47238564`, `9f22c1e2` |
| cimage-ctors | the owning constructors 0x602028 / 0x602110 (+ their C2 aliases) | next |

## Partial rows still undefined at link (9)

Read these before starting one; each has the real blocker named.

| address | symbol | why it is still partial |
|---|---|---|
| 0x522f70 | `sfc::math::graph::GraphSparse<PFGOuterEdge>::clear()` | Not a plain `clear`: two nested `std::map`s (outer `map<int,PFGOuterEdge*>` at +0x04/+0x0c, inner `map<int,_InNode*>` inside each edge at +0x08..), a virtual release slot +0x20 on each node, and hand-walked STLport `_Rb_tree` iterators (`node[1]`/`node[2]`/`node[3]` = parent/left/right). Needs a container-level reading, not a line-by-line one. |
| 0x5ff9b4 | `glitch::video::CImage::fill(SColor const&)` | Needs the `pixel_format` PFD table, which is not modelled. |
| 0x5d4714 | `glitch::video::CMaterialRenderer::getTechniqueID(char const*) const` | Needs `SSharedStringHeapEntry::SData::get(name,false)` and the technique map. |
| 0x40de48 | `PlayerLightTweaker::onSetValue(std::string const&)` | Needs `glitch::debugger::CTweakable` as a class and `dh2/LevelConfig`; the older note may be stale, recheck the headers first. |
| 0x65a9a8, 0x65ac74, 0x65ac5c, 0x65a98c | `CResFileManager::get` x2 and `load` x2 | Need the file-map `_Rb_tree`, `IBuffer`, and the zip/texture stand-ins in canonical headers (a layout request). |
| 0x657d58 | `CResFile::releaseRemovableBuffer(unsigned char)` | Same cluster: needs the rb-tree, `IBuffer` and the stand-ins. |


## Running

- Test triage: the 34 failing tests (see the crash-site map in Build state above). The cheapest next
  wins are the rows whose crash is a missing delegate (`MenuManager::GetRoot`), and the shared
  `PlayerManager::GetPlayerByCharacter` path (2 tests).
- armdiff: the ARM32 differential tester (NDK armv7a build plus Unicorn).

## Next

1. Resolve the six duplicate global definitions so `link/library-duplicates` passes.
2. Triage the 34 failing tests: separate seam crashes (a stub or a null reconstructed call) from real
   reconstruction defects, and ledger each one.
3. Continue the partial rows, biggest first: scene 444, video 245, net-session-online 192,
   game-world 176, game-app 118, net-transport 96, glitch-core-io 96.
4. Platform layer: the JNI entry points (167 skipped rows) and the Android glue, required for an Android build.
5. Review the skipped rows whose notes give no clear reason.
6. armdiff across every reconstructed function that makes no calls, then the ones that make calls.
7. The rename and cleanup pass: the evidence-based member names collected by the waves, statics and signatures.
8. ARM64 Android build of the whole tree.
