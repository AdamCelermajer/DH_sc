# Track B status

Updated 2026-10-15 by the integrator. Rules and decisions: `TRACK-B.md`. Totals: `py -3 tools/decomp_progress.py`.

## Totals (whole binary in scope: 25,218 functions)

| status | functions | share |
|---|---:|---:|
| reconstructed | 18,138 | 71.9% |
| upstream (Lua 5.1.4, Box2D 2.0.1, gameswf r1714, tinyxml, libjpeg, FreeType, Musepack) | 3,984 | 15.8% |
| partial | 1,024 | 4.1% |
| skipped | 2,072 | 8.2% |

Every queued row has a ledger row, every ledger is a valid 5-field CSV, and `decomp_progress.py`
reports **0 ignored rows**. A false-positive screen (`tools/trackb/verify_rows.py`, which flags a
`reconstructed` row whose body is empty/null or whose address appears only under `src/recon/`) reports
**0 suspect rows across all 18 ledgers**, so the `reconstructed` figure is not inflated.

| category | queued | reconstructed | upstream | partial | skipped |
|---|---:|---:|---:|---:|---:|
| game-world | 1,527 | 1,333 | 0 | 102 | 92 |
| game-app | 1,409 | 1,241 | 2 | 85 | 81 |
| game-character | 1,143 | 1,097 | 0 | 0 | 46 |
| game-items-save | 831 | 765 | 0 | 0 | 66 |
| game-ui | 808 | 775 | 0 | 0 | 33 |
| game-anim | 365 | 335 | 0 | 0 | 30 |
| glitch-scene-collada-gui | 4,735 | 4,097 | 3 | 346 | 289 |
| glitch-video | 1,701 | 891 | 0 | 139 | 671 |
| glitch-core-io | 1,336 | 1,049 | 17 | 65 | 205 |
| net-session-online | 1,244 | 955 | 0 | 144 | 145 |
| net-transport | 1,048 | 904 | 0 | 41 | 103 |
| pydata-accessors | 2,273 | 2,270 | 0 | 3 | 0 |
| vox-audio | 1,140 | 997 | 72 | 23 | 48 |
| core-platform | 653 | 477 | 9 | 14 | 153 |
| tinyxml-and-c-libs | 1,846 | 31 | 1,791 | 0 | 24 |
| vendor-gameswf | 2,370 | 918 | 1,304 | 62 | 86 |
| vendor-box2d | 318 | 2 | 316 | 0 | 0 |
| vendor-lua | 471 | 1 | 470 | 0 | 0 |

At the start of Track B (`2482a90d`) the scope was 15,164 functions with 7,472 reconstructed.

### Reading a `partial` note

Partial notes are free text and are **not** a reliable blocker index. Mining them for a blocker name
gives a strong-looking signal that does not survive reading the rows: `IVideoDriver` appears in 68 of
them, but most of those mentions are explaining a blocker that was **CLEARED**, not an active one
("both old blockers CLEARED", "the previous reason was WRONG"). Across all 1,024 partial rows, 291
mention a cleared/obsolete blocker, 542 state an active missing item, and 451 use other wording. Treat
every note as a claim to re-test against the listing, and treat a row whose *body* exists but whose
*note* is stale as a candidate to flip rather than as new work.

## Build state (2026-10-15, `build/tb-int`, native MinGW, GCC 16.2.0)

`ninja -k 0`: **1,540 edges, exit 0, 0 compile errors, 0 undefined symbols, 0 multiple definitions**
(163 symbols are satisfied by `link-stubs/`, which is expected). `ctest`: **824 of 824 pass** on a
clean build directory.

Two build-hygiene hazards cost real time this round; both are environmental, not defects:

- **`build/tb-int` cannot be shared by concurrent builds.** Several workers building at once produced
  `ar: ... file truncated`, `ninja: error: failed recompaction: Permission denied`, and
  `ld.exe: cannot open output file ...: Permission denied`, and one stub regeneration started
  mid-build wrote a nonsensical 240-stub CSV. Serialize builds, or give each worker its own directory.
- **`link/library-duplicates` is reconfigure-sensitive, and its failures are usually NOT defects.**
  `link-stubs/link_stubs.csv` is a `CMAKE_CONFIGURE_DEPENDS` input and `CMakeLists.txt` numbers the
  stubs **by CSV row index** (`stub_1..stub_N` via `file(GENERATE)`), so editing the CSV after
  configure leaves stale `stub_NN.cpp` files defining symbols that real code now implements. Always
  re-run `cmake` before believing that test. This is also the mechanism behind the intermittent
  `CMake Generate step failed`.
