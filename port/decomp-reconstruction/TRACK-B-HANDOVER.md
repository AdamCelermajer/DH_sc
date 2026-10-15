# Parallel-worker round: handover for the next session

Written at the end of a round that ran 20+ concurrent worker batches. Read this before touching the
failing tests: several of them are NOT regressions from missing reconstruction, they are open design
questions that a worker correctly refused to guess at.

State at the end of the round (build green):
- ninja 820/820 exit 0, 164 undefined symbols (was 173), 0 multiple definitions.
- ctest 40 failed of 818 (the pre-round baseline was 34 of 813, but 5 new test targets appeared and the
  tree was rewritten underneath; see "the baseline is not reproducible" below).
- TOTALS 25,218 = 17,914 reconstructed / 3,984 upstream / 1,237 partial / 2,083 skipped.
  The partial count fell 1,608 -> 1,237 this round.

## The baseline is not reproducible - do not chase a "34"

Two independent workers tried to diff against the 34-failure baseline and could not: no baseline LIST is
persisted anywhere. The only recorded full-run list is
`build/tb-int/Testing/Temporary/LastTestsFailed.log` (138 entries, captured while net-session-online and
collada were mid-edit). Compare against a fresh run you take yourself, not against "34".

## Open design questions (do NOT guess; each needs a decision)

These are the reason several tests fail. Each is a case where the 32-bit original's storage cannot be
expressed directly with host-width types, and a worker correctly stopped instead of inventing.

### The two single classes that block the most rows

- **`glitch::video::CBatchDriver` does not exist at all** (forward-declared only, at
  `glitch-video/include/glitch-video/video_types.hpp:73`). It is the single biggest blocker in the tree:
  it gates ~11 `glitch-scene-collada-gui` rows (CSceneManager's ctors/dtors 0x58d908 / 0x58ddd0 /
  0x58ef00 / 0x58eb70 / 0x58ef1c, all six compile/compileInternal rows, and CBatchSceneNode's
  registerSolidBatches / registerTransparentBatches / renderBatchBBoxes) plus `game-world`'s
  `Level::_LoadBatching`. Reconstructing it is glitch-video's job and would be one of the highest-value
  single tasks left.
- **`CMaterialRendererManager::createMaterialRenderer` (0x5d8b28) has no body** (it is `skipped`). It
  gates six CSkyBoxSceneNode / CBillboardTextSceneNode rows. NOTE: the older claim that the
  material-parameter "seam" blocked them is STALE — `material.parameters.setParameter` stores into the
  material's own value buffer now.
- **`CVertexStreams::computeBoundingBox` (0x5a0ec0) is declared with no body** — the specific missing
  item for `CBatchMesh::updateBoundingBox` (0x57a6d4) and `CMesh::recalculateBoundingBox` (0x6bc2cc).

### Reusable recovery methods (both proved this session)

- **A `getStringsInternal`-style table is always recoverable.** These functions are 2-3 instructions
  returning a pointer to a file-local static table. `get_bytes` the returned address, then read each
  pointer as a string. Two rows closed this way: `getStringsInternal(E_LIGHT_TYPE*)` 0x5a18a8 ->
  table 0x99b5fc = {Point, Spot, Directional, Ambient, NULL}, and
  `getStringsInternal(E_INTERPOLATION_MODE*)` 0x667c38 -> table 0x99b868 = {Default, Step, Linear}.
- **When a note says a symbol's ADDRESS is unknown, look in the ELF symbol table**, not for a data
  xref: find the symbol's NAME string, take the `Elf32_Sym` entry that references it, and read its
  `st_value` field at entry offset +0x04. `_ZN3rnd19DIRECTION_OPPOSITESE` -> name at 0x121282 ->
  Elf32_Sym at 0x56d98 -> st_value 0x999880 -> the four int32 {2,3,0,1} (now defined in
  `game-app/src/rnd_direction_tables.cpp` with `sDirections`).
