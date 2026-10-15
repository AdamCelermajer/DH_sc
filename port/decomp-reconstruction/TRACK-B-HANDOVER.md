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

1. **`CBatchSceneNode`'s visible-segment cache block** (`glitch-scene-collada-gui`). The visible-entry
   records (20 bytes, `firstIndex` at +0x10) hold a WORD offset into the cache block at +0x154, and each
   cache word is an `SSegment*`. In the 32-bit original a pointer is exactly one word; on this 64-bit host
   a pointer does not fit, and `firstIndex`/`base` are word offsets so the `4 *` arithmetic cannot simply
   become `8 *` without deciding what a "word" means. This is what segfaults
   `glitch-scene-collada-gui/scene_batch_visible_test` in `computeMaxIndices`. Deciding it fixes that test
   and unblocks the `registerSolidBatches`/`registerTransparentBatches` rows.
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

- `glitch-scene-collada-gui/scene_batch_visible_test` - design question 1.
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

