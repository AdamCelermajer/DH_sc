# Stub audit: bugs of the first-boot kind, found statically (owner Q, 2026-10-16)

The first Android boot (FIRST-BOOT.md) found eight bugs in rows the ledger calls `reconstructed`. They fall into
patterns. This audit swept `port/decomp-reconstruction/*/src` and `*/include` (not `upstream/`) for the same patterns.
Method: grep over the tree; `llvm-nm -S` of the unstripped arm64 `libdh2.so` against the binary's function sizes
(`queue.csv`); IDA read-only for the `.init_array` (539 static initialisers), `.data` initial values and call
graphs; and two instrumented Android builds in `build/own-q` (an overlay `intrusive_ptr.hpp` that made `T*`
construction explicit, then deprecated, so the compiler listed every raw-pointer construction).

Files modified in the last 15 minutes by owner R were not touched (CColladaFactory.cpp was edited after it had been quiet for 15 minutes).

## Totals per pattern

| # | pattern | found | fixed | left |
|---|---|---|---|---|
| 1 | bodies that are placeholders | 2 real, 3 seams (+ ~100 stale comments) | 0 | 5 documented |
| 2 | static data / init not matching the binary | 9 values + 1 registry + 1 (CSignIn provider) + init steps | 10 | 2 documented |
| 3 | allocation sizes, 32-bit strides on host tables | 2 allocations + 3 stride sites | 5 | 0 |
| 3b | raw 32-bit offsets into host objects (same class as 3) | 5 sites | 4 | 1 documented |
| 4 | `intrusive_ptr(raw)` without add-ref | 4 | 4 | 0 |
| 5 | registrations never called | 1 (message types) | 1 | 0 |

## 1. Placeholder bodies

Swept: `abort()`, `assert(false)`, `unreconstructed`, `stub`, `TODO`, `not reconstructed`, `port seam`, `placeholder`,
bodies much smaller than the binary's (host symbol size from the arm64 `.so` against `queue.csv` `body_bytes`, 7,797
reconstructed rows with 100 bytes or more; 436 under 30%).

- 204 `std::abort()` sites: all but two are the original's deliberate crash (`ASSERT` at level 2 stores to address 0,
  `_assert2`); every one has the explanation beside it. The two others: `glitch-video/src/batch_driver.cpp:379`
  (the original also writes through a null pointer there, NOTES (1) in the file) and `unreconstructed.cpp`
  (see below). Kept.
- `glitch::video::detail::unreconstructed()` (glitch-video) has no caller left (the include in `video_driver.cpp` is
  unused). Nothing to fix.
- Size outliers (host body under 30% of the binary's): the 436 are tables folded into loops (`Structs::X::read`,
  `Arrays::X::readNames/finalize`), thunks to a shared body (`~X` D1 to D2, `CXMLReaderImpl` wrappers), delegating
  helpers (`AISDefault`, `CharAISkillScript::_Projectile*`), and `NativeGetPlayerStats`, which is table-driven. The
  ones read in full (about 25: `NativeGetPlayerStats`, `v2EmuController::_onEvent`, `Script_*::Execute`,
  `Quest::GetScriptIDFromID`, `AISDefault::*`, `SkinnedMeshSceneNode::prepareSkinForRendering`, ...) are complete.
  No empty body was found for a function whose Hex-Rays body does work.
- About 135 rows say `PARTIAL` in the comment above the function but are `reconstructed` in the ledger. The ones
  read (`FS_PlayMusic`, `FS_LoadLevel2`, `VisualObject::GetPosition`, `Application::IsOnlineGame`,
  `GetCurrentLevel`, `AnimatedFX::HasCompleted`, `VoxEngine::UpdateSourcesThreaded`, `Savegame` file seams) have
  been completed since the comment was written, or differ only in a leftover return register. The comments are
  stale, not the code. Not edited (comments only).