- **Incremental green is not proof.** A `100% of 819` run once reported here was stale: it had run
  against binaries built *before* a header change, while a clean build of the same tree gave 823/824.
  Take the clean-build number.

### HISTORICAL - the 2026-10-14 crash map (all of these are FIXED; kept for the reasoning only)

The suite is green as of 2026-10-15. Everything below this line is the previous round's failure list
and is retained because it records *why* each test was broken, which is still the best guide to the
stub hazards in this tree. Do not treat any of it as an open failure.

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
  10 partial, 3 upstream; the link stub set is 117 symbols).

  **The CImage cluster is now closed end to end** (u1-video follow-up): the owning constructors 0x602028 and
  0x602110, the (data, copy, own) constructor 0x602670, initData 0x601988, fill 0x5ff9b4 and the destructors
  0x601804 / 0x6018b8 are all reconstructed, and glitch-video/image_fill_test (registered, passing) exercises
  them together. One combination is knowingly left leaking and is named in the 0x601804 ledger row: an
  m_ownsData-clear image built with the mip flag allocates its level array in initData but skips it in the
  destructor. No caller builds that combination today, and the no-own constructor passes the mip data through
  instead. Worth closing when the constructors 0x6021f8 / 0x6027e8 are done.
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
| cimage-ctors | the owning constructors 0x602028 / 0x602110 (+ their C2 aliases) | `c786d316` |
| cimage-dtor | `~CImage` 0x601804 / 0x6018b8 / 0x60196c; the CImage cluster closed | `e09bc03d` |
| partial-game-anim | the last 3 game-anim partials; the category is now 335/335 | `b9983937` |
| partial-nettx | verified net-transport rows whose only gap was a dropped debug trace | next |

## What does NOT make a row `partial` (policy, applied from the integrator)

322 rows were already ledgered `reconstructed` while their notes mention a debug assert or a dropped trace, so
that alone is not a reason to hold a row back. The line is drawn at observable behaviour:

- A dropped **XP_DEBUG_OUT / debug trace** is a logging deviation only: the return value, the state writes and
  the control flow are all reproduced. Such a row is `reconstructed` with the trace named in its note. The
  precedent is in this file's own category: `CAndroidSocket::Recv` (0x8410d8) has been `reconstructed` with
  "XP_API_GET_TIME result unused and dropped" since before this pass.
- A dropped **debug assert** is the same, unless the assert's condition is a precondition whose violation
  changes what the code then does. `Buffer::setSize`'s assert is such a precondition but is documented in the
  row; the dropped asserts at net-transport level are all `gAssertLevel` guards.
- A **host-necessitated deviation** (a null guard where the original dereferences and crashes, joining a
  `std::thread` so the process does not terminate, a `std::clock`/`steady_clock` substitution) is still
  `reconstructed`, because no faithful choice exists; it is named in the note.
- A row **stays `partial`** when something observable differs or is missing: a return value taken from a trace
  result that was not recovered, an unbounded stack copy that is not reproduced, a spin-on-zero-read loop that
  is not reproduced, or a body that does not match the listing.

Verified and moved this pass (each read against its own Hex-Rays, not taken from a note):
0x840f68 CreateSocket, 0x8410f8 Send, 0x8412b4 CloseSocket, 0x846488 sendEstablishConnectionPackage,
0x8466fc receiveDataLen. About 14 more net-transport rows look like trace-only candidates
(SendBroadcast, RecvFromUnkownIP, RecvFrom, SendTo, ConnectTo, ConnectByName, SetNonBlocking, Thread::Run,
Connection::sendLoginPackage, ConnectionLobby::sendLoginPackage/receiveDataLen/sendData,
DataPacketLobby::addPacket, LCAndroidSocket::*) but were NOT reclassified, because each needs its own
reading first.


## The two same-named particle classes (settled)

`glitch::scene::CParticleSystemSceneNode` and `glitch::collada::CParticleSystemSceneNode` are different
classes that share a name, and more than one blocker note pointed at the wrong one:

- `glitch::scene::CParticleSystemSceneNode` (`scene/CParticleSystemSceneNode.hpp`) is the real node: it derives
  from `IParticleSystemSceneNode`, owns the `+0x178` emitter holder, and has `getPosition` / `setPosition`. Its
  base declares the live-particle count as **`getNumberOfParticles()`**, not `getParticleCount()`.
- `glitch::collada::CParticleSystemSceneNode` (in `collada_scene_accessors.hpp`) is a two-method stub
  (`getUID`, `getTransformedBoundingBox`) whose `getParticleCount` is a placeholder returning 0
  (`recon/slice_20.cpp`). Do not reach for it when a particle node is needed.