- **A compiler-static data table in the shipped `.so` is `.bss`**: a raw `get_bytes` returns zeroes.
  The values exist only as the explicit stores inside each function's one-time guard block, and the
  compiler SHUFFLES the store order, so they must be decoded by tracking register constants across the
  store run. Two generators were recovered this way (`createPlaneMesh` 0x9f7ec8, `createCubeMesh`
  0x9f7f4c, 224 words total). `tools/trackb/listing_index.py` exists for a one-pass listing index,
  because `decomp_asm.py` rescans 145 MB per address and is unusable in bulk.

### Two wrong source comments that will mislead the next worker

- `glitch-scene-collada-gui/src/scene/CSceneManager_virtuals.cpp:818` — the comment immediately above
  `CSceneManager::clear` is a copy of **GSViewer::Dtor**'s (it quotes 0x3871c8 and the
  `Application::m_device +0x10 / +0x1c` path). `CSceneManager::clear` 0x589888 is really
  `rootNode->removeAll()` through the parent's vtable +0x64 plus erasing the vector at +0x90..+0x098.
  The port's body is right; only the comment is wrong.
- `glitch-scene-collada-gui/src/scene/CCameraSceneNode.cpp:93` — says "0x582254 ... D1Ev (skipped;
  D0 0x35c318 likewise)", i.e. swapped. 0x35c318 is the D1 and 0x582254 is the D0.

### Dead stand-ins that make rows look unreconstructed

`glitch-scene-collada-gui/src/recon/slice_05.cpp` still carries stand-ins (`billboardOnRegisterSceneNode`,
`billboardGetMaterial`, `billboardSetColor` x2, `billboardSetMaterial`, `billboardClone`,
`lightSceneNodeCopyConstruct`, `lightSceneNodeConstruct`, `CLightSceneNode_ctor_C1/C2`,
`lightSceneNodeClone`) for rows that are ALREADY live in `src/scene/`. They return null or do nothing,
so a row screened by "is there a body?" looks unfinished when it is not.
`tools/trackb/verify_rows.py` screens a ledger for exactly this: rows whose address appears ONLY under
`src/recon/`, or whose body is empty or returns null. It found 17 false-positive `reconstructed` rows.

### `CMatchingGLLive`'s 360-byte member record - structure recovered, ready to finish

`net-session-online`'s remaining bulk (147 partial rows) is the four GLLive blocks, and the worker on it
named the 360-byte record as the highest-leverage next step. Recovered from
`CMatchingGLLive::ReadPacketData` (0x81e4d0):

- The class holds **three different member-record arrays**, not one:
  * `this + 0x18` - `NetStructArray<CMatching::MemberInfoNetStruct, 32>`, the BASE's array (stride 432).
  * `this + 0x3A00` (14848) - `CMatchingGLLive::MemberInfoNetStruct[32]`, **stride 360 (0x168)**. This is
    the one still modelled as a stand-in.
  * (plus, on the Local side, `CMatchingLocal::MemberInfoNetStruct[32]` at +0x4BE0 stride 408 and
    `ServerBackupNetStruct[32]` at +0x7EE8 stride 344.)
- `ReadPacketData` walks the GLLive array as
  `v12 = *(int(**)(char*, _BOOL4, NetBitStream*, int, int))(*(_DWORD*)v11 + 24)` then
  `v12((char*)this + 360*v10 + 14848, isNotSelf, a4, a2, 0)` - so **the record's vtable slot +0x18 is its
  load/unserialize entry**, and it returns non-null when the record belongs to this member, which triggers
  `CMatchingGLLive::SyncMemberId` (0x81e25c). The loop is a do/while over all 32 elements, and `isNotSelf`
  is `CMatching::GetMemberIndex(this, *((_DWORD*)this + 3472)) != v10`.
- Before the loop it calls `CRoomAttributes::Unserialize` on whatever the class's own vtable slot +164
  returns. The class vtable is `_ZTV15CMatchingGLLive` at 0x992828.