- Seams left, with reason:
  - `glitch-scene-collada-gui/src/recon/slice_18.cpp` and `slice_33.cpp`: "neutral value" stand-ins in namespace
    `recon_*` (not original symbols). Superseded by the real rows in `collada/` and `ps/`; not in `libdh2.so`.
  - `glitch::registerInitStep` steps are never registered: `processBufferHeapInit(true)` (256 KB pool at startup),
    the shared-string-heap init, and `video::(anon)::init` (type-string bank). `glitch::init()` runs an empty list.
    Effect: process buffers come from the allocator instead of the pool, string heap is created at first use. No
    functional difference found, and registering the pool step would change allocator behaviour under owner R's
    boot, so it is left. A one-line change in `slice_24.cpp` (`registerInitStep(core::processBufferHeapInit)`) when
    wanted.
  - `AnimatedFX::SetEndPoint` (game-anim): reads `CParticleSystemSceneNode::unk_178` (the emitter holder at node
    +0x178, an unrecovered 32-bit structure) with raw offsets `holder + 4`, `sub + 8`, `sub + 0x2c`. `unk_178` is
    always null in the port, so the function returns early (no memory error, but the end-point distance of
    projectile FX is never applied). Needs the emitter-holder layout first.
  - `CSignIn::Provider()` starts at 0 (dummy sign-in); the binary's `s_SignInProvider` starts at 1 (Gameloft Live).
    Left at 0: provider 1 constructs `CSignInGLLive`, which needs the online stack; offline boot is not served by it.

## 2. Static data and initialisers

The `.data` section is 24 KB. Every named object with a non-zero initial value was compared with the tree
(`datanames` list from IDA, about 35 objects). The 9 below did not match; the others did.

Fixed (value from the binary):

| symbol | binary | tree before | file |
|---|---|---|---|
| `glitch::os::Timer::VirtualTimerSpeed` | 1.0f (0x99b804) | 0.0f ("placeholder: not recovered"): `Timer::getTime()` returned only the base time, so every virtual clock stood still | glitch-core-io/src/logging.cpp |
| `glitch::os::Randomizer::Seed` | 0x0f0f0f0f (0x99b800) | 0 | glitch-core-io/src/randomizer.cpp |
| `vox::DriverCallbackSourceInterface::s_distanceModel_` | 2 | 0 | vox-audio/src/drivers-threads/driver_callback_source_setup.cpp |
| `...::s_dopplerFactor_` | 1.0f | 0 | same |
| `...::s_alteredSpeedOfSound_` | 343.3f (0x43aba666) | 0 | same |
| `...::s_driverCallbackPeriod_` | 0x7fffffff | 0 | same |
| `vox::DriverAndroid::s_androidAPILevel` | 3 | 0 | vox-audio/src/driver_android.cpp |
| `CMatching::s_matchingProvider` | 1 | 0 | net-session-online/include/.../c_matching_base.hpp (test updated) |
| `Level::Update()::s_autoLevelLoadWaitTime` | 5000 | uninitialised | game-world/src/g1-game-world-Level-7c7f5d-3.cpp |

Static initialisers (`.init_array`, 539 entries; 233 larger than 100 bytes): 265 do more than set `vHalf = 0.5f`.
They are singleton constructions (`Singleton<T>::s_inst`, present as dedicated TUs `singleton_*.cpp`),
`GS*::s_inst`, list/deque sentinels (`Savegame::s_jobList`, `_s_pool`, `g_SubtitlesQueue`), `Vec3f_*`, gameswf
`tu_string` statics (upstream sources), and:

- **CMessage type registry** (static initialiser of Core/Application.cpp, 0x32db64): `InitializeMessageRegistration()`,
  13 x `RegisterMessageType("CMsg...", GetNewInstance<CMsg...>)`, `TerminateMessageRegistration()`. The tree had all
  pieces and called none of them: every message type id stayed -1 and the creator array stayed empty (network
  messages could never be created or unserialised). Added `game-app/src/static_init_message_registration.cpp`
  (static initialiser in the original order; libraries link whole-archive so it runs at load).
- `glitch::registerInitStep` x3: see seams above.

Left: `vHalf` is a constant in the tree (`kHalf`), not a variable (fine). `CSignIn` provider: see seams.
`Structs::*::m_dataOffsets` (24 KB of offset tables in `.data`) are not used by the host code (typed members instead).

## 3. Allocation sizes and 32-bit strides

All `CustomAlloc / GlitchAlloc / VoxAlloc / malloc / operator new[]` calls with a literal or arithmetic size were
listed (about 220). Fixed:

- `Application::OnInterruptResume`: `CustomAlloc(8)` for `GS_InterruptLoading` (two vptrs, 8 bytes on the 32-bit
  build, 16+ on the host): heap overrun. Now `sizeof`. (game-app/src/g1-game-app-Application-b291be-2.cpp)
- `GSTest` debug key: `CustomAlloc(4)` for `TestUD`, `TestUD2` (vptr): same. Now `sizeof`. (g1-game-app-GSTest-c7a9f5-0.cpp)

The pydata element tables (`pydata/element_table.hpp`) allocate `sizeof(Elem)` and keep the 32-bit stride in a header
word; but three readers still indexed the host table with the original byte stride (`members + 24 * index`,
`members + 72 * i`), reading the wrong row from index 1 on (and past the end for large indices). Fixed to typed
access:

- `DamagerRowWord` (Arrays::GameObjectDamager, 24-byte rows): used by every attack-resolution path of
  `Character` (damage type, immunity time, range). (game-character/src/g1-game-character-Character-ee9946-8.cpp)
- `DoorTableWord` (Arrays::Doors, 24-byte rows): door sounds and visual. (game-world/src/g1-game-world-Door-478d47-0.cpp)
- `MenuBase::FS_LoadLevel2` (Arrays::LevelList, 72-byte rows). (game-ui/src/g1-game-ui-MenuBase-8948cc-0.cpp)

### 3b. Raw 32-bit offsets into host objects

Same class: code written as `*(base + 0x..)` against the original layout, executed on a 64-bit host object.
Found by grepping byte-pointer arithmetic with hex offsets. Fixed to the typed members:

- `PlayerLightTweaker::onSetValue("m_fogDirectionMask")` (game-app/src/g1-game-app-pack3-d077cc-0.cpp) wrote three
  words at `+0x458..0x460` of the driver's `CGlobalMaterialParameterManager`, a 0x3c-byte object (heap overrun).
  The asm (0x40e25c) stores through `device + 0x1c`, the game `SceneManager`, whose `unk_458/45c/460` are what
  `Level::Update` reads. Now writes those.
- `RenderFX::GetText` (`edit_text_character` +0x138/+0x139/+0x144) now reads `m_text.c_str()`; `GetAlpha`
  (`character + 0x48` then `+0x18`) now reads `m_local_cxform->m_[3][0]`; `SetTextBufferingEnabled` (`root + 0x85`)
  now writes `m_shift_key_state`. (game-ui/src/n2-game-ui-RenderFX-849cdc-0.cpp). `GetText` backs every UI text
  field read.
- `MenuBase::SetVisible` and the options-menu character used `((uint8_t*)character)[0x9b]`; now `m_visible`.
  (game-ui/src/g1-game-ui-MenuBase-8948cc-3.cpp, -4.cpp)

Left: `AnimatedFX::SetEndPoint` (see seams in 1).

## 4. intrusive_ptr

`T*` construction was made explicit, then deprecated, in an overlay header (not committed) and every library
rebuilt for arm64: 2 copy-initialisations from raw pointers (both adopt a returned reference: correct) and 31 explicit
raw constructions. Triage: 27 adopt a reference the callee returns or a `new` object born with count 1 (correct);
4 wrap an object that already has an owner and do not add a use, so the temporary's destructor steals the owner's
reference:

- `CCoronasSceneNode.cpp:244`: `intrusive_ptr<const CVertexStreams> streams(vertexStreams)` in the render function
  releases the node's own reference every frame (use after free from the second frame). Fixed (`, true`).
- `AnimatorSet.cpp:179`: `intrusive_ptr<CAnimationSet>(animationSet->m_animSet)` in the base initialiser; the comment
  says "a use held during the call". Fixed (`, true`).
- `ok2-IVideoDriver.cpp:604`: `intrusive_ptr<const CVertexStreams>(unk_140.get())` for the driver's 2D vertex
  attribute map. Fixed (`, true`).
- `CColladaFactory.cpp:151`: `map(instance->vertexAttributeMap)`. The instance keeps the reference with an explicit
  add-ref at line 171, so the local destroys it: the map the instance points to is freed after the first material
  built from it. Fixed (`, true`) once the file had been quiet for 15 minutes.