- The node that `VisualObject::GetSpecificNode(0x70656164)` / `(0x70656E64)` returns is the scene class; the
  sibling `AnimatedFX::SetEndPoint`, which reads the same `+0x178` holder, is what settles it. Reaching it needs
  a `dynamic_cast` from the `scene::ISceneNode*` that `GetSpecificNode` returns.


## Rows still undefined at link (173 symbols, `reports/decomp-reconstruction/link-missing.csv`)

THIS NUMBER MOVES EVERY WAVE - read the CSV, do not trust a count written here. Of the 173, the
recoverable ones are mostly `partial` rows whose blocker is named on the row; the rest are `skipped`
by design (JNI entry points, vendored GL/audio glue such as `gluTessCallback`, the mpc demux
helpers).

Rows that USED to be listed here and are now done, so the old blocker text is gone:
- `glitch::video::CImage::fill` (0x5ff9b4) - reconstructed and covered by `glitch-video/image_fill_test`.
- `PlayerLightTweaker::onSetValue` (0x40de48) - reconstructed; the "needs CTweakable and dh2/LevelConfig"
  blocker was stale, both existed.
- `GraphSparse<PFGOuterEdge>::clear()` (0x522f70) - the old "hand-walked STLport rb-tree iterators"
  description was WRONG; the walk is two plain `std::map::clear` calls plus a virtual delete per node
  (identical to the reconstructed inner instantiation 0x52328c). It stays partial for one concrete
  reason: `PFGOuterEdge` and `PFGOuterNode` have no definition anywhere in the tree AND no symbols in
  the ELF, only `.symtab` STT_FILE entries, so the deleted classes do not exist to write against.

Still genuinely blocked, with the exact missing item:
- `CResFileManager::get` x2 / `load` x2 and `CResFile::releaseRemovableBuffer` - the file-map rb-tree,
  `IBuffer` and the zip/texture stand-ins.
- `CMaterialRenderer::getTechniqueID` (0x5d4714) - `SSharedStringHeapEntry::SData::get(name,false)` and
  the technique map.
- The STLport allocator pair `_M_allocate_block` / `_M_deallocate_block` and
  `__node_alloc::_M_deallocate`, reported unrecovered tree-wide; they gate `_String_base` rows in
  game-app, glitch-core-io and glitch-scene-collada-gui.
- `glitch::ps` / `collada/particle_system` rows - the headers do not exist in the tree.

## Running

- Test triage: the 34 failing tests (see the crash-site map in Build state above). The cheapest next
  wins are the rows whose crash is a missing delegate (`MenuManager::GetRoot`), and the shared
  `PlayerManager::GetPlayerByCharacter` path (2 tests).
- armdiff: the ARM32 differential tester (NDK armv7a build plus Unicorn).

## Next

1. The 8 parallel worker batches in flight (see "Running"): keep every slot busy - the concurrency cap
   is 8 children, and each batch is a category sweep plus the named-unblocker work.
2. Confirm the `IDevice +0x34` retype (requested by the glitch-scene-collada-gui worker) unblocks
   `CTweakable::loadXML`; the claim is that the listing of 0x32b6b8 calls `IFileSystem::createXMLReader`
   slot +0x50 on it while `os/device.hpp` declares it `FileSystemWin32*`.
3. Recover the STLport allocator pair - highest-leverage single item left, it gates rows in four
   categories.
4. Triage the failing tests (34 at the pre-round baseline, plus 3 segfaults introduced by newly
   written bodies that need a driver the test does not provide - a fixer is on them).
5. The skipped rows whose notes give no clear reason.

## Corrections made this session (do not regress these)

- `NetBitStream::WriteString` (0x80edb0): the third argument is NOT a simple cap. A clamp was tried
  and reverted because `net_bit_stream_test` calls `WriteString("hello", 0)` and expects a full
  round-trip. The row is `partial` with the open question named.
- `StreamBuffer::StreamBuffer(IStreamBase*)` (0x3171b8): the original DOES read the whole source. The
  Hex-Rays call is `v5(a2, firstBlock, total, 0)` against `(IStreamBase*, _DWORD, _DWORD, _DWORD)`, so
  the trailing `0` is the HIGH word of the 64-bit count (`read` takes its length in r2:r3), not the
  count. A note claiming otherwise was wrong.
- `CIrrFactory::createSceneManager` returns `glitch::scene::CSceneManager*`; the game's `IrrFactory`
  narrows it to `::SceneManager*`. Covariance needs the complete type, so `dh2/IrrFactory.hpp` includes
  `dh2/SceneManager.hpp` rather than forward-declaring `SceneManager`.
- The two same-named particle classes (above) - one is a two-method stub.