To finish the record: dump `_ZTVN15CMatchingGLLive19MemberInfoNetStructE`, find its constructor, and
recover the `DeclareMember` order exactly as was done for the other two records. The earlier worker's
warning applies: this record holds a `NetStructString<16>` where the base's holds a `NetStructUInt<8>`,
which is why it is 360 rather than 432.

## Two workers stopped mid-edit and left the tree unbuildable

Both are real incidents, recorded so they are not repeated and so the lost work is recoverable.

1. **`glitch-video/src/image.cpp`** (earlier round) — 1,155 lines added, 8 compile errors including raw
   Hex-Rays output pasted unedited (`__builtin_sdiv`) and members that do not exist (`CImage::left`/`top`).
   Its worker was still running; it converged by itself after ~10 minutes and the file now compiles and is
   committed. 22 ledger rows.

2. **`net-session-online/src/multiplayer_callback.cpp`** (2026-10-15) — 276 insertions / 22 deletions, then
   the worker **finished** and stopped with the file broken and **no ledger row written**. Errors:
   `'RenderFX' is not a base of 'MenuBase'`, `no matching function for call to 'CMatching::IsInRoom()'`, and
   `'class MenuBase' has no member named 'InvokeASCallback'`. All three are calls written against an API that
   does not exist in the tree, i.e. exactly what rule 5 forbids. The integrator stashed that work to restore
   the build:

   `git stash list` -> `stash@{0}: WIP on track-b/development: 3f6ce686`

   **A worker redoing this must `git stash show -p stash@{0}` first and treat it as a draft to be checked
   against the real headers, not as code to reapply.** The specific traps: `MenuBase` does not derive from
   `RenderFX` (the original reaches an `RenderFX` object some other way); `CMatching::IsInRoom()` has no
   zero-argument overload in the port; and `RenderFX::InvokeASCallback` has two overloads, one taking
   `gameswf::character*` first — a previous worker guessed the other one and was rejected.

**Lesson for every worker**: a file you touch must compile when you stop. If a body cannot be finished
cleanly, leave that row `partial` with the blocker named and keep the file building. Run
`./tools/trackb/check_tu.sh <file>` as the last action before reporting.

## THE `CMatching` VTABLE REORDER - one decision, four consequences, now fully evidenced

This has been deferred repeatedly because it is the only remaining item that changes vtable layout across
a class hierarchy. The evidence is now complete, so it should be settled once by an owner rather than
worked around per row. A `-Woverloaded-virtual` audit (report:
`default-workspace/dh2-warn-audit/WARN-AUDIT-overloaded-virtual.md`) established all of it from the
recovered binary. **Do not fix these piecemeal** - the two `IsInRoom` overloads and the base declaration
must move together.

1. **`GetRoomAttributes` and `GenerateNewMemberId` are 8 bytes out in the port.** The port puts
   `GetRoomAttributes` at `+0xa4` and `GenerateNewMemberId` at `+0xac`; the binary has them at `+0xac`
   and `+0xb4`. `CMatchingGLLive::ReadPacketData` (0x81e4d0) and `WritePacketData` (0x81f53c) both
   dispatch through the affected range, so **their room half currently calls the wrong function**.
   This is what keeps those two rows `partial`.
2. **`CMatchingGLLive::GenerateNewMemberId() const` is a genuine mis-override.** `_ZTV15CMatchingGLLive`
   dyn `+0x0ac` holds the NON-const `CMatchingGLLive::GenerateNewMemberId()`, the same slot
   `_ZTV14CMatchingLocal` gives its own; the symbol is `_ZN15CMatchingGLLive19GenerateNewMemberIdEv`
   (0x81bbf4, no `K`). The port's `const` therefore breaks the override, emits a symbol the original does
   not have (rule-10 violation), and adds a slot - so a virtual call on a `CMatchingGLLive` through
   `CMatching*`/`CMatchingLocal*` silently runs `CMatchingLocal`'s body (0x80675c), never 0x81bbf4.