`operator=(T*)` (adds a use) and `reset(T*)` (adopts) were listed the same way: 4 sites, all consistent with their
comments (`AssetManager` use counting, `ISceneNodeAnimator::setTimelineCtrl`, gameswf bitmap_info). The explicit
`->grab()/drop()` sites (about 60) were not re-derived against the binary one by one: they are transcribed from
the listings and their pairs are commented.

## 5. Registrations and tables never called

- CMessage registry: see 2 (fixed).
- Call-graph check: the 1,363 reconstructed functions whose name appears at most twice in the tree (definition and
  declaration) were compared with the binary's code xrefs (IDA): 99 have callers in the binary; all 99 are
  renamed callers, inlined helpers, or constructors reached through base-class initialisers (verified for
  `PriorityBankManager::_CanAddEmitter`, `EmitterObj::_IsDone`, `NetBitStream::ShiftMemCopy` and the Convert*LC pair).
  The only registration functions with no callers in the tree (`SceneManager::LoadFXLib`, `MenuBase::RegisterSlideEvent`,
  `MenuManager::RegisterDummyMenu`, `MenuFX::RegisterStates`, `VisualObject::CreateShadowMesh/CreateXrayMesh`,
  `Character::InitCoopCam`) also have no code caller in the binary (data xrefs only: dead code / dynsym).
- `.data.rel.ro` tables (`objectCreationMap`, `s_LoadingStepsNames`, `s_SWFfileToLoad`, `s_supportedUncompiledData`,
  `rnd::Direction::sDirections`, `s_conditionImplDataMap`, `s_RewardImplDataMap`): all present.

## Files changed by this audit

game-app: `g1-game-app-Application-b291be-2.cpp`, `g1-game-app-GSTest-c7a9f5-0.cpp`, `g1-game-app-pack3-d077cc-0.cpp`,
`static_init_message_registration.cpp` (new); game-character: `g1-game-character-Character-ee9946-8.cpp`;
game-world: `g1-game-world-Door-478d47-0.cpp`, `g1-game-world-Level-7c7f5d-3.cpp`; game-ui:
`g1-game-ui-MenuBase-8948cc-0.cpp`, `-3.cpp`, `-4.cpp`, `n2-game-ui-RenderFX-849cdc-0.cpp`; game-anim:
`g1-game-anim-AnimatorSet-8cd819-0.cpp`; glitch-core-io: `logging.cpp`, `randomizer.cpp`; glitch-video:
`ok2-IVideoDriver.cpp`; glitch-scene-collada-gui: `collada/CCoronasSceneNode.cpp`, `collada/CColladaFactory.cpp`; vox-audio:
`drivers-threads/driver_callback_source_setup.cpp`, `driver_android.cpp`; net-session-online:
`include/net-session-online/c_matching_base.hpp`, `tests/r1n-net-session-online-CMatching-9acceb-1_test.cpp`.
No ledger row changed status (all fixes are inside rows that stay `reconstructed`).

## Second round (seams closed, .data / .init_array comparison)

- **Init steps.** The binary registers three steps from static initialisers: ProcessBufferHeap.cpp (0x534564) registers
  `processBufferHeapInit`; EShaderParameterType.cpp (0x5e194c) calls `registerSharedStringHeapInit` (0x6a54d0, once-guard,
  registers `initSharedStringHeap`) then registers the type-string bank `init` once; SSharedStringHeapEntry.cpp (0x6a550c) calls
  `registerSharedStringHeapInit` again (no-op). Now done the same way (static initialisers in `process_buffer_heap.cpp`,
  `shared_string_heap.cpp` (new `registerSharedStringHeapInit`), `shader_parameter_type_bank.cpp`). `initSharedStringHeap(true)`
  is idempotent in the port because the table is also created on first use. `glitch::init()` now gives the process buffer heap its 0x40000 bytes.
- **AnimatedFX::SetEndPoint / HasCompleted.** The node tag is 0x70656164 ('daep', collada::CParticleSystemSceneNode); the port used
  0x70656E64 (no node has it) and, in HasCompleted, cast to the wrong class (scene::IParticleSystemSceneNode), so every FX reported
  "completed" at once. Both fixed. SetEndPoint: node +0x178 is the particle system, +0x60 its PEmitterModel (+8 type, +4 PDBox
  domain, domain +0x2c = `sizes.X`), as in the listing.