3. **`CMatchingLocal::IsInRoom() const` is a genuine mis-override.** `_ZTV9CMatching` (0x991f40) has
   **both** `+0x64 IsInRoom()` and `+0x68 IsInRoom(int)` as PURE slots, and `CMatchingLocal` implements
   both (0x805cc8 / 0x805c1c, neither const). The port's base declares only `IsInRoom(int)` - its `+0x64`
   stand-in is the differently-named `UnknownGate()` - so the derived cannot override.
4. **A dispatch bug the compiler warning CANNOT see at any level** (`-Woverloaded-virtual` verbosity 0, 1
   and 2 all miss it): `CMatchingLocal::IsInRoom(std::int32_t) const` has the base's parameter list but is
   `const`, so it is not even virtual and does not override. **Every virtual `IsInRoom(idx)` call runs the
   base's `return false` stub instead of 0x805c1c.** `CMatchingGLLive::IsInRoom(int)` (correctly non-const)
   then cannot override either, so GLLive carries an extra slot.
   Five more `CMatchingLocal` members carry a `const` the binary lacks - `IsRoomClosed`, `IsRoomHidden`,
   `GetMemberMask`, `GetInvitationRoomId`, `GetMemberName`. Each one's own port comment already quotes the
   non-const mangled name, so they are provable rule-10 symbol mismatches, but not dispatch bugs.
   **Constraint:** `GetMemberMask` must STAY `const` while the base `MemberBit(int)` is `const`.