- **Sign-in provider.** `CSignIn::s_SignInProvider` is .data = 1 and nothing writes it, so the Android build always uses
  CSignInGLLive; the port now starts at 1. Single player never calls `CSignIn::Get` (GoToMainMenu gates it on the online flag); online
  the GLX login sends through the existing GLXPlayerLogin/socket path, which fails as the original does when the connection fails.
- **.data comparison, scripted** (symbol table + section bytes, 196 non-zero named objects, plus the 539 `.init_array` bodies):
  new proven mismatches fixed: `IShadowVolumeSceneNode::Technique{ZPass,ZPassSecond,ZFail,ZFailSecond,Draw}` 0 -> 0xff;
  `GLXPlayerPing::SendEchoRequest::nSeq` 0 -> 1; the `CEventQueue<...,100>` globals (`CMatching::m_eventQueue`,
  `m_eventQueueInternal`, `CMatchingGLLive::s_eventQueueGLLiveInternal`) expiry 0 -> 100.
  Checked, no change: string tables (all but 9 tables found in order in the tree; the 9 are unreferenced exports or upstream
  libs), exports with no code reference (`Device::m_model`, `gUsePrintf`, `mIsDemo`, `Objective_EventReceiver::m_curModuleId`,
  `SupervisedEventName`, the cube-map-face / buffer-type / vertex-value-type name tables), upstream-library data (tinyxml, mpc,
  gameswf, box2d), window-dimension tables, `PlaceHolderColor`, `UNINIT_FOG_COLOR`, Vec3f constants. Not changed because the
  value is written before it is read (unobservable): the billboard baker statics (`Color`, `Position`, `TexCoords`, `Normal`).
  `glitch::memory::Matrix4Pool/Aabbox3dfPool` are not used by the tree.

## raw offsets, round 2 (owner Q2)

Sweep of game*, core-platform, pydata-accessors, net*, vox-audio, glitch* (not vendor/upstream) for code that reaches a record or object through
the original 32-bit byte offsets: every `reinterpret_cast`/`static_cast<T*>(void*)` (about 280 in game/core/net/vox, 405 in glitch), char*/uint8_t*
arithmetic with literal offsets, memcpy/memset with literal sizes, `sizeof` assumptions, `void***` vtable reads, offset helper functions, and a script
that checks every `Arrays::X::members` cast against the element type `ReadArrayTable<Elem>` builds (129 sites, all typed correctly; `void*`
intermediates included). Bytes that are file or network data are left (decision 2).

Fixed (5):
- `MenuWorldMap::Init` (game-ui `g1-game-ui-MenuWorldMap-7eeef2-0.cpp`): read `Arrays::WorldMapLockers` as bytes at +4/+8/+16/+20/+28/+32 of a
  12-byte stride. The rows are host `Structs::WorldMapLocker` objects (vptr, OnState, QuestID; 16 bytes on arm64), so it read the vptr half and
  neighbouring rows. Now `WorldMapLockerRow(n).OnState / .QuestID`.
- `CallMatchingSlot44` (net-session-online `multiplayer_callback.cpp`): read the function pointer at vptr + 0x44 (slot 17 of a 32-bit vtable; on arm64
  that is the middle of slot 8, so a wild call when the player count drops to 3). Now the typed virtual `CMatching::OpenRoom(bool)`, which is what the
  binary's vtables hold at +0x44 (c_matching_base.hpp).
- `DecoderMPC8Cursor` constructor (vox-audio `decoder_mpc8_cursor_io.cpp`): the "open" test read the first two 32-bit words of `mpc_demux_t`, which is two
  pointers (`r`, `d`); on arm64 that tested the low and high halves of one pointer. Now reads two pointers. The `mpc_streaminfo` receive block was
  `uint32_t[93]` (372 bytes) but the vendored struct is 376 bytes on arm64 (pointer-width `profile_name`, 8-byte offsets): enlarged to 128 words.
- `MpcFrameInfo` (vox-audio `deps/mpc_demux.hpp`): lacked `is_key_frame`; `mpc_demux_decode` writes it (+0x10 on arm64), so every Decode overran the
  16-byte stack struct. Added the member (24 bytes, as libmpcdec's `mpc_frame_info`). Test fake `mpc_demux` now holds two pointers.
- `Billboard::DeclareProperties` ("position", "size"; game-world `g1-game-world-Billboard-1e5d36-0.cpp`): passed `(char*)this - 4` as the property object, a
  32-bit offset guess that is wrong on any layout (the other properties use the PropertyMap subobject). Now the PropertyMap pointer, so m_offset
  is measured the same way as `AddProperty<T>` and `ValueAt` finds the member.

Left, with the reason:
- `glitch-scene-collada-gui/src/recon/slice_18.cpp`, `slice_20.cpp` (`void*` object views with `+0x10`, `+0x8c`, `+0x90`, vtable `[2]` of a `void***`):
  free-function stand-ins in namespaces `recon_18`/`recon_20`; nothing outside the slices and their tests calls them (grep), the real classes are in
  `ps/` and `collada/`. They would break on a real object but are unreachable.
- `Arrays::*::members` casts (129), `TableRow<Row>`, `static_cast<const std::int32_t*>(Struct.List / Interact / Spells / RandomCam / DisplayProps /
  CamAnims / SubLoots / list_entries)`: tables are host element arrays (element_table.hpp) and the int arrays are real `int32` blocks (`ReadWords`,
  4 * count), so the typed access is right. Element-pointer fields (`Steps`, `Conds`, `PreReqs`, `Rewards*`, `MerchandiseList`, `Spawns`, `CharInfo`,
  `factions`, `AttrBonusList`, `RandomLootEntries`...) are all cast to the element type their reader allocates.
- `SimpleTypeProperty::ValueAt` (`object + m_offset`): m_offset is measured with host pointers when the property is made.
- `CBatchSceneNode` visible-entry block (`block + 4 * firstIndex` holding host `SSegment*`), `CBatchMesh::segmentAt`, `SPassBinding` tables, `texture.m_levelSizes`
  bitmap, `quantize`/`vertex_transform` strided access: sized and padded for host width in their own headers (comments there); strides come from
  the stream descriptors, not from literals.
- `RandomGroup::sentinel()` (vox-audio groups.hpp): the sentinel is the two link words; the element word is never read.
- `DataReloaderManager::_MonitoredFiles`, `PolyStat`, `ProjectileManager` destructors: pointer differences on `uintptr_t`, host width.
- `GetDedicatedServer` (`memset(block, 0, 0x30)` of an opaque `_tDedicatedServerInfo`): the record has no layout in the tree and nothing reads it.
- `CMsgDropLoot`/`MultiplayerCallback` payload offsets (+0x04..+0x18), `tMsgRaisedEventData`, `StartGamePayload`, `CNetworkId` address bytes, ping and
  handshake packets: network packets, fixed layout (static_assert'ed).
- `decoder_native_parse` record walks, `ok2-res`/`CResFileDecode` (`v.raw(v.u32(a + 0x14))`), `CAnimationStreaming` words, `config_file` image: file images.
- `CDriverBinding::getProcessBuffer` (`mask` is a `uint32_t` compared with a stream pointer): the value only round-trips through the
  member it is compared against, so the truncation is consistent; no caller outside tests.
- `Application::Init` `reinterpret_cast<IEventReceiver*>(this)` and the `device->sceneManager` casts: same vtable shape / primary base, round trips.
- `ItemObject::QuestListHoldsItem` (pointer truncated to 32 bits and compared with item ids), `PlayerSavegame::IsSentinelUser`: deliberate, as the listing does it.

Files changed: game-ui `g1-game-ui-MenuWorldMap-7eeef2-0.cpp`; game-world `g1-game-world-Billboard-1e5d36-0.cpp`; net-session-online `multiplayer_callback.cpp`;
vox-audio `decoder-mswav-mpc8/decoder_mpc8_cursor_io.cpp`, `include/vox-audio/deps/mpc_demux.hpp`, `tests/decoder_mpc8_cursor_demux_test.cpp`.