Two further layout-neutral items an owner should take at the same time, both proved by the same audit:
`CMatching::SearchRoom` (0x7feb40) needs a new virtual slot at `+0x08` (its listing tail-calls the search
object's `+0x08`, which the port does not declare) - this is the single real blocker on three
net-session rows (`0x49b580`, `0x49b660`, and `joinRoom`'s `+0x90 GetRoomList`); and
`CUserPointerAttribute::getString(char*)` **is** virtual in the binary (a new slot `+0x10c`; its `_ZTV` is
280 B/68 slots against `IAttribute`'s 276 B/67), while the port declares it non-virtual, so the port's
vtable is one slot short.

### The rest

1. **`CBatchSceneNode`'s visible-segment list** (`glitch-scene-collada-gui`) - CORRECTED and partly FIXED; a narrower
   decision is still open. The list is NOT in the cache block at +0x154 (the previous round assumed it was):
   computeMaxIndices 0x57e5b8 does `LDR R2,[R0,#0x158]` and 0x57e5cc `ADD R3, R2, R1,LSL#2` (R1 = record word +0x10,
   firstIndex); updateIndices 0x57f740/0x57f75c forms the same address. `firstIndex` is therefore a WORD offset into the
   +0x158 visible-ENTRY block, which holds the 20-byte records and, after them, each batch's array of
   `CBatchMesh::SSegment*`. +0x154 is only the index-data cache: 12-byte slots {count, capacity, base}, word 1 = the
   batch's OWN mesh-buffer index count (0x57ecc8: `SBatch +0x00` -> `CMeshBuffer +0x20`), word 2 = the running offset
   into the index area.
   Fixed from the listing: `visibleSegment()` read the +0x154 block (now +0x158); `updateIndices` cleared a cache slot
   instead of the record's flags word at +0x0C (0x57f800); `invalidateVisibleIndexCache` sized and offset the cache by
   segment-info counts instead of the batches' index counts (0x57ec48); `postCompile` never wrote +0x144 although the
   listing stores the same value into +0x144 and +0x15C (0x57f000 / 0x57f06c). `scene_batch_visible_test` passes again
   (option (i): the test stores its SSegment* values where the listing-backed reader looks, `4 * firstIndex` BYTES into
   the +0x158 block, and its cache-slot / `updateIndices` expectations were corrected to the listing).
   STILL OPEN (this is the decision): in the 32-bit original one SSegment* is exactly one word, so a list of N entries
   occupies N words and postCompile 0x57ef64 (and invalidate 0x57eca8 for the index base) advances the next offset by one
   word per entry. On this 64-bit host a pointer is 8 bytes, so the +0x158 allocation - 4 * (20 * batchCount +
   totalSegments) bytes, postCompile 0x57eecc - has 4 bytes per list entry where each entry needs 8: postCompile
   UNDER-ALLOCATES (once something writes the lists that is a heap overflow, not merely an overlap) and two batches whose
   lists are both non-empty overlap. Nothing writes those lists yet (`addVisibleSegments` is `skipped`, `onRegisterSceneNode`
   only forwards to the base), so no current test and no game path exercises it; the port keeps the original's word offsets
   with host-width elements (see the comments in `src/scene/CBatchSceneNode.cpp` and its header). The choice to make before
   `addVisibleSegments` / `onRegisterSceneNode` can be written:
   (a) double the offsets (firstIndex and the cache slot bases advance 2 words per entry and the allocation doubles),
   (b) store a one-word segment index instead of a pointer (keeps every listing offset literally true; costs a lookup), or
   (c) keep the pointers in a separate host-width array with the +0x158 words as indices into it. (b) is the only one that
   leaves the original's arithmetic untouched.
2. **`IDevice`'s virtual order** (`glitch-core-io/os/device.hpp`). The binary is +0x0c `yield()`,
   +0x10 `CAndroidOSDevice::sleep(unsigned int, bool)` (0x6a0434), +0x14 getVideoModeList, ...,
   +0x30 closeDevice. The port declares a different order and has neither `yield` nor `sleep`, so its
   +0x10 is not sleep and `Application::Update`'s pacing cannot be written. Reordering is an ABI change
   for every IDevice user - it needs a pass of its own, deliberately not done here.
3. **`IDevice +0x34` (the file system)**, `FileSystemWin32`, and `FileSystemBase` are THREE unrelated
   classes in the tree: `FileSystemWin32` (core-platform/filesystem_win32.hpp) and `FileSystemBase`
   (filesystem_base.hpp) are independent and neither derives from `glitch::io::IFileSystem`. The binary's
   +0x34 is the factory's `IFileSystem` (proved from 0x672040: `createFileSystem`'s sret into +0x34 with
   the refcount raised 0->1). A plain retype is NOT safe - three call sites bind `FileSystemWin32*`. A
   worker also warned that the POINTEE is unproven (the game's `IrrFactory::createFileSystem` 0x350aec
   allocates a 0x540-byte `FileSystemWin32`, so the object could be either concrete type), so a
   reinterpret_cast is a bridge with an unproven cast, not a correct model. `CTweakable::loadXML`
   (0x32b6b8) waits on this.
4. **STLport `_String_base` vs the port's `std::string`.** The allocator machinery IS now reconstructed
   (`core-platform/src/stlport_alloc.cpp`, `stlport_alloc.hpp`, with a passing `stlport_alloc_test`), but
   the game headers spell their string members `std::string`, i.e. `std::allocator`, while the recovered
   instantiations are `SAllocator`-based. Retyping those members is header-wide and would flip the
   GSFlashMenu / CTweakable / SIDedCollection / texture-manager rows. `_String_base::_M_swap` is
   deliberately NOT written: its listing is not a pointer exchange (it re-seats pointers and copies the
   inline buffer), and no row is blocked on it.
5. **`CMatching`'s vtable order** (`net-session-online/include/net-session-online/c_matching_base.hpp`).
   Documented in a `KNOWN DEFECT` block at the top of that file with the binary slot order from four
   vtables and the three proved consequences. Rows whose only gap is a slot correctly stay `partial`.
6. **`IMaterialParameters` rule-10 spelling.** The emitted instance is
   `IMaterialParametersTyped<Derived>`, not the original `IMaterialParameters<Owner, Header>`, and the R
   dispatchers mangle the enum parameter as `i`. Pre-existing for 85 reconstructed siblings. The 4 R Cvt
   dispatchers cannot close until the 12 skipped `CMaterialRenderer` matrix/intrusive rows exist.

## Known-failing tests and what each actually needs

- `glitch-scene-collada-gui/scene_batch_visible_test` - FIXED (design question 1: the list is in the +0x158 block, see
  the corrected item 1 under "The rest"; the test now passes and the remaining decision there is the writer side).
- `net-session-online/r1n-...-CMatching-9acceb-0/1_test` - assertions on
  `CMatching::GetMemberIdByIndex(32)` and `RoomNetEnabled`; same batch as the record types.
- `game-world/g1-game-world-CameraBase-b32e3c-0_test`, `CameraTarget-255099-0_test`,
  `g1-game-world-PFObject-34f48a-0_test` - segfaults in newly written camera and `PFObject::DBG_*`
  bodies; each needs the same treatment the `Level` rows got (re-read the body, then the test).
- `game-app/g1-game-app-DataReloaderManager-df2149-0_test` - three "block and the group are released"
  assertions; the released-buffer accounting is wrong, not missing.
- `core-platform/perf_counters_test` - unexplained; not investigated this round.
- `vox-audio/drivers_threads_source_mix_test` (0xc0000374, heap corruption) - in another worker's file.

## Test-stub hazard (classified, mostly NOT to be changed)

1,724 test-local definitions have a trivial body where `src/` also defines the symbol; 1,129 of those
shadow a real body of >=3 statements. The overwhelming majority are LEGITIMATE SEAMS - a test replacing an
engine dependency on purpose. The dangerous sub-class is specific: **24 singleton/instance accessors that
return null** (9 symbols, listed in the worker's report). Because tests link with
`--allow-multiple-definition` and the first definition wins, such a stub also disables the real singleton
for the whole executable. Two were fixed and verified this round:
- `game-app/tests/g1-game-app-GSFlashMenu-353474-0_test.cpp` - a null `COnline::GetInstance()` while
  `GSFlashMenu::Update` dereferences it unconditionally.
- `vendor-gameswf/tests/v1-vendor-gameswf-gameswf-fddef6-4_test.cpp` - aborting `tu_string::resize` /
  `stricmp` stubs that now run during a STATIC INITIALIZER, killing the process before `main`.
The remaining 22 are latent; each needs its own verification because swapping in the real accessor
reaches the Application singleton and changes what the test observes.

## Other findings worth keeping

- **A library-owned null singleton is a hazard that test stubs cannot fix.** `vox-audio/src/link1_VoxSoundManager_statics.cpp`
  defines `VoxSoundManager::s_instance = nullptr` and only `CreateInstance` (0x36ca88, a `skipped` row) ever
  sets it, while `Script_StopSound::Execute` (0x45fe2c) dereferences it with no null check, as the original
  does. So ANY test whose batch reaches a game body that touches `s_instance` segfaults regardless of what
  test-local stubs it defines. `g1-game-app-pack12-35588a-0_test` was failing exactly this way. Before
  diagnosing such a crash as a bad body, check whether the library's own static is null.
- **TinyXML IS vendored** at `tinyxml-and-c-libs/upstream/tinyxml-2.5.3/tinyxml.h`, on the build's include
  path (`TiXmlHandle::ToElement` 0x483a48 is there). Several ledger notes claim "TinyXML is not vendored"
  and hold `rnd::*` `LoadFromXml` rows back on it — that claim is FALSE and those rows (~7) are writable.
- **`rnd::sDirections` data, recovered**: table at 0x956AE4, stride 0x10, four records
  `{0,"north",0,-1}, {1,"east",1,0}, {2,"south",0,1}, {3,"west",-1,0}`, names at
  0x8d4dc0/0x8d4dc8/0x8d4dd0/0x8d4dd8. `rnd::Direction` is declared; `sDirections` is not defined.
  **`DIRECTION_OPPOSITES`** is referenced from code (`_ZN3rnd19DIRECTION_OPPOSITESE`, name string at
  0x121282) but its ADDRESS was never chased, so its four ints must come from the binary before anyone
  writes it — it blocks about six rows (`IsStraight`, `GetFlag`, `LinkToOtherBlocks`, `Rule::Impl` ctor,
  `ForceBlock::Impl::FilterExits`, `Tile::TrySpawn`). Do not guess those four values.
- `rnd::gDistributions` (6-byte rows) is likewise undeclared.
- `dh2/CameraBase.hpp` already declared `m_camera` (`ICameraSceneNode*`, +0x08) publicly; the accessor
  `GetCameraNode()` was the whole gap, and it is non-virtual so it changes no layout or vtable.
- The ledger's `CSceneManager` "K 0x28..0x34 registerSceneNodes" comments are shifted by 4 from the real
  table: **+0x3c is `registerSceneNodes`, not `drawAll`**. `dh2/SceneManager.hpp` labels its overrides with
  `decomp_vtable.py`'s address point, while the true dynamic offsets are those minus 0x0c, which is what
  `CSceneManager.hpp` and every call site use. Correcting both headers would save the next worker a detour.
- `BatchNodeCompiler::Compile` (0x50dc84) claims "RootSceneNode is not an ISceneNode here" - that is FALSE
  (`RootSceneNode : CRootSceneNode : CSceneNode : CEmptySceneNode : ISceneNode`), so its two loops and its
  GAP B can be written.
- `DBG_DrawGlitchNodeBoundingBox` (0x50e6a0) is a `skipped` core-platform row that was absent from BOTH
  `link_stubs.csv` and `link-missing.csv`, which broke 8 links until a stub row appeared. If it goes
  missing again, that is why. `DBG_GetDetailedNodeName` (0x51049c) is the same shape but is correctly
  absent from BOTH (nothing references it), so it needs no stub unless something starts calling it.
- `link_stubs.csv` still carries three rows nothing references any more: `g_consoleFileLister`,
  `_Z18DbgDrawOwnerDriverv` and `_Z25DbgDrawBoxViaDriverSlot28PvPKfj` (the Console uses the real
  `IDevice::fileSystem`; the box draw uses `IVideoDriver::draw3DBox`). Safe to drop on a stub regeneration.
- `core-platform/NOTES.md` lines 36/223/226-229 still describe removed DbgDraw seams and a
  `DeviceFeatureFlags` stand-in.
- **`_String_base::_M_swap`** (0x433880 char, 0x8a4c58 wchar_t) is deliberately NOT written. Its listing is
  NOT a pointer exchange: when both sides are on their inline buffer it swaps the 16-byte buffer and then
  re-seats each side's `end` and `data` relative to its own address (so `data == this` stays true), and the
  else-branch swaps the four words and the two pointer fields. The port has no `_String_base` type (its
  string members are `std::string`), and no row is blocked on it.
- **`clang`-visible hazard, three times this round**: a worker rewrote a header (making a base abstract or a
  member disappear) while its tests still assumed the old shape. `filesystem_base.hpp` /
  `_FileHandle::obfuscated_` and `cmatching_local_*.hpp` both did this. When a header changes, compile the
  TESTS, not just the sources.
- **A returned `intrusive_ptr` built from a local handle's `.get()` is a dangling pointer, not a reference transfer.**
  The port's `intrusive_ptr(T*)` takes the reference it is given and adds no use, and the glitch objects are born with
  count 0 (`CMesh::CMesh` clears +0x04, so `intrusive_ptr<CMesh> result(new CMesh(), true)` leaves count 1). So
  `return boost::intrusive_ptr<IMesh>(result.get());` lets `result`'s destructor delete the object and hands the caller
  freed memory; the listing's tail (`*a1 = v60; ++count; drop(v60)`, e.g. 0x59b7c8..0x59b814) is the intrusive_ptr
  UPCAST, i.e. `return result;`. Three rows had it - `createMeshCopy` 0x59b2a4, `createPlaneMesh` 0x6d72d0,
  `createCubeMesh` 0x6d8cb4 - and it segfaulted `scene_mesh_functions_test` at `copy->getMeshBufferCount()`. Those three
  are fixed; a `.get()` in a `return boost::intrusive_ptr<...>` is the shape to grep for.

